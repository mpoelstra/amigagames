"""All full-patch transitions must match compact transfer pixels exactly."""
from pathlib import Path
import sys
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from preview_drowned_native import decode
p=ROOT/'build/drowned-slice/assets'
reference=Image.open(p/'drowned-patches.png')
packed=decode(p/'drowned-patches.spbm')
cursor=0
for f in range(23):
    box=(0,19,32,83) if f<9 else (0,24,80,85)
    width,height=box[2]-box[0],box[3]-box[1]
    rows=width*height//16
    cell=Image.frombytes('P',(width,height),packed.crop((0,cursor,16,cursor+rows)).tobytes())
    cursor+=rows
    expected=reference.crop((0,f*96,96,(f+1)*96))
    assert cell.tobytes()==expected.crop(box).tobytes()
    for prior in (range(9) if f<9 else range(9,23)):
        changed=reference.crop((0,prior*96,96,(prior+1)*96))
        changed.paste(cell,box[:2])
        assert changed.tobytes()==expected.tobytes(),(prior,f)
assert cursor==packed.height==5422
print('PASS: all 277 full-frame transitions equal compact planar patches, including static surrounds')
