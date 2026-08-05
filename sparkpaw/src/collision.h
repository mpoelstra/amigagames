#ifndef SPARKPAW_COLLISION_H
#define SPARKPAW_COLLISION_H

#include <exec/types.h>

BOOL collisionLoad(void);
BOOL collisionSolidAt(WORD x,WORD y);
BOOL collisionSolidHorizontal(WORD left,WORD right,WORD y);
BOOL collisionSolidVertical(WORD x,WORD top,WORD bottom);

#endif