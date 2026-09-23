"""Approved static vegetation: native palette, exact shrub-only occlusion mask."""
from pathlib import Path
from PIL import Image
from preview_drowned_native import decode,indexed
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-governor';A=O/'assets'
s=Image.open(R/'assets/concept/drowned-vegetation-v1.png').convert('RGBA');f=decode(A/'drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
def plant(box,size):
 im=s.crop(box);a=im.getchannel('A').point(lambda p:255 if p>=200 else 0);b=a.getbbox();im=im.crop(b);a=a.crop(b)
 im=im.resize(size,Image.Resampling.BOX);a=a.resize(size,Image.Resampling.BOX).point(lambda p:255 if p>=128 else 0)
 n=indexed(im.convert('RGB'),pal,first=1);n.putdata([p if m else 0 for p,m in zip(n.getdata(),a.getdata())]);return n,a
for box,size,x in [((0,0,524,1000),(64,136),1416),((530,300,900,1000),(48,96),1552)]:
 n,a=plant(box,size);f.paste(n,(x,201-size[1]),a)
n,a=plant((1110,800,1536,1000),(32,20));f.paste(n,(1504,181),a)
rows=[sum((1<<(31-x)) for x in range(32) if a.getpixel((x,y))) for y in range(20)]
(O/'drowned_shrub_mask.h').write_text('static const ULONG drownedShrubMask[20]={'+','.join('0x%08xUL'%v for v in rows)+'};\n')
save_spbm(A/'drowned-route.spbm',f,pal,4);assert decode(A/'drowned-route.spbm').tobytes()==f.tobytes()
rear=decode(A/'drowned-rear.spbm').convert('RGB');cam=1360;bg=rear.crop((cam//4,0,cam//4+320,208));fg=f.crop((cam,0,cam+320,208));bg.paste(fg.convert('RGB'),(0,0),Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes())));bg.resize((1280,832),Image.Resampling.NEAREST).save(O/'vegetation-native.png')
print('Static trees64x136/48x96 behind player; shrub32x20 foreground; exact80byte mask, noChip allocation')
