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
SOURCES = src/sparkpaw.c src/assets.c src/audio.c src/collision.c src/enemies.c \
	src/game.c src/player.c src/projectiles.c
HEADERS = src/assets.h src/audio.h src/collision.h src/enemies.h src/game.h \
	src/player.h src/projectiles.h
RUNTIME_ASSETS = assets/runtime/storm-world.spbm \
	assets/runtime/storm-front.spbm \
	assets/runtime/storm-rear.spbm \
	assets/runtime/storm-collision.bin \
	assets/runtime/energy-shot.raw \
	assets/runtime/clockwork-beetle.spbm \
	assets/runtime/sparkpaw-sprites.spbm \
	assets/runtime/sparkpaw-sprites3.spbm \
	assets/runtime/sparkpaw-sprites4.spbm

.PHONY: all clean assets release bench

all: assets $(TARGET)

bench: $(BENCH_TARGET)

$(BENCH_TARGET): src/aga_renderbench.c
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -o $@ $<

assets: $(RUNTIME_ASSETS)

$(RUNTIME_ASSETS): tools/generate_runtime_assets.py \
	assets/sprites/sparkpaw-sprites-reference-transparent.png \
	assets/sprites/sparkpaw-supplemental-poses-v2-transparent.png \
	assets/sprites/sparkpaw-run-compact-v3-transparent.png \
	assets/sprites/sparkpaw-jump-refinement-v5-transparent.png \
	assets/sprites/sparkpaw-motion-refinement-v3-transparent.png \
	assets/sprites/sparkpaw-turn-refinement-v4-transparent.png \
	assets/sprites/sparkpaw-idle-refinement-v3-transparent.png \
	assets/sprites/sparkpaw-combat-refinement-v7-transparent.png \
	assets/sprites/sparkpaw-air-fire-v8-transparent.png \
	assets/sprites/sparkpaw-crouch-fire-v9-transparent.png \
	assets/enemies/clockwork-beetle-animations.json \
	sfx/raw/energy-shot.raw
	$(PYTHON) tools/generate_runtime_assets.py

sfx/raw/energy-shot.raw: tools/generate_sparkpaw_sfx.py
	$(PYTHON) tools/generate_sparkpaw_sfx.py

$(TARGET): $(SOURCES) $(HEADERS) $(RUNTIME_ASSETS)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

release: all
	$(PYTHON) tools/make_release.py

clean:
	$(RM) $(TARGET) $(BENCH_TARGET) src/*.o
