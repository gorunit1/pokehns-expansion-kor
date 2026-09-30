import json, os, re, subprocess, collections
from concurrent.futures import ThreadPoolExecutor
UP='/home/hjm0725/pokeemerald-expansion-upstream'
rows=json.load(open('rows.json'))
cls={r['seq']:r for r in json.load(open('classified.json'))}
def git(*a):
    return subprocess.run(['git','-C',UP]+list(a),capture_output=True,text=True).stdout
def full(h):
    return git('rev-parse',h+'^{commit}').strip()
# commit -> row
c2row={}
for r in rows:
    for c in r['commits'].replace(',',' ').split():
        if c and c!='-':
            f=full(c)
            if f: c2row[f]=r
status={}
for r in rows:
    s=float(r['seq'])
    if r['seq']=='127': status[r['seq']]='9655'
    elif s>127: status[r['seq']]=cls[r['seq']]['status']
    else: status[r['seq']]='done'
DEFER={'9655','file','dep9655','unit','blocked'}
def hunks(c):
    out=git('diff','-U3','--no-renames',c+'^',c)
    res=collections.defaultdict(list); cur=None
    for line in out.split('\n'):
        if line.startswith('--- '):
            p=line[4:]; cur=None if p=='/dev/null' else p[2:]
        elif line.startswith('@@') and cur:
            m=re.match(r'@@ -(\d+)(?:,(\d+))? ',line)
            st=int(m.group(1)); n=int(m.group(2) if m.group(2) is not None else 1)
            if n>0: res[cur].append((st,n))
    return res
def analyze(r):
    hits=collections.defaultdict(set)
    for c in r['commits'].replace(',',' ').split():
        if not c or c=='-': continue
        fc=full(c)
        for f,hs in hunks(fc).items():
            args=['blame','--porcelain']
            for st,n in hs: args+=['-L',f'{st},+{n}']
            args+=[fc+'^','--',f]
            out=git(*args)
            for line in out.split('\n'):
                m=re.match(r'^([0-9a-f]{40}) \d+ \d+',line)
                if m and m.group(1) in c2row:
                    d=c2row[m.group(1)]
                    if d['seq']!=r['seq'] and status.get(d['seq']) in DEFER:
                        hits[f"{d['seq']}#{d['pr']}({status[d['seq']]})"].add(f)
    return r['seq'],{k:sorted(v) for k,v in hits.items()}
cands=[r for r in rows if float(r['seq'])>127 and cls[r['seq']]['status']=='OK']
with ThreadPoolExecutor(8) as ex:
    res=dict(ex.map(analyze,cands))
json.dump(res,open('blame_hits.json','w'),ensure_ascii=False,indent=1)
n=sum(1 for v in res.values() if v)
print('cands',len(cands),'with hunk-level dependency on deferred rows',n)
for r in cands:
    v=res[r['seq']]
    if v: print(r['seq'],r['pr'],r['size'],'|','; '.join(f"{k}:{','.join(os.path.basename(x) for x in fs)}" for k,fs in v.items()))
# reorder risk: only deferred rows sequenced before the candidate; docs/*.md overlaps ignored
risk={}
for s,v in res.items():
    ks={k:[f for f in fs if not (f.startswith('docs/') or f.endswith('.md'))] for k,fs in v.items() if float(k.split('#')[0])<float(s)}
    ks={k:fs for k,fs in ks.items() if fs}
    if ks: risk[s]=ks
json.dump(risk,open('reorder_risk.json','w'),ensure_ascii=False,indent=1)
