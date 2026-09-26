"""Host-only LZSS parsing improvement; unchanged bounded SPL1 wire format."""
from collections import defaultdict,deque
import struct,zlib
from pack_disk_asset import HEADER,decode

def pack(raw):
    size=len(raw);lengths=bytearray(size);distances=[0]*size
    index=defaultdict(deque)
    for pos in range(size):
        q=index[raw[pos:pos+3]]
        while q and pos-q[0]>4096:q.popleft()
        best=0;distance=0
        for prev in reversed(list(q)[-64:]):
            n=0
            while n<18 and pos+n<size and raw[prev+n]==raw[pos+n]:n+=1
            if n>best:best=n;distance=pos-prev
            if best==18:break
        if best>=3:lengths[pos]=best;distances[pos]=distance
        q.append(pos)
    # Literal=8 data bits+1 flag bit; match=16+1. Final group pads <8 bits.
    cost=[0]*(size+1);take=bytearray(size)
    for pos in range(size-1,-1,-1):
        best=9+cost[pos+1];chosen=1
        for n in range(3,lengths[pos]+1):
            value=17+cost[pos+n]
            if value<best:best=value;chosen=n
        cost[pos]=best;take[pos]=chosen
    out=bytearray();pos=0
    while pos<size:
        at=len(out);out.append(0);flags=0
        for bit in range(7,-1,-1):
            if pos>=size:break
            n=take[pos]
            if n==1:out.append(raw[pos])
            else:
                flags|=1<<bit;out.extend(struct.pack('>H',((n-3)<<12)|(distances[pos]-1)))
            pos+=n
        out[at]=flags
    data=HEADER.pack(b'SPL1',size,zlib.crc32(raw)&0xffffffff,len(out))+out
    assert decode(data)==raw
    return data

def pack_delta(raw):
    differences=bytes((v-(raw[i-1] if i else 0))&255 for i,v in enumerate(raw))
    data=pack(differences)
    result=HEADER.pack(b'SPD1',len(raw),zlib.crc32(raw)&0xffffffff,len(data)-16)+data[16:]
    assert decode(result)==raw
    return result
