#include <assert.h>
#include <stdio.h>
#include "../src/control_options.h"

int main(void)
{
    int seen[8]={0},row,mode,start;
    assert(controlJumpHeld(CONTROL_JOYSTICK,1,0));
    assert(!controlJumpHeld(CONTROL_JOYSTICK,0,1));
    assert(controlJumpHeld(CONTROL_JOYPAD,0,1));
    assert(!controlJumpHeld(CONTROL_JOYPAD,1,0));
    assert(CAMPAIGN_START_STORM_RUINS==0);
    assert(CAMPAIGN_START_STORMRAIL==1);
    assert(campaignOptionsVariant(CONTROL_JOYSTICK,
        CAMPAIGN_START_STORM_RUINS,0)==0);
    for(row=0;row<2;row++) for(mode=0;mode<2;mode++)
        for(start=0;start<2;start++) {
            int variant=campaignOptionsVariant((enum ControlMode)mode,
                (enum CampaignStartSection)start,row);
            assert(variant>=0&&variant<8&&!seen[variant]);
            seen[variant]=1;
        }
    puts("PASS: joystick and joypad select exclusive controller jump sources");
    return 0;
}
