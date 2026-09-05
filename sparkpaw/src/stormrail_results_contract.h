#ifndef SPARKPAW_STORMRAIL_RESULTS_CONTRACT_H
#define SPARKPAW_STORMRAIL_RESULTS_CONTRACT_H

/* The complete deterministic interlude, including departure/boarding and the
   finale, owns this par. elapsedFields is the sole clock source. */
#define STORMRAIL_RESULTS_FIELDS_PER_SECOND 50UL
#define STORMRAIL_RESULTS_PAR_SECONDS 150UL
#define STORMRAIL_RESULTS_PAR_FIELDS \
    (STORMRAIL_RESULTS_PAR_SECONDS*STORMRAIL_RESULTS_FIELDS_PER_SECOND)
#define STORMRAIL_RESULTS_TIME_MULTIPLIER 10UL
#define STORMRAIL_RESULTS_MAX_TIME_BONUS \
    (STORMRAIL_RESULTS_PAR_SECONDS*STORMRAIL_RESULTS_TIME_MULTIPLIER)
#define STORMRAIL_RESULTS_HARRIER_SCORE 320UL

/* These bits are a reviewable reset manifest. gameInit() must establish all
   of them before a resident Stormrail replay is published. */
#define STORMRAIL_REPLAY_RESET_SCORE       (1UL<<0)
#define STORMRAIL_REPLAY_RESET_TIME        (1UL<<1)
#define STORMRAIL_REPLAY_RESET_ENEMIES     (1UL<<2)
#define STORMRAIL_REPLAY_RESET_DEBRIS      (1UL<<3)
#define STORMRAIL_REPLAY_RESET_FINALE      (1UL<<4)
#define STORMRAIL_REPLAY_RESET_PLAYER      (1UL<<5)
#define STORMRAIL_REPLAY_RESET_DIAMONDS    (1UL<<6)
#define STORMRAIL_REPLAY_RESET_PICKUPS     (1UL<<7)
#define STORMRAIL_REPLAY_RESET_PROJECTILES (1UL<<8)
#define STORMRAIL_REPLAY_RESET_INPUT       (1UL<<9)
#define STORMRAIL_REPLAY_RESET_ALL         ((1UL<<10)-1UL)

struct StormrailResultsSnapshot {
    unsigned short enemies;
    unsigned short diamonds;
    unsigned long elapsedFields;
    unsigned long liveSectionScore;
    unsigned short timeBonusSeconds;
    unsigned long timeBonus;
    unsigned long totalScore;
    unsigned char finalized;
};

static unsigned short stormrailResultsBonusSeconds(unsigned long elapsedFields)
{
    unsigned long elapsedSeconds=elapsedFields/
        STORMRAIL_RESULTS_FIELDS_PER_SECOND;
    if(elapsedSeconds>=STORMRAIL_RESULTS_PAR_SECONDS) return 0;
    return (unsigned short)(STORMRAIL_RESULTS_PAR_SECONDS-elapsedSeconds);
}

static void stormrailResultsReset(struct StormrailResultsSnapshot *result)
{
    result->enemies=0;
    result->diamonds=0;
    result->elapsedFields=0;
    result->liveSectionScore=0;
    result->timeBonusSeconds=0;
    result->timeBonus=0;
    result->totalScore=0;
    result->finalized=0;
}

/* Idempotence is deliberate: tally entry, Fire skip and replay cannot award
   time or the already-live 320-point Harrier kill a second time. */
static void stormrailResultsFinalize(struct StormrailResultsSnapshot *result,
                                     unsigned short enemies,
                                     unsigned short diamonds,
                                     unsigned long elapsedFields,
                                     unsigned long liveSectionScore)
{
    if(result->finalized) return;
    result->enemies=enemies;
    result->diamonds=diamonds;
    result->elapsedFields=elapsedFields;
    result->liveSectionScore=liveSectionScore;
    result->timeBonusSeconds=stormrailResultsBonusSeconds(elapsedFields);
    result->timeBonus=(unsigned long)result->timeBonusSeconds*
        STORMRAIL_RESULTS_TIME_MULTIPLIER;
    result->totalScore=liveSectionScore+result->timeBonus;
    result->finalized=1;
}

#endif
