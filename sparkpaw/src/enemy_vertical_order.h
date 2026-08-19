#ifndef SPARKPAW_ENEMY_VERTICAL_ORDER_H
#define SPARKPAW_ENEMY_VERTICAL_ORDER_H

static void enemyVerticalOrder(const short *y,unsigned char *order,
                               unsigned char count)
{
    unsigned char at,scan,key;
    for(at=0;at<count;at++) order[at]=at;
    for(at=1;at<count;at++) {
        key=order[at]; scan=at;
        while(scan>0&&y[key]<y[order[scan-1]]) {
            order[scan]=order[scan-1]; scan--;
        }
        order[scan]=key;
    }
}

#endif
