#include "title.h"

#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
#include <dos/dos.h>
#include <proto/dos.h>
#endif

#include "assets.h"
#include "audio.h"
#include "platform_amiga.h"

#define COPPER_WORDS 380
#define SCREEN_ROW_BYTES 40
#define BPLCON0_SIX_PLANES_AGA 0x6201
#define BPLCON2_KILLEHB 0x0200
#define BPLCON3_BORDER_BLANK 0x0020
#define DIWHIGH_PAL_320X256 0x2100
#define FADE_FRAMES 24
#define DISPLAY_LOCK_FRAMES 35
#define PALETTE_SAFE_LINE 100
#define COPPER_ARM_MIN_LINE 100
#define COPPER_ARM_MAX_LINE 250
#define CHARGING_PATCH_X 48
#define CHARGING_PATCH_Y 192
#define CHARGING_PATCH_W 224
#define CHARGING_PATCH_H 40
#define INTRO_PASSAGE_HOLD_FRAMES 240
#define INTRO_TEXT_SCROLL_ROWS 72
#define INTRO_SKIP_HOLD_FRAMES 30
#define READY_MENU_PATCH_Y 118
#define READY_MENU_PATCH_H 104
#define READY_MENU_PATCH_X 64
#define READY_MENU_PATCH_W 192
#define READY_MENU_STATE_START 0
#define READY_MENU_STATE_OPTIONS 1
#define READY_MENU_STATE_JUMP 2
#define READY_MENU_STATE_FIRE 3
#define READY_MENU_STATE_CAMPAIGN_BASE 4
#define READY_MENU_STATE_COUNT 12
#define INTRO_TEXT_FADE_FRAMES 12
#define INTRO_TEXT_PAGE_ROWS 176

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static struct View *previousView;
static UWORD *copper[2];
static UWORD paletteValuePos[2][2][64];
static UWORD introPointerPos[2][6][2];
static UWORD introPalettePos[2][2];
static UWORD copperPos,savedDma;
static UBYTE buildCopperIndex,currentCopper;
static BOOL displayed;
static ULONG titleStartFrame;
static ULONG chargingStartFrame;
static const char *failureReason="unknown title failure";
static ULONG chipFree,chipLargest;
static BOOL buildingIntroCopper;
static struct BitMap *readyMenuBack;
static UBYTE readyMenuBufferIndex;
static struct BitMap *scoreBuffers[2];
static UBYTE scoreBufferIndex;

#define SCORE_NUMBER_X 176
#define SCORE_TILE_W 8
#define SCORE_TILE_H 12
#define SCORE_CLEAR_H 16
#define SCORE_FIELD_CELLS 7
#define SCORE_ROW_COUNT 4
#define SCORE_VALUE_Y_OFFSET 3
#define SCORE_CHAR_COUNT 11
#define SCORE_MENU_REPLAY_ROW 11
#define SCORE_MENU_CONTINUE_ROW 12
#define SCORE_MENU_BACK_ROW 13
#define SCORE_MENU_ARROW_ROW 14
#define SCORE_GLYPH_ROWS 15
#define SCORE_GLYPH_ATLAS_W 144
#define SCORE_PROMPT_X 88
#define SCORE_PROMPT_Y 216
#define SCORE_PROMPT_W 144
#define SCORE_PROMPT_H 40
#define SCORE_MENU_ARROW_X 104
#define SCORE_PAR_SECONDS 120
#define SCORE_TIME_MULTIPLIER 10
static const UWORD scoreRowY[SCORE_ROW_COUNT]={116,137,158,179};

#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
static BPTR introDiagnosticFile;

static void introDiagnosticOpen(void)
{
    introDiagnosticFile=Open("PROGDIR:whdintrodiag.log",MODE_NEWFILE);
    if(introDiagnosticFile) {
        FPrintf(introDiagnosticFile,"Sparkpaw WHDLoad intro diagnostic alpha.55\n");
        Flush(introDiagnosticFile);
    }
}

static void introDiagnosticEvent(const char *event,UWORD plate)
{
    if(!introDiagnosticFile) return;
    FPrintf(introDiagnosticFile,
            "%s plate=%ld failure=%s ioerr=%ld chip_free=%ld chip_largest=%ld fast_free=%ld fast_largest=%ld\n",
            (LONG)event,(LONG)(plate+1),(LONG)assetsLoadFailureReason(),IoErr(),
            AvailMem(MEMF_CHIP),AvailMem(MEMF_CHIP|MEMF_LARGEST),
            AvailMem(MEMF_FAST),AvailMem(MEMF_FAST|MEMF_LARGEST));
    Flush(introDiagnosticFile);
}

static void introDiagnosticClose(void)
{
    if(introDiagnosticFile) Close(introDiagnosticFile);
    introDiagnosticFile=0;
}
#endif

#ifdef SPARKPAW_STORY_INTRO
static void stageIntroText(UWORD passage,UWORD offset);
static void stageIntroTextColour(const struct PlanarAsset *asset,UWORD level);
#endif

#ifdef SPARKPAW_STORY_INTRO
static BOOL introFireHeld(void)
{
    return ((*(volatile UBYTE *)0xbfe001)&0x80)==0;
}

static BOOL introImmediateSkip(void)
{
    return ((*(volatile UBYTE *)0xbfe001)&0x40)==0;
}

static BOOL waitIntroPassage(UWORD passage)
{
    UWORD frames=0,held;
    while(introFireHeld()) WaitTOF();
    while(frames<INTRO_PASSAGE_HOLD_FRAMES) {
        if(introImmediateSkip()) return TRUE;
        if(introFireHeld()) {
            held=0;
            while(introFireHeld()&&held<INTRO_SKIP_HOLD_FRAMES) {
                if(introImmediateSkip()) return TRUE;
                WaitTOF(); held++;
            }
            if(held>=INTRO_SKIP_HOLD_FRAMES) {
                while(introFireHeld()) WaitTOF();
                return TRUE;
            }
            if(frames<=INTRO_TEXT_SCROLL_ROWS) {
                stageIntroText(passage,INTRO_TEXT_SCROLL_ROWS);
                frames=INTRO_TEXT_SCROLL_ROWS+1;
            } else return FALSE;
        } else {
            if(frames<=INTRO_TEXT_SCROLL_ROWS) stageIntroText(passage,frames);
            WaitTOF(); frames++;
        }
    }
    return FALSE;
}
#endif

