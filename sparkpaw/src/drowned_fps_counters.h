/* Read-only CIA-A TOD observations, NOT exact visible deadline counts.
   Keep zero deltas: TOD and the publication window have different phases. */
struct DrownedFpsTotals {
    ULONG intervals, fields, zero, one, two, threePlus, maximum;
};
static struct DrownedFpsTotals fpsTotal, fpsRegion[8], fpsFerry[2][2], fpsReset;
static ULONG fpsPreviousField, fpsPreviousStep, fpsResets, fpsPauses;
static UBYTE fpsPrimed, fpsPreviousRegion, fpsPreviousUpper, fpsAfterReset;
static UBYTE fpsResetTail;
static UWORD fpsMaxPublishLine;
static UBYTE fpsRegionFor(WORD x)
{
#ifdef SPARKPAW_LEVEL1_RING_TEST
    if(x<512) return 0;
    if(x<1024) return 1;
    if(x<1536) return 2;
    if(x<2048) return 3;
    if(x<2560) return 4;
    if(x<3072) return 5;
    if(x<3328) return 6;
#else
    if(x<1376) return 0;
    if(x<2128) return 1;
    if(x<2400) return 2;
    if(x<2800) return 3;
    if(x<3200) return 4;
    if(x<3888) return 5;
    if(x<4656) return 6;
#endif
    return 7;
}
static void fpsAdd(struct DrownedFpsTotals *s, ULONG fields)
{
    s->intervals++; s->fields+=fields;
    if(!fields) s->zero++;
    else if(fields==1) s->one++;
    else if(fields==2) s->two++;
    else s->threePlus++;
    if(fields>s->maximum) s->maximum=fields;
}
static void fpsRecord(ULONG field, UWORD line, WORD x, WORD y, ULONG step)
{
    ULONG fields;
    UBYTE reset=(UBYTE)(fpsPrimed && step<fpsPreviousStep);
    if(line>fpsMaxPublishLine) fpsMaxPublishLine=line;
    if(fpsPrimed) {
        fields=(field-fpsPreviousField)&0x00ffffffUL;
        fpsAdd(&fpsTotal,fields);
        fpsAdd(&fpsRegion[fpsPreviousRegion],fields);
        /* Camera teleport can rebuild the two targets on successive frames.
           Preserve those intervals separately; don't blame normal ferry work. */
        if(reset || fpsResetTail) fpsAdd(&fpsReset,fields);
        else if(fpsPreviousRegion==4)
            fpsAdd(&fpsFerry[fpsAfterReset][fpsPreviousUpper],fields);
    }
    if(reset) { fpsResets++; fpsAfterReset=1; fpsResetTail=1; }
    else fpsResetTail=0;
    fpsPreviousField=field; fpsPreviousStep=step;
    fpsPreviousRegion=fpsRegionFor(x); fpsPreviousUpper=(UBYTE)(y<140);
    fpsPrimed=1;
}
