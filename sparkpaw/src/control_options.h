#ifndef SPARKPAW_CONTROL_OPTIONS_H
#define SPARKPAW_CONTROL_OPTIONS_H

enum ControlMode {
    CONTROL_JOYSTICK,
    CONTROL_JOYPAD
};

enum CampaignStartSection {
    CAMPAIGN_START_STORM_RUINS,
    CAMPAIGN_START_STORMRAIL,
    CAMPAIGN_START_DROWNED
};

static int campaignOptionsVariant(enum ControlMode controlMode,
                                  enum CampaignStartSection startSection,
                                  int optionRow)
{
    return (optionRow?4:0)+
        (controlMode==CONTROL_JOYPAD?2:0)+
        (startSection==CAMPAIGN_START_STORMRAIL?1:0);
}

static int controlJumpHeld(enum ControlMode mode,int up,int secondary)
{
    return mode==CONTROL_JOYPAD?secondary:up;
}

#endif
