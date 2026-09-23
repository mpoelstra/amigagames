/* Same samples, gains and priorities as the accepted Level-1 proof. */
static const char *paths[FX_COUNT]={
 "PROGDIR:assets/runtime/energy-shot.raw","PROGDIR:assets/runtime/player-hurt.raw",
 "PROGDIR:assets/runtime/enemy-hit.raw","PROGDIR:assets/runtime/enemy-death.raw",
#ifdef SPARKPAW_DROWNED_JOINED
 "PROGDIR:assets/runtime/pump-shot.raw",
#else
 "PROGDIR:assets/runtime/strider-shot.raw",
#endif
 "PROGDIR:assets/runtime/jump.raw",
 "PROGDIR:assets/runtime/collect-spark.raw","PROGDIR:assets/runtime/water-splash.raw",
 "PROGDIR:assets/runtime/stormstone-core.raw","PROGDIR:assets/runtime/tally-tick.raw",
 "PROGDIR:assets/runtime/extra-life.raw","PROGDIR:assets/runtime/harrier-fan-charge.raw",
 "PROGDIR:assets/runtime/harrier-fan-fire.raw","PROGDIR:assets/runtime/harrier-hunter-charge.raw",
 "PROGDIR:assets/runtime/harrier-hunter-fire.raw","PROGDIR:assets/runtime/collect-spark.raw"
#ifdef SPARKPAW_DROWNED_JOINED
 ,"PROGDIR:assets/runtime/checkpoint.raw"
#endif
};
static const UBYTE priorities[FX_COUNT]={127,9,6,8,7,4,5,10,11,3,10,7,7,7,8,5
#ifdef SPARKPAW_DROWNED_JOINED
 ,10
#endif
};
static const UBYTE volumes[FX_COUNT]={60,64,60,64,64,58,58,64,64,54,62,64,64,64,64,64
#ifdef SPARKPAW_DROWNED_JOINED
 ,58
#endif
};
static const UBYTE cooldowns[FX_COUNT]={0,16,4,6,12,4,3,20,55,1,20,16,8,16,6,3
#ifdef SPARKPAW_DROWNED_JOINED
 ,32
#endif
};
