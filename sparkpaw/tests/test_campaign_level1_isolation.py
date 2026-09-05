#!/usr/bin/env python3
"""Static contract for the accepted alpha.68 Level-1 campaign boundary."""
from hashlib import sha256
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

accepted = {
    "storm-front.spbm": "4645a37128fddda244b2e14eaa03b62a39321c7eb3099d15ff8755cbbff07716",
    "storm-rear.spbm": "600374464bcf3951734f1b60e72b9bbc2fdba7287a6189634d3f0248338ec247",
    "sparkpaw-sprites4.spbm": "e02eadbad5879b563aa2ca020e87f67f9ab2d70c232acc8f41c279c1cedf47b3",
}
for name, expected in accepted.items():
    actual = sha256((ROOT / "assets/runtime" / name).read_bytes()).hexdigest()
    assert actual == expected, f"Level-1 asset drift: {name}: {actual}"

assets = (ROOT / "src/assets.c").read_text()
renderer = (ROOT / "src/renderer.c").read_text()
main = (ROOT / "src/main.c").read_text()
collision = (ROOT / "src/collision.c").read_text()
game = (ROOT / "src/game.c").read_text()
assert '"PROGDIR:assets/runtime/sparkpaw-sprites4-storm.spbm":' in assets
assert '"PROGDIR:assets/runtime/sparkpaw-sprites4.spbm",' in assets
level1_return = assets.index("if(!loadStormrailGameplay) return TRUE;")
campaign_load = assets.split("#ifdef SPARKPAW_CAMPAIGN", 1)[1]
assert campaign_load.index('"PROGDIR:assets/runtime/stormstone-core.spbm"') < campaign_load.index("if(!loadStormrailGameplay) return TRUE;")
assert assets.index('"PROGDIR:assets/runtime/stormrail-flight-rear.spbm"') > level1_return
assert "stormrailHistory=target->stormrail;" in renderer
assert "target->stormrail=stormrailHistory;" in renderer
assert "if(game->stormrailActive) {\n        UBYTE chosen=" in renderer
assert "game->stormrailActive?\n            44+60+i*12-1:44+64+i*4-1" in renderer
assert "game->stormrailActive?\n            44+108+i*11-1:44+136+i*3-1" in renderer
capture_block = main[main.index("/* Whole-display captures belong only"):]
assert capture_block.startswith(
    "/* Whole-display captures belong only to focused Stormrail proof executables."
)
assert "#if defined(SPARKPAW_STORMRAIL_PROOF) && !defined(SPARKPAW_CAMPAIGN)" \
    in capture_block[:400]
assert capture_block.index("rendererCaptureStormrailProof(0)") < \
    capture_block.index("rendererDiagnosticUpdateEntry")
solid_query = collision[collision.index("BOOL collisionSolidAt"):]
assert "if(stormrailCollision)" in solid_query[:500]
assert "gameStormrailActive()" not in solid_query[:500]
assert "stormrailCollision=gameStormrailActive();" in collision
level1_update = game[game.index("void gameUpdate(void)"):]
assert "if(game.stormrailActive) {" in level1_update[:700]
print("campaign Level-1 isolation contract: ok")
