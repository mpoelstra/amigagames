#include <assert.h>
#include "../src/rolling_renderer_contract.h"

int main(void)
{
    struct RollingRendererContract renderer;
    struct RollingColumnWork work;
    unsigned char target;
    unsigned short row[8]={0,1,2,3,4,5,6,7};
    long camera,world,physical;

    assert(ROLLING_MIN_W==336);
    assert(rollingFixedPublishBoundary(0));
    assert(rollingFixedPublishBoundary(4));
    assert(!rollingFixedPublishBoundary(5));
    assert(!rollingFixedPublishBoundary(43));
    assert(ROLLING_RUNTIME_W==512);
    assert(rollingWindowOrigin(0,3392,ROLLING_RUNTIME_W)==0);
    assert(rollingWindowOrigin(47,3392,ROLLING_RUNTIME_W)==0);
    assert(rollingWindowOrigin(48,3392,ROLLING_RUNTIME_W)==16);
    assert(rollingWindowOrigin(3392-320,3392,ROLLING_RUNTIME_W)==2880);
    assert(rollingRectFits(100,64,64,ROLLING_RUNTIME_W));
    assert(!rollingRectFits(48,64,64,ROLLING_RUNTIME_W));
    assert(!rollingRectFits(520,64,64,ROLLING_RUNTIME_W));
    assert(rollingHysteresisOrigin(0,0,3392,ROLLING_RUNTIME_W)==0);
    assert(rollingHysteresisOrigin(128,0,3392,ROLLING_RUNTIME_W)==0);
    assert(rollingHysteresisOrigin(144,0,3392,ROLLING_RUNTIME_W)==80);
    assert(rollingHysteresisOrigin(128,80,3392,ROLLING_RUNTIME_W)==0);
    assert(rollingHysteresisOrigin(144,80,3392,ROLLING_RUNTIME_W)==80);
    assert(rollingHysteresisOrigin(3392-320,2880,3392,
                                   ROLLING_RUNTIME_W)==2880);
    assert(ROLLING_PHYSICAL_W==1536);
    assert(rollingRingWindowOrigin(0,3392,ROLLING_RUNTIME_W)==0);
    assert(rollingRingWindowOrigin(512,3392,ROLLING_RUNTIME_W)==416);
    assert(rollingRingWindowOrigin(3392-320,3392,ROLLING_RUNTIME_W)==2880);
    assert(rollingRingPhysicalX(416,512,ROLLING_RUNTIME_W)==416);
    assert(rollingRingPhysicalX(864,512,ROLLING_RUNTIME_W)==864);
    assert(rollingRingPhysicalX(927,1023,ROLLING_RUNTIME_W)==927);
    assert(rollingRingPhysicalX(1375,1023,ROLLING_RUNTIME_W)==1375);
    assert((rollingRingPhysicalX(511,510,ROLLING_RUNTIME_W)&15)==(511&15));
    assert((rollingRingPhysicalX(512,511,ROLLING_RUNTIME_W)&15)==(512&15));
    assert((rollingRingPhysicalX(513,512,ROLLING_RUNTIME_W)&15)==(513&15));
    assert(rollingRingFetchFits(0,ROLLING_RUNTIME_W,ROLLING_MIN_W));
    assert(rollingRingFetchFits(511,ROLLING_RUNTIME_W,ROLLING_MIN_W));
    assert(!ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(0,464));
    assert(ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(0,480));
    assert(ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(0,496));
    assert(ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(1,0));
    assert(ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(1,496));
    assert(ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(2,336));
    assert(!ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(2,352));
    {
        long phase,byteAt,byteEnd,physicalWord;
        for(phase=0;phase<ROLLING_RUNTIME_W;phase++) {
            byteAt=rollingAga32CorrectedByteOffset(
                ROLLING_RUNTIME_W+phase)-ROLLING_PLAYFIELD_GUARD_BYTES;
            byteEnd=byteAt+ROLLING_PLAYFIELD_FETCH_BYTES;
            for(physicalWord=byteAt*8;physicalWord<byteEnd*8;
                physicalWord+=ROLLING_TILE_W) {
                unsigned short copy=(unsigned short)(
                    physicalWord/ROLLING_RUNTIME_W);
                unsigned short slot=(unsigned short)(
                    physicalWord&(ROLLING_RUNTIME_W-1));
                assert(copy<ROLLING_RING_COPIES);
                assert(ROLLING_PHYSICAL_WORD_MAY_BE_FETCHED(copy,slot));
            }
        }
    }
    assert(rollingRectFits(416,64,416,ROLLING_RUNTIME_W));
    assert(rollingRectFits(864,64,416,ROLLING_RUNTIME_W));
    for(camera=0;camera<=3392-ROLLING_VIEW_W;camera+=16) {
        long origin=rollingRingWindowOrigin(camera,3392,ROLLING_RUNTIME_W);
        assert(rollingRingFetchFits(camera,ROLLING_RUNTIME_W,
                                    ROLLING_MIN_W));
        for(world=camera-96;world<=camera+ROLLING_VIEW_W+32;world+=16) {
            if(world<0||world+ROLLING_MAX_ACTOR_W>3392) continue;
            assert(rollingRectFits(world,ROLLING_MAX_ACTOR_W,origin,
                                   ROLLING_RUNTIME_W));
            physical=rollingRingPhysicalX(world,camera,ROLLING_RUNTIME_W);
            assert(physical>=0);
            assert(physical+ROLLING_MAX_ACTOR_W<=ROLLING_PHYSICAL_W);
            assert((physical&(ROLLING_RUNTIME_W-1))==
                   (world&(ROLLING_RUNTIME_W-1)));
        }
        for(world=camera;world<camera+ROLLING_MIN_W;world+=16) {
            physical=rollingRingPhysicalX(world,camera,ROLLING_RUNTIME_W);
            assert(physical>=ROLLING_RUNTIME_W);
            assert(physical+16<=ROLLING_PHYSICAL_W);
            assert((physical&(ROLLING_RUNTIME_W-1))==
                   (world&(ROLLING_RUNTIME_W-1)));
        }
    }
    rollingShiftWords(row,8,2);
    assert(row[0]==2&&row[1]==3&&row[5]==7);
    row[0]=0; row[1]=1; row[2]=2; row[3]=3;
    row[4]=4; row[5]=5; row[6]=6; row[7]=7;
    rollingShiftWords(row,8,-2);
    assert(row[2]==0&&row[3]==1&&row[7]==5);
    assert(rollingRequiredWidth(42,16,0)==336);
    assert(rollingRequiredWidth(42,16,16)==352);
    assert(rollingRequiredWidth(44,32,0)==352);
    assert(rollingAga32ByteOffset(0)==0);
    assert(rollingAga32ByteOffset(31)==0);
    assert(rollingAga32ByteOffset(32)==4);
    assert(rollingAga32Bplcon1(0,0)==0x00ff);
    assert(rollingAga32Bplcon1(15,15)==0x0000);
    assert(rollingAga32Bplcon1(16,16)==0x44ff);
    assert(rollingAga32Bplcon1(31,31)==0x4400);
    assert(rollingAga32Bplcon1(32,32)==0x00ff);
    assert(rollingAga32CorrectedByteOffset(0)==0);
    assert(rollingAga32CorrectedByteOffset(15)==0);
    assert(rollingAga32CorrectedByteOffset(16)==4);
    assert(rollingAga32CorrectedBplcon1(0,0)==0x44ff);
    assert(rollingAga32CorrectedBplcon1(15,15)==0x4400);
    assert(rollingAga32CorrectedBplcon1(16,16)==0x00ff);
    assert(rollingAga32CorrectedBplcon1(47,47)==0x4400);
    assert(rollingAga32CorrectedBplcon1(48,48)==0x00ff);
    assert(rollingAga64CorrectedByteOffset(0)==0);
    assert(rollingAga64CorrectedByteOffset(47)==0);
    assert(rollingAga64CorrectedByteOffset(48)==8);
    assert(rollingAga64CorrectedBplcon1(0,0)==0xccff);
    assert(rollingAga64CorrectedBplcon1(47,47)==0x4400);
    assert(rollingAga64CorrectedBplcon1(48,48)==0x00ff);
    rollingContractInit(&renderer);
    assert(renderer.displayedTarget==0&&renderer.activeCopper==0);
    assert(!rollingBeginBuild(&renderer,0));
    target=rollingInactiveTarget(&renderer);
    assert(target==1&&rollingBeginBuild(&renderer,target));

    work=rollingColumnWork(0,15);
    assert(work.count==0);
    rollingCommitColumns(&renderer.target[target],15,work);
    work=rollingColumnWork(15,16);
    assert(work.direction==1&&work.count==1&&work.firstWorldX==352);
    rollingCommitColumns(&renderer.target[target],16,work);
    assert(renderer.target[target].ringColumn==1);
    work=rollingColumnWork(16,0);
    assert(work.direction==-1&&work.count==1&&work.firstWorldX==0);
    rollingCommitColumns(&renderer.target[target],0,work);
    assert(renderer.target[target].ringColumn==0);

    work=rollingColumnWork(0,4096);
    assert(work.count==ROLLING_COLUMNS);
    rollingCommitColumns(&renderer.target[target],4096,work);
    rollingSynchronizeDynamic(&renderer.target[target],7,11);
    assert(renderer.target[target].waterGeneration==7);
    assert(renderer.target[target].diamondGeneration==11);
    assert(!rollingCompleteBuild(&renderer.target[target],0,13));
    assert(!rollingPublish(&renderer,target,1));
    assert(renderer.displayedTarget==0&&renderer.activeCopper==0);
    assert(rollingCompleteBuild(&renderer.target[target],1,13));
    assert(!rollingPublish(&renderer,target,0));
    assert(rollingPublish(&renderer,target,1));
    assert(renderer.displayedTarget==1&&renderer.activeCopper==1);
    assert(renderer.target[0].state==ROLLING_TARGET_READY);
    assert(renderer.target[1].state==ROLLING_TARGET_DISPLAYED);
    assert(renderer.publishedGeneration==1);
    assert(renderer.droppedPublications==2);

    assert(rollingTripwireActive(300,364,0,48));
    assert(!rollingTripwireActive(369,433,0,48));
    assert(rollingTripwireActive(-48,-1,0,48));
    assert(!rollingTripwireActive(-96,-49,0,48));
    return 0;
}
