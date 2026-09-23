import json, subprocess, sys
import numpy as np

SR=11025
N=4096
H=256

def decode(path):
    p=subprocess.run(['ffmpeg','-v','error','-i',path,'-ac','1','-ar',str(SR),'-f','f32le','-'],capture_output=True,check=True)
    return np.frombuffer(p.stdout,dtype='<f4').copy()

def analyze(path):
    y=decode(path)
    frames=np.lib.stride_tricks.sliding_window_view(y,N)[::H]
    window=np.hanning(N).astype('float32')
    freq=np.fft.rfftfreq(N,1/SR)
    mag=np.empty((len(frames),N//2+1),dtype='float32')
    for i in range(0,len(frames),256):
        mag[i:i+256]=abs(np.fft.rfft(frames[i:i+256]*window,axis=1)).astype('float32')
    times=np.arange(len(frames))*H/SR
    energy=np.sqrt(np.mean(frames*frames,axis=1))
    spec=mag[:,(freq>=50)&(freq<5000)]
    ff=freq[(freq>=50)&(freq<5000)]
    centroid=(spec@ff)/(spec.sum(axis=1)+1e-6)
    high=spec[:,ff>=2200].sum(axis=1)/(spec.sum(axis=1)+1e-6)
    # Harmonic pitch-class energy is an estimate for a full mix, not note transcription.
    chroma=np.zeros((len(frames),12),dtype='float32')
    mask=(freq>=65)&(freq<1000)
    midi=np.rint(69+12*np.log2(freq[mask]/440)).astype(int)
    weighted=mag[:,mask]/np.sqrt(freq[mask])[None,:]
    for pc in range(12):chroma[:,pc]=weighted[:,midi%12==pc].sum(axis=1)
    chroma/=chroma.sum(axis=1,keepdims=True)+1e-8
    names=['C','C#','D','Eb','E','F','F#','G','Ab','A','Bb','B']
    total=chroma[energy>np.quantile(energy,.25)].mean(axis=0)
    # Positive spectral flux with relative compression suppresses steady bass.
    band=mag[:,(freq>=80)&(freq<4500)]
    flux=np.maximum(0,np.diff(np.log1p(band),axis=0)).mean(axis=1)
    flux=np.r_[0,flux]
    flux-=np.convolve(flux,np.ones(17)/17,mode='same')
    flux=np.maximum(flux,0)
    # Autocorrelation offers several beat subdivisions; show candidate BPMs.
    x=flux-flux.mean()
    ac=[]
    for bpm in np.arange(80,201,.5):
        lag=round(60*SR/H/bpm)
        ac.append(float(np.dot(x[:-lag],x[lag:])/(np.linalg.norm(x[:-lag])*np.linalg.norm(x[lag:])+1e-9)))
    order=np.argsort(ac)[::-1]
    peaks=[]
    for j in order:
        bpm=80+j*.5
        if all(abs(bpm-p)>3 for p,_ in peaks):peaks.append((bpm,ac[j]))
        if len(peaks)==6:break
    # 8-second slices show arrangement changes without claiming instrument IDs.
    sections=[]
    section_chroma=[]
    for start in np.arange(0,len(y)/SR,8):
        ids=(times>=start)&(times<start+8)
        if not ids.any():continue
        ch=chroma[ids].mean(axis=0)
        section_chroma.append(ch)
        sections.append(dict(at_s=round(float(start),1),rms=round(float(np.sqrt(np.mean(energy[ids]**2))),3),centroid_hz=round(float(np.median(centroid[ids]))),top_pitch_classes=[names[k] for k in np.argsort(ch)[-3:][::-1]]))
    sc=np.asarray(section_chroma)
    drift=np.abs(np.diff(sc,axis=0)).sum(axis=1)
    return dict(file=path,seconds=round(len(y)/SR,2),peak_dbfs=round(float(20*np.log10(max(abs(y).max(),1e-8))),1),rms_dbfs=round(float(20*np.log10(np.sqrt(np.mean(y*y))+1e-8)),1),median_centroid_hz=round(float(np.median(centroid))),upper_band_ratio=round(float(np.median(high)),3),tonal_change_mean=round(float(drift.mean()),3),tonal_change_max=round(float(drift.max()),3),largest_tonal_shifts_s=[round(float((j+1)*8),1) for j in np.argsort(drift)[-4:][::-1]],beat_candidates=[dict(bpm=round(float(a),1),correlation=round(float(b),3)) for a,b in peaks],pitch_classes=[dict(note=names[k],share=round(float(total[k]),3)) for k in np.argsort(total)[::-1]],sections=sections)

for p in sys.argv[1:]:
    print(json.dumps(analyze(p),indent=2))
