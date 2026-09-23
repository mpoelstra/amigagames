"""Precompute approved water v1 and four small authored waterfall patches."""
from pathlib import Path
import json, struct
from PIL import Image
from preview_drowned_native import decode

R=Path(__file__).resolve().parents[1];O=R/'build/drowned-full';A=O/'assets'
A.mkdir(parents=True,exist_ok=True)
rear=decode(A/'drowned-rear.spbm');width,height=rear.size
approved=json.loads((R/'assets/concept/drowned-water-animation-v1/manifest.json').read_text())
positions=approved['positions'];wave=approved['shore_wave'];bands=approved['shore_bands']
def planar(im):
    assert im.width%16==0 and max(im.tobytes())<8
    out=bytearray()
    for p in range(3):
        for y in range(im.height):
            for x in range(0,im.width,8):
                out.append(sum(((im.getpixel((x+b,y))>>p)&1)<<(7-b) for b in range(8)))
    return bytes(out)
payload=bytearray(b'DRA1')
water_offset=len(payload);water_h=14;water_stride=width//8
for frame in range(24):
    im=rear.crop((0,185,width,199))
    for x,y,phase,length in positions:
        age=(frame+phase)%24
        if age>=10:continue
        size=max(1,((1,2,3,4,5,5,4,3,2,1)[age]*length+4)//5)
        pen=(5,6,6,7,7,7,6,6,5,5)[age]
        left=x+age//5-size//2
        for px in range(max(0,left),min(width,left+size)):im.putpixel((px,y-185),pen)
    payload.extend(planar(im))
# Source-aligned rectangles retain all surrounding original pixels. Last one
# contains the exact approved32x44 waterfall within a48px aligned rectangle.
specs=[(112,112,32,52,20,48,(23,26,29)),
       (432,132,32,44,16,39,(16,19,22)),
       (656,120,32,52,1,47,(14,17,20)),
       (1216,140,48,44,0,0,())]
falls=[]
for index,(x,y,w,h,top,bottom,lanes) in enumerate(specs):
    original=rear.crop((x,y,x+w,y+h));offset=len(payload)
    for phase in range(4):
        im=original.copy()
        if index==3:
            im.paste(Image.open(R/f'assets/concept/drowned-waterfall-preview-v1/frame-{phase}.png'),(8,0))
        else:
            for yy in range(top,bottom):
                for lane,shift in zip(lanes,(0,5,8)):
                    if original.getpixel((lane,yy))<5:continue
                    pos=(yy-phase*3+shift)%12
                    if pos<3:im.putpixel((lane,yy),6)
                    elif pos==3:im.putpixel((lane,yy),7)
            for j,lane in enumerate(lanes):
                yy=min(h-1,bottom+j%2)
                for xx in range(lane-2,min(w,lane-2+(3,5,4,2)[(phase+j)%4])):
                    if original.getpixel((xx,yy))>=4:im.putpixel((xx,yy),7 if (phase+j)%4<2 else 6)
        payload.extend(planar(im))
    falls.append(dict(x=x,y=y,w=w,h=h,stride=w//8,offset=offset,frame_bytes=w//8*h*3))
(A/'drowned-amb.bin').write_bytes(payload)
pal=[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)]
values=[]
for frame in range(24):
    row=[]
    for top,bottom,phase,strength in bands:
        delta=wave[(frame+phase)%24]*strength//3
        for pen in (4,6):
            rgb=[min(255,c+delta) for c in pal[pen]]
            row.extend([((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4),
                        ((rgb[0]&15)<<8)|((rgb[1]&15)<<4)|(rgb[2]&15)])
    values.append(row)
header=f'''/* Generated offline; approved glints/shore wave, bounded rear patches. */
#define DRA_BYTES {len(payload)}L
#define DRA_WIDTH {width}
#define DRA_WATER_OFFSET {water_offset}L
#define DRA_WATER_STRIDE {water_stride}
#define DRA_WATER_FRAME_BYTES {water_stride*water_h*3}L
#define DRA_STAGE_STRIDE 44
#define DRA_STAGE_BYTES 1848L
#define DRA_FALL_COUNT {len(falls)}
struct DrownedRearFall {{ WORD x,y,width,height,stride; LONG offset,frameBytes; }};
static const struct DrownedRearFall drownedRearFalls[DRA_FALL_COUNT]={{
'''+''.join('{%d,%d,%d,%d,%d,%dL,%dL},\n'%(f['x'],f['y'],f['w'],f['h'],f['stride'],f['offset'],f['frame_bytes']) for f in falls)+'''};
static const UWORD drownedShoreValues[24][12]={
'''+''.join('{'+','.join('0x%03x'%v for v in row)+'},\n' for row in values)+'};\n'
(O/'drowned_rear_ambience_data.h').write_text(header)
(O/'rear-ambience.json').write_text(json.dumps(dict(width=width,frames=24,frame_ticks=6,
    water_offset=water_offset,water_stride=water_stride,water_frame_bytes=water_stride*14*3,
    falls=falls,fast_bytes=len(payload),chip_stage_bytes=1848,
    status='native test candidate, performance unmeasured'),indent=2)+'\n')
print(f'Rear ambience: {len(payload)} Fast source bytes,1848 Chip stage bytes,4 falls; preview-approved palette')