static void cmove(UWORD reg,UWORD value)
{
    copper[buildCopperIndex][copperPos++]=reg;
    copper[buildCopperIndex][copperPos++]=value;
}

static void cptr(UWORD reg,APTR value)
{
    ULONG pointer=(ULONG)value;
    cmove(reg,(UWORD)(pointer>>16));
    cmove((UWORD)(reg+2),(UWORD)pointer);
}

static UBYTE fadeComponent(UBYTE component,UWORD level)
{
    return (UBYTE)(((ULONG)component*level)>>8);
}

static UWORD paletteWord(const UBYTE *rgb,UWORD level,BOOL lowNibble)
{
    UBYTE red=fadeComponent(rgb[0],level);
    UBYTE green=fadeComponent(rgb[1],level);
    UBYTE blue=fadeComponent(rgb[2],level);
    if(lowNibble)
        return (UWORD)(((red&15)<<8)|((green&15)<<4)|(blue&15));
    return (UWORD)(((red>>4)<<8)|((green>>4)<<4)|(blue>>4));
}

static void writePalette(const struct PlanarAsset *asset,UWORD level)
{
    UWORD bank,index;
    for(bank=0;bank<2;bank++) {
        cmove(0x106,(UWORD)(0x0020|(bank<<13)));
        for(index=0;index<32;index++) {
            const UBYTE *rgb=asset->palette[bank*32+index];
            UWORD pen=(UWORD)(bank*32+index);
            paletteValuePos[buildCopperIndex][0][pen]=copperPos+1;
            cmove((UWORD)(0x180+index*2),
                  paletteWord(rgb,level,FALSE));
        }
        cmove(0x106,(UWORD)(0x0220|(bank<<13)));
        for(index=0;index<32;index++) {
            const UBYTE *rgb=asset->palette[bank*32+index];
            UWORD pen=(UWORD)(bank*32+index);
            paletteValuePos[buildCopperIndex][1][pen]=copperPos+1;
            cmove((UWORD)(0x180+index*2),
                  paletteWord(rgb,level,TRUE));
        }
    }
    cmove(0x106,0x0020);
}

static void buildCopper(const struct PlanarAsset *asset,UBYTE index,UWORD level)
{
    UBYTE plane;
    buildCopperIndex=index; copperPos=0;
    cmove(0x08e,0x2c81); cmove(0x090,0x2cc1);
    /* DIWSTRT/DIWSTOP reset DIWHIGH for legacy compatibility. Write the
       complete PAL 320x256 stop high bits afterwards so an AGA Workbench
       mode cannot leak its extended display-window state into this View. */
    cmove(0x1e4,DIWHIGH_PAL_320X256);
    cmove(0x092,0x0038); cmove(0x094,0x00d0);
    /* The OS-managed reference View selected ordinary 64-colour AGA mode.
       KILLEHB is required here as well; without it pens 32..63 are interpreted
       as half-bright copies of pens 0..31. ECSENA makes the AGA controls,
       including border blanking and palette banking, explicit rather than
       inheriting their availability from the preceding Workbench display. */
    cmove(0x100,BPLCON0_SIX_PLANES_AGA); cmove(0x102,0x0000);
    cmove(0x104,BPLCON2_KILLEHB); cmove(0x106,BPLCON3_BORDER_BLANK);
    cmove(0x108,(UWORD)(asset->bitmap->BytesPerRow-SCREEN_ROW_BYTES));
    cmove(0x10a,(UWORD)(asset->bitmap->BytesPerRow-SCREEN_ROW_BYTES));
    cmove(0x10c,0x0000); cmove(0x1fc,0x0000);
    for(plane=0;plane<6;plane++)
        cptr((UWORD)(0x0e0+plane*4),asset->bitmap->Planes[plane]);
    writePalette(asset,level);
#ifdef SPARKPAW_STORY_INTRO
    if(buildingIntroCopper) {
        /* PAL display row 168 starts at hardware line 44+168=212. Keep the
           illustration fixed and fetch the lower band from its own tall
           source window so only the text pointer moves. */
        copper[index][copperPos++]=0xd401;
        copper[index][copperPos++]=0xfffe;
        for(plane=0;plane<6;plane++) {
            ULONG pointer=(ULONG)(asset->bitmap->Planes[plane]+256L*asset->bitmap->BytesPerRow);
            introPointerPos[index][plane][0]=copperPos+1;
            cmove((UWORD)(0x0e0+plane*4),(UWORD)(pointer>>16));
            introPointerPos[index][plane][1]=copperPos+1;
            cmove((UWORD)(0x0e2+plane*4),(UWORD)pointer);
        }
        cmove(0x106,0x2020);
        introPalettePos[index][0]=copperPos+1;
        cmove(0x1be,paletteWord(asset->palette[63],0,FALSE));
        cmove(0x106,0x2220);
        introPalettePos[index][1]=copperPos+1;
        cmove(0x1be,paletteWord(asset->palette[63],0,TRUE));
        cmove(0x106,0x0020);
    }
#endif
    copper[index][copperPos++]=0xffff;
    copper[index][copperPos++]=0xfffe;
}

static BOOL allocateCopper(void)
{
    UBYTE index;
    for(index=0;index<2;index++) {
        copper[index]=(UWORD *)AllocMem(COPPER_WORDS*sizeof(UWORD),
                                        MEMF_CHIP|MEMF_CLEAR);
        if(!copper[index]) return FALSE;
    }
    return TRUE;
}

static void installCopper(UBYTE index)
{
    WaitTOF();
    hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
    hardware->cop1lc=(ULONG)copper[index]; hardware->copjmp1=0;
    hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER;
    currentCopper=index; displayed=TRUE;
}

static UWORD rasterLine(void)
{
    UWORD high=hardware->vposr,low=hardware->vhposr;
    return (UWORD)(((high&7)<<8)|(low>>8));
}

