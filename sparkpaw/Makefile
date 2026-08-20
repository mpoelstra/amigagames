VBCC_ROOT := $(CURDIR)/.toolchain/sdk
NDK_INCLUDE := $(CURDIR)/.toolchain/ndk/Include_H
export VBCC := $(VBCC_ROOT)
export PATH := $(VBCC_ROOT)/bin:$(PATH)
export TMPDIR := $(CURDIR)/build/tmp

CC = $(VBCC_ROOT)/bin/vc
HOST_CC ?= cc
PYTHON ?= python3
CFLAGS ?= +aos68k -O2 -cpu=68020 -notmpfile -I$(NDK_INCLUDE)
RELEASE_RENDERER_FLAGS = -DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
	-DSPARKPAW_AGA32_FETCH_CANDIDATE -DSPARKPAW_AGA32_ORIGIN_CORRECTED \
	-DSPARKPAW_AGA32_LEFT_GUARD -DSPARKPAW_AGA64_PLAYER_SPRITE
TARGET = sparkpaw
ADF_TARGET = build/sparkpaw-adf
PHASE6_MEMORY_TARGET = build/sparkpaw-phase6-memory
RENDER_DIAG_TARGET = build/sparkpaw-renderdiag
RENDER_DIAG_ADF_TARGET = build/sparkpaw-renderdiag-adf
ADF_FRONT_ASSET = build/adf-assets/storm-front.spr1
ADF_REAR_ASSET = build/adf-assets/storm-rear.spr1
ADF_STRIDER_ASSET = build/adf-assets/clockwork-storm-strider.spr1
ADF_PLAYER_ASSET = build/adf-assets/sparkpaw-sprites4.spr1
ADF_ASSETS = $(ADF_FRONT_ASSET) $(ADF_REAR_ASSET) $(ADF_STRIDER_ASSET) $(ADF_PLAYER_ASSET)
BENCH_TARGETS = sparkpaw-renderbench-4plus3 sparkpaw-renderbench-4plus4
AGA32_SCROLL_PROOF_TARGET = build/sparkpaw-aga32-scroll-proof
AGA_ORIGIN_REFERENCE_TARGET = build/sparkpaw-aga-origin-reference-fmode0
AGA_ORIGIN_CANDIDATE_TARGET = build/sparkpaw-aga-origin-candidate-fmode1
AGA_ORIGIN_CORRECTED_TARGET = build/sparkpaw-aga-origin-corrected-fmode1
AGA_LEFT_GUARD_TARGET = build/sparkpaw-aga-left-prefetch-guard-fmode1
AGA64_ORIGIN_CANDIDATE_TARGET = build/sparkpaw-aga64-origin-candidate-fmode3
AGA64_ORIGIN_CORRECTED_TARGET = build/sparkpaw-aga64-origin-corrected-fmode3
AGA64_RING_REFERENCE_TARGET = build/sparkpaw-aga64-ring-reference-fmode0
AGA64_RING_CANDIDATE_TARGET = build/sparkpaw-aga64-ring-candidate-fmode3
FMODE0_EARLY_REFERENCE_TARGET = build/sparkpaw-fmode0-ring-reference-ddf30
FMODE0_EARLY_CANDIDATE_TARGET = build/sparkpaw-fmode0-ring-candidate-ddf28
FMODE0_EARLY_CORRECTED_TARGET = build/sparkpaw-fmode0-ring-candidate-ddf28-corrected
AGA64_PLAYER_SPRITE_TARGET = build/sparkpaw-aga64-player-sprite-proof
BENCH_ASSETS = assets/runtime/renderbench-strider-idle.raw \
	assets/runtime/renderbench-rear16.raw
SOURCES = src/main.c src/renderer.c src/hud.c src/assets.c src/audio.c src/collision.c src/collectibles.c src/performance_profile.c \
	src/enemies.c src/game.c src/level_data.c src/platform_amiga.c src/player.c src/projectiles.c \
	src/title.c
