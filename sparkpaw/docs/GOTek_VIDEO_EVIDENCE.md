# Gotek loading video evidence — 2026-09-05

Alpha.68 Gotek loading / repeated track changes
================================================

Original recording/image: sparkpaw/testresults/gotek-loading.mp4
Renamed evidence: Phase 6C.10-observed-gotek-loading.mp4
Date supplied/reviewed: 2026-09-05
Evidence source: user-supplied real-Amiga/Gotek video; report from user's brother
Build/version: OLED scrolling image label visibly reads Sparkpaw-0.6.0-alpha.68;
actual mounted ADF hash, CPU, firmware and drive settings not supplied.
Launch medium: ADF through Gotek. This is not the new 0.7 HD release.
SHA256: 6171c2a2bd7283a051284174fd825d2defd4509668012b6c1a311b0d2908e0c9
Original bytes preserved by rename. No video transcode.

Media metadata
--------------
Duration 83.261667 s; H.264 Baseline, 464x832, 2498 frames, approximately 30 fps
(variable average 1498800/49957); AAC stereo, 44.1 kHz; 17392575 bytes.

What to inspect
---------------
The OLED T: readout and disk activity during loading. User quote:
"Zag ook op mijn oled display bij laden dat hij steed aan begin van track van
disk zat, dan weer einde track en steeds heen en weer. Kan dat ook nog efficienter?"
Reviewed overview, explicit timestamp frames, a four-samples/second interval
at 49-57 seconds and 32 consecutive source frames starting near 49.7 seconds.
Derived PNGs remain under build/gotek-video-audit, not replacement evidence.

Observed result
---------------
OLED track readout changes widely in both directions and repeatedly revisits
39/40 in the reviewed interval. Explicit samples include approximately:
20s T:20.1; 25s T:24.0; 30s T:01.0; 35s T:07.0; 40s T:39.0;
45s T:54.0; 50s T:39.0; 55s T:76.1; 60s T:33.0; 75s T:40.0.
In consecutive frames near 49.7-50.8s, it dwells at 40.0, then 39.0,
steps through intermediate values toward 75.1 and subsequently 76.0.
These are samples of displayed positions, not a complete DOS read trace;
intermediate track numbers may be seek transit rather than file reads.
At the end the camera shows the Sparkpaw ready menu with START GAME/OPTIONS.
Capture begins during activity, without a visible power-on/start marker.
83.26 seconds must NOT be reported as total cold boot or complete loading time.
The clip does not establish complete gameplay, audio health or new-alpha acceptance.

Analysis or follow-up
---------------------
Original protected alpha.68 FFS image offline map has root block 880 (cylinder
40), S/assets/assets-runtime directory blocks 866/867/868 (cylinder 39).
Repeated visible 39/40 visits are consistent with filesystem metadata access;
this is a stronger hypothesis than 'all files fragmented', but not proof of
which block is requested. Exact filmed ADF parity is unverified.
The same local image places LOADING on cylinders77-78, front19-22, rear22-29,
player0-13 and Strider50-58. All of those files have contiguous data runs.
Only score glyphs and ReadMe have two data runs in the 35-file local image.

Next layout proof must account for metadata, directory lookup/cache and file
header reads in addition to payload placement. Do not assume alphabetical
renaming alone solves it. A source-derived consumption-order ADF with unchanged
file bytes, plus cold-load timing on the same device/configuration, remains
the first controlled comparison. If metadata traffic persists, evaluate a
bounded filesystem-cache change separately before a new container/trackloader.
No binary, game code, ADF or Gotek settings changed during this review.

Acceptance
----------
Observation confirmed: visible wide track-position changes and ready menu
reached during this clip. Loading efficiency cause and improvement remain
unmeasured. No FPS, new release, complete-run or platform acceptance claim.

Related plans: [ADF loading](ADF_LOAD_OPTIMIZATION_PLAN.md),
[campaign multidisk](MULTI_ADF_CAMPAIGN_PLAN.md).
