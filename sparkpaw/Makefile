VBCC_ROOT := $(CURDIR)/.toolchain/sdk
NDK_INCLUDE := $(CURDIR)/.toolchain/ndk/Include_H
export VBCC := $(VBCC_ROOT)
export PATH := $(VBCC_ROOT)/bin:$(PATH)
export TMPDIR := $(CURDIR)/build/tmp

CC = $(VBCC_ROOT)/bin/vc
HOST_CC ?= cc
PYTHON ?= python3
CFLAGS ?= +aos68k -O2 -cpu=68020 -notmpfile -I$(NDK_INCLUDE)
STAGE5L_RENDERER_FLAGS = -DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
	-DSPARKPAW_AGA32_FETCH_CANDIDATE -DSPARKPAW_AGA32_ORIGIN_CORRECTED \
	-DSPARKPAW_AGA32_LEFT_GUARD -DSPARKPAW_AGA64_PLAYER_SPRITE
RELEASE_RENDERER_FLAGS = $(STAGE5L_RENDERER_FLAGS) \
	-DSPARKPAW_HUD_SEAM_FRONT_BLACK -DSPARKPAW_SPRITE_STAGE_CACHE \
	-DSPARKPAW_STORY_INTRO
TARGET = sparkpaw
ADF_TARGET = build/sparkpaw-adf
WHDLOAD_TARGET = build/sparkpaw-whdload
WHDLOAD_INTRO_DIAG_TARGET = build/sparkpaw-whdload-introdiag
PHASE6_MEMORY_TARGET = build/sparkpaw-phase6-memory
RENDER_DIAG_TARGET = build/sparkpaw-renderdiag
INTRO_PROOF_TARGET = build/sparkpaw-intro-proof
REPLAY_PROOF_TARGET = build/sparkpaw-replay-proof
INTRO_PROOF_DRAWER ?= IntroProof-030-HD
STORY_INTRO_ASSETS = assets/runtime/intro1.spbm assets/runtime/intro2.spbm \
	assets/runtime/intro3.spbm assets/runtime/intro4.spbm \
	assets/runtime/intro5.spbm
HUD_SEAM_DIAG_TARGET = build/sparkpaw-hud-seam-fmode-first
HUD_SEAM_H2_DIAG_TARGET = build/sparkpaw-hud-seam-h2-retain-ddfstop
HUD_SEAM_H3_DIAG_TARGET = build/sparkpaw-hud-seam-h3-native-3plane
HUD_SEAM_H4_DIAG_TARGET = build/sparkpaw-hud-seam-h4-right-fetch-guard
HUD_SEAM_H5_SPRITE_ISOLATION_TARGET = build/sparkpaw-hud-seam-h5-sprites-off
HUD_SEAM_H5_BITPLANE_ISOLATION_TARGET = build/sparkpaw-hud-seam-h5-bitplanes-off
HUD_SEAM_H6_FRONT_ISOLATION_TARGET = build/sparkpaw-hud-seam-h6-front16-black
HUD_SEAM_H6_REAR_ISOLATION_TARGET = build/sparkpaw-hud-seam-h6-rear8-black
HUD_SEAM_H7_PRODUCTION_TARGET = build/sparkpaw-hud-seam-h7-production
STAGE2_PROFILE_TARGET = build/sparkpaw-stage2-profile
STAGE2_MINIMAL_CADENCE_TARGET = build/sparkpaw-stage2-minimal-cadence
SCORE_CAMERA_CENTER_TARGET = build/sparkpaw-score-camera-center
SCORE_CAMERA_DEADZONE_TARGET = build/sparkpaw-score-camera-deadzone
SCORE_PROJECTILE_PIXEL_TARGET = build/sparkpaw-score-projectile-pixel
SCORE_PROJECTILE_COALESCED_TARGET = build/sparkpaw-score-projectile-coalesced
STAGE2_DYNAMIC_REFERENCE_TARGET = build/sparkpaw-stage2-dynamic-reference
STAGE2_DYNAMIC_WORD_COPY_TARGET = build/sparkpaw-stage2-dynamic-word-copy
STAGE2_DYNAMIC_UNROLLED_TARGET = build/sparkpaw-stage2-dynamic-unrolled
STAGE2_CANONICAL_REFERENCE_TARGET = build/sparkpaw-stage2-canonical-reference
STAGE2_CANONICAL_WORD_TARGET = build/sparkpaw-stage2-canonical-word-copy
STAGE2_COLLISION_REFERENCE_TARGET = build/sparkpaw-stage2-collision-pixel
STAGE2_COLLISION_TILE_TARGET = build/sparkpaw-stage2-collision-tile
STAGE2_ENEMY_CACHE_WORD_TARGET = build/sparkpaw-stage2-enemy-cache-word
STAGE2_PLAYER_SPRITE_WORD_TARGET = build/sparkpaw-stage2-player-sprite-word
STAGE2_SPRITE_STAGE_REFERENCE_TARGET = build/sparkpaw-stage2-sprite-stage-reference
STAGE2_SPRITE_STAGE_CACHE_TARGET = build/sparkpaw-stage2-sprite-stage-cache
STAGE2_RING_REACH_REFERENCE_TARGET = build/sparkpaw-stage2-ring-all-copies
STAGE2_RING_REACH_CANDIDATE_TARGET = build/sparkpaw-stage2-ring-fetch-copies
STAGE2_COPPER_COPY_REFERENCE_TARGET = build/sparkpaw-stage2-copper-full-copy
STAGE2_COPPER_PATCH_TARGET = build/sparkpaw-stage2-copper-patch-only
STAGE2_COLUMN_CPU_TARGET = build/sparkpaw-stage2-column-cpu
STAGE2_COLUMN_BLIT_TARGET = build/sparkpaw-stage2-column-multiplane-blit
STAGE2_CANONICAL_RESTORE_TARGET = build/sparkpaw-stage2-canonical-restore
STAGE2_TARGET_CLEAN_REFERENCE_TARGET = build/sparkpaw-stage2-target-clean-reference
STAGE2_ENEMY_COPY_REFERENCE_TARGET = build/sparkpaw-stage2-enemy-copy-reference
STAGE2_ENEMY_COPY_ON_UNLOAD_TARGET = build/sparkpaw-stage2-enemy-copy-on-unload
STAGE2_ENEMY_TRAVERSAL_LINEAR_REFERENCE_TARGET = build/sparkpaw-stage2-enemy-traversal-linear-reference
STAGE2_BOB_PER_PLANE_REFERENCE_TARGET = build/sparkpaw-stage2-bob-per-plane-reference
STAGE2_RING_COLUMN_GENERIC_REFERENCE_TARGET = build/sparkpaw-stage2-ring-column-generic-reference
STAGE2_HAZARD_SCAN_REFERENCE_TARGET = build/sparkpaw-stage2-hazard-scan-reference
STAGE2_HAZARD_CACHE_TARGET = build/sparkpaw-stage2-hazard-cache
STAGE2_PHASE_LINE100_TARGET = build/sparkpaw-stage2-phase-line100
STAGE2_PHASE_IMMEDIATE_TARGET = build/sparkpaw-stage2-phase-immediate
STAGE2_COLLECTIBLE_CANONICAL_TARGET = build/sparkpaw-stage2-collectible-canonical
STAGE2_COLLECTIBLE_H4_TARGET = build/sparkpaw-stage2-collectible-h4
RENDER_DIAG_ADF_TARGET = build/sparkpaw-renderdiag-adf
ADF_FRONT_ASSET = build/adf-assets/storm-front.spr1
ADF_REAR_ASSET = build/adf-assets/storm-rear.spr1
ADF_STRIDER_ASSET = build/adf-assets/clockwork-storm-strider.spr1
ADF_PLAYER_ASSET = build/adf-assets/sparkpaw-sprites4.spr1
ADF_INTRO_ASSETS = build/adf-assets/intro1.spr1 \
	build/adf-assets/intro2.spr1 build/adf-assets/intro3.spr1 \
	build/adf-assets/intro4.spr1 build/adf-assets/intro5.spr1
