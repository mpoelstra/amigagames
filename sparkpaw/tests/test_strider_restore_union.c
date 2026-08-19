#include <assert.h>
#include "../src/strider_restore_union.h"

int main(void)
{
    short x,y,w,h;
    assert(striderRestoreUnion(100,50,140,50,64,64,&x,&y,&w,&h));
    assert(x==100&&y==50&&w==104&&h==64);
    assert(striderRestoreUnion(140,50,100,54,64,64,&x,&y,&w,&h));
    assert(x==100&&y==50&&w==104&&h==68);
    assert(!striderRestoreUnion(100,50,164,50,64,64,&x,&y,&w,&h));
    assert(!striderRestoreUnion(100,50,100,114,64,64,&x,&y,&w,&h));
    return 0;
}
