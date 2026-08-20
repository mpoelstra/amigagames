#ifndef SPARKPAW_ROLLING_RENDERER_CONTRACT_H
#define SPARKPAW_ROLLING_RENDERER_CONTRACT_H

#define ROLLING_VIEW_W 320
#define ROLLING_FINE_SCROLL 16
#define ROLLING_TILE_W 16
#define ROLLING_FETCH_BYTES 42
#define ROLLING_AGA32_FETCH_BYTES 44
#define ROLLING_MIN_W (ROLLING_FETCH_BYTES*8)
#define ROLLING_GUARDED_W (ROLLING_MIN_W+ROLLING_TILE_W)
#define ROLLING_COLUMNS (ROLLING_GUARDED_W/ROLLING_TILE_W)
#define ROLLING_PUBLISH_LAST_LINE 4
#define ROLLING_ACTOR_MARGIN 32
#define ROLLING_MAX_ACTOR_W 64
#define ROLLING_RUNTIME_W 512
#define ROLLING_RING_COPIES 3
#define ROLLING_PHYSICAL_W (ROLLING_RUNTIME_W*ROLLING_RING_COPIES)
#define ROLLING_CAMERA_LOCAL_MIN 64
#define ROLLING_CAMERA_LOCAL_MAX 128

enum RollingTargetState {
    ROLLING_TARGET_DISPLAYED,
    ROLLING_TARGET_BUILDING,
    ROLLING_TARGET_READY
};

struct RollingTargetContract {
    enum RollingTargetState state;
    long worldOrigin;
    unsigned short ringColumn;
    unsigned long cleanGeneration;
    unsigned long waterGeneration;
    unsigned long diamondGeneration;
    unsigned long objectGeneration;
};

struct RollingRendererContract {
    struct RollingTargetContract target[2];
    unsigned char displayedTarget;
    unsigned char activeCopper;
    unsigned long publishedGeneration;
    unsigned long droppedPublications;
};

struct RollingColumnWork {
    long firstWorldX;
    unsigned short count;
    signed char direction;
};

static int rollingFixedPublishBoundary(unsigned short rasterLine)
{
    return rasterLine<=ROLLING_PUBLISH_LAST_LINE;
}

static long rollingWindowOrigin(long cameraX,long worldWidth,long targetWidth)
{
    long origin=(cameraX&~15L)-ROLLING_ACTOR_MARGIN;
    long maximum=worldWidth-targetWidth;
    if(origin<0) origin=0;
    if(origin>maximum) origin=maximum;
    return origin&~15L;
}

static long rollingHysteresisOrigin(long cameraX,long currentOrigin,
                                    long worldWidth,long targetWidth)
{
    long aligned=cameraX&~15L;
    long local=aligned-currentOrigin;
    long origin=currentOrigin;
    long maximum=worldWidth-targetWidth;
    if(local>ROLLING_CAMERA_LOCAL_MAX)
        origin=aligned-ROLLING_CAMERA_LOCAL_MIN;
    else if(local<ROLLING_CAMERA_LOCAL_MIN)
        origin=aligned-ROLLING_CAMERA_LOCAL_MAX;
    if(origin<0) origin=0;
    if(origin>maximum) origin=maximum;
    return origin&~15L;
}

static long rollingRingWindowOrigin(long cameraX,long worldWidth,long ringWidth)
{
    long origin=(cameraX&~15L)-96;
    long maximum=worldWidth-ringWidth;
    if(origin<0) origin=0;
    if(origin>maximum) origin=maximum;
    return origin&~15L;
}

static long rollingRingPhysicalX(long worldX,long cameraX,long ringWidth)
{
    return ringWidth+(cameraX&(ringWidth-1))+worldX-cameraX;
}

static int rollingRingFetchFits(long cameraX,long ringWidth,long fetchWidth)
{
    long start=ringWidth+(cameraX&(ringWidth-1));
    return start>=0&&start+fetchWidth<=ringWidth*ROLLING_RING_COPIES;
}

static int rollingRectFits(long x,long width,long origin,long targetWidth)
{
    return x>=origin&&x+width<=origin+targetWidth;
}

static void rollingShiftWords(unsigned short *row,unsigned short count,
                              short words)
{
    short x;
    if(words>0) {
        for(x=0;x<(short)(count-words);x++) row[x]=row[x+words];
    } else if(words<0) {
        short amount=(short)-words;
        for(x=(short)(count-1);x>=amount;x--) row[x]=row[x-amount];
    }
}

static unsigned short rollingRequiredWidth(unsigned short fetchBytes,
                                           unsigned short alignment,
                                           unsigned short constructionGuard)
{
    unsigned short width=(unsigned short)(fetchBytes*8+constructionGuard);
    return (unsigned short)(((width+alignment-1)/alignment)*alignment);
}

/* AGA 32-bit bitplane fetches need a longword-aligned pointer.  Keep the
   coarse address on a 32-pixel boundary and express the complete 0..31 pixel
   delay with BPLCON1's AGA extension bits. */
static unsigned short rollingAga32ScrollField(unsigned short delay,
                                               unsigned short playfield)
{
    delay&=31;
    if(playfield==1)
        return (unsigned short)((delay&15)|((delay&16)<<6));
    return (unsigned short)(((delay&15)<<4)|((delay&16)<<10));
}

static unsigned short rollingAga32Bplcon1(long front,long rear)
{
    /* Preserve the proven FMODE=0 phase for offsets 0..15, then wrap through
       AGA's extended half of the 32-pixel shifter: 15..0,31..16. */
    unsigned short ff=(unsigned short)((15-(front&31))&31);
    unsigned short rf=(unsigned short)((15-(rear&31))&31);
    return (unsigned short)(rollingAga32ScrollField(ff,1)|
                            rollingAga32ScrollField(rf,2));
}

