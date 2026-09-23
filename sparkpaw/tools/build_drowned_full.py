"""Join the reviewed route and finale; reuse approved static art throughout."""
from pathlib import Path
import json,shutil,runpy
from PIL import Image
from preview_drowned_native import decode,indexed
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-full';A=O/'assets';A.mkdir(parents=True,exist_ok=True)
J=R/'build/drowned-joined';G=R/'build/drowned-governor';offset=3248;width=5120
for p in (G/'assets').iterdir():shutil.copy2(p,A/p.name)
for n in ['governor_art.h','drowned_pontoon_art.h','drowned_route_coins.h','drowned_shrub_mask.h','spillwing_clearance.h']:
 shutil.copy2((J if n in ('drowned_route_coins.h','spillwing_clearance.h') else G)/n,O/n)
# Reuse exact accepted pieces and palette converter.
kit=runpy.run_path(str(R/'tools/build_governor_platforms.py'))
veg=runpy.run_path(str(R/'tools/build_drowned_vegetation.py'))
front=Image.new('P',(width,208));land=decode(J/'assets/drowned-route.spbm');front.putpalette(land.getpalette())
front.paste(land.crop((0,0,offset,208)),(0,0))
front.paste(decode(G/'assets/drowned-route.spbm').crop((0,0,1872,208)),(offset,0))
pal=[tuple(front.getpalette()[i:i+3]) for i in range(0,48,3)]
def paste(im,x,y):
 front.paste(im,(x,y),Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes())))
data=json.loads((R/'assets/levels/drowned-route-v1.json').read_text())
manifest=json.loads((J/'manifest.json').read_text())
water=manifest['water']
# Continue the collectible trail without increasing the48-slot runtime pool.
tail_coins=[[3264,176],[3376,176],[3472,116],[3536,110],[3592,116],
            [3784,176],[4016,176],[4144,124],[4304,176],[4384,176],
            [4664,176],[4816,176]]
manifest['coins']+=tail_coins
assert len(manifest['coins'])<=48
(O/'drowned_route_coins.h').write_text('static const WORD routeCoins[][2]={'+
 ','.join('{%d,%d}'%tuple(v) for v in manifest['coins'])+'};\n')
# Whole-panel ground only on dry terrain: preserve all water pixels/masks.
for x in range(0,offset,16):
 if not any(wx<=x<wx+80 for wx in water):
  front.paste(kit['strip'].crop((x%32,0,x%32+16,8)),(x,200))
platforms=data['platforms']+[[2768,160,96],[2912,128,16],[3008,96,16],[3104,128,16]]
for x,y,w in platforms:
 # Original dry precision piers have supports; ferry decks stay suspended.
 supported=x<2400
 front.paste(0,(x,y,x+w,200 if supported else y+16))
 deck=kit['deck']
 # End sections preserve detail instead of crushing a192px platform to16px.
 front.paste(deck.crop((0,0,w,16)),(x,y))
 if supported:
  if w<=32:paste(kit['left'].resize((w,200-y-16),Image.Resampling.NEAREST),x,y+16)
  else:
   for px,leg in [(x+4,kit['left']),(x+w-36,kit['right'])]:
    paste(leg.resize((32,200-y-16),Image.Resampling.NEAREST),px,y+16)
# Restore static elevated geyser lip, never covered by vegetation.
lip=decode(R/'assets/concept/drowned-geyser-native-v2/lip.spbm');paste(lip,1792,170)
# Dense clusters on dry shore; precision/ferry landing silhouettes remain clear.
# x,width,height,species,mirrored. Different silhouettes, clustered unevenly.
trees=[(24,56,126,0,False),(84,28,64,1,True),
       (336,40,92,1,False),(704,48,118,0,True),
       (864,52,116,1,True),(1104,48,124,0,False),
       (1280,48,106,1,False),(2144,56,132,0,True),
       (2208,32,72,1,False),(2256,40,98,0,False),
       (3200,40,92,1,True),(offset+40,48,120,0,False),
       (offset+480,48,96,1,True),(offset+1120,40,102,0,True)]
boxes=[(0,0,524,1000),(530,300,900,1000)]
for x,w,h,species,flip in trees:
 n,a=veg['plant'](boxes[species],(w,h))
 if flip:n=n.transpose(Image.Transpose.FLIP_LEFT_RIGHT);a=a.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
 front.paste(n,(x,201-h),a)
