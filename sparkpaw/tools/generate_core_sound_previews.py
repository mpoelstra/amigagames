#!/usr/bin/env python3
"""Create review-only WAV alternatives for the Stormstone Core reward."""
from pathlib import Path
import math
import random
import wave

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs/concepts/story-intro/assets/core-sound-previews"
RATE = 11025


def render(values: list[float]) -> list[int]:
    peak=max(1.0,max(abs(v) for v in values))
    return [int(max(-127,min(127,v*118/peak))) for v in values]


def write(name: str, values: list[float]) -> None:
    pcm=render(values)
    with wave.open(str(OUT/name),"wb") as wav:
        wav.setnchannels(1); wav.setsampwidth(1); wav.setframerate(RATE)
        wav.writeframes(bytes(value+128 for value in pcm))


def tone(freq: float, start: float, duration: float, volume: float=1.0,
         square: bool=False) -> list[float]:
    values=[0.0]*int(RATE*1.15)
    begin=int(start*RATE); length=int(duration*RATE)
    for i in range(length):
        phase=math.sin(2*math.pi*freq*i/RATE)
        if square: phase=1.0 if phase>=0 else -1.0
        env=min(1.0,i/(RATE*.008))*max(0.0,1-i/length)**1.3
        values[begin+i]+=phase*env*volume
    return values


def mix(*tracks: list[float]) -> list[float]:
    return [sum(values) for values in zip(*tracks)]


def raw_current() -> None:
    raw=(ROOT/"sfx/raw/stormstone-core.raw").read_bytes()
    signed=[value if value<128 else value-256 for value in raw]
    with wave.open(str(OUT/"00-current-stormstone-core.wav"),"wb") as wav:
        wav.setnchannels(1); wav.setsampwidth(1); wav.setframerate(RATE)
        wav.writeframes(bytes((value+128)&255 for value in signed))


def main() -> None:
    OUT.mkdir(parents=True,exist_ok=True)
    raw_current()
    write("01-arcade-fanfare.wav",mix(
        tone(523,.00,.18,.75,True),tone(659,.16,.18,.78,True),
        tone(784,.32,.22,.82,True),tone(1047,.52,.46,1.0,True),
        tone(2093,.54,.34,.22)))
    storm_triumph=mix(
        tone(392,.00,.22,.55),tone(523,.14,.24,.65),
        tone(659,.29,.27,.72),tone(784,.46,.52,.92),
        tone(1568,.48,.42,.3),tone(98,.00,.18,.3))
    write("02-storm-triumph.wav",storm_triumph)
    (ROOT/"sfx/raw/stormstone-core-triumph.raw").write_bytes(
        bytes(value & 255 for value in render(storm_triumph)))
    rng=random.Random(0xB1A4D4)
    sparkle=[0.0]*int(RATE*1.15)
    for n in range(9):
        start=int((.08+n*.075)*RATE)
        for i in range(int(.07*RATE)):
            sparkle[start+i]+=(math.sin(2*math.pi*(1250+n*95)*i/RATE)*
                               max(0,1-i/(.07*RATE))*.3)
    for i in range(int(.035*RATE)):
        sparkle[i]+=(rng.random()*2-1)*(1-i/(.035*RATE))*.35
    write("03-arcade-crystal-jackpot.wav",mix(
        tone(660,.00,.13,.65,True),tone(880,.11,.14,.7,True),
        tone(1109,.22,.16,.75,True),tone(1320,.36,.5,.9,True),sparkle))
    print(f"wrote four previews to {OUT}")


if __name__ == "__main__":
    main()