ADF_STATUS_ASSETS = build/adf-assets/sparkpaw-level-loading.spr1 \
	build/adf-assets/level-charge-patch.spr1 \
	build/adf-assets/level-ready.spr1 \
	build/adf-assets/level-ready-menu.spr1 \
	build/adf-assets/level-complete.spr1
ADF_ASSETS = $(ADF_FRONT_ASSET) $(ADF_REAR_ASSET) $(ADF_STRIDER_ASSET) \
	$(ADF_PLAYER_ASSET) $(ADF_STATUS_ASSETS)
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
HEADERS = src/assets.h src/audio.h src/audio_contract.h src/extra_life_contract.h src/beetle_hitbox.h src/collision.h src/collectibles.h src/control_options.h src/enemies.h src/enemy_vertical_order.h src/frame_phase.h src/game.h src/hud.h \
	src/level_data.h src/performance_profile.h src/platform_amiga.h src/player.h src/projectiles.h src/renderer.h src/title.h \
	src/camera_contract.h src/collision_hazard_cache.h src/packed_crc32.h src/projectile_render_visibility.h src/projectile_sweep.h src/rolling_renderer_contract.h src/sprite_stage_cache.h src/strider_restore_union.h src/water_update_visibility.h src/world_config.h
RUNTIME_ASSETS = assets/runtime/storm-world.spbm \
	assets/runtime/sparkpaw-title.spbm \
	assets/runtime/sparkpaw-level-loading.spbm \
	assets/runtime/level-charge-patch.spbm \
	assets/runtime/sparkpaw-hud-base.spbm \
	assets/runtime/sparkpaw-hud-health.spbm \
	assets/runtime/sparkpaw-hud-lives.spbm \
	assets/runtime/sparkpaw-hud-diamonds.spbm \
	assets/runtime/sparkpaw-hud-score.spbm \
	assets/runtime/sparkpaw-diamond.spbm \
	assets/runtime/stormstone-core.spbm \
	assets/runtime/sparkpaw-extra-life.spbm \
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
	assets/runtime/stormstone-core.raw \
	assets/runtime/tally-tick.raw \
	assets/runtime/extra-life.raw \
	assets/runtime/sparkpaw-level-complete.spbm \
	assets/runtime/sparkpaw-score-glyphs.spbm \
	assets/runtime/clockwork-beetle.spbm \
	assets/runtime/clockwork-storm-strider.spbm \
	assets/runtime/sparkpaw-sprites.spbm \
	assets/runtime/sparkpaw-sprites3.spbm \
	assets/runtime/sparkpaw-sprites4.spbm

.PHONY: all clean test assets release whdload whdload-introdiag-package bench bench-package renderdiag
.PHONY: aga32-scroll-proof aga32-scroll-proof-package aga-origin-ab-proof aga-origin-ab-proof-package aga-origin-corrected-proof aga-origin-corrected-proof-package aga-left-guard-proof-package
.PHONY: aga64-origin-proof-package aga64-origin-corrected-proof-package aga64-ring-ab-proof-package fmode0-early-word-ab-proof-package fmode0-early-word-corrected-package aga64-player-sprite-proof-package aga64-gameplay-package aga32-gameplay-corrected-package aga32-left-guard-gameplay-package
.PHONY: renderdiag-package renderdiag-adf-package hud-seam-diagnostic-package hud-seam-h2-diagnostic-package hud-seam-h3-diagnostic-package hud-seam-h4-diagnostic-package hud-seam-h5-source-isolation-package hud-seam-h6-playfield-isolation-package hud-seam-h7-production-package
.PHONY: stage2-profile-package stage2-profile-68020-package stage2-residual-cost-profile-package stage2-minimal-cadence-package stage2-enemy-breakdown-68020-package stage2-loading-profile-package stage2-setup-breakdown-package stage2-enemy-cache-word-68030-package stage2-enemy-cache-word-68020-package stage2-player-sprite-word-68030-package stage2-player-sprite-word-68020-package
.PHONY: stage2-dynamic-word-copy-ab-package stage2-dynamic-word-copy-68020-ab-package stage2-dynamic-unrolled-68030-ab-package stage2-dynamic-unrolled-68020-ab-package stage2-canonical-word-68030-ab-package stage2-canonical-word-68020-ab-package stage2-collision-tile-68030-ab-package stage2-collision-tile-68020-ab-package stage2-sprite-stage-cache-68030-ab-package stage2-sprite-stage-cache-68020-ab-package stage2-ring-reach-68030-ab-package stage2-ring-reach-68020-ab-package stage2-ring-reach-h3-68030-package
.PHONY: stage2-copper-patch-68030-ab-package stage2-copper-patch-68020-ab-package
.PHONY: stage2-column-blit-68030-ab-package stage2-column-blit-68020-ab-package
.PHONY: stage2-column-layout-probe-package
.PHONY: stage2-canonical-restore-68030-ab-package stage2-canonical-restore-68020-ab-package stage2-canonical-restore-production-68030-package
.PHONY: stage2-enemy-copy-on-unload-68030-ab-package stage2-enemy-copy-on-unload-68020-ab-package
.PHONY: stage2-enemy-traversal-lookup-68030-ab-package
.PHONY: stage2-enemy-traversal-lookup-68020-ab-package
.PHONY: stage2-bob-hoist-setup-68030-ab-package
.PHONY: stage2-bob-hoist-setup-68020-ab-package
.PHONY: stage2-ring-column-specialized-68030-ab-package
.PHONY: stage2-ring-column-specialized-68020-ab-package
.PHONY: stage2-hazard-cache-68030-ab-package stage2-hazard-cache-68020-ab-package
.PHONY: stage2-phase-start-68030-ab-package stage2-phase-start-68020-ab-package
.PHONY: stage2-collectible-h4-68030-ab-package stage2-collectible-h4-68020-ab-package
.PHONY: visual-slice-proof adf-report phase6-memory intro-proof-package

all: assets $(TARGET)

test:
	@mkdir -p build/host-tests
	$(PYTHON) tests/test_runtime_memory_layout.py
	$(PYTHON) tests/test_sparkpaw_icon.py
	$(PYTHON) tests/test_shared_diamond_master.py
	$(PYTHON) tests/test_extra_life_asset.py
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-audio-contract tests/test_audio_contract.c
	build/host-tests/test-audio-contract
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-extra-life-contract tests/test_extra_life_contract.c
	build/host-tests/test-extra-life-contract
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-control-options tests/test_control_options.c
	build/host-tests/test-control-options
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-camera-contract tests/test_camera_contract.c
	build/host-tests/test-camera-contract
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
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-enemy-cache-word-conversion tests/test_enemy_cache_word_conversion.c
	build/host-tests/test-enemy-cache-word-conversion
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-player-sprite-word-conversion tests/test_player_sprite_word_conversion.c
	build/host-tests/test-player-sprite-word-conversion
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-collision-tile-spans tests/test_collision_tile_spans.c
	build/host-tests/test-collision-tile-spans
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-sprite-stage-cache tests/test_sprite_stage_cache.c
	build/host-tests/test-sprite-stage-cache
	$(HOST_CC) -std=c99 -Wall -Wextra -Werror -o build/host-tests/test-collision-hazard-cache tests/test_collision_hazard_cache.c
	build/host-tests/test-collision-hazard-cache

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

READY_SCREEN_ASSET = assets/runtime/sparkpaw-ready-screen.spbm
READY_MENU_ASSET = assets/runtime/readymenu.spbm

assets: $(RUNTIME_ASSETS) $(STORY_INTRO_ASSETS) $(READY_SCREEN_ASSET) $(READY_MENU_ASSET)

$(READY_SCREEN_ASSET) $(READY_MENU_ASSET): tools/generate_ready_screen.py tools/generate_intro_proof.py \
	docs/concepts/ready-screen/assets/sparkpaw-ready-background-source-v3.png \
	docs/concepts/ready-screen/assets/sparkpaw-logo-wordmark-source-v2.png
	$(PYTHON) tools/generate_ready_screen.py

