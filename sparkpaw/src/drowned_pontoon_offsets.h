/* Normal program BSS, prepared once with the pontoon assets. World position
   bounds come from pontoon physics; no per-frame division or multiplication. */
static UWORD pontoonPositionOffset[PONTOON_RIGHT-PONTOON_LEFT+1];
static LONG pontoonPhaseOffset[32];
static void preparePontoonOffsets(void)
{
    WORD i;
    UWORD offset=(PONTOON_LEFT%80)*56;
    LONG phaseOffset=0;
    for(i=0;i<=PONTOON_RIGHT-PONTOON_LEFT;i++) {
        pontoonPositionOffset[i]=offset;
        offset=(UWORD)(offset+56);
        if(offset>=80*56) offset=0;
    }
    for(i=0;i<32;i++) {
        pontoonPhaseOffset[i]=phaseOffset;
        phaseOffset+=80L*56;
    }
}
/* VBCC otherwise emits a call with three stack arguments for this expression.
   Each argument is evaluated once. Keep the lookup in the caller's hot path. */
#define pontoonMaskOffset(x,y,frame) \
    (pontoonPhaseOffset[(((y)-189)<<4)+(frame)]+ \
     pontoonPositionOffset[(x)-PONTOON_LEFT])
