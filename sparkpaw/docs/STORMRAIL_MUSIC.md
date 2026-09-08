# Stormrail music — Iron Horizon

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


9 September2026, unnumbered main-HD-game candidate. User explicitly requested
catchy, atmospheric/epic shooter accompaniment plus existing SFX, following a
general positive report for the main-game Level1 integration. Requested prior
configuration was030; individual transitions and020/real-hardware acceptance
are not inferred from “lijkt allemaal goed”.

## Composition and budget

Original172 BPM,64 bars, nominal89.30s (host micromod preview89.16s due to
sample-clock tick rounding). Am/F/C/G hook; eight-bar departure,16-bar theme,
eight-bar arpeggio drive, eight-bar atmospheric bridge,16-bar varied return,
eight-bar drive into the loop. Percussion/bass/lead-or-chord use three voices.
Harmonic power fifth is baked into one sample; no extra runtime music mixing.
Fourth MOD channel is entirely empty including effects. Bass and rhythm keep
moving during the bridge. A new brighter lead distinguishes it from Level1.
Subjective catchiness/epic character and in-game SFX clarity await user audition.

MOD/source/generator/manifest/host preview: `music/stormrail/`. Module29020
bytes, split into17468 Fast score +11552 Chip bank. Mixer adds the existing224
Chip output bytes and48130 Fast scaled effects. Original Chip SFX remain for
results; code/allocator overhead is additional. Copper Sprint is unloaded
before Iron Horizon loads, so the two scores/banks are never co-resident.
No ADF space claim; alpha.4 Disk1 still has only4.5KiB free. No HD compression.

## Integration

The already-integrated backend now takes a section selector at load. Its
historical level1Audio* API names remain to avoid unrelated refactoring.
Main HD source loads `rail-score.bin`/`rail-bank.bin` for Stormrail and the
unchanged `pulse-*` pair for Level1. Same ptplayer assembly, 48/64 music gain,
block mixer, all16 effect mappings/priorities/gains/cooldowns and interrupt
lifecycle. No per-frame section lookup was added to the mixer or player.

Start after title release; continue from departure through Harrier with no
new disk access or timer-owner change. Life reset does not restart the track.
Results quiesce CIA-B and AUD3, restore the vector and use original tally SFX;
resident replay reinitializes the song. Escape/back-to-title restore Neon Sky.
Harrier fan/hunter warnings and fire, debris, hurt and collectibles retain
existing game triggers. No renderer, gameplay, encounter or projectile change.
Main HD only; ADF/WHDLoad compile paths retain preceding audio.

## Evidence and next action

Native main build and full existing host suite pass. Actual backend lifecycle
host test alternates both section loads across24 start/stop cycles and checks
expected selected score size, load failures, title-live exclusion, vector
restore and cleanup under sanitizers. The inherited independent mixer oracle
still passes40000 operations/13225 paired tails. Generator checks empty channel4,
MOD sample lengths, loop duration and absence of clipped host-preview samples.
Host preview is micromod, not native ptplayer or gameplay SFX evidence.

Standard staging verifies56 assets/references;61 alpha.4 release files are
byte-identical. Sole test `dist/Stormrail-Music-030-HD/Sparkpaw-Music`.
Prior `Level1-Music-Game-030-HD` drawer preserved in older-builds.
First user audition:030, READY OPTIONS->STORMRAIL, normal play through route
and Harrier; judge music and clarity of effects. Results/replay and Escape
as reached. No A/B/performance experiment or diagnostic-save instruction.
No Codex emulator launch, release, SemVer, commit or push.

## Music campaign ADF — 9 September 2026

Music campaign ADF candidate — 9 September 2026. User reports the Stormrail
HD music “ok gaat goed” after the requested030 audition; no extra machine/log
or individual-transition details supplied. User now authorized an ADF version.

Sole active test set: `dist/Music-Campaign-ADF/Music-Disk1.adf` and
`Music-Disk2.adf`. Ordinary880-KiB FFS disks, PAL A1200/2MB Chip+8MB Fast.
Disk1 has30 blocks/15KiB free; Disk2 has318 blocks/159KiB free. Title/READY
Neon Sky, Level1 Copper Sprint, Stormrail Iron Horizon + mixed SFX; results
original SFX. Like alpha.4 ADF, story intro/Hero Drive omitted.

Disk-only: pack scores/banks/SFX with existing SPR1/SPL1 decoder; externalize
40192-byte READY masks as6423-byte packed data, loaded once into Fast before
READY. No gameplay decompression, no HD compression, no audio/visual changes.
Markers SP07M1/SP07M2 reject mixing these files with older SP07D disks.
Native compile/full host suite/actual C decode+per-disk readback checks pass;
HD executable byte-identical and61 alpha.4 release files preserved. Boot,
DF0 swap/DF1 discovery and audio remain user-native gates; no emulator run.
Use first030, OPTIONS->STORMRAIL for quick Disk2 testing. No A/B/log save.
Prior Stormrail HD drawer and previous multidisk build/evidence archived intact.
No release/commit/push. See `sparkpaw/docs/MUSIC_CAMPAIGN_ADF.md` and
`MUSIC_CAMPAIGN_ADF_TEST.txt`.
