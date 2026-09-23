#ifdef SPARKPAW_DROWNED_GOVERNOR
#include "drowned_governor.h"
#endif
#include "drowned_slice.h"
#ifdef SPARKPAW_DROWNED_PONTOON
#include "drowned_pontoon.h"
#endif
#include "level_data.h"
#include "projectiles.h"
#ifdef SPARKPAW_DROWNED_ENCOUNTER
#include "enemies.h"
#endif

#ifdef SPARKPAW_DROWNED_ROUTE
#include "drowned_route_layout.h"
#endif

static UWORD pressureTick,gateTick;
static BOOL panelVisible;
void drownedReset(void) {
#ifdef SPARKPAW_DROWNED_GOVERNOR
 governorReset();
#endif
#ifdef SPARKPAW_DROWNED_PONTOON
 drownedPontoonReset();
#endif
 pressureTick=0; gateTick=0; panelVisible=FALSE; }
#ifdef SPARKPAW_DROWNED_JOINED
void drownedRespawn(BOOL saved)
{
    BOOL opened=gateTick!=0;
    drownedReset();
    if(saved&&opened) gateTick=43;
}
#endif
void drownedSetView(WORD cameraX)
{
#ifdef SPARKPAW_DROWNED_GOVERNOR
 governorView(cameraX);
#endif
    /* Require the entire control panel plus a small reading margin on screen.
       Projectiles may travel beyond the viewport, but cannot operate unseen
       machinery. Cache this once per game update, not per sweep pixel. */
#if defined(SPARKPAW_DROWNED_SPILLWING) && !defined(SPARKPAW_DROWNED_JOINED)
    panelVisible=FALSE;
#else
    panelVisible=768>=cameraX+8&&784<=cameraX+320-8;
#endif
}
void drownedTick(void)
{
#ifdef SPARKPAW_DROWNED_GOVERNOR
 governorTick();
#endif
    if(++pressureTick==200) pressureTick=0;
    if(gateTick&&gateTick<43) gateTick++;
}
UBYTE drownedJetFrame(void)
{ if(pressureTick<100) return 0;
  if(pressureTick<130) return (pressureTick/5)&1?8:1;
  if(pressureTick<135) return 2;
  if(pressureTick<185) return (UBYTE)(3+((pressureTick-135)/4)%4);
  return pressureTick<191?7:8; }
UBYTE drownedGateFrame(void)
{ if(!gateTick) return 9;
  if(gateTick<10) return 10;
  return (UBYTE)(11+(gateTick-10)/3); }
BOOL drownedGateSolid(WORD x,WORD y)
{
#if defined(SPARKPAW_DROWNED_GOVERNOR) && !defined(SPARKPAW_DROWNED_FULL)
 return FALSE;
#endif
    static const UBYTE lifts[12]={0,2,5,10,16,23,31,39,47,54,59,61};
    UBYTE frame;
    WORD bottom;
    /* Most collision probes are nowhere near this mechanism. Reject those
       before calling frame selection or computing the moving leaf bottom. */
#if (defined(SPARKPAW_DROWNED_PONTOON) || defined(SPARKPAW_DROWNED_SPILLWING)) && !defined(SPARKPAW_DROWNED_JOINED)
    return FALSE;
#endif
    if(x<DROWNED_HEADER_LEFT||x>=DROWNED_HEADER_RIGHT) return FALSE;
    if(y>=DROWNED_HEADER_TOP&&y<DROWNED_HEADER_BOTTOM) return TRUE;
    if(gateTick>=43||y<128||y>=197) return FALSE;
    frame=drownedGateFrame();
    bottom=(WORD)(197-(frame<11?0:lifts[frame-11]));
    return y<bottom;
}
BOOL drownedHeaderOverlaps(WORD left,WORD top,WORD right,WORD bottom)
{
#if (defined(SPARKPAW_DROWNED_PONTOON) || defined(SPARKPAW_DROWNED_SPILLWING)) && !defined(SPARKPAW_DROWNED_JOINED)
 return FALSE;
#endif
#ifdef SPARKPAW_DROWNED_FULL
 if(right>=DROWNED_FINALE_OFFSET+1312&&left<DROWNED_FINALE_OFFSET+1344&&bottom>=120&&top<136)return TRUE;
#endif
 return right>=DROWNED_HEADER_LEFT&&left<DROWNED_HEADER_RIGHT&&
         bottom>=DROWNED_HEADER_TOP&&top<DROWNED_HEADER_BOTTOM; }
