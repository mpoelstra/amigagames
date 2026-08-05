/*
 * ChipSnake wrapper around Arnaud Carre's Light Speed Player.
 *
 * MOD files are converted on the host to an LSP score and sample bank.  The
 * score may live in any memory; Paula requires the sample bank in Chip RAM.
 * Playback is advanced by an Exec vertical-blank interrupt server, independent
 * of the rendering and game loops.
 */

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <exec/interrupts.h>
#include <devices/audio.h>
#include <hardware/intbits.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>

#include "modplayer.h"

void lsp_init(__reg("a0") void *score, __reg("a1") void *bank);
void lsp_stop(void);
void lsp_enable(__reg("d0") UBYTE enabled);
void lsp_set_rate(__reg("d0") UWORD frameRate);
void lsp_vblank_interrupt(void);

static struct MsgPort *audioPort;
static struct IOAudio *audioRequest;
static UBYTE audioChannels = 0x0f;
static UBYTE *scoreData;
static UBYTE *bankData;
static ULONG scoreSize;
static ULONG bankSize;
static BOOL playerReady;
static BOOL playing;
static BOOL interruptInstalled;
static struct Interrupt vblankInterrupt;
static char interruptName[] = "ChipSnake LSP";

static void releaseMusicData(void)
{
    if (scoreData != NULL) {
        FreeMem(scoreData, scoreSize);
        scoreData = NULL;
    }
    if (bankData != NULL) {
        FreeMem(bankData, bankSize);
        bankData = NULL;
    }
    scoreSize = 0;
    bankSize = 0;
}

static UBYTE *loadFile(const char *filename, ULONG memoryFlags,
                       ULONG *sizeOut)
{
    BPTR file;
    LONG size;
    LONG readSize;
    UBYTE *data;

    *sizeOut = 0;
    file = Open((STRPTR)filename, MODE_OLDFILE);
    if (file == 0) {
        return NULL;
    }
    Seek(file, 0, OFFSET_END);
    size = Seek(file, 0, OFFSET_CURRENT);
    Seek(file, 0, OFFSET_BEGINNING);
    if (size <= 0) {
        Close(file);
        return NULL;
    }
    data = AllocMem((ULONG)size, memoryFlags);
    if (data == NULL) {
        Close(file);
        return NULL;
    }
    readSize = Read(file, data, size);
    Close(file);
    if (readSize != size) {
        FreeMem(data, (ULONG)size);
        return NULL;
    }
    *sizeOut = (ULONG)size;
    return data;
}

static BOOL makeLspFilename(const char *source, const char *extension,
                            char *destination, ULONG capacity)
{
    ULONG length;
    char *dot;

    length = (ULONG)strlen(source);
    if (length + (ULONG)strlen(extension) + 1 >= capacity) {
        return FALSE;
    }
    strcpy(destination, source);
    dot = strrchr(destination, '.');
    if (dot == NULL) {
        dot = destination + length;
    }
    strcpy(dot, extension);
    return TRUE;
}

BOOL musicInitialize(UWORD displayRate)
{
    if (playerReady) {
        return TRUE;
    }

    audioPort = CreateMsgPort();
    if (audioPort == NULL) {
        return FALSE;
    }
    audioRequest = (struct IOAudio *)CreateIORequest(
        audioPort, sizeof(struct IOAudio));
    if (audioRequest == NULL) {
        DeleteMsgPort(audioPort);
        audioPort = NULL;
        return FALSE;
    }
    audioRequest->ioa_Request.io_Message.mn_Node.ln_Pri = ADALLOC_MAXPREC;
    audioRequest->ioa_Request.io_Command = ADCMD_ALLOCATE;
    audioRequest->ioa_Request.io_Flags = ADIOF_NOWAIT;
    audioRequest->ioa_Data = &audioChannels;
    audioRequest->ioa_Length = 1;
    if (OpenDevice(AUDIONAME, 0,
                   (struct IORequest *)audioRequest, 0) != 0) {
        DeleteIORequest(audioRequest);
        DeleteMsgPort(audioPort);
        audioRequest = NULL;
        audioPort = NULL;
        return FALSE;
    }
    memset(&vblankInterrupt, 0, sizeof(vblankInterrupt));
    vblankInterrupt.is_Node.ln_Type = NT_INTERRUPT;
    vblankInterrupt.is_Node.ln_Pri = -10;
    vblankInterrupt.is_Node.ln_Name = interruptName;
    vblankInterrupt.is_Code = lsp_vblank_interrupt;
    AddIntServer(INTB_VERTB, &vblankInterrupt);
    interruptInstalled = TRUE;
    /*
     * The game owns a PAL 50 Hz screen.  Use that display rate explicitly:
     * SysBase->VBlankFrequency can retain a Workbench/boot-time value and
     * caused the same LSP score to run differently from HD and ADF.
     */
    lsp_set_rate(displayRate);
    playerReady = TRUE;
    return TRUE;
}

BOOL musicIsReady(void)
{
    return playerReady;
}

void musicStop(void)
{
    Disable();
    lsp_enable(0);
    if (playing) {
        lsp_stop();
        playing = FALSE;
    }
    Enable();
    releaseMusicData();
}

void musicShutdown(void)
{
    musicStop();
    if (playerReady) {
        if (interruptInstalled) {
            RemIntServer(INTB_VERTB, &vblankInterrupt);
            interruptInstalled = FALSE;
        }
        CloseDevice((struct IORequest *)audioRequest);
        DeleteIORequest(audioRequest);
        DeleteMsgPort(audioPort);
        audioRequest = NULL;
        audioPort = NULL;
        playerReady = FALSE;
    }
}

void musicFrame(void)
{
    /* Playback runs from the Exec vertical-blank interrupt server. */
}

BOOL musicPlay(const char *filename)
{
    char scoreFilename[256];
    char bankFilename[256];

    musicStop();
    if (!playerReady ||
        !makeLspFilename(filename, ".lsmusic",
                         scoreFilename, sizeof(scoreFilename)) ||
        !makeLspFilename(filename, ".lsbank",
                         bankFilename, sizeof(bankFilename))) {
        return FALSE;
    }

    scoreData = loadFile(scoreFilename, MEMF_ANY, &scoreSize);
    bankData = loadFile(bankFilename, MEMF_CHIP, &bankSize);
    if (scoreData == NULL || bankData == NULL ||
        scoreSize < 12 || bankSize < 4 ||
        scoreData[0] != 'L' || scoreData[1] != 'S' ||
        scoreData[2] != 'P' || scoreData[3] != '1' ||
        scoreData[4] != bankData[0] ||
        scoreData[5] != bankData[1] ||
        scoreData[6] != bankData[2] ||
        scoreData[7] != bankData[3]) {
        releaseMusicData();
        return FALSE;
    }

    Disable();
    lsp_init(scoreData, bankData);
    playing = TRUE;
    lsp_enable(1);
    Enable();
    return TRUE;
}