$(RUNTIME_ASSETS): tools/generate_runtime_assets.py tools/generate_score_screen.py \
	docs/concepts/score-system/sparkpaw-level-complete-score-concept-v4-title-character.png \
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
	sfx/raw/jump.raw \
	sfx/raw/tally-tick.raw \
	sfx/raw/extra-life.raw
	$(PYTHON) tools/generate_runtime_assets.py

$(STORY_INTRO_ASSETS): tools/generate_intro_proof.py \
	docs/concepts/story-intro/assets/intro-plate-01-balance-source-v2-aga-polish.png \
	docs/concepts/story-intro/assets/intro-plate-02-instruction-source-v2-aga-polish.png \
	docs/concepts/story-intro/assets/intro-plate-03-reversed-network-source-v5-from-scratch-aga.png \
	docs/concepts/story-intro/assets/intro-plate-04-motive-source-v4-aga-polish.png \
	docs/concepts/story-intro/assets/intro-plate-05-quest-source-v2-aga-polish.png
	$(PYTHON) tools/generate_intro_proof.py

sfx/raw/energy-shot.raw sfx/raw/player-hurt.raw sfx/raw/enemy-hit.raw sfx/raw/enemy-death.raw sfx/raw/strider-shot.raw \
sfx/raw/jump.raw sfx/raw/collect-spark.raw sfx/raw/tally-tick.raw sfx/raw/extra-life.raw: \
	tools/generate_sparkpaw_sfx.py
	$(PYTHON) tools/generate_sparkpaw_sfx.py

$(TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS) $(STORY_INTRO_ASSETS) $(READY_SCREEN_ASSET) $(READY_MENU_ASSET)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(WHDLOAD_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS) $(STORY_INTRO_ASSETS) $(READY_SCREEN_ASSET) $(READY_MENU_ASSET)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_WHDLOAD -DSPARKPAW_ROLLING_PROTOTYPE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(WHDLOAD_INTRO_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS) $(STORY_INTRO_ASSETS) $(READY_SCREEN_ASSET) $(READY_MENU_ASSET)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_WHDLOAD \
		-DSPARKPAW_WHDLOAD_INTRO_DIAGNOSTIC \
		-DSPARKPAW_ROLLING_PROTOTYPE $(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(INTRO_PROOF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS) \
		$(STORY_INTRO_ASSETS) $(READY_SCREEN_ASSET) $(READY_MENU_ASSET)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_STORY_INTRO -DSPARKPAW_ROLLING_PROTOTYPE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(REPLAY_PROOF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS) \
		$(READY_SCREEN_ASSET) $(READY_MENU_ASSET)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_REPLAY_PROOF -DSPARKPAW_ROLLING_PROTOTYPE \
		$(filter-out -DSPARKPAW_STORY_INTRO,$(RELEASE_RENDERER_FLAGS)) \
		-o $@ $(SOURCES)

replay-proof: $(REPLAY_PROOF_TARGET)

intro-proof-package: $(INTRO_PROOF_TARGET)
	$(PYTHON) tools/stage_hd_test.py \
		--drawer $(INTRO_PROOF_DRAWER) \
		--executable $(INTRO_PROOF_TARGET) \
		--executable-name Sparkpaw-Intro-Proof \
		--readme docs/concepts/story-intro/INTRO_PROOF_TEST.txt

$(ADF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS) $(STORY_INTRO_ASSETS) $(READY_SCREEN_ASSET) $(READY_MENU_ASSET)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DADF_PACKED_ASSETS -DSPARKPAW_ROLLING_PROTOTYPE \
		$(filter-out -DSPARKPAW_STORY_INTRO,$(RELEASE_RENDERER_FLAGS)) \
		-o $@ $(SOURCES)

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

build/adf-assets/intro1.spr1: assets/runtime/intro1.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@
build/adf-assets/intro2.spr1: assets/runtime/intro2.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@
build/adf-assets/intro3.spr1: assets/runtime/intro3.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@
build/adf-assets/intro4.spr1: assets/runtime/intro4.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@
build/adf-assets/intro5.spr1: assets/runtime/intro5.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

build/adf-assets/sparkpaw-level-loading.spr1: assets/runtime/sparkpaw-level-loading.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

build/adf-assets/level-charge-patch.spr1: assets/runtime/level-charge-patch.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

build/adf-assets/level-ready.spr1: assets/runtime/sparkpaw-ready-screen.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

build/adf-assets/level-ready-menu.spr1: assets/runtime/readymenu.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@

build/adf-assets/level-complete.spr1: assets/runtime/sparkpaw-level-complete.spbm tools/pack_adf_asset.py
	$(PYTHON) tools/pack_adf_asset.py $< $@


release: all $(ADF_TARGET) $(ADF_ASSETS) $(WHDLOAD_TARGET)
	$(PYTHON) tools/make_release.py
	$(PYTHON) tools/make_whdload.py

whdload: all $(WHDLOAD_TARGET)
	$(PYTHON) tools/make_whdload.py

whdload-introdiag-package: assets $(WHDLOAD_INTRO_DIAG_TARGET)
	$(PYTHON) tools/make_whdload.py --intro-diagnostic

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

hud-seam-diagnostic-package: assets $(HUD_SEAM_DIAG_TARGET)
	@mkdir -p dist/Stage5L-H1-HUD-Seam-FMODE-First-HD/assets/runtime
	cp $(HUD_SEAM_DIAG_TARGET) dist/Stage5L-H1-HUD-Seam-FMODE-First-HD/Sparkpaw-HUD-Seam-Diagnostic
	cp $(RUNTIME_ASSETS) dist/Stage5L-H1-HUD-Seam-FMODE-First-HD/assets/runtime/
	cp docs/HUD_SEAM_FMODE_FIRST_TEST.txt dist/Stage5L-H1-HUD-Seam-FMODE-First-HD/ReadMe.txt

hud-seam-h2-diagnostic-package: assets $(HUD_SEAM_H2_DIAG_TARGET)
	@mkdir -p dist/Stage5L-H2-HUD-Seam-Retain-DDFSTOP-HD/assets/runtime
	cp $(HUD_SEAM_H2_DIAG_TARGET) dist/Stage5L-H2-HUD-Seam-Retain-DDFSTOP-HD/Sparkpaw-HUD-Seam-Diagnostic
	cp $(RUNTIME_ASSETS) dist/Stage5L-H2-HUD-Seam-Retain-DDFSTOP-HD/assets/runtime/
	cp docs/HUD_SEAM_H2_RETAIN_DDFSTOP_TEST.txt dist/Stage5L-H2-HUD-Seam-Retain-DDFSTOP-HD/ReadMe.txt

hud-seam-h3-diagnostic-package: assets $(HUD_SEAM_H3_DIAG_TARGET)
	@mkdir -p dist/Stage5L-H3-HUD-Native-3Plane-HD/assets/runtime
	cp $(HUD_SEAM_H3_DIAG_TARGET) dist/Stage5L-H3-HUD-Native-3Plane-HD/Sparkpaw-HUD-Seam-Diagnostic
	cp $(RUNTIME_ASSETS) dist/Stage5L-H3-HUD-Native-3Plane-HD/assets/runtime/
	cp docs/HUD_SEAM_H3_NATIVE_3PLANE_TEST.txt dist/Stage5L-H3-HUD-Native-3Plane-HD/ReadMe.txt

hud-seam-h4-diagnostic-package: assets $(HUD_SEAM_H4_DIAG_TARGET)
	@mkdir -p dist/Stage5L-H4-HUD-Right-Fetch-Guard-HD/assets/runtime
	cp $(HUD_SEAM_H4_DIAG_TARGET) dist/Stage5L-H4-HUD-Right-Fetch-Guard-HD/Sparkpaw-HUD-Seam-Diagnostic
	cp $(RUNTIME_ASSETS) dist/Stage5L-H4-HUD-Right-Fetch-Guard-HD/assets/runtime/
	cp docs/HUD_SEAM_H4_RIGHT_FETCH_GUARD_TEST.txt dist/Stage5L-H4-HUD-Right-Fetch-Guard-HD/ReadMe.txt

hud-seam-h5-source-isolation-package: assets $(HUD_SEAM_H5_SPRITE_ISOLATION_TARGET) $(HUD_SEAM_H5_BITPLANE_ISOLATION_TARGET)
	@mkdir -p dist/Stage5L-H5-Seam-Source-Isolation-AB-HD/assets/runtime
	cp $(HUD_SEAM_H5_SPRITE_ISOLATION_TARGET) dist/Stage5L-H5-Seam-Source-Isolation-AB-HD/A-Sprites-Off
	cp $(HUD_SEAM_H5_BITPLANE_ISOLATION_TARGET) dist/Stage5L-H5-Seam-Source-Isolation-AB-HD/B-Bitplanes-Off
	cp $(RUNTIME_ASSETS) dist/Stage5L-H5-Seam-Source-Isolation-AB-HD/assets/runtime/
	cp docs/HUD_SEAM_H5_SOURCE_ISOLATION_TEST.txt dist/Stage5L-H5-Seam-Source-Isolation-AB-HD/ReadMe.txt

hud-seam-h6-playfield-isolation-package: assets $(HUD_SEAM_H6_FRONT_ISOLATION_TARGET) $(HUD_SEAM_H6_REAR_ISOLATION_TARGET)
	@mkdir -p dist/Stage5L-H6A-FRONT16-Palette-Isolation-HD/assets/runtime
	@mkdir -p dist/Stage5L-H6B-REAR8-Palette-Isolation-HD/assets/runtime
	cp $(HUD_SEAM_H6_FRONT_ISOLATION_TARGET) dist/Stage5L-H6A-FRONT16-Palette-Isolation-HD/Sparkpaw-H6A-FRONT16-Black
	cp $(RUNTIME_ASSETS) dist/Stage5L-H6A-FRONT16-Palette-Isolation-HD/assets/runtime/
	cp docs/HUD_SEAM_H6A_FRONT16_TEST.txt dist/Stage5L-H6A-FRONT16-Palette-Isolation-HD/ReadMe.txt
	cp $(HUD_SEAM_H6_REAR_ISOLATION_TARGET) dist/Stage5L-H6B-REAR8-Palette-Isolation-HD/Sparkpaw-H6B-REAR8-Black
	cp $(RUNTIME_ASSETS) dist/Stage5L-H6B-REAR8-Palette-Isolation-HD/assets/runtime/
	cp docs/HUD_SEAM_H6B_REAR8_TEST.txt dist/Stage5L-H6B-REAR8-Palette-Isolation-HD/ReadMe.txt

hud-seam-h7-production-package: assets $(HUD_SEAM_H7_PRODUCTION_TARGET)
	@mkdir -p dist/Stage5L-H7-FRONT16-Seam-Fix-Production-HD/assets/runtime
	cp $(HUD_SEAM_H7_PRODUCTION_TARGET) dist/Stage5L-H7-FRONT16-Seam-Fix-Production-HD/Sparkpaw
	cp $(RUNTIME_ASSETS) dist/Stage5L-H7-FRONT16-Seam-Fix-Production-HD/assets/runtime/
	cp docs/HUD_SEAM_H7_PRODUCTION_TEST.txt dist/Stage5L-H7-FRONT16-Seam-Fix-Production-HD/ReadMe.txt

stage2-profile-package: assets $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Whole-Codebase-Profiler-HD/assets/runtime
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Whole-Codebase-Profiler-HD/Sparkpaw-Stage2-Profiler
	cp $(RUNTIME_ASSETS) dist/Stage2-Whole-Codebase-Profiler-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PROFILE_TEST.txt dist/Stage2-Whole-Codebase-Profiler-HD/ReadMe.txt

stage2-profile-68020-package: assets $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Whole-Codebase-Profiler-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Whole-Codebase-Profiler-FS-UAE-68020-HD/Sparkpaw-Stage2-Profiler-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Whole-Codebase-Profiler-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PROFILE_68020_TEST.txt dist/Stage2-Whole-Codebase-Profiler-FS-UAE-68020-HD/ReadMe.txt

stage2-residual-cost-profile-package: assets $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68030-HD/Sparkpaw-Residual-Cost-Profiler-68030
	cp $(RUNTIME_ASSETS) dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RESIDUAL_COST_PROFILE_TEST.txt dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68020-HD/Sparkpaw-Residual-Cost-Profiler-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RESIDUAL_COST_PROFILE_TEST.txt dist/Stage2-Post-Alpha45-Residual-Cost-Profiler-FS-UAE-68020-HD/ReadMe.txt

stage2-minimal-cadence-package: assets $(STAGE2_MINIMAL_CADENCE_TARGET)
	@mkdir -p dist/Stage2-Minimal-Cadence-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Minimal-Cadence-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_MINIMAL_CADENCE_TARGET) dist/Stage2-Minimal-Cadence-FS-UAE-68030-HD/Sparkpaw-Minimal-Cadence-68030
	cp $(RUNTIME_ASSETS) dist/Stage2-Minimal-Cadence-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_MINIMAL_CADENCE_TEST.txt dist/Stage2-Minimal-Cadence-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_MINIMAL_CADENCE_TARGET) dist/Stage2-Minimal-Cadence-FS-UAE-68020-HD/Sparkpaw-Minimal-Cadence-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Minimal-Cadence-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_MINIMAL_CADENCE_TEST.txt dist/Stage2-Minimal-Cadence-FS-UAE-68020-HD/ReadMe.txt