BOOL drownedJetTouches(WORD left,WORD top,WORD right,WORD bottom)
{
#ifdef SPARKPAW_DROWNED_GOVERNOR
#ifdef SPARKPAW_DROWNED_FULL
 if(governorHazard(left,top,right,bottom))return TRUE;
#else
 return governorHazard(left,top,right,bottom);
#endif
#endif
#if (defined(SPARKPAW_DROWNED_PONTOON) || defined(SPARKPAW_DROWNED_SPILLWING)) && !defined(SPARKPAW_DROWNED_JOINED)
    return FALSE;
#endif
    if(pressureTick<135||pressureTick>=185) return FALSE;
    if(right>=460&&left<=467&&bottom>=152&&top<192) return TRUE;
#ifdef SPARKPAW_DROWNED_ROUTE
    /* Same 32x64 jet patch, raised 24px with the platform. */
    if(right>=1804&&left<=1811&&bottom>=128&&top<168) return TRUE;
#endif
    return FALSE;
}
UBYTE drownedPanelHit(WORD x,WORD y)
{
    if(panelVisible&&!gateTick&&x>=768&&x<784&&y>=160&&y<184) {
        gateTick=1; return PROJECTILE_ENEMY_HIT;
    }
    return PROJECTILE_ENEMY_MISS;
}
BOOL drownedPanelSweep(WORD start,WORD end,WORD y,WORD *hitX)
{
    if(!panelVisible||gateTick||y<160||y>=184) return FALSE;
    if(start<=end) {
        if(end<768||start>783) return FALSE;
        *hitX=start<768?768:start;
    } else {
        if(start<768||end>783) return FALSE;
        *hitX=start>783?783:start;
    }
    return TRUE;
}

/* Isolated load encounter uses existing enemy art/AI; no campaign spawn edits. */
#ifdef SPARKPAW_DROWNED_ENCOUNTER
#ifndef SPARKPAW_DROWNED_ROUTE
static const struct EnemyPatrolSurface drownedSurfaces[]={
    {496,608,200},{328,448,200}
};
static const struct EnemySpawnCandidate drownedSpawns[]={
    {552,568,1,0,ENEMY_TYPE_CLOCKWORK_BEETLE,ENEMY_POLICY_RESPAWN,1},
    {360,376,-1,1,ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,ENEMY_POLICY_RESPAWN,1}
};
#endif
const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count)
{
#if defined(SPARKPAW_DROWNED_GOVERNOR) && !defined(SPARKPAW_DROWNED_FULL)
 static const struct EnemySpawnCandidate guards[]={
 {1072,1088,-1,0,ENEMY_TYPE_CLOCKWORK_BEETLE,ENEMY_POLICY_RESPAWN,1},
 {624,640,-1,1,ENEMY_TYPE_CLOCKWORK_BEETLE,ENEMY_POLICY_RESPAWN,1},
 {1120,1120,-1,2,2,ENEMY_POLICY_RESPAWN,1},
 {848,848,1,3,2,ENEMY_POLICY_RESPAWN,1},
 {120,136,1,4,ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,ENEMY_POLICY_RESPAWN,1},
 {304,320,-1,5,ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,ENEMY_POLICY_RESPAWN,1}};
 *count=6;return guards;
#endif
#ifdef SPARKPAW_DROWNED_ROUTE
#if defined(SPARKPAW_DROWNED_PONTOON) && !defined(SPARKPAW_DROWNED_FERRY)
    *count=0;return 0;
#endif
    *count=sizeof(drownedRouteSpawns)/sizeof(drownedRouteSpawns[0]);
    return drownedRouteSpawns;
#else
    *count=2; return drownedSpawns;
#endif
}
const struct EnemyPatrolSurface *levelEnemyPatrolSurface(UBYTE id)
{
#if defined(SPARKPAW_DROWNED_GOVERNOR) && !defined(SPARKPAW_DROWNED_FULL)
 static const struct EnemyPatrolSurface floors[]={{1024,1264,200},{560,784,200},{1024,1264,200},{736,1152,200},{96,416,200},{192,384,144}};
 return id<6?&floors[id]:0;
#endif
#ifdef SPARKPAW_DROWNED_ROUTE
    return id<sizeof(drownedRouteSurfaces)/sizeof(drownedRouteSurfaces[0])?&drownedRouteSurfaces[id]:0;
#else
    return id<2?&drownedSurfaces[id]:0;
#endif
}
UBYTE drownedEncounterHit(WORD x,WORD y)
{
    UBYTE hit=enemiesHitProjectile(x,y);
#ifdef SPARKPAW_DROWNED_GOVERNOR
#ifdef SPARKPAW_DROWNED_FULL
    return hit!=PROJECTILE_ENEMY_MISS?hit:(x>=DROWNED_FINALE_OFFSET?governorHit(x,y):drownedPanelHit(x,y));
#else
    return hit!=PROJECTILE_ENEMY_MISS?hit:governorHit(x,y);
#endif
#else
    return hit!=PROJECTILE_ENEMY_MISS?hit:drownedPanelHit(x,y);
#endif
}
BOOL drownedEncounterSweep(WORD start,WORD end,WORD y,WORD *hitX)
{
    WORD enemyX,panelX;
    BOOL enemy=enemiesFirstProjectileHitOnSweep(start,end,y,&enemyX);
#ifdef SPARKPAW_DROWNED_GOVERNOR
#ifdef SPARKPAW_DROWNED_FULL
    BOOL panel=start>=DROWNED_FINALE_OFFSET?governorSweep(start,end,y,&panelX):drownedPanelSweep(start,end,y,&panelX);
#else
    BOOL panel=governorSweep(start,end,y,&panelX);
#endif
#else
    BOOL panel=drownedPanelSweep(start,end,y,&panelX);
#endif
    if(!enemy&&!panel) return FALSE;
    if(enemy&&(!panel||(start<=end?enemyX<=panelX:enemyX>=panelX)))
        *hitX=enemyX;
    else *hitX=panelX;
    return TRUE;
}
#else
const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count)
{ *count=0; return 0; }
const struct EnemyPatrolSurface *levelEnemyPatrolSurface(UBYTE surfaceId)
{ return 0; }
#endif
#if defined(SPARKPAW_DROWNED_ROUTE)
const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count)
{
#if defined(SPARKPAW_DROWNED_GOVERNOR) && !defined(SPARKPAW_DROWNED_FULL)
 static const struct EnemyTraversalLink jumps[]={
 {4,4,1,96,100,132,138,320,-900,60},
 {4,4,-1,136,140,98,104,-320,-900,60},
 {5,5,1,224,228,261,267,320,-900,60},
 {5,5,-1,266,270,227,233,-320,-900,60}};
 *count=4;return jumps;
#endif
#if defined(SPARKPAW_DROWNED_SPILLWING) && !defined(SPARKPAW_DROWNED_JOINED)
 *count=0;return 0;
#else
 *count=sizeof(drownedRouteLinks)/sizeof(drownedRouteLinks[0]);return drownedRouteLinks;
#endif
}
#elif defined(SPARKPAW_DROWNED_JUMP_PROOF)
/* Bounded dry-ground audition: same surface in both directions. No extra
   enemies, geometry or forced water crossing while reviewing the new poses. */