# x,width,height,mirrored. Native alpha retained separately for every clump.
shrubs=[(112,24,13,False),(400,32,20,True),(688,20,12,False),
        (744,24,16,True),(1152,28,18,False),(1336,24,14,True),
        (2160,32,18,True),(2280,24,15,False),
        (offset+440,28,16,False),(offset+1504,32,20,False)]
objects=[]
for x,w,h,flip in shrubs:
 n,a=veg['plant']((1110,800,1536,1000),(w,h))
 if flip:n=n.transpose(Image.Transpose.FLIP_LEFT_RIGHT);a=a.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
 y=201-h
 if x!=offset+1504:front.paste(n,(x,y),a)
 bits=[sum(1<<(31-i) for i in range(w) if a.getpixel((i,row))) for row in range(h)]
 objects.append((x,y,w,h,bits))
# Approved flower concept: A behind player, B in front of feet.
# Native palette stays unchanged: violet A blossoms and warm cream B blossoms.
flower_source=Image.open(R/'assets/concept/drowned-flowers-v1.png').convert('RGBA')
def flower(species,w,h):
 box=(80,200,750,730) if species==0 else (1050,230,1700,730)
 im=flower_source.crop(box);alpha=im.getchannel('A').point(lambda v:255 if v>=200 else 0)
 bbox=alpha.getbbox();im=im.crop(bbox);alpha=alpha.crop(bbox)
 # Preserve sparse blossom highlights instead of averaging them into foliage.
 highlight=Image.new('L',im.size)
 highlight.putdata([255 if al>=200 and ((r>g*1.35 and b>g*.70 and r>85) if species==0
                   else (r>170 and g>130 and b>50 and r>b*1.2)) else 0
                   for r,g,b,al in im.getdata()])
 highlight=highlight.resize((w,h),Image.Resampling.BOX)
 im=im.resize((w,h),Image.Resampling.BOX)
 alpha=alpha.resize((w,h),Image.Resampling.BOX).point(lambda v:255 if v>=128 else 0)
 n=indexed(im.convert('RGB'),pal,first=1)
 for y in range(h*2//3):
  for x in range(w):
   if highlight.getpixel((x,y))>=30:
    alpha.putpixel((x,y),255)
    n.putpixel((x,y),7 if species==0 else 4)
 n.putdata([v if a else 0 for v,a in zip(n.getdata(),alpha.getdata())])
 return n,alpha
flowers=[(64,16,12,0),(356,18,13,0),(1112,16,12,0),
         (2200,16,12,0),(3296,18,13,0),(4408,16,12,0),
         (96,16,11,1),(416,16,12,1),(1304,18,12,1),
         (2264,16,12,1),(3728,18,12,1),(4384,16,12,1)]
flower_preview=Image.new('RGB',(160,40),(25,38,48))
for species in range(2):
 n,a=flower(species,18,13);flower_preview.paste(n.convert('RGB'),(20+species*80,14),a)
flower_preview.resize((960,240),Image.Resampling.NEAREST).save(O/'flowers-native.png')
for x,w,h,species in flowers:
 n,a=flower(species,w,h);y=201-h
 front.paste(n,(x,y),a)
 if species==1:
  bits=[sum(1<<(31-i) for i in range(w) if a.getpixel((i,row))) for row in range(h)]
  objects.append((x,y,w,h,bits))
# Exact accepted station group was pasted with finale, unchanged.
save_spbm(A/'drowned-route.spbm',front,pal,4)
# Continuous authored extension: never repeat unmatched panorama edges.
rear=decode(R/'assets/concept/drowned-panorama-v2/drowned-rear.spbm')
original_rear=decode(J/'assets/drowned-rear.spbm')
assert rear.getpalette()==original_rear.getpalette()
assert rear.height==208 and rear.width >= (width-320)//4+352
assert rear.crop((0,0,832,208)).tobytes()==original_rear.crop((0,0,832,208)).tobytes()
save_spbm(A/'drowned-rear.spbm',rear,[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)],3)
# Collision retained byte-for-byte over the existing route; finale owns its geometry.
old=(J/'assets/drowned-route.bin').read_bytes();collision=bytearray(width//16*14)
for y in range(14):collision[y*(width//16):y*(width//16)+offset//16]=old[y*220:y*220+offset//16]
(A/'drowned-route.bin').write_bytes(collision)
surfaces=manifest['surfaces'];ns=len(surfaces)
surfaces += [[a+offset,b+offset,y] for a,b,y in [(1024,1264,200),(560,784,200),(1024,1264,200),(736,1152,200),(96,416,200),(192,384,144)]]
spawns=manifest['spawns']
for a,b,di,s,t in [(1072,1088,-1,0,0),(624,640,-1,1,0),(1120,1120,-1,2,2),(848,848,1,3,2),(120,136,1,4,1),(304,320,-1,5,1)]:
 spawns.append([a+offset,b+offset,di,ns+s,t,1,1])
links=manifest['links']
for v in [[4,4,1,96,100,132,138,320,-900,60],[4,4,-1,136,140,98,104,-320,-900,60],[5,5,1,224,228,261,267,320,-900,60],[5,5,-1,266,270,227,233,-320,-900,60]]:
 v[0]+=ns;v[1]+=ns
 for i in range(3,7):v[i]+=offset
 links.append(v)
assert len(surfaces)<=25 and len(spawns)<=24
def rows(v):return '{'+','.join('{'+','.join(map(str,r))+'}' for r in v)+'}'
(O/'drowned_route_layout.h').write_text(
 'static const struct EnemyPatrolSurface drownedRouteSurfaces[]='+rows(surfaces)+';\n'+
 'static const struct EnemySpawnCandidate drownedRouteSpawns[]='+rows(spawns)+';\n'+
 'static const struct EnemyTraversalLink drownedRouteLinks[]='+rows(links)+';\n'+
 'static const WORD drownedRouteWater[]={'+','.join(map(str,water))+'};\n')
# Mask covers static shrub silhouettes only; retain both gate posts.
for x in [834,offset+1346]:
 m=[sum(1<<(31-i) for i in range(18) if front.getpixel((x+i,112+y))) for y in range(88)]
 objects.append((x,112,18,88,m))
objects.sort()
header='struct DrownedOccluder {WORD x,y,w,h;ULONG bits[88];};\nstatic const struct DrownedOccluder drownedOccluders[]={'
header+=','.join('{'+','.join(map(str,(x,y,w,h)))+',{'+','.join('0x%08xUL'%v for v in m)+'}}' for x,y,w,h,m in objects)+'};\n'
#64px buckets skip distant plants; safe also for a48px player spanning buckets.
first=[next((i for i,o in enumerate(objects) if o[0]+o[2]>x),len(objects)) for x in range(0,width+1,64)]
header+='static const UBYTE drownedOccluderFirst[]={'+','.join(map(str,first))+'};\n'
(O/'drowned_full_masks.h').write_text(header)
# Conservative column tops include every dynamic patch and collectible envelope.
patches=[(448,131,32),(768,136,80),(1792,107,32),(2320,152,48)]
patches +=[(offset+x,y,w) for x,y,w in [(704,136,32),(960,88,32),(1216,136,32),(832,131,32),(1088,131,32),(1280,136,80)]]
patches +=[(x,197,80) for x in water]+[(x-16,y-2,48) for x,y in manifest['coins']]
tops=[]
for x in range(0,width,16):
 box=front.crop((x,0,x+16,208)).getbbox();top=box[1] if box else 208
 for px,py,pw in patches:
  if x<px+pw and x+16>px:top=min(top,py)
 tops.append(top)
(O/'drowned_column_tops.h').write_text('static const UBYTE drownedColumnTops[320]={'+','.join(map(str,tops))+'};\n')
manifest.update(world_width=width,finale_offset=offset,finale=True,trees=trees,shrubs=shrubs,flowers=flowers,tail_coins=tail_coins)
(O/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
views=Image.new('RGB',(1280,832))
for i,cam in enumerate(range(0,width,320)):
 bg=rear.convert('RGB').crop((cam//4,0,cam//4+320,208));fg=front.crop((cam,0,cam+320,208))
 bg.paste(fg.convert('RGB'),(0,0),Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes())))
 views.paste(bg,((i%4)*320,(i//4)*208))
views.save(O/'full-art-review.png')
print('Full route5120px:19spawn sites,20surfaces,14additional trees of2species,10varied foreground shrubs, accepted station group retained.')