stage2-enemy-breakdown-68020-package: assets $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Enemy-Breakdown-Profiler-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Enemy-Breakdown-Profiler-FS-UAE-68020-HD/Sparkpaw-Enemy-Breakdown-Profiler
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Breakdown-Profiler-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_BREAKDOWN_TEST.txt dist/Stage2-Enemy-Breakdown-Profiler-FS-UAE-68020-HD/ReadMe.txt

stage2-enemy-traversal-lookup-68030-ab-package: assets $(STAGE2_ENEMY_TRAVERSAL_LINEAR_REFERENCE_TARGET) $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_ENEMY_TRAVERSAL_LINEAR_REFERENCE_TARGET) dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68030-HD/Sparkpaw-A-Linear-Search
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_TRAVERSAL_A_68030_TEST.txt dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68030-HD/Sparkpaw-B-Direct-Lookup
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_TRAVERSAL_B_68030_TEST.txt dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68030-HD/ReadMe.txt

stage2-enemy-traversal-lookup-68020-ab-package: assets $(STAGE2_ENEMY_TRAVERSAL_LINEAR_REFERENCE_TARGET) $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_ENEMY_TRAVERSAL_LINEAR_REFERENCE_TARGET) dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68020-HD/Sparkpaw-A-Linear-Search
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_TRAVERSAL_A_68020_TEST.txt dist/Stage2-Enemy-Traversal-A-Linear-Search-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68020-HD/Sparkpaw-B-Direct-Lookup
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_TRAVERSAL_B_68020_TEST.txt dist/Stage2-Enemy-Traversal-B-Direct-Lookup-FS-UAE-68020-HD/ReadMe.txt

stage2-bob-hoist-setup-68030-ab-package: assets $(STAGE2_BOB_PER_PLANE_REFERENCE_TARGET) $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_BOB_PER_PLANE_REFERENCE_TARGET) dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68030-HD/Sparkpaw-A-Per-Plane-Setup
	cp $(RUNTIME_ASSETS) dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_BOB_SETUP_A_68030_TEST.txt dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68030-HD/Sparkpaw-B-Hoisted-Setup
	cp $(RUNTIME_ASSETS) dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_BOB_SETUP_B_68030_TEST.txt dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68030-HD/ReadMe.txt

stage2-bob-hoist-setup-68020-ab-package: assets $(STAGE2_BOB_PER_PLANE_REFERENCE_TARGET) $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_BOB_PER_PLANE_REFERENCE_TARGET) dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68020-HD/Sparkpaw-A-Per-Plane-Setup
	cp $(RUNTIME_ASSETS) dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_BOB_SETUP_A_68020_TEST.txt dist/Stage2-Bob-Setup-A-Per-Plane-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68020-HD/Sparkpaw-B-Hoisted-Setup
	cp $(RUNTIME_ASSETS) dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_BOB_SETUP_B_68020_TEST.txt dist/Stage2-Bob-Setup-B-Hoisted-FS-UAE-68020-HD/ReadMe.txt

