COPPER SPRINT - BLOCK MIXER TIMING GATE

Open Audio-Level1 from this drawer in Amiga Shell.
First gate: FS-UAE PAL A1200 / 68030 / 2 MB Chip + 8 MB Fast.
Same 94-second audition: first 47 seconds music, then all effects and
scripted overlaps. Allow automatic completion. LMB can stop early.
Returns to Shell/Workbench and saves renderdiag.log in this drawer.

Play the measured and control versions on the same configuration.
Report any audible difference, crackles, slowdown or missing effects.
Keep each log in its own drawer; no copying/renaming is needed.
Only after 030 listening and log review: the same pair on 020.

Music, samples, mixing, gains and scripted foreground stalls are identical.
This is still audio-only: gameplay interrupt ownership is a later gate.
No production gameplay, intro/title or release assets have been changed.

MEASURED: ReadEClock times mixer, music and DMA timer service bodies.
The timer is read via AmigaOS, never reprogrammed. Nested measured music
is excluded from mixer exclusive time. Raw clock calibration is logged.
Service totals include instrumentation and other OS preemption; they
exclude Exec dispatch and outer saves. Gaps do not prove dropouts.

Block-v1 candidate: only mixRender changed to silence/copy/add spans.
Compare measured cost against preserved EClock-v1 logs on the same CPU
and emulator settings. Music, gain, clock probes and script are unchanged.
No native performance improvement is claimed before log review.
