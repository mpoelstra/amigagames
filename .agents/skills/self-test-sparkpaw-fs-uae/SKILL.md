---
name: self-test-sparkpaw-fs-uae
description: Build and run short automatic Sparkpaw proofs in FS-UAE without using dist or asking the user to replay the intro and level. Use for rendering, palette, Copper, Blitter Bob, animation, collision, audio-state or late-level changes that can be verified with a compile-guarded setup, bounded native frames and an Amiga-written framebuffer or log artifact.
---

# Self-test Sparkpaw in FS-UAE

Exercise the real Amiga code path through the smallest deterministic proof.
Keep self-test artifacts separate from user candidates and releases.

## Establish one proof

1. Read `CODEX_HANDOFF.md`, `sparkpaw/README.md`, the relevant roadmap and the
   source around the target subsystem.
2. State one hypothesis and one machine-verifiable acceptance criterion.
3. Prefer production code plus compile-guarded setup and capture hooks. Do not
   reproduce the renderer in a host simulation.
4. Use `run-sparkpaw-test-cycle` when subjective play, controller feel, audible
   output, cadence acceptance or real hardware remains necessary.

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

For visual issues, dump the active rolling display target after publication,
not a source asset or pre-composition buffer. A standard proof contains magic,
width, height, bytes-per-row, camera X, target X and target Y followed by four
native planes. For state/audio logic, write a bounded contract log instead.

Use `scripts/decode_planar_proof.py` for `ELPF` or `ELP2` framebuffer dumps.
Inspect the decoded PNG; file creation alone is not acceptance.

## Run from a temporary directory hard drive

Use `scripts/run_fsuae_proof.py`. It creates a unique drawer below
`sparkpaw/build/fsuae-selftest`, copies the proof executable and runtime assets,
adds `S/startup-sequence`, and mounts that ordinary host directory as FS-UAE
`DH0:`. This is not an HDF, ADF or `dist` build.

Run with escalation because this launches FS-UAE:

```sh
python3 .agents/skills/self-test-sparkpaw-fs-uae/scripts/run_fsuae_proof.py \
  --project sparkpaw --executable sparkpaw/build/my-proof \
  --name my-proof --artifact proof.raw
```

The runner polls the host-visible artifact and terminates FS-UAE when the file
is stable. A timeout fails the proof; inspect the FS-UAE log and fast-boot path
before retrying.

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