stage2-ring-column-specialized-68030-ab-package: assets $(STAGE2_RING_COLUMN_GENERIC_REFERENCE_TARGET) $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Ring-Column-A-Generic-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_RING_COLUMN_GENERIC_REFERENCE_TARGET) dist/Stage2-Ring-Column-A-Generic-FS-UAE-68030-HD/Sparkpaw-A-Generic-Column
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Column-A-Generic-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_COLUMN_A_68030_TEST.txt dist/Stage2-Ring-Column-A-Generic-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68030-HD/Sparkpaw-B-Specialized-Column
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_COLUMN_B_68030_TEST.txt dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68030-HD/ReadMe.txt

stage2-ring-column-specialized-68020-ab-package: assets $(STAGE2_RING_COLUMN_GENERIC_REFERENCE_TARGET) $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-Ring-Column-A-Generic-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_RING_COLUMN_GENERIC_REFERENCE_TARGET) dist/Stage2-Ring-Column-A-Generic-FS-UAE-68020-HD/Sparkpaw-A-Generic-Column
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Column-A-Generic-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_COLUMN_A_68020_TEST.txt dist/Stage2-Ring-Column-A-Generic-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68020-HD/Sparkpaw-B-Specialized-Column
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_COLUMN_B_68020_TEST.txt dist/Stage2-Ring-Column-B-Specialized-FS-UAE-68020-HD/ReadMe.txt

stage2-loading-profile-package: assets $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-CHARGING-Profiler-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-CHARGING-Profiler-FS-UAE-68020-HD/Sparkpaw-CHARGING-Profiler
	cp $(RUNTIME_ASSETS) dist/Stage2-CHARGING-Profiler-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_LOADING_PROFILE_TEST.txt dist/Stage2-CHARGING-Profiler-FS-UAE-68020-HD/ReadMe.txt

stage2-setup-breakdown-package: assets $(STAGE2_PROFILE_TARGET)
	@mkdir -p dist/Stage2-CHARGING-Setup-Breakdown-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PROFILE_TARGET) dist/Stage2-CHARGING-Setup-Breakdown-FS-UAE-68020-HD/Sparkpaw-Setup-Breakdown
	cp $(RUNTIME_ASSETS) dist/Stage2-CHARGING-Setup-Breakdown-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_SETUP_BREAKDOWN_TEST.txt dist/Stage2-CHARGING-Setup-Breakdown-FS-UAE-68020-HD/ReadMe.txt

stage2-enemy-cache-word-68030-package: assets $(STAGE2_ENEMY_CACHE_WORD_TARGET)
	@mkdir -p dist/Stage2-Enemy-Cache-H2-Guard-Corrected-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_ENEMY_CACHE_WORD_TARGET) dist/Stage2-Enemy-Cache-H2-Guard-Corrected-FS-UAE-68030-HD/Sparkpaw-Enemy-Cache-H2
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Cache-H2-Guard-Corrected-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_CACHE_WORD_68030_TEST.txt dist/Stage2-Enemy-Cache-H2-Guard-Corrected-FS-UAE-68030-HD/ReadMe.txt

stage2-enemy-cache-word-68020-package: assets $(STAGE2_ENEMY_CACHE_WORD_TARGET)
	@mkdir -p dist/Stage2-Enemy-Cache-H2-Loading-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_ENEMY_CACHE_WORD_TARGET) dist/Stage2-Enemy-Cache-H2-Loading-FS-UAE-68020-HD/Sparkpaw-Enemy-Cache-H2-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-Cache-H2-Loading-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_CACHE_WORD_68020_TEST.txt dist/Stage2-Enemy-Cache-H2-Loading-FS-UAE-68020-HD/ReadMe.txt

stage2-player-sprite-word-68030-package: assets $(STAGE2_PLAYER_SPRITE_WORD_TARGET)
	@mkdir -p dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_PLAYER_SPRITE_WORD_TARGET) dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68030-HD/Sparkpaw-Player-Sprite-Cache-Word
	cp $(RUNTIME_ASSETS) dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PLAYER_SPRITE_WORD_68030_TEST.txt dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68030-HD/ReadMe.txt

stage2-player-sprite-word-68020-package: assets $(STAGE2_PLAYER_SPRITE_WORD_TARGET)
	@mkdir -p dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PLAYER_SPRITE_WORD_TARGET) dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68020-HD/Sparkpaw-Player-Sprite-Cache-Word-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PLAYER_SPRITE_WORD_68020_TEST.txt dist/Stage2-Player-Sprite-Cache-Word-Copy-FS-UAE-68020-HD/ReadMe.txt

stage2-dynamic-unrolled-68030-ab-package: assets $(STAGE2_DYNAMIC_WORD_COPY_TARGET) $(STAGE2_DYNAMIC_UNROLLED_TARGET)
	@mkdir -p dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_DYNAMIC_WORD_COPY_TARGET) dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68030-HD/Sparkpaw-A-Indexed
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_UNROLLED_A_TEST.txt dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_DYNAMIC_UNROLLED_TARGET) dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68030-HD/Sparkpaw-B-Unrolled
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_UNROLLED_B_TEST.txt dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68030-HD/ReadMe.txt

stage2-dynamic-unrolled-68020-ab-package: assets $(STAGE2_DYNAMIC_WORD_COPY_TARGET) $(STAGE2_DYNAMIC_UNROLLED_TARGET)
	@mkdir -p dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_DYNAMIC_WORD_COPY_TARGET) dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68020-HD/Sparkpaw-A-Indexed-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_UNROLLED_68020_A_TEST.txt dist/Stage2-Dynamic-Sync-A-Indexed-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_DYNAMIC_UNROLLED_TARGET) dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68020-HD/Sparkpaw-B-Unrolled-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_UNROLLED_68020_B_TEST.txt dist/Stage2-Dynamic-Sync-B-Unrolled-FS-UAE-68020-HD/ReadMe.txt

stage2-canonical-word-68030-ab-package: assets $(STAGE2_CANONICAL_REFERENCE_TARGET) $(STAGE2_CANONICAL_WORD_TARGET)
	@mkdir -p dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_CANONICAL_REFERENCE_TARGET) dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68030-HD/Sparkpaw-A-CopyMem
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_COPYMEM_68030_TEST.txt dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_CANONICAL_WORD_TARGET) dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68030-HD/Sparkpaw-B-Word-Copy
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_WORD_68030_TEST.txt dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68030-HD/ReadMe.txt

stage2-canonical-word-68020-ab-package: assets $(STAGE2_CANONICAL_REFERENCE_TARGET) $(STAGE2_CANONICAL_WORD_TARGET)
	@mkdir -p dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_CANONICAL_REFERENCE_TARGET) dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68020-HD/Sparkpaw-A-CopyMem-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_COPYMEM_68020_TEST.txt dist/Stage2-Ring-Roll-A-CopyMem-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_CANONICAL_WORD_TARGET) dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68020-HD/Sparkpaw-B-Word-Copy-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_WORD_68020_TEST.txt dist/Stage2-Ring-Roll-B-Word-Copy-FS-UAE-68020-HD/ReadMe.txt

stage2-collision-tile-68030-ab-package: assets $(STAGE2_COLLISION_REFERENCE_TARGET) $(STAGE2_COLLISION_TILE_TARGET)
	@mkdir -p dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_COLLISION_REFERENCE_TARGET) dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68030-HD/Sparkpaw-A-Pixel-Spans
	cp $(RUNTIME_ASSETS) dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLISION_PIXEL_68030_TEST.txt dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_COLLISION_TILE_TARGET) dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68030-HD/Sparkpaw-B-Tile-Spans
	cp $(RUNTIME_ASSETS) dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLISION_TILE_68030_TEST.txt dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68030-HD/ReadMe.txt

