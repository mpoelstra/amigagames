"""Convert the approved recessed-geyser family; no production asset writes."""
from collections import Counter
from pathlib import Path
import json
from PIL import Image
from generate_runtime_assets import save_spbm
from preview_drowned_native import decode
from preview_drowned_polish import PAL
from preview_drowned_jet import keyed

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'assets/concept/drowned-geyser-native-v2'
NATIVEPAL = [tuple((v >> 4) * 17 for v in c) for c in PAL]

def blank(size):
    im = Image.new('P', size, 0)
    im.putpalette([v for c in NATIVEPAL for v in c] + [0] * 720)
    return im

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    src = Image.open(ROOT / 'assets/concept/drowned-geyser-animation-source-v2.png').convert('RGB')
    # One whole-sheet scale. The generated second ROW has its baseline four
    # native pixels higher; register that entire row, never fit individual poses.
    scaled = src.resize((128,128), Image.Resampling.BOX)
    frames = []
    for i in range(8):
        cell = scaled.crop(((i%4)*32,(i//4)*64,(i%4+1)*32,(i//4+1)*64))
        small = keyed(cell, [NATIVEPAL[p] for p in (0,5,6,11)])
        frame = blank((32,64))
        pixels = bytes((0,5,6,11)[p] for p in small.tobytes())
        small.putdata(pixels)
        small.putpalette(frame.getpalette())
        frame.paste(small, (0, 4 if i >= 4 else 0))
        frames.append(frame)
    atlas = blank((32,512))
    for i,frame in enumerate(frames):
        atlas.paste(frame,(0,i*64))
    concept = Image.open(ROOT / 'assets/concept/drowned-geyser-source-v2.png').convert('RGB')
    # Fixed iron lip with both rivets, directly from the approved first panel.
    lip = keyed(concept.crop((116,674,366,724)).resize((32,6),Image.Resampling.BOX),NATIVEPAL)
    lip.putpalette(atlas.getpalette())
    for name,im in [('geyser',atlas),('lip',lip)]:
        save_spbm(OUT/(name+'.spbm'),im,NATIVEPAL,4)
        im.save(OUT/(name+'-indexed.png'))
        decoded=decode(OUT/(name+'.spbm'))
        assert decoded.tobytes()==im.tobytes()
        assert decoded.getpalette()==im.getpalette()
    sheet=blank((256,70))
    for i,frame in enumerate(frames):
        sheet.paste(frame,(i*32,0));sheet.paste(lip,(i*32,63))
    sheet.resize((1536,420),Image.Resampling.NEAREST).save(OUT/'frames-6x.png')
    counts=Counter(atlas.tobytes()); assert set(counts)<=set((0,5,6,11))
    assert counts[5]>counts[6] and counts[5]>counts[11]
    bounds=[Image.frombytes('L',f.size,bytes(255 if p else 0 for p in f.tobytes())).getbbox() for f in frames]
    assert all(b and b[0]>0 and b[2]<32 for b in bounds)
    (OUT/'manifest.json').write_text(json.dumps({
        'status':'approved direction; native Slice3 user review pending',
        'frames':8,'cell':[32,64],'source_size':src.size,
        'whole_sheet_scale':[128,128],'second_row_registration_y':4,
        'per_frame_scale':False,'bounds':bounds,'pen_counts':dict(counts),
        'water_pens':[0,5,6,11],'palette':NATIVEPAL,'plane_bytes':8192,
        'lip_size':[32,6],'world_water_origin':[448,131],'world_lip_origin':[448,194],
        'note':'Planar atlas is baked into existing 23-patch Fast source, no extra runtime allocation.'
    },indent=2)+'\n')
    print('Geyser: 8 cells, fixed scale, shared water palette, blue-dominant, SPBM roundtrip PASS')

if __name__=='__main__':
    main()