static const struct EnemyTraversalLink drownedJumpLinks[]={
    {1,1,1,346,350,382,388,320,-900,60},
    {1,1,-1,386,390,349,354,-320,-900,60}
};
const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count)
{ *count=2; return drownedJumpLinks; }
#else
const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count)
{ *count=0; return 0; }
#endif
WORD levelWaterLeft(UBYTE index) {
#if defined(SPARKPAW_DROWNED_GOVERNOR) && !defined(SPARKPAW_DROWNED_FULL)
 return 3400;
#endif
#ifdef SPARKPAW_DROWNED_ROUTE
#if defined(SPARKPAW_DROWNED_PONTOON) && !defined(SPARKPAW_DROWNED_JOINED)
    return index<LEVEL_WATER_COUNT?160+index*80:-1;
#else
    return index<LEVEL_WATER_COUNT?drownedRouteWater[index]:-1;
#endif
#else
    return index==0?240:(index==1?608:-1);
#endif
}
BOOL levelWaterColumnAt(WORD x)
{
#if defined(SPARKPAW_DROWNED_SPILLWING) && !defined(SPARKPAW_DROWNED_FERRY)
    return FALSE;
#endif
#ifdef SPARKPAW_DROWNED_ROUTE
    UBYTE i;
    for(i=0;i<LEVEL_WATER_COUNT;i++)
        if(x>=levelWaterLeft(i)&&x<levelWaterLeft(i)+LEVEL_WATER_W)return TRUE;
    return FALSE;
#else
    return (x>=240&&x<320)||(x>=608&&x<688);
#endif
}
BOOL levelHazardColumnAt(WORD x) { return levelWaterColumnAt(x); }
BOOL levelPlayerInWater(WORD left,WORD right,WORD bottom)
{ return levelWaterColumnAt((left+right)>>1)&&bottom>=LEVEL_WATER_DEATH_Y; }
BOOL levelPlayerTouchesWater(WORD left,WORD right,WORD bottom)
{ return levelWaterColumnAt((left+right)>>1)&&bottom>=LEVEL_WATER_SPLASH_Y; }
BOOL levelPlayerFallsInDryGap(WORD left,WORD right,WORD bottom) { return FALSE; }
BOOL levelPlayerTouchesStormstoneCore(WORD left,WORD top,WORD right,WORD bottom)
{
#ifdef SPARKPAW_DROWNED_GOVERNOR
 return governorComplete()&&right>=1624+DROWNED_FINALE_OFFSET&&left<=1656+DROWNED_FINALE_OFFSET&&bottom>=120&&top<160;
#else
 return FALSE;
#endif
}