stage2-collision-tile-68020-ab-package: assets $(STAGE2_COLLISION_REFERENCE_TARGET) $(STAGE2_COLLISION_TILE_TARGET)
	@mkdir -p dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_COLLISION_REFERENCE_TARGET) dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68020-HD/Sparkpaw-A-Pixel-Spans-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLISION_PIXEL_68020_TEST.txt dist/Stage2-Collision-A-Pixel-Spans-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_COLLISION_TILE_TARGET) dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68020-HD/Sparkpaw-B-Tile-Spans-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLISION_TILE_68020_TEST.txt dist/Stage2-Collision-B-Tile-Spans-FS-UAE-68020-HD/ReadMe.txt

stage2-sprite-stage-cache-68030-ab-package: assets $(STAGE2_SPRITE_STAGE_REFERENCE_TARGET) $(STAGE2_SPRITE_STAGE_CACHE_TARGET)
	@mkdir -p dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_SPRITE_STAGE_REFERENCE_TARGET) dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68030-HD/Sparkpaw-A-Always-Copy
	cp $(RUNTIME_ASSETS) dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_SPRITE_STAGE_REFERENCE_68030_TEST.txt dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_SPRITE_STAGE_CACHE_TARGET) dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68030-HD/Sparkpaw-B-Pose-Cache
	cp $(RUNTIME_ASSETS) dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_SPRITE_STAGE_CACHE_68030_TEST.txt dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68030-HD/ReadMe.txt

stage2-sprite-stage-cache-68020-ab-package: assets $(STAGE2_SPRITE_STAGE_REFERENCE_TARGET) $(STAGE2_SPRITE_STAGE_CACHE_TARGET)
	@mkdir -p dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_SPRITE_STAGE_REFERENCE_TARGET) dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68020-HD/Sparkpaw-A-Always-Copy-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_SPRITE_STAGE_REFERENCE_68020_TEST.txt dist/Stage2-Sprite-Stage-A-Always-Copy-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_SPRITE_STAGE_CACHE_TARGET) dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68020-HD/Sparkpaw-B-Pose-Cache-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_SPRITE_STAGE_CACHE_68020_TEST.txt dist/Stage2-Sprite-Stage-B-Pose-Cache-FS-UAE-68020-HD/ReadMe.txt

stage2-ring-reach-68030-ab-package: assets $(STAGE2_RING_REACH_REFERENCE_TARGET) $(STAGE2_RING_REACH_CANDIDATE_TARGET)
	@mkdir -p dist/Stage2-Ring-Reach-A-All-Copies-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Ring-Reach-B-Fetch-Copies-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_RING_REACH_REFERENCE_TARGET) dist/Stage2-Ring-Reach-A-All-Copies-FS-UAE-68030-HD/Sparkpaw-A-All-Copies
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Reach-A-All-Copies-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_REACH_REFERENCE_68030_TEST.txt dist/Stage2-Ring-Reach-A-All-Copies-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_RING_REACH_CANDIDATE_TARGET) dist/Stage2-Ring-Reach-B-Fetch-Copies-FS-UAE-68030-HD/Sparkpaw-B-Fetch-Copies
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Reach-B-Fetch-Copies-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_REACH_CANDIDATE_68030_TEST.txt dist/Stage2-Ring-Reach-B-Fetch-Copies-FS-UAE-68030-HD/ReadMe.txt

stage2-ring-reach-h3-68030-package: assets $(STAGE2_RING_REACH_CANDIDATE_TARGET)
	@mkdir -p dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_RING_REACH_CANDIDATE_TARGET) dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68030-HD/Sparkpaw-H3-B-Postincrement-68030
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_REACH_H3_68030_TEST.txt dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68030-HD/ReadMe.txt

stage2-copper-patch-68030-ab-package: assets $(STAGE2_COPPER_COPY_REFERENCE_TARGET) $(STAGE2_COPPER_PATCH_TARGET)
	@mkdir -p dist/Stage2-Copper-A-Full-Copy-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Copper-B-Patch-Only-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_COPPER_COPY_REFERENCE_TARGET) dist/Stage2-Copper-A-Full-Copy-FS-UAE-68030-HD/Sparkpaw-A-Full-Copper-Copy
	cp $(RUNTIME_ASSETS) dist/Stage2-Copper-A-Full-Copy-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COPPER_COPY_68030_TEST.txt dist/Stage2-Copper-A-Full-Copy-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_COPPER_PATCH_TARGET) dist/Stage2-Copper-B-Patch-Only-FS-UAE-68030-HD/Sparkpaw-B-Copper-Patch-Only
	cp $(RUNTIME_ASSETS) dist/Stage2-Copper-B-Patch-Only-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COPPER_PATCH_68030_TEST.txt dist/Stage2-Copper-B-Patch-Only-FS-UAE-68030-HD/ReadMe.txt

stage2-copper-patch-68020-ab-package: assets $(STAGE2_COPPER_COPY_REFERENCE_TARGET) $(STAGE2_COPPER_PATCH_TARGET)
	@mkdir -p dist/Stage2-Copper-A-Full-Copy-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Copper-B-Patch-Only-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_COPPER_COPY_REFERENCE_TARGET) dist/Stage2-Copper-A-Full-Copy-FS-UAE-68020-HD/Sparkpaw-A-Full-Copper-Copy-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Copper-A-Full-Copy-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COPPER_COPY_68020_TEST.txt dist/Stage2-Copper-A-Full-Copy-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_COPPER_PATCH_TARGET) dist/Stage2-Copper-B-Patch-Only-FS-UAE-68020-HD/Sparkpaw-B-Copper-Patch-Only-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Copper-B-Patch-Only-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COPPER_PATCH_68020_TEST.txt dist/Stage2-Copper-B-Patch-Only-FS-UAE-68020-HD/ReadMe.txt

stage2-column-blit-68030-ab-package: assets $(STAGE2_COLUMN_CPU_TARGET) $(STAGE2_COLUMN_BLIT_TARGET)
	@mkdir -p dist/Stage2-Column-H3-A-CPU-Words-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Column-H3-B-Split-Blit-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_COLUMN_CPU_TARGET) dist/Stage2-Column-H3-A-CPU-Words-FS-UAE-68030-HD/Sparkpaw-H3-A-CPU-Words
	cp $(RUNTIME_ASSETS) dist/Stage2-Column-H3-A-CPU-Words-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLUMN_CPU_68030_TEST.txt dist/Stage2-Column-H3-A-CPU-Words-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_COLUMN_BLIT_TARGET) dist/Stage2-Column-H3-B-Split-Blit-FS-UAE-68030-HD/Sparkpaw-H3-B-Split-Blit
	cp $(RUNTIME_ASSETS) dist/Stage2-Column-H3-B-Split-Blit-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLUMN_SPLIT_BLIT_68030_TEST.txt dist/Stage2-Column-H3-B-Split-Blit-FS-UAE-68030-HD/ReadMe.txt

stage2-column-layout-probe-package: assets $(STAGE2_COLUMN_BLIT_TARGET)
	@mkdir -p dist/Stage2-Column-H2-Plane-Layout-Probe-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_COLUMN_BLIT_TARGET) dist/Stage2-Column-H2-Plane-Layout-Probe-FS-UAE-68030-HD/Sparkpaw-H2-Plane-Layout-Probe
	cp $(RUNTIME_ASSETS) dist/Stage2-Column-H2-Plane-Layout-Probe-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLUMN_LAYOUT_PROBE.txt dist/Stage2-Column-H2-Plane-Layout-Probe-FS-UAE-68030-HD/ReadMe.txt

stage2-canonical-restore-68030-ab-package: assets $(STAGE2_TARGET_CLEAN_REFERENCE_TARGET) $(STAGE2_CANONICAL_RESTORE_TARGET)
	@mkdir -p dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_TARGET_CLEAN_REFERENCE_TARGET) dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68030-HD/Sparkpaw-A-Clean-Targets
	cp $(RUNTIME_ASSETS) dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_RESTORE_A_68030_TEST.txt dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_CANONICAL_RESTORE_TARGET) dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68030-HD/Sparkpaw-B-Display-Only
	cp $(RUNTIME_ASSETS) dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_RESTORE_B_68030_TEST.txt dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68030-HD/ReadMe.txt

