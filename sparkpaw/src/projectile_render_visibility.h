#ifndef SPARKPAW_PROJECTILE_RENDER_VISIBILITY_H
#define SPARKPAW_PROJECTILE_RENDER_VISIBILITY_H

static int projectileRenderVisible(int x,int width,int cameraX,
                                   int screenWidth,int margin)
{
    return x+width>=cameraX-margin&&x<=cameraX+screenWidth+margin;
}

#endif
