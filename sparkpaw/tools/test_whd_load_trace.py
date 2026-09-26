"""Sanitize the actual loading timer collector without booting an emulator."""
import argparse
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[1]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--state',action='store_true')
    p.add_argument('--output-dir',required=True,type=Path)
    args=p.parse_args();out=args.output_dir.resolve();out.mkdir(parents=True,exist_ok=False)
    def body(name):
        return '\n'.join(s for s in (ROOT/'src'/name).read_text().splitlines()
                         if not s.startswith('#include'))
    stub=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
typedef uint32_t ULONG;typedef uint16_t UWORD;typedef int32_t LONG;
typedef int BOOL;typedef int BPTR;typedef char *STRPTR;
#define SPARKPAW_WHD_LOAD_TRACE
#define TRUE 1
#define FALSE 0
#define MEMF_FAST 4
#define MEMF_CHIP 2
#define MODE_NEWFILE 1006
static struct { ULONG VBCounter; } gfx;
#define GfxBase (&gfx)
static unsigned io,lines,footers;
static ULONG AvailMem(ULONG flags) { return flags*10000; }
static BPTR Open(const char *n,LONG mode) {
 assert(!strcmp(n,"PROGDIR:load-times.log")&&mode==MODE_NEWFILE);io++;return 1;
}
static LONG FPrintf(BPTR f,const char *s,...) {
 assert(f==1);lines++;if(strstr(s,"complete=1"))footers++;return 1;
}
static LONG Flush(BPTR f) { assert(f==1);io++;return 1; }
static LONG Close(BPTR f) { assert(f==1);io++;return 1; }
'''
    driver=r'''
static BOOL work(ULONG ticks,BOOL result) { gfx.VBCounter+=ticks;return result; }
int main(void) {
 unsigned i;
 assert(!WLT_CALL(WLT_FILES,work(12,FALSE)));
 assert(count==1&&rows[0].fields==12&&!rows[0].success&&rows[0].section==1);
 assert(rows[0].fast==40000&&rows[0].chip==20000);
 whdLoadTraceSection(3);gfx.VBCounter=200;whdLoadTraceBegin(WLT_READY);
 gfx.VBCounter=210;whdLoadTraceBegin(WLT_MENU_CACHE);
 gfx.VBCounter=240;assert(whdLoadTraceEnd(WLT_MENU_CACHE,TRUE));
 gfx.VBCounter=260;whdLoadTraceSection(1);assert(whdLoadTraceEnd(WLT_READY,TRUE));
 assert(rows[1].fields==30&&rows[2].fields==60&&rows[2].section==3);
 gfx.VBCounter=0xfffffff0U;whdLoadTraceBegin(WLT_AUDIO);
 gfx.VBCounter=5;whdLoadTraceEnd(WLT_AUDIO,TRUE);assert(rows[3].fields==21);
 whdLoadTraceBegin(99);whdLoadTraceEnd(99,FALSE);assert(count==4);
 for(i=0;i<300;i++)assert(WLT_CALL(WLT_RENDERER,work(1,TRUE)));
 assert(count==192&&overflow&&!io&&!lines);
 whdLoadTraceWrite();assert(io==3&&footers==1&&lines==197);
 puts("PASS: nested phases, section ownership, wrap, failure, capacity and explicit-only log I/O");
 return 0;
}
'''
    collector=body('whd_load_trace.c')
    if args.state:
        stub += r'''
#define SPARKPAW_WHD_LOAD_STATE
 typedef uint8_t UBYTE;
 struct Custom { UWORD dmaconr,intenar,intreqr; } customState={0x3c0,0x6020,0x20};
 struct CIA { UBYTE ciacra,ciacrb; } ciaState={1,0};
 static ULONG platformFieldCounter(void) { return gfx.VBCounter&0xffffffU; }
 ULONG whdLoadTraceReadCacr(void) { return gfx.VBCounter?1:0; }
'''
        collector=collector.replace('(volatile struct Custom *)0xdff000','&customState').replace('(volatile struct CIA *)0xbfd000','&ciaState')
        driver=driver.replace('lines==197','lines==390')
        driver=driver.replace('assert(rows[0].fast==40000&&rows[0].chip==20000);','''assert(rows[0].fast==40000&&rows[0].chip==20000);
 assert(rows[0].before.cacr==0&&rows[0].after.cacr==1);
 assert(rows[0].before.tod==0&&rows[0].after.tod==12);
 assert(rows[0].before.dma==0x3c0&&rows[0].after.intena==0x6020);
 customState.dmaconr=0x300;ciaState.ciacrb=9;
 assert(rows[0].before.dma==0x3c0&&rows[0].after.crb==0);''')
    c=out/'trace.c';c.write_text(stub+body('whd_load_trace.h')+collector+driver)
    exe=out/'trace'
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',
                    '-fsanitize=address,undefined',str(c),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)

if __name__=='__main__':main()
