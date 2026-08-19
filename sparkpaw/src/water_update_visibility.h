#ifndef SPARKPAW_WATER_UPDATE_VISIBILITY_H
#define SPARKPAW_WATER_UPDATE_VISIBILITY_H

static int waterUpdateVisible(int waterX,int cameraX,int screenWidth,
                              int waterWidth,int margin)
{
    return waterX+waterWidth>=cameraX-margin&&
           waterX<=cameraX+screenWidth+margin;
}

#endif
