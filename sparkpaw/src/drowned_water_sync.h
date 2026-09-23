/* Merge adjacent dirty canonical water rectangles only. The existing copier
   owns ring clipping, all three physical copies, and Blitter completion. */
static void drownedSynchronizeWater(struct PrototypeTarget *target)
{
    WORD i=0;
    while(i<LEVEL_WATER_COUNT) {
        WORD first,left,right;
        if(target->waterFrame[i]==waterDrawnFrame[i]) { i++; continue; }
        first=i;
        left=levelWaterLeft(i);
        right=(WORD)(left+WATER_W);
        i++;
        while(i<LEVEL_WATER_COUNT &&
              target->waterFrame[i]!=waterDrawnFrame[i] &&
              levelWaterLeft(i)==right) {
            right=(WORD)(right+WATER_W);
            i++;
        }
        drownedCopyPatchRect(target,left,WATER_Y,(WORD)(right-left),WATER_H);
        /* Canonical strips may have different phases. Track each separately. */
        while(first<i) {
            target->waterFrame[first]=waterDrawnFrame[first];
            first++;
        }
    }
}
