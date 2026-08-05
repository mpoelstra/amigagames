VBCC_ROOT := $(CURDIR)/.toolchain/sdk
NDK_INCLUDE := $(CURDIR)/.toolchain/ndk/Include_H
export VBCC := $(VBCC_ROOT)
export PATH := $(VBCC_ROOT)/bin:$(PATH)
export TMPDIR := $(CURDIR)/build/tmp

CC = $(VBCC_ROOT)/bin/vc
PYTHON ?= python3
CFLAGS ?= +aos68k -O2 -I$(NDK_INCLUDE)
TARGET = futsal
SOURCES = src/futsal.c src/modplayer.c src/sfxplayer.c
LSP_OBJECT = src/lspplayer.o
LSP_CONVERT = third_party/LSPlayer/build/LSPConvert
MUSIC_FILES = music/mrdigs-futsal-title.lsmusic \
	music/mrdigs-futsal-title.lsbank \
	music/mrdigs-futsal-ingame.lsmusic \
	music/mrdigs-futsal-ingame.lsbank

.PHONY: all clean assets release whdload

all: assets $(TARGET)

release: all whdload
	$(PYTHON) tools/make_release.py

whdload: all
	$(PYTHON) tools/make_whdload.py

assets: assets/title/mrdigs-futsal-title.iff \
	assets/options/mrdigs-futsal-options.iff \
	sfx/futsal-sfx.bank futsal.info \
	$(MUSIC_FILES)

assets/title/mrdigs-futsal-title.iff: assets/concepts/mrdigs-futsal-title-concept.png tools/png_to_title_ilbm.py
	$(PYTHON) tools/png_to_title_ilbm.py $< $@

assets/options/mrdigs-futsal-options.iff: assets/concepts/mrdigs-futsal-options-concept.png tools/png_to_options_ilbm.py
	$(PYTHON) tools/png_to_options_ilbm.py $< $@

sfx/futsal-sfx.bank: tools/generate_futsal_sfx.py
	$(PYTHON) tools/generate_futsal_sfx.py

build/icon/mrdigs-futsal-icon-48.bmp: tools/generate_futsal_icon.py
	$(PYTHON) tools/generate_futsal_icon.py

futsal.info: build/icon/mrdigs-futsal-icon-48.bmp tools/make_workbench_icon.py
	$(PYTHON) tools/make_workbench_icon.py $< $@

music/mrdigs-futsal-title.mod: tools/generate_futsal_title_mod.py \
	tools/generate_futsal_title_candidates.py
	$(PYTHON) $<

music/mrdigs-futsal-ingame.mod: tools/generate_futsal_ingame_mod.py
	$(PYTHON) $<

music/%.lsmusic music/%.lsbank: music/%.mod $(LSP_CONVERT)
	$(LSP_CONVERT) $< -shrink -fixed50hz

$(TARGET): $(SOURCES) $(LSP_OBJECT)
	@mkdir -p "$(TMPDIR)"
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LSP_OBJECT)

$(LSP_OBJECT): src/lspplayer.s third_party/LSPlayer/LightSpeedPlayer.asm
	$(VBCC_ROOT)/bin/vasmm68k_mot -Fhunk -quiet -nowarn=62 $< -o $@

clean:
	$(RM) $(TARGET) src/*.o
