#!/usr/bin/env python3
"""Build the versioned Sparkpaw WHDLoad packages."""
from __future__ import annotations

import shutil
import struct
import subprocess
import sys
import time
import zipfile
from pathlib import Path

from PIL import Image
from make_release import RELEASE_NAME, RELEASE_VERSION, ROADMAP_CHECKPOINT, RUNTIME_FILES

ROOT = Path(__file__).resolve().parents[1]
DEV = ROOT / ".toolchain" / "whdload-dev" / "WHDLoad"
NDK_INCLUDE = ROOT / ".toolchain" / "ndk" / "Include_I"
AMIGAINFO = ROOT / ".toolchain" / "amigainfo"
VASM = ROOT / ".toolchain" / "sdk" / "bin" / "vasmm68k_mot"
SLAVE_SOURCE = ROOT / "whdload" / "Sparkpaw.asm"
SLAVE = ROOT / "whdload" / "Sparkpaw.Slave"
STAGE_ROOT = ROOT / "build" / "whdload"
STAGE_NAME = f"{RELEASE_NAME}-WHDLoad"
STAGE = STAGE_ROOT / STAGE_NAME
DIST = ROOT / "dist"


def require(path: Path, description: str) -> None:
    if not path.is_file():
        raise SystemExit(f"missing {description}: {path}")


def assemble() -> None:
    require(VASM, "vasm assembler")
    require(DEV / "Include" / "whdload.i", "WHDLoad include files")
    require(NDK_INCLUDE / "exec" / "execbase.i", "NDK assembler includes")
    require(DEV / "Src" / "sources" / "whdload" / "kick31.s", "WHDLoad kick31 source")
    subprocess.run([
        str(VASM), "-m68000", "-Fhunkexe", "-nosym", "-quiet", "-nowarn=62",
        f"-I{ROOT / 'whdload' / 'include'}", f"-I{DEV / 'Include'}",
        f"-I{NDK_INCLUDE}", f"-I{DEV / 'Src' / 'sources'}",
        str(SLAVE_SOURCE), "-o", str(SLAVE),
    ], cwd=ROOT, check=True)


def load_amigainfo_api():
    if str(AMIGAINFO) not in sys.path:
        sys.path.insert(0, str(AMIGAINFO))
    from amigainfo import load, save
    from amigainfo.models import IconType
    return load, save, IconType


def icon_pen(rgb: tuple[int, int, int]) -> int:
    red, green, blue = rgb
    brightness = (red * 30 + green * 59 + blue * 11) // 100
    if brightness < 38:
        return 1
    if brightness > 205 and abs(red - green) < 65:
        return 2
    if blue > red and blue > green:
        return 3
    return 0


def base_icon_bytes() -> bytes:
    source = ROOT / "assets" / "concept" / "sparkpaw-title-concept-aga64-preview.png"
    require(source, "Sparkpaw icon source")
    with Image.open(source).convert("RGB") as image:
        side = min(image.size)
        left = (image.width - side) // 2
        top = (image.height - side) // 2
        resized = image.crop((left, top, left + side, top + side)).resize(
            (48, 48), Image.Resampling.LANCZOS
        )
        pixels = resized.load()
        rows = [pixels[x, y] for y in range(48) for x in range(48)]
    planar = bytearray()
    for plane in range(2):
        for y in range(48):
            for word_index in range(3):
                word = 0
                for bit in range(16):
                    if icon_pen(rows[y * 48 + word_index * 16 + bit]) & (1 << plane):
                        word |= 1 << (15 - bit)
                planar += struct.pack(">H", word)
    gadget = struct.pack(
        ">IhhhhHHHIIIiIHI", 0, 0, 0, 48, 48, 0x0004, 0x0001, 0x0001,
        1, 0, 0, 0, 0, 0, 1,
    )
    disk_object = (struct.pack(">HH", 0xE310, 1) + gadget +
                   struct.pack(">BBIIiiIIi", 3, 0, 0, 0, -1, -1, 0, 0, 65536))
    image_header = struct.pack(">hhhhhIBBI", 0, 0, 48, 48, 2, 1, 0x03, 0, 0)
    return disk_object + image_header + planar


