#include <assert.h>
#include <stdio.h>

#include "../src/stormrail_results_contract.h"
#include "../src/stormrail_contract.h"

int main(void)
{
    struct StormrailResultsSnapshot result;
    unsigned long firstTotal;

    assert(STORMRAIL_RESULTS_PAR_SECONDS==150UL);
    assert(STORMRAIL_RESULTS_PAR_FIELDS==7500UL);
    assert(STORMRAIL_RESULTS_MAX_TIME_BONUS==1500UL);
    assert(STORMRAIL_RESULTS_HARRIER_SCORE==320UL);
    assert(stormrailFinaleActors[STORMRAIL_FINALE_HARRIER].score==
           STORMRAIL_RESULTS_HARRIER_SCORE);
    assert(stormrailFinaleActors[STORMRAIL_FINALE_HARRIER].awardId==60);
    assert(STORMRAIL_REPLAY_RESET_ALL==
        (STORMRAIL_REPLAY_RESET_SCORE|STORMRAIL_REPLAY_RESET_TIME|
         STORMRAIL_REPLAY_RESET_ENEMIES|STORMRAIL_REPLAY_RESET_DEBRIS|
         STORMRAIL_REPLAY_RESET_FINALE|STORMRAIL_REPLAY_RESET_PLAYER|
         STORMRAIL_REPLAY_RESET_DIAMONDS|STORMRAIL_REPLAY_RESET_PICKUPS|
         STORMRAIL_REPLAY_RESET_PROJECTILES|STORMRAIL_REPLAY_RESET_INPUT));

    assert(stormrailResultsBonusSeconds(0)==150);
    assert(stormrailResultsBonusSeconds(49)==150);
    assert(stormrailResultsBonusSeconds(50)==149);
    assert(stormrailResultsBonusSeconds(7499)==1);
    assert(stormrailResultsBonusSeconds(7500)==0);
    assert(stormrailResultsBonusSeconds(500000UL)==0);

    stormrailResultsReset(&result);
    stormrailResultsFinalize(&result,23,41,6000UL,1845UL);
    assert(result.finalized);
    assert(result.enemies==23&&result.diamonds==41);
    assert(result.elapsedFields==6000UL);
    assert(result.liveSectionScore==1845UL);
    assert(result.timeBonusSeconds==30);
    assert(result.timeBonus==300UL);
    assert(result.totalScore==2145UL);
    firstTotal=result.totalScore;

    /* A second entry may not recalculate the bonus, replace the completed-run
       sources or add the Harrier's already-live 320 points. */
    stormrailResultsFinalize(&result,99,99,0,firstTotal+
                             STORMRAIL_RESULTS_HARRIER_SCORE);
    assert(result.enemies==23&&result.diamonds==41);
    assert(result.elapsedFields==6000UL);
    assert(result.liveSectionScore==1845UL);
    assert(result.totalScore==firstTotal);

    stormrailResultsReset(&result);
    assert(!result.finalized&&result.totalScore==0&&result.timeBonus==0);
    assert(result.enemies==0&&result.diamonds==0&&result.elapsedFields==0);
    assert(result.liveSectionScore==0);

    puts("PASS: Stormrail results par, one-shot score and replay reset contract");
    return 0;
}
