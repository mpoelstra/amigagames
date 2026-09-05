from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
main = (ROOT / "src/main.c").read_text()
game = (ROOT / "src/game.c").read_text()
collision = (ROOT / "src/collision.c").read_text()
player = (ROOT / "src/player.c").read_text()
title = (ROOT / "src/title.c").read_text()
assets = (ROOT / "src/assets.c").read_text()
makefile = (ROOT / "Makefile").read_text()
renderer = (ROOT / "src/renderer.c").read_text()

assert "RESULT_DECISION_CONTINUE" in main
assert "RESULT_DECISION_BACK_TO_TITLE" in main
assert "assetsSetStormrailGameplay(TRUE)" in main
assert "assetsSetStormrailGameplay(FALSE)" in main
assert main.count("rendererCleanup(); audioUnload();") >= 2
assert "if(game.stormrailActive)" in game
assert "if(game.stormrailActive)\n        return game.stormrailFinaleActive" in game
assert "stormrailCollision=gameStormrailActive();" in collision
assert "if(stormrailCollision)" in collision
assert "gameStormrailActive()?129L:156L" in player
campaign_target = makefile.split("$(CAMPAIGN_TEST_TARGET):", 1)[1].split(
    "campaign-test:", 1)[0]
assert "$(RELEASE_RENDERER_FLAGS)" in campaign_target
assert "filter-out -DSPARKPAW_STORY_INTRO" not in campaign_target
assert "if(!loadStormrailGameplay) return TRUE;" in assets
assert "if(game->stormrailActive&&!buildStormrailPatterns())" in (ROOT / "src/renderer.c").read_text()
assert "game->stormrailActive?\n            44+60+i*12-1:44+64+i*4-1" in renderer
replay_loading = title.split("BOOL titleShowReplayLoading(void)", 1)[1].split(
    "BOOL titleShowLevelReady", 1)[0]
assert replay_loading.index("installCopper(next)") < replay_loading.index(
    "assetsUnloadLevelComplete()")
assert "scoreBuffers[index]=NULL" in replay_loading
cleanup = renderer.split("void rendererCleanup(void)", 1)[1].split(
    "UWORD *rendererCopperList", 1)[0]
for cleared in ("stormVehicleMask=stormVehicleBits=NULL",
                "heartMask=heartBits=NULL",
                "stormProofPlanes[frame]=NULL",
                "stormProofRowBytes[frame]=0",
                "memset(enemyCaches,0,sizeof(enemyCaches))",
                "memset(prototypeTarget,0,sizeof(prototypeTarget))",
                "memset(prototypeCopper,0,sizeof(prototypeCopper))"):
    assert cleared in cleanup
for function in ("drawCollectibleBobs", "drawEnemyBob", "drawProjectileBobs",
                 "drawCoreBob", "drawExtraLifeBob", "drawSplashBob"):
    body = title if function.startswith("title") else (ROOT / "src/renderer.c").read_text()
    section = body.split(f"static void {function}", 1)[1].split("}", 1)[0]
    assert "if(game->stormrailActive) return;" in section
assert "SCORE_MENU_REPLAY_ROW" in title
assert "SCORE_MENU_CONTINUE_ROW" in title
assert "SCORE_MENU_BACK_ROW" in title
assert "SCORE_MENU_ARROW_X" in title
assert "selection=(menuMode!=1)?1:0" in title
assert "BOOL directionHeld=TRUE;" in title
show_results = main.split("if(!titleShowLevelComplete())", 1)[1].split(
    "platformFinishTakeover(titleCopperList())", 1)[0]
assert "platformResetGameInput();" in show_results
assert "playerState()->health,result->diamonds" in main
assert main.count("gameRestoreCampaignVitals(campaign.postLevel1Lives") == 2
assert "game.score=0; game.elapsedFields=0;" in game
assert "CAMPAIGN_START_STORM_RUINS" in main
assert "CAMPAIGN_START_STORMRAIL" in main
assert "switchPreparedLevel1ToStormrail" in main
assert "campaignStartAtStormrail" in main
assert "case 0x45: flag=GAMEKEY_ESCAPE;" in (
    ROOT / "src/platform_amiga.c").read_text()
assert "if(platformGameEscapeRequested())" in main
escape_return = main.split("if(state==APP_RETURN_READY)", 1)[1].split(
    "if(state==APP_LEVEL_COMPLETE)", 1)[0]
assert "campaignReset(&campaign);" in escape_return
assert "startSection=CAMPAIGN_START_STORM_RUINS;" in escape_return
assert "titleShowMain()" in escape_return
assert "titleShowLevelReady()" in escape_return
assert "titleRunLevelReadyMenu" in escape_return
reset_gameplay = renderer.split("void rendererResetGameplay(void)", 1)[1].split(
    "void rendererCleanup(void)", 1)[0]
assert "resetStormrailApproachCopperPalettes();" in reset_gameplay
palette_reset = renderer.split(
    "static void resetStormrailApproachCopperPalettes(void)", 1)[1].split(
    "#endif", 1)[0]
assert "for(list=0;list<2;list++)" in palette_reset
assert "stormApproachPaletteValue(stage,pen)" in palette_reset
handoff = title.split("BOOL titleShowMainFromResults(void)", 1)[1].split(
    "BOOL titlePrepareLevelLoading", 1)[0]
assert handoff.index("installCopper(next)") < handoff.index(
    "assetsUnloadLevelComplete()")
assert "titleShowMainFromResults()" in main
back = main.split("if(decision==RESULT_DECISION_BACK_TO_TITLE)", 1)[1]
assert back.index("rendererCleanup()") < back.index("titleShowMainFromResults()")
results_branch = main.split("if(state==APP_LEVEL_COMPLETE)", 1)[1]
fade_transition = results_branch.split("titleFadeOut();", 1)[1]
assert "platformReleaseForLoading(TRUE);" in fade_transition[:700]
print("PASS: campaign decisions, section loads and Level-1 isolation are wired")
