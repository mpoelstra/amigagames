# Storm Light — game over

User-selected on 10 September 2026 from `/Users/mpoelstra/Downloads/AMIGAMUSIC/StormLight.mod`. The user identifies this as a previously created track. The checked-in MOD is a byte-identical copy; see manifest.json for its SHA-256.

One unchanged master serves HD, WHDLoad and ADF. Build with `make assets/runtime/storm-light.lsmusic`. Existing LSPConvert uses `-shrink -fixed50hz`: 202,432 bytes of samples and 5,487 bytes of score, approximately 62 seconds at 125 BPM. Shrinking removes unreachable sample tails, without resampling or instrument removal. Native playback still requires user acceptance.

The two rejected After the Storm experiments have been retained under build/game-over-work for local reference and are no longer part of the runtime manifest or build.
