"""Player-facing ASCII ReadMe shared by HD and both WHDLoad editions."""
from pathlib import Path
from textwrap import fill


def personal_note():
    # User-authored wording: normalize layout only, never rewrite the prose.
    source = Path(__file__).resolve().parents[1] / 'docs/PERSONAL_NOTE.txt'
    return '\n\n'.join(fill(paragraph, width=72, break_long_words=False,
                            break_on_hyphens=False)
                       for paragraph in source.read_text(encoding='ascii').strip().split('\n\n'))


def game_readme(version, edition, *, candidate=False):
    labels={'hd':'Hard-drive edition','whd':'WHDLoad - 8 MB Fast edition','high':'WHDLoad - High RAM edition'}
    title=f'SPARKPAW: THE STORMSTONE QUEST\nVersion {version}\n{labels[edition]}'
    if candidate:title+='\nUNNUMBERED TEST BUILD - flight-controls correction for review'
    launch={
        'hd':'''Extract the complete archive to your Amiga hard drive and open its drawer.
Double-click Sparkpaw. Keep the assets drawer alongside the executable.''',
        'whd':'''Requires WHDLoad 19 or newer and your own Kickstart 3.1 A1200 ROM and
matching RTB installed for WHDLoad. These are not supplied.
Extract the complete archive and double-click the Sparkpaw icon.
Keep the supplied PAL and NOCACHE settings. Leave PRELOAD disabled for
this 8 MB edition. F10 returns to Workbench.''',
        'high':'''Requires at least 16 MB Fast RAM, WHDLoad 19 or newer, and your own
Kickstart 3.1 A1200 ROM and matching RTB installed for WHDLoad. These are
not supplied. Choose standard WHDLoad if you have 8 MB Fast RAM.
Extract the complete archive and double-click the Sparkpaw icon.
Keep the supplied PAL and PRELOAD settings. The whole campaign loads
at startup, so the initial wait can be longer. F10 returns to Workbench.'''}[edition]
    return personal_note()+'\n\n'+'='*70+'\n\n'+title+'\n'+'='*70+'''

WELCOME
-------
Sparkpaw is an original side-scrolling action platformer and aerial shooter
for the Commodore Amiga 1200. Explore storm-lit ruins, battle clockwork
machines and take to the skies in your Skimmer. This is a playable alpha
of a growing adventure; feedback is welcome!

THE STORY
---------
For centuries, the ancient Stormstone kept the valley's weather in balance.
Its five Cores governed Lightning, Rain, Wind, Warmth and Balance.

Grand Archivolt was built to guard this system. Then a damaged instruction
changed his mission: CONTAIN ALL WEATHER. RELEASE NOTHING.
He removed the Cores, locked them away and reversed the weather network.
Now the stations draw in their elements without limit, tearing the valley
apart.

Sparkpaw, a young feline inventor with an energy gauntlet, sets out to
recover the Cores and put the sky back where it belongs.

IN THIS ALPHA
-------------
* Storm Ruins: platforms, water hazards, clockwork enemies and the first
  Stormstone Core. Electrical pulses cross the purple sky and illuminate
  the tower crystal; distant machinery flickers to life.
* Stormrail Skimmer: board your ship for an aerial combat interlude.
* Drowned Turbines: flooded platforms, moving pontoons, animated waterfalls,
  new enemies, a checkpoint and the Rain Core encounter.
* Lives, health, collected diamonds and score carry through the campaign.
* Story introduction, music and sound effects, results screens and replay.
* OPTIONS offers section selection, audio settings and controller choice.
  HD and WHDLoad also include SOUNDTEST.

The complete five-Core adventure is still in development.
See ReleaseNotes.txt for changes since the previous public version.

REQUIREMENTS
------------
PAL Amiga 1200 or compatible AGA machine, 68020 or faster, 2 MB Chip RAM
and 8 MB Fast RAM. WHDLoad High RAM requires 16 MB Fast RAM.
Connect a joystick or compatible two-button joypad to joystick port 2.

INSTALLATION
------------
'''+launch+'''

CONTROLS
--------
Menus: Up/Down selects; Left/Right changes options; Fire or Space confirms.

On foot - CONTROL: JOYSTICK
  Left / Right       Move
  Up                 Jump (hold for a higher jump)
  Down               Crouch
  Primary button     Shoot
  Second button      Unused

On foot - CONTROL: JOYPAD
  D-pad Left / Right Move
  Second button      Jump (hold for a higher jump)
  D-pad Down         Crouch
  Primary button     Shoot
  D-pad Up           Unused for jumping

Flying the Skimmer - BOTH controller modes
  Up / Down          Fly up / down
  Left / Right       Fly left / right
  Primary button     Shoot (hold to keep firing)
  Second button      Unused

Keyboard
  A / D              Move left / right
  W                  Jump on foot; fly up in the Skimmer
  S                  Crouch on foot; fly down in the Skimmer
  Space              Shoot
  P                  Pause / resume
  Esc                Return to the ready menu

Story intro: Fire reveals/advances the text. Hold Fire or use the left
mouse button to skip. WHDLoad: F10 quits to Workbench.

ABOUT THE CREATOR
-----------------
Sparkpaw is a project by MrDig / MrDig Productions.
MrDig leads the creative direction, shapes the art and gameplay, and tests
builds in FS-UAE and on Amiga hardware. Development is a collaboration with
OpenAI's Codex, assisting with programming, tools, artwork and debugging.
The game grows through repeated hands-on testing and player feedback.

DOWNLOADS, NEWS AND FEEDBACK
---------------------------
Official game page:
  https://mrdig.itch.io/sparkpaw

More from MrDig:
  https://mrdig.itch.io/

Please report problems on the game's itch.io page. Include the version,
edition (HD, WHDLoad or ADF), Amiga/CPU, Chip/Fast RAM, and the steps needed
to reproduce the issue. Screenshots or a short recording are very helpful.

ALPHA STATUS
------------
Compatibility and performance can vary with accelerator and display setup.
Final-release hardware testing is ongoing; an intermittent HUD-boundary
visual issue on real Amiga hardware remains under investigation.

Thank you for playing Sparkpaw!
'''
