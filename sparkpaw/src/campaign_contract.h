#ifndef SPARKPAW_CAMPAIGN_CONTRACT_H
#define SPARKPAW_CAMPAIGN_CONTRACT_H

enum CampaignSection {
    CAMPAIGN_SECTION_LEVEL1,
    CAMPAIGN_SECTION_STORMRAIL,
    CAMPAIGN_SECTION_TITLE
};

enum ResultDecision {
    RESULT_DECISION_NONE,
    RESULT_DECISION_REPLAY_CURRENT,
    RESULT_DECISION_CONTINUE,
    RESULT_DECISION_BACK_TO_TITLE
};

struct CampaignState {
    enum CampaignSection section;
    unsigned long bankedScore;
    unsigned long postLevel1Score;
    unsigned char postLevel1Lives;
    unsigned char postLevel1Health;
    unsigned char postLevel1Diamonds;
    unsigned char lightningCoreRecovered;
    unsigned char postLevel1SnapshotValid;
    unsigned char decisionLatched;
};

static void campaignReset(struct CampaignState *state)
{
    state->section=CAMPAIGN_SECTION_LEVEL1;
    state->bankedScore=0;
    state->postLevel1Score=0;
    state->postLevel1Lives=0;
    state->postLevel1Health=0;
    state->postLevel1Diamonds=0;
    state->lightningCoreRecovered=0;
    state->postLevel1SnapshotValid=0;
    state->decisionLatched=0;
}

static void campaignBeginResults(struct CampaignState *state)
{
    state->decisionLatched=0;
}

static void campaignStartAtStormrail(struct CampaignState *state,
    unsigned char lives,unsigned char health,unsigned char diamonds)
{
    campaignReset(state);
    state->section=CAMPAIGN_SECTION_STORMRAIL;
    state->postLevel1Lives=lives;
    state->postLevel1Health=health;
    state->postLevel1Diamonds=diamonds;
    state->lightningCoreRecovered=1;
    state->postLevel1SnapshotValid=1;
}

static enum ResultDecision campaignAcceptDecision(
    struct CampaignState *state,enum ResultDecision decision,
    unsigned long completedSectionScore,unsigned char remainingLives,
    unsigned char remainingHealth,unsigned char heldDiamonds)
{
    if(state->decisionLatched||decision==RESULT_DECISION_NONE)
        return RESULT_DECISION_NONE;
    if(state->section==CAMPAIGN_SECTION_LEVEL1) {
        if(decision==RESULT_DECISION_REPLAY_CURRENT) {
            campaignReset(state);
        } else if(decision==RESULT_DECISION_CONTINUE) {
            state->bankedScore=completedSectionScore;
            state->postLevel1Score=completedSectionScore;
            state->postLevel1Lives=remainingLives;
            state->postLevel1Health=remainingHealth;
            state->postLevel1Diamonds=heldDiamonds;
            state->lightningCoreRecovered=1;
            state->postLevel1SnapshotValid=1;
            state->section=CAMPAIGN_SECTION_STORMRAIL;
            state->decisionLatched=1;
        } else return RESULT_DECISION_NONE;
    } else if(state->section==CAMPAIGN_SECTION_STORMRAIL) {
        if(decision==RESULT_DECISION_REPLAY_CURRENT) {
            if(!state->postLevel1SnapshotValid) return RESULT_DECISION_NONE;
            state->bankedScore=state->postLevel1Score;
            state->decisionLatched=1;
        } else if(decision==RESULT_DECISION_BACK_TO_TITLE) {
            state->section=CAMPAIGN_SECTION_TITLE;
            state->bankedScore=0;
            state->postLevel1Score=0;
            state->postLevel1Lives=0;
            state->postLevel1Health=0;
            state->postLevel1Diamonds=0;
            state->lightningCoreRecovered=0;
            state->postLevel1SnapshotValid=0;
            state->decisionLatched=1;
        } else return RESULT_DECISION_NONE;
    } else return RESULT_DECISION_NONE;
    return decision;
}

#endif
