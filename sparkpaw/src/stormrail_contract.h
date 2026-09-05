#ifndef SPARKPAW_STORMRAIL_CONTRACT_H
#define SPARKPAW_STORMRAIL_CONTRACT_H

#define STORMRAIL_FLIGHT_MIN_X 4
#define STORMRAIL_FLIGHT_MAX_X 236
#define STORMRAIL_FLIGHT_MIN_Y 0
#define STORMRAIL_FLIGHT_MAX_Y 176
#define STORMRAIL_PLAYFIELD_W 320
#define STORMRAIL_PLAYFIELD_H 208
#define STORMRAIL_FLIGHT_BOB_Y_OFFSET 0
#define STORMRAIL_FLIGHT_BOB_W 80
#define STORMRAIL_FLIGHT_BOB_H 32
#define STORMRAIL_LAUNCH_BOB_H 46
#define STORMRAIL_FLIGHT_MUZZLE_X 77
#define STORMRAIL_FLIGHT_MUZZLE_Y 20
#define STORMRAIL_PILOT_SPRITE_X 28
#define STORMRAIL_PILOT_SPRITE_Y (-1)
#define STORMRAIL_REAR_LOOP_W 768
#define STORMRAIL_REAR_PARALLAX_SHIFT 2
#define STORMRAIL_FLIGHT_PRESENTATION_W 512
#define STORMRAIL_APPROACH_RETIRE_X 192
#define STORMRAIL_MODE_APPROACH 0
#define STORMRAIL_MODE_BOARDING 1
#define STORMRAIL_MODE_LAUNCH_OUT 2
#define STORMRAIL_MODE_FLIGHT 3
#define STORMRAIL_LAUNCH_OUT_FRAMES 24
#define STORMRAIL_FADE_STEPS 8
#define STORMRAIL_FADE_IN_FRAMES 8
#define STORMRAIL_REENTRY_X (-104)
#define STORMRAIL_BOARDING_SCREEN_X 180
#define STORMRAIL_PLAY_X 72
#define STORMRAIL_CONTROL_DELAY 32
#define STORMRAIL_MAX_ENEMIES 5
#define STORMRAIL_MAX_PLAYER_SHOTS 5
#define STORMRAIL_MAX_HOSTILE_SHOTS 4
#define STORMRAIL_MAX_REWARDS 4
#define STORMRAIL_REWARD_DIAMOND 0
#define STORMRAIL_REWARD_HEART 1
#define STORMRAIL_HEART_PICKUP_ID 34
#define STORMRAIL_HEART_RING_FIRST_DIAMOND_ID 35
#define STORMRAIL_HEART_PICKUP_DISTANCE 2600UL
#define STORMRAIL_FORMATION_COUNT 8
#define STORMRAIL_FORMATION0_DISTANCE 520UL
#define STORMRAIL_FORMATION1_DISTANCE 1450UL
#define STORMRAIL_FORMATION2_DISTANCE 2400UL
#define STORMRAIL_FORMATION3_DISTANCE 3400UL
#define STORMRAIL_FORMATION4_DISTANCE 4400UL
#define STORMRAIL_FORMATION5_DISTANCE 12150UL
#define STORMRAIL_FORMATION6_DISTANCE 13400UL
#define STORMRAIL_FORMATION7_DISTANCE 14100UL
#define STORMRAIL_FREE_LINE_COUNT 5
#define STORMRAIL_FREE_LINE0_DISTANCE 950UL
#define STORMRAIL_FREE_LINE1_DISTANCE 1950UL
#define STORMRAIL_FREE_LINE2_DISTANCE 2950UL
#define STORMRAIL_FREE_LINE3_DISTANCE 3950UL
#define STORMRAIL_FREE_LINE4_DISTANCE 12850UL
#define STORMRAIL_GATE5_FREE_FIRST_ID 38
#define STORMRAIL_PATTERN_REWARD_FIRST_ID 42
#define STORMRAIL_OBSTACLE_DISTANCE 5800UL
#define STORMRAIL_DEBRIS_EVENT_COUNT 48
#define STORMRAIL_DEBRIS_LAST_EVENT_DISTANCE 11330UL
#define STORMRAIL_SLICE_END_DISTANCE 15500UL
#define STORMRAIL_OBSTACLE_MAX_AGE 180
#define STORMRAIL_TAP_COOLDOWN 6
#define STORMRAIL_HOLD_DELAY 10
#define STORMRAIL_HOLD_INTERVAL 9
#define STORMRAIL_HIT_INVULN 36
#define STORMRAIL_CADENCE_START 400
#define STORMRAIL_CADENCE_END 1400
#define STORMRAIL_ENEMY_W 48
#define STORMRAIL_ENEMY_H 32
#define STORMRAIL_ENEMY_HITTABLE_X 296
#define STORMRAIL_MAX_OBSTACLES 6
#define STORMRAIL_OBSTACLE_BIG 0
#define STORMRAIL_OBSTACLE_SHARD 1
#define STORMRAIL_OBSTACLE_PILLAR 2
#define STORMRAIL_OBSTACLE_BIG_W 48
#define STORMRAIL_OBSTACLE_BIG_H 40
#define STORMRAIL_OBSTACLE_BIG_HITTABLE_X 296
#define STORMRAIL_OBSTACLE_SHARD_W 16
#define STORMRAIL_OBSTACLE_SHARD_H 16
#define STORMRAIL_OBSTACLE_PILLAR_W 32
#define STORMRAIL_OBSTACLE_PILLAR_H 40
#define STORMRAIL_OBSTACLE_BIG_HP 6
#define STORMRAIL_OBSTACLE_CONTACT_GRACE 12
#define STORMRAIL_OBSTACLE_SMALL_HP 2
#define STORMRAIL_OBSTACLE_PILLAR_HP 4
#define STORMRAIL_OBSTACLE_HIT_FLASH 4
#define STORMRAIL_HOSTILE_W 16
#define STORMRAIL_HOSTILE_H 8

