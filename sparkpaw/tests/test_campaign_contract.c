#include <assert.h>
#include <stdio.h>
#include "../src/campaign_contract.h"

int main(void)
{
    struct CampaignState state;
    campaignStartAtStormrail(&state,3,6,0);
    assert(state.section==CAMPAIGN_SECTION_STORMRAIL);
    assert(state.postLevel1SnapshotValid&&state.lightningCoreRecovered);
    assert(state.postLevel1Lives==3&&state.postLevel1Health==6&&
           state.postLevel1Diamonds==0&&!state.postLevel1Score);
    campaignReset(&state);
    campaignBeginResults(&state);
    assert(campaignAcceptDecision(&state,RESULT_DECISION_CONTINUE,
                                  12345,2,3,23)==RESULT_DECISION_CONTINUE);
    assert(state.section==CAMPAIGN_SECTION_STORMRAIL);
    assert(state.bankedScore==12345&&state.postLevel1Score==12345);
    assert(state.postLevel1Lives==2&&state.lightningCoreRecovered);
    assert(state.postLevel1Health==3&&state.postLevel1Diamonds==23);
    assert(campaignAcceptDecision(&state,RESULT_DECISION_CONTINUE,
                                  12345,2,3,23)==RESULT_DECISION_NONE);
    assert(state.bankedScore==12345);
    campaignBeginResults(&state);
    assert(campaignAcceptDecision(&state,RESULT_DECISION_REPLAY_CURRENT,
                                  99999,1,1,49)==RESULT_DECISION_REPLAY_CURRENT);
    assert(state.bankedScore==12345&&state.postLevel1SnapshotValid);
    assert(state.postLevel1Lives==2&&state.postLevel1Health==3&&
           state.postLevel1Diamonds==23);
    campaignBeginResults(&state);
    assert(campaignAcceptDecision(&state,RESULT_DECISION_BACK_TO_TITLE,
                                  77777,1,1,49)==RESULT_DECISION_BACK_TO_TITLE);
    assert(state.section==CAMPAIGN_SECTION_TITLE);
    assert(!state.bankedScore&&!state.postLevel1SnapshotValid);
    assert(!state.postLevel1Lives&&!state.postLevel1Health&&
           !state.postLevel1Diamonds);
    campaignReset(&state);
    campaignBeginResults(&state);
    assert(campaignAcceptDecision(&state,RESULT_DECISION_REPLAY_CURRENT,
                                  5000,1,1,0)==RESULT_DECISION_REPLAY_CURRENT);
    assert(state.section==CAMPAIGN_SECTION_LEVEL1&&!state.bankedScore);
    puts("campaign contract: ok");
    return 0;
}
