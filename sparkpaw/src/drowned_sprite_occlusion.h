/* Local near-post occlusion for the isolated 64-pixel attached-sprite slice.
   Inputs are inactive Chip stages and an immutable Fast-RAM silhouette.
   No display bitmap, source sprite, control words or terminator is modified. */
#ifndef DROWNED_SPRITE_OCCLUSION_H
#define DROWNED_SPRITE_OCCLUSION_H
#ifdef SPARKPAW_DROWNED_GOVERNOR
#define DROWNED_POST_X 1346
#else
#define DROWNED_POST_X 834
#endif
#define DROWNED_POST_Y 112
#define DROWNED_POST_W 18
#define DROWNED_POST_H 88

static BOOL drownedSpriteOverlapsPost(WORD x,WORD y)
{
    return x+48>DROWNED_POST_X && x<DROWNED_POST_X+DROWNED_POST_W &&
           y+48>DROWNED_POST_Y && y<DROWNED_POST_Y+DROWNED_POST_H;
}

static void drownedMaskSprite(UWORD *a,UWORD *b,const ULONG *post,
                              WORD x,WORD y)
{
    WORD row,chunk;
    for(row=0;row<48;row++) {
        WORD py=y+row-DROWNED_POST_Y;
        if(py>=0 && py<DROWNED_POST_H) {
            ULONG silhouette=post[py];
            for(chunk=0;chunk<3;chunk++) {
                WORD shift=x+chunk*16-DROWNED_POST_X;
                ULONG aligned=0;
                UWORD keep;
                WORD at=8+row*8+chunk;
                if(shift>=0 && shift<32) aligned=silhouette<<shift;
                else if(shift<0 && shift>-32) aligned=silhouette>>(-shift);
                keep=(UWORD)~(aligned>>16);
                a[at]&=keep; a[at+4]&=keep;
                b[at]&=keep; b[at+4]&=keep;
            }
        }
    }
}
#ifdef SPARKPAW_DROWNED_GOVERNOR
static BOOL drownedSpriteOverlapsShrub(WORD x,WORD y)
{ return x+48>1504&&x<1536&&y+48>181&&y<201; }
static void drownedMaskShrub(UWORD *a,UWORD *b,const ULONG *mask,WORD x,WORD y)
{
 WORD row,chunk;
 for(row=0;row<20;row++) {
  WORD spriteRow=181+row-y;
  if(spriteRow>=0&&spriteRow<48) for(chunk=0;chunk<3;chunk++) {
   WORD shift=x+chunk*16-1504,at=8+spriteRow*8+chunk;
   ULONG bits=0;UWORD keep;
   if(shift>=0&&shift<32)bits=mask[row]<<shift;
   else if(shift<0&&shift>-32)bits=mask[row]>>(-shift);
   keep=(UWORD)~(bits>>16);
   a[at]&=keep;a[at+4]&=keep;b[at]&=keep;b[at+4]&=keep;
  }
 }
}
#endif
#endif
