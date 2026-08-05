#ifndef SPARKPAW_ASSETS_H
#define SPARKPAW_ASSETS_H

#include <exec/types.h>
#include <graphics/gfx.h>

struct PlanarAsset {
    struct BitMap *bitmap;
    UWORD width,height,rowBytes;
    UBYTE depth,hasMask;
    UBYTE palette[32][3];
    UBYTE *mask;
};

BOOL assetsLoadGameplay(void);
void assetsUnloadGameplay(void);
const struct PlanarAsset *assetsFrontClean(void);
const struct PlanarAsset *assetsRearWorld(void);
const struct PlanarAsset *assetsPlayerSprites(void);
const struct PlanarAsset *assetsEnemySprites(void);

#endif