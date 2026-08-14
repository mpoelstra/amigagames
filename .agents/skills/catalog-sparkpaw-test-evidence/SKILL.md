---
name: catalog-sparkpaw-test-evidence
description: Inspect, safely rename and document Sparkpaw MOV, PNG and screenshot evidence in sparkpaw/testresults with a meaningful phase/result basename and matching TXT sidecar. Use whenever Codex is asked to look at, review, diagnose or preserve a newly supplied testresults recording or image, especially timestamp-named captures, before or alongside analyze-amiga-test-evidence.
---

# Catalog Sparkpaw Test Evidence

Catalog evidence as part of reviewing it. Never leave a newly reviewed generic
timestamp or screenshot name undocumented.

## Identify the evidence

1. Read `CODEX_HANDOFF.md` section “How to handle supplied test evidence” and
   inspect existing paired files in `sparkpaw/testresults` for local convention.
2. Apply `analyze-amiga-test-evidence` to inspect the actual MOV or PNG before
   naming it. Preserve the file bytes; renaming is allowed, transcoding is not.
3. Determine the roadmap phase only from current docs or user context. If it is
   unknown, use `Unassigned` rather than inventing a checkpoint.

## Choose a meaningful basename

Use `<Phase>-<status-or-purpose>-<specific-subject>` such as:

```text
Phase 6B.5-accepted-ledge-recovery.mov
Phase 6B.5-edge-hover-regression.png
Unassigned-real-amiga-hud-boundary-glitch.mov
```

Keep it concise, unique and stable. Describe what the evidence proves or shows,
not a speculative implementation cause. Retain the original extension and use
the identical basename for the `.txt` sidecar.

Before renaming, resolve the exact source and destination, check that neither
the destination media file nor sidecar exists, and inspect `git status`. Never
overwrite, delete or bulk-rename evidence. If the source is outside
`sparkpaw/testresults`, move it there only when the user supplied it as project
evidence. Leave established meaningful reference names such as
`thundercats-level1.mov` unchanged.

## Write the sidecar

Create `<basename>.txt` with only known facts under these headings:

```text
<descriptive title>
===================

Original recording/image: <original filename and supplied path>
Renamed evidence: <new filename>
Date supplied/reviewed: <date>
Evidence source: <user-supplied FS-UAE, real Amiga, screenshot, or unknown>
Build/version: <known value or unknown>
Launch medium: <HD, ADF, or unknown>

Media metadata
--------------

<duration, codec, dimensions, fps, frame count, audio and byte size as applicable>

What to inspect
---------------

<focused reproduction and visual/audio checkpoints>

Observed result
---------------

<what the evidence visibly establishes>

Analysis or follow-up
---------------------

<supported diagnosis, correction status and remaining test>

Acceptance
----------

<accepted/rejected/pending and exact FS-UAE, ADF or real-hardware boundary>
```

Omit inapplicable fields; do not fabricate them. Preserve conflicting or
intermittent observations. Report the rename and sidecar path after completion.
