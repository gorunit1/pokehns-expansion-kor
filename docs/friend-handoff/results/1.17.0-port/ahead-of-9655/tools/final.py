import json, os, csv
rows=json.load(open('rows.json'))
cls={r['seq']:r for r in json.load(open('classified.json'))}
risk=json.load(open('reorder_risk.json'))
bypr={}
for r in rows: bypr.setdefault(r['pr'],[]).append(r)
byunit={}
for r in rows: byunit.setdefault(r['unit'],[]).append(r)
cand=[r for r in rows if float(r['seq'])>127 and cls[r['seq']]['status']=='OK']
held={r['seq']:'hunk' for r in cand if r['seq'] in risk}
changed=True
while changed:
    changed=False
    for r in cand:
        if r['seq'] in held: continue
        s=float(r['seq'])
        for d in [x.strip() for x in r['deps'].split(',') if x.strip() not in ('','-')]:
            for dr in bypr.get(d,[]):
                if dr['seq'] in held: held[r['seq']]=f"deps #{d}(seq {dr['seq']}) 보류"; changed=True
        if r['seq'] in held: continue
        for u in byunit[r['unit']]:
            if float(u['seq'])<s and u['seq'] in held:
                held[r['seq']]=f"unit 선행 seq {u['seq']} 보류"; changed=True; break
clean=[r for r in cand if r['seq'] not in held]
print('cand',len(cand),'clean',len(clean),'held',len(held), 'held-by-propagation',sum(1 for v in held.values() if v!='hunk'))
for s,v in held.items():
    if v!='hunk': print(' ',s,[r['pr'] for r in cand if r['seq']==s][0],v)
T=set(f for f in [r for r in rows if r['pr']=='9655'][0]['files'] if f.startswith('test/'))
with open('final_list.tsv','w') as f:
    f.write('seq\tpr\tclass\tunit\tdeps\tsize\tkind\tkor\tsave\trom_est\tnote\ttitle\n')
    for r in cand:
        c='clean' if r['seq'] not in held else 'hold'
        note=[]
        if r['seq'] in risk: note.append('hunk:'+';'.join(f"{k}:{','.join(os.path.basename(x) for x in v)}" for k,v in risk[r['seq']].items()))
        elif r['seq'] in held: note.append(held[r['seq']])
        tov=sorted(os.path.basename(x) for x in r['files'] if x in T)
        if tov: note.append('9655테스트겹침:'+','.join(tov))
        f.write('\t'.join([r['seq'],r['pr'],c,r['unit'],r['deps'],r['size'],r['kind'],r['korean_touch'],r['save_impact'],r['rom_est'],' / '.join(note) or '-',r['title']])+'\n')