stage2-canonical-restore-68020-ab-package: assets $(STAGE2_TARGET_CLEAN_REFERENCE_TARGET) $(STAGE2_CANONICAL_RESTORE_TARGET)
	@mkdir -p dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_TARGET_CLEAN_REFERENCE_TARGET) dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68020-HD/Sparkpaw-A-Clean-Targets-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_RESTORE_A_68020_TEST.txt dist/Stage2-Canonical-Restore-A-Clean-Targets-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_CANONICAL_RESTORE_TARGET) dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68020-HD/Sparkpaw-B-Display-Only-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_RESTORE_B_68020_TEST.txt dist/Stage2-Canonical-Restore-B-Display-Only-FS-UAE-68020-HD/ReadMe.txt

stage2-canonical-restore-production-68030-package: assets $(STAGE2_CANONICAL_RESTORE_TARGET)
	@mkdir -p dist/Stage2-Canonical-Restore-Production-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_CANONICAL_RESTORE_TARGET) dist/Stage2-Canonical-Restore-Production-FS-UAE-68030-HD/Sparkpaw-Canonical-Restore-Production
	cp $(RUNTIME_ASSETS) dist/Stage2-Canonical-Restore-Production-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_CANONICAL_RESTORE_PRODUCTION_68030_TEST.txt dist/Stage2-Canonical-Restore-Production-FS-UAE-68030-HD/ReadMe.txt

stage2-enemy-copy-on-unload-68030-ab-package: assets $(STAGE2_ENEMY_COPY_REFERENCE_TARGET) $(STAGE2_ENEMY_COPY_ON_UNLOAD_TARGET)
	@mkdir -p dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_ENEMY_COPY_REFERENCE_TARGET) dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68030-HD/Sparkpaw-A-Copy-Every-Tick
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_COPY_A_68030_TEST.txt dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_ENEMY_COPY_ON_UNLOAD_TARGET) dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68030-HD/Sparkpaw-B-Copy-On-Unload
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_COPY_B_68030_TEST.txt dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68030-HD/ReadMe.txt

stage2-enemy-copy-on-unload-68020-ab-package: assets $(STAGE2_ENEMY_COPY_REFERENCE_TARGET) $(STAGE2_ENEMY_COPY_ON_UNLOAD_TARGET)
	@mkdir -p dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_ENEMY_COPY_REFERENCE_TARGET) dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68020-HD/Sparkpaw-A-Copy-Every-Tick-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_COPY_A_68020_TEST.txt dist/Stage2-Enemy-State-A-Copy-Every-Tick-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_ENEMY_COPY_ON_UNLOAD_TARGET) dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68020-HD/Sparkpaw-B-Copy-On-Unload-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_ENEMY_COPY_B_68020_TEST.txt dist/Stage2-Enemy-State-B-Copy-On-Unload-FS-UAE-68020-HD/ReadMe.txt

stage2-hazard-cache-68030-ab-package: assets $(STAGE2_HAZARD_SCAN_REFERENCE_TARGET) $(STAGE2_HAZARD_CACHE_TARGET)
	@mkdir -p dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_HAZARD_SCAN_REFERENCE_TARGET) dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68030-HD/Sparkpaw-A-Boundary-Scan
	cp $(RUNTIME_ASSETS) dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_HAZARD_CACHE_A_68030_TEST.txt dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_HAZARD_CACHE_TARGET) dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68030-HD/Sparkpaw-B-Column-Cache
	cp $(RUNTIME_ASSETS) dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_HAZARD_CACHE_B_68030_TEST.txt dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68030-HD/ReadMe.txt

stage2-hazard-cache-68020-ab-package: assets $(STAGE2_HAZARD_SCAN_REFERENCE_TARGET) $(STAGE2_HAZARD_CACHE_TARGET)
	@mkdir -p dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_HAZARD_SCAN_REFERENCE_TARGET) dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68020-HD/Sparkpaw-A-Boundary-Scan-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_HAZARD_CACHE_A_68020_TEST.txt dist/Stage2-Hazard-A-Boundary-Scan-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_HAZARD_CACHE_TARGET) dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68020-HD/Sparkpaw-B-Column-Cache-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_HAZARD_CACHE_B_68020_TEST.txt dist/Stage2-Hazard-B-Column-Cache-FS-UAE-68020-HD/ReadMe.txt

stage2-phase-start-68030-ab-package: assets $(STAGE2_PHASE_LINE100_TARGET) $(STAGE2_PHASE_IMMEDIATE_TARGET)
	@mkdir -p dist/Stage2-Phase-A-Line100-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Phase-B-Immediate-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_PHASE_LINE100_TARGET) dist/Stage2-Phase-A-Line100-FS-UAE-68030-HD/Sparkpaw-A-Line100
	cp $(RUNTIME_ASSETS) dist/Stage2-Phase-A-Line100-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PHASE_A_68030_TEST.txt dist/Stage2-Phase-A-Line100-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_PHASE_IMMEDIATE_TARGET) dist/Stage2-Phase-B-Immediate-FS-UAE-68030-HD/Sparkpaw-B-Immediate
	cp $(RUNTIME_ASSETS) dist/Stage2-Phase-B-Immediate-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PHASE_B_68030_TEST.txt dist/Stage2-Phase-B-Immediate-FS-UAE-68030-HD/ReadMe.txt

stage2-phase-start-68020-ab-package: assets $(STAGE2_PHASE_LINE100_TARGET) $(STAGE2_PHASE_IMMEDIATE_TARGET)
	@mkdir -p dist/Stage2-Phase-A-Line100-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Phase-B-Immediate-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_PHASE_LINE100_TARGET) dist/Stage2-Phase-A-Line100-FS-UAE-68020-HD/Sparkpaw-A-Line100-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Phase-A-Line100-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PHASE_A_68020_TEST.txt dist/Stage2-Phase-A-Line100-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_PHASE_IMMEDIATE_TARGET) dist/Stage2-Phase-B-Immediate-FS-UAE-68020-HD/Sparkpaw-B-Immediate-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Phase-B-Immediate-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_PHASE_B_68020_TEST.txt dist/Stage2-Phase-B-Immediate-FS-UAE-68020-HD/ReadMe.txt

stage2-collectible-h4-68030-ab-package: assets $(STAGE2_COLLECTIBLE_CANONICAL_TARGET) $(STAGE2_COLLECTIBLE_H4_TARGET)
	@mkdir -p dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_COLLECTIBLE_CANONICAL_TARGET) dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68030-HD/Sparkpaw-H4-A-Canonical
	cp $(RUNTIME_ASSETS) dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLECTIBLE_H4_A_68030_TEST.txt dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_COLLECTIBLE_H4_TARGET) dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68030-HD/Sparkpaw-H4-B-Always-Redraw
	cp $(RUNTIME_ASSETS) dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLECTIBLE_H4_B_68030_TEST.txt dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68030-HD/ReadMe.txt

stage2-collectible-h4-68020-ab-package: assets $(STAGE2_COLLECTIBLE_CANONICAL_TARGET) $(STAGE2_COLLECTIBLE_H4_TARGET)
	@mkdir -p dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_COLLECTIBLE_CANONICAL_TARGET) dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68020-HD/Sparkpaw-H4-A-Canonical-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLECTIBLE_H4_A_68020_TEST.txt dist/Stage2-Collectible-H4-A-Canonical-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_COLLECTIBLE_H4_TARGET) dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68020-HD/Sparkpaw-H4-B-Always-Redraw-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_COLLECTIBLE_H4_B_68020_TEST.txt dist/Stage2-Collectible-H4-B-Always-Redraw-FS-UAE-68020-HD/ReadMe.txt

stage2-ring-reach-68020-ab-package: assets $(STAGE2_RING_REACH_REFERENCE_TARGET) $(STAGE2_RING_REACH_CANDIDATE_TARGET)
	@mkdir -p dist/Stage2-Ring-Reach-H3-A-All-Copies-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_RING_REACH_REFERENCE_TARGET) dist/Stage2-Ring-Reach-H3-A-All-Copies-FS-UAE-68020-HD/Sparkpaw-H3-A-All-Copies-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Reach-H3-A-All-Copies-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_REACH_REFERENCE_68020_TEST.txt dist/Stage2-Ring-Reach-H3-A-All-Copies-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_RING_REACH_CANDIDATE_TARGET) dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68020-HD/Sparkpaw-H3-B-Postincrement-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_RING_REACH_H3_68020_TEST.txt dist/Stage2-Ring-Reach-H3-B-Postincrement-FS-UAE-68020-HD/ReadMe.txt