HEADERS = src/assets.h src/audio.h src/beetle_hitbox.h src/collision.h src/collectibles.h src/enemies.h src/enemy_vertical_order.h src/frame_phase.h src/game.h src/hud.h \
	src/level_data.h src/performance_profile.h src/platform_amiga.h src/player.h src/projectiles.h src/renderer.h src/title.h \
	src/packed_crc32.h src/projectile_render_visibility.h src/projectile_sweep.h src/rolling_renderer_contract.h src/strider_restore_union.h src/water_update_visibility.h src/world_config.h
RUNTIME_ASSETS = assets/runtime/storm-world.spbm \
	assets/runtime/sparkpaw-title.spbm \
	assets/runtime/sparkpaw-level-loading.spbm \
	assets/runtime/level-charge-patch.spbm \
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

.PHONY: all clean test assets release bench bench-package aga32-scroll-proof aga32-scroll-proof-package aga-origin-ab-proof aga-origin-ab-proof-package aga-origin-corrected-proof aga-origin-corrected-proof-package aga-left-guard-proof-package aga64-origin-proof-package aga64-origin-corrected-proof-package aga64-ring-ab-proof-package fmode0-early-word-ab-proof-package fmode0-early-word-corrected-package aga64-player-sprite-proof-package aga64-gameplay-package aga32-gameplay-corrected-package aga32-left-guard-gameplay-package renderdiag renderdiag-package renderdiag-adf-package visual-slice-proof adf-report phase6-memory

all: assets $(TARGET)

test:
	@mkdir -p build/host-tests
	$(PYTHON) tests/test_runtime_memory_layout.py
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-beetle-hitbox tests/test_beetle_hitbox.c
	build/host-tests/test-beetle-hitbox
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-water-update-visibility tests/test_water_update_visibility.c
	build/host-tests/test-water-update-visibility
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-enemy-vertical-order tests/test_enemy_vertical_order.c
	build/host-tests/test-enemy-vertical-order
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-strider-restore-union tests/test_strider_restore_union.c
	build/host-tests/test-strider-restore-union
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-projectile-sweep tests/test_projectile_sweep.c
	build/host-tests/test-projectile-sweep
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-projectile-render-visibility tests/test_projectile_render_visibility.c
	build/host-tests/test-projectile-render-visibility
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-packed-crc32 tests/test_packed_crc32.c
	build/host-tests/test-packed-crc32
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -DSPARKPAW_HOST_TEST -o build/host-tests/test-frame-phase tests/test_frame_phase.c
	build/host-tests/test-frame-phase
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-rolling-renderer-contract tests/test_rolling_renderer_contract.c
	build/host-tests/test-rolling-renderer-contract

bench: $(BENCH_TARGETS)

bench-package: bench
	@mkdir -p build/renderbench-test
	cp sparkpaw-renderbench-4plus3 build/renderbench-test/Sparkpaw-Renderbench-4plus3
	cp sparkpaw-renderbench-4plus4 build/renderbench-test/Sparkpaw-Renderbench-4plus4
	cp assets/runtime/renderbench-rear16.raw build/renderbench-test/
	cp assets/runtime/renderbench-strider-idle.raw build/renderbench-test/
	cp docs/RENDERBENCH.txt build/renderbench-test/ReadMe.txt

aga32-scroll-proof: $(AGA32_SCROLL_PROOF_TARGET)

aga32-scroll-proof-package: aga32-scroll-proof
	@mkdir -p dist/Stage5D3-AGA32-Coordinated-DDF-Calibration-HD
	cp $(AGA32_SCROLL_PROOF_TARGET) dist/Stage5D3-AGA32-Coordinated-DDF-Calibration-HD/Sparkpaw-AGA32-DDF-Proof
	cp docs/AGA32_SCROLL_PROOF.txt dist/Stage5D3-AGA32-Coordinated-DDF-Calibration-HD/ReadMe.txt

aga-origin-ab-proof: $(AGA_ORIGIN_REFERENCE_TARGET) $(AGA_ORIGIN_CANDIDATE_TARGET)

