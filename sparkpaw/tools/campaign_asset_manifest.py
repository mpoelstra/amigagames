"""Authoritative logical ownership for the integrated campaign runtime."""

SHARED_PRESENTATION = frozenset({
    "sparkpaw-title.spbm", "sparkpaw-level-loading.spbm",
    "level-charge-patch.spbm", "sparkpaw-ready-screen.spbm",
    "readymenu.spbm", "sparkpaw-level-complete.spbm",
    "sparkpaw-score-glyphs.spbm", "tally-tick.raw",
    "intro1.spbm", "intro2.spbm", "intro3.spbm", "intro4.spbm",
    "intro5.spbm",
})

SHARED_GAMEPLAY = frozenset({
    "sparkpaw-hud-base.spbm", "sparkpaw-hud-health.spbm",
    "sparkpaw-hud-lives.spbm", "sparkpaw-hud-diamonds.spbm",
    "sparkpaw-hud-score.spbm", "sparkpaw-diamond.spbm",
    "energy-shot.raw", "player-hurt.raw", "enemy-hit.raw",
    "enemy-death.raw", "collect-spark.raw",
})

LEVEL1 = frozenset({
    "storm-front.spbm", "storm-rear.spbm", "storm-collision.bin",
    "sparkpaw-sprites4.spbm", "clockwork-beetle.spbm",
    "clockwork-storm-strider.spbm", "strider-shot.raw", "jump.raw",
    "water-splash.raw", "sparkpaw-extra-life.spbm", "extra-life.raw",
    "stormstone-core.spbm", "stormstone-core.raw",
})

STORMRAIL = frozenset({
    "stormrail-front.spbm", "stormrail-rear.spbm",
    "stormrail-flight-rear.spbm", "sparkpaw-sprites4-storm.spbm",
    "stormrail-family.spbm", "stormrail-heart.spbm",
    "stormrail-obstacles.spbm", "harrier-fan-charge.raw",
    "harrier-fan-fire.raw", "harrier-hunter-charge.raw",
    "harrier-hunter-fire.raw",
})

GROUPS = {
    "shared_presentation": SHARED_PRESENTATION,
    "shared_gameplay": SHARED_GAMEPLAY,
    "level1": LEVEL1,
    "stormrail": STORMRAIL,
}
ALL = frozenset().union(*GROUPS.values())
