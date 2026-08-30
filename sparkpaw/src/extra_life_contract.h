#ifndef SPARKPAW_EXTRA_LIFE_CONTRACT_H
#define SPARKPAW_EXTRA_LIFE_CONTRACT_H

#define EXTRA_LIFE_HIDDEN 0
#define EXTRA_LIFE_DROPPING 1
#define EXTRA_LIFE_READY 2
#define EXTRA_LIFE_COLLECTED 3
#define EXTRA_LIFE_X 3328
#define EXTRA_LIFE_TRIGGER_X 3248
#define EXTRA_LIFE_START_Y 0
#define EXTRA_LIFE_GROUND_Y 178
#define EXTRA_LIFE_W 16
#define EXTRA_LIFE_H 22

static long extraLifePlaneWordOffset(unsigned char plane,int y,
                                     int sourceWords)
{
    return (long)plane*EXTRA_LIFE_H*sourceWords+(long)y*sourceWords;
}

static unsigned char extraLifeShouldReveal(unsigned char state,
                                            int left,int right)
{
    return state==EXTRA_LIFE_HIDDEN&&((left+right)>>1)>=EXTRA_LIFE_TRIGGER_X;
}

static int extraLifeDropY(int y)
{
    y+=3;
    return y>EXTRA_LIFE_GROUND_Y?EXTRA_LIFE_GROUND_Y:y;
}

static unsigned char extraLifeTouches(int left,int top,int right,int bottom,
                                       int y)
{
    return right>=EXTRA_LIFE_X&&left<EXTRA_LIFE_X+EXTRA_LIFE_W&&
           bottom>=y&&top<y+EXTRA_LIFE_H;
}

#endif