static void waitOwnedDisplayFrame(void)
{
    /* Exec interrupts are disabled after platformFinishTakeover(), so WaitTOF
       can no longer complete. Synchronise the ready-screen input/fade to one
       real PAL frame boundary directly from the beam instead. */
    if(rasterLine()<300) while(rasterLine()<300) { }
    while(rasterLine()>=300) { }
}

static void waitOwnedCopperArmWindow(void)
{
    /* Change only COP1LC while the current list is safely in flight. The
       Copper reloads this address naturally at the next vertical blank; an
       explicit COPJMP1 here can race that automatic restart on real AGA. */
    if(rasterLine()>=COPPER_ARM_MAX_LINE)
        while(rasterLine()>=COPPER_ARM_MAX_LINE) { }
    while(rasterLine()<COPPER_ARM_MIN_LINE) { }
}

#ifdef SPARKPAW_STORY_INTRO
static void stageIntroText(UWORD passage,UWORD offset)
{
    const struct PlanarAsset *asset=assetsStoryIntro();
    UBYTE plane;
    if(offset>INTRO_TEXT_SCROLL_ROWS) offset=INTRO_TEXT_SCROLL_ROWS;
    while(rasterLine()<PALETTE_SAFE_LINE) { }
    for(plane=0;plane<6;plane++) {
        ULONG pointer=(ULONG)(asset->bitmap->Planes[plane]+
            (256L+(LONG)passage*INTRO_TEXT_PAGE_ROWS+offset)*asset->bitmap->BytesPerRow);
        copper[currentCopper][introPointerPos[currentCopper][plane][0]]=(UWORD)(pointer>>16);
        copper[currentCopper][introPointerPos[currentCopper][plane][1]]=(UWORD)pointer;
    }
}

static void stageIntroTextColour(const struct PlanarAsset *asset,UWORD level)
{
    const UBYTE *rgb=asset->palette[63];
    while(rasterLine()<PALETTE_SAFE_LINE) { }
    copper[currentCopper][introPalettePos[currentCopper][0]]=
        paletteWord(rgb,level,FALSE);
    copper[currentCopper][introPalettePos[currentCopper][1]]=
        paletteWord(rgb,level,TRUE);
}

static void fadeIntroText(const struct PlanarAsset *asset)
{
    UWORD frame;
    for(frame=0;frame<INTRO_TEXT_FADE_FRAMES;frame++) {
        stageIntroTextColour(asset,(UWORD)(256-
            ((ULONG)(frame+1)*256)/INTRO_TEXT_FADE_FRAMES));
        WaitTOF();
    }
}
#endif

static void stagePalette(const struct PlanarAsset *asset,UWORD level)
{
    UWORD pen;
    while(rasterLine()<PALETTE_SAFE_LINE) { }
    for(pen=0;pen<64;pen++) {
        const UBYTE *rgb=asset->palette[pen];
        copper[currentCopper][paletteValuePos[currentCopper][0][pen]]=
            paletteWord(rgb,level,FALSE);
        copper[currentCopper][paletteValuePos[currentCopper][1][pen]]=
            paletteWord(rgb,level,TRUE);
    }
}

static void fadeTo(const struct PlanarAsset *asset,BOOL fadeIn)
{
    UWORD frame;
    for(frame=1;frame<=FADE_FRAMES;frame++) {
        UWORD level=fadeIn?(UWORD)(((ULONG)frame*256)/FADE_FRAMES):
                           (UWORD)(((ULONG)(FADE_FRAMES-frame)*256)/FADE_FRAMES);
        stagePalette(asset,level);
        WaitTOF();
    }
}

static BOOL titleShowInternal(BOOL playStory)
{
#ifdef SPARKPAW_STORY_INTRO
    static const UBYTE passageCounts[5]={2,2,3,2,2};
    UBYTE plate,passage,next;
    BOOL skipIntro=FALSE;
#endif
    chipFree=AvailMem(MEMF_CHIP);
    chipLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
    introDiagnosticOpen();
    introDiagnosticEvent("before_load",0);
#endif
#ifdef SPARKPAW_STORY_INTRO
    if(playStory&&!assetsLoadStoryIntro(0)) {
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
        introDiagnosticEvent("load_failed",0); introDiagnosticClose();
#endif
        failureReason="six-plane intro proof asset load failed"; return FALSE;
    } else if(!playStory&&!assetsLoadTitle()) {
        failureReason="six-plane title asset load failed"; return FALSE;
    }
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
    introDiagnosticEvent("load_ok",0);
#endif
#else
    if(!assetsLoadTitle()) {
        failureReason="six-plane title asset load failed"; return FALSE;
    }
#endif
    if(!allocateCopper()) {
        failureReason="title Copper allocation failed";
        titleRelease(); return FALSE;
    }
    previousView=GfxBase->ActiView;
    savedDma=hardware->dmaconr&DMAF_ALL;
    /* Detach the active OS View before owning COP1LC directly. DOS and
       interrupts remain available for loading, but graphics.library no
       longer has a Workbench Copper to reinstall at VBlank. A minimal ADF
       boot happened to leave an inert View; Workbench does not. */
    LoadView(NULL);
    WaitTOF(); WaitTOF();
#ifdef SPARKPAW_STORY_INTRO
    if(playStory) {
        buildingIntroCopper=TRUE;
        buildCopper(assetsStoryIntro(),0,0);
        buildingIntroCopper=FALSE;
    } else buildCopper(assetsTitle(),0,0);
#else
    buildCopper(assetsTitle(),0,0);
#endif
    installCopper(0);
    /* Give scandoublers time to lock to the PAL low-resolution display while
       it is deliberately black, then begin the authored fade. */
    {
        UWORD frame;
        for(frame=0;frame<DISPLAY_LOCK_FRAMES;frame++) WaitTOF();
    }
#ifdef SPARKPAW_STORY_INTRO
    if(playStory) {
    for(plate=0;plate<5&&!skipIntro;plate++) {
        if(plate) {
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
            introDiagnosticEvent("before_load",plate);
#endif
            if(!assetsLoadStoryIntro(plate)) {
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
                introDiagnosticEvent("load_failed",plate);
                introDiagnosticClose();
#endif
                failureReason="six-plane intro plate asset load failed";
                titleRelease(); return FALSE;
            }
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
            introDiagnosticEvent("load_ok",plate);
#endif
            next=currentCopper^1;
            buildingIntroCopper=TRUE;
            buildCopper(assetsStoryIntro(),next,0);
            buildingIntroCopper=FALSE;
            installCopper(next);
        }
        fadeTo(assetsStoryIntro(),TRUE);
        for(passage=0;passage<passageCounts[plate]&&!skipIntro;passage++) {
            stageIntroText(passage,0);
            stageIntroTextColour(assetsStoryIntro(),256);
            skipIntro=waitIntroPassage(passage);
            fadeIntroText(assetsStoryIntro());
        }
        fadeTo(assetsStoryIntro(),FALSE);
        assetsUnloadStoryIntro();
    }
    if(!assetsLoadTitle()) {
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
        introDiagnosticEvent("title_load_failed",4); introDiagnosticClose();
#endif
        failureReason="six-plane title asset load after intro failed";
        titleRelease(); return FALSE;
    }
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
    introDiagnosticEvent("intro_complete",4); introDiagnosticClose();
#endif
    buildCopper(assetsTitle(),currentCopper^1,0);
    installCopper(currentCopper^1);
    }
#endif
    fadeTo(assetsTitle(),TRUE);
    titleStartFrame=GfxBase->VBCounter;
    return TRUE;
}

