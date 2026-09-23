#ifndef SPARKPAW_SPILLWING_H
#define SPARKPAW_SPILLWING_H
#include "enemies.h"
void spillwingInit(struct Enemy *enemy);
void spillwingUpdate(struct Enemy *enemy,WORD cameraX,WORD playerCenterX);
WORD spillwingLeft(const struct Enemy *enemy);
WORD spillwingRight(const struct Enemy *enemy);
BOOL spillwingHit(const struct Enemy *enemy,WORD x,WORD y);
#endif
