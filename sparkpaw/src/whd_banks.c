#include "whd_load_trace.h"
/* WHDLoad-only, bounded phase storage. Large DOS Read calls take kickfs's
   direct resload_LoadFileOffset branch. No slave pointer/ABI injection and
   no change to the ordinary HD/ADF MaxTransfer-safe reader. The paired slave
   excludes files from its cache: these bytes have exactly one cache owner. */
#include "whd_banks.h"
#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <string.h>

#define BANK_LIMIT (2UL*1024*1024)
#define ENTRY_BYTES 40UL
#define HANDLE_COUNT 4
struct Bank { UBYTE *data; ULONG size,count; };
struct BankFile { const UBYTE *data; ULONG size,pos; };
static struct Bank banks[3]; /* common, intro, current level */
static struct BankFile handles[HANDLE_COUNT];
static UWORD currentSection;

static ULONG be32(const UBYTE *p)
{ return ((ULONG)p[0]<<24)|((ULONG)p[1]<<16)|((ULONG)p[2]<<8)|p[3]; }

static BOOL idle(void)
{
    UWORD i;
    for(i=0;i<HANDLE_COUNT;i++) if(handles[i].data) return FALSE;
    return TRUE;
}

static void releaseBank(struct Bank *bank)
{
    if(bank->data) FreeMem(bank->data,bank->size);
    memset(bank,0,sizeof(*bank));
}

static BOOL loadBank(struct Bank *bank,const char *name)
{
    BPTR file;
    LONG length;
    ULONG i,end;
    const UBYTE *entry;
    if(bank->data) return FALSE;
    file=Open((STRPTR)name,MODE_OLDFILE);
    if(!file) return FALSE;
    if(Seek(file,0,OFFSET_END)<0 ||
       (length=Seek(file,0,OFFSET_BEGINNING))<16 ||
       (ULONG)length>BANK_LIMIT) { Close(file); return FALSE; }
    bank->size=(ULONG)length;
    bank->data=AllocMem(bank->size,MEMF_FAST);
    if(!bank->data) { Close(file); releaseBank(bank); return FALSE; }
    /* Fresh handle; no small header read that could seed kickfs IOCACHE. */
    if(Read(file,bank->data,length)!=length) {
        Close(file); releaseBank(bank); return FALSE;
    }
    Close(file);
    if(memcmp(bank->data,"SPB1",4) || be32(bank->data+8)!=bank->size ||
       be32(bank->data+12)!=0) goto invalid;
    bank->count=be32(bank->data+4);
    if(!bank->count || bank->count>96 ||
       bank->count>(bank->size-16)/ENTRY_BYTES) goto invalid;
    end=16+bank->count*ENTRY_BYTES;
    for(i=0;i<bank->count;i++) {
        entry=bank->data+16+i*ENTRY_BYTES;
        if(!entry[0] || !memchr(entry,0,32) ||
           be32(entry+32)!=end || !be32(entry+36) ||
           be32(entry+36)>bank->size-end) goto invalid;
        if(i && strcmp((const char *)(entry-ENTRY_BYTES),(const char *)entry)>=0)
            goto invalid;
        end+=be32(entry+36);
    }
    if(end!=bank->size) goto invalid;
    return TRUE;
invalid:
    releaseBank(bank); return FALSE;
}

BOOL whdBanksSelect(UWORD section)
{
    const char *name;
    if(!idle() || section<1 || section>3) return FALSE;
#ifdef SPARKPAW_WHD_LOAD_TRACE
    whdLoadTraceSection(section);
#endif
    if(section==currentSection) return TRUE;
    name=section==1?"PROGDIR:level1.spb":
         section==2?"PROGDIR:level2.spb":"PROGDIR:level3.spb";
    releaseBank(&banks[2]); currentSection=0;
    if(!loadBank(&banks[2],name)) return FALSE;
    currentSection=section; return TRUE;
}

BOOL whdBanksBoot(void)
{
    if(!idle() || banks[0].data || banks[1].data || banks[2].data) return FALSE;
    if(loadBank(&banks[0],"PROGDIR:common.spb") &&
       loadBank(&banks[1],"PROGDIR:intro.spb") && whdBanksSelect(1)) return TRUE;
    whdBanksClose(); return FALSE;
}

BOOL whdBanksFinishIntro(void)
{
    if(!idle()) return FALSE;
    releaseBank(&banks[1]); return TRUE;
}

void whdBanksClose(void)
{
    UWORD i;
    /* Call only after consumers have retired; never invalidate a live reader. */
    if(!idle()) return;
    for(i=0;i<3;i++) releaseBank(&banks[i]);
    currentSection=0;
}

BPTR whdBankOpen(const char *name,LONG mode)
{
    UWORD slot,b;
    ULONG i;
    const UBYTE *entry;
    const char *prefix="PROGDIR:assets/runtime/";
    if(mode!=MODE_OLDFILE || strncmp(name,prefix,strlen(prefix))) return 0;
    name+=strlen(prefix);
    for(slot=0;slot<HANDLE_COUNT;slot++) if(!handles[slot].data) break;
    if(slot==HANDLE_COUNT) return 0;
    for(b=0;b<3;b++) for(i=0;i<banks[b].count;i++) {
        entry=banks[b].data+16+i*ENTRY_BYTES;
        if(!strcmp(name,(const char *)entry)) {
            handles[slot].data=banks[b].data+be32(entry+32);
            handles[slot].size=be32(entry+36); handles[slot].pos=0;
            return (BPTR)(slot+1);
        }
    }
    return 0;
}

static struct BankFile *getHandle(BPTR file)
{
    if(file<1 || file>HANDLE_COUNT || !handles[file-1].data) return NULL;
    return &handles[file-1];
}

LONG whdBankRead(BPTR file,void *target,LONG size)
{
    struct BankFile *f=getHandle(file);
    ULONG count;
    if(!f || size<0 || (!target && size)) return -1;
    count=(ULONG)size;
    if(count>f->size-f->pos) count=f->size-f->pos;
    if(count) CopyMem((APTR)(f->data+f->pos),target,count);
    f->pos+=count; return (LONG)count;
}

LONG whdBankSeek(BPTR file,LONG offset,LONG mode)
{
    struct BankFile *f=getHandle(file);
    LONG base,next,old;
    if(!f) return -1;
    if(mode==OFFSET_BEGINNING) base=0;
    else if(mode==OFFSET_CURRENT) base=(LONG)f->pos;
    else if(mode==OFFSET_END) base=(LONG)f->size;
    else return -1;
    /* All lengths <=2 MiB; check before adding a hostile signed offset. */
    if(offset < -base || offset > (LONG)f->size-base) return -1;
    next=base+offset; old=(LONG)f->pos; f->pos=(ULONG)next; return old;
}

LONG whdBankClose(BPTR file)
{
    struct BankFile *f=getHandle(file);
    if(!f) return 0;
    memset(f,0,sizeof(*f)); return -1;
}
