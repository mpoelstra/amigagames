# Intro storyboard v2 production plan
Status: accepted in supplied FS-UAE/68030 and FS-UAE/68020 HD testing and
promoted to the normal alpha.47 build behind `SPARKPAW_STORY_INTRO`.

## Story contract

The Stormstone is one ancient weather machine. Its five installed Cores are
Lightning, Rain, Wind, Warmth and Balance; Balance coordinates the other four.
Weather stations normally distribute their energy through the valley.

Grand Archivolt was built to guard that system and contain only dangerous
weather. A damaged order now reads `CONTAIN ALL WEATHER. RELEASE NOTHING.` He
removes all five Cores, seals four in remote stations and keeps Balance in his
archive. He reverses the network, so every station pulls its element inward
without limit. Sparkpaw sees the resulting destruction and sets out to free
one Core per level and return all five to the Stormstone.

Verb discipline: Archivolt **contains weather**, Sparkpaw **recovers Cores**,
and the player **collects Storm Shards**.

## Five plates and exact passages

Each plate keeps art in the upper 320x168 area and text in a dark 88-pixel
reading band. Every passage rises once, holds, and fades independently before
the next passage appears over the same still.

1. **The Stormstone** — balanced valley, central machine and five stations.
   - `THE ANCIENT STORMSTONE / KEPT THE VALLEY'S WEATHER / IN PERFECT BALANCE.`
   - `FOUR CORES RULED LIGHTNING, / RAIN, WIND AND WARMTH. / THE FIFTH BALANCED THEM.`
2. **The damaged order** — Archivolt as solemn mechanical caretaker.
   - `GRAND ARCHIVOLT WAS BUILT / TO GUARD THE STORMSTONE / AND CONTROL WILD WEATHER.`
   - `A DAMAGED ORDER COMMANDED: / CONTAIN ALL WEATHER. / RELEASE NOTHING.`
3. **The reversed network** — five empty central sockets, sealed stations and
   weather drawn endlessly inward.
   - `ARCHIVOLT TORE THE CORES / FROM THE STORMSTONE / AND SEALED THEM AWAY.`
   - `HE REVERSED THE STATIONS. / EACH CORE DREW ITS ELEMENT / INWARD WITHOUT END.`
   - `RAIN BECAME FLOODS. / WIND BECAME HURRICANES. / LIGHTNING NEVER STOPPED.`
4. **Sparkpaw's motive** — Sparkpaw outside his damaged home, tracing the first
   station with his shard-powered gauntlet.
   - `SPARKPAW WATCHED THE STORMS / TEAR THROUGH HIS HOME. / SOMEONE HAD TO FREE THE CORES.`
   - `HIS SHARD POWERED GAUNTLET / COULD FIND AND CARRY THEM, / BACK TO THE STORMSTONE.`
5. **The quest** — Sparkpaw faces the route to the archive.
   - `RECOVER ONE CORE FROM / EACH WEATHER STATION.`
   - `RETURN THEM TO THE STORMSTONE. / RESTORE THE NATURAL SKY. / STOP GRAND ARCHIVOLT.`

## Presentation and controls

- crisp native pixel text, three lines maximum per passage;
- whole-pixel upward entrance followed by roughly 3.4 seconds of fully static
  reading time;
- text-only fade between passages, full palette fade between plates;
- Fire during entrance completes it, another tap advances, held Fire skips;
- left mouse immediately skips every remaining plate and returns to the title;
- plate 1 alone keeps a fixed lower-left white `LMB to skip intro` hint with a
  one-pixel black shadow; plates 2..5 remain unlabelled;
- only one tall plate asset is resident at a time;
- optional later motion is limited to cloud drift, Core/station pulses,
  Archivolt's eye or bell, lightning and Sparkpaws scarf.

## AGA art polish

All five proof plates now use the title screen as the colour, contrast and
pixel-cluster reference: deep navy shadows, violet atmosphere, cyan machinery,
warm orange focal light and larger silhouettes designed for the final 320x168
art window. Slide 3 was redesigned completely from scratch as a distant valley
overview. Its five identical recessed crystal niches are integrated into one
Stormstone façade in a 1-2-2 formation, sharing its perspective, material,
shadows and cyan rim light; no separate socket overlay remains.

The isolated proof is retained as historical test evidence. Alpha.47 packages
the same presentation before the existing title in HD and ADF builds.
