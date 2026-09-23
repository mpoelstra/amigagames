"""Approved platform-kit art conversion; same collision deck positions."""
from pathlib import Path
from collections import deque
from PIL import Image
from preview_drowned_native import decode,indexed
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];A=R/'build/drowned-governor/assets';O=R/'build/drowned-governor'
s=Image.open(R/'assets/concept/drowned-platform-kit-v2.png').convert('RGB')
f=decode(A/'drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
def piece(box,size,transparent=True):
 im=s.crop(box);w,h=im.size;mask=Image.new('L',im.size,255)
 if transparent:
  # Flood only the blue backing connected to crop edges; retain dark outlines.
  q=deque([(x,y) for x in range(w) for y in (0,h-1)]+[(x,y) for y in range(h) for x in (0,w-1)]);seen=set()
  while q:
   x,y=q.popleft()
   if (x,y) in seen or not(0<=x<w and 0<=y<h):continue
   seen.add((x,y));r,g,b=im.getpixel((x,y))
   if not(7<=r<=35 and 22<=g<=65 and 40<=b<=95 and b>g>r):continue
   mask.putpixel((x,y),0);q.extend(((x-1,y),(x+1,y),(x,y-1),(x,y+1)))
 im=im.resize(size,Image.Resampling.BOX);mask=mask.resize(size,Image.Resampling.NEAREST);out=indexed(im,pal,first=1)
 out.putdata([p if a else 0 for p,a in zip(out.getdata(),mask.getdata())]);return out
# The flat deck excludes the concept's raised end ornaments.
deck=piece((153,101,1271,166),(192,16),False)
left=piece((176,161,385,430),(32,40));right=piece((1035,161,1252,430),(32,40))
def paste(im,x,y):f.paste(im,(x,y),Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes())))
for x,y,w in [(192,144,192),(880,152,144)]:
 f.paste(0,(x,y,x+w,200));paste(deck.resize((w,16),Image.Resampling.NEAREST),x,y)
 for px,leg in [(x+8,left),(x+w-40,right)]:paste(leg.resize((32,200-y-16),Image.Resampling.NEAREST),px,y+16)
# One complete panel, regular seams: no partial end-cap fragments repeated.
strip=piece((540,886,619,919),(32,8),False)
for x in range(32):
 strip.putpixel((x,0),10);strip.putpixel((x,1),9)
 strip.putpixel((x,6),8);strip.putpixel((x,7),1)
for y in range(2,6):
 strip.putpixel((0,y),1);strip.putpixel((31,y),8)
for x in range(0,f.width,32):f.paste(strip,(x,200))
save_spbm(A/'drowned-route.spbm',f,pal,4);assert decode(A/'drowned-route.spbm').tobytes()==f.tobytes()
rear=decode(A/'drowned-rear.spbm').convert('RGB')
for cam,name in [(128,'platform-kit-native'),(1328,'foreground-passage-native')]:
 bg=rear.crop((cam//4,0,cam//4+320,208));fg=f.crop((cam,0,cam+320,208));bg.paste(fg.convert('RGB'),(0,0),Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes())));bg.resize((1280,832),Image.Resampling.NEAREST).save(O/(name+'.png'))
print('Platform kit: approved pixels, unchanged geometry/palette; regular32px floor panels; rejected foreground post removed')
