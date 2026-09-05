#!/usr/bin/env python3
"""Read-only campaign disk budget and original alpha.68 FFS layout audit.

SPR1 sizes are offline storage estimates, not a working campaign disk loader.
No dist artifact is written. Data-block cylinder travel is not DOS seek timing.
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / '.toolchain/amitools'))
from campaign_asset_manifest import GROUPS, SHARED_PRESENTATION, SHARED_GAMEPLAY, LEVEL1, STORMRAIL
from pack_adf_asset import pack, decode
from report_adf_storage import ffs_file_blocks
from amitools.fs.blkdev.BlkDevFactory import BlkDevFactory
from amitools.fs.ADFSVolume import ADFSVolume


def main():
    output = ROOT / 'build/campaign-media-report'
    output.mkdir(parents=True, exist_ok=True)
    rows = {}
    for group, names in GROUPS.items():
        for name in sorted(names):
            raw = (ROOT / 'assets/runtime' / name).read_bytes()
            packed = pack(raw) if name.endswith('.spbm') else raw
            if name.endswith('.spbm'):
                assert decode(packed) == raw
            rows[name] = dict(group=group, raw_bytes=len(raw),
                              estimated_disk_bytes=len(packed),
                              estimated_ffs_blocks=ffs_file_blocks(len(packed)))
    intro = {n for n in SHARED_PRESENTATION if n.startswith('intro')}
    common = SHARED_PRESENTATION - intro | SHARED_GAMEPLAY
    # Current shared audio loads Harrier samples even for Level 1.
    audio_extra = {n for n in STORMRAIL if n.endswith('.raw')}
    # Current Stormrail preparation also loads Level-1 common conversions/audio.
    resident_legacy = LEVEL1 - {'storm-front.spbm', 'storm-rear.spbm',
                               'sparkpaw-sprites4.spbm'}
    sets = {'boot_intro_only': SHARED_PRESENTATION,
            'level1_without_story': common | LEVEL1 | audio_extra,
            'stormrail_without_story_current_loads': common | STORMRAIL | resident_legacy,
            'whole_campaign_with_story': set(rows)}
    budgets = {}
    exe_bytes = (ROOT / 'build/sparkpaw-campaign-play').stat().st_size
    for name, files in sets.items():
        asset_blocks = sum(rows[n]['estimated_ffs_blocks'] for n in files)
        # Includes executable, boot/root/bitmap/three directories. Not future
        # loader growth, extra metadata/disk labels, ReadMe or guard headroom.
        total = asset_blocks + ffs_file_blocks(exe_bytes) + 7
        budgets[name] = dict(files=sorted(files), asset_blocks=asset_blocks,
                             with_current_executable_and_minimum_fs_blocks=total,
                             capacity_blocks=1760, remaining_blocks=1760-total)
    image = ROOT / 'dist/Sparkpaw-0.6.0-alpha.68.adf'
    dev = BlkDevFactory().open(str(image), read_only=True)
    volume = ADFSVolume(dev)
    volume.open()
    layout = []
    def walk(node):
        for child in node.get_entries():
            if child.is_dir():
                walk(child)
            else:
                blocks = child.data_blk_nums
                runs = sum(i == 0 or b != blocks[i-1]+1 for i, b in enumerate(blocks))
                cylinders = [b//22 for b in blocks]
                layout.append(dict(path=str(child.get_node_path_name()),
                                   bytes=child.get_size(), header=child.block.blk_num,
                                   extensions=child.ext_blk_nums, data_blocks=blocks,
                                   data_runs=runs, min_cylinder=min(cylinders, default=0),
                                   max_cylinder=max(cylinders, default=0)))
    walk(volume.get_root_dir())
    result = dict(budget_limitations='SPR1 bitmap estimates; raw other files; not runtime support or native timing',
                  groups=rows, budgets=budgets,
                  alpha68=dict(used_blocks=volume.get_used_blocks(), free_blocks=volume.get_free_blocks(),
                               files=layout))
    dev.close()
    (output/'report.json').write_text(json.dumps(result, indent=2))
    print(json.dumps({'budgets':{n:{k:v for k,v in b.items() if k!='files'} for n,b in budgets.items()},
                      'alpha68_files':len(layout), 'alpha68_fragmented_data_files':sum(f['data_runs']>1 for f in layout)},indent=2))


if __name__ == '__main__':
    main()