#define STORMRAIL_FINALE_PHASE_ARRIVAL 0
#define STORMRAIL_FINALE_PHASE_COMBAT 1
#define STORMRAIL_FINALE_PHASE_OPENING 2
#define STORMRAIL_FINALE_PHASE_EXIT 3
#define STORMRAIL_FINALE_PHASE_COMPLETE 4
#define STORMRAIL_FINALE_ARRIVAL_TICKS 32
#define STORMRAIL_FINALE_ACTOR_COUNT 1
#define STORMRAIL_FINALE_ATTACK_COUNT 2
#define STORMRAIL_FINALE_VOLLEY_SHOTS 3
#define STORMRAIL_FINALE_ATTACK_FAN 0
#define STORMRAIL_FINALE_ATTACK_HUNTER 1
#define STORMRAIL_FINALE_HARRIER 0
#define STORMRAIL_FINALE_GATE_X 288
#define STORMRAIL_FINALE_OPEN_TICKS 48
#define STORMRAIL_FINALE_CENTER_X 112
#define STORMRAIL_FINALE_CENTER_Y 88
#define STORMRAIL_FINALE_RESPAWN_X 48
#define STORMRAIL_FINALE_RESPAWN_Y STORMRAIL_FINALE_CENTER_Y
#define STORMRAIL_FINALE_EXIT_X 328
#define STORMRAIL_FINALE_MIN_FIRE_GAP 12
#define STORMRAIL_FINALE_ATTACK_PERIOD 160
#define STORMRAIL_FINALE_TELEGRAPH_TICKS 20
#define STORMRAIL_FINALE_HUNTER_INTERVAL 12
#define STORMRAIL_FINALE_HUNTER_HP 60
#define STORMRAIL_FINALE_MIDLEVEL_LOADS 0
#define STORMRAIL_FINALE_RETRY_PRESERVES_HP 1
#define STORMRAIL_FINALE_RETRY_REPLAYS_ARRIVAL 0
#define STORMRAIL_FINALE_CONTACT_DAMAGE 1
#define STORMRAIL_FINALE_PLAYER_HIT_X 10
#define STORMRAIL_FINALE_PLAYER_HIT_Y 4
#define STORMRAIL_FINALE_PLAYER_HIT_W 60
#define STORMRAIL_FINALE_PLAYER_HIT_H 24
/* Renderer-only opaque bounds inside the unchanged 80x46 actor contract. */
#define STORMRAIL_FINALE_RENDER_X_OFFSET 8
#define STORMRAIL_FINALE_RENDER_Y_OFFSET 1
#define STORMRAIL_FINALE_RENDER_W 63
#define STORMRAIL_FINALE_RENDER_H 45

