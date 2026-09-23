#!/usr/bin/env python3
"""Offline feasibility only: test a rebased two-copy ring against real helpers.
Does not alter renderer contracts or prove DMA/raster/actor lifecycle safety.
"""
from pathlib import Path
import subprocess,tempfile,json
R=Path(__file__).resolve().parents[1]
source=r'''
#include <assert.h>
#include <stdio.h>
#include "rolling_renderer_contract.h"
int main(void){
 int feasible=0;
 for(long base=0;base<=512;base+=32){
  long minActor=99999,maxActor=0,minFetch=99999,maxFetch=0;int valid=1;
  for(long camera=0;camera<=5120-320;camera++){
   long origin=rollingRingWindowOrigin(camera,5120,512);
   long old=rollingRingPhysicalX(origin,camera,512),first=old-512+base,last=first+512;
   /* All resident-window rectangles, including source-shift padding, are
      contained in these word-aligned endpoints. Current origin is tile aligned. */
   assert(first%16==0&&last%16==0);
   long fetch=rollingAga32CorrectedByteOffset(base+(camera&511))*8-32;
   long end=fetch+48*8;
   if(first<minActor)minActor=first;if(last>maxActor)maxActor=last;
   if(fetch<minFetch)minFetch=fetch;if(end>maxFetch)maxFetch=end;
   if(first<0||last>1024||fetch<0||end>1024)valid=0;
   if(base==96){
    long oldFetch=rollingAga32CorrectedByteOffset(512+(camera&511))*8-32;
    assert(oldFetch-512==fetch-base); /* same world-pixel phase, no BPLCON1 change */
    for(long world=origin;world<origin+512;world+=16){
     long dest=rollingRingPhysicalX(world,camera,512)-512+base;
     assert(dest>=0&&dest+16<=1024);
     assert(((dest-base)&511)==(world&511));
     /* Canonical writes must rotate slot by base, NOT just delete copy2. */
     long slot=(world+base)&511;
     assert(dest==slot||dest==slot+512);
    }
   }
  }
  if(valid){feasible++;printf("{\"base\":%ld,\"actor_window_min\":%ld,\"actor_window_end_max\":%ld,\"fetch_min\":%ld,\"fetch_end_max\":%ld}\n",base,minActor,maxActor,minFetch,maxFetch);}
 }
 assert(feasible==1);return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'proof.c').write_text(source)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I'+str(R/'src'),str(p/'proof.c'),'-o',str(p/'proof')],check=True)
 output=subprocess.check_output([str(p/'proof')],text=True)
proof={'status':'geometric feasibility only; runtime contract unchanged','camera_positions':4801,'world_width':5120,'candidate':json.loads(output),'old_target_bytes':1536//8*208*4*2,'new_target_bytes':1024//8*208*4*2,'chip_saving':512//8*208*4*2,'copied_words_reduction_fraction':1/3,'limitations':['all DMA consumers/lifetimes not yet converted or tested','new wrap positions can change blit setup count','no measured FPS gain','not historical H3 fetch-union pruning: two complete translated copies, no per-word reach predicate']}
print(json.dumps(proof,indent=2))
