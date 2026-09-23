/* Minimal diagnostic only. Attribute intervals to the preceding player X. */
struct DrownedCadenceRegion { ULONG intervals,fields,missed,longFrames,maxFields; };
static struct DrownedCadenceRegion drownedCadenceRegions[6];
static UBYTE drownedCadenceRegion(WORD x)
{
    if(x<1376) return 0;
    if(x<2128) return 1;
    if(x<2400) return 2;
    if(x<2800) return 3;
    if(x<3200) return 4;
    return 5;
}
static void drownedCadenceRecord(WORD x,ULONG fields)
{
    struct DrownedCadenceRegion *r=&drownedCadenceRegions[drownedCadenceRegion(x)];
    r->intervals++; r->fields+=fields;
    if(fields>1) r->missed++;
    if(fields>2) r->longFrames++;
    if(fields>r->maxFields) r->maxFields=fields;
}