BOOL titleShow(void) { return titleShowInternal(TRUE); }
BOOL titleShowMain(void) { return titleShowInternal(FALSE); }

BOOL titleShowMainFromResults(void)
{
    UBYTE next=currentCopper^1,index;
    if(!displayed||!assetsLevelComplete()->bitmap) {
        failureReason="results display unavailable for title transition";
        return FALSE;
    }
    if(!assetsLoadTitle()) {
        failureReason="six-plane title asset load failed";
        return FALSE;
    }
    /* The old score Copper remains valid until its inactive replacement is
       complete. This ordering is mandatory while Raster/Copper DMA stays on:
       freeing titleRelease() first leaves the custom chips reading released
       score memory and can hang forever on a black frame. */
    buildCopper(assetsTitle(),next,0);
    installCopper(next);
    fadeTo(assetsTitle(),TRUE);
    for(index=0;index<2;index++) if(scoreBuffers[index]) {
        FreeBitMap(scoreBuffers[index]); scoreBuffers[index]=NULL;
    }
    assetsUnloadScoreGlyphs();
    assetsUnloadLevelComplete();
    titleStartFrame=GfxBase->VBCounter;
    return TRUE;
}

BOOL titlePrepareLevelLoading(void)
{
    if(!displayed) {
        failureReason="title display unavailable for loading image";
        return FALSE;
    }
    if(!assetsLoadLevelLoading()) {
        failureReason="six-plane loading image asset load failed"; return FALSE;
    }
    if(!assetsLoadLevelCharging()) {
        failureReason="six-plane charging image asset load failed";
        assetsUnloadLevelLoading();
        return FALSE;
    }
    buildCopper(assetsLevelLoading(),currentCopper^1,0);
    return TRUE;
}

BOOL titleShowLevelCharging(void)
{
    UBYTE plane; UWORD row;
    const struct PlanarAsset *loading=assetsLevelLoading();
    const struct PlanarAsset *patch=assetsLevelCharging();
#ifdef SPARKPAW_MULTI_ADF
    if(!patch->bitmap&&!assetsLoadLevelCharging()) return FALSE;
#endif
    if(!displayed||!loading->bitmap||!patch->bitmap||
       patch->width!=CHARGING_PATCH_W||patch->height!=CHARGING_PATCH_H) {
        failureReason="charging image was not prepared"; return FALSE;
    }
    /* Keep one complete floppy image.  Fade it fully black, replace only the
       status band from the CPU-only Fast-RAM patch, then reveal CHARGING.
       This is a one-shot write, never a read/modify/write of visible Chip RAM. */
    fadeTo(loading,FALSE);
    WaitTOF();
    for(plane=0;plane<6;plane++) for(row=0;row<CHARGING_PATCH_H;row++)
        CopyMem(patch->bitmap->Planes[plane]+(LONG)row*patch->bitmap->BytesPerRow,
                loading->bitmap->Planes[plane]+
                    (LONG)(CHARGING_PATCH_Y+row)*loading->bitmap->BytesPerRow+
                    CHARGING_PATCH_X/8,
                CHARGING_PATCH_W/8);
    fadeTo(loading,TRUE);
    chargingStartFrame=GfxBase->VBCounter;
    assetsUnloadLevelCharging();
    return TRUE;
}

void titleWaitLevelCharging(UWORD frames)
{
    while((ULONG)(GfxBase->VBCounter-chargingStartFrame)<frames) WaitTOF();
}

BOOL titleShowLevelLoading(void)
{
    UBYTE next=currentCopper^1;
    if(!assetsLevelLoading()->bitmap) {
        failureReason="loading image was not prepared"; return FALSE;
    }
    fadeTo(assetsTitle(),FALSE);
    next=currentCopper^1;
    installCopper(next);
    assetsUnloadTitle();
    fadeTo(assetsLevelLoading(),TRUE);
#ifdef SPARKPAW_MULTI_ADF
    assetsRetireOldLoading();
#endif
    return TRUE;
}

BOOL titleShowReplayLoading(void)
{
    UBYTE next=currentCopper^1,index;
    if(!displayed) {
        failureReason="score display unavailable for replay loading";
        return FALSE;
    }
    if(!assetsLoadLevelLoading()) {
        failureReason="six-plane replay loading image asset load failed";
        return FALSE;
    }
    /* The score display has already faded to black and Exec/DOS own the
       machine again.  Install the ordinary loading image while they remain
       available, so the much slower 68020 renderer rebuild is not presented
       as an indefinite black-screen hang. */
    buildCopper(assetsLevelLoading(),next,0);
    installCopper(next);
    fadeTo(assetsLevelLoading(),TRUE);
#ifdef SPARKPAW_MULTI_ADF
    assetsRetireOldLoading();
#endif
    /* The loading Copper is now wholly independent. Retire the old results
       buffers and assets before the renderer rebuild, so titleFadeOut() later
       selects the visible loading palette rather than restaging score colours
       over the disk image. */
    for(index=0;index<2;index++) if(scoreBuffers[index]) {
        FreeBitMap(scoreBuffers[index]); scoreBuffers[index]=NULL;
    }
    assetsUnloadScoreGlyphs();
    assetsUnloadLevelComplete();
    return TRUE;
}