aga-origin-ab-proof-package: aga-origin-ab-proof
	@mkdir -p dist/Stage5D4-AGA-Fetch-Origin-AB-HD/FMODE0-Reference
	@mkdir -p dist/Stage5D4-AGA-Fetch-Origin-AB-HD/FMODE1-Candidate
	cp $(AGA_ORIGIN_REFERENCE_TARGET) dist/Stage5D4-AGA-Fetch-Origin-AB-HD/FMODE0-Reference/Sparkpaw-Origin-Reference
	cp $(AGA_ORIGIN_CANDIDATE_TARGET) dist/Stage5D4-AGA-Fetch-Origin-AB-HD/FMODE1-Candidate/Sparkpaw-Origin-Candidate
	cp docs/AGA_FETCH_ORIGIN_AB_PROOF.txt dist/Stage5D4-AGA-Fetch-Origin-AB-HD/ReadMe.txt

aga-origin-corrected-proof: $(AGA_ORIGIN_CORRECTED_TARGET)

aga-origin-corrected-proof-package: aga-origin-corrected-proof
	@mkdir -p dist/Stage5D5-AGA32-Aligned-Phase-Correction-HD
	cp $(AGA_ORIGIN_CORRECTED_TARGET) dist/Stage5D5-AGA32-Aligned-Phase-Correction-HD/Sparkpaw-Origin-Corrected
	cp docs/AGA32_ORIGIN_CORRECTED_PROOF.txt dist/Stage5D5-AGA32-Aligned-Phase-Correction-HD/ReadMe.txt

aga-left-guard-proof-package: $(AGA_LEFT_GUARD_TARGET)
	@mkdir -p dist/Stage5F-AGA32-Left-Prefetch-Guard-HD
	cp $(AGA_LEFT_GUARD_TARGET) dist/Stage5F-AGA32-Left-Prefetch-Guard-HD/Sparkpaw-Left-Guard-Proof
	cp docs/AGA32_LEFT_GUARD_PROOF.txt dist/Stage5F-AGA32-Left-Prefetch-Guard-HD/ReadMe.txt

aga64-origin-proof-package: $(AGA_ORIGIN_CORRECTED_TARGET) $(AGA64_ORIGIN_CANDIDATE_TARGET)
	@mkdir -p dist/Stage5H-AGA64-Fetch-Origin-AB-HD/FMODE1-Reference
	@mkdir -p dist/Stage5H-AGA64-Fetch-Origin-AB-HD/FMODE3-Candidate
	cp $(AGA_ORIGIN_CORRECTED_TARGET) dist/Stage5H-AGA64-Fetch-Origin-AB-HD/FMODE1-Reference/Sparkpaw-Origin-Reference
	cp $(AGA64_ORIGIN_CANDIDATE_TARGET) dist/Stage5H-AGA64-Fetch-Origin-AB-HD/FMODE3-Candidate/Sparkpaw-Origin-Candidate
	cp docs/AGA64_FETCH_ORIGIN_PROOF.txt dist/Stage5H-AGA64-Fetch-Origin-AB-HD/ReadMe.txt

aga64-origin-corrected-proof-package: $(AGA64_ORIGIN_CORRECTED_TARGET)
	@mkdir -p dist/Stage5H2-AGA64-Corrected-Origin-HD
	cp $(AGA64_ORIGIN_CORRECTED_TARGET) dist/Stage5H2-AGA64-Corrected-Origin-HD/Sparkpaw-Origin-Corrected
	cp docs/AGA64_CORRECTED_ORIGIN_PROOF.txt dist/Stage5H2-AGA64-Corrected-Origin-HD/ReadMe.txt

aga64-ring-ab-proof-package: assets $(AGA64_RING_REFERENCE_TARGET) $(AGA64_RING_CANDIDATE_TARGET)
	@mkdir -p dist/Stage5J-AGA64-Production-Ring-AB-HD/FMODE0-Reference/assets/runtime
	@mkdir -p dist/Stage5J-AGA64-Production-Ring-AB-HD/FMODE3-Candidate/assets/runtime
	cp $(AGA64_RING_REFERENCE_TARGET) dist/Stage5J-AGA64-Production-Ring-AB-HD/FMODE0-Reference/Sparkpaw-Ring-Reference
	cp $(AGA64_RING_CANDIDATE_TARGET) dist/Stage5J-AGA64-Production-Ring-AB-HD/FMODE3-Candidate/Sparkpaw-Ring-Candidate
	cp $(RUNTIME_ASSETS) dist/Stage5J-AGA64-Production-Ring-AB-HD/FMODE0-Reference/assets/runtime/
	cp $(RUNTIME_ASSETS) dist/Stage5J-AGA64-Production-Ring-AB-HD/FMODE3-Candidate/assets/runtime/
	cp docs/AGA64_PRODUCTION_RING_AB_PROOF.txt dist/Stage5J-AGA64-Production-Ring-AB-HD/ReadMe.txt

