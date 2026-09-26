#!/usr/bin/env python3
"""Freeze approved v3 indexed frames as complete planar patches for an HD test."""
import hashlib,json
from pathlib import Path
from PIL import Image
import preview_level1_rear_ambience as preview

R=Path(__file__).resolve().parents[1];O=R/'build/level1-electric-v3-20260926'
S=R/'assets/concept/level1-rear-ambience-study-v3'
def planar(im):
    w,h=im.size;assert w%16==0
    values=list(im.get_flattened_data());out=bytearray()
    for plane in range(3):
        for y in range(h):
            for x in range(0,w,8):
                out.append(sum(((values[y*w+x+k]>>plane)&1)<<(7-k) for k in range(8)))
    return bytes(out)
def main():
    (O/'assets').mkdir(parents=True,exist_ok=True)
    frames=[Image.open(S/f'frame-{i:02d}-indices.png').copy() for i in range(48)]
    original,_=preview.decode(preview.REAR);base=preview.indexed_image(original)
    manifest=json.loads((S/'manifest.json').read_text())
    assert hashlib.sha256(preview.REAR.read_bytes()).hexdigest()==manifest['source_sha256']
    rects=[(192,24,48,64)]+[(((x-1)//16)*16,y,64,24) for x,y in preview.SITES]
    blob=bytearray(b'L1A3');patches=[]
    for x,y,w,h in rects:
        unique=[];states=[];images=[]
        for frame in frames:
            im=frame.crop((x,y,x+w,y+h));raw=planar(im)
            if raw not in unique: unique.append(raw);images.append(im)
            states.append(unique.index(raw))
        patches.append(dict(x=x,y=y,width=w,height=h,stride=w//8,frame_bytes=w//8*h*3,
                            offset=len(blob),states=states,count=len(unique),images=images))
        blob.extend(b''.join(unique))
    # Every pixel of every reviewed rear frame must be reconstructed by patches.
    for i,frame in enumerate(frames):
        rebuilt=base.copy()
        for p in patches: rebuilt.paste(p['images'][p['states'][i]],(p['x'],p['y']))
        assert rebuilt.tobytes()==frame.tobytes(),i
    (O/'assets/l1-electric.bin').write_bytes(blob)
    h=['/* Generated from approved v3; complete opaque patches, no new pens. */',
       '#define L1_REAR_PATCHES 9','#define L1_REAR_PHASES 48',
       f'#define L1_REAR_BYTES {len(blob)}UL','#define L1_REAR_STAGE_BYTES 1152UL',
       'struct L1RearPatch { UWORD x,y,stride,height,frameBytes; ULONG offset; UBYTE states[48]; };',
       'static const struct L1RearPatch l1RearPatches[L1_REAR_PATCHES]={']
    for p in patches:
        h.append('{%d,%d,%d,%d,%d,%dUL,{%s}},'%(p['x'],p['y'],p['stride'],p['height'],p['frame_bytes'],p['offset'],','.join(map(str,p['states']))))
    h+=['};']
    (O/'level1_rear_ambience_data.h').write_text('\n'.join(h)+'\n')
    for p in patches:del p['images']
    report={'bytes':len(blob),'sha256':hashlib.sha256(blob).hexdigest(),'patches':patches,
            'review_reconstruction':'all 48 complete rear images match exactly'}
    (O/'asset-manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['bytes'],report['sha256'],report['review_reconstruction'])
if __name__=='__main__':main()
