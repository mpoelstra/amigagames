#include <assert.h>
#include <stdio.h>

#include "../src/control_options.h"

int main(void)
{
    int seen[8]={0},row,button,start;
    assert(secondaryButtonAddsJump(SECONDARY_BUTTON_JUMP,1));
    assert(!secondaryButtonAddsFire(SECONDARY_BUTTON_JUMP,1));
    assert(!secondaryButtonAddsJump(SECONDARY_BUTTON_FIRE,1));
    assert(secondaryButtonAddsFire(SECONDARY_BUTTON_FIRE,1));
    assert(!secondaryButtonAddsJump(SECONDARY_BUTTON_JUMP,0));
    assert(!secondaryButtonAddsFire(SECONDARY_BUTTON_FIRE,0));
    assert(CAMPAIGN_START_STORM_RUINS==0);
    assert(CAMPAIGN_START_STORMRAIL==1);
    assert(campaignOptionsVariant(SECONDARY_BUTTON_JUMP,
        CAMPAIGN_START_STORM_RUINS,0)==0);
    for(row=0;row<2;row++) for(button=0;button<2;button++)
        for(start=0;start<2;start++) {
            int variant=campaignOptionsVariant(
                (enum SecondaryButtonAction)button,
                (enum CampaignStartSection)start,row);
            assert(variant>=0&&variant<8&&!seen[variant]);
            seen[variant]=1;
        }
    puts("PASS: secondary button mapping is exclusive and defaults safely");
    return 0;
}
