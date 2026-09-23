"""Execute real candidate state, collision, player physics and projectile sweep."""
from pathlib import Path
import subprocess, tempfile, sys
ROOT=Path(__file__).resolve().parents[1]
subprocess.run([sys.executable,str(ROOT/'tools/build_drowned_slice_assets.py')],check=True)
with tempfile.TemporaryDirectory() as directory:
    p=Path(directory)
    for folder in ('exec','dos','proto'): (p/folder).mkdir()
    (p/'exec/types.h').write_text('''#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stddef.h>
typedef int BOOL; typedef int8_t BYTE; typedef uint8_t UBYTE;
typedef int16_t WORD; typedef uint16_t UWORD; typedef int32_t LONG;
typedef uint32_t ULONG; typedef void *BPTR;
#define TRUE 1
#define FALSE 0
#endif
''')
    (p/'dos/dos.h').write_text('#define MODE_OLDFILE 1005\n')
    (p/'proto/dos.h').write_text('#include <exec/types.h>\nBPTR Open(const char*,LONG); LONG Read(BPTR,void*,LONG); LONG Close(BPTR);\n')
    (p/'test.c').write_text(r'''
#include <assert.h>
#include <stdio.h>
#include "drowned_slice.h"
#include "collision.h"
#include "player.h"
#include "projectiles.h"
#include "level_data.h"
BPTR Open(const char *name,LONG mode) { return fopen("build/drowned-slice/assets/drowned-collision.bin","rb"); }
LONG Read(BPTR f,void *p,LONG n) { return fread(p,1,n,f); }
LONG Close(BPTR f) { return fclose(f); }
void platformReadGameKeys(BOOL*a,BOOL*b,BOOL*c,BOOL*d,BOOL*e) {*a=*b=*c=*d=*e=0;}
BOOL platformSecondaryButtonHeld(void) { return 0; }
static void sound(void) {}
static void resetAt(int x,int y,int speed) {
    struct PlayerState *s; playerInit(); s=(struct PlayerState *)playerState();
    s->x=x*256; s->y=y*256; s->vx=speed; s->grounded=1;
}
static int jumpAcross(int start,int y,int destination) {
    int t; resetAt(start,y,650);
    for(t=0;t<80;t++) {
        playerUpdatePhysics(0,1,0,t==0);
        if((playerState()->y>>8)>165) return 0;
        if((playerState()->x>>8)>=destination&&playerState()->grounded) return 1;
    }
    return 0;
}
static void gateTruthTable(void) {
    static const int lift[12]={0,2,5,10,16,23,31,39,47,54,59,61};
    int tick,xx,yy;
    drownedReset();drownedSetView(600);
    for(tick=0;tick<=44;tick++) {
        int f=tick==0?9:(tick<10?10:11+((tick>43?43:tick)-10)/3);
        int bottom=197-(f<11?0:lift[f-11]);
        for(xx=-1;xx<=961;xx++) for(yy=-1;yy<=257;yy++) {
            int expected=(xx>=800&&xx<832&&yy>=120&&yy<136)||
                (tick<43&&xx>=800&&xx<832&&yy>=128&&yy<bottom);
            assert(drownedGateSolid(xx,yy)==expected);
        }
        assert(!drownedGateSolid(-32768,120));
        assert(!drownedGateSolid(32767,120));
        assert(!drownedGateSolid(800,-32768));
        assert(!drownedGateSolid(800,32767));
        if(!tick) drownedPanelHit(768,170); else drownedTick();
    }
    drownedReset();drownedSetView(600);
}
int main(void) {
    gateTruthTable();
    int t,closed; WORD x,l,top,r,b;
    drownedReset();drownedSetView(600); assert(collisionLoad());
    assert(collisionSolidAt(128,160)&&!collisionSolidAt(127,160));
    assert(!collisionSolidAt(240,207)&&collisionSolidAt(239,200));
    assert(jumpAcross(190,121,326));
    assert(jumpAcross(560,121,696));
    /* The optional platform is reachable from the dry floor. */
    resetAt(82,161,650);
    for(t=0;t<40;t++) { playerUpdatePhysics(0,1,0,t==0); if(playerState()->grounded) break; }
    assert((playerState()->y>>8)==121&&(playerState()->x>>8)>=104);
    drownedReset();drownedSetView(600);
    for(t=0;t<200;t++) {
        assert(drownedJetFrame()==(t<100?0:t<130?((t/5)&1?8:1):t<135?2:t<185?3+((t-135)/4)%4:t<191?7:8));
        assert(drownedJetTouches(460,150,467,190)==(t>=135&&t<185));
        assert(!drownedJetTouches(468,150,480,190)); drownedTick();
    }
    assert(drownedJetFrame()==0);
    assert(drownedPanelSweep(730,810,174,&x)&&x==768);
    assert(drownedPanelSweep(810,730,174,&x)&&x==783);
    assert(!drownedPanelSweep(730,810,159,&x));
    /* Real projectile advances from the muzzle and unlatches the panel. */
    projectilesInit(); projectilesSpawn(704,161,0,0,0,sound);
    for(t=0;t<8;t++) projectilesUpdate(600,collisionSolidAt,collisionFirstSolidOnSweep,
        drownedPanelHit,drownedPanelSweep,sound,sound);
    assert(drownedGateFrame()==10);
    for(t=0;t<42;t++) {
        static const int lifts[12]={0,2,5,10,16,23,31,39,47,54,59,61};
        int f=drownedGateFrame(),bottom=197-(f<11?0:lifts[f-11]);
        int yy,end,ref,probe;
        assert(collisionSolidAt(800,160)==(160<bottom));
        /* Tile-span queries must retain exact pixel behaviour as the door
           bottom crosses non-tile-aligned positions. */
        for(yy=110;yy<205;yy++) {
            end=yy+38;ref=0;
            for(probe=yy;probe<=end;probe++) if(collisionSolidAt(800,probe)) ref=1;
            assert(collisionSolidVertical(800,yy,end)==ref);
        }
        drownedTick();
    }
    assert(!collisionSolidAt(800,160)&&drownedGateFrame()==22);
    assert(drownedPanelHit(768,170)==0);
    resetAt(740,161,650);
    for(t=0;t<60;t++) playerUpdatePhysics(0,1,0,0);
    assert((playerState()->x>>8)>840);
    /* Permanent housing survives opening, with exact non-tile y120 edge. */
    assert(collisionSolidAt(800,120)&&collisionSolidAt(831,135));
    assert(!collisionSolidAt(800,119)&&!collisionSolidAt(800,136));
    assert(collisionSolidVertical(810,115,123));
    assert(collisionFirstSolidOnSweep(780,850,124,&x)&&x==800);
    assert(collisionFirstSolidOnSweep(850,780,124,&x)&&x==831);
    for(l=780;l<840;l++) for(top=110;top<145;top++) {
        int ref=0,probe;
        for(probe=l;probe<=l+23;probe++) if(collisionSolidAt(probe,top)) ref=1;
        assert(collisionSolidHorizontal(l,l+23,top)==ref);
        ref=0;
        for(probe=top;probe<=top+20;probe++) if(collisionSolidAt(l,probe)) ref=1;
        assert(collisionSolidVertical(l,top,top+20)==ref);
    }
    /* Real jump: visible 48px head stays below y136; no damage, normal fall. */
    resetAt(804,161,0);
    {
        int low=161,health=playerState()->health;
        for(t=0;t<60;t++) {
            playerUpdatePhysics(0,0,0,t==0);
            if((playerState()->y>>8)<low) low=playerState()->y>>8;
        }
        assert(low==144&&playerState()->grounded&&(playerState()->y>>8)==161);
        assert(playerState()->health==health);
    }
    /* Side entry blocked even when only the visible head meets the beam. */
    resetAt(772,140,650);playerUpdatePhysics(0,1,0,0);
    assert((playerState()->x>>8)==772&&playerState()->wallBlocked);
    resetAt(828,140,-650);playerUpdatePhysics(1,0,0,0);
    assert((playerState()->x>>8)==828&&playerState()->wallBlocked);
    /* Top of housing is an ordinary solid support surface. */
    resetAt(804,75,0);
    for(t=0;t<20;t++) playerUpdatePhysics(0,0,0,0);
    assert(playerState()->grounded&&(playerState()->y>>8)==81);
    resetAt(804,161,0);playerUpdatePhysics(0,0,1,0);
    assert(playerState()->crouching);playerUpdatePhysics(0,0,0,0);
    assert(!playerState()->crouching);
    drownedReset();drownedSetView(600); assert(collisionSolidAt(800,160));
    assert(collisionSolidAt(800,120));
    resetAt(740,161,650);
    for(t=0;t<60;t++) playerUpdatePhysics(0,1,0,0);
    assert((playerState()->x>>8)==772);
    puts("PASS: actual physics reaches both banks/platform; projectile opens gate; damage and reset boundaries");
    return 0;
}
''')
    subprocess.run(['cc','-std=c99','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_WORLD_W=960',
        '-I'+str(p),'-I'+str(ROOT/'src'),str(p/'test.c'),
        *[str(ROOT/'src'/name) for name in ('drowned_slice.c','collision.c','player.c','projectiles.c')],
        '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
