/* Static foreground silhouettes. Sorted bounds reject all distant objects.
   Work only on inactive attached-sprite staging memory. */
static UWORD drownedFirstOccluder(WORD x)
{
 if(x<0)return 0;
 if((UWORD)(x>>6)>=sizeof(drownedOccluderFirst))return sizeof(drownedOccluders)/sizeof(drownedOccluders[0]);
 return drownedOccluderFirst[x>>6];
}
static BOOL drownedFullOverlap(WORD x,WORD y)
{
 UWORD i;
 for(i=drownedFirstOccluder(x);i<sizeof(drownedOccluders)/sizeof(drownedOccluders[0]);i++){
  const struct DrownedOccluder *o=&drownedOccluders[i];
  if(o->x>=x+48)break;
  if(x<o->x+o->w&&y<o->y+o->h&&y+48>o->y)return TRUE;
 }
 return FALSE;
}
static void drownedFullMask(UWORD *a,UWORD *b,WORD x,WORD y)
{
 UWORD i;
 for(i=drownedFirstOccluder(x);i<sizeof(drownedOccluders)/sizeof(drownedOccluders[0]);i++){
  const struct DrownedOccluder *o=&drownedOccluders[i];
  WORD row,chunk;
  if(o->x>=x+48)break;
  if(x>=o->x+o->w||y>=o->y+o->h||y+48<=o->y)continue;
  for(row=0;row<48;row++){
   WORD py=y+row-o->y;
   if(py<0||py>=o->h)continue;
   for(chunk=0;chunk<3;chunk++){
    WORD shift=x+chunk*16-o->x,at=8+row*8+chunk;
    ULONG bits=0;UWORD keep;
    if(shift>=0&&shift<32)bits=o->bits[py]<<shift;
    else if(shift<0&&shift>-32)bits=o->bits[py]>>(-shift);
    keep=(UWORD)~(bits>>16);
    a[at]&=keep;a[at+4]&=keep;b[at]&=keep;b[at+4]&=keep;
   }
  }
 }
}
