# Title and intro music — 6 September 2026

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


Update, 7 September 2026: the user reopens this research for gameplay music/SFX
and then the whole audio system. See [AUDIO_SYSTEM_PLAN.md](AUDIO_SYSTEM_PLAN.md)
for the current proposed direction and [INGAME_MUSIC_AUDIT.md](INGAME_MUSIC_AUDIT.md)
for source findings. No presentation player or release changes. Historical
parked/candidate statements below do not override this research-only scope.

Released in 0.7.0-alpha.3. See CURRENT_STATUS.md and RELEASE_0_7_0_ALPHA_3.md
for current acceptance and open platform gates. Candidate entries below are
historical; their active drawer/pending release statements are superseded.

## Current candidate — 7 September 2026

User selected VIII Hero Drive for the intro. It now plays once from the first
intro fade, stops at skip/completion, and releases its bank before title loading.
Neon Sky then plays title through READY; gameplay/results remain SFX-only.
The LSP frame count bounds intro replay so slow I/O cannot restart the cue.
Intro bank is 190,812 Chip bytes, score 5,752 Fast bytes; 3,028 PAL frames
(~60.56 seconds). The title bank is separate, not simultaneously resident.

Sole active test drawer: `dist/Intro-Title-Music-HD`, 52 runtime files. Native
build, complete host suite (including repeated intro/title switches and failure
cleanup), LSP simulated replay and staged byte parity pass. No emulator run or
native audio acceptance. User FS-UAE/030 first, then accepted 020/performance,
then real A1200. No release, commit or push.

At the user's request, dist now contains only all seven alpha.2 release entries,
the new test drawer and older-builds. All 28 other top-level entries (including
alpha.68, Chip2, the previous title test, my-files and even-older-builds) were
moved into older-builds and their file hashes checked. No files deleted.
Archive mapping and build/package evidence: build/intro-music/.


## Current candidate

The user selected E — Neon Sky (160 BPM) for the large title through LOADING,
CHARGING and READY/options. The approved MOD remains byte-identical in
music/neon-sky.mod. music.c and the locally vendored MIT LightSpeedPlayer v1.31
use a fixed-PAL-50-Hz conversion (original tempo retained with frame quantization).
The existing repository LSPConvert tool generates two shared presentation files:
neon-sky.lsmusic (7,625 Fast bytes) and neon-sky.lsbank (142,796 Chip bytes).
Only unused sample tails are trimmed; no disk-compression loader is introduced.

Music starts before the first title fade-in frame, after the story is finished
or skipped; it also restarts on both return-to-title paths. Exec VBlank replay
continues during DOS loading and renderer preparation. At exclusive display
takeover the already active audio DMA channels are preserved without a restart;
the READY display's beam-synchronised frame wait drives the same player while
Exec interrupts are disabled. Following the READY fade, playback/DMA stops
before freeing score/bank. All gameplay and results retain SFX only. Optional
music load/allocation failure falls back to silence rather than failing startup.
Audio channel reservation is released at platform close. No music interrupt
player work runs during interrupt-disabled gameplay.

The bank raises the loading/READY Chip peak by about 139.45 KiB; it is released
before gameplay or a direct-selected Stormrail reload. This does not claim that
1.45 MB free Chip is sufficient. Existing Chip2 hardware acceptance remains
valid for Chip2, not automatically for this music candidate.

## Test gate

Sole active game candidate: dist/Title-Music-HD (Sparkpaw plus ReadMe and 50
runtime files). Executable SHA256:
d4422361477faacbc4f195b5a5f822e9a9463a16237303f95c86cc41c65741bb.
Full host suite, actual music lifecycle/failure tests under ASan/UBSan, native
compile and staged byte parity pass. LSPConvert's simulated Paula replay also
completes its 3,605-frame stream (~72.1 seconds). This is not native listening
or visual acceptance. All 102 protected release files and accepted Chip2 ZIP
are unchanged. Build/test/conversion/verification records are under build/.

User's explicit order: user-run FS-UAE/68030 functional/listening check first;
only after acceptance, separate 020/performance testing; real A1200 afterwards.
Check intro completion/skip, title start, uninterrupted loading/charging,
READY/options plus a complete loop, SFX-only gameplay, direct Stormrail,
return-to-title restart and normal exit. No emulator is run by Codex. No new
release/other media, SemVer, commit or push. Intro remains silent in this build.
Future ADF capacity and WHDLoad replay timing need their own validation.

## Intro inspection and composition

Inspected all five current native previews and all eleven actual text passages
in tools/generate_intro_proof.py. The current story is balance, damaged command,
reversed network/storms, threatened home and inventor's gauntlet, then departure.
The obsolete toaster gag and 32–36-second draft are not the production script.

Runtime: 11 passages * (240 hold/scroll + 12 text-fade frames), plus five pairs
of 24-frame image fades = 3,012 PAL frames / 60.24 seconds. Display lock adds
35 frames and view/copper setup a few more; file I/O and user advances/skips
make exact wall time variable. Three 32-bar, 126-BPM studies each render to
60.95238 seconds. Their pulse is spacious/half-time, not title-screen dance pace.
Approximate musical scene boundaries are 0, 11.43, 22.86, 38.10 and 49.52 seconds.
Future integration must stop on intro skip/completion rather than delay the
existing story to finish a track. Modules loop if a player keeps running them;
the authored final tonic/fade provides the end of the first complete cue.

