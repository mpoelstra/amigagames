#include "mix.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "mix_reference.h"
static uint32_t seed=0x53706172U;
static unsigned randomValue(void){seed=seed*1664525U+1013904223U;return seed>>16;}
static void parity(void)
{
 Mixer m,ref;Effect f[FX_COUNT];int8_t samples[FX_COUNT][601];
 int8_t actual[603],expected[603];unsigned i,j,k,n,id;
 memset(&m,0,sizeof(m));memset(f,0,sizeof(f));
 for(i=0;i<FX_COUNT;i++) {
   for(j=0;j<601;j++)samples[i][j]=(int8_t)((int)(randomValue()%128)-64);
   f[i].data=samples[i];f[i].priority=(uint8_t)(randomValue()%12);
   f[i].cooldown=(uint8_t)(randomValue()%20);f[i].length=randomValue()%602;
 }
 f[0].priority=127;ref=m;
 for(k=0;k<40000;k++) {
   id=randomValue()%FX_COUNT;f[id].length=randomValue()%602;
   if(randomValue()%3==0)assert(mixRequest(&m,f,id)==mixRequest(&ref,f,id));
   if(randomValue()%2){mixField(&m);mixField(&ref);}
   n=randomValue()%602;
   memset(actual,0x55,sizeof(actual));memset(expected,0x55,sizeof(expected));
   mixRender(&m,actual+1,n);mixRenderReference(&ref,expected+1,n);
   assert(!memcmp(actual,expected,sizeof(actual)));assert(!memcmp(&m,&ref,sizeof(m)));
 }
 /* Explicitly exercise every tail around the actual 112-byte DMA boundary,
    including both orders, coincident endings, unaligned pointers and zero. */
 for(i=0;i<=114;i++)for(j=0;j<=114;j++) {
   memset(&m,0,sizeof(m));m.voice[0].data=samples[0]+1;
   m.voice[1].data=samples[1]+1;m.voice[0].remaining=i;m.voice[1].remaining=j;
   m.voice[0].priority=m.voice[1].priority=5;ref=m;
   for(k=0;k<3;k++) {
     mixRender(&m,actual,112);mixRenderReference(&ref,expected,112);
     assert(!memcmp(actual,expected,112));assert(!memcmp(&m,&ref,sizeof(m)));
   }
 }
 puts("block/reference parity: 40000 randomized operations and 13225 paired tails pass");
}
int main(void)
{
 Mixer m;Effect f[FX_COUNT];int8_t a[257],b[257],out[600];unsigned i;
 parity();
 memset(&m,0,sizeof(m));memset(f,0,sizeof(f));
 for(i=0;i<257;i++){a[i]=(int8_t)((i%128)-64);b[i]=63;}
 for(i=0;i<FX_COUNT;i++){f[i].data=b;f[i].length=257;f[i].priority=5;f[i].cooldown=3;}
 f[0].data=a;f[0].priority=127;f[0].cooldown=0;
 f[1].priority=9;f[8].priority=11;
 assert(mixRequest(&m,f,0));assert(mixRequest(&m,f,1));
 assert(!mixRequest(&m,f,2));assert(!mixRequest(&m,f,1));
 mixRender(&m,out,600);
 for(i=0;i<257;i++)assert(out[i]==a[i]+b[i]);
 for(i=257;i<600;i++)assert(out[i]==0);
 assert(m.completed[0]==1&&m.completed[1]==1);
 for(i=0;i<3;i++)mixField(&m);
 assert(mixRequest(&m,f,1));assert(mixRequest(&m,f,8));
 assert(!mixRequest(&m,f,1));mixRender(&m,out,600);
 assert(m.completed[1]==2);assert(!mixRequest(&m,f,FX_COUNT));
 puts("actual C: full tail, silence, overlap, cooldown, priority, replacement, bounds pass");
 return 0;
}
