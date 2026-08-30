#ifndef SPARKPAW_GAME_H
#define SPARKPAW_GAME_H

#include <exec/types.h>
#include "extra_life_contract.h"

struct GameState {
    LONG cameraX;
    LONG frameCounter;
    ULONG enemySeed;
    ULONG score;
    ULONG elapsedFields;
    ULONG lastFieldCounter;
    UWORD enemiesDefeated;
    UWORD diamondsCollected;
    UBYTE lives;
    UBYTE diamonds;
    UBYTE waterSplashTimer;
    UBYTE coreCollectTimer;
    UBYTE extraLifeState;
    WORD extraLifeY;
    WORD waterSplashX;
};

#define GAME_START_LIVES 3
#define GAME_MAX_LIVES 9
#define GAME_DIAMONDS_PER_LIFE 50

void gameInit(ULONG enemySeed);
void gameUpdate(void);
BOOL gameLevelComplete(void);
const struct GameState *gameState(void);

#endif
