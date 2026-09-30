import json, os, collections
rows=json.load(open('classified.json'))
allrows=json.load(open('rows.json'))
r127=[r for r in allrows if r['seq']=='127'][0]
r127['status']='9655'; r127['s']=127.0
deferred=[r127]+[r for r in rows if r['status'] in ('file','dep9655','unit','blocked')]
out=[]
for c in rows:
    if c['status']!='OK': continue
    cf=set(c['files'])
    src=collections.defaultdict(list); tst=collections.defaultdict(list)
    for d in deferred:
        if d['s']>=c['s']: continue
        for f in cf & set(d['files']):
            (tst if f.startswith('test/') else src)[f].append(f"{d['seq']}#{d['pr']}")
    c['ov_src']=dict(src); c['ov_test']=dict(tst)
    out.append(c)
json.dump(out,open('cand_overlap.json','w'),ensure_ascii=False,indent=0)
n0=sum(1 for c in out if not c['ov_src'] and not c['ov_test'])
print('candidates',len(out),'no overlap',n0,'src overlap',sum(1 for c in out if c['ov_src']),'test-only overlap',sum(1 for c in out if not c['ov_src'] and c['ov_test']))
for c in out:
    if c['ov_src']:
        print(c['seq'],c['pr'],c['size'],'|', '; '.join(f"{os.path.basename(f)}<{','.join(v[:4])}{'…' if len(v)>4 else ''}>" for f,v in list(c['ov_src'].items())[:5]), ('(+%d files)'%(len(c['ov_src'])-5) if len(c['ov_src'])>5 else ''))
