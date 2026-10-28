# flatten.py <in.s> <out.s>: inline `call Label` subroutines (label .. first `return`, case-insensitive `call`) so that every animation label can be compared byte by byte.
import re,sys
src=open(sys.argv[1]).read().split('\n')
labre=re.compile(r'^([A-Za-z_][A-Za-z0-9_]*)::?\s*(@.*)?$')
labels={}
for i,l in enumerate(src):
    m=labre.match(l)
    if m: labels[m.group(1)]=i
def body(name,depth=0):
    i=labels[name]+1; out=[]
    while i<len(src):
        l=src[i]
        if labre.match(l): return None          # runs into another label: not a simple subroutine
        t=l.strip()
        if t.startswith('return'): return out
        if re.match(r'(goto|jump)',t): return None
        m=re.match(r'(?i:call)\s+([A-Za-z_][A-Za-z0-9_]*)\s*$',t)
        if m and m.group(1) in labels and depth<8:
            b=body(m.group(1),depth+1)
            if b is None: out.append(l)
            else: out.extend(b)
        else: out.append(l)
        i+=1
    return None
res=[];kept=0;inl=0
for l in src:
    m=labre.match(l)
    if m:
        res.append(l); continue
    t=l.strip()
    mc=re.match(r'(?i:call)\s+([A-Za-z_][A-Za-z0-9_]*)\s*$',t)
    if mc and mc.group(1) in labels:
        b=body(mc.group(1))
        if b is not None: res.extend(b); inl+=1; continue
        kept+=1
    res.append(l)
open(sys.argv[2],'w').write('\n'.join(res))
print('inlined',inl,'kept calls',kept,file=sys.stderr)
