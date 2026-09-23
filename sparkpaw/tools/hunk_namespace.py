"""Strict Amiga Hunk OBJECT symbol namespace transform; never changes code.
Only explicit EXT/SYMBOL names are rewritten. Relocations and segment payloads
stay byte-exact. Unsupported records fail closed rather than guessing lengths.
"""
import struct
from pathlib import Path
class HunkError(ValueError):pass
def transform(data,names=None):
 names=names or {};pos=0;out=bytearray();defined=set();refs=set();payloads=[]
 def take(n):
  nonlocal pos
  if n<0 or pos+n>len(data):raise HunkError('truncated object')
  r=data[pos:pos+n];pos+=n;return r
 def word():return struct.unpack('>I',take(4))[0]
 def put(n):out.extend(struct.pack('>I',n))
 def name(n):return take(n*4).rstrip(b'\0').decode('ascii')
 def emit_name(s,kind=0):
  raw=s.encode('ascii');n=(len(raw)+3)//4;put((kind<<24)|n);out.extend(raw.ljust(n*4,b'\0'))
 while pos<len(data):
  tag=word();kind=tag&0x3fffffff;put(tag)
  if kind in (999,1000):
   n=word();put(n);out.extend(take(n*4))
  elif kind in (1001,1002,1009):
   n=word();put(n);p=take(n*4);out.extend(p)
   if kind in (1001,1002):payloads.append((kind,p))
  elif kind==1003:put(word())
  elif kind in (1004,1005,1006,1015,1016,1017):
   while True:
    n=word();put(n)
    if not n:break
    out.extend(take(4*(n+1)))
  elif kind==1007:
   while True:
    h=word()
    if not h:put(0);break
    t=h>>24;s=name(h&0xffffff);emit_name(names.get(s,s),t)
    if t in (1,2,3):defined.add(s);put(word())
    elif t in (129,131,132,133,134,135,136,138,139):
     refs.add(s);n=word();put(n);out.extend(take(n*4))
    elif t==130:
     refs.add(s);put(word());n=word();put(n);out.extend(take(n*4))
    else:raise HunkError(f'unsupported EXT subtype {t}')
  elif kind==1008:
   while True:
    n=word()
    if not n:put(0);break
    s=name(n);emit_name(names.get(s,s));put(word())
  elif kind==1010:pass
  else:raise HunkError(f'unsupported Hunk {kind} at{pos-4}')
 return bytes(out),defined,refs,payloads

def namespace_objects(paths,destination,export='_drownedCampaignRun'):
 objects={str(p):Path(p).read_bytes() for p in paths};defs=set()
 for raw in objects.values():defs.update(transform(raw)[1])
 if export not in defs:raise HunkError('missing module entry')
 names={s:'__drowned_'+s for s in defs if s!=export}
 destination=Path(destination);destination.mkdir(parents=True,exist_ok=True);outputs=[]
 for i,(name,raw) in enumerate(objects.items()):
  updated=transform(raw,names)[0];old=transform(raw);new=transform(updated)
  if old[3]!=new[3]:raise HunkError('segment payload changed')
  if {names.get(s,s) for s in old[1]}!=new[1]:raise HunkError('definition mismatch')
  if {names.get(s,s) for s in old[2]}!=new[2]:raise HunkError('reference mismatch')
  p=destination/(f'{i:02d}-'+Path(name).name);p.write_bytes(updated);outputs.append(p)
 return outputs,names
