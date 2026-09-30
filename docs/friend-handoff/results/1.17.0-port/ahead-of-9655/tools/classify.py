import json, os, re
rows=json.load(open('rows.json'))
for r in rows: r['s']=float(r['seq'])
bypr={}
for r in rows: bypr.setdefault(r['pr'],[]).append(r)
byunit={}
for r in rows: byunit.setdefault(r['unit'],[]).append(r)
def deps(r): return [d.strip() for d in r['deps'].split(',') if d.strip() and d.strip()!='-']
# already applied rows (from results docs)
APPLIED={'138':'#9707','163':'#9796','176':'#9864','250':'#10281(HnS 동등)','265':'#10345','319':'#10573','321':'#10589','329':'#10647','330':'#10648','291':'#10429(8280caf163 HnS 구현)'}
T9655_TESTS=None
tgt=[r for r in rows if r['pr']=='9655'][0]
T9655_TESTS=set(f for f in tgt['files'] if f.startswith('test/'))
after=[r for r in rows if r['s']>127]
# 1) dependency closure on 9655 (incl. same unit)
dep9655=set()
changed=True
unit9655=tgt['unit']
while changed:
    changed=False
    for r in after:
        if r['seq'] in dep9655: continue
        why=None
        if r['unit']==unit9655: why='same unit U-battlemsg-9655'
        for d in deps(r):
            if d=='9655': why='deps 9655'
            for dr in bypr.get(d,[]):
                if dr['seq'] in dep9655: why=f'deps {d}(seq {dr["seq"]})→9655'
        if why: dep9655.add(r['seq']); r['why_dep']=why; changed=True
# 2) file block
fileblock={r['seq'] for r in after if r['blockhits']}
# unit propagation: if any member of a unit is excluded, whole unit excluded
excl=set(dep9655)|fileblock
changed=True
while changed:
    changed=False
    for r in after:
        if r['seq'] in excl or r['seq'] in APPLIED: continue
        u=[x for x in byunit[r['unit']] if x['s']>127 and x['s']<r['s'] and x['seq'] in excl]
        if u: excl.add(r['seq']); r['why_unit']=f"unit {r['unit']} member seq {u[0]['seq']} excluded"; changed=True
# 3) blocked by deps on excluded rows (not applied, not before 127)
blocked={}
changed=True
while changed:
    changed=False
    for r in after:
        if r['seq'] in excl or r['seq'] in blocked or r['seq'] in APPLIED: continue
        for d in deps(r):
            for dr in bypr.get(d,[]):
                if dr['s']>127 and (dr['seq'] in excl or dr['seq'] in blocked):
                    blocked[r['seq']]=f"deps #{d}(seq {dr['seq']})"; changed=True
        if r['seq'] not in blocked:
            u=[x for x in byunit[r['unit']] if x['s']>127 and x['s']<r['s'] and x['seq'] in blocked]
            if u: blocked[r['seq']]=f"unit {r['unit']} earlier member seq {u[0]['seq']} blocked"; changed=True
out=[]
for r in after:
    s=r['seq']
    if s in APPLIED: st='applied'
    elif s in dep9655: st='dep9655'
    elif s in fileblock: st='file'
    elif s in excl: st='unit'
    elif s in blocked: st='blocked'
    else: st='OK'
    r['status']=st
    r['testov']=sorted(f for f in r['files'] if f in T9655_TESTS)
    out.append(r)
import collections
print(collections.Counter(r['status'] for r in out))
json.dump(out,open('classified.json','w'),ensure_ascii=False,indent=0)
with open('candidates.tsv','w') as f:
    f.write('seq\tpr\tunit\tdeps\tsize\tkind\tkor\tsave\tgroup\ttest_overlap_9655\tnfiles\ttitle\n')
    for r in out:
        if r['status']=='OK':
            f.write('\t'.join([r['seq'],r['pr'],r['unit'],r['deps'],r['size'],r['kind'],r['korean_touch'],r['save_impact'],r['group'],','.join(os.path.basename(x) for x in r['testov']) or '-',str(len(r['files'])),r['title']])+'\n')
with open('excluded.tsv','w') as f:
    for r in out:
        if r['status']!='OK':
            f.write('\t'.join([r['seq'],r['pr'],r['status'],r.get('why_dep','') or r.get('why_unit','') or blocked.get(r['seq'],'') or ','.join(os.path.basename(x) for x in r['blockhits']),r['title']])+'\n')