fmode0-early-word-ab-proof-package: assets $(FMODE0_EARLY_REFERENCE_TARGET) $(FMODE0_EARLY_CANDIDATE_TARGET)
	@mkdir -p dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/DDF30-Reference/assets/runtime
	@mkdir -p dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/DDF28-Candidate/assets/runtime
	cp $(FMODE0_EARLY_REFERENCE_TARGET) dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/DDF30-Reference/Sparkpaw-DDF30-Reference
	cp $(FMODE0_EARLY_CANDIDATE_TARGET) dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/DDF28-Candidate/Sparkpaw-DDF28-Candidate
	cp $(RUNTIME_ASSETS) dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/DDF30-Reference/assets/runtime/
	cp $(RUNTIME_ASSETS) dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/DDF28-Candidate/assets/runtime/
	cp docs/FMODE0_HALF_STEP_PREFETCH_AB_PROOF.txt dist/Stage5K-FMODE0-Half-Step-Prefetch-AB-HD/ReadMe.txt

fmode0-early-word-corrected-package: assets $(FMODE0_EARLY_CORRECTED_TARGET)
	@mkdir -p dist/Stage5K2-FMODE0-Half-Step-Prefetch-Corrected-HD/assets/runtime
	cp $(FMODE0_EARLY_CORRECTED_TARGET) dist/Stage5K2-FMODE0-Half-Step-Prefetch-Corrected-HD/Sparkpaw-DDF28-Corrected
	cp $(RUNTIME_ASSETS) dist/Stage5K2-FMODE0-Half-Step-Prefetch-Corrected-HD/assets/runtime/
	cp docs/FMODE0_HALF_STEP_PREFETCH_CORRECTED_TEST.txt dist/Stage5K2-FMODE0-Half-Step-Prefetch-Corrected-HD/ReadMe.txt

aga64-player-sprite-proof-package: assets $(AGA64_PLAYER_SPRITE_TARGET)
	@mkdir -p dist/Stage5L-AGA64-Wide-Sparkpaw-HD/assets/runtime
	cp $(AGA64_PLAYER_SPRITE_TARGET) dist/Stage5L-AGA64-Wide-Sparkpaw-HD/Sparkpaw-Wide-Sprite-Proof
	cp $(RUNTIME_ASSETS) dist/Stage5L-AGA64-Wide-Sparkpaw-HD/assets/runtime/
	cp docs/AGA64_WIDE_PLAYER_SPRITE_PROOF.txt dist/Stage5L-AGA64-Wide-Sparkpaw-HD/ReadMe.txt

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