struct StormrailFinaleActorContract {
    short x,y;
    unsigned char width,height,hp;
    unsigned char awardId;
    unsigned short score;
};

struct StormrailFinaleAttackContract {
    unsigned char actor,kind;
    unsigned char phase;
    unsigned char period;
    unsigned char telegraphTicks;
    unsigned char shotCount,shotInterval;
    signed char dx;
    signed char dyMode;
};

static const struct StormrailFinaleActorContract stormrailFinaleActors[1]={
    {188,81,80,46,120,60,320}
};

#define stormrailFinaleRenderBoundsValid() \
    (STORMRAIL_FINALE_RENDER_X_OFFSET>=0&& \
     STORMRAIL_FINALE_RENDER_Y_OFFSET>=0&& \
     STORMRAIL_FINALE_RENDER_X_OFFSET+STORMRAIL_FINALE_RENDER_W<= \
        stormrailFinaleActors[STORMRAIL_FINALE_HARRIER].width&& \
     STORMRAIL_FINALE_RENDER_Y_OFFSET+STORMRAIL_FINALE_RENDER_H<= \
        stormrailFinaleActors[STORMRAIL_FINALE_HARRIER].height)

static short stormrailFinaleActorX(unsigned long tick,unsigned char actor)
{
    unsigned short phase;
    phase=(unsigned short)((tick>>1)%80);
    return (short)(stormrailFinaleActors[actor].x+
        (phase<40?(short)phase-20:59-(short)phase));
}

static short stormrailFinaleActorY(unsigned long tick,unsigned char actor)
{
    unsigned short phase;
    phase=(unsigned short)((tick>>1)%112);
    return (short)(stormrailFinaleActors[actor].y+
        (phase<56?(short)phase-28:83-(short)phase));
}

static const struct StormrailFinaleAttackContract stormrailFinaleAttacks[2]={
    {STORMRAIL_FINALE_HARRIER,STORMRAIL_FINALE_ATTACK_FAN,32,
     STORMRAIL_FINALE_ATTACK_PERIOD,STORMRAIL_FINALE_TELEGRAPH_TICKS,
     STORMRAIL_FINALE_VOLLEY_SHOTS,0,-4,0},
    {STORMRAIL_FINALE_HARRIER,STORMRAIL_FINALE_ATTACK_HUNTER,96,
     STORMRAIL_FINALE_ATTACK_PERIOD,STORMRAIL_FINALE_TELEGRAPH_TICKS,
     3,STORMRAIL_FINALE_HUNTER_INTERVAL,-5,1}
};

static const signed char stormrailFinaleVolleyDy[STORMRAIL_FINALE_VOLLEY_SHOTS]={-2,0,2};

static int stormrailFinaleAllDestroyed(const unsigned char hp[STORMRAIL_FINALE_ACTOR_COUNT])
{
    return hp[STORMRAIL_FINALE_HARRIER]==0;
}

