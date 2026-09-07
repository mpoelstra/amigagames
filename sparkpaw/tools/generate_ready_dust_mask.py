"""Conservative background-only mask shared by every READY menu state."""
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[1]
def decode(name):
 b=(ROOT/'assets/runtime'/name).read_bytes();w,h,d,m,r=struct.unpack('>HHBBH',b[4:12]);pal=[tuple(b[12+3*i:15+3*i]) for i in range(1<<d)];o=12+3*(1<<d)
 return w,h,pal,[[sum(((b[o+p*r*h+y*r+x//8]>>(7-x%8))&1)<<p for p in range(d)) for x in range(w)] for y in range(h)]
def mask(state=0):
 from collections import deque
 w,h,pal,pix=decode('sparkpaw-ready-screen.spbm');pw,ph,pp,patch=decode('readymenu.spbm')
 for y in range(104):pix[y+118][64:256]=patch[state*104+y][:]
 # Background is dark blue, not just COLOR00. Reject foreground edge pixels,
 # then flood from the frame edge so enclosed dark art/text interiors stay solid.
 dark=[[pal[pix[y][x]][0]<=12 and pal[pix[y][x]][1]<=18 and pal[pix[y][x]][2]<=35 for x in range(w)] for y in range(h)]
 safe=[[all(dark[yy][xx] for yy in range(max(0,y-1),min(h,y+2)) for xx in range(max(0,x-1),min(w,x+2))) for x in range(w)] for y in range(h)]
 seen=set();q=deque()
 for y in range(h):
  for x in (0,w-1):
   if safe[y][x]:seen.add((x,y));q.append((x,y))
 for x in range(w):
  for y in (0,h-1):
   if safe[y][x] and (x,y) not in seen:seen.add((x,y));q.append((x,y))
 while q:
  x,y=q.popleft()
  for xx,yy in ((x-1,y),(x+1,y),(x,y-1),(x,y+1)):
   if 0<=xx<w and 0<=yy<h and safe[yy][xx] and (xx,yy) not in seen:seen.add((xx,yy));q.append((xx,yy))
 result=bytearray(40*256)
 for x,y in seen:result[y*40+x//8]|=128>>(x%8)
 return result
if __name__=='__main__':
 base=mask();_,height,_,_=decode('readymenu.spbm');patches=[]
 for state in range(height//104):
  m=mask(state);patches.extend(m[y*40+8:y*40+32] for y in range(118,222))
 data=b''.join(patches)
 def array(name,b):return 'static const unsigned char '+name+'['+str(len(b))+']={\n'+',\n'.join(','.join(str(v) for v in b[i:i+40]) for i in range(0,len(b),40))+'\n};\n'
 (ROOT/'src/ready_dust_mask.h').write_text('/* Generated edge-connected background, current menu state only. */\n'+array('readyDustMask',base)+array('readyDustMenuMask',data))
 print('Base background pixels:',sum(v.bit_count() for v in base),'mask bytes',len(base)+len(data))
