# Pre-level ready screen

Status: accepted on FS-UAE/68030 and FS-UAE/68020 HD for alpha.48 Phase 6C.4.

This screen appears after `CHARGING`, after gameplay preparation, and before
the gameplay Copper is published. It uses a separately extracted Sparkpaw logo,
an original Sparkpaw hero composition, a large dark field, Level-1-derived
stone/machine edge architecture, `PRESS FIRE TO START` and two restrained
credit lines. There is no title-landscape crop or separator below the logo.

The supplied ThunderCats title screenshot informed only the hierarchy: one
large brand, one interaction prompt, credits anchored at the foot and generous
negative space. No ThunderCats pixels, characters or typography are reused.

Proposed text:

```text
PRESS FIRE TO START

2026 MRDIG PRODUCTIONS
100% MADE WITH AI
```

The implementation decodes the screen temporarily into Fast RAM and
copies it once into the existing six-plane loading bitmap while faded to black.
It must not allocate another displayable Chip-RAM bitmap or alter gameplay's
4+3 dual-playfield, Copper, HUD or renderer timing contracts. Joystick Fire and
Space both advance only after the level and renderer are fully prepared.

The HD edition retains the complete five-plate cinematic intro. The space-bound
ADF edition omits those five plates and begins at the existing title, while
retaining `LOADING`, `CHARGING` and this same full-quality 64-colour ready
screen. The final alpha.48 DOS1/FFS image uses 1,353 blocks (676 KiB), leaving
407 free. This is a packaging/presentation difference only; gameplay is shared.
