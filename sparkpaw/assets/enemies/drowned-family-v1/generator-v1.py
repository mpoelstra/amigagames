"""Candidate native families, using approved pixels and precomputed joint poses.
No animation calculation on Amiga. Level 1 sheets remain untouched.
"""
import json, math
from PIL import Image, ImageDraw
from preview_drowned_enemy_walk import ROOT, IN, cut, transform, knee, walker_frames, crab_frames
from preview_drowned_enemy_actions import walker_actions, crab_actions, folded
from generate_runtime_assets import save_spbm, bitmap_mask
from preview_drowned_native import decode
OUT=ROOT/'assets/enemies/drowned-family-v1'
RUNTIME=ROOT/'build/drowned-slice/assets'
PAL=json.loads((IN/'manifest.json').read_text())['palette']
LOOKUP={tuple(c):i for i,c in enumerate(PAL)}

def mirror(im):return im.transpose(Image.Transpose.FLIP_LEFT_RIGHT)

def airborne(src,lift):
    # Same tank, thighs, shins and boots, with feet tucked toward the hips.
    out=Image.new('RGBA',src.size)
    for h,k,f,u,l,feet in [((34,39),(35,49),(39,59),(28,37,44,51),(29,47,44,58),(28,58,50,63)),((23,39),(18,47),(20,59),(11,37,29,50),(11,46,29,58),(8,58,29,63))]:
        foot=(f[0],f[1]-lift);j=knee(h,foot,math.dist(h,k),math.dist(k,f))
        out.alpha_composite(transform(cut(src,l),k,f,j,foot))
        out.alpha_composite(transform(cut(src,u),h,k,h,j))
        out.alpha_composite(cut(src,feet),(0,-lift))
    out.alpha_composite(cut(src,(0,0,64,40)))
    return out

def rotor(im,phase):
    # Four tiny authored blade-light patterns inside the existing dark hub.
    out=im.copy();d=ImageDraw.Draw(out)
    d.rectangle((12,13,15,16),fill=tuple(PAL[8])+(255,))
    pts=[[(13,13),(14,16)],[(14,13),(13,16)],[(15,14),(12,15)],[(15,15),(12,14)]][phase%4]
    for p in pts:d.point(p,fill=tuple(PAL[10])+(255,))
    d.point((14,14),fill=tuple(PAL[11])+(255,))
    return out

def front_pivot(src):
    # Native front-facing assembly: retain shell/tank pixel clusters, move
    # the gauge onto the centerline, and draw the two load-bearing legs.
    # No whole-pose scaling or transparent crossfade through the actor.
    out=Image.new('RGBA',src.size)
    if src.width==64:
        brace=folded(src,3)
        leg=cut(brace,(8,40,29,63))
        out.alpha_composite(leg,(2,0));out.alpha_composite(mirror(leg),(-2,0))
        # Half of the tank's curved lit surface, reflected around its front.
        panel=src.crop((20,14,34,40))
        out.alpha_composite(panel,(18,17))
        out.alpha_composite(mirror(panel),(32,17))
        pipe=src.crop((13,14,20,35))
        out.alpha_composite(pipe,(12,18));out.alpha_composite(mirror(pipe),(45,18))
        out.alpha_composite(src.crop((16,5,27,17)),(27,6))
        d=ImageDraw.Draw(out)
        # Recessed center seam and one sensor retain steel/cyan identity.
        d.line((31,23,31,35),fill=tuple(PAL[8])+(255,))
        d.line((30,26,33,26),fill=tuple(PAL[6])+(255,))
    else:
        left=cut(src,(0,6,16,24))
        out.alpha_composite(left);out.alpha_composite(mirror(left))
    return out

def families():
    w=Image.open(IN/'walker-rgba.png').convert('RGBA');c=Image.open(IN/'crab-rgba.png').convert('RGBA')
    wa=walker_actions(w);ca=crab_actions(c)
    # Existing Walker slots 0..27 keep their semantic mapping; turn appends.
    wt=[folded(w,1),front_pivot(w),mirror(front_pivot(w)),mirror(folded(w,1))]
    wf=walker_frames(w)+[wt[0]]+wa['charge']+wa['fire']+wa['hit']
    wf += [folded(w,1),folded(w,3),airborne(w,5),airborne(w,2),folded(w,4),folded(w,1)]+wa['death']+wt
    # Crab: 8 walk, reserved 8/9, 4 death, 4 pivot, 4 hit.
    cw=[rotor(f,i) for i,f in enumerate(crab_frames(c))]
    ct=[cw[1],front_pivot(c),mirror(front_pivot(c)),mirror(cw[1])]
    cf=cw+[c,c]+ca['death']+ct+ca['hit']
    assert len(wf)==32 and len(cf)==22
    return {'pump-walker':wf,'turbine-crab':cf}

def indexed(im):
    out=Image.new('P',im.size);out.putpalette([v for c in PAL for v in c]+[0]*720)
    out.putdata([LOOKUP[p[:3]] if p[3] else 0 for p in im.getdata()])
    return out

def main():
    OUT.mkdir(exist_ok=True);RUNTIME.mkdir(parents=True,exist_ok=True)
    report={}
    for name,frames in families().items():
        w,h=frames[0].size;n=len(frames)
        sheet=Image.new('RGBA',(w*2,h*n))
        left_first=name=='turbine-crab'
        for i,f in enumerate(frames):
            for side,im in enumerate([mirror(f),f] if left_first else [f,mirror(f)]):sheet.alpha_composite(im,(side*w,i*h))
        ix=indexed(sheet);ix.save(OUT/(name+'.png'),transparency=0)
        path=RUNTIME/(name+'.spbm');save_spbm(path,ix,PAL,4,bitmap_mask(ix))
        assert decode(path).tobytes()==ix.tobytes()
        # Audit exact mirrors and occupied bounds without per-frame fitting.
        bounds=[]
        for i in range(n):
            a=ix.crop((0,i*h,w,(i+1)*h));b=ix.crop((w,i*h,w*2,(i+1)*h))
            assert mirror(a).tobytes()==b.tobytes()
            bounds.append(a.point(lambda p:255 if p else 0).getbbox())
        contact=Image.new('RGBA',(w*8,h*((n+7)//8)))
        for i,f in enumerate(frames):contact.alpha_composite(f,((i%8)*w,(i//8)*h))
        contact.resize((contact.width*3,contact.height*3),Image.Resampling.NEAREST).save(OUT/(name+'-contact.png'))
        turn=frames[28:32] if w==64 else frames[14:18]
        seq=[frames[0]]*8+turn+[mirror(frames[0])]*8+[mirror(f) for f in turn]
        gif=[]
        for f in seq:
            bg=Image.new('RGBA',f.size,(17,24,32,255));bg.alpha_composite(f)
            gif.append(bg.convert('RGB').resize((w*6,h*6),Image.Resampling.NEAREST))
        gif[0].save(OUT/(name+'-turn.gif'),save_all=True,append_images=gif[1:],duration=60,loop=0)
        words=w//16+1
        report[name]={'frames':n,'cell':[w,h],'source_left_first':left_first,'bounds':bounds,'spbm_bytes':path.stat().st_size,'cache_bytes':2*n*h*words*2*5,'cache_domain':'Fast' if w==64 else 'Chip'}
    (OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))
if __name__=='__main__':main()
