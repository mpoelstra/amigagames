#include "drowned_governor.h"
#include "projectiles.h"
static UBYTE lock,damage;
static UWORD tick,exitTick;
static WORD camera;
static BOOL closing;
static const WORD xs[3]={256,512,768},ys[3]={136,88,136};
BOOL governorComplete(void){return lock==3&&exitTick==43;}
void governorReset(void){lock=damage=0;tick=exitTick=0;camera=0;closing=FALSE;}
void governorView(WORD x){camera=x-448-DROWNED_FINALE_OFFSET;}
void governorTick(void){if(lock==3&&exitTick<43)exitTick++;if(lock<3&&++tick==(lock==0?200:lock==1?175:155)){tick=0;closing=TRUE;}}
UBYTE governorFrame(UBYTE item){
 if(item<3){if(item<lock)return 5;if(item>lock)return 0;
  if(tick<45)return 1;if(tick<100)return 0;return 2+damage;}
 if(item==5)return lock==3?5:0;
 if(lock==3||item!=(lock<2?3:4))return 0;
 if(tick<45)return (tick/5)&1?8:1;
 if(tick<100)return 3+((tick-45)/4)%4;
 return 0;
}
BOOL governorSweep(WORD start,WORD end,WORD y,WORD *hit){
 UBYTE i;BOOL found=FALSE;WORD best=0;
 start-=448+DROWNED_FINALE_OFFSET;end-=448+DROWNED_FINALE_OFFSET;
 for(i=0;i<3;i++){
  WORD left=xs[i]+4,right=xs[i]+27,x;
  if(i<lock||left<camera+8||right>camera+311||y<ys[i]+16||y>=ys[i]+48)continue;
  if(start<=end){if(end<left||start>right)continue;x=start<left?left:start;}
  else {if(start<left||end>right)continue;x=start>right?right:start;}
  if(!found||(start<=end?x<best:x>best)){best=x;found=TRUE;}
 }
 if(found)*hit=best+448+DROWNED_FINALE_OFFSET;return found;
}
UBYTE governorHit(WORD x,WORD y){
 x-=448+DROWNED_FINALE_OFFSET;
 if(lock<3&&x>=xs[lock]+4&&x<=xs[lock]+27&&y>=ys[lock]+16&&y<ys[lock]+48&&
    xs[lock]+4>=camera+8&&xs[lock]+27<=camera+311&&tick>=100){
  if(++damage==3){lock++;damage=0;tick=0;closing=FALSE;}
 }
 return PROJECTILE_ENEMY_HIT;
}
BOOL governorHazard(WORD left,WORD top,WORD right,WORD bottom){
 WORD x=lock<2?384:640;
 left-=448+DROWNED_FINALE_OFFSET;right-=448+DROWNED_FINALE_OFFSET;
 return lock<3&&tick>=45&&tick<100&&right>=x+12&&left<=x+19&&bottom>=152&&top<192;
}
UBYTE governorExitFrame(void){
 if(!exitTick)return 9;
 if(exitTick<10)return 10;
 return 11+(exitTick-10)/3;
}
BOOL governorSolid(WORD x,WORD y){
 x-=DROWNED_FINALE_OFFSET;
 if(x<0||x>=1872||y<0)return TRUE;
 if(x>=192&&x<384&&y>=144&&y<160)return TRUE;
 x-=448;
 if(y>=200)return TRUE;
 if(x>=432&&x<576&&y>=152&&y<168)return TRUE;
 if(x>=864&&x<896){
  static const UBYTE lifts[12]={0,2,5,10,16,23,31,39,47,54,59,61};
  UBYTE frame=governorExitFrame();
  if(y>=120&&y<136)return TRUE;
  if(exitTick<43&&y>=128&&y<197)return y<197-(frame<11?0:lifts[frame-11]);
 }
 return FALSE;
}

UBYTE governorArtFrame(UBYTE item){
 if(item==5)return 34+governorFrame(item);
 if(item<3&&item==lock){
  if(closing&&tick<14)return 13+damage*7+(tick>>1);
  if(tick>=86&&tick<100)return 6+((tick-86)>>1);
 }
 return governorFrame(item);
}