- I — The Sleeping Sky: open, sustained synth harmony, slow pulse and broad lead.
- II — Archivolt's Lament: a darker melodic contour, crystalline opening and
  restrained mechanical plucks during Archivolt's scene.
- III — A Shard of Hope: warmer rising answers, more percussion late in the
  story and a hopeful final statement.

These are original compositions with synthesized samples and actual micromod
renders, not reference transcriptions. MOD/WAV/MP3 and generators/manifests are
in music/experiments/intro-variations. Expanded sample banks are listening
experiments pending choice and later runtime conversion. No intro cue is yet
integrated or aurally reviewed by the assistant.

## Research and design rationale

Chris Huelsbeck's own [Turrican anthology description](https://www.huelsbeck.com/turrican-soundtrack-anthology)
identifies the synthesizer scores and memorable melodies. This supports the
user's requested synth/melody direction, not a claim about a reference BPM.
For the intro, sustained harmony and a restrained opening are composition
choices derived from the actual reading time and pictures; the darker middle
and hopeful return follow the story without copying a Turrican melody.

The [Amiga hardware audio-interrupt documentation](https://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node016B.html)
and [CIA resource documentation](https://wiki.amigaos.net/wiki/CIA_Resource)
were consulted for interrupt ownership. This implementation instead uses the
existing repository's PAL VBlank-based LSP approach and polled owned-display
frames, avoiding a new CIA timer dependency in Sparkpaw. Native timing remains
a user-test gate. Vendored player license is in third_party/LSPlayer/LICENSE.

## Hope revisions — 6 September 2026

User prefers III A Shard of Hope and requests a cheerful peaceful opening,
darker middle, hopeful finish and livelier varied tempo/rhythm. Added IV Morning
Sparks, V Clockwork Dawn and VI Skybound under music/experiments/hope-variations.
These use actual scene tempo changes, distinct bass/drum patterns, major opening
and resolution with a minor middle. All retain long lead phrases and run about
60–61 seconds. Listening choice pending; no intro integration or change to the
currently user-tested Title-Music-HD drawer. See that folder's comparison.json
for exact timings/hashes and README for the rhythmic directions.

### Skybound continuous-energy finale variants

User prefers Skybound's opening but rejects the later tempo dip and requests
multiple hopeful, melodic, energetic Amiga-style endings. Added Sparkpaw Rising,
Hero Drive and Shards of Victory under music/experiments/skybound-finales.
First 25 bars and sample data are byte-identical to Skybound. All three maintain
164 BPM after the opening and finish at ~60.45 seconds, with different lead/drum/
tracker accents. Render and opening-parity checks pass; listening choice pending.
No game build, integration, release or commit changes in this revision.

## Title music ADF test — 7 September 2026

User requests ADF testing without story intro and asks whether title music fits.
The first raw-music Disk 1 exceeded capacity. The disk-only music loader now
reuses the existing CRC-checked SPL1/SPR1 reader for score/bank, decoding directly
into Fast/Chip allocations without an extra full-size copy. Both decoded files
are identical to HD: 150,421 raw bytes become 99,650 stored bytes. No lossy sample
change and no HD compression. Intro music references are excluded from no-story
builds; neither Hero Drive nor story plates are on the ADFs. Music files follow
the established disk resolver, including DF1 and DF0 swap handling.

Same active music test set: dist/Intro-Title-Music-HD/ADF/Music-Disk1.adf and
Music-Disk2.adf. Disk 1 has 92 free blocks / 46 KiB, Disk 2 has 345 / 172.5 KiB.
Both are 901,120-byte DOS1/FFS images; Disk 1 boot checksum verified. Full file
readback, actual C decode/CRC comparison, per-volume dependency checks, 45
reader cases, DOS-stub media tests, full host suite and native compilation pass.
Actual music continuity during floppy I/O remains user FS-UAE/030 testing, then
accepted 020/performance and real hardware. No automatic emulator run.

All 57 latest-release files and the existing HD test payload remain unchanged.
No release, commit or push. Build/readback/capacity evidence and hashes are in
build/multidisk-probe/media.json and music-*.log. Historical probe packager now
includes title-music ownership/order but excludes HD-only intro music.

Replay research on tentative skipping is recorded in [MUSIC_REPLAY_AUDIT.md](MUSIC_REPLAY_AUDIT.md).
Fixed50 timing and the READY polling handoff are audit concerns, not measured
native faults. Active test builds remain unchanged pending user evidence.

### 7 September 2026 — music ADF accepted, replay research parked, READY dust inquiry

User reports the ADF version with music works; exact CPU and individual swap/
return checks are not restated, and real-hardware acceptance is not inferred.
User explicitly parks replay/player-alternative research. Current test artifacts
remain unchanged. Supplied READY screenshot is preserved with a sidecar as
Unassigned-reference-ready-menu-dust.png in testresults.

READY dust is feasible as a separate small presentation effect: reuse the
existing hidden buffers, restore old particle rectangles from clean/menu state,
and reuse Stormrail's 1px/2px/grit shapes with colours mapped to the READY
palette. Keep logo/menu/credits readable, movement right-to-left and game dust
renderer unchanged. No additional full-screen buffer should be needed; concrete
frame cost/dirty-region handling and music cadence need implementation review.
This records feasibility, not an implemented or accepted dust effect.
