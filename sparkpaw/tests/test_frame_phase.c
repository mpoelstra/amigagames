#include <assert.h>
#include "../src/frame_phase.h"

int main(void)
{
    struct FramePhaseClock clock={0};
    framePhaseSample(&clock,100,0);
    assert(framePhaseStamp(&clock)==100);
    framePhaseSample(&clock,253,0);
    assert(framePhaseStamp(&clock)==253);
    framePhaseSample(&clock,17,0);
    assert(framePhaseStamp(&clock)==329);
    framePhaseSample(&clock,40,2);
    assert(clock.epoch==3 && framePhaseStamp(&clock)==976);
    framePhaseSample(&clock,12,1);
    assert(clock.epoch==5 && framePhaseStamp(&clock)==1572);
    return 0;
}
