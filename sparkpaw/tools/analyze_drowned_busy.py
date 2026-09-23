#!/usr/bin/env python3
"""Inspect sparse raw checkpoints; estimates include IRQ/DMA/observer time.

312-line PAL is assumed. TOD and raster must represent the same frame away
from the guarded edge. Never silently repair negative/inconsistent deltas.
This cannot isolate pure CPU execution or distinguish each deferred DMA wait.
"""
import argparse,json,statistics
from pathlib import Path
PAIRS={'empty_probe':(0,18),'game':(18,3),'ai':(1,2),'sprite_hud_copper':(3,4),
 'restores_all':(4,7),'enemy_restore':(5,6),'water_build':(7,8),
 'patch_build':(8,9),'ring_and_dynamic_sync':(9,10),'patch_sync':(10,11),
 'other_draw':(11,12),'enemy_draw':(12,13),'projectile_draw':(13,14),
 'final_wait_history':(14,15),'bob_total':(4,15),'publish_wait':(15,16),
 'rear_update':(16,17)}
def elapsed(a,b):
 if not a['valid'] or not b['valid']:return None
 if not(0<=a['line']<312 and 0<=b['line']<312):return None
 fields=(b['field']-a['field'])&0xffffff
 if fields==0:
  return b['line']-a['line'] if b['line']>=a['line'] else None
 if not(8<=a['line']<=300 and 8<=b['line']<=300):return None
 if fields>8:return None
 lines=fields*312+b['line']-a['line']
 return lines if lines>=0 else None

def analyze(text):
 if not text.endswith('post_run=complete\n'):raise ValueError('Incomplete diagnostic save')
 if 'variant=busy_sparse_discovery\n' not in text:raise ValueError('Not a busy discovery log')
 samples={};header=None
 for line in text.splitlines():
  if not line.startswith(('busy_sample=','busy_count=','busy_stamp=','busy_v1 ')):continue
  if line.startswith('busy_v1 '):header={k:int(v) for k,v in (p.split('=') for p in line.split()[1:])};continue
  pairs=dict(p.split('=') for p in line.split());tag=next(iter(pairs));i=int(pairs.pop(tag));values={k:int(v) for k,v in pairs.items()}
  if tag=='busy_sample':
   if i in samples:raise ValueError('Duplicate sample')
   samples[i]={'meta':values,'stamp':{}}
  elif tag=='busy_count':samples[i]['count']=values
  else:
   mark=values.pop('mark')
   if mark in samples[i]['stamp']:raise ValueError('Duplicate checkpoint')
   samples[i]['stamp'][mark]=values
 if header is None or header['samples']!=len(samples) or len(samples)>header['capacity']:raise ValueError('Invalid sample count')
 result={'clock':'conditional 312-line PAL estimates; IRQ/DMA and observer included, not pure CPU','header':header,'groups':{},'invalid_pairs':0,'reset_samples':0,'incomplete_samples':0}
 for s in samples.values():
  if s['meta']['after']<s['meta']['step']:result['reset_samples']+=1;continue
  if 17 not in s['stamp']:result['incomplete_samples']+=1;continue
  count=s['count'];actors=count['e0']+count['e1']+count['e2']
  for group in (str(s['meta']['group']),str(s['meta']['group'])+f'_enemies_{actors}'):
   g=result['groups'].setdefault(group,{'samples':0,'scopes':{},'draw_words':[],'restore_words':[],'shots':[]})
   g['samples']+=1
   for key in ('draw_words','restore_words','shots'):g[key].append(count[key])
   for name,(first,last) in PAIRS.items():
    if first not in s['stamp'] or last not in s['stamp']:continue
    value=elapsed(s['stamp'][first],s['stamp'][last])
    if value is None:
     if group==str(s['meta']['group']):result['invalid_pairs']+=1
     continue
    g['scopes'].setdefault(name,[]).append(value)
 for g in result['groups'].values():
  for k,values in g['scopes'].items():
   ordered=sorted(values);g['scopes'][k]={'n':len(values),'median_lines':statistics.median(values),'max_lines':max(values),'mean_lines':sum(values)/len(values)}
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('log',type=Path);a=p.parse_args()
 print(json.dumps(analyze(a.log.read_text()),indent=2))