static long rollingAga32ByteOffset(long x)
{
    return (x>>5)<<2;
}

/* Stage 5D4 measured FMODE1 one 16-pixel cell to the right of FMODE0; D5
   accepted this phase bias through repeated FS-UAE/68030 sweeps. Treat the
   fetch phase as x+16 so delay and coarse pointer wrap remain coordinated. */
static unsigned short rollingAga32CorrectedBplcon1(long front,long rear)
{
    return rollingAga32Bplcon1(front+16,rear+16);
}

static long rollingAga32CorrectedByteOffset(long x)
{
    return rollingAga32ByteOffset(x+16);
}

static unsigned short rollingAga64Bplcon1(long front,long rear)
{
    unsigned short ff=(unsigned short)((15-(front&63))&63);
    unsigned short rf=(unsigned short)((15-(rear&63))&63);
    unsigned short frontField=(unsigned short)((ff&15)|((ff&48)<<6));
    unsigned short rearField=(unsigned short)(((rf&15)<<4)|((rf&48)<<10));
    return (unsigned short)(frontField|rearField);
}

static unsigned short rollingAga64CorrectedBplcon1(long front,long rear)
{
    return rollingAga64Bplcon1(front+16,rear+16);
}

static long rollingAga64CorrectedByteOffset(long x)
{
    return ((x+16)>>6)<<3;
}

static void rollingContractInit(struct RollingRendererContract *renderer)
{
    renderer->displayedTarget=0; renderer->activeCopper=0;
    renderer->publishedGeneration=0; renderer->droppedPublications=0;
    renderer->target[0].state=ROLLING_TARGET_DISPLAYED;
    renderer->target[1].state=ROLLING_TARGET_READY;
    renderer->target[0].worldOrigin=renderer->target[1].worldOrigin=0;
    renderer->target[0].ringColumn=renderer->target[1].ringColumn=0;
    renderer->target[0].cleanGeneration=renderer->target[1].cleanGeneration=0;
    renderer->target[0].waterGeneration=renderer->target[1].waterGeneration=0;
    renderer->target[0].diamondGeneration=renderer->target[1].diamondGeneration=0;
    renderer->target[0].objectGeneration=renderer->target[1].objectGeneration=0;
}

static unsigned char rollingInactiveTarget(
    const struct RollingRendererContract *renderer)
{
    return (unsigned char)(renderer->displayedTarget^1);
}

static int rollingBeginBuild(struct RollingRendererContract *renderer,
                             unsigned char target)
{
    if(target==renderer->displayedTarget||target>1||
       renderer->target[target].state==ROLLING_TARGET_DISPLAYED) return 0;
    renderer->target[target].state=ROLLING_TARGET_BUILDING;
    return 1;
}

static struct RollingColumnWork rollingColumnWork(long oldOrigin,long newOrigin)
{
    struct RollingColumnWork work={0,0,0};
    long oldColumn=oldOrigin/ROLLING_TILE_W;
    long newColumn=newOrigin/ROLLING_TILE_W;
    long delta=newColumn-oldColumn;
    if(delta>0) {
        work.direction=1;
        work.count=(unsigned short)(delta>ROLLING_COLUMNS?
                                    ROLLING_COLUMNS:delta);
        work.firstWorldX=(newColumn+ROLLING_COLUMNS-work.count)*ROLLING_TILE_W;
    } else if(delta<0) {
        delta=-delta; work.direction=-1;
        work.count=(unsigned short)(delta>ROLLING_COLUMNS?
                                    ROLLING_COLUMNS:delta);
        work.firstWorldX=newColumn*ROLLING_TILE_W;
    }
    return work;
}

static void rollingCommitColumns(struct RollingTargetContract *target,
                                 long newOrigin,
                                 struct RollingColumnWork work)
{
    long columnDelta=newOrigin/ROLLING_TILE_W-target->worldOrigin/ROLLING_TILE_W;
    long ring=(long)target->ringColumn+columnDelta;
    while(ring<0) ring+=ROLLING_COLUMNS;
    target->ringColumn=(unsigned short)(ring%ROLLING_COLUMNS);
    target->worldOrigin=newOrigin;
    if(work.count) target->cleanGeneration++;
}

static void rollingSynchronizeDynamic(struct RollingTargetContract *target,
                                      unsigned long water,
                                      unsigned long diamonds)
{
    target->waterGeneration=water;
    target->diamondGeneration=diamonds;
}

static int rollingCompleteBuild(struct RollingTargetContract *target,
                                int finalBlitComplete,
                                unsigned long objectGeneration)
{
    if(target->state!=ROLLING_TARGET_BUILDING||!finalBlitComplete) return 0;
    target->objectGeneration=objectGeneration;
    target->state=ROLLING_TARGET_READY;
    return 1;
}

static int rollingPublish(struct RollingRendererContract *renderer,
                          unsigned char target,int atBoundary)
{
    unsigned char old;
    if(!atBoundary||target==renderer->displayedTarget||target>1||
       renderer->target[target].state!=ROLLING_TARGET_READY) {
        renderer->droppedPublications++; return 0;
    }
    old=renderer->displayedTarget;
    renderer->target[old].state=ROLLING_TARGET_READY;
    renderer->target[target].state=ROLLING_TARGET_DISPLAYED;
    renderer->displayedTarget=target;
    renderer->activeCopper^=1;
    renderer->publishedGeneration++;
    return 1;
}

static int rollingTripwireActive(long actorLeft,long actorRight,long cameraX,
                                 long margin)
{
    return actorRight>=cameraX-margin&&
           actorLeft<=cameraX+ROLLING_VIEW_W+margin;
}

#endif
