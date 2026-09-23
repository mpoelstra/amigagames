#ifndef SPARKPAW_DROWNED_RING_LAYOUT_H
#define SPARKPAW_DROWNED_RING_LAYOUT_H
/* Full copies remain valid everywhere. Candidate changes the physical origin
   and copy count together; world geometry and the 512px logical ring do not. */
#if defined(SPARKPAW_DROWNED_TWO_COPY_RING) || \
    (defined(SPARKPAW_LEVEL1_TWO_COPY_RING) && defined(SPARKPAW_LEVEL1_RENDERER_TU_ISOLATION))
#define SPARKPAW_RING_TWO_COPY
#define DROWNED_RING_COPIES 2
#define DROWNED_RING_BASE 96
#define DROWNED_RING_SLOT(x) (((x)+DROWNED_RING_BASE)&(PROTOTYPE_RING_W-1))
#else
#define DROWNED_RING_COPIES 3
#define DROWNED_RING_BASE PROTOTYPE_RING_W
#define DROWNED_RING_SLOT(x) ((x)&(PROTOTYPE_RING_W-1))
#endif
#endif