static int stormrailFinalePlayerContact(short playerX,short playerY,
                                        short actorX,short actorY,
                                        unsigned char actor)
{
    return playerX+STORMRAIL_FINALE_PLAYER_HIT_X+
               STORMRAIL_FINALE_PLAYER_HIT_W>=actorX&&
           playerX+STORMRAIL_FINALE_PLAYER_HIT_X<=
               actorX+stormrailFinaleActors[actor].width&&
           playerY+STORMRAIL_FINALE_PLAYER_HIT_Y+
               STORMRAIL_FINALE_PLAYER_HIT_H>=actorY&&
           playerY+STORMRAIL_FINALE_PLAYER_HIT_Y<=
               actorY+stormrailFinaleActors[actor].height;
}

static short stormrailFinaleContactResolveX(short actorX)
{
    return (short)(actorX-STORMRAIL_FINALE_PLAYER_HIT_X-
                   STORMRAIL_FINALE_PLAYER_HIT_W);
}

#define stormrailFinaleGateResident(phase,gateOpen) \
    ((phase)==STORMRAIL_FINALE_PHASE_COMBAT&&(gateOpen)==0)

#define stormrailFinaleShouldBegin(distance,active) \
    ((distance)==STORMRAIL_SLICE_END_DISTANCE&&!(active))

#define stormrailFinaleGateArrivalOffset(phase,tick) \
    ((short)(((phase)==STORMRAIL_FINALE_PHASE_ARRIVAL&& \
              (tick)<STORMRAIL_FINALE_ARRIVAL_TICKS)? \
        STORMRAIL_FINALE_ARRIVAL_TICKS-(tick):0))

/* Move the complete 32px gate width over 48 ticks. At tick 47 only its final
   edge pixel remains visible; tick 48 removes it without a half-width pop. */
#define stormrailFinaleGateOpenOffset(open) \
    ((short)(((unsigned short)(open)*2U)/3U))

#define stormrailFinaleHarrierArrivalOffset(phase,tick) \
    ((short)(stormrailFinaleGateArrivalOffset((phase),(tick))*4))

#define stormrailFinalePlayerMayMove(phase) \
    ((phase)==STORMRAIL_FINALE_PHASE_ARRIVAL|| \
     (phase)==STORMRAIL_FINALE_PHASE_COMBAT)

#define stormrailRestoreWordOffset(physicalX) \
    ((unsigned short)(physicalX)&15U)
#define stormrailRestoreWordCount(physicalX,width) \
    ((stormrailRestoreWordOffset(physicalX)+(width)+15U)>>4)
#define stormrailRestoreFootprintWorldX(worldX,physicalX) \
    ((short)((worldX)-(short)stormrailRestoreWordOffset(physicalX)))
#define stormrailRestoreFootprintWidth(physicalX,width) \
    ((short)(stormrailRestoreWordCount((physicalX),(width))*16U))

static int stormrailFinaleAttackDue(unsigned long tick,unsigned char attack)
{
    const struct StormrailFinaleAttackContract *cue=&stormrailFinaleAttacks[attack];
    unsigned long phase=tick%cue->period;
    unsigned char shot;
    for(shot=0;shot<cue->shotCount;shot++)
        if(phase==(unsigned long)cue->phase+
                  (unsigned long)shot*cue->shotInterval) return 1;
    return 0;
}

static int stormrailFinaleAttackEnabled(unsigned char harrierHp,
                                         unsigned char attack)
{
    return stormrailFinaleAttacks[attack].kind!=STORMRAIL_FINALE_ATTACK_HUNTER||
           harrierHp<=STORMRAIL_FINALE_HUNTER_HP;
}

static int stormrailFinaleAttackChargeDue(unsigned long tick,
                                           unsigned char attack)
{
    const struct StormrailFinaleAttackContract *cue=&stormrailFinaleAttacks[attack];
    return tick%cue->period==(unsigned long)cue->phase-cue->telegraphTicks;
}

static int stormrailFinaleAttackTelegraphing(unsigned long tick,
                                              unsigned char attack)
{
    const struct StormrailFinaleAttackContract *cue=&stormrailFinaleAttacks[attack];
    unsigned long phase=tick%cue->period;
    unsigned long start=(unsigned long)cue->phase-cue->telegraphTicks;
    return phase>=start&&phase<cue->phase;
}

