"""SPL1: bounded 4 KiB-window LZSS for disk-only bitmap loading.

16-byte big-endian header: magic, decoded size, CRC32, payload size.
MSB-first flag groups: 0 literal, 1 match. Match word: length-3 (4 bits),
distance-1 (12 bits). Overlap is intentional; references before output fail.
"""
import struct,zlib
from collections import defaultdict,deque
HEADER=struct.Struct('>4sIII')
def pack(raw):
 out=bytearray();pos=0;index=defaultdict(deque)
 while pos<len(raw):
  flag_at=len(out);out.append(0);flags=0
  for bit in range(7,-1,-1):
   if pos>=len(raw):break
   q=index[raw[pos:pos+3]]
   while q and pos-q[0]>4096:q.popleft()
   length=0;distance=0
   for prev in reversed(list(q)[-64:]):
    n=0
    while n<18 and pos+n<len(raw) and raw[prev+n]==raw[pos+n]:n+=1
    if n>length:length=n;distance=pos-prev
    if n==18:break
   count=length if length>=3 else 1
   if length>=3:
    flags|=1<<bit;word=((length-3)<<12)|(distance-1);out.extend(struct.pack('>H',word))
   else:out.append(raw[pos])
   for i in range(count):index[raw[pos+i:pos+i+3]].append(pos+i)
   pos+=count
  out[flag_at]=flags
 result=HEADER.pack(b'SPL1',len(raw),zlib.crc32(raw)&0xffffffff,len(out))+out
 assert decode(result)==raw
 return result

def decode(data):
 if len(data)<16:raise ValueError('header')
 magic,size,crc,n=HEADER.unpack_from(data)
 if magic!=b'SPL1' or n!=len(data)-16:raise ValueError('format')
 out=bytearray();at=16
 while len(out)<size:
  if at>=len(data):raise ValueError('flags')
  flags=data[at];at+=1
  for bit in range(7,-1,-1):
   if len(out)==size:break
   if flags&(1<<bit):
    if at+2>len(data):raise ValueError('match')
    word=int.from_bytes(data[at:at+2],'big');at+=2;distance=(word&4095)+1;length=(word>>12)+3
    if distance>len(out) or len(out)+length>size:raise ValueError('reference')
    for _ in range(length):out.append(out[-distance])
   else:
    if at>=len(data):raise ValueError('literal')
    out.append(data[at]);at+=1
 if at!=len(data) or zlib.crc32(out)&0xffffffff!=crc:raise ValueError('integrity')
 return bytes(out)
