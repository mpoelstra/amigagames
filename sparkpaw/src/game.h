#ifndef SPARKPAW_GAME_H
#define SPARKPAW_GAME_H

#include <exec/types.h>

struct GameState {
    LONG cameraX;
    LONG frameCounter;
};

void gameInit(void);
void gameUpdate(void);
const struct GameState *gameState(void);

#endif