static unsigned char stormrailFinaleAttackShot(unsigned long tick,
                                                unsigned char attack)
{
    const struct StormrailFinaleAttackContract *cue=&stormrailFinaleAttacks[attack];
    unsigned long phase=tick%cue->period;
    if(phase<cue->phase||!cue->shotInterval) return 0;
    return (unsigned char)((phase-cue->phase)/cue->shotInterval);
}

static int stormrailFinaleMayFire(unsigned long tick,unsigned long lastFireTick,
                                  int hasFired)
{
    return !hasFired||tick-lastFireTick>=STORMRAIL_FINALE_MIN_FIRE_GAP;
}

static short stormrailFinaleApproach(short value,short target,short step)
{
    if(value<target) return (short)(value+step>target?target:value+step);
    if(value>target) return (short)(value-step<target?target:value-step);
    return value;
}

static long stormrailClamp(long value,long minimum,long maximum)
{
    if(value<minimum) return minimum;
    if(value>maximum) return maximum;
    return value;
}

static unsigned char stormrailRestoreHeart(unsigned char health,
                                            unsigned char maximum)
{
    return (unsigned char)(health>=maximum-1?maximum:health+2);
}

static unsigned char stormrailObstacleSafeBaseFrame(unsigned char type,
                                                     unsigned char frame)
{
    if(type==STORMRAIL_OBSTACLE_PILLAR) return 0;
    if(type==STORMRAIL_OBSTACLE_BIG) return frame>=2?2:0;
    return frame>=4?4:(frame>=2?2:0);
}

static int stormrailReentryCorrectionActive(unsigned long timer,long x)
{
    return timer<STORMRAIL_CONTROL_DELAY&&x<STORMRAIL_PLAY_X;
}

static long stormrailFlightWorldX(long cameraX,long screenX)
{
    return cameraX+screenX;
}

static int stormrailVerticalRectSafe(long y,long height)
{
    return y>=0&&height>=0&&y+height<=STORMRAIL_PLAYFIELD_H;
}

static int stormrailFireDue(unsigned long heldTicks,int wasHeld)
{
    if(!wasHeld) return 1;
    return heldTicks>=STORMRAIL_HOLD_DELAY&&
           ((heldTicks-STORMRAIL_HOLD_DELAY)%STORMRAIL_HOLD_INTERVAL)==0;
}

static int stormrailEnemyHittable(long x)
{
    /* Require half of the 48px silhouette to enter the 320px viewport. */
    return x<=STORMRAIL_ENEMY_HITTABLE_X;
}

static int stormrailObstacleHittable(long x,int type)
{
    if(type==STORMRAIL_OBSTACLE_BIG)
        /* Match enemy fairness: shots connect once half the rock is visible. */
        return x<=STORMRAIL_OBSTACLE_BIG_HITTABLE_X;
    return x<=320-STORMRAIL_OBSTACLE_SHARD_W;
}

static int stormrailObstacleWidth(int type)
{
    return type==STORMRAIL_OBSTACLE_BIG?STORMRAIL_OBSTACLE_BIG_W:
           type==STORMRAIL_OBSTACLE_PILLAR?STORMRAIL_OBSTACLE_PILLAR_W:
           STORMRAIL_OBSTACLE_SHARD_W;
}

static int stormrailObstacleHeight(int type)
{
    return type==STORMRAIL_OBSTACLE_BIG?STORMRAIL_OBSTACLE_BIG_H:
           type==STORMRAIL_OBSTACLE_PILLAR?STORMRAIL_OBSTACLE_PILLAR_H:
           STORMRAIL_OBSTACLE_SHARD_H;
}

static long stormrailRearScroll(unsigned long distance)
{
    return (long)((distance>>STORMRAIL_REAR_PARALLAX_SHIFT)%
                  STORMRAIL_REAR_LOOP_W);
}

static long stormrailPresentationX(unsigned long distance)
{
    return (long)(distance&(STORMRAIL_FLIGHT_PRESENTATION_W-1));
}

#endif