def make_icons() -> None:
    load, save, IconType = load_amigainfo_api()
    icon = load(base_icon_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = "WHDLoad"
    icon.tooltypes = ["SLAVE=Sparkpaw.Slave", "PRELOAD", "PAL"]
    (STAGE / "Sparkpaw.info").write_bytes(save(icon))
    readme_icon = load(base_icon_bytes())
    readme_icon.type = IconType.PROJECT
    readme_icon.default_tool = "MultiView"
    readme_icon.tooltypes = []
    (STAGE / "ReadMe.txt.info").write_bytes(save(readme_icon))


def crc16(data: bytes) -> int:
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc & 0xFFFF


def dos_time(timestamp: float) -> int:
    value = time.localtime(timestamp)
    return ((max(1980, min(2107, value.tm_year)) - 1980) << 25 |
            value.tm_mon << 21 | value.tm_mday << 16 | value.tm_hour << 11 |
            value.tm_min << 5 | value.tm_sec // 2)


def lha_member(name: str, data: bytes, timestamp: float) -> bytes:
    encoded = name.encode("latin-1")
    body = (b"-lh0-" + struct.pack("<IIIBBB", len(data), len(data),
            dos_time(timestamp), 0x20, 0, len(encoded)) + encoded +
            struct.pack("<H", crc16(data)))
    return bytes((len(body), sum(body) & 255)) + body + data


def make_lha() -> Path:
    output = DIST / f"{STAGE_NAME}.lha"
    with output.open("wb") as archive:
        for path in sorted(STAGE.rglob("*")):
            if path.is_file():
                relative = path.relative_to(STAGE_ROOT).as_posix()
                archive.write(lha_member(relative, path.read_bytes(), path.stat().st_mtime))
        archive.write(b"\0")
    return output


def main() -> None:
    assemble()
    if STAGE_ROOT.exists():
        shutil.rmtree(STAGE_ROOT)
    data = STAGE / "data"
    (data / "assets" / "runtime").mkdir(parents=True)
    shutil.copy2(SLAVE, STAGE / "Sparkpaw.Slave")
    shutil.copy2(ROOT / "sparkpaw", data / "Sparkpaw")
    for name in RUNTIME_FILES:
        source = ROOT / "assets" / "runtime" / name
        require(source, f"runtime asset {name}")
        shutil.copy2(source, data / "assets" / "runtime" / name)
    readme = (ROOT / "whdload" / "ReadMe.txt").read_text(encoding="ascii").format(
        RELEASE_NAME=RELEASE_NAME, RELEASE_VERSION=RELEASE_VERSION,
        ROADMAP_CHECKPOINT=ROADMAP_CHECKPOINT,
    )
    (STAGE / "ReadMe.txt").write_text(readme, encoding="ascii")
    make_icons()
    DIST.mkdir(exist_ok=True)
    zip_path = Path(shutil.make_archive(str(DIST / STAGE_NAME), "zip", STAGE_ROOT, STAGE_NAME))
    lha_path = make_lha()
    with zipfile.ZipFile(zip_path) as archive:
        required = {
            f"{STAGE_NAME}/Sparkpaw.Slave", f"{STAGE_NAME}/Sparkpaw.info",
            f"{STAGE_NAME}/ReadMe.txt", f"{STAGE_NAME}/data/Sparkpaw",
        }
        missing = required.difference(archive.namelist())
        if missing:
            raise SystemExit(f"WHDLoad ZIP verification failed: {sorted(missing)}")
    print("Wrote", zip_path)
    print("Wrote", lha_path)
    print("Prepared", STAGE)


if __name__ == "__main__":
    main()
