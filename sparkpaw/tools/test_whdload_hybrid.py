#!/usr/bin/env python3
"""Exercise actual hybrid reader source with split reads and corrupt streams."""
import argparse
import random
import subprocess
from pathlib import Path

from pack_adf_asset import pack as rle
from pack_disk_asset import pack as lz, pack_delta

ROOT = Path(__file__).resolve().parents[1]


def build_reader(out):
    source = (ROOT / 'src/assets.c').read_text()
    raw = source[source.index('#define RAW_READ_CHUNK'):source.index('\n#ifdef ADF_PACKED_ASSETS')]
    start = source.index('struct PackedReader {')
    end = source.index('\n#endif\n\n#if defined(SPARKPAW_MULTI_ADF)||defined(SPARKPAW_WHD_PACKED)\n/* Reuse', start)
    body = source[start:end]
    start = source.index('UBYTE *assetsLoadDiskData(')
    body += source[start:source.index('\n#endif', start)]
    header = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef uint8_t UBYTE; typedef uint16_t UWORD; typedef uint32_t ULONG;
typedef int32_t LONG; typedef int BOOL; typedef char *STRPTR; typedef FILE *BPTR;
#define SPARKPAW_WHD_PACKED
#ifndef TEST_PACKED_ONLY
#define SPARKPAW_WHD_HYBRID
#endif
#define TRUE 1
#define FALSE 0
#define MODE_OLDFILE 0
#define OFFSET_BEGINNING SEEK_SET
#define OFFSET_CURRENT SEEK_CUR
#define OFFSET_END SEEK_END
#define AllocMem(n,flags) ((void)(flags),malloc(n))
#define FreeMem(p,n) free(p)
#define Open(n,m) fopen(n,"rb")
#define Close(f) fclose(f)
#define Read(f,p,n) ((LONG)fread(p,1,n,f))
#define CopyMem(s,d,n) memcpy(d,s,n)
#ifndef TEST_PACKED_ONLY
static LONG Seek(BPTR f,LONG n,int mode) {
    long before=ftell(f); return fseek(f,n,mode)?-1:(LONG)before;
}
#endif
#include "packed_crc32.h"
static UBYTE diskDecodeWindow[4096];
'''
    driver = r'''
int main(int argc,char **argv) {
    struct PackedReader r; UBYTE actual[333],expected[333];
    FILE *reference; int n,part=0; BOOL ok=TRUE;
    assert(argc==4); reference=fopen(argv[2],"rb"); assert(reference);
    if(!packedOpen(argv[1],&r)) { assert(!atoi(argv[3])); fclose(reference); return 0; }
    while((n=fread(expected,1,++part%3==0?12:part%3==1?97:333,reference))>0) {
        if(!packedRead(&r,actual,n)||memcmp(actual,expected,n)) {ok=FALSE;break;}
    }
    ok=packedClose(&r,ok); assert(ok==atoi(argv[3]));
    if(ok) {
        ULONG size; UBYTE *all; long length=ftell(reference);
        all=assetsLoadDiskData(argv[1],0,&size);
        if(length>0&&length<=512L*1024L) {
            long i; assert(all&&size==(ULONG)length); rewind(reference);
            for(i=0;i<length;i++) assert(all[i]==fgetc(reference)); free(all);
        } else assert(!all&&!size);
    }
    fclose(reference); return 0;
}
'''
    c = out / 'reader.c'
    c.write_text(header + raw + body + driver)
    for name, flags in [('reader', []), ('packed-reader', ['-DTEST_PACKED_ONLY']),
                        ('fast-reader', ['-DTEST_PACKED_ONLY', '-DSPARKPAW_WHD_FAST_DECODE'])]:
        # Legacy mode does not use the raw helper in this extracted harness.
        subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror',
                        '-Wno-unused-function', '-fsanitize=address,undefined',
                        '-I'+str(ROOT/'src'), *flags, str(c), '-o', str(out/name)], check=True)


def check(reader, stored, original, valid):
    subprocess.run([str(reader), str(stored), str(original), str(int(valid))], check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir', required=True, type=Path)
    args = parser.parse_args()
    out = args.output_dir.resolve()
    out.mkdir(parents=True, exist_ok=False)
    build_reader(out)
    rng = random.Random(68030)
    samples = [b'A', b'raw', b'A'*9000, bytes(range(256))*40,
               bytes(rng.randrange(256) for _ in range(8000))]
    samples += [(ROOT/'assets/runtime'/n).read_bytes() for n in
                ['intro1.spbm', 'readymenu.spbm', 'sparkpaw-sprites4.spbm']]
    cases = 0
    for raw in samples:
        reference, stored = out/'reference', out/'stored'
        reference.write_bytes(raw)
        stored.write_bytes(raw)
        check(out/'reader', stored, reference, True); cases += 1
        stored.write_bytes(raw[:-1])
        check(out/'reader', stored, reference, False); cases += 1
        for encoder in (rle, lz, pack_delta):
            encoded = encoder(raw)
            for name in ('reader', 'packed-reader', 'fast-reader'):
                stored.write_bytes(encoded)
                check(out/name, stored, reference, True); cases += 1
                corrupt = bytearray(encoded); corrupt[8] ^= 1
                stored.write_bytes(corrupt)
                check(out/name, stored, reference, False); cases += 1
                stored.write_bytes(encoded[:-1])
                check(out/name, stored, reference, False); cases += 1
    # Invalid backreference, declared output too short/long, and trailing input.
    import struct, zlib
    reference.write_bytes(b'abc')
    malformed = [struct.pack('>4sIII', b'SPL1', 3, zlib.crc32(b'abc'), 3)+b'\x80\0\0']
    encoded = lz(b'abc')
    for size in (2, 4):
        bad = bytearray(encoded); struct.pack_into('>I', bad, 4, size)
        malformed.append(bytes(bad))
    bad = bytearray(encoded+b'X'); struct.pack_into('>I', bad, 12, len(bad)-16)
    malformed.append(bytes(bad))
    for data in malformed:
        stored.write_bytes(data)
        for name in ('packed-reader', 'fast-reader'):
            check(out/name, stored, reference, False); cases += 1
    print('Hybrid/legacy reader split-read and integrity cases passed:', cases)


if __name__ == '__main__':
    main()
