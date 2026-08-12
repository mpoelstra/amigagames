#ifndef SPARKPAW_ASSETS_H
#define SPARKPAW_ASSETS_H

#include <exec/types.h>
#include <graphics/gfx.h>

struct PlanarAsset {
    struct BitMap *bitmap;
    UWORD width,height,rowBytes;
    UBYTE depth,hasMask;
    UBYTE palette[64][3];
    UBYTE *mask;
};

BOOL assetsLoadTitle(void);
void assetsUnloadTitle(void);
BOOL assetsLoadLevelLoading(void);
void assetsUnloadLevelLoading(void);
BOOL assetsLoadLevelCharging(void);
void assetsUnloadLevelCharging(void);
BOOL assetsLoadGameplay(void);
void assetsUnloadGameplay(void);
const struct PlanarAsset *assetsTitle(void);
const struct PlanarAsset *assetsLevelLoading(void);
const struct PlanarAsset *assetsLevelCharging(void);
const struct PlanarAsset *assetsFrontClean(void);
const struct PlanarAsset *assetsRearWorld(void);
const struct PlanarAsset *assetsPlayerSprites(void);
const struct PlanarAsset *assetsEnemySprites(void);
const struct PlanarAsset *assetsStriderSprites(void);
const struct PlanarAsset *assetsHudBase(void);
const struct PlanarAsset *assetsHudHealth(void);
const struct PlanarAsset *assetsHudLives(void);
const struct PlanarAsset *assetsHudDiamonds(void);
const struct PlanarAsset *assetsCollectibleDiamond(void);

#endif