#ifdef SPARKPAW_MULTI_ADF
BOOL titleShowInsertDisk(UBYTE disk)
{
    /* Every section-load caller has already published the loading image.
       Reuse it: only the 224x40 status strip needs a cold read before swap. */
    if(!assetsLoadDiskPatch(disk)) return FALSE;
    return titleShowLevelCharging();
}
#endif

BOOL titleShowLevelReady(void)
{
    UBYTE plane;
    LONG planeBytes;
    const struct PlanarAsset *loading=assetsLevelLoading();
    const struct PlanarAsset *ready;
    struct PlanarAsset backDisplay;
    if(!displayed||!loading->bitmap) {
        failureReason="loading display unavailable for ready screen";
        return FALSE;
    }
    if(!assetsLoadLevelReady()) {
        failureReason="six-plane ready screen asset load failed";
        return FALSE;
    }
    if(!assetsLoadLevelReadyMenu()) {
        failureReason="ready menu patch asset load failed";
        assetsUnloadLevelReady();
        return FALSE;
    }
    ready=assetsLevelReady();
    if(!ready->bitmap||ready->width!=320||ready->height!=256||
       !assetsLevelReadyMenu()->bitmap||
       assetsLevelReadyMenu()->width!=READY_MENU_PATCH_W||
       assetsLevelReadyMenu()->height!=READY_MENU_PATCH_H*READY_MENU_STATE_COUNT) {
        failureReason="ready screen asset has invalid geometry";
        assetsUnloadLevelReadyMenu();
        assetsUnloadLevelReady();
        return FALSE;
    }
    readyMenuBack=AllocBitMap(ready->width,ready->height,ready->depth,
                              BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!readyMenuBack) {
        failureReason="tear-free ready buffer allocation failed";
        assetsUnloadLevelReadyMenu();
        assetsUnloadLevelReady();
        return FALSE;
    }
    /* Gameplay is already fully prepared. Seed both presentation buffers while
       black; later menu changes patch only the hidden buffer and publish it
       through the inactive Copper list. */
    fadeTo(loading,FALSE);
    WaitTOF();
    planeBytes=(LONG)loading->bitmap->BytesPerRow*loading->height;
    for(plane=0;plane<6;plane++) {
        CopyMem(ready->bitmap->Planes[plane],loading->bitmap->Planes[plane],
                planeBytes);
        CopyMem(ready->bitmap->Planes[plane],readyMenuBack->Planes[plane],
                planeBytes);
    }
    readyMenuBufferIndex=0;
    backDisplay=*ready;
    backDisplay.bitmap=readyMenuBack;
    buildCopper(&backDisplay,currentCopper^1,256);
    stagePalette(ready,0);
    fadeTo(ready,TRUE);
    return TRUE;
}

static void copyScoreBase(struct BitMap *destination)
{
    const struct BitMap *source=assetsLevelComplete()->bitmap;
    UBYTE plane;
    LONG bytes=(LONG)source->BytesPerRow*256;
    for(plane=0;plane<6;plane++)
        CopyMem(source->Planes[plane],destination->Planes[plane],bytes);
}

BOOL titleShowLevelComplete(void)
{
    struct PlanarAsset display;
    UBYTE index;
    chipFree=AvailMem(MEMF_CHIP);
    chipLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
    if(!assetsLoadLevelComplete()||!assetsLoadScoreGlyphs()) {
        failureReason="level-complete assets unavailable";
        return FALSE;
    }
    if(assetsLevelComplete()->width!=320||assetsLevelComplete()->height!=256||
       assetsScoreGlyphs()->width!=SCORE_GLYPH_ATLAS_W||
       assetsScoreGlyphs()->height!=SCORE_TILE_H*SCORE_GLYPH_ROWS) {
        failureReason="level-complete assets have invalid geometry";
        return FALSE;
    }
    if(!allocateCopper()) {
        failureReason="level-complete Copper allocation failed"; return FALSE;
    }
    for(index=0;index<2;index++) {
        scoreBuffers[index]=AllocBitMap(320,256,6,
                                        BMF_CLEAR|BMF_DISPLAYABLE,NULL);
        if(!scoreBuffers[index]) {
            failureReason="level-complete display allocation failed";
            return FALSE;
        }
        copyScoreBase(scoreBuffers[index]);
    }
    previousView=GfxBase->ActiView;
    savedDma=hardware->dmaconr&DMAF_ALL;
    LoadView(NULL); WaitTOF(); WaitTOF();
    display=*assetsLevelComplete(); display.bitmap=scoreBuffers[0];
    buildCopper(&display,0,0); installCopper(0);
    scoreBufferIndex=0;
    fadeTo(&display,TRUE);
    return TRUE;
}

static UBYTE scoreGlyphIndex(char value)
{
    if(value>='0'&&value<='9') return (UBYTE)(value-'0');
    return 10;
}

static void scoreCopyRect(const struct BitMap *source,struct BitMap *dest,
                          UWORD sourceX,UWORD sourceY,UWORD destX,UWORD destY,
                          UWORD width,UWORD height)
{
    UBYTE plane; UWORD row; UWORD bytes=width>>3;
    for(plane=0;plane<6;plane++) for(row=0;row<height;row++)
        CopyMem(source->Planes[plane]+(LONG)(sourceY+row)*
                source->BytesPerRow+(sourceX>>3),
                dest->Planes[plane]+(LONG)(destY+row)*dest->BytesPerRow+
                (destX>>3),bytes);
}

