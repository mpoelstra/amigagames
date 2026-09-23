#!/usr/bin/env python3
"""Offline full-level rear-art review; no runtime or release writes."""
import hashlib
import json
from pathlib import Path
from PIL import Image, ImageDraw
from generate_runtime_assets import save_spbm
from preview_drowned_native import decode, indexed
from prepare_drowned_layers import rgba, encoded
from pack_disk_asset import pack as lz_pack, decode as lz_decode
from pack_adf_asset import pack as rle_pack, decode as rle_decode

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT/'assets/concept/drowned-panorama-v1'
SOURCE = ROOT/'assets/concept/drowned-full-rear-source-v2.png'
NATIVE = ROOT/'assets/concept/drowned-native-v1'
CAMERAS = (0,512,1120,1760,2400,3072)
LABELS = ('Onderhoudsinlaat','Inlaat en turbinehof','Bypasshal',
          'Turbinegalerij','Hoofdregelaar: omgeving','Rustige stationomgeving')


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    protected={str(p):hashlib.sha256(p.read_bytes()).hexdigest()
        for p in (ROOT/'assets/runtime').glob('*') if p.is_file()}
    source=Image.open(SOURCE).convert('RGB')
    # Inspected letterboxed source is 2057x764, active rows 188..575.
    # Crop at the target aspect ratio, preserving round architecture.
    assert source.size==(2057,764)
    crop=(0,190,2057,572)
    art=source.crop(crop)
    assert abs((art.width/art.height)/(1120/208)-1)<0.002
    native=art.resize((1120,208),Image.Resampling.LANCZOS)
    palette=json.loads((NATIVE/'manifest.json').read_text())['rear_palette']
    rear=indexed(native,palette)
    rear.save(OUT/'rear-indexed.png')
    rear.resize((2240,416),Image.Resampling.NEAREST).save(OUT/'panorama-2x.png')
    save_spbm(OUT/'drowned-rear.spbm',rear,palette,3)
    actual=decode(OUT/'drowned-rear.spbm')
    assert actual.tobytes()==rear.tobytes()
    assert actual.getpalette()==rear.getpalette()
    assert max(rear.tobytes())<8
    assert all(c%17==0 for rgb in palette for c in rgb)
    for cam in range(3073):
        origin=cam//4
        assert origin+320<=rear.width
        assert origin+352<=rear.width
    front=rgba(Image.open(NATIVE/'front-indexed.png'))
    hud=Image.open(NATIVE/'preview-1x.png').convert('RGB').crop((0,208,320,256))
    player=rgba(decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48)))
    def frame(cam,reference=False):
        image=Image.new('RGB',(320,256))
        image.paste(rear.convert('RGB').crop((cam//4,0,cam//4+320,208)),(0,0))
        if reference:
            image.paste(front,(0,0),front)
            image.paste(player,(62,152),player)
        image.paste(hud,(0,208))
        assert image.crop((0,208,320,256)).tobytes()==hud.tobytes()
        return image
    sheet=Image.new('RGB',(960,552),(15,25,35))
    draw=ImageDraw.Draw(sheet)
    for i,(cam,label) in enumerate(zip(CAMERAS,LABELS)):
        image=frame(cam)
        image.save(OUT/f'scene-{i+1}-1x.png')
        frame(cam,True).save(OUT/f'scene-{i+1}-scale-1x.png')
        x=(i%3)*320;y=(i//3)*276
        sheet.paste(image,(x,y+20))
        draw.text((x+6,y+4),f'{i+1}. {label}',fill=(210,226,235))
    sheet.save(OUT/'route-overview.png')
    raw=(OUT/'drowned-rear.spbm').read_bytes()
    rle=rle_pack(raw);lz=lz_pack(raw)
    assert rle_decode(rle)==raw and lz_decode(lz)==raw
    sizes={'raw_spbm':len(raw),'SPR1':len(rle),'SPL1':len(lz)}
    sources=dict(rear=encoded(rear),hud=encoded(hud),front=encoded(front),player=encoded(player))
    html='''<!doctype html><html lang="nl"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Drowned Turbines — volledige achtergrond</title>
<style>body{max-width:1120px;margin:24px auto;padding:0 20px;background:#101b25;color:#dce7ef;font:16px system-ui}h1{font-size:26px}#view{width:min(100%,800px);display:block;margin:auto;image-rendering:pixelated}#map{width:100%;image-rendering:pixelated;margin-top:20px}button{background:#294451;color:inherit;padding:10px;border:1px solid #628897;border-radius:5px;margin:5px}input[type=range]{width:min(65%,650px)}small{color:#acc0cc}nav{margin:14px 0}</style>
<h1>Drowned Turbines · volledige achtergrond</h1>
<p>3392 pixels levelruimte · 1120 × 208 achtergrond · acht kleuren · kwart scroll</p>
<canvas id="view" width="320" height="256"></canvas>
<nav id="scenes"></nav>
<button id="play">Automatisch scrollen</button><input id="camera" type="range" min="0" max="3072" value="0" aria-label="Camerapositie"><output id="position">0</output>
<p><label><input id="reference" type="checkbox"> Toon bestaande voorgrond en Sparkpaw als vaste schaalreferentie</label></p>
<canvas id="map" width="1120" height="208"></canvas>
<p id="chapter"></p><small>Alleen achtergrondreview. Het huisje komt apart in de voorgrond, de Core als Bob. De optionele vaste voorgrond is geen uitgewerkt volledig level. HUD ongewijzigd. Geen native performance- of gameplaytest.</small>
<script>const sources=ASSETS,cameras=CAMERAS,labels=LABELS,images={};const view=document.querySelector('#view'),ctx=view.getContext('2d'),map=document.querySelector('#map'),m=map.getContext('2d'),slider=document.querySelector('#camera'),toggle=document.querySelector('#reference'),button=document.querySelector('#play');ctx.imageSmoothingEnabled=false;m.imageSmoothingEnabled=false;let cam=0,auto=false,direction=1,last=0;
function pause(){auto=false;button.textContent='Automatisch scrollen'}
cameras.forEach((c,i)=>{const b=document.createElement('button');b.textContent=labels[i];b.onclick=()=>{pause();cam=c;slider.value=c};document.querySelector('#scenes').appendChild(b)});
slider.oninput=()=>{pause();cam=+slider.value};button.onclick=()=>{auto=!auto;button.textContent=auto?'Pauzeren':'Automatisch scrollen'};
Promise.all(Object.entries(sources).map(([k,src])=>new Promise((resolve,reject)=>{const im=new Image();im.onload=()=>{images[k]=im;resolve()};im.onerror=reject;im.src=src}))).then(()=>requestAnimationFrame(draw));
function draw(t){const dt=last?Math.min(50,t-last):0;last=t;if(auto){cam+=direction*dt*.12;if(cam>=3072){cam=3072;direction=-1}if(cam<=0){cam=0;direction=1}slider.value=Math.floor(cam)}const c=Math.floor(cam),r=Math.floor(c/4);ctx.drawImage(images.rear,r,0,320,208,0,0,320,208);if(toggle.checked){ctx.drawImage(images.front,0,0);ctx.drawImage(images.player,62,152)}ctx.drawImage(images.hud,0,208);m.drawImage(images.rear,0,0);m.fillStyle='#0008';m.fillRect(0,0,r,208);m.fillRect(r+320,0,1120-r-320,208);m.strokeStyle='#ffdf8f';m.lineWidth=2;m.strokeRect(r+1,1,318,206);document.querySelector('#position').textContent=c+' / 3072';document.querySelector('#chapter').textContent='Achtergrondvenster '+r+'–'+(r+319)+' · overlappende zichten, geen zes afzonderlijke kamers';requestAnimationFrame(draw)}</script></html>'''
    (OUT/'index.html').write_text(html.replace('ASSETS',json.dumps(sources)).replace('CAMERAS',json.dumps(CAMERAS)).replace('LABELS',json.dumps(LABELS)))
    report=dict(status='full-level background concept, native review pending, no runtime integration',
        world_width=3392,camera_max=3072,rear_scroll_max=768,visible_width=320,
        conservative_fetch_width=352,rear_dimensions=[1120,208],source_crop=crop,
        palette=palette,palette_policy='same fixed eight pens as accepted native V1',
        source_planar_bytes=87360,source_stride=140,
        guarded_stride_lower_bound=142,guarded_planar_lower_bound=88608,
        note='graphics.library may pad physical stride; total residency includes source and guarded copy, renderer/HUD/sprites/audio separately',
        stored_bytes=sizes,checks=['all 3073 camera origins fit visible and conservative fetch',
            'three-plane SPBM exact index/palette round-trip','SPR1 and SPL1 decode to identical SPBM',
            'HUD unchanged across six example compositions','runtime asset hashes unchanged'],
        scenes=[dict(camera=c,rear_origin=c//4,label=l) for c,l in zip(CAMERAS,LABELS)],
        limitations=['static concept, not collision or gameplay','no native Copper or palette-consumer integration',
            'no native memory or FPS measurement','not an ADF filesystem fit proof',
            'station and Core deliberately absent from rear'],
        source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest())
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==v for p,v in protected.items())
    (OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__=='__main__':main()
