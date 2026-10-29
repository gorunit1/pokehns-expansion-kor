#!/usr/bin/env python3
# usage: python3 dev_scripts/hns_verify/port/wincheck.py   (from the HnS repo root)
#   Since seq 508 #9892 InitWindows() asserts when the template array lacks the DUMMY_WIN_TEMPLATE terminator.
#   For every InitWindows(X) call in src/, finds the definition of X and prints OK/BAD from its last element
#   (DUMMY_WIN_TEMPLATE or .bg = 0xFF); 2D arrays are checked row by row. MANUAL = argument is not a plain
#   array name (check by hand). A last element of "#endif" means the terminator is under #if (check the condition;
#   battle_dome.c/roulette.c use UBFIX, which include/config/general.h defines).
import re,glob,sys,os
files=glob.glob('src/**/*.c',recursive=True)+glob.glob('src/**/*.h',recursive=True)
text={f:open(f,encoding='utf-8',errors='replace').read() for f in files}
def strip_comments(s):
    s=re.sub(r'/\*.*?\*/','',s,flags=re.S); return re.sub(r'//[^\n]*','',s)
clean={f:strip_comments(t) for f,t in text.items()}
def find_def(name,prefer):
    pat=re.compile(r'WindowTemplate\s+'+re.escape(name)+r'\s*\[[^\]]*\](\s*\[[^\]]*\])?\s*=\s*\{')
    order=[prefer]+[f for f in clean if f!=prefer]
    for f in order:
        m=pat.search(clean[f])
        if m: return f,m
    return None,None
def body_from(s,start):
    depth=0;i=start
    while True:
        c=s[i]
        if c=='{': depth+=1
        elif c=='}':
            depth-=1
            if depth==0: return s[start+1:i]
        i+=1
def top_elems(b):
    out=[];depth=0;cur=''
    for c in b:
        if c in '{(': depth+=1
        if c in '})': depth-=1
        if c==',' and depth==0: out.append(cur.strip()); cur=''
        else: cur+=c
    if cur.strip(): out.append(cur.strip())
    return out
def is_term(e):
    e=re.sub(r'\s+',' ',e)
    return 'DUMMY_WIN_TEMPLATE' in e or re.search(r'\.bg\s*=\s*0xFF',e) or re.match(r'\{ ?0xFF',e) or re.search(r'\.bg\s*=\s*255',e)
for f,s in clean.items():
    for m in re.finditer(r'\bInitWindows\s*\(([^;]*?)\)\s*[;)\]]',s):
        arg=m.group(1).strip()
        line=s[:m.start()].count('\n')+1
        if arg.startswith('const struct'): continue
        name=re.sub(r'^\(*&?','',arg); name=re.split(r'[\[\)]',name)[0].strip()
        df,dm=find_def(name,f)
        if not df:
            print(f'MANUAL {f}:{line} arg={arg}'); continue
        body=body_from(clean[df],dm.end()-1)
        if dm.group(1):  # 2D array: check each row
            rows=top_elems(body)
            bad=[i for i,r in enumerate(rows) if not is_term(top_elems(r.strip()[1:-1] if r.strip().startswith('{') else r)[-1])]
            print(f'{"OK  " if not bad else "BAD "} {f}:{line} {name} (2D, rows {len(rows)}, bad {bad}) def {df}')
        else:
            el=top_elems(body)
            ok=is_term(el[-1]) if el else False
            print(f'{"OK  " if ok else "BAD "} {f}:{line} {name} last={el[-1][:60]!r} def {df}')
