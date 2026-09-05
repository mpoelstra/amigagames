#include <assert.h>
#include <stdio.h>

#include "../src/stormrail_contract.h"

int main(void)
{
    long x=72,y=132;
    int frame;
    unsigned char targetCanary[2][2]={{0x5a,0xa5},{0x5a,0xa5}};

    for(frame=0;frame<1200;frame++) {
        int target=frame&1;
        long dx=(frame<300)?-3:(frame<600)?3:0;
        long dy=(frame<450)?3:-3;
        x=stormrailClamp(x+dx,STORMRAIL_FLIGHT_MIN_X,
                         STORMRAIL_FLIGHT_MAX_X);
        y=stormrailClamp(y+dy,STORMRAIL_FLIGHT_MIN_Y,
                         STORMRAIL_FLIGHT_MAX_Y);
        assert(x>=STORMRAIL_FLIGHT_MIN_X&&x<=STORMRAIL_FLIGHT_MAX_X);
        assert(y>=STORMRAIL_FLIGHT_MIN_Y&&y<=STORMRAIL_FLIGHT_MAX_Y);
        assert(stormrailVerticalRectSafe(y+STORMRAIL_FLIGHT_BOB_Y_OFFSET,
                                         STORMRAIL_FLIGHT_BOB_H));
        assert(targetCanary[target][0]==0x5a);
        assert(targetCanary[target][1]==0xa5);
    }
    assert(stormrailClamp(STORMRAIL_FLIGHT_MAX_Y-1+3,
                          STORMRAIL_FLIGHT_MIN_Y,
                          STORMRAIL_FLIGHT_MAX_Y)==STORMRAIL_FLIGHT_MAX_Y);
    assert(!stormrailVerticalRectSafe(STORMRAIL_PLAYFIELD_H-
                                      STORMRAIL_FLIGHT_BOB_H+1,
                                      STORMRAIL_FLIGHT_BOB_H));
    assert(stormrailVerticalRectSafe(STORMRAIL_PLAYFIELD_H-
                                     STORMRAIL_FLIGHT_BOB_H,
                                     STORMRAIL_FLIGHT_BOB_H));
    assert(stormrailRearScroll(0)==0);
    assert(stormrailRearScroll((unsigned long)STORMRAIL_REAR_LOOP_W*4-1)==767);
    assert(stormrailRearScroll((unsigned long)STORMRAIL_REAR_LOOP_W*4)==0);
    assert(stormrailRearScroll((unsigned long)STORMRAIL_REAR_LOOP_W*12+44)==11);
    for(frame=0;frame<20000;frame++) {
        unsigned long distance=(unsigned long)frame*4;
        long presentation=stormrailPresentationX(distance);
        assert(presentation>=0&&presentation<STORMRAIL_FLIGHT_PRESENTATION_W);
    }
    assert(stormrailPresentationX(32764)==508);
    assert(stormrailPresentationX(32768)==0);
    assert(stormrailReentryCorrectionActive(0,STORMRAIL_REENTRY_X));
    assert(stormrailReentryCorrectionActive(STORMRAIL_CONTROL_DELAY-1,4));
    assert(!stormrailReentryCorrectionActive(STORMRAIL_CONTROL_DELAY,4));
    assert(!stormrailReentryCorrectionActive(64,4));
    assert(stormrailFlightWorldX(137,STORMRAIL_BOARDING_SCREEN_X)-137==
           STORMRAIL_BOARDING_SCREEN_X);
    assert(STORMRAIL_MAX_ENEMIES==5);
    assert(STORMRAIL_MAX_PLAYER_SHOTS==5);
    assert(STORMRAIL_MAX_HOSTILE_SHOTS==4);
    assert(STORMRAIL_MAX_REWARDS>=3&&STORMRAIL_MAX_REWARDS<=5);
    assert(stormrailFinaleAttackEnabled(120,STORMRAIL_FINALE_ATTACK_FAN));
    assert(!stormrailFinaleAttackEnabled(120,STORMRAIL_FINALE_ATTACK_HUNTER));
    assert(stormrailFinaleAttackChargeDue(12,STORMRAIL_FINALE_ATTACK_FAN));
    assert(stormrailRestoreHeart(2,6)==4);
    assert(stormrailRestoreHeart(3,6)==5);
    assert(stormrailRestoreHeart(5,6)==6);
    assert(stormrailRestoreHeart(6,6)==6);
    assert(STORMRAIL_HEART_RING_FIRST_DIAMOND_ID==
           STORMRAIL_HEART_PICKUP_ID+1);
    assert(STORMRAIL_MAX_OBSTACLES==6);
    assert(STORMRAIL_DEBRIS_EVENT_COUNT==48);
    assert(STORMRAIL_SLICE_END_DISTANCE-
           STORMRAIL_DEBRIS_LAST_EVENT_DISTANCE>=4000UL);
    assert(stormrailObstacleWidth(STORMRAIL_OBSTACLE_BIG)==48);
    assert(stormrailObstacleHeight(STORMRAIL_OBSTACLE_BIG)==40);
    assert(stormrailObstacleWidth(STORMRAIL_OBSTACLE_SHARD)==16);
    assert(stormrailObstacleHeight(STORMRAIL_OBSTACLE_SHARD)==16);
    assert(stormrailObstacleWidth(STORMRAIL_OBSTACLE_PILLAR)==32);
    assert(stormrailObstacleHeight(STORMRAIL_OBSTACLE_PILLAR)==40);
    assert(STORMRAIL_OBSTACLE_BIG_HP==6);
    assert(stormrailObstacleSafeBaseFrame(STORMRAIL_OBSTACLE_BIG,4)==2);
    assert(stormrailObstacleSafeBaseFrame(STORMRAIL_OBSTACLE_SHARD,5)==4);
    assert(stormrailObstacleSafeBaseFrame(STORMRAIL_OBSTACLE_PILLAR,2)==0);
    assert(!stormrailObstacleHittable(297,STORMRAIL_OBSTACLE_BIG));
    assert(stormrailObstacleHittable(296,STORMRAIL_OBSTACLE_BIG));
    assert(!stormrailObstacleHittable(305,STORMRAIL_OBSTACLE_SHARD));
    assert(stormrailObstacleHittable(304,STORMRAIL_OBSTACLE_SHARD));
    assert(!stormrailEnemyHittable(STORMRAIL_ENEMY_HITTABLE_X+1));
    assert(stormrailEnemyHittable(STORMRAIL_ENEMY_HITTABLE_X));
    assert(stormrailEnemyHittable(-STORMRAIL_ENEMY_W+1));
    assert(STORMRAIL_FORMATION0_DISTANCE>STORMRAIL_CONTROL_DELAY);
    assert(STORMRAIL_FORMATION_COUNT==8);
    assert(STORMRAIL_FORMATION1_DISTANCE>STORMRAIL_FORMATION0_DISTANCE+700);
    assert(STORMRAIL_FORMATION2_DISTANCE>STORMRAIL_FORMATION1_DISTANCE+700);
    assert(STORMRAIL_FORMATION3_DISTANCE>STORMRAIL_FORMATION2_DISTANCE+700);
    assert(STORMRAIL_FORMATION4_DISTANCE<STORMRAIL_OBSTACLE_DISTANCE-1000);
    assert(STORMRAIL_FORMATION5_DISTANCE>
           STORMRAIL_DEBRIS_LAST_EVENT_DISTANCE+700);
    assert(STORMRAIL_FREE_LINE4_DISTANCE>STORMRAIL_FORMATION5_DISTANCE);
    assert(STORMRAIL_FORMATION6_DISTANCE>STORMRAIL_FREE_LINE4_DISTANCE+500);
    assert(STORMRAIL_FORMATION7_DISTANCE>STORMRAIL_FORMATION6_DISTANCE+500);
    assert(STORMRAIL_SLICE_END_DISTANCE>STORMRAIL_FORMATION7_DISTANCE+1000);
    assert(STORMRAIL_GATE5_FREE_FIRST_ID==STORMRAIL_HEART_RING_FIRST_DIAMOND_ID+3);
    assert(STORMRAIL_PATTERN_REWARD_FIRST_ID==STORMRAIL_GATE5_FREE_FIRST_ID+4);
    assert(STORMRAIL_PATTERN_REWARD_FIRST_ID+
           (STORMRAIL_FORMATION_COUNT-4)*4-1<64);
    assert(STORMRAIL_TAP_COOLDOWN==6);
    assert(STORMRAIL_HOLD_DELAY==10);
    assert(STORMRAIL_HOLD_INTERVAL==9);
    assert(STORMRAIL_CADENCE_END-STORMRAIL_CADENCE_START==1000);
    assert(STORMRAIL_FREE_LINE0_DISTANCE>STORMRAIL_FORMATION0_DISTANCE);
    assert(STORMRAIL_FREE_LINE1_DISTANCE>STORMRAIL_FORMATION1_DISTANCE);
    assert(STORMRAIL_FREE_LINE2_DISTANCE>STORMRAIL_FORMATION2_DISTANCE);
    assert(STORMRAIL_FREE_LINE3_DISTANCE>STORMRAIL_FORMATION3_DISTANCE);
    assert(stormrailFireDue(0,0));
    for(frame=0;frame<STORMRAIL_HOLD_DELAY;frame++)
        assert(!stormrailFireDue((unsigned long)frame,1));
    assert(stormrailFireDue(STORMRAIL_HOLD_DELAY,1));
    assert(!stormrailFireDue(STORMRAIL_HOLD_DELAY+1,1));
    assert(stormrailFireDue(STORMRAIL_HOLD_DELAY+STORMRAIL_HOLD_INTERVAL,1));
    {
        unsigned char hp[STORMRAIL_FINALE_ACTOR_COUNT]={120};
        unsigned long tick,lastFireTick=0;
        int hasFired=0,launches=0,lastActor=-1;
        assert(STORMRAIL_SLICE_END_DISTANCE==15500UL);
        assert(STORMRAIL_FINALE_ACTOR_COUNT==1);
        assert(STORMRAIL_FINALE_VOLLEY_SHOTS==3);
        assert(stormrailFinaleActors[0].width==80);
        assert(stormrailFinaleActors[0].height==46);
        for(tick=0;tick<384;tick++) {
            short bossX=stormrailFinaleActorX(tick,STORMRAIL_FINALE_HARRIER);
            short bossY=stormrailFinaleActorY(tick,STORMRAIL_FINALE_HARRIER);
            assert(bossX>=168&&bossX<=207);
            assert(bossY>=53&&bossY<=108);
            assert(bossX+stormrailFinaleActors[0].width<=288);
        }
        assert(stormrailFinaleActors[0].awardId==60);
        assert(STORMRAIL_FINALE_CONTACT_DAMAGE==1);
        assert(!stormrailFinalePlayerContact(100,81,188,81,0));
        assert(stormrailFinalePlayerContact(118,81,188,81,0));
        assert(stormrailFinaleContactResolveX(188)==118);
        assert(!stormrailFinaleAllDestroyed(hp));
        hp[0]=0;
        assert(stormrailFinaleAllDestroyed(hp));

        for(tick=0;tick<384;tick++) {
            unsigned char attack;
            int due=0;
            for(attack=0;attack<STORMRAIL_FINALE_ATTACK_COUNT;attack++) {
                if(stormrailFinaleAttackTelegraphing(tick,attack))
                    assert(!stormrailFinaleAttackDue(tick,attack));
                if(stormrailFinaleAttackDue(tick,attack)) {
                    assert(!due);
                    due=1;
                    assert(stormrailFinaleMayFire(tick,lastFireTick,hasFired));
                    if(hasFired) assert(tick-lastFireTick>=12);
                    lastFireTick=tick;
                    hasFired=1;
                    lastActor=stormrailFinaleAttacks[attack].actor;
                    launches++;
                }
            }
        }
        assert(launches==9);
        assert(lastActor==STORMRAIL_FINALE_HARRIER);
        assert(stormrailFinaleAttacks[0].kind==STORMRAIL_FINALE_ATTACK_FAN);
        assert(stormrailFinaleAttacks[1].kind==STORMRAIL_FINALE_ATTACK_HUNTER);
        assert(STORMRAIL_FINALE_HUNTER_HP==60);
        assert(stormrailFinaleAttackShot(108,1)==1);
        assert(stormrailFinaleAttackShot(120,1)==2);
        assert(stormrailFinaleVolleyDy[0]==-2);
        assert(stormrailFinaleVolleyDy[1]==0);
        assert(stormrailFinaleVolleyDy[2]==2);
        assert(stormrailFinaleApproach(70,88,2)==72);
        assert(stormrailFinaleApproach(90,88,2)==88);
        assert(stormrailFinaleApproach(88,88,2)==88);
        assert(STORMRAIL_MAX_HOSTILE_SHOTS==4);
        assert(STORMRAIL_MAX_PLAYER_SHOTS==5);
    }
    puts("PASS: Stormrail baseline and data-driven Gate-6 finale contract");
    return 0;
}
