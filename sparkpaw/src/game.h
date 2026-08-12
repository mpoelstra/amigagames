#ifndef SPARKPAW_GAME_H
#define SPARKPAW_GAME_H

#include <exec/types.h>

struct GameState {
    LONG cameraX;
    LONG frameCounter;
    ULONG enemySeed;
    UBYTE lives;
    UBYTE diamonds;
};

#define GAME_START_LIVES 3
#define GAME_MAX_LIVES 9
#define GAME_DIAMONDS_PER_LIFE 50

void gameInit(ULONG enemySeed);
void gameUpdate(void);
const struct GameState *gameState(void);

#endif
