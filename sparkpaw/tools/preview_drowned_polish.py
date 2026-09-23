"""Offline native-size art review. No runtime or dist output."""
from pathlib import Path
import json, hashlib
from PIL import Image
from preview_drowned_native import decode,indexed
from generate_runtime_assets import FRONT16,save_spbm,water_animation_pen
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'assets/concept/drowned-polish-native-v1'
SOURCE=ROOT/'assets/concept/drowned-polish-parts-source-v1.png'
# Review palette: shared base pens retained; purple extension replaced by copper.
PAL=FRONT16[:8]+[(34,43,51),(68,82,91),(119,136,145),(221,225,221),
                 (68,43,26),(119,77,43),(187,128,68),(238,187,119)]
def main():
 OUT.mkdir(parents=True,exist_ok=True)
 protected={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for base in [ROOT/'assets/runtime',ROOT/'dist'] for p in base.rglob('*') if p.is_file()}
 src=Image.open(SOURCE).convert('RGB')
 front=Image.new('P',(320,208),0)
 front.putpalette([v for c in PAL for v in c]+[0]*720)
 def part(box,size,at):
  rgb=src.crop(box).resize(size,Image.Resampling.BOX)
  im=indexed(rgb,PAL,first=1)
  # The source uses black empty space; preserve silhouette, not luminance alpha.
  mask=Image.frombytes('L',size,bytes(255 if max(c)>18 else 0 for c in rgb.getdata()))
  front.paste(im,at,mask)
 # Keep top of cap at y160, floor at y200. Rail is decorative, not collision.
 part((0,391,380,818),(64,40),(0,160))
 # Include the integrated gauge/pipe/frame. Intended visual envelope only;
 # the exact inner shutter collider must be authored against reviewed pixels.
 part((1040,308,1554,818),(80,88),(228,112))
 part((0,818,376,878),(80,8),(0,200))
 part((703,818,1554,878),(160,8),(160,200))
 front.save(OUT/'front-indexed.png');save_spbm(OUT/'front.spbm',front,PAL,4)
 rear=decode(ROOT/'assets/concept/drowned-panorama-v1/drowned-rear.spbm').crop((160,0,480,208))
 rear.save(OUT/'rear-indexed.png')
 rp=[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)]
 save_spbm(OUT/'rear.spbm',rear,rp,3)
 frame=rear.convert('RGB')
 mask=Image.frombytes('L',front.size,bytes(255 if v else 0 for v in front.tobytes()))
 frame.paste(front.convert('RGB'),(0,0),mask)
 water=Image.new('P',(80,11));water.putpalette(front.getpalette())
 for y in range(11):
  for x in range(80):water.putpixel((x,y),water_animation_pen(0,x,y))
 frame.paste(water.convert('RGB'),(80,197))
 player=decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48))
 alpha=Image.frombytes('L',player.size,bytes(255 if v else 0 for v in player.tobytes()))
 frame.paste(player.convert('RGB'),(174,152),alpha)
 hud=Image.open(ROOT/'assets/concept/drowned-native-v1/preview-1x.png').convert('RGB').crop((0,208,320,256))
 full=Image.new('RGB',(320,256));full.paste(frame,(0,0));full.paste(hud,(0,208))
 full.save(OUT/'preview-1x.png');full.resize((1280,1024),Image.Resampling.NEAREST).save(OUT/'preview-4x.png')
 for name,im in [('front',front),('rear',rear)]:
  actual=decode(OUT/(name+'.spbm'));assert actual.tobytes()==im.tobytes();assert actual.getpalette()==im.getpalette()
 assert full.crop((0,208,320,256)).tobytes()==hud.tobytes()
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==v for p,v in protected.items())
 (OUT/'manifest.json').write_text(json.dumps({'status':'native-size indexed static art review; NOT runtime accepted',
  'source':str(SOURCE.relative_to(ROOT)),'front_palette':PAL,'front_planes':4,'rear_planes':3,
  'playfield':[320,208],'hud_y':208,'floor_y':200,'player_cell':[48,48],
  'raw_planar_bytes':{'front':33280,'rear':24960},
  'limitations':['rear unchanged cropped full-level master','component adaptation, not final map/collision',
  'new foreground pens 8..15 require shared Bob/effect audit','no animation/timing/RAM allocation claim'],
  'protected_files':len(protected)},indent=2)+'\n')
 print('Verified indexed SPBM roundtrip, unchanged HUD, player source and runtime/dist hashes')
if __name__=='__main__':main()