static void scoreDrawText(struct BitMap *dest,UBYTE row,const char *text,
                          UBYTE length)
{
    const struct BitMap *glyphs=assetsScoreGlyphs()->bitmap;
    UWORD x=(UWORD)(SCORE_NUMBER_X+
                    (SCORE_FIELD_CELLS-length)*SCORE_TILE_W);
    UBYTE index;
    for(index=0;index<length;index++,x+=SCORE_TILE_W) {
        char value=text[index];
        if(value==' ') continue;
        scoreCopyRect(glyphs,dest,0,
                      (UWORD)(scoreGlyphIndex(value)*SCORE_TILE_H),
                      x,(UWORD)(scoreRowY[row]+SCORE_VALUE_Y_OFFSET),
                      SCORE_TILE_W,SCORE_TILE_H);
    }
}

static UBYTE appendDecimal(char *target,ULONG value,UBYTE minimum)
{
    char reverse[10]; UBYTE count=0,index;
    do { reverse[count++]=(char)('0'+value%10); value/=10; } while(value);
    while(count<minimum) reverse[count++]='0';
    for(index=0;index<count;index++) target[index]=reverse[count-1-index];
    return count;
}

static void scoreExpression(char *text,UBYTE *length,ULONG count,UWORD value,
                            UBYTE minimum)
{
    UBYTE at=appendDecimal(text,count,minimum);
    text[at++]='X';
    at+=appendDecimal(text+at,value,3);
    *length=at;
}

static void publishScoreState(UWORD enemies,UWORD diamonds,UWORD timeSeconds,
                              ULONG total,UBYTE menu,UBYTE selection)
{
    struct BitMap *hidden=scoreBuffers[scoreBufferIndex^1];
    struct PlanarAsset display=*assetsLevelComplete();
    char text[10]; UBYTE length,row,next=(UBYTE)(currentCopper^1);
    for(row=0;row<SCORE_ROW_COUNT;row++)
        scoreCopyRect(assetsLevelComplete()->bitmap,hidden,SCORE_NUMBER_X,
                      scoreRowY[row],SCORE_NUMBER_X,scoreRowY[row],
                      SCORE_FIELD_CELLS*SCORE_TILE_W,SCORE_CLEAR_H);
    scoreCopyRect(assetsLevelComplete()->bitmap,hidden,SCORE_PROMPT_X,
                  SCORE_PROMPT_Y,SCORE_PROMPT_X,SCORE_PROMPT_Y,
                  SCORE_PROMPT_W,SCORE_PROMPT_H);
    scoreExpression(text,&length,enemies,20,3); scoreDrawText(hidden,0,text,length);
    scoreExpression(text,&length,diamonds,5,3); scoreDrawText(hidden,1,text,length);
    scoreExpression(text,&length,timeSeconds,10,3); scoreDrawText(hidden,2,text,length);
    length=appendDecimal(text,total,6); scoreDrawText(hidden,3,text,length);
    if(menu) {
        UWORD replayY=menu==1?232:218;
        UWORD secondRow=menu==2?SCORE_MENU_CONTINUE_ROW:SCORE_MENU_BACK_ROW;
        scoreCopyRect(assetsScoreGlyphs()->bitmap,hidden,0,
                      SCORE_MENU_REPLAY_ROW*SCORE_TILE_H,
                      SCORE_PROMPT_X,replayY,SCORE_PROMPT_W,SCORE_TILE_H);
        if(menu!=1)
            scoreCopyRect(assetsScoreGlyphs()->bitmap,hidden,0,
                          secondRow*SCORE_TILE_H,
                          SCORE_PROMPT_X,234,SCORE_PROMPT_W,SCORE_TILE_H);
        if(menu!=1)
            scoreCopyRect(assetsScoreGlyphs()->bitmap,hidden,0,
                          SCORE_MENU_ARROW_ROW*SCORE_TILE_H,
                          SCORE_MENU_ARROW_X,selection?234:218,
                          SCORE_TILE_W,SCORE_TILE_H);
    }
    display.bitmap=hidden; buildCopper(&display,next,256);
    waitOwnedCopperArmWindow(); hardware->cop1lc=(ULONG)copper[next];
    currentCopper=next; scoreBufferIndex^=1;
}

static BOOL resultFire(void)
{
    BOOL left,right,down,jump,fire;
    platformReadGameKeys(&left,&right,&down,&jump,&fire);
    return fire||((*(volatile UBYTE *)0xbfe001)&0x80)==0;
}

void titleRunLevelComplete(UWORD enemies,UWORD diamonds,
                           ULONG elapsedFields,ULONG liveScore)
{
    UWORD elapsed=(UWORD)(elapsedFields/50UL);
    UWORD timeSeconds=elapsed<SCORE_PAR_SECONDS?
                      (UWORD)(SCORE_PAR_SECONDS-elapsed):0;
    titleRunLevelCompleteWithBonus(enemies,diamonds,timeSeconds,liveScore);
}

static enum ResultDecision runLevelCompleteMenu(UWORD enemies,UWORD diamonds,
    UWORD timeSeconds,ULONG liveScore,UBYTE menuMode);

enum ResultDecision titleRunLevelCompleteMenu(UWORD enemies,UWORD diamonds,
    ULONG elapsedFields,ULONG liveScore,BOOL stormrail)
{
    UWORD elapsed=(UWORD)(elapsedFields/50UL);
    UWORD timeSeconds=elapsed<SCORE_PAR_SECONDS?
                      (UWORD)(SCORE_PAR_SECONDS-elapsed):0;
    return runLevelCompleteMenu(enemies,diamonds,timeSeconds,liveScore,
                                stormrail?3:2);
}

enum ResultDecision titleRunLevelCompleteWithBonusMenu(UWORD enemies,
    UWORD diamonds,UWORD timeSeconds,ULONG liveScore,BOOL stormrail)
{
    return runLevelCompleteMenu(enemies,diamonds,timeSeconds,liveScore,
                                stormrail?3:2);
}

void titleRunLevelCompleteWithBonus(UWORD enemies,UWORD diamonds,
                                    UWORD timeSeconds,ULONG liveScore)
{
    (void)runLevelCompleteMenu(enemies,diamonds,timeSeconds,liveScore,1);
}

