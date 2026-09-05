from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
game = (ROOT / "src/game.c").read_text()
main = (ROOT / "src/main.c").read_text()
title = (ROOT / "src/title.c").read_text()
renderer = (ROOT / "src/renderer.c").read_text()
score_generator = (ROOT / "tools/generate_score_screen.py").read_text()

# Gate-6 COMPLETE, not distance/combat/opening, owns results entry.
complete = "game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMPLETE"
assert complete in game
assert "game.stormrailResults.finalized" in game
assert "SPARKPAW_STORMRAIL_RESULTS_TEST" in game

# The immutable snapshot consumes only the completed Stormrail run sources.
assert "stormrailResultsFinalize(&game.stormrailResults" in game
for source in (
    "game.enemiesDefeated", "game.diamondsCollected",
    "game.elapsedFields", "game.score",
):
    assert source in game
assert "accountElapsedFields();" in game.split("#ifdef SPARKPAW_STORMRAIL_PROOF", 2)[2]

# Actual Stormrail enemy kills and diamonds feed the reused visible rows.
assert "game.enemiesDefeated++;" in game
assert "game.diamondsCollected++;" in game

# The presenter and source art remain shared; no Stormrail results asset exists.
assert "titleShowLevelComplete()" in main
assert "titleRunLevelCompleteWithBonus" in main
assert "assetsLevelComplete()" in title
assert "assetsScoreGlyphs()" in title
assert 'PROMPT = "REPLAY LEVEL"' in score_generator
assert "REPLAY INTERLUDE" not in score_generator
assert "titleRunLevelCompleteWithBonus(enemies,diamonds" in main
assert "runLevelCompleteMenu(enemies,diamonds,timeSeconds,liveScore,1)" in title

# Replay uses resident initialization/reset only. No level-file load may appear
# between results handling and the next APP_PLAYING transition.
results_branch = main.split("if(state==APP_LEVEL_COMPLETE)", 1)[1]
assert "titleFadeOut();" in results_branch
assert "gameInit(" in results_branch
assert "rendererResetGameplay();" in results_branch
assert "platformResetGameInput();" in results_branch
# The integrated campaign adds separately guarded cross-section loads. The
# common tail after those decisions remains the resident replay operation.
resident_tail = results_branch.split("#endif\n        DateStamp(&levelTime);", 1)[1]
assert "loadLevelFiles()" not in resident_tail
assert "titleShowLevelLoading()" not in resident_tail
assert "titleShowLevelCharging()" not in resident_tail

# The original departure sources normally retire after the one-way handoff.
# Results builds keep them resident so replay can really return to boarding
# without a load or use-after-free.
release = renderer.split("assetsReleaseStormrailApproach();", 1)[0]
assert "#ifndef SPARKPAW_STORMRAIL_RESULTS_TEST" in release[-300:]

# gameInit is the single fresh-run reset. Require every reset category and all
# bounded pools/history arrays before publication can occur.
init = game.split("void gameInit(ULONG enemySeed)", 1)[1].split(
    "void gameUpdate(void)", 1
)[0]
required_resets = (
    "game.score=0", "game.elapsedFields=0", "game.enemiesDefeated=0",
    "game.diamondsCollected=0", "game.lives=GAME_START_LIVES",
    "game.diamonds=0", "game.stormrailHealth=PLAYER_MAX_HEALTH",
    "game.stormrailInvuln=0", "game.stormrailDistance=0",
    "game.stormrailPickupConsumed=0", "game.stormrailPickupConsumedHigh=0",
    "game.stormrailEnemyScoreAwarded=0",
    "game.stormrailObstacleScoreAwarded=0",
    "game.stormrailObstacleScoreAwardedHigh=0",
    "game.stormrailDebrisNextEvent=0", "game.stormrailFinaleActive=0",
    "game.stormrailFinaleLastFireTick=0",
    "stormrailResultsReset(&game.stormrailResults)",
    "game.stormrailFireHeld=0", "game.stormrailFireHoldTicks=0",
)
for reset in required_resets:
    assert reset in init, reset
for pool in (
    "game.stormrailShotActive[stormShot]=FALSE",
    "game.stormrailEnemyActive[stormShot]=FALSE",
    "game.stormrailHostileActive[stormShot]=FALSE",
    "game.stormrailRewardActive[stormShot]=FALSE",
    "game.stormrailObstacleActive[stormShot]=FALSE",
    "game.stormrailFinaleHp[stormShot]=stormrailFinaleActors[stormShot].hp",
    "game.stormrailFinaleFlash[stormShot]=0",
    "game.stormrailFormationSpawned[stormShot]=0",
    "game.stormrailFreeLineSpawned[stormShot]=0",
):
    assert pool in init, pool
assert "playerInit();" in init
assert "projectilesInit();" in init

print("PASS: COMPLETE trigger, shared presenter and resident no-carry replay integration")
