/* Loading-only guest VBlank measurements. No per-frame gameplay profiler,
   no automatic filesystem writes and no assertion of host disk wall time. */
#include "whd_load_trace.h"
#include <graphics/gfxbase.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#ifdef SPARKPAW_WHD_LOAD_STATE
#include <hardware/custom.h>
#include <hardware/cia.h>
#include "platform_amiga.h"
ULONG whdLoadTraceReadCacr(void);
struct LoadState {
    ULONG tod,cacr;
    UWORD dma,intena,intreq;
    UBYTE cra,crb;
};
static struct LoadState stateStarted[WLT_PHASE_COUNT];
static void readState(struct LoadState *s)
{
    volatile struct Custom *custom=(volatile struct Custom *)0xdff000;
    volatile struct CIA *ciab=(volatile struct CIA *)0xbfd000;
    s->tod=platformFieldCounter();
    s->cacr=whdLoadTraceReadCacr(); /* Supervisor read only, NEVER write CACR. */
    s->dma=custom->dmaconr;s->intena=custom->intenar;s->intreq=custom->intreqr;
    s->cra=ciab->ciacra;s->crb=ciab->ciacrb;
    /* Do not read CIA ICR: that would clear pending interrupts. */
}
#endif
#define TRACE_CAPACITY 192
struct TraceRow { ULONG fields,fast,chip; UWORD section,phase,success;
#ifdef SPARKPAW_WHD_LOAD_STATE
    struct LoadState before,after;
#endif
};
static struct TraceRow rows[TRACE_CAPACITY];
static ULONG started[WLT_PHASE_COUNT];
static UWORD startedSection[WLT_PHASE_COUNT],count,section=1;
static BOOL overflow;
static const char *names[WLT_PHASE_COUNT]={
    "gameplay_assets","collision","audio","renderer_prepare","ready_total",
    "ready_menu_cache","charging_min_wait","renderer_setup",
    "renderer_enemies","renderer_effects","renderer_targets","renderer_final",
    "boot_banks","return_release","return_cleanup","return_title"};
void whdLoadTraceSection(UWORD value) { section=value; }
void whdLoadTraceBegin(UWORD phase)
{
    if(phase>=WLT_PHASE_COUNT) return;
#ifdef SPARKPAW_WHD_LOAD_STATE
    readState(&stateStarted[phase]);
#endif
    started[phase]=GfxBase->VBCounter; startedSection[phase]=section;
}
BOOL whdLoadTraceEnd(UWORD phase,BOOL success)
{
    struct TraceRow *r;
    if(phase>=WLT_PHASE_COUNT) return success;
    if(count>=TRACE_CAPACITY) { overflow=TRUE; return success; }
    r=&rows[count++];r->fields=GfxBase->VBCounter-started[phase];
#ifdef SPARKPAW_WHD_LOAD_STATE
    r->before=stateStarted[phase];readState(&r->after);
#endif
    r->section=startedSection[phase];r->phase=phase;r->success=success;
    r->fast=AvailMem(MEMF_FAST);r->chip=AvailMem(MEMF_CHIP);
    return success;
}
void whdLoadTraceWrite(void)
{
    UWORD i;
    BPTR file=Open("PROGDIR:load-times.log",MODE_NEWFILE);
    if(!file) return;
    FPrintf(file,"Sparkpaw WHD-LevelTimes diagnostic v1\n");
    FPrintf(file,"clock=guest_vblank pal_fields_per_second=50\n");
    FPrintf(file,"Disk host-switch wall time is NOT measured. Nested ready_menu_cache is INCLUDED in ready_total; renderer subphases are INCLUDED in renderer_prepare.\n");
    FPrintf(file,"Free-memory snapshots are phase ends, NOT exact high-water marks.\n");
#ifdef SPARKPAW_WHD_LOAD_STATE
    FPrintf(file,"state_probe=1 values_decimal; CIAA_TOD delta is supplemental, host-switch semantics not assumed. CACR read via Exec Supervisor, no cache writes.\n");
#endif
    for(i=0;i<count;i++) {
      FPrintf(file,
        "section=%ld phase=%s fields=%ld ok=%ld fast_free=%ld chip_free=%ld\n",
        (LONG)rows[i].section,(STRPTR)names[rows[i].phase],rows[i].fields,
        (LONG)rows[i].success,rows[i].fast,rows[i].chip);
#ifdef SPARKPAW_WHD_LOAD_STATE
      FPrintf(file,"state row=%ld tod_delta=%ld cacr_before=%ld cacr_after=%ld dma_before=%ld dma_after=%ld intena_before=%ld intena_after=%ld intreq_before=%ld intreq_after=%ld ciab_cra_before=%ld ciab_cra_after=%ld ciab_crb_before=%ld ciab_crb_after=%ld\n",
          (LONG)i,(rows[i].after.tod-rows[i].before.tod)&0xffffffUL,
          rows[i].before.cacr,rows[i].after.cacr,
          (LONG)rows[i].before.dma,(LONG)rows[i].after.dma,
          (LONG)rows[i].before.intena,(LONG)rows[i].after.intena,
          (LONG)rows[i].before.intreq,(LONG)rows[i].after.intreq,
          (LONG)rows[i].before.cra,(LONG)rows[i].after.cra,
          (LONG)rows[i].before.crb,(LONG)rows[i].after.crb);
#endif
    }
    FPrintf(file,"complete=1 rows=%ld overflow=%ld\n",(LONG)count,(LONG)overflow);
    Flush(file);Close(file);
}
