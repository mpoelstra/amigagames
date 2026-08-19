#ifndef SPARKPAW_STRIDER_RESTORE_UNION_H
#define SPARKPAW_STRIDER_RESTORE_UNION_H

static int striderRestoreUnion(short ax,short ay,short bx,short by,
                               short width,short height,short *outX,
                               short *outY,short *outW,short *outH)
{
    short left,right,top,bottom;
    if(ax+width<=bx||bx+width<=ax||ay+height<=by||by+height<=ay)
        return 0;
    left=ax<bx?ax:bx; right=ax>bx?ax:bx;
    top=ay<by?ay:by; bottom=ay>by?ay:by;
    *outX=left; *outY=top;
    *outW=(short)(right+width-left);
    *outH=(short)(bottom+height-top);
    return 1;
}

#endif
