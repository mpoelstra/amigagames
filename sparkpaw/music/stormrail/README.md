# Stormrail — Iron Horizon

Original three-voice shooter track: 172 BPM, 64 bars, about 89 seconds per
loop. Driving syncopated bass, a recurring four-bar Am/F/C/G melodic hook,
brighter lead and sampled power fifths. Arrangement: eight-bar departure,
sixteen-bar theme, eight-bar arpeggio drive, eight-bar atmospheric section,
sixteen-bar returning/varied theme, eight-bar drive back to the loop start.

Paula music voices: percussion / bass / lead or chord accent. The power-fifth
sample contains its harmony in the waveform; it does not consume hidden
voices. Fourth MOD channel has no notes or commands and belongs to the same
2-voice SFX mixer as Level 1. Gameplay mixer, gains, priority and cooldown
rules are unchanged. Keep Harrier warning/fire, hurt and pickup effects clear.

Generate with generate.py using Python + NumPy. The deterministic generator
writes iron-horizon.mod, manifest.json and iron-horizon-preview.wav. Preview
is host micromod at the intended 48/64 music gain, not native ptplayer or
proof of in-game SFX balance. No external music samples or melodies used.

Runtime split: 17468 bytes score in Fast RAM, 11552 bytes music bank in Chip.
Total new music asset payload: 29020 bytes. Only the active section's track
is loaded; Copper Sprint is released before loading Iron Horizon. Music runs
continuously from departure through the Harrier fight and life resets; results
stop it, resident replay starts it afresh. No extra load at the finale.

Native sound/feel approval remains pending the user's first 030 audition.
Main HD only; no ADF/WHDLoad adoption or release is implied.