static enum ResultDecision runLevelCompleteMenu(UWORD enemies,UWORD diamonds,
    UWORD timeSeconds,ULONG liveScore,UBYTE menuMode)
{
    UWORD shownEnemies=enemies,shownDiamonds=diamonds,shownTime=timeSeconds;
    ULONG total=0,expected=liveScore+(ULONG)timeSeconds*SCORE_TIME_MULTIPLIER;
    UBYTE phase=0,tick=0,selection=(menuMode!=1)?1:0;
    /* Every two-choice results menu defaults to its forward/exit action:
       CONTINUE after Level 1 and BACK TO TITLE after Stormrail. Require one
       neutral menu sample before accepting navigation: a completion jump maps
       to Up in this UI and must never move that explicit default. */
    BOOL directionHeld=TRUE;
#ifdef SPARKPAW_REPLAY_PROOF
    publishScoreState(0,0,0,liveScore,1,0);
    waitOwnedDisplayFrame();
    waitOwnedDisplayFrame();
    return RESULT_DECISION_REPLAY_CURRENT;
#endif
    publishScoreState(shownEnemies,shownDiamonds,shownTime,total,0,0);
    while(phase<3) {
        BOOL fire=resultFire();
        waitOwnedDisplayFrame(); audioUpdate();
        if(fire) {
            if(phase==0) { total+=(ULONG)shownEnemies*20UL; shownEnemies=0; }
            else if(phase==1) { total+=(ULONG)shownDiamonds*5UL; shownDiamonds=0; }
            else { total+=(ULONG)shownTime*10UL; shownTime=0; }
            publishScoreState(shownEnemies,shownDiamonds,shownTime,total,0,0);
            phase++;
            while(resultFire()) { waitOwnedDisplayFrame(); audioUpdate(); }
            continue;
        }
        if(++tick<4) continue;
        tick=0;
        if(phase==0&&shownEnemies) { shownEnemies--; total+=20; }
        else if(phase==1&&shownDiamonds) {
            UWORD step=shownDiamonds>20?2:1;
            shownDiamonds-=step; total+=(ULONG)step*5UL;
        } else if(phase==2&&shownTime) {
            UWORD step=shownTime>60?10:shownTime>20?5:1;
            if(step>shownTime) step=shownTime;
            shownTime-=step; total+=(ULONG)step*10UL;
        } else { phase++; continue; }
        audioPlayTallyTick();
        publishScoreState(shownEnemies,shownDiamonds,shownTime,total,0,0);
    }
    if(total!=expected) total=expected;
    publishScoreState(0,0,0,total,menuMode,selection);
    while(resultFire()) { waitOwnedDisplayFrame(); audioUpdate(); }
    for(;;) {
        BOOL left,right,down,up,fire;
        UWORD value=*(volatile UWORD *)0xdff00c;
        platformReadGameKeys(&left,&right,&down,&up,&fire);
        down=down||(((value^(value>>1))&0x0001)!=0);
        up=up||(((value^(value>>1))&0x0100)!=0);
        fire=fire||(((*(volatile UBYTE *)0xbfe001)&0x80)==0);
        if(!up&&!down) directionHeld=FALSE;
        else if(menuMode!=1&&!directionHeld) {
            selection^=1; directionHeld=TRUE;
            publishScoreState(0,0,0,total,menuMode,selection);
        }
        if(fire) break;
        waitOwnedDisplayFrame(); audioUpdate();
    }
    while(resultFire()) { waitOwnedDisplayFrame(); audioUpdate(); }
    if(!selection) return RESULT_DECISION_REPLAY_CURRENT;
    return menuMode==3?RESULT_DECISION_BACK_TO_TITLE:RESULT_DECISION_CONTINUE;
}

static void readReadyMenuInput(BOOL *up,BOOL *down,BOOL *left,BOOL *right,
                               BOOL *fire)
{
    UWORD value=*(volatile UWORD *)0xdff00c;
    BOOL keyLeft,keyRight,keyDown,keyUp,keyFire;
    platformReadGameKeys(&keyLeft,&keyRight,&keyDown,&keyUp,&keyFire);
    *left=((value&0x0200)!=0)||keyLeft;
    *right=((value&0x0002)!=0)||keyRight;
    *down=(((value^(value>>1))&0x0001)!=0)||keyDown;
    *up=(((value^(value>>1))&0x0100)!=0)||keyUp;
    *fire=(((*(volatile UBYTE *)0xbfe001)&0x80)==0)||keyFire;
}

static void showReadyMenuState(UBYTE state)
{
    UBYTE next,plane;
    UWORD row;
    LONG sourceRowBytes,targetRowBytes,planeOffset,targetOffset;
    const struct PlanarAsset *loading=assetsLevelLoading();
    const struct PlanarAsset *ready=assetsLevelReady();
    const struct PlanarAsset *patches=assetsLevelReadyMenu();
    struct PlanarAsset hiddenDisplay=*ready;
    struct BitMap *hidden=readyMenuBufferIndex?loading->bitmap:readyMenuBack;
    sourceRowBytes=patches->bitmap->BytesPerRow;
    targetRowBytes=hidden->BytesPerRow;
    planeOffset=(LONG)state*READY_MENU_PATCH_H*sourceRowBytes;
    targetOffset=(LONG)READY_MENU_PATCH_Y*targetRowBytes+
                 READY_MENU_PATCH_X/8;
    for(plane=0;plane<6;plane++)
        for(row=0;row<READY_MENU_PATCH_H;row++)
            CopyMem(patches->bitmap->Planes[plane]+planeOffset+
                    (LONG)row*sourceRowBytes,
                    hidden->Planes[plane]+targetOffset+
                    (LONG)row*targetRowBytes,sourceRowBytes);
    hiddenDisplay.bitmap=hidden;
    next=currentCopper^1;
    buildCopper(&hiddenDisplay,next,256);
    waitOwnedCopperArmWindow();
    hardware->cop1lc=(ULONG)copper[next];
    waitOwnedDisplayFrame();
    currentCopper=next;
    readyMenuBufferIndex^=1;
}

#ifdef SPARKPAW_CAMPAIGN
static UBYTE readyCampaignOptionsState(
    enum SecondaryButtonAction secondaryAction,
    enum CampaignStartSection startSection,UBYTE optionRow)
{
    return (UBYTE)(READY_MENU_STATE_CAMPAIGN_BASE+
        campaignOptionsVariant(secondaryAction,startSection,optionRow));
}
#endif

