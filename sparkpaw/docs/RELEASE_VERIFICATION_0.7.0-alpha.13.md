# Release verification — 0.7.0-alpha.13

26 September 2026. Phase 7B.1 Harrier finale. The user accepted the focused
explosion art and final louder three-hit sound, then requested a new alpha,
documentation, commit and push. Public [itch downloads](https://mrdig.itch.io/sparkpaw)
and its latest devlog still show alpha.8; there is no itch upload here.

## Changes since local alpha.12

- A 64-PAL-tick, one-shot Harrier defeat phase shows three hull ruptures, a
  full 112x64 masked fireburst, flying fragments and fading residue. It starts
  after the lethal hit, retires boss shots, blocks further damage and keeps
  the gate closed until the existing opening/results sequence.
- A 1.24-second, 13,672-byte Amiga PCM cue gives three progressively stronger
  impacts and a longer final boom. The established SFX/music mixer plays it
  once per kill; SFX ONLY uses the direct Paula path. Pausing, retrying and
  starting a fresh run retain their prior lifecycle.
- HD and both WHDLoad SOUNDTEST menus now include HARRIER DEFEAT. The preview
  loads its Chip sample only when selected and releases it on leaving the
  SOUNDTEST. The ADF menu continues to omit SOUNDTEST.
- The three-disk ADF uses a wider host-side search for selected lossless
  SPL1-packed assets. The Amiga decoder and decoded assets are unchanged.
  This recovered the Disk 1 minimum of 16 free blocks. The first failed
  13-block alpha.13 attempt is preserved in
  `build/alpha13-pre-soundtest-adf13-attempt/`; the separate 16-block proof
  is in `build/alpha13-adf-wide-proof/`.
- Two old alpha.5 release-art files were removed by the user and included in
  this commit at their request.

## Verification and limits

- `make PYTHON=../.venv/bin/python3`: PASS, ordinary campaign executable.
- `make test PYTHON=../.venv/bin/python3`: PASS, including Harrier frame/sample
  parity, once-only defeat state, boss shots/damage/pause/reset/results,
  SOUNDTEST selection/load/release, menu display reacquisition, and campaign
  audio/resource ownership.
- `make release PYTHON=../.venv/bin/python3`: PASS. Four native 68020-target
  campaign variants, HD, standard WHDLoad, High RAM WHDLoad and three ADFs.
- `tools/verify_checkpoint_release.py`: PASS. Independent archive extraction,
  member CRCs, all-file three-ADF readback, icons, 30-character path limits,
  trace-free release flags and all six packaged ReadMe files checked. The
  standard WHDLoad common bank contains the exact 13,672-byte cue; the HD and
  High RAM drawers contain the same bytes. The ReadMe retains the user-authored
  personal note and identifies the correct version and edition.
- ADF free 512-byte blocks: Disk 1 **16**, Disk 2 **184**, Disk 3 **105**.
  Disk 1 meets the existing minimum exactly and has no reserve above it.
- The 13 prebuilt planar frames occupy 66,560 program/Fast bytes and stage one
  5,120-byte frame in Chip. At most one frame copy every five PAL ticks is
  50 KiB/s during the visible sequence. The masked draw/restore upper traffic
  estimate is about 24 KiB per visible frame. These are transfer bounds, not
  measured CPU/Blitter time or spare-frame-time evidence.
- The defeat sample uses 13,672 Chip bytes in the fallback path and 13,672
  Fast bytes in the gameplay mixer when Stormrail loads. SOUNDTEST briefly
  loads the same 13,672 Chip bytes at READY and frees them on exit. The READY
  cache grows by 3,345 Fast bytes for the new sound label/state. Standard
  WHDLoad's resident common bank also retains the raw cue. High RAM preloads
  5,844,230 bytes; reservation plus preload is 11,611,398 bytes. These are
  budget counts, not startup or cadence acceptance.

Minimum target remains PAL A1200/AGA, 68020, 2 MB Chip and 8 MB Fast.
High RAM WHDLoad requires 16 MB Fast. The user approved the focused look and
sound on an HD test; the user has not yet tested these exact alpha.13 HD,
WHDLoad or ADF packages, nor minimum-68020 cadence or real hardware. The
intermittent real-Amiga HUD-boundary issue remains open. No FS-UAE was launched
by Codex.

The 181 alpha.12 release files are byte-identical in
`dist/older-builds/alpha12-before-harrier-20260926/`; its hash manifest is
there. The final focused HD test's 78 files are intact in
`dist/older-builds/Harrier-Death-030-H-approved-20260926/`. Earlier test
drawers and evidence were retained.

## Release artifacts

| File | Bytes | SHA-256 |
| --- | ---: | --- |
| Sparkpaw-0.7.0-alpha.13.zip | 1648905 | `13eaefaeebd9547b56ce585290e664ff688005ed9b18404b038e50d323f1b49a` |
| Sparkpaw-0.7.0-alpha.13.lha | 1669268 | `325437973b7544d44b690e1c87821bc22903e443a52ea106d42fc0fd1e6c538a` |
| Sparkpaw-0.7.0-alpha.13-WHDLoad.zip | 1650548 | `b0fac791ebd21be3f6bd44412dd881643c292f2c7c789f175200cb997b2a05ea` |
| Sparkpaw-0.7.0-alpha.13-WHDLoad.lha | 1681524 | `a0fe076a81d750dc0308731e8c444099fc1c9fc506cfbd5c75f262d77eaff8b4` |
| Sparkpaw-0.7.0-alpha.13-WHDLoad-HighRAM.zip | 1654644 | `11b3f968913816c51b63e61155381a0cdd01a7dcefc8b0c3e6ee307b353c7a64` |
| Sparkpaw-0.7.0-alpha.13-WHDLoad-HighRAM.lha | 1674426 | `b94af07d019568a4a4673c28f136ea48a6a000ce4504c267e59edfd1594d4f02` |
| Sparkpaw-0.7.0-alpha.13-Disk1.adf | 901120 | `6029d1eccf662b8ec9e2f7261e572a810475cd96bebb0c07c4f707ac788dc279` |
| Sparkpaw-0.7.0-alpha.13-Disk2.adf | 901120 | `6cea6beded1f76139dc47995d429ebaea56b36c8c175bbff0f0dd1224d594203` |
| Sparkpaw-0.7.0-alpha.13-Disk3.adf | 901120 | `286b7b2faeb8d2708a3ad2bb8bf89a39885c430215dea1a6fb1a727a0da7a4e1` |

Machine-readable hashes: `ALPHA13_ARTIFACT_SHA256.json`. Build evidence:
`build/release-0.7.0-alpha.13/` and its
`checkpoint-release-verification.json`.
