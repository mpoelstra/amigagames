#ifndef SPARKPAW_BEETLE_HITBOX_H
#define SPARKPAW_BEETLE_HITBOX_H

static BOOL beetleHitboxContains(WORD projectileX,WORD projectileY,
                                 WORD beetleX,WORD beetleY)
{
    return projectileX>=beetleX+2&&projectileX<=beetleX+ENEMY_W-3&&
           projectileY>=beetleY+7&&projectileY<=beetleY+ENEMY_H-1;
}

#endif