stage2-dynamic-word-copy-ab-package: assets $(STAGE2_DYNAMIC_REFERENCE_TARGET) $(STAGE2_DYNAMIC_WORD_COPY_TARGET)
	@mkdir -p dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68030-HD/assets/runtime
	@mkdir -p dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68030-HD/assets/runtime
	cp $(STAGE2_DYNAMIC_REFERENCE_TARGET) dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68030-HD/Sparkpaw-A-Reference
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_REFERENCE_TEST.txt dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68030-HD/ReadMe.txt
	cp $(STAGE2_DYNAMIC_WORD_COPY_TARGET) dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68030-HD/Sparkpaw-B-Word-Copy
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68030-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_WORD_COPY_TEST.txt dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68030-HD/ReadMe.txt

stage2-dynamic-word-copy-68020-ab-package: assets $(STAGE2_DYNAMIC_REFERENCE_TARGET) $(STAGE2_DYNAMIC_WORD_COPY_TARGET)
	@mkdir -p dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68020-HD/assets/runtime
	@mkdir -p dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68020-HD/assets/runtime
	cp $(STAGE2_DYNAMIC_REFERENCE_TARGET) dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68020-HD/Sparkpaw-A-Reference-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_REFERENCE_68020_TEST.txt dist/Stage2-Dynamic-Sync-A-Reference-FS-UAE-68020-HD/ReadMe.txt
	cp $(STAGE2_DYNAMIC_WORD_COPY_TARGET) dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68020-HD/Sparkpaw-B-Word-Copy-68020
	cp $(RUNTIME_ASSETS) dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68020-HD/assets/runtime/
	cp docs/PERFORMANCE_68020_STAGE2_DYNAMIC_WORD_COPY_68020_TEST.txt dist/Stage2-Dynamic-Sync-B-Word-Copy-FS-UAE-68020-HD/ReadMe.txt

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

$(HUD_SEAM_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA32_LEFT_GUARD \
		-DSPARKPAW_AGA64_PLAYER_SPRITE \
		-DSPARKPAW_HUD_SEAM_FMODE_FIRST -o $@ $(SOURCES)

$(HUD_SEAM_H2_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA32_LEFT_GUARD \
		-DSPARKPAW_AGA64_PLAYER_SPRITE \
		-DSPARKPAW_HUD_SEAM_FMODE_FIRST \
		-DSPARKPAW_HUD_SEAM_KEEP_DDFSTOP -o $@ $(SOURCES)

$(HUD_SEAM_H3_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA32_LEFT_GUARD \
		-DSPARKPAW_AGA64_PLAYER_SPRITE \
		-DSPARKPAW_HUD_SEAM_FMODE_FIRST \
		-DSPARKPAW_HUD_SEAM_KEEP_DDFSTOP \
		-DSPARKPAW_HUD_NATIVE_3PLANE -o $@ $(SOURCES)

$(HUD_SEAM_H4_DIAG_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_BLITTER_PRIORITY_CANDIDATE \
		-DSPARKPAW_AGA32_FETCH_CANDIDATE \
		-DSPARKPAW_AGA32_ORIGIN_CORRECTED \
		-DSPARKPAW_AGA32_LEFT_GUARD \
		-DSPARKPAW_AGA64_PLAYER_SPRITE \
		-DSPARKPAW_HUD_SEAM_FMODE_FIRST \
		-DSPARKPAW_HUD_SEAM_KEEP_DDFSTOP \
		-DSPARKPAW_HUD_NATIVE_3PLANE \
		-DSPARKPAW_HUD_RIGHT_FETCH_GUARD -o $@ $(SOURCES)

$(HUD_SEAM_H5_SPRITE_ISOLATION_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC $(STAGE5L_RENDERER_FLAGS) \
		-DSPARKPAW_HUD_SEAM_ISOLATE_SPRITES -o $@ $(SOURCES)

$(HUD_SEAM_H5_BITPLANE_ISOLATION_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC $(STAGE5L_RENDERER_FLAGS) \
		-DSPARKPAW_HUD_SEAM_ISOLATE_BITPLANES -o $@ $(SOURCES)

$(HUD_SEAM_H6_FRONT_ISOLATION_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC $(STAGE5L_RENDERER_FLAGS) \
		-DSPARKPAW_HUD_SEAM_ISOLATE_FRONT_PALETTE -o $@ $(SOURCES)

$(HUD_SEAM_H6_REAR_ISOLATION_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC $(STAGE5L_RENDERER_FLAGS) \
		-DSPARKPAW_HUD_SEAM_ISOLATE_REAR_PALETTE -o $@ $(SOURCES)

$(HUD_SEAM_H7_PRODUCTION_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_PROFILE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC $(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(STAGE2_MINIMAL_CADENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC \
		-DSPARKPAW_DISABLE_WAIT_PROFILE $(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(SCORE_CAMERA_CENTER_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC \
		-DSPARKPAW_DISABLE_WAIT_PROFILE $(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(SCORE_CAMERA_DEADZONE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC \
		-DSPARKPAW_DISABLE_WAIT_PROFILE \
		-DSPARKPAW_CAMERA_DEADZONE_REFERENCE $(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(SCORE_PROJECTILE_PIXEL_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_PROJECTILE_PIXEL_SWEEP_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(SCORE_PROJECTILE_COALESCED_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC $(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(STAGE2_DYNAMIC_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_DYNAMIC_RING_COPYMEM_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) \
		-o $@ $(SOURCES)

$(STAGE2_DYNAMIC_WORD_COPY_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_DYNAMIC_RING_INDEXED_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_DYNAMIC_UNROLLED_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_CANONICAL_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_CANONICAL_RING_COPYMEM_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_CANONICAL_WORD_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COLLISION_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_COLLISION_PIXEL_SPAN_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COLLISION_TILE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_ENEMY_CACHE_WORD_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_PLAYER_SPRITE_WORD_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_SPRITE_STAGE_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_SPRITE_STAGE_ALWAYS_COPY_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_SPRITE_STAGE_CACHE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_RING_REACH_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_RING_REACH_CANDIDATE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_FETCH_RELEVANT_RING_COPIES \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COPPER_COPY_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_COPPER_FULL_COPY_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COPPER_PATCH_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COLUMN_CPU_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COLUMN_BLIT_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_CANONICAL_MULTIPLANE_BLIT \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_CANONICAL_RESTORE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_TARGET_CLEAN_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_TARGET_CLEAN_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_ENEMY_COPY_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_ENEMY_COPY_EVERY_TICK_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_ENEMY_COPY_ON_UNLOAD_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_ENEMY_TRAVERSAL_LINEAR_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_ENEMY_TRAVERSAL_LINEAR_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_BOB_PER_PLANE_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_BOB_PER_PLANE_SETUP_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_RING_COLUMN_GENERIC_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_RING_COLUMN_GENERIC_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_HAZARD_SCAN_REFERENCE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC -DSPARKPAW_COLLISION_HAZARD_SCAN_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_HAZARD_CACHE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_PHASE_LINE100_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_UPDATE_LINE100_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_PHASE_IMMEDIATE_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COLLECTIBLE_CANONICAL_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DSPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(STAGE2_COLLECTIBLE_H4_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		$(RELEASE_RENDERER_FLAGS) -o $@ $(SOURCES)

$(RENDER_DIAG_ADF_TARGET): Makefile $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)" "$(dir $@)"
	$(CC) $(CFLAGS) -DADF_PACKED_ASSETS -DSPARKPAW_ROLLING_PROTOTYPE \
		-DSPARKPAW_RENDER_DIAGNOSTIC \
		-DDIAGNOSTIC_TRACE_FRAMES=512 \
		-o $@ $(SOURCES)

clean:
	$(RM) $(TARGET) $(BENCH_TARGETS) sparkpaw-renderbench src/*.o
