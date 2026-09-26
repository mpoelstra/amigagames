#!/usr/bin/env python3
"""Generate release data from the accepted v5 frames; preserve test builds."""
import build_level1_rear_ambience_v5 as source
from pathlib import Path
import shutil
source.O=source.R/'build/level1-rear-release-assets'
source.main()
shutil.copy2(source.O/'assets/l1-electric.bin',source.R/'assets/runtime/l1-electric.bin')
shutil.copy2(source.O/'level1_rear_ambience_data.h',source.R/'src/level1_rear_ambience_release_data.h')
