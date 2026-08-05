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

void lsp_init(__reg("a0") void *, __reg("a1") void *);
void lsp_stop(void);
void lsp_enable(__reg("d0") UBYTE);
void lsp_set_rate(__reg("d0") UWORD);
void lsp_vblank_interrupt(void);

static struct MsgPort *port;
static struct IOAudio *request;
static UBYTE channels = 0x0f;
static UBYTE *scoreData, *bankData;
static ULONG scoreSize, bankSize;
static BOOL ready, playing, installed;
static struct Interrupt interrupt;
static char interruptName[] = "MrDig Futsal LSP";

static void releaseData(void)
{
    if (scoreData) FreeMem(scoreData, scoreSize);
    if (bankData) FreeMem(bankData, bankSize);
    scoreData = bankData = NULL;
    scoreSize = bankSize = 0;
}

static UBYTE *load(const char *name, ULONG flags, ULONG *size)
{
    BPTR f;
    LONG length;
    UBYTE *p;
    *size = 0;
    f = Open((STRPTR)name, MODE_OLDFILE);
    if (!f) return NULL;
    Seek(f, 0, OFFSET_END);
    length = Seek(f, 0, OFFSET_CURRENT);
    Seek(f, 0, OFFSET_BEGINNING);
    if (length <= 0 || !(p = AllocMem((ULONG)length, flags))) {
        Close(f); return NULL;
    }
    if (Read(f, p, length) != length) {
        FreeMem(p, (ULONG)length); Close(f); return NULL;
    }
    Close(f); *size = (ULONG)length; return p;
}

static BOOL names(const char *source, char *score, char *bank)
{
    char *dot;
    if (strlen(source) > 235) return FALSE;
    strcpy(score, source); strcpy(bank, source);
    dot = strrchr(score, '.'); if (!dot) dot = score + strlen(score);
    strcpy(dot, ".lsmusic");
    dot = strrchr(bank, '.'); if (!dot) dot = bank + strlen(bank);
    strcpy(dot, ".lsbank");
    return TRUE;
}

BOOL musicInitialize(UWORD displayRate)
{
    if (ready) return TRUE;
    if (!(port = CreateMsgPort())) return FALSE;
    request = (struct IOAudio *)CreateIORequest(port, sizeof(*request));
    if (!request) { DeleteMsgPort(port); port = NULL; return FALSE; }
    request->ioa_Request.io_Message.mn_Node.ln_Pri = ADALLOC_MAXPREC;
    request->ioa_Request.io_Command = ADCMD_ALLOCATE;
    request->ioa_Request.io_Flags = ADIOF_NOWAIT;
    request->ioa_Data = &channels; request->ioa_Length = 1;
    if (OpenDevice(AUDIONAME, 0, (struct IORequest *)request, 0)) {
        DeleteIORequest(request); DeleteMsgPort(port);
        request = NULL; port = NULL; return FALSE;
    }
    memset(&interrupt, 0, sizeof(interrupt));
    interrupt.is_Node.ln_Type = NT_INTERRUPT;
    interrupt.is_Node.ln_Pri = -10;
    interrupt.is_Node.ln_Name = interruptName;
    interrupt.is_Code = lsp_vblank_interrupt;
    AddIntServer(INTB_VERTB, &interrupt);
    installed = TRUE;
    /*
     * Use the rate of the screen requested by the game, not
     * SysBase->VBlankFrequency. Workbench can change the active monitor
     * without updating that boot-time field, which made identical music
     * run at 50 Hz from floppy and 60 Hz from a Workbench hard disk.
     */
    lsp_set_rate(displayRate);
    ready = TRUE;
    return TRUE;
}

BOOL musicPlay(const char *filename)
{
    char sn[256], bn[256];
    musicStop();
    if (!ready || !names(filename, sn, bn)) return FALSE;
    scoreData = load(sn, MEMF_ANY, &scoreSize);
    bankData = load(bn, MEMF_CHIP, &bankSize);
    if (!scoreData || !bankData || scoreSize < 12 || bankSize < 4 ||
        memcmp(scoreData, "LSP1", 4) ||
        memcmp(scoreData + 4, bankData, 4)) {
        releaseData(); return FALSE;
    }
    Disable(); lsp_init(scoreData, bankData); playing = TRUE;
    lsp_enable(1); Enable();
    return TRUE;
}

void musicStop(void)
{
    Disable(); lsp_enable(0);
    if (playing) { lsp_stop(); playing = FALSE; }
    Enable(); releaseData();
}

void musicShutdown(void)
{
    musicStop();
    if (ready) {
        if (installed) RemIntServer(INTB_VERTB, &interrupt);
        CloseDevice((struct IORequest *)request);
        DeleteIORequest(request); DeleteMsgPort(port);
        request = NULL; port = NULL; ready = installed = FALSE;
    }
}

BOOL musicIsReady(void) { return ready; }
