#ifndef SPARKPAW_CONTROL_OPTIONS_H
#define SPARKPAW_CONTROL_OPTIONS_H

enum SecondaryButtonAction {
    SECONDARY_BUTTON_JUMP,
    SECONDARY_BUTTON_FIRE
};

enum CampaignStartSection {
    CAMPAIGN_START_STORM_RUINS,
    CAMPAIGN_START_STORMRAIL
};

static int campaignOptionsVariant(enum SecondaryButtonAction secondaryAction,
                                  enum CampaignStartSection startSection,
                                  int optionRow)
{
    return (optionRow?4:0)+
        (secondaryAction==SECONDARY_BUTTON_FIRE?2:0)+
        (startSection==CAMPAIGN_START_STORMRAIL?1:0);
}

static int secondaryButtonAddsJump(enum SecondaryButtonAction action,
                                   int held)
{
    return action==SECONDARY_BUTTON_JUMP&&held;
}

static int secondaryButtonAddsFire(enum SecondaryButtonAction action,
                                   int held)
{
    return action==SECONDARY_BUTTON_FIRE&&held;
}

#endif
