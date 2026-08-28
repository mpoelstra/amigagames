# Pre-level ready menu

Status: accepted on FS-UAE/68030 and FS-UAE/68020 HD for alpha.48 Phase 6C.4.
Alpha.49 additionally reserves pure-black palette pen 0; supplied FS-UAE/68030
HD and real-A1200/Indivision HD testing accept removal of the previously
visible one-pixel full-height `COLOR00` border.

Alpha.54 extends the accepted composition with `START GAME` and `OPTIONS`.
After two rejected full-width source/patch variants, the final atlas changes
only word-aligned x=80..239, preserves both lower corners byte-identically,
omits credits from Options and uses symmetric arrows around JUMP/FIRE. Supplied
FS-UAE/68030, FS-UAE/68020 and real-A1200/68030 HD testing accepts the final
presentation, direct start and both secondary-button mappings. The 68020 test
was production-style and supplies no cadence log. ADF and WHDLoad remain open.

Alpha.58 corrects the intermittent transition and idle fragments captured on a
real A1200. The previous implementation copied six planes into the displayed
bitmap. The current implementation seeds two complete ready buffers, patches
only the hidden buffer, arms COP1LC mid-frame without COPJMP1 and lets vertical
restart publish the complete list. Full takeover also keeps sprite DMA off
because these presentation Copper lists own no sprite pointers; gameplay
enables it only with its pointer-owning Copper list. This adds one 61,440-byte
displayable Chip bitmap. Supplied FS-UAE/68030 and real-A1200/68030 HD testing
accepts idle screens, rapid menu switching and gameplay entry. ADF and WHDLoad
acceptance remain pending.

This screen appears after `CHARGING`, after gameplay preparation, and before
the gameplay Copper is published. It uses a separately extracted Sparkpaw logo,
an original Sparkpaw hero composition, a large dark field, Level-1-derived
stone/machine edge architecture, a compact menu and two restrained credit
lines. Alpha.53 replaces the single prompt with `START GAME` and `OPTIONS`;
the former remains selected by default. Options contains only the session-local
`SECOND BUTTON` choice, defaulting to `JUMP` and permitting `FIRE`. There is no
title-landscape crop or separator below the logo.

The supplied ThunderCats title screenshot informed only the hierarchy: one
large brand, one interaction prompt, credits anchored at the foot and generous
negative space. No ThunderCats pixels, characters or typography are reused.

Current main-menu text:

```text
START GAME
OPTIONS

2026 MRDIG PRODUCTIONS
100% MADE WITH AI
```

The implementation decodes the screen temporarily into Fast RAM and seeds two
six-plane displayable buffers while faded to black. Menu changes patch only the
hidden buffer; a complete inactive Copper list publishes it atomically at a PAL
frame boundary. Gameplay's 4+3 dual-playfield, HUD and renderer timing contracts
remain unchanged. Joystick Fire and Space both advance only after the level and
renderer are fully prepared.

Four deterministic central bands—Start selected, Options selected, Jump and
Fire—share the ready screen's exact palette in one Fast-RAM patch atlas. The
selected band is copied only into the hidden ready buffer. The atlas remains in
Fast RAM; the second displayable bitmap adds 61,440 bytes of Chip RAM. Up/down
selects the main menu, left/right changes the option and Fire/Space returns
from Options.

The HD edition retains the complete five-plate cinematic intro. The space-bound
ADF edition omits those five plates and begins at the existing title, while
retaining `LOADING`, `CHARGING` and this same full-quality 64-colour ready
screen. The final alpha.48 DOS1/FFS image uses 1,353 blocks (676 KiB), leaving
407 free. This is a packaging/presentation difference only; gameplay is shared.
