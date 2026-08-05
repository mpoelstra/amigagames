#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>
#include "sfxplayer.h"

struct Entry {
    char name[17];
    ULONG offset, length;
    UWORD period;
    UBYTE volume, priority;
};

static UBYTE *bank;
static UWORD *silence;
static ULONG bankSize;
static struct Entry entries[16];
static UWORD count;
static UBYTE nextChannel;
static UWORD channelFrames[4];
static UBYTE channelReloadPending[4];

static UWORD be16(UBYTE *p) { return ((UWORD)p[0] << 8) | p[1]; }
static ULONG be32(UBYTE *p) {
    return ((ULONG)p[0] << 24) | ((ULONG)p[1] << 16) |
           ((ULONG)p[2] << 8) | p[3];
}

BOOL sfxLoad(const char *filename)
{
    BPTR f;
    LONG size;
    UBYTE *p;
    UWORD i;
    sfxFree();
    f = Open((STRPTR)filename, MODE_OLDFILE);
    if (!f) return FALSE;
    Seek(f, 0, OFFSET_END); size = Seek(f, 0, OFFSET_CURRENT);
    Seek(f, 0, OFFSET_BEGINNING);
    if (size < 40 || !(bank = AllocMem((ULONG)size, MEMF_CHIP))) {
        Close(f); return FALSE;
    }
    if (Read(f, bank, size) != size) {
        FreeMem(bank, (ULONG)size); bank = NULL; Close(f); return FALSE;
    }
    Close(f); bankSize = (ULONG)size;
    if (memcmp(bank, "MDFS", 4) || be16(bank + 4) != 1) {
        sfxFree(); return FALSE;
    }
    count = be16(bank + 6); if (count > 16) count = 16;
    silence = (UWORD *)AllocMem(2, MEMF_CHIP | MEMF_CLEAR);
    for (i = 0; i < count; ++i) {
        p = bank + 8 + i * 32;
        memcpy(entries[i].name, p, 16); entries[i].name[16] = 0;
        entries[i].offset = be32(p + 16);
        entries[i].length = be32(p + 20);
        entries[i].period = be16(p + 24);
        entries[i].volume = p[26]; entries[i].priority = p[27];
        if (entries[i].offset + entries[i].length > bankSize)
            entries[i].length = 0;
    }
    return TRUE;
}

void sfxPlay(const char *name, WORD pan)
{
    volatile UWORD *dmacon = (volatile UWORD *)0xdff096;
    volatile ULONG *location;
    volatile UWORD *length, *period, *volume;
    UWORD i, channel, bit;
    struct Entry *e = NULL;
    if (!bank) return;
    for (i = 0; i < count; ++i)
        if (!strcmp(entries[i].name, name)) { e = &entries[i]; break; }
    if (!e || !e->length) return;
    channel = pan < 0 ? 3 : (pan > 0 ? 2 : (nextChannel++ & 1 ? 3 : 2));
    bit = (UWORD)(1 << channel);
    location = (volatile ULONG *)(0xdff0a0 + channel * 16);
    length = (volatile UWORD *)(0xdff0a4 + channel * 16);
    period = (volatile UWORD *)(0xdff0a6 + channel * 16);
    volume = (volatile UWORD *)(0xdff0a8 + channel * 16);
    Disable();
    *dmacon = bit;
    *location = (ULONG)(bank + e->offset);
    *length = (UWORD)(e->length / 2);
    *period = e->period; *volume = e->volume;
    *dmacon = (UWORD)(0x8000 | bit);
    /*
     * Keep an independent end time as well as Paula's silent reload.
     * Some hardware/emulator timings can otherwise leave a tiny word
     * repeating at an audible high pitch after a one-shot effect.
     */
    channelFrames[channel]=(UWORD)
        (((e->length*e->period*50UL)+3546894UL)/3546895UL+2);
    /*
     * Paula needs time to latch the effect location/length after DMA starts.
     * Program the silent reload on the next video tick rather than
     * immediately; an immediate rewrite can occasionally be latched as the
     * active sample and leave a stale tiny loop whining at high pitch.
     */
    channelReloadPending[channel]=silence?1:0;
    Enable();
}

void sfxTick(void)
{
    volatile UWORD *dmacon=(volatile UWORD *)0xdff096;
    volatile ULONG *location;
    volatile UWORD *length,*period,*volume;
    UWORD channel,bit;
    for(channel=2;channel<=3;++channel) {
        if(channelReloadPending[channel]) {
            location=(volatile ULONG *)(0xdff0a0+channel*16);
            length=(volatile UWORD *)(0xdff0a4+channel*16);
            Disable();
            *location=(ULONG)silence;
            *length=1;
            Enable();
            channelReloadPending[channel]=0;
        }
        if(channelFrames[channel]&&!--channelFrames[channel]) {
            bit=(UWORD)(1<<channel);
            location=(volatile ULONG *)(0xdff0a0+channel*16);
            length=(volatile UWORD *)(0xdff0a4+channel*16);
            period=(volatile UWORD *)(0xdff0a6+channel*16);
            volume=(volatile UWORD *)(0xdff0a8+channel*16);
            Disable();
            *dmacon=bit;
            *volume=0;
            if(silence) {
                *location=(ULONG)silence;
                *length=1;
                *period=124;
            }
            Enable();
        }
    }
}

void sfxFree(void)
{
    volatile UWORD *dmacon = (volatile UWORD *)0xdff096;
    *dmacon = 0x000c;
    if (bank) FreeMem(bank, bankSize);
    if (silence) FreeMem(silence, 2);
    bank = NULL; bankSize = 0; count = 0;
    silence = NULL;
    memset(channelFrames,0,sizeof(channelFrames));
    memset(channelReloadPending,0,sizeof(channelReloadPending));
}
