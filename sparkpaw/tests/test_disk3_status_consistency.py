"""Pin accepted 1/2 art and verify all three font-sheet ADF status strips."""
from pathlib import Path
import hashlib
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from generate_disk_status import make_patch, read_spbm
from generate_ready_screen import spbm_payload

STATUS = ROOT / 'build/multidisk-probe/status'
EXPECTED = {
    1: '4f5f8df0da68e02412f315093d57d14746a77791bf99b658d6734424542044fd',
    2: 'c7bacacd27513d0767d627d4bb02064d47ca54d2b08672093e60bc25805dd543',
}
SOURCE3 = ROOT / 'assets/concept/sparkpaw-insert-disk-type-v2-disk3.png'
assert hashlib.sha256(SOURCE3.read_bytes()).hexdigest() == (
    '846d5e5959eb75878fc3223e52d5a4fbc54297f756df1be5216dd9041de260b2')

patches = {}
for number in (1, 2, 3):
    path = STATUS / f'disk{number}-patch.spbm'
    data = path.read_bytes()
    if number in EXPECTED:
        assert hashlib.sha256(data).hexdigest() == EXPECTED[number], (
            f'Approved INSERT DISK {number} art changed; review the whole set')
    image = read_spbm(path)
    assert image.size == (224, 40) and image.mode == 'P'
    patches[number] = image

palette = patches[1].getpalette()
assert patches[2].getpalette() == palette
assert patches[3].getpalette() == palette
assert (STATUS / 'disk3-patch.spbm').read_bytes() == spbm_payload(
    make_patch(3, palette), 224, 40)
for number, image in patches.items():
    # Every complete text line has the same 216x24 conversion and 4x6 inset.
    # A separately pasted oversized digit must not escape that band.
    assert all(image.getpixel((x, y)) == 0
               for y in range(40) for x in range(224)
               if not (4 <= x < 220 and 6 <= y < 30)), number

saved = ROOT / 'build/campaign-drowned/adf/status/disk3-patch.spbm'
if saved.exists():
    assert saved.read_bytes() == (STATUS / 'disk3-patch.spbm').read_bytes()
print('INSERT DISK 1/2/3 source, scale, palette and package contract: PASS')
