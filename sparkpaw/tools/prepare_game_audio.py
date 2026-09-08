"""Reproducible three-voice ptplayer adapter; preserve pristine upstream source."""
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]

def player_source():
    s=(ROOT/'third_party/ptplayer/ptplayer.asm').read_text()
    for old,new,count in [
        ('\tlea\tAUD3LC(a6),a5\n\tlea\tmt_chan4(a4),a2\n\tbsr\tmt_checkfx','\t; Channel 4 belongs exclusively to the mixer.',2),
        ('\tlea\tAUD3LC(a6),a5\n\tlea\tmt_chan4(a4),a2\n\tbsr\tmt_playvoice','\taddq.l\t#4,a1\t; consume certified empty fourth-track event',1)]:
        assert s.count(old)==count
        s=s.replace(old,new)
    # music.c owns audio.device for the entire session. Reserve only CIA-B here.
    a=s.index('\t; open audio.device and allocate all four audio channels')
    b=s.index('\tifeq\tNO_TIMERS',a)
    s=s[:a]+'\tmove.l\t4.w,a6\t; shared audio.device owner: music.c\n\n'+s[b:]
    a=s.index('\t; reset the player\n');b=s.index('\tifeq\tNO_TIMERS',a)
    s=s[:a]+'\tbra\t.popout\t; no Paula/filter writes while title is playing\n\n'+s[b:]
    a=s.index('.no_rsrc:\n');b=s.index('.fail:\n',a)
    s=s[:a]+'.no_rsrc:\n'+s[b:]
    a=s.index('\t; close audio.device, free channels');b=s.index('\tmove.l\t(sp)+,a6',a)
    s=s[:a]+'\t; Shared audio.device owner remains music.c.\n'+s[b:]
    # Public setters are only called under platform/Exec Disable. Do not let
    # upstream setters transiently re-enable the machine master interrupt.
    s=s.replace('\tDISABLE\n','\t; caller owns interrupt exclusion\n')
    s=s.replace('\tENABLE\n','\t; caller restores interrupt mask\n')
    s=s.replace('mt_ioport:\n','\txdef\t_mt_pause_timer_b\n_mt_pause_timer_b:\n\tclr.b\tCIAB+CIACRB\n\tlea\tTB_toggle(pc),a0\n\tclr.b\t(a0)\n\trts\n\nmt_ioport:\n',1)
    return s

def main():
    build=ROOT/'build';build.mkdir(exist_ok=True)
    (build/'game_ptplayer.asm').write_text(player_source())
    for source,prefix in [('music/level1/copper-sprint.mod','pulse'),
                          ('music/stormrail/iron-horizon.mod','rail')]:
        mod=(ROOT/source).read_bytes()
        assert mod[1080:1084]==b'M.K.' and 0<mod[950]<=128
        split=1084+1024*(max(mod[952:952+mod[950]])+1)
        assert all(mod[i:i+4]==bytes(4) for i in range(1096,split,16))
        assert sum(int.from_bytes(mod[42+i*30:44+i*30],'big')*2 for i in range(31))==len(mod)-split
        (ROOT/f'assets/runtime/{prefix}-score.bin').write_bytes(mod[:split])
        (ROOT/f'assets/runtime/{prefix}-bank.bin').write_bytes(mod[split:])
if __name__=='__main__':main()
