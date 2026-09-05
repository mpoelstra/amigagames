> Historical session prompt. Do not execute as current work; see [CURRENT_STATUS.md](CURRENT_STATUS.md).

# Next-session prompt — Stormrail Gate 3

Status: historisch en voltooid. Gebruik voor de volgende werksessie
`NEXT_SESSION_STORMRAIL_GATE5A_PROMPT.md`; voer deze Gate-3-prompt niet opnieuw
uit.

Lees eerst volledig `CODEX_HANDOFF.md`, `sparkpaw/README.md`,
`sparkpaw/docs/PHASE6D_PROGRESSION_PLAN.md` en
`sparkpaw/docs/STORMRAIL_INTERLUDE_PLAN.md`. Lees vóór wijzigingen ook de
relevante Sparkpaw FS-UAE-, test-cycle-, visual-, animatie- en
test-evidenceskills.

We gaan verder met uitsluitend Stormrail Gate 3: de eerste representatieve
enemy/formation/action-slice in de interlude tussen Level 1 en Level 2.

Behoud Gate 1 en Gate 2/2.5 als baseline. De korte klifapproach, boarding,
acceleratie, fade-overgang, open-flight controls, compacte Skimmer/pilot,
schietpunt, snelle vuursnelheid, schermgrenzen, brisk autoscroll, naadloze
Stormrail-parallax en de 13-stage/23-state palettetable mogen geen regressie
vertonen.
Breng de verworpen handgetekende sink-neck/scarf-aanpassing niet terug. De
cockpit-pilot is voorlopig geaccepteerd; besteed deze sessie niet opnieuw aan
losse hoofdvarianten tenzij een aantoonbare technische regressie dat vereist.

Ontwerp en implementeer één coherente Gate-3-slice met:

- twee kleine, visueel duidelijk verschillende vliegende enemytypes;
- twee deterministische, data-gedreven formaties met goed leesbare entree,
  vliegroute en exit;
- correcte player-shot/enemy-hit/death-afhandeling en eerlijke enemy-fire waar
  die voor het tweede type nodig is;
- complete-formation-detectie en één korte reward chain van 3–5 bestaande
  Sparkpaw-diamonds;
- genoeg timing en rust tussen patronen om movement, aim, ontwijken en rapid
  fire plezierig te beoordelen;
- geen corridorwanden en nog geen uitgebreide hazards, eindbaas, volledige
  2,5-minutenroute, resultaten of campaign-integratie.

Koppel encounter spawning aan de monotone `stormrailDistance`, niet aan de
begrensde presentatiecamera of panorama-wrap. Houd objectpools en
on-screen-aantallen expliciet begrensd. Start met maximaal vijf kleine enemies,
vijf player shots en vier hostile shots tegelijk. Flight mode moet exclusief
eigenaar zijn van zijn enemies, projectielen, rewards en renderhistorie, zodat
platform-Level 1 en Stormrail elkaar niet kunnen bijten.

De art moet op native AGA-schaal strak, origineel en onmiddellijk leesbaar zijn
tegen alle vier routepaletten. Gebruik sterke silhouetten, spaarzame animatie
en schone maskers; geen domme scaling, tijdelijke programmeurspixels of dure
per-pixel runtime-effecten. Reserveer visuele ruimte en CPU/Chip-RAM voor latere
formaties en de finale.

Werk incrementeel. Leg eerst de encounter/pool/timeline-contracten en het
performancebudget vast. Test kleine technische risico's zelf met de
compile-guarded direct-start proof buiten `dist`. Controleer logs én echte
zichtbare emulatorframes. Gate daarna eerst een betekenisvolle art/feel-versie
op 68030 en pas vervolgens de cadence op 68020. Vergelijk met de geaccepteerde
lege baseline: 49,26 FPS over 1000 intervallen, 992 one-field, nul
ownership-violations en 214/214 shots. Onderzoek regressies met gerichte
profiling en inspecteer zo nodig de door vbcc gegenereerde assembly; gebruik de
low-overhead cadence logger voor het uiteindelijke FPS-oordeel.

Behoud precies één actieve testdrawer in `sparkpaw/dist`; archiveer
superseded/rejected drawers met duidelijke provenance en behoud testbewijs.
Wijzig alpha.68, releasebestanden, ADF/multidisk, Level 2 en campaign/results
niet. Stop na een coherente Gate-3-slice voor mijn expliciete acceptatie voordat
je Gate 4, hazards of de volledige interlude gaat bouwen.
