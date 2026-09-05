> Historical session prompt. Do not execute as current work; see [CURRENT_STATUS.md](CURRENT_STATUS.md).

# Next session prompt — Stormrail interlude results

Lees eerst volledig `CODEX_HANDOFF.md`, `sparkpaw/README.md`,
`sparkpaw/docs/PHASE6D_PROGRESSION_PLAN.md`,
`sparkpaw/docs/STORMRAIL_INTERLUDE_PLAN.md`,
`sparkpaw/docs/STORMRAIL_DEBRIS_BASELINES.md`,
`sparkpaw/docs/STORMRAIL_GATE5A_CONTRACT.md` en
`sparkpaw/docs/STORMRAIL_GATE6_FINALE_CONTRACT.md`. Lees vóór wijzigingen ook
de relevante Sparkpaw FS-UAE-, test-cycle-, visual- en test-evidenceskills.

Ga uitsluitend verder met het statistieken-/resultatenscherm ná de complete
Stormrail-interlude. De trigger is Gate-6 `COMPLETE`, dus pas nadat de 120-HP
Harrier is vernietigd, hostile fire gestopt is, de poort geopend is en de
Skimmer automatisch door de Storm Ruins-poort is gevlogen.

Leg vóór implementatie één data-gedreven resultatencontract plus hosttests
vast. De eerste interludeversie hergebruikt het bestaande Level-1-statsscherm
exact: dezelfde 320x256 zes-plane compositie, dezelfde vier zichtbare items
`ENEMIES x20`, `DIAMONDS x5`, `TIME x10` en de totale `SCORE`, dezelfde posities,
fonts, kleuren, tallyvolgorde, versnelling/skip, input-debounce en tallyaudio.
Voeg in deze stap geen Stormrail-specifieke statistiek, label, campagneveld of
nieuwe results-art toe. Bepaal verder expliciet:

- dat enemies, diamonds, elapsed time en live score uit de afgeronde
  Stormrail-run komen en niet uit Level 1;
- een eerlijke, begrensde time-bonus voor deze deterministische route. Gebruik
  `elapsedFields` als monotone bron en kies één gedocumenteerde par-tijd op basis
  van de complete interlude, niet alleen de boss-timer. Bonus mag nooit negatief
  zijn, mag door life-loss/retry niet resetten en wordt exact één keer berekend;
- dat de 320 Harrier-punten al in de live interlude-score zitten en niet bij de
  tally dubbel worden toegekend;
- dat de getoonde `SCORE` uitsluitend de Stormrail-section score plus de
  eenmalige time-bonus is; toekomstig campaign banking blijft buiten scope;
- dat het bestaande prompt `REPLAY LEVEL` zichtbaar en selecteerbaar blijft.
  Fire na de tally moet volledig naar zwart faden en daarna een verse resident
  Stormrail-interlude starten bij departure/boarding, zonder Level 1, nieuwe
  assetload, diskicoon of Workbench;
- dat replay alle lokale Stormrail-runstate reset: section score, elapsed time,
  enemies/debris/finale, Harrier-HP, lives/health, diamonds, pickup-/awardbits,
  projectielen en inputhistorie. Hosttests bewijzen dat niets uit de afgeronde
  run kan worden meegenomen of opnieuw toegekend.

Begin klein en omkeerbaar. Hergebruik de bestaande Level-1-results-presenter en
assets zonder visuele variant; alleen de trigger, Stormrail-bronwaarden,
interlude-par-tijd en afgebakende test-eindstaat mogen verschillen.
Voeg nog geen Level-2-load, campaign banking, aparte `REPLAY INTERLUDE`-tekst,
nieuwe release, SemVer, ADF/multidisk of gewijzigde alpha.68-bestanden toe. Toon
alleen de echte bestaande `REPLAY LEVEL`-actie en geen misleidende `CONTINUE`.

Bescherm de volledige geaccepteerde Stormrailbaseline, in het bijzonder de
monotone afstand 15500, finale-local timers, 120/60-HP Harrierfasen, 320-punten-
award ID 60, persistent pickups/score, half-hart contact, intacte muur, stof,
line-252-HUD, rendererownership en de ene resident load. Vergroot geen pool en
voeg geen allocations per frame, runtime scaling/rotation of per-pixeleffecten
toe.

Gebruik hosttests en compilechecks zelfstandig. Start FS-UAE alleen bij hoge
uitzondering voor één vooraf benoemde native-only diagnose die niet anders te
bewijzen is. Boot nooit uit `sparkpaw/dist`. Controleer eerst dat de complete-
interlude stock-68020 cadence is geaccepteerd. Houd precies één actieve
volledige user-testdrawer in `sparkpaw/dist`, archiveer de cadence-drawer intact
wanneer een resultatenversie wordt klaargezet, en laat mij de zichtbare tally,
timing, audio, input, replaytransitie en feel testen. Stop daarna voor mijn
expliciete acceptatie.
