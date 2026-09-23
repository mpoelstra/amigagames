#ifndef SPARKPAW_DROWNED_CAMPAIGN_H
#define SPARKPAW_DROWNED_CAMPAIGN_H
/* Cold ABI only: never pass pointers into either engine's private GameState. */
struct DrownedCampaignEntry {
    unsigned long bankedScore,seed;
    unsigned char lives,health,diamonds,controlMode,audioMode;
};
enum DrownedCampaignReturn {
    DROWNED_CAMPAIGN_ERROR=-1,
    DROWNED_CAMPAIGN_TITLE=0,
    DROWNED_CAMPAIGN_READY=1,
    DROWNED_CAMPAIGN_QUIT=2
};
int drownedCampaignRun(const struct DrownedCampaignEntry *entry);
#endif
