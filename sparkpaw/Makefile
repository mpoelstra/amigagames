VBCC_ROOT := $(CURDIR)/.toolchain/sdk
NDK_INCLUDE := $(CURDIR)/.toolchain/ndk/Include_H
export VBCC := $(VBCC_ROOT)
export PATH := $(VBCC_ROOT)/bin:$(PATH)
export TMPDIR := $(CURDIR)/build/tmp

CC = $(VBCC_ROOT)/bin/vc
PYTHON ?= python3
CFLAGS ?= +aos68k -O2 -cpu=68020 -notmpfile -I$(NDK_INCLUDE)
TARGET = sparkpaw
BENCH_TARGET = sparkpaw-renderbench
BENCH_ASSET = assets/runtime/renderbench-strider-idle.raw
SOURCES = src/main.c src/renderer.c src/hud.c src/assets.c src/audio.c src/collision.c src/collectibles.c \
	src/enemies.c src/game.c src/level_data.c src/platform_amiga.c src/player.c src/projectiles.c \
	src/title.c
HEADERS = src/assets.h src/audio.h src/collision.h src/collectibles.h src/enemies.h src/game.h src/hud.h \
	src/level_data.h src/platform_amiga.h src/player.h src/projectiles.h src/renderer.h src/title.h
RUNTIME_ASSETS = assets/runtime/storm-world.spbm \
	assets/runtime/sparkpaw-title.spbm \
	assets/runtime/sparkpaw-level-loading.spbm \
	assets/runtime/sparkpaw-level-charging.spbm \
	assets/runtime/sparkpaw-hud-base.spbm \
	assets/runtime/sparkpaw-hud-health.spbm \
	assets/runtime/sparkpaw-hud-lives.spbm \
	assets/runtime/sparkpaw-hud-diamonds.spbm \
	assets/runtime/sparkpaw-diamond.spbm \
	assets/runtime/storm-front.spbm \
	assets/runtime/storm-rear.spbm \
	assets/runtime/storm-collision.bin \
	assets/runtime/energy-shot.raw \
	assets/runtime/player-hurt.raw \
	assets/runtime/enemy-hit.raw \
	assets/runtime/strider-shot.raw \
	assets/runtime/jump.raw \
	assets/runtime/collect-spark.raw \
	assets/runtime/clockwork-beetle.spbm \
	assets/runtime/clockwork-storm-strider.spbm \
	assets/runtime/sparkpaw-sprites.spbm \
	assets/runtime/sparkpaw-sprites3.spbm \
	assets/runtime/sparkpaw-sprites4.spbm

.PHONY: all clean assets release bench

all: assets $(TARGET)

bench: $(BENCH_TARGET)

$(BENCH_ASSET): tools/generate_renderbench_assets.py \
	assets/enemies/clockwork-storm-strider-biped-production-v5-transparent.png
	$(PYTHON) tools/generate_renderbench_assets.py

$(BENCH_TARGET): src/aga_renderbench.c $(BENCH_ASSET)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -o $@ $<

assets: $(RUNTIME_ASSETS)

$(RUNTIME_ASSETS): tools/generate_runtime_assets.py \
	assets/concept/sparkpaw-title-concept-aga64-preview.png \
	assets/concept/sparkpaw-level-loading-concept-v3.png \
	assets/concept/sparkpaw-level-charging-concept-v2.png \
	assets/concept/sparkpaw-hud-concept-v1.png \
	assets/sprites/sparkpaw-sprites-reference-transparent.png \
	assets/sprites/sparkpaw-supplemental-poses-v2-transparent.png \
	assets/sprites/sparkpaw-run-compact-v3-transparent.png \
	assets/sprites/sparkpaw-jump-refinement-v5-transparent.png \
	assets/enemies/clockwork-storm-strider-64x64-aga15-idle-v3.png \
	assets/enemies/clockwork-storm-strider-64x64-aga15-walk-rig-v1.png \
	assets/enemies/clockwork-storm-strider-64x64-aga15-turn-v1.png \
	assets/enemies/clockwork-storm-strider-premium-shoot-source-v1-transparent.png \
	assets/sprites/sparkpaw-motion-refinement-v3-transparent.png \
	assets/sprites/sparkpaw-turn-refinement-v4-transparent.png \
	assets/sprites/sparkpaw-idle-refinement-v3-transparent.png \
	assets/sprites/sparkpaw-combat-refinement-v7-transparent.png \
	assets/sprites/sparkpaw-air-fire-v8-transparent.png \
	assets/sprites/sparkpaw-crouch-fire-v9-transparent.png \
	assets/sprites/sparkpaw-hurt-v12-transparent.png \
	assets/sprites/sparkpaw-crouch-hurt-v13-transparent.png \
	assets/enemies/clockwork-beetle-concept-v2-transparent.png \
	assets/enemies/clockwork-beetle-animations.json \
	assets/enemies/clockwork-storm-strider-biped-production-v5-transparent.png \
	sfx/raw/energy-shot.raw \
	sfx/raw/player-hurt.raw \
	sfx/raw/enemy-hit.raw \
	sfx/raw/strider-shot.raw \
	sfx/raw/jump.raw
	$(PYTHON) tools/generate_runtime_assets.py

sfx/raw/energy-shot.raw sfx/raw/player-hurt.raw sfx/raw/enemy-hit.raw sfx/raw/strider-shot.raw \
	sfx/raw/jump.raw sfx/raw/collect-spark.raw: \
	tools/generate_sparkpaw_sfx.py
	$(PYTHON) tools/generate_sparkpaw_sfx.py

$(TARGET): $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

release: all
	$(PYTHON) tools/make_release.py

clean:
	$(RM) $(TARGET) $(BENCH_TARGET) src/*.o
