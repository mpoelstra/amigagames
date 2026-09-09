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
#include "music.h"
#include <hardware/custom.h>
#include "platform_amiga.h"
#ifdef SPARKPAW_MULTI_ADF
#include "assets.h"
#endif

void lsp_init(__reg("a0") void *, __reg("a1") void *);
void lsp_stop(void);
void lsp_set_once(__reg("d0") UBYTE);
void lsp_frame(void);
UBYTE lsp_active(void);
void lsp_enable(__reg("d0") UBYTE);
void lsp_set_rate(__reg("d0") UWORD);
void lsp_vblank_interrupt(void);

static struct MsgPort *port;
static struct IOAudio *request;
static UBYTE channels = 0x0f;
static UBYTE *scoreData, *bankData;
static ULONG scoreSize, bankSize;
#ifdef SPARKPAW_THREE_ADF
static UBYTE *gameOverScoreCache,*gameOverBankCache;
static ULONG gameOverScoreSize,gameOverBankSize;
#endif
static BOOL ready, playing, installed, titleData;
static struct Interrupt interrupt;
static char interruptName[] = "Sparkpaw title LSP";

static void releaseData(void)
{
    if (scoreData) FreeMem(scoreData, scoreSize);
    if (bankData) FreeMem(bankData, bankSize);
    scoreData = bankData = NULL;
    scoreSize = bankSize = 0;
}

static UBYTE *load(const char *name, ULONG flags, ULONG *size)
{
#ifdef SPARKPAW_MULTI_ADF
#ifdef SPARKPAW_THREE_ADF
    if(gameOverScoreCache&&gameOverBankCache&&strstr(name,"storm-light.")) {
        BOOL bank=strstr(name,".lsbank")!=NULL;
        UBYTE *copy; ULONG bytes=bank?gameOverBankSize:gameOverScoreSize;
        *size=0; copy=AllocMem(bytes,flags);
        if(!copy) return NULL;
        CopyMem(bank?gameOverBankCache:gameOverScoreCache,copy,bytes);
        *size=bytes; return copy;
    }
#endif
    return assetsLoadDiskData(name,flags,size);
#else
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
#endif
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

static BOOL musicPlay(const char *score, const char *bank, BOOL once)
{
    if (!musicInitialize(50)) return FALSE;
    musicStop();
    scoreData = load(score, MEMF_FAST, &scoreSize);
    bankData = load(bank, MEMF_CHIP, &bankSize);
    if (!scoreData || !bankData || scoreSize < 12 || bankSize < 4 ||
        memcmp(scoreData, "LSP1", 4) ||
        memcmp(scoreData + 4, bankData, 4)) {
        releaseData(); return FALSE;
    }
    Disable(); lsp_init(scoreData, bankData); lsp_set_once((UBYTE)once);
    playing = TRUE; titleData=!once;
    lsp_enable(1); Enable();
    return TRUE;
}

void musicSuspend(void)
{
    Disable(); lsp_enable(0);
    if (playing) { lsp_stop(); playing = FALSE; }
    Enable();
}

void musicStop(void) { musicSuspend(); releaseData(); }
BOOL musicAudible(void) { return playing&&lsp_active(); }
/* Resident title restart only; safe with READY's nested Disable. */
BOOL musicRestartTitle(void)
{
    if(!titleData||!scoreData||!bankData) return FALSE;
    musicSuspend();
    Disable(); lsp_init(scoreData,bankData); lsp_set_once(0);
    playing=TRUE; lsp_enable(1); Enable();
    return TRUE;
}

void musicShutdown(void)
{
    musicStop();
#ifdef SPARKPAW_THREE_ADF
    if(gameOverScoreCache) FreeMem(gameOverScoreCache,gameOverScoreSize);
    if(gameOverBankCache) FreeMem(gameOverBankCache,gameOverBankSize);
    gameOverScoreCache=gameOverBankCache=NULL;
    gameOverScoreSize=gameOverBankSize=0;
#endif
    if (ready) {
        if (installed) RemIntServer(INTB_VERTB, &interrupt);
        CloseDevice((struct IORequest *)request);
        DeleteIORequest(request); DeleteMsgPort(port);
        request = NULL; port = NULL; ready = installed = FALSE;
    }
}

BOOL musicIsReady(void) { return ready; }
BOOL musicIsPlaying(void) { return playing; }

/* Called only with Exec interrupts disabled by the display owner. The normal
   VBlank server keeps playing while DOS loads/converts gameplay resources. */
void musicOwnedFrame(void)
{
    lsp_frame();
}

BOOL musicPlayTitle(void)
{
    return musicPlay("PROGDIR:assets/runtime/neon-sky.lsmusic",
                     "PROGDIR:assets/runtime/neon-sky.lsbank", FALSE);
}
BOOL musicPlayIntro(void)
{
#ifdef SPARKPAW_STORY_INTRO
    return musicPlay("PROGDIR:assets/runtime/hero-drive.lsmusic",
                     "PROGDIR:assets/runtime/hero-drive.lsbank", TRUE);
#else
    return FALSE;
#endif
}

BOOL musicPlayGameOver(void)
{
    BOOL ok=musicPlay("PROGDIR:assets/runtime/storm-light.lsmusic",
                      "PROGDIR:assets/runtime/storm-light.lsbank",FALSE);
    titleData=FALSE; /* This bank must never serve a resident Neon restart. */
    return ok;
}

#ifdef SPARKPAW_THREE_ADF
BOOL musicPreloadGameOver(void)
{
    if(gameOverScoreCache&&gameOverBankCache) return TRUE;
    gameOverScoreCache=assetsLoadDiskData("PROGDIR:assets/runtime/storm-light.lsmusic",MEMF_FAST,&gameOverScoreSize);
    gameOverBankCache=assetsLoadDiskData("PROGDIR:assets/runtime/storm-light.lsbank",MEMF_FAST,&gameOverBankSize);
    return gameOverScoreCache&&gameOverBankCache;
}
#endif
