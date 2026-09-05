#include <assert.h>
#include <stdio.h>

#include "../src/stormrail_contract.h"

int main(void)
{
    unsigned char hp[STORMRAIL_FINALE_ACTOR_COUNT]={120};
    unsigned char phase=STORMRAIL_FINALE_PHASE_ARRIVAL;
    unsigned char gateOpen=0;
    unsigned char hostileActive=0;
    unsigned long distance=STORMRAIL_SLICE_END_DISTANCE;
    unsigned long tick,lastLaunch=0;
    int hasLaunched=0;
    short x=72,y=44;

    assert(stormrailFinaleActors[STORMRAIL_FINALE_HARRIER].width==80);
    assert(stormrailFinaleActors[STORMRAIL_FINALE_HARRIER].height==46);
    assert(stormrailFinaleRenderBoundsValid());
    assert(STORMRAIL_FINALE_RENDER_X_OFFSET==8);
    assert(STORMRAIL_FINALE_RENDER_Y_OFFSET==1);
    assert(STORMRAIL_FINALE_RENDER_W==63);
    assert(STORMRAIL_FINALE_RENDER_H==45);
    assert(!stormrailFinaleShouldBegin(15499UL,0));
    assert(stormrailFinaleShouldBegin(15500UL,0));
    assert(!stormrailFinaleShouldBegin(15500UL,1));
    assert(stormrailFinaleGateArrivalOffset(
        STORMRAIL_FINALE_PHASE_ARRIVAL,0)==32);
    assert(stormrailFinaleGateArrivalOffset(
        STORMRAIL_FINALE_PHASE_ARRIVAL,31)==1);
    assert(stormrailFinaleGateArrivalOffset(
        STORMRAIL_FINALE_PHASE_COMBAT,0)==0);
    assert(stormrailFinaleHarrierArrivalOffset(
        STORMRAIL_FINALE_PHASE_ARRIVAL,0)==128);
    assert(stormrailFinalePlayerMayMove(STORMRAIL_FINALE_PHASE_ARRIVAL));
    assert(STORMRAIL_FINALE_RETRY_PRESERVES_HP==1);
    assert(STORMRAIL_FINALE_RETRY_REPLAYS_ARRIVAL==0);
    assert(STORMRAIL_FINALE_CONTACT_DAMAGE==1);
    assert(!stormrailFinalePlayerContact(100,81,188,81,
        STORMRAIL_FINALE_HARRIER));
    assert(stormrailFinalePlayerContact(118,81,188,81,
        STORMRAIL_FINALE_HARRIER));
    assert(stormrailFinalePlayerContact(150,81,188,81,
        STORMRAIL_FINALE_HARRIER));
    assert(!stormrailFinalePlayerContact(150,0,188,81,
        STORMRAIL_FINALE_HARRIER));
    assert(stormrailFinaleContactResolveX(188)==118);
    assert(984+16<=1000);
    assert(stormrailRestoreFootprintWorldX(984,984)==976);
    assert(stormrailRestoreFootprintWidth(984,16)==32);
    assert(stormrailRestoreFootprintWorldX(984,984)+
           stormrailRestoreFootprintWidth(984,16)>1000);
    assert(STORMRAIL_FINALE_MIDLEVEL_LOADS==0);
    assert(stormrailFinaleGateResident(STORMRAIL_FINALE_PHASE_COMBAT,0));
    assert(!stormrailFinaleGateResident(STORMRAIL_FINALE_PHASE_OPENING,0));
    assert(stormrailFinaleAttackEnabled(61,STORMRAIL_FINALE_ATTACK_FAN));
    assert(!stormrailFinaleAttackEnabled(61,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackEnabled(60,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackChargeDue(76,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackTelegraphing(76,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackDue(96,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackDue(108,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackDue(120,STORMRAIL_FINALE_ATTACK_HUNTER));

    for(tick=0;tick<800;tick++) {
        unsigned char attack;
        assert(distance==15500UL);
        assert(stormrailFinaleActorX(tick,STORMRAIL_FINALE_HARRIER)>=168);
        assert(stormrailFinaleActorX(tick,STORMRAIL_FINALE_HARRIER)<=207);
        assert(stormrailFinaleActorY(tick,STORMRAIL_FINALE_HARRIER)>=53);
        assert(stormrailFinaleActorY(tick,STORMRAIL_FINALE_HARRIER)<=108);
        if(phase==STORMRAIL_FINALE_PHASE_ARRIVAL) {
            assert(!hostileActive);
            assert(stormrailFinaleGateArrivalOffset(phase,(unsigned short)tick)
                   >=0);
            if(tick>=STORMRAIL_FINALE_ARRIVAL_TICKS) {
                phase=STORMRAIL_FINALE_PHASE_COMBAT;
                tick=0;
            }
        } else if(phase==STORMRAIL_FINALE_PHASE_COMBAT) {
            assert(stormrailFinaleGateResident(phase,gateOpen));
            for(attack=0;attack<STORMRAIL_FINALE_ATTACK_COUNT;attack++)
                if(hp[stormrailFinaleAttacks[attack].actor]&&
                   stormrailFinaleAttackEnabled(
                       hp[STORMRAIL_FINALE_HARRIER],attack)&&
                   stormrailFinaleAttackDue(tick,attack)&&
                   stormrailFinaleMayFire(tick,lastLaunch,hasLaunched)) {
                    unsigned char emitted=
                        stormrailFinaleAttacks[attack].kind==
                            STORMRAIL_FINALE_ATTACK_FAN?
                            STORMRAIL_FINALE_VOLLEY_SHOTS:1;
                    assert(hostileActive+emitted<=STORMRAIL_MAX_HOSTILE_SHOTS);
                    hostileActive+=emitted;
                    lastLaunch=tick;
                    hasLaunched=1;
                }
            hostileActive=0;
            if(tick==240) hp[STORMRAIL_FINALE_HARRIER]=0;
            if(stormrailFinaleAllDestroyed(hp)) {
                phase=STORMRAIL_FINALE_PHASE_OPENING;
                tick=0;
                hostileActive=0;
            }
        } else if(phase==STORMRAIL_FINALE_PHASE_OPENING) {
            assert(!stormrailFinaleGateResident(phase,gateOpen));
            assert(!hostileActive);
            if(tick<STORMRAIL_FINALE_OPEN_TICKS) gateOpen=(unsigned char)tick;
            else {
                gateOpen=STORMRAIL_FINALE_OPEN_TICKS;
                phase=STORMRAIL_FINALE_PHASE_EXIT;
                tick=0;
            }
        } else if(phase==STORMRAIL_FINALE_PHASE_EXIT) {
            assert(!hostileActive);
            x=stormrailFinaleApproach(x,STORMRAIL_FINALE_CENTER_X,2);
            y=stormrailFinaleApproach(y,STORMRAIL_FINALE_CENTER_Y,2);
            if(x==STORMRAIL_FINALE_CENTER_X&&y==STORMRAIL_FINALE_CENTER_Y)
                break;
        }
    }
    assert(phase==STORMRAIL_FINALE_PHASE_EXIT);
    assert(gateOpen==STORMRAIL_FINALE_OPEN_TICKS);
    assert(x==STORMRAIL_FINALE_CENTER_X);
    assert(y==STORMRAIL_FINALE_CENTER_Y);
    assert(stormrailFinaleAllDestroyed(hp));
    assert(STORMRAIL_FINALE_RESPAWN_X<STORMRAIL_FINALE_CENTER_X);
    assert(STORMRAIL_FINALE_RESPAWN_Y==STORMRAIL_FINALE_CENTER_Y);
    assert(stormrailFinaleGateOpenOffset(0)==0);
    assert(stormrailFinaleGateOpenOffset(24)==16);
    assert(stormrailFinaleGateOpenOffset(47)==31);
    assert(stormrailFinaleGateOpenOffset(STORMRAIL_FINALE_OPEN_TICKS)==32);
    puts("PASS: Gate-6 latch, kill gate, fire stop, opening and centring contract");
    return 0;
}
