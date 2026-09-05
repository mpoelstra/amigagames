#ifndef SPARKPAW_ASSETS_H
#define SPARKPAW_ASSETS_H

#include <exec/types.h>
#include <graphics/gfx.h>

struct PlanarAsset {
    struct BitMap *bitmap;
    UWORD width,height,rowBytes;
    UBYTE depth,hasMask;
    UBYTE cpuOnlyBitmap;
    UBYTE palette[64][3];
    UBYTE *mask;
};

BOOL assetsLoadTitle(void);
void assetsUnloadTitle(void);
BOOL assetsLoadStoryIntro(UWORD plate);
void assetsUnloadStoryIntro(void);
#ifdef SPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC
const char *assetsLoadFailureReason(void);
#endif
BOOL assetsLoadLevelLoading(void);
#ifdef SPARKPAW_MULTI_ADF
void assetsRetireOldLoading(void);
BOOL assetsLoadDiskPatch(UBYTE disk);
#endif
void assetsUnloadLevelLoading(void);
BOOL assetsLoadLevelCharging(void);
void assetsUnloadLevelCharging(void);
BOOL assetsLoadLevelReady(void);
void assetsUnloadLevelReady(void);
BOOL assetsLoadLevelReadyMenu(void);
void assetsUnloadLevelReadyMenu(void);
BOOL assetsLoadLevelComplete(void);
void assetsUnloadLevelComplete(void);
BOOL assetsLoadScoreGlyphs(void);
void assetsUnloadScoreGlyphs(void);
BOOL assetsLoadGameplay(void);
#ifdef SPARKPAW_CAMPAIGN
void assetsSetStormrailGameplay(BOOL active);
#endif
void assetsUnloadGameplayConversionSources(void);
void assetsUnloadGameplay(void);
const struct PlanarAsset *assetsTitle(void);
const struct PlanarAsset *assetsStoryIntro(void);
const struct PlanarAsset *assetsLevelLoading(void);
const struct PlanarAsset *assetsLevelCharging(void);
const struct PlanarAsset *assetsLevelReady(void);
const struct PlanarAsset *assetsLevelReadyMenu(void);
const struct PlanarAsset *assetsLevelComplete(void);
const struct PlanarAsset *assetsScoreGlyphs(void);
const struct PlanarAsset *assetsFrontClean(void);
const struct PlanarAsset *assetsRearWorld(void);
#ifdef SPARKPAW_STORMRAIL_PROOF
const struct PlanarAsset *assetsStormrailFlightRear(void);
#endif
const struct PlanarAsset *assetsPlayerSprites(void);
const struct PlanarAsset *assetsEnemySprites(void);
const struct PlanarAsset *assetsStriderSprites(void);
const struct PlanarAsset *assetsHudBase(void);
const struct PlanarAsset *assetsHudHealth(void);
const struct PlanarAsset *assetsHudLives(void);
const struct PlanarAsset *assetsHudDiamonds(void);
const struct PlanarAsset *assetsHudScore(void);
const struct PlanarAsset *assetsCollectibleDiamond(void);
#ifdef SPARKPAW_STORMRAIL_PROOF
const struct PlanarAsset *assetsStormrailHeart(void);
#endif
const struct PlanarAsset *assetsStormstoneCore(void);
const struct PlanarAsset *assetsExtraLife(void);
#ifdef SPARKPAW_STORMRAIL_PROOF
const struct PlanarAsset *assetsStormrailFamily(void);
const struct PlanarAsset *assetsStormrailObstacles(void);
void assetsReleaseStormrailApproach(void);
#endif

#endif
