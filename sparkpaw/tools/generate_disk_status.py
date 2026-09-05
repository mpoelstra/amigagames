"""Create only disk-message status strips, reusing the loading palette/art."""
from pathlib import Path
from PIL import Image,ImageDraw
from generate_ready_screen import GLYPHS,spbm_payload
ROOT=Path(__file__).resolve().parents[1]
def read_spbm(path):
 raw=path.read_bytes();w=int.from_bytes(raw[4:6],'big');h=int.from_bytes(raw[6:8],'big');depth=raw[8];stride=int.from_bytes(raw[10:12],'big');pal=raw[12:12+3*(1<<depth)];start=12+len(pal)
 im=Image.new('P',(w,h));im.putpalette(list(pal)+[0]*(768-len(pal)));pixels=im.load()
 for y in range(h):
  for x in range(w):pixels[x,y]=sum(((raw[start+p*stride*h+y*stride+x//8]>>(7-x%8))&1)<<p for p in range(depth))
 return im

def main():
 out=ROOT/'build/multidisk-probe/status';out.mkdir(parents=True,exist_ok=True)
 base=read_spbm(ROOT/'assets/runtime/sparkpaw-level-loading.spbm');pal=base.getpalette();assert pal[:3]==[0,0,0]
 def nearest(rgb):return min(range(64),key=lambda i:sum((pal[3*i+k]-rgb[k])**2 for k in range(3)))
 sheet=Image.open(ROOT/'assets/concept/sparkpaw-insert-disk-type-v1.png').convert('RGB')
 # Generated typography sheet -> native indexed status assets only. The shared
 # floppy artwork is never included in a disk-message runtime asset.
 bands=[(88,195,1811,336),(88,484,1811,626)]
 preview=Image.new('RGB',(960,256));preview.paste(base.convert('RGB'),(0,0))
 for disk in (1,2):
  patch=Image.new('P',(224,40),0);patch.putpalette(pal)
  letters=sheet.crop(bands[disk-1]).resize((216,24),Image.Resampling.LANCZOS)
  pix=letters.load();dst=patch.load()
  for y in range(24):
   for x in range(216):dst[x+4,y+6]=nearest(pix[x,y])
  (out/f'disk{disk}-patch.spbm').write_bytes(spbm_payload(patch,224,40))
  composed=base.copy();composed.paste(patch,(48,192));preview.paste(composed.convert('RGB'),(disk*320,0))
  # No changes outside status rectangle; exact loading palette retained.
  for y in range(256):
   for x in range(320):
    if not (48<=x<272 and 192<=y<232):assert composed.getpixel((x,y))==base.getpixel((x,y))
 preview.save(out/'preview.png')
 preview.resize((1920,512),Image.Resampling.NEAREST).save(out/'preview-2x.png')
 print(out)
if __name__=='__main__':main()
