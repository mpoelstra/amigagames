"""Approved imagegen components; fixed-pivot offline motion, existing AGA pens.
Runtime copies opaque precomposited patches, never rotates or alpha-composites.
"""
from pathlib import Path
import json, math, shutil
from PIL import Image, ImageDraw
from preview_drowned_native import decode, indexed
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1]; O=R/'build/drowned-joined'; A=O/'assets'
V=R/'assets/concept/drowned-checkpoint-native-v1'; V.mkdir(exist_ok=True)
src=Image.open(R/'assets/concept/drowned-checkpoint-parts-v1.png').convert('RGB')
# Explicit silhouette excludes generated backdrop/halo; body is identical in every frame.
mask=Image.new('L',src.size);d=ImageDraw.Draw(mask)
d.polygon([(302,713),(302,690),(312,636),(346,610),(346,590),(360,573),(385,573),(385,610),(402,610),(402,550),(417,538),(417,512),(425,498),(425,320),(415,312),(415,121),(430,105),(446,88),(469,88),(469,65),(523,65),(523,88),(548,88),(561,114),(575,124),(575,177),(594,177),(602,191),(619,202),(624,233),(609,253),(575,253),(575,313),(561,323),(561,514),(577,545),(585,573),(595,573),(595,567),(626,567),(636,590),(636,612),(660,629),(678,660),(685,713)],fill=255)
body=src.crop((302,65,686,714)).resize((28,48),Image.Resampling.LANCZOS)
bm=mask.crop((302,65,686,714)).resize((28,48),Image.Resampling.NEAREST)
# Arm source includes its left hinge. Scale is fixed for all 8 poses.
am=Image.new('L',src.size);ImageDraw.Draw(am).polygon([(588,830),(597,807),(639,807),(650,823),(668,808),(917,808),(934,826),(946,842),(946,888),(923,913),(910,925),(658,925),(645,910),(598,925),(588,906)],fill=255)
arm=src.crop((588,800,948,927)).resize((26,9),Image.Resampling.LANCZOS)
armmask=am.crop((588,800,948,927)).resize((26,9),Image.Resampling.NEAREST)
front=decode(A/'drowned-route.spbm');pal=[tuple(front.getpalette()[i:i+3]) for i in range(0,48,3)]
body=indexed(body,pal,1);body.paste(0,(0,0,28,48),Image.eval(bm,lambda x:255-x))
arm=indexed(arm,pal,1);arm.paste(0,(0,0,26,9),Image.eval(armmask,lambda x:255-x))
patches=[];sprites=[];pivot=(21,11)
for n,angle in enumerate([90,80,65,45,25,10,0,0]):
 cell=Image.new('P',(48,48));cell.putpalette(front.getpalette())
 # Rotate arm about its source hinge (2,4), with inverse nearest sampling.
 rad=math.radians(angle);c,s=math.cos(rad),math.sin(rad)
 for y in range(48):
  for x in range(48):
   dx,dy=x-pivot[0],y-pivot[1];u=round(c*dx+s*dy+2);v=round(-s*dx+c*dy+4)
   if 0<=u<26 and 0<=v<9 and arm.getpixel((u,v)):cell.putpixel((x,y),arm.getpixel((u,v)))
 cell.paste(body,(0,0),Image.frombytes('L',body.size,bytes(255 if p else 0 for p in body.tobytes())))
 if n>=3:
  # Lamp recolouring only; fixed body/mask and medallion engraving remain.
  for y in range(8,15):
   for x in range(10,17):
    p=cell.getpixel((x,y));rgb=pal[p]
    if rgb[0]>rgb[2]*1.3 and rgb[0]>90:
     target=(0,210,235) if sum(rgb)<500 else (120,255,255)
     cell.putpixel((x,y),min(range(1,16),key=lambda k:sum((pal[k][j]-target[j])**2 for j in range(3))))
 sprites.append(cell)
 bg=front.crop((2320,152,2368,200));bg.paste(cell,(0,0),Image.frombytes('L',cell.size,bytes(255 if p else 0 for p in cell.tobytes())))
 patches.append(bg)
atlas=Image.new('P',(48,384));atlas.putpalette(front.getpalette())
for n,p in enumerate(patches):atlas.paste(p,(0,n*48))
save_spbm(A/'checkpoint.spbm',atlas,pal,4)
shutil.copy2(R/'assets/audio/checkpoint-v2/checkpoint.raw',A/'checkpoint.raw')
review=[]
for p in sprites:
 rgb=Image.new('RGB',(48,48),(25,43,53));rgb.paste(p.convert('RGB'),(0,0),Image.frombytes('L',p.size,bytes(255 if v else 0 for v in p.tobytes())));review.append(rgb.resize((288,288),Image.Resampling.NEAREST))
review[0].save(V/'activation.gif',save_all=True,append_images=review[1:],duration=[900,80,80,100,100,100,100,1200],loop=0)
board=Image.new('RGB',(384,48))
for n,p in enumerate(sprites):board.paste(p.convert('RGB'),(n*48,0))
board.resize((1536,192),Image.Resampling.NEAREST).save(V/'frames.png')
atlas.save(V/'patches-indexed.png')
(V/'manifest.json').write_text(json.dumps(dict(frames=8,width=48,height=48,pivot=pivot,world=[2320,152],fast_planar_bytes=9216,extra_chip_stage_bytes=0,sample_chip_bytes=5286,palette=pal),indent=2)+'\n')
assert decode(A/'checkpoint.spbm').tobytes()==atlas.tobytes()
print('Checkpoint:8 fixed-pivot48x48 frames,9216 Fast bytes,5286 Chip audio bytes; existing610-byte stage reused.')
