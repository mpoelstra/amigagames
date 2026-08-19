#include <assert.h>
#include "../src/enemy_vertical_order.h"

int main(void)
{
    short y[4]={176,50,112,50};
    unsigned char order[4];
    enemyVerticalOrder(y,order,4);
    assert(order[0]==1);
    assert(order[1]==3);
    assert(order[2]==2);
    assert(order[3]==0);
    return 0;
}
