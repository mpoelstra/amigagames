VBCC_ROOT := $(CURDIR)/.toolchain/sdk
NDK_INCLUDE := $(CURDIR)/.toolchain/ndk/Include_H
export VBCC := $(VBCC_ROOT)
export PATH := $(VBCC_ROOT)/bin:$(PATH)
export TMPDIR := $(CURDIR)/build/tmp

CC = $(VBCC_ROOT)/bin/vc
PYTHON ?= python3
CFLAGS ?= +aos68k -O2 -cpu=68020 -notmpfile -I$(NDK_INCLUDE)
TARGET = sparkpaw
ADF_TARGET = build/sparkpaw-adf
PHASE6_MEMORY_TARGET = build/sparkpaw-phase6-memory
RENDER_DIAG_TARGET = build/sparkpaw-renderdiag
ADF_FRONT_ASSET = build/adf-assets/storm-front.spr1
ADF_REAR_ASSET = build/adf-assets/storm-rear.spr1
ADF_STRIDER_ASSET = build/adf-assets/clockwork-storm-strider.spr1
ADF_PLAYER_ASSET = build/adf-assets/sparkpaw-sprites4.spr1
ADF_ASSETS = $(ADF_FRONT_ASSET) $(ADF_REAR_ASSET) $(ADF_STRIDER_ASSET) $(ADF_PLAYER_ASSET)
BENCH_TARGETS = sparkpaw-renderbench-4plus3 sparkpaw-renderbench-4plus4
BENCH_ASSETS = assets/runtime/renderbench-strider-idle.raw \
	assets/runtime/renderbench-rear16.raw
SOURCES = src/main.c src/renderer.c src/hud.c src/assets.c src/audio.c src/collision.c src/collectibles.c \
	src/enemies.c src/game.c src/level_data.c src/platform_amiga.c src/player.c src/projectiles.c \
	src/title.c
HEADERS = src/assets.h src/audio.h src/collision.h src/collectibles.h src/enemies.h src/game.h src/hud.h \
	src/level_data.h src/platform_amiga.h src/player.h src/projectiles.h src/renderer.h src/title.h \
	src/world_config.h
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
	assets/runtime/enemy-death.raw \
	assets/runtime/strider-shot.raw \
	assets/runtime/jump.raw \
	assets/runtime/collect-spark.raw \
	assets/runtime/water-splash.raw \
	assets/runtime/clockwork-beetle.spbm \
	assets/runtime/clockwork-storm-strider.spbm \
	assets/runtime/sparkpaw-sprites.spbm \
	assets/runtime/sparkpaw-sprites3.spbm \
	assets/runtime/sparkpaw-sprites4.spbm

.PHONY: all clean assets release bench bench-package renderdiag renderdiag-package visual-slice-proof adf-report phase6-memory

all: assets $(TARGET)

bench: $(BENCH_TARGETS)

bench-package: bench
	@mkdir -p build/renderbench-test
	cp sparkpaw-renderbench-4plus3 build/renderbench-test/Sparkpaw-Renderbench-4plus3
	cp sparkpaw-renderbench-4plus4 build/renderbench-test/Sparkpaw-Renderbench-4plus4
	cp assets/runtime/renderbench-rear16.raw build/renderbench-test/
	cp assets/runtime/renderbench-strider-idle.raw build/renderbench-test/
	cp docs/RENDERBENCH.txt build/renderbench-test/ReadMe.txt

visual-slice-proof:
	$(PYTHON) tools/generate_visual_slice_proof.py
	$(PYTHON) tools/generate_parallax_proof.py

$(BENCH_ASSETS): tools/generate_renderbench_assets.py \
	assets/levels/storm-parallax-rear16-preview.png \
	assets/enemies/clockwork-storm-strider-biped-production-v5-transparent.png
	$(PYTHON) tools/generate_renderbench_assets.py

sparkpaw-renderbench-4plus3: src/aga_renderbench.c $(BENCH_ASSETS)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -o $@ $<

sparkpaw-renderbench-4plus4: src/aga_renderbench.c $(BENCH_ASSETS)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -DREAR_PLANES=4 -o $@ $<

assets: $(RUNTIME_ASSETS)

$(RUNTIME_ASSETS): tools/generate_runtime_assets.py \
	assets/concept/sparkpaw-title-concept-aga64-preview.png \
	assets/concept/sparkpaw-level-loading-concept-v3.png \
	assets/concept/sparkpaw-level-charging-concept-v2.png \
	assets/concept/sparkpaw-parallax-master-concept-v4.png \
	assets/concept/sparkpaw-foreground-kit-concept-v2.png \
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
	assets/sprites/sparkpaw-ledge-teeter-v1-transparent.png \
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
	sfx/raw/enemy-death.raw \
	sfx/raw/strider-shot.raw \
	sfx/raw/jump.raw
	$(PYTHON) tools/generate_runtime_assets.py

sfx/raw/energy-shot.raw sfx/raw/player-hurt.raw sfx/raw/enemy-hit.raw sfx/raw/enemy-death.raw sfx/raw/strider-shot.raw \
	sfx/raw/jump.raw sfx/raw/collect-spark.raw: \
	tools/generate_sparkpaw_sfx.py
	$(PYTHON) tools/generate_sparkpaw_sfx.py

$(TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -DSPARKPAW_RENDER_DIAGNOSTIC -o $@ $(SOURCES)

$(ADF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DADF_PACKED_ASSETS -o $@ $(SOURCES)

$(PHASE6_MEMORY_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_WORLD_W=3072 -DPHASE6_MEMORY_TEST \
		-o $@ $(SOURCES)

$(ADF_FRONT_ASSET): assets/runtime/storm-front.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

$(ADF_REAR_ASSET): assets/runtime/storm-rear.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

$(ADF_STRIDER_ASSET): assets/runtime/clockwork-storm-strider.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

$(ADF_PLAYER_ASSET): assets/runtime/sparkpaw-sprites4.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

release: all $(ADF_TARGET) $(ADF_ASSETS)
	$(PYTHON) tools/make_release.py

adf-report: all
	$(PYTHON) tools/report_adf_storage.py

phase6-memory: all $(PHASE6_MEMORY_TARGET)
	$(PYTHON) tools/build_phase6_memory_test.py

renderdiag: assets $(RENDER_DIAG_TARGET)

renderdiag-package: renderdiag
	@mkdir -p build/renderdiag-test/assets/runtime
	cp $(RENDER_DIAG_TARGET) build/renderdiag-test/Sparkpaw-Renderdiag
	cp $(RUNTIME_ASSETS) build/renderdiag-test/assets/runtime/
	cp docs/RENDERDIAG.txt build/renderdiag-test/ReadMe.txt

$(RENDER_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_RENDER_DIAGNOSTIC -o $@ $(SOURCES)

clean:
	$(RM) $(TARGET) $(BENCH_TARGETS) sparkpaw-renderbench src/*.o
