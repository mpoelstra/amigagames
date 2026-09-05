---
name: self-test-sparkpaw-fs-uae
description: Exceptionally run a short automatic Sparkpaw FS-UAE proof outside dist when a deterministic native-only fault cannot be resolved credibly with host tests, compilation or prompt user testing. Never use it as the default visual, motion, cadence, audio-feel or playtesting loop.
---

# Self-test Sparkpaw in FS-UAE

Exercise the real Amiga code path through the smallest deterministic proof,
but only as an exception. MrDig is the primary runtime tester. Keep self-test
artifacts separate from user candidates and releases.

## Decide whether the exception is justified

Default to host contracts, native compilation and one focused user drawer.
Do not launch FS-UAE merely because an automatic proof is available or because
another visual iteration is convenient. Use this skill only when all apply:

- the open question is objective and machine-verifiable on the native path;
- host tests and code inspection cannot establish it;
- a bounded proof materially spares the user a long replay; and
- the proof answers one named question rather than starting an autonomous
  polish loop.

Cadence, controller feel, movement smoothness, artistic quality, sound impact
and encounter play remain user-test gates. After one automatic proof, return to
a single staged user candidate unless newly found native corruption makes one
further diagnostic indispensable. Never perform repeated private tweak/run
cycles after the user asks to test the next iteration personally.

## Establish one proof

1. Read `CODEX_HANDOFF.md`, `sparkpaw/README.md`, the relevant roadmap and the
   source around the target subsystem.
2. State one hypothesis and one machine-verifiable acceptance criterion.
3. Prefer production code plus compile-guarded setup and capture hooks. Do not
   reproduce the renderer in a host simulation.
4. Use `run-sparkpaw-test-cycle` for the normal iteration path and whenever
   subjective play, controller feel, audible output, cadence acceptance or real
   hardware remains necessary.

## Bypass presentation completely

Do not merely shorten `titleWaitFrames` or skip the ready menu. `titleShow()`
still plays the five-plate story when `SPARKPAW_STORY_INTRO` is defined.

For a focused proof:

- compile without `SPARKPAW_STORY_INTRO`, or take a proof-only branch before
  `titleShow()` that prepares gameplay directly;
- bypass title/loading/charging/ready fades, waits and input unless startup is
  the subsystem under test;
- initialize camera, actor and feature directly at the target scene;
- run a bounded number of real update/render/publish frames;
- restore Exec/DOS ownership before writing the artifact;
- compile every shortcut out of production.

Never use `make -B`: it can invoke the complete asset generator once per
generated prerequisite. Generate changed assets once, then compile a dedicated
proof target or direct proof executable.

## Capture native evidence

For visual issues, a contract log alone is never sufficient. Dump the active
rolling display target after publication, not a source asset or pre-composition
buffer. A standard proof contains magic,
width, height, bytes-per-row, camera X, target X and target Y followed by four
native planes. For state/audio logic, write a bounded contract log instead.

Use `scripts/decode_planar_proof.py` for `ELPF` or `ELP2` framebuffer dumps.
Inspect the decoded PNG; file creation alone is not acceptance.

### Mandatory whole-display visual gate

Native plane dumps do not include hardware sprites, Copper palette changes,
border behaviour or emulator presentation. Any candidate whose visible result
depends on those features must additionally capture the actual FS-UAE Amiga
frame with `--screenshot-at`. Capture every materially different phase (for
example approach, transition and gameplay), not merely startup or a convenient
static frame.

For each captured PNG the main agent must open the image and inspect it at
native scale and enlarged nearest-neighbour scale. Explicitly reject:

- torn, noisy or horizontally displaced bitplanes;
- invalid palette bands, checker patterns, debug blocks or exposed buffers;
- missing, malformed, wrongly layered or wrongly coloured sprites/Bobs;
- HUD/display-boundary corruption and unreadable silhouettes;
- a composition that is technically intact but visibly below the requested
  art direction.

Record the inspected screenshot paths and a short visual verdict alongside the
contract log. Never stage into `dist` from this skill or call an automatic
proof user acceptance. When this exceptional proof is justified, inspect its
required whole-display captures before staging a later candidate. The normal
workflow does not require a private FS-UAE capture first: source/asset review
plus host and compile checks may lead directly to one focused drawer for
MrDig.

## Run from a temporary directory hard drive

Use `scripts/run_fsuae_proof.py`. It creates a unique drawer below
`sparkpaw/build/fsuae-selftest`, copies the proof executable and runtime assets,
adds `S/startup-sequence`, and mounts that ordinary host directory as FS-UAE
`DH0:`. This is not an HDF, ADF or `dist` build.

Run with escalation because this launches FS-UAE:

```sh
python3 .agents/skills/self-test-sparkpaw-fs-uae/scripts/run_fsuae_proof.py \
  --project sparkpaw --executable sparkpaw/build/my-proof \
  --name my-proof --artifact proof.raw \
  --screenshot-at 2.0 --screenshot-at 5.0
```

The runner polls the host-visible artifact and requests FS-UAE's own screenshot
action at the specified seconds. On macOS it first activates FS-UAE and sends
its documented Command-S shortcut through System Events. If macOS denies only
the synthetic keystroke but Screen Recording is available, it falls back to a
full host-display capture after activating FS-UAE. Such captures must visibly
contain the unobscured emulator window and be recorded as host-display rather
than internal-emulator evidence. A timeout, missing capture or obscured window
still fails the proof.

Decode standard planar evidence with:

```sh
python3 .agents/skills/self-test-sparkpaw-fs-uae/scripts/decode_planar_proof.py \
  sparkpaw/build/fsuae-selftest/<run>/proof.raw \
  sparkpaw/build/fsuae-selftest/<run>/proof.png \
  --crop-around-target 96 64 --scale 4
```

## Preserve boundaries

- Never write into `sparkpaw/dist` during self-test iteration.
- Do not change SemVer, releases, roadmap acceptance or checkpoints.
- Preserve official release hashes.
- Catalog useful accepted/rejected evidence; leave transient drawers in
  ignored `build/`.
- After proof acceptance, compile without proof macros and run host/native
  tests before preparing any user candidate.