$(AGA32_SCROLL_PROOF_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA32_SCROLL_PROOF -o $@ $<

$(AGA_ORIGIN_REFERENCE_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA_SCROLL_CALIBRATION -o $@ $<

$(AGA_ORIGIN_CANDIDATE_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA_SCROLL_CALIBRATION -DAGA32_FETCH_MODE -o $@ $<

$(AGA_ORIGIN_CORRECTED_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA_SCROLL_CALIBRATION \
		-DAGA32_FETCH_MODE -DAGA32_ORIGIN_CORRECTION -o $@ $<

$(AGA_LEFT_GUARD_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA_SCROLL_CALIBRATION \
		-DAGA32_FETCH_MODE -DAGA32_ORIGIN_CORRECTION \
		-DAGA32_LEFT_GUARD_PROOF -o $@ $<

$(AGA64_ORIGIN_CANDIDATE_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA64_FETCH_PROOF -o $@ $<

$(AGA64_ORIGIN_CORRECTED_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DREAR_PLANES=3 -DAGA64_FETCH_PROOF \
		-DAGA64_ORIGIN_CORRECTION -o $@ $<

$(AGA64_RING_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE -o $@ $(SOURCES)

$(AGA64_RING_CANDIDATE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA64_FETCH_CANDIDATE -o $@ $(SOURCES)

$(FMODE0_EARLY_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE -o $@ $(SOURCES)

$(FMODE0_EARLY_CANDIDATE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_FMODE0_EARLY_WORD_GUARD -o $@ $(SOURCES)

$(FMODE0_EARLY_CORRECTED_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_FMODE0_EARLY_WORD_GUARD -o $@ $(SOURCES)

$(AGA64_PLAYER_SPRITE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA32_LEFT_GUARD \
		-DSPARKPAW_AGA64_PLAYER_SPRITE -o $@ $(SOURCES)

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
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(ADF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DADF_PACKED_ASSETS -DSPARKPAW_ROLLING_PROTOTYPE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(PHASE6_MEMORY_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_WORLD_W=3072 -DPHASE6_MEMORY_TEST \
		-DSPARKPAW_ROLLING_PROTOTYPE \
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
	@mkdir -p build/renderdiag-stage1-hd/assets/runtime
	cp $(RENDER_DIAG_TARGET) build/renderdiag-stage1-hd/Sparkpaw-Renderdiag
	cp $(RUNTIME_ASSETS) build/renderdiag-stage1-hd/assets/runtime/
	cp docs/RENDERDIAG.txt build/renderdiag-stage1-hd/ReadMe.txt

aga32-gameplay-corrected-package: renderdiag
	@mkdir -p dist/Stage5E-AGA32-Corrected-Gameplay-HD/assets/runtime
	cp $(RENDER_DIAG_TARGET) dist/Stage5E-AGA32-Corrected-Gameplay-HD/Sparkpaw-Renderdiag
	cp $(RUNTIME_ASSETS) dist/Stage5E-AGA32-Corrected-Gameplay-HD/assets/runtime/
	cp docs/AGA32_GAMEPLAY_CORRECTED_TEST.txt dist/Stage5E-AGA32-Corrected-Gameplay-HD/ReadMe.txt

aga32-left-guard-gameplay-package: renderdiag
	@mkdir -p dist/Stage5G-AGA32-Left-Guard-Gameplay-HD/assets/runtime
	cp $(RENDER_DIAG_TARGET) dist/Stage5G-AGA32-Left-Guard-Gameplay-HD/Sparkpaw-Renderdiag
	cp $(RUNTIME_ASSETS) dist/Stage5G-AGA32-Left-Guard-Gameplay-HD/assets/runtime/
	cp docs/AGA32_LEFT_GUARD_GAMEPLAY_TEST.txt dist/Stage5G-AGA32-Left-Guard-Gameplay-HD/ReadMe.txt

aga64-gameplay-package: renderdiag
	@mkdir -p dist/Stage5I-AGA64-Gameplay-HD/assets/runtime
	cp $(RENDER_DIAG_TARGET) dist/Stage5I-AGA64-Gameplay-HD/Sparkpaw-Renderdiag
	cp $(RUNTIME_ASSETS) dist/Stage5I-AGA64-Gameplay-HD/assets/runtime/
	cp docs/AGA64_GAMEPLAY_TEST.txt dist/Stage5I-AGA64-Gameplay-HD/ReadMe.txt

renderdiag-adf-package: assets $(ADF_ASSETS) $(RENDER_DIAG_ADF_TARGET)
	$(PYTHON) tools/make_renderdiag_adf.py

$(RENDER_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA64_FETCH_CANDIDATE -o $@ $(SOURCES)

$(RENDER_DIAG_ADF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DADF_PACKED_ASSETS -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DDIAGNOSTIC_TRACE_FRAMES=512 \
		-o $@ $(SOURCES)

clean:
	$(RM) $(TARGET) $(BENCH_TARGETS) sparkpaw-renderbench src/*.o
