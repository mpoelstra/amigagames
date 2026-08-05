VBCC_ROOT := $(CURDIR)/.toolchain/sdk
NDK_INCLUDE := $(CURDIR)/.toolchain/ndk/Include_H
export VBCC := $(VBCC_ROOT)
export PATH := $(VBCC_ROOT)/bin:$(PATH)
export TMPDIR := $(CURDIR)/.toolchain/tmp

CC = $(VBCC_ROOT)/bin/vc
PYTHON ?= python3
CFLAGS ?= +aos68k -O2 -I$(NDK_INCLUDE)

TARGET = snake
SOURCES = src/snake.c src/modplayer.c
LSP_OBJECT = src/lspplayer.o
LSP_CONVERT = third_party/LSPlayer/build/LSPConvert
MOD_FILES = music/chipsnake-title.mod music/chipsnake-circuit.mod \
	music/chipsnake-turbo.mod music/chipsnake-thunder.mod \
	music/chipsnake-neon.mod music/chipsnake-hall.mod
LSP_FILES = $(MOD_FILES:.mod=.lsmusic) $(MOD_FILES:.mod=.lsbank)
PROTRACKER_STAMP = music/.protracker-compatible

.PHONY: all clean release whdload

all: $(TARGET)

release: all whdload
	$(PYTHON) tools/make_release.py

whdload: all
	$(PYTHON) tools/make_whdload.py

$(TARGET): $(SOURCES) $(LSP_OBJECT) $(LSP_FILES)
	@test -x "$(CC)" || \
		{ echo "VBCC is missing. See README.md for toolchain setup."; exit 1; }
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LSP_OBJECT)

$(LSP_OBJECT): src/lspplayer.s third_party/LSPlayer/LightSpeedPlayer.asm
	$(VBCC_ROOT)/bin/vasmm68k_mot -Fhunk -quiet -nowarn=62 \
	  $< -o $@

$(PROTRACKER_STAMP): $(MOD_FILES) tools/fix_protracker_compat.py
	$(PYTHON) tools/fix_protracker_compat.py
	@touch $@

music/%.lsmusic: music/%.mod $(PROTRACKER_STAMP) $(LSP_CONVERT)
	$(LSP_CONVERT) $< -shrink -fixed50hz

music/%.lsbank: music/%.mod $(PROTRACKER_STAMP) $(LSP_CONVERT)
	$(LSP_CONVERT) $< -shrink -fixed50hz

$(LSP_CONVERT): third_party/LSPlayer/CMakeLists.txt
	cmake -S third_party/LSPlayer -B third_party/LSPlayer/build \
	  -DCMAKE_BUILD_TYPE=Release
	cmake --build third_party/LSPlayer/build --config Release

clean:
	$(RM) $(TARGET) src/*.o
