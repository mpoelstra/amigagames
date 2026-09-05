> Historical session prompt. Do not execute as current work; see [CURRENT_STATUS.md](CURRENT_STATUS.md).

# Next-session prompt — Stormrail Gate 5A post-debris patterns

Lees eerst volledig `CODEX_HANDOFF.md`, `sparkpaw/README.md`,
`sparkpaw/docs/PHASE6D_PROGRESSION_PLAN.md`,
`sparkpaw/docs/STORMRAIL_INTERLUDE_PLAN.md` en
`sparkpaw/docs/STORMRAIL_DEBRIS_BASELINES.md`. Lees vóór wijzigingen ook de
relevante Sparkpaw FS-UAE-, test-cycle-, visual-, animatie- en
test-evidenceskills.

Ga uitsluitend verder met Stormrail Gate 5A: een eerste post-debris
enemy-patternslice in de nog lege speelruimte na het geaccepteerde debrisveld.
Gebruik `sparkpaw/testresults/Phase 6D-reference-project-x-enemy-patterns.mov`
als bewegingsreferentie. Bestudeer het fragment temporeel; kopieer geen art of
exacte routes.

Bescherm de complete huidige baseline:

- Gate 1 boarding en Gate 2/2.5 transition, controls, autoscroll, parallax en
  13-stage/23-state palettetable;
- Gate 3's vier vroege formaties, twee enemytypes, audio, collision, compacte
  score, restart en persistent pickup/scoregedrag;
- Debris5.2's ene monotone 48-event choreografie, alle ruin-artfamilies,
  drukte/variatie, zes obstacle-slots en duidelijke lege uitgang;
- de tragere zes-hit carrier die health-heart ID 33 dropt;
- het native klassieke heart en de ruime heart-plus-drie-diamond ring met IDs
  34..37 tussen de vroege golven;
- de geaccepteerde complete stock-68020 cadence: 49,96 FPS over 2.886
  intervallen, 2.885 one-field, nul two-field, één three-field, nul
  ownership-violations en 334/334 shots.

Leg vóór implementatie één data-gedreven post-debris timelinecontract vast.
Maak daarna twee formaties met uitsluitend de bestaande Dart- en Orb-art. Hun
nieuwe bewegingsflows moeten aantoonbaar zijn afgeleid van de vlieg- en
in/uitbeeldgrammatica in de lokale Project-X-testresultreferentie, aangepast
aan Sparkpaws snelheid, schermruimte en fairness:

1. een goed leesbare curl/loop met uitsluitend Darts, die vanaf rechts
   binnenkomt, zichtbaar draait of terugbuigt en daarna definitief links/rechts
   verlaat. Zo kan de nieuwe route eerst zonder extra gedragsruis worden
   beoordeeld;
2. een duidelijk andere gemengde crossing/rejoin- of roterende keten: drie
   snelle Darts openen/tekenen de route en één of twee tragere Pulse Orbs
   fungeren als schietende ankers. Hiermee worden aim, ontwijken en
   gecontroleerd rapid fire samen getest zonder de vijf-enemycap te verhogen.

Plaats tussen deze twee nieuwe golven één korte free-diamondpickupformatie als
adempauze en positioneringsprikkel. Gebruik 3–4 bestaande Sparkpaw-diamonds in
een compact maar niet opeengepakt boog-, ruit- of slalompatroon. Geef elke
diamond een nieuwe unieke persistente section-ID en stream ze door de bestaande
vier-slot rewardpool; vergroot de pool niet. De pickups moeten voldoende vroeg
binnenkomen en volledig verzamelbaar zijn voordat golf twee begint.

Het moet aanvoelen als een volgende acte na de benauwde debrispassage, niet als
nog meer van hetzelfde. Geef eerst kort herstel, daarna actie en vervolgens
weer een leesbare lege reserve. Geen enemy mag blind van links spawnen of in een
oneindige lus blijven. Complete-formation rewards blijven 3–5 bestaande
diamonds met unieke persistente IDs. Koppel alles aan monotone
`stormrailDistance`, nooit aan camera/panorama-wrap.

Behoud maximaal vijf enemies, vijf player shots, vier hostile shots, vier
rewards en zes debris-objecten tegelijk. Voeg geen nieuwe enemy-art, extra
poolcapaciteit, runtime scaling/rotation, per-pixel effecten of allocations per
frame toe. Storm Ruins dust, clouds, nieuwe hazards, 1UP, finale/poort,
resultaten, Level 2 en campaign-integratie blijven buiten Gate 5A.

Werk kort en incrementeel: contract/hosttest, één compile-guarded technische
proof buiten `dist`, daarna snel één betekenisvolle 68030-drawer voor mijn
visuele/feel-test. Pas na mijn acceptatie volgt dezelfde low-overhead 68020
cadence gate. Houd precies één actieve testdrawer in `sparkpaw/dist`, archiveer
superseded drawers intact en wijzig alpha.68, releases, ADF/multidisk en Level 2
niet. Stop na deze twee post-debris formaties voor expliciete acceptatie.
