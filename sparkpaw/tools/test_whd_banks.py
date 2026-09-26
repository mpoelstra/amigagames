"""Sanitize actual bank backend: lifetime, bounds, I/O count and failure paths."""
import argparse
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]

STUBS = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
typedef uint8_t UBYTE; typedef uint16_t UWORD; typedef uint32_t ULONG;
typedef int32_t LONG; typedef int BOOL; typedef int32_t BPTR;
typedef void *APTR; typedef char *STRPTR;
#define TRUE 1
#define FALSE 0
#define MEMF_FAST 4
#define MODE_OLDFILE 1005
#define OFFSET_BEGINNING -1
#define OFFSET_CURRENT 0
#define OFFSET_END 1
static FILE *disk;
static unsigned reads,allocations,failAlloc,shortRead;
static size_t resident,peak;
static const char *root;
static BPTR Open(STRPTR name,LONG mode) {
    char path[4096]; assert(mode==MODE_OLDFILE&&!disk);
    assert(!strncmp(name,"PROGDIR:",8));
    snprintf(path,sizeof(path),"%s/%s",root,name+8);
    disk=fopen(path,"rb"); return disk?100:0;
}
static LONG Seek(BPTR f,LONG offset,LONG mode) {
    long old; assert(f==100&&disk);old=ftell(disk);
    return fseek(disk,offset,mode<0?SEEK_SET:mode>0?SEEK_END:SEEK_CUR)?-1:(LONG)old;
}
static LONG Read(BPTR f,void *p,LONG n) {
    assert(f==100&&disk&&n>=4096); reads++;
    return (LONG)fread(p,1,shortRead?n-1:n,disk);
}
static LONG Close(BPTR f) { assert(f==100&&disk);fclose(disk);disk=NULL;return -1; }
static void *AllocMem(ULONG n,ULONG flags) {
    void *p;assert(flags==MEMF_FAST);if(failAlloc)return NULL;
    p=malloc(n);assert(p);allocations++;resident+=n;if(resident>peak)peak=resident;return p;
}
static void FreeMem(void *p,ULONG n) { assert(resident>=n&&allocations);resident-=n;allocations--;free(p); }
#define CopyMem(s,d,n) memcpy(d,s,n)
void whdBanksClose(void);
'''

DRIVER = r'''
static void verifyBank(unsigned b) {
    ULONG i;unsigned old=reads;
    for(i=0;i<banks[b].count;i++) {
        const UBYTE *entry=banks[b].data+16+i*40;
        const UBYTE *expected=banks[b].data+be32(entry+32);
        char name[128]; UBYTE bytes[997];ULONG at=0,size=be32(entry+36);BPTR f;
        snprintf(name,sizeof(name),"PROGDIR:assets/runtime/%s",entry);
        f=whdBankOpen(name,MODE_OLDFILE);assert(f);
        assert(!whdBanksSelect(2)&&!whdBanksFinishIntro());
        assert(whdBankSeek(f,INT_MIN,OFFSET_CURRENT)==-1);
        assert(whdBankSeek(f,INT_MAX,OFFSET_CURRENT)==-1);
        assert(whdBankSeek(f,0,OFFSET_END)==0);
        assert(whdBankRead(f,bytes,1)==0);
        assert(whdBankSeek(f,0,OFFSET_BEGINNING)==(LONG)size);
        assert(whdBankRead(f,bytes,-1)==-1);
        while(at<size) {
            LONG want=(size-at)>sizeof(bytes)?sizeof(bytes):size-at;
            assert(whdBankRead(f,bytes,want)==want);
            assert(!memcmp(bytes,expected+at,want));at+=want;
        }
        assert(whdBankClose(f)==-1);assert(!whdBankClose(f));
        assert(whdBankRead(f,bytes,1)==-1);
    }
    assert(reads==old); /* No hidden disk I/O from virtual asset reads. */
}
int main(int argc,char **argv) {
    unsigned i,old;BPTR f[4];char name[128];
    assert(argc==3);root=argv[1];
    if(atoi(argv[2])) { assert(!whdBanksBoot());assert(!resident&&!disk);return 0; }
    failAlloc=1;assert(!whdBanksBoot());assert(!resident&&!disk);failAlloc=0;
    shortRead=1;assert(!whdBanksBoot());assert(!resident&&!disk);shortRead=0;
    old=reads;assert(whdBanksBoot());assert(reads==old+3);
    assert(!whdBanksBoot());verifyBank(0);verifyBank(1);verifyBank(2);
    snprintf(name,sizeof(name),"PROGDIR:assets/runtime/%s",banks[0].data+16);
    for(i=0;i<4;i++) { f[i]=whdBankOpen(name,MODE_OLDFILE);assert(f[i]); }
    assert(!whdBankOpen(name,MODE_OLDFILE));
    for(i=0;i<4;i++)assert(whdBankClose(f[i]));
    assert(!whdBankOpen("PROGDIR:assets/runtime/missing",MODE_OLDFILE));
    assert(!whdBankOpen("elsewhere",MODE_OLDFILE));
    assert(!whdBanksSelect(0)&&!whdBanksSelect(4));
    old=reads;assert(whdBanksSelect(1));assert(old==reads);
    assert(whdBanksFinishIntro());assert(!banks[1].data);
    assert(!whdBankOpen("PROGDIR:assets/runtime/intro1.spr1",MODE_OLDFILE));
    for(i=2;i<=3;i++) { old=reads;assert(whdBanksSelect(i));assert(reads==old+1);verifyBank(0);verifyBank(2); }
    old=reads;assert(whdBanksSelect(1));assert(reads==old+1);verifyBank(2);
    whdBanksClose();assert(!resident&&!allocations&&!disk);
    assert(whdBanksBoot());whdBanksClose();assert(!resident);
    printf("bank lifecycle verified; peak source buffers=%zu; disk Read calls=%u\n",peak,reads);
    return 0;
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', required=True, type=Path)
    args = parser.parse_args()
    build = args.build_dir.resolve()
    manifest = json.loads((build/'banks-manifest.json').read_text())
    data = Path(manifest['stage'])/'data'
    out = build/'bank-tests'
    out.mkdir(exist_ok=False)
    body = '\n'.join(line for line in (ROOT/'src/whd_banks.c').read_text().splitlines()
                     if not line.startswith('#include'))
    c = out/'backend.c'
    c.write_text(STUBS+body+DRIVER)
    exe = out/'backend'
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',
                    '-fsanitize=address,undefined',str(c),'-o',str(exe)],check=True)
    subprocess.run([str(exe),str(data),'0'],check=True)
    raw = (data/'common.spb').read_bytes()
    cases = [raw[:-1],b'FAIL'+raw[4:]]
    for offset,value in [(4,97),(8,len(raw)+1),(12,1),(48,0),(52,0),(52,0xffffffff)]:
        corrupt=bytearray(raw);corrupt[offset:offset+4]=value.to_bytes(4,'big');cases.append(corrupt)
    corrupt=bytearray(raw);corrupt[16:48]=b'X'*32;cases.append(corrupt)
    # Duplicate/out-of-order directory name must fail before any consumer.
    corrupt=bytearray(raw);corrupt[56:88]=corrupt[16:48];cases.append(corrupt)
    bad=out/'malformed';bad.mkdir()
    for body in cases:
        (bad/'common.spb').write_bytes(body)
        subprocess.run([str(exe),str(bad),'1'],check=True)
    print(f'PASS: actual C backend, all staged entries, lifecycle/faults and {len(cases)} malformed containers')


if __name__=='__main__':
    main()
