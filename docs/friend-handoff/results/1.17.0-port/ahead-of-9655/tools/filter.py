import csv, re, subprocess, sys, os, json
REPO='/home/hjm0725/pokehns-expansion-kor'
UP='/home/hjm0725/pokeemerald-expansion-upstream'
D=REPO+'/docs/friend-handoff/results/1.17.0-sync-plan'
rows=[r for r in csv.DictReader(open(D+'/port_sequence.tsv'),delimiter='\t')]
for r in rows: r['s']=float(r['seq'])
master={r['pr']:r for r in csv.DictReader(open(D+'/all_prs_master.tsv'),delimiter='\t')}
bypr={}
for r in rows: bypr.setdefault(r['pr'],[]).append(r)
BLOCK=['battle_message.c','battle_string_ids.h','battle_scripts_1.s','battle_scripts_2.s',
 'battle_script_commands.c','battle_script_commands.h','battle_scripts.h','battle_util.c',
 'battle_hold_effects.c','battle_end_turn.c','pokemon.c']
def upfiles(pr, commits):
    fs=set()
    for c in commits.split(','):
        c=c.strip()
        if not c or c=='-': continue
        out=subprocess.run(['git','-C',UP,'show','--name-only','--format=','-M',c],capture_output=True,text=True).stdout
        fs.update(x for x in out.split('\n') if x)
    return fs
res=[]
for r in rows:
    m=master.get(r['pr'])
    commits=m['commits'] if m else ''
    files=set(m['files'].split()) if m else set()
    gf=upfiles(r['pr'],commits.replace(' ',',')) if commits else set()
    r['files']=files|gf
    r['files_note']='' if (not gf or gf==files) else 'master_vs_git_diff'
    r['commits']=commits
    r['blockhits']=sorted(f for f in r['files'] if os.path.basename(f) in BLOCK)
json.dump([{k:(sorted(v) if isinstance(v,set) else v) for k,v in r.items()} for r in rows],open('rows.json','w'),ensure_ascii=False,indent=0)