void titleRunLevelReadyMenu(enum SecondaryButtonAction *secondaryAction,
    enum CampaignStartSection *startSection)
{
    UBYTE state=READY_MENU_STATE_START;
#ifdef SPARKPAW_CAMPAIGN
    UBYTE optionRow=0;
#endif
    BOOL up,down,left,right,fire;
    BOOL oldUp=FALSE,oldDown=FALSE,oldLeft=FALSE,oldRight=FALSE,oldFire;
    readReadyMenuInput(&up,&down,&left,&right,&fire);
    oldFire=fire;
    while(oldFire) {
#ifdef SPARKPAW_WHDLOAD
        if(platformWHDLoadQuitRequested()) return;
#endif
        waitOwnedDisplayFrame();
        readReadyMenuInput(&up,&down,&left,&right,&oldFire);
    }
    oldUp=up; oldDown=down; oldLeft=left; oldRight=right;
    for(;;) {
        waitOwnedDisplayFrame();
        readReadyMenuInput(&up,&down,&left,&right,&fire);
#ifdef SPARKPAW_WHDLOAD
        if(platformWHDLoadQuitRequested()) return;
#endif
        if(state<READY_MENU_STATE_JUMP) {
            if((up&&!oldUp)||(down&&!oldDown)) {
                state=(state==READY_MENU_STATE_START)?
                      READY_MENU_STATE_OPTIONS:READY_MENU_STATE_START;
                showReadyMenuState(state);
            }
            if(fire&&!oldFire) {
                if(state==READY_MENU_STATE_START) return;
#ifdef SPARKPAW_CAMPAIGN
                state=readyCampaignOptionsState(*secondaryAction,
                                                 *startSection,optionRow);
#else
                state=(*secondaryAction==SECONDARY_BUTTON_JUMP)?
                      READY_MENU_STATE_JUMP:READY_MENU_STATE_FIRE;
#endif
                showReadyMenuState(state);
            }
        } else {
#ifdef SPARKPAW_CAMPAIGN
            if((up&&!oldUp)||(down&&!oldDown)) {
                optionRow^=1;
                state=readyCampaignOptionsState(*secondaryAction,
                                                 *startSection,optionRow);
                showReadyMenuState(state);
            }
#endif
            if((left&&!oldLeft)||(right&&!oldRight)) {
#ifdef SPARKPAW_CAMPAIGN
                if(optionRow) {
                    *startSection=(*startSection==CAMPAIGN_START_STORM_RUINS)?
                        CAMPAIGN_START_STORMRAIL:CAMPAIGN_START_STORM_RUINS;
                } else
#endif
                {
                    *secondaryAction=(*secondaryAction==SECONDARY_BUTTON_JUMP)?
                                     SECONDARY_BUTTON_FIRE:SECONDARY_BUTTON_JUMP;
                }
#ifdef SPARKPAW_CAMPAIGN
                state=readyCampaignOptionsState(*secondaryAction,
                                                 *startSection,optionRow);
#else
                state=(*secondaryAction==SECONDARY_BUTTON_JUMP)?
                      READY_MENU_STATE_JUMP:READY_MENU_STATE_FIRE;
#endif
                showReadyMenuState(state);
            }
            if(fire&&!oldFire) {
                state=READY_MENU_STATE_START;
#ifdef SPARKPAW_CAMPAIGN
                optionRow=0;
#endif
                showReadyMenuState(state);
            }
        }
        oldUp=up; oldDown=down; oldLeft=left; oldRight=right; oldFire=fire;
    }
}

UWORD *titleCopperList(void) { return copper[currentCopper]; }

void titleFadeOut(void)
{
    if(displayed&&assetsLevelComplete()->bitmap) {
        UWORD frame;
        for(frame=1;frame<=FADE_FRAMES;frame++) {
            stagePalette(assetsLevelComplete(),(UWORD)(
                ((ULONG)(FADE_FRAMES-frame)*256)/FADE_FRAMES));
            waitOwnedDisplayFrame();
        }
    } else if(displayed&&assetsLevelReady()->bitmap) {
        UWORD frame;
        for(frame=1;frame<=FADE_FRAMES;frame++) {
            stagePalette(assetsLevelReady(),(UWORD)(
                ((ULONG)(FADE_FRAMES-frame)*256)/FADE_FRAMES));
            waitOwnedDisplayFrame();
        }
        assetsUnloadLevelReadyMenu();
        assetsUnloadLevelReady();
    } else if(displayed&&assetsLevelLoading()->bitmap)
        fadeTo(assetsLevelLoading(),FALSE);
}

const char *titleFailureReason(void) { return failureReason; }
ULONG titleChipFree(void) { return chipFree; }
ULONG titleChipLargest(void) { return chipLargest; }

void titleWaitFrames(UWORD frames)
{
    while((ULONG)(GfxBase->VBCounter-titleStartFrame)<frames) WaitTOF();
}

void titleRestoreSystemView(void)
{
    if(!displayed) return;
    WaitTOF();
    hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
    if(previousView) LoadView(previousView);
    hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|savedDma;
    WaitTOF(); WaitTOF();
    displayed=FALSE;
}

void titleRelease(void)
{
    UBYTE index;
    displayed=FALSE;
    if(readyMenuBack) {
        FreeBitMap(readyMenuBack);
        readyMenuBack=NULL;
    }
    for(index=0;index<2;index++) if(scoreBuffers[index]) {
        FreeBitMap(scoreBuffers[index]); scoreBuffers[index]=NULL;
    }
    assetsUnloadTitle(); assetsUnloadStoryIntro();
    assetsUnloadLevelLoading(); assetsUnloadLevelCharging();
    assetsUnloadLevelReadyMenu(); assetsUnloadLevelReady();
    assetsUnloadScoreGlyphs(); assetsUnloadLevelComplete();
    for(index=0;index<2;index++) {
        if(copper[index]) {
            FreeMem(copper[index],COPPER_WORDS*sizeof(UWORD));
            copper[index]=NULL;
        }
    }
}
