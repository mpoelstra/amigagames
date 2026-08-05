#include "audio.h"

#include <exec/memory.h>
#include <dos/dos.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/dos.h>
#include <proto/exec.h>

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static UBYTE *shotSample;
static LONG shotSampleBytes;
static UBYTE shotDmaTicks;
static BOOL hardwareActive;

BOOL audioLoad(void)
{
    BPTR file=Open("PROGDIR:assets/runtime/energy-shot.raw",MODE_OLDFILE);
    LONG size;
    if(!file) return FALSE;
    Seek(file,0,OFFSET_END); size=Seek(file,0,OFFSET_BEGINNING);
    if(size<=0) { Close(file); return FALSE; }
    shotSample=(UBYTE *)AllocMem(size,MEMF_CHIP);
    if(!shotSample||Read(file,shotSample,size)!=size) {
        Close(file);
        if(shotSample) FreeMem(shotSample,size);
        shotSample=NULL; return FALSE;
    }
    Close(file); shotSampleBytes=size; return TRUE;
}

void audioUnload(void)
{
    if(shotSample) {
        FreeMem(shotSample,shotSampleBytes);
        shotSample=NULL; shotSampleBytes=0;
    }
}

void audioSetHardwareActive(BOOL active)
{
    hardwareActive=active;
    if(!active) {
        hardware->dmacon=DMAF_AUD0;
        shotDmaTicks=0;
    }
}

void audioPlayShot(void)
{
    if(!shotSample||!hardwareActive) return;
    hardware->dmacon=DMAF_AUD0;
    hardware->aud[0].ac_ptr=(UWORD *)shotSample;
    hardware->aud[0].ac_len=(UWORD)(shotSampleBytes>>1);
    hardware->aud[0].ac_per=322;
    hardware->aud[0].ac_vol=60;
    hardware->dmacon=DMAF_SETCLR|DMAF_AUD0;
    shotDmaTicks=9;
}

void audioUpdate(void)
{
    if(hardwareActive&&shotDmaTicks&&!--shotDmaTicks)
        hardware->dmacon=DMAF_AUD0;
}