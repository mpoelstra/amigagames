#!/usr/bin/env python3
"""Prepare an offline 960px layer/water study, never production assets."""
import base64
import hashlib
import io
import json
from pathlib import Path
from PIL import Image
from generate_runtime_assets import FRONT16, save_spbm, water_animation_pen
from preview_drowned_native import decode, indexed

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'assets/concept/drowned-layers-v1'
OLD = ROOT / 'assets/concept/drowned-native-v1'
REAR_SOURCE = ROOT / 'assets/concept/drowned-rear-clean-source-v1.png'
WATERS = (240, 608)
PLATFORMS = ((128,144),(432,128),(720,160))


def rgba(image):
    result = image.convert('RGBA')
    result.putalpha(Image.frombytes('L',image.size,
        bytes(255 if p else 0 for p in image.tobytes())))
    return result


def encoded(image):
    stream = io.BytesIO()
    image.save(stream,format='PNG')
    return 'data:image/png;base64,'+base64.b64encode(stream.getvalue()).decode()


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    protected = {str(p):hashlib.sha256(p.read_bytes()).hexdigest()
        for folder in [ROOT/'assets/runtime',ROOT/'dist']
        for p in folder.glob('*') if p.is_file()}
    palette = json.loads((OLD/'manifest.json').read_text())['rear_palette']
    # 512 covers all 960px world camera phases at quarter speed, including
    # a 352px fetch at camera=640 (160+352=512). No repeat join is needed.
    rear = indexed(Image.open(REAR_SOURCE).convert('RGB').resize((512,208),
        Image.Resampling.LANCZOS),palette)
    oldfront = Image.open(OLD/'front-indexed.png')
    front = Image.new('P',(960,208),0)
    front.putpalette(oldfront.getpalette())
    floor = oldfront.crop((0,200,120,208))
    for x in range(0,960,120):
        front.paste(floor,(x,200))
    # Reuse the approved indexed platform material at its exact native scale.
    # This is a three-placement blockout; a richer final kit remains later art.
    cap = oldfront.crop((216,140,320,156))
    support = oldfront.crop((242,156,254,200))
    for x,y in PLATFORMS:
        front.paste(cap,(x,y))
        height = 200-y-16
        for offset in range(0,height,44):
            strip = support.crop((0,0,12,min(44,height-offset)))
            front.paste(strip,(x+28,y+16+offset))
    for x in WATERS:
        front.paste(0,(x,197,x+80,208))
    water = Image.new('P',(80,176),0)
    water.putpalette(oldfront.getpalette())
    for f in range(16):
        for y in range(11):
            for x in range(80):
                water.putpixel((x,f*11+y),water_animation_pen(f,x,y))
    for name,img,pal,depth in [('front',front,FRONT16,4),
                               ('rear',rear,palette,3),
                               ('water',water,FRONT16,4)]:
        img.save(OUT/(name+'-indexed.png'))
        save_spbm(OUT/(name+'.spbm'),img,pal,depth)
        actual = decode(OUT/(name+'.spbm'))
        assert actual.tobytes()==img.tobytes()
        assert actual.getpalette()==img.getpalette()
        assert max(img.tobytes()) < 1<<depth
    hud = Image.open(OLD/'preview-1x.png').convert('RGB').crop((0,208,320,256))
    player = rgba(decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48)))
    front_rgba, water_rgba = rgba(front), rgba(water)
    def frame(camera,phase):
        out=Image.new('RGB',(320,256))
        out.paste(rear.convert('RGB').crop((camera//4,0,camera//4+320,208)),(0,0))
        fg=front_rgba.crop((camera,0,camera+320,208))
        out.paste(fg,(0,0),fg)
        for wx in WATERS:
            part=water_rgba.crop((0,phase*11,80,phase*11+11))
            out.paste(part,(wx-camera,197),part)
        if camera<128:
            out.paste(player,(64-camera,152),player)
        out.paste(hud,(0,208))
        assert out.crop((0,208,320,256)).tobytes()==hud.tobytes()
        return out
    for i,camera in enumerate((0,320,640)):
        frame(camera,0).save(OUT/f'view-{i+1}-1x.png')
        frame(camera,0).resize((1280,1024),Image.Resampling.NEAREST).save(OUT/f'view-{i+1}-4x.png')
    # Exhaust every integer camera origin, not merely the displayed examples.
    for camera in range(641):
        assert camera+320<=front.width
        assert camera//4+352<=rear.width
    for phase in range(16):
        assert all(water.getpixel((x,phase*11+y))==water_animation_pen(phase,x,y)
            for y in range(11) for x in range(80))
        frame(160,phase)
    imgs={'rear':encoded(rear),'front':encoded(front_rgba),
          'water':encoded(water_rgba),'hud':encoded(hud),'player':encoded(player)}
    html='''<!doctype html><html lang="nl"><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Drowned Turbines — lagenstudie</title>
<style>body{margin:28px auto;max-width:1000px;padding:0 20px;background:#101b25;color:#dae6ed;font:17px system-ui}h1{font-size:26px}canvas{width:min(100%,960px);image-rendering:pixelated;background:black}input{width:min(65%,600px)}button{padding:9px 16px;margin:12px;color:#dae6ed;background:#294453;border:1px solid #668898;border-radius:5px}small{color:#aac1ce}</style>
<h1>Drowned Turbines · terrein, parallax en water</h1>
<p>Dezelfde HUD en waterframes als de bestaande basis. Achtergrond beweegt op kwart snelheid.</p>
<canvas width="320" height="256" id="scene"></canvas>
<div><button id="play">Automatisch scrollen</button><input id="camera" type="range" min="0" max="640" value="0" aria-label="Camerapositie"><output id="pos">0 / 640</output></div>
<small>Offline artstudie, geen speelbare Amiga-build. De speler staat op wereldpositie 64 als schaalreferentie. Geen botsing, vijanden, drukjets of muziektest.</small>
<script>const sources=ASSETS;const images={};const canvas=document.querySelector('#scene'),ctx=canvas.getContext('2d');ctx.imageSmoothingEnabled=false;
const slider=document.querySelector('#camera'),pos=document.querySelector('#pos'),button=document.querySelector('#play');let auto=false,cam=0,direction=1,last=0;
button.onclick=()=>{auto=!auto;button.textContent=auto?'Scrollen pauzeren':'Automatisch scrollen'};slider.oninput=()=>{cam=Number(slider.value);auto=false;button.textContent='Automatisch scrollen'};
Promise.all(Object.entries(sources).map(([key,src])=>new Promise((resolve,reject)=>{const im=new Image();im.onload=()=>{images[key]=im;resolve()};im.onerror=reject;im.src=src}))).then(()=>requestAnimationFrame(draw));
function draw(t){const delta=last?Math.min(50,t-last):0;last=t;if(auto){cam+=direction*delta*.045;if(cam>=640){cam=640;direction=-1}if(cam<=0){cam=0;direction=1}slider.value=Math.floor(cam)}const c=Math.floor(cam),phase=Math.floor(t/40)%16;ctx.drawImage(images.rear,Math.floor(c/4),0,320,208,0,0,320,208);ctx.drawImage(images.front,c,0,320,208,0,0,320,208);for(const x of [240,608])ctx.drawImage(images.water,0,phase*11,80,11,x-c,197,80,11);if(c<128)ctx.drawImage(images.player,64-c,152);ctx.drawImage(images.hud,0,208);pos.textContent=c+' / 640';requestAnimationFrame(draw)}</script></html>'''
    (OUT/'index.html').write_text(html.replace('ASSETS',json.dumps(imgs)))
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==value for p,value in protected.items())
    report=dict(status='offline layer study; not a playable slice',world=[960,208],rear=[512,208],
        camera_range=[0,640],fetch_width=352,water_x=list(WATERS),water_y=197,
        water_frames=16,water_frame_ticks=2,hud_y=208,hud_changed=False,
        platforms=[dict(x=x,y=y,width=104) for x,y in PLATFORMS],
        front_palette='unchanged FRONT16',rear_palette=palette,
        planar_bytes=dict(front=99840,rear=39936,water=7040,total=146816),
        verified=['641 camera origins have complete rear fetch and front coverage',
                  'all 14080 water pixels match existing generator formula',
                  'SPBM pixel and palette round-trip',
                  'HUD unchanged for all 16 tested water phases',
                  'existing runtime assets and dist top-level files hash-unchanged'],
        not_verified=['native scrolling/Copper timing','physics or route reachability',
                      'new enemy art, pressure/sluis interaction or soundtrack',
                      'complete Chip/Fast allocation peak or ADF fit'],
        source_sha256=hashlib.sha256(REAR_SOURCE.read_bytes()).hexdigest())
    (OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__=='__main__':main()
