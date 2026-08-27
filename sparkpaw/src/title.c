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
#define CHARGING_PATCH_X 48
#define CHARGING_PATCH_Y 192
#define CHARGING_PATCH_W 224
#define CHARGING_PATCH_H 40
#define INTRO_PASSAGE_HOLD_FRAMES 240
#define INTRO_TEXT_SCROLL_ROWS 72
#define INTRO_SKIP_HOLD_FRAMES 30
#define READY_MENU_PATCH_Y 118
#define READY_MENU_PATCH_H 104
#define READY_MENU_PATCH_X 80
#define READY_MENU_PATCH_W 160
#define READY_MENU_STATE_START 0
#define READY_MENU_STATE_OPTIONS 1
#define READY_MENU_STATE_JUMP 2
#define READY_MENU_STATE_FIRE 3
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

BOOL titleShow(void)
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
    if(!assetsLoadStoryIntro(0)) {
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
        introDiagnosticEvent("load_failed",0); introDiagnosticClose();
#endif
        failureReason="six-plane intro proof asset load failed"; return FALSE;
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
    buildingIntroCopper=TRUE;
    buildCopper(assetsStoryIntro(),0,0);
    buildingIntroCopper=FALSE;
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
#endif
    fadeTo(assetsTitle(),TRUE);
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
    return TRUE;
}

BOOL titleShowLevelReady(void)
{
    UBYTE plane;
    LONG planeBytes;
    const struct PlanarAsset *loading=assetsLevelLoading();
    const struct PlanarAsset *ready;
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
       assetsLevelReadyMenu()->height!=READY_MENU_PATCH_H*4) {
        failureReason="ready screen asset has invalid geometry";
        assetsUnloadLevelReadyMenu();
        assetsUnloadLevelReady();
        return FALSE;
    }
    /* Gameplay is already fully prepared. Replace the resident status bitmap
       once while black; the temporary source stays in Fast RAM and never adds
       another displayable Chip bitmap. */
    fadeTo(loading,FALSE);
    WaitTOF();
    planeBytes=(LONG)loading->bitmap->BytesPerRow*loading->height;
    for(plane=0;plane<6;plane++)
        CopyMem(ready->bitmap->Planes[plane],loading->bitmap->Planes[plane],
                planeBytes);
    stagePalette(ready,0);
    fadeTo(ready,TRUE);
    return TRUE;
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
    UBYTE plane;
    UWORD row;
    LONG sourceRowBytes,targetRowBytes,planeOffset,targetOffset;
    const struct PlanarAsset *loading=assetsLevelLoading();
    const struct PlanarAsset *patches=assetsLevelReadyMenu();
    while(rasterLine()<252) { }
    sourceRowBytes=patches->bitmap->BytesPerRow;
    targetRowBytes=loading->bitmap->BytesPerRow;
    planeOffset=(LONG)state*READY_MENU_PATCH_H*sourceRowBytes;
    targetOffset=(LONG)READY_MENU_PATCH_Y*targetRowBytes+
                 READY_MENU_PATCH_X/8;
    for(plane=0;plane<6;plane++)
        for(row=0;row<READY_MENU_PATCH_H;row++)
            CopyMem(patches->bitmap->Planes[plane]+planeOffset+
                    (LONG)row*sourceRowBytes,
                    loading->bitmap->Planes[plane]+targetOffset+
                    (LONG)row*targetRowBytes,sourceRowBytes);
}

void titleRunLevelReadyMenu(enum SecondaryButtonAction *secondaryAction)
{
    UBYTE state=READY_MENU_STATE_START;
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
                state=(*secondaryAction==SECONDARY_BUTTON_JUMP)?
                      READY_MENU_STATE_JUMP:READY_MENU_STATE_FIRE;
                showReadyMenuState(state);
            }
        } else {
            if((left&&!oldLeft)||(right&&!oldRight)) {
                *secondaryAction=(*secondaryAction==SECONDARY_BUTTON_JUMP)?
                                 SECONDARY_BUTTON_FIRE:SECONDARY_BUTTON_JUMP;
                state=(*secondaryAction==SECONDARY_BUTTON_JUMP)?
                      READY_MENU_STATE_JUMP:READY_MENU_STATE_FIRE;
                showReadyMenuState(state);
            }
            if(fire&&!oldFire) {
                state=READY_MENU_STATE_START;
                showReadyMenuState(state);
            }
        }
        oldUp=up; oldDown=down; oldLeft=left; oldRight=right; oldFire=fire;
    }
}

UWORD *titleCopperList(void) { return copper[currentCopper]; }

void titleFadeOut(void)
{
    if(displayed&&assetsLevelReady()->bitmap) {
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
    assetsUnloadTitle(); assetsUnloadStoryIntro();
    assetsUnloadLevelLoading(); assetsUnloadLevelCharging();
    assetsUnloadLevelReadyMenu(); assetsUnloadLevelReady();
    for(index=0;index<2;index++) {
        if(copper[index]) {
            FreeMem(copper[index],COPPER_WORDS*sizeof(UWORD));
            copper[index]=NULL;
        }
    }
}
