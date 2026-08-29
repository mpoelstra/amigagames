#ifndef SPARKPAW_CAMERA_CONTRACT_H
#define SPARKPAW_CAMERA_CONTRACT_H

#define CAMERA_PLAYER_LEFT_ANCHOR 144

static long cameraCenteredTarget(long playerX)
{
    return playerX-CAMERA_PLAYER_LEFT_ANCHOR;
}

#endif
