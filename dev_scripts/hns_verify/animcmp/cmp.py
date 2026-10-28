# cmp.py <dir> [label...]: compare <dir>/old.o and <dir>/new.o label by label (bytes + relocations; REL addends
# read from the bytes and turned into label+offset). Prints DIFF lines, counts, and labels found on one side only.
import re,subprocess,sys,collections
D=sys.argv[1]
def load(p):
    o=D+p+'.o'
    t=subprocess.run(['arm-none-eabi-objdump','-t',o],capture_output=True,text=True).stdout.split('\n')
    syms=collections.defaultdict(list)
    for l in t:
        m=re.match(r'^([0-9a-f]{8}) (.{7}) (\S+)\s+([0-9a-f]{8}) (\S+)$',l)
        if not m: continue
        sec=m.group(3); name=m.group(5)
        if sec in ('*ABS*','*UND*') or name.startswith('.') or name==sec: continue
        syms[sec].append((int(m.group(1),16),name))
    # bytes
    data=collections.defaultdict(bytearray); cur=None
    for l in subprocess.run(['arm-none-eabi-objdump','-s',o],capture_output=True,text=True).stdout.split('\n'):
        m=re.match(r'^Contents of section (\S+):',l)
        if m: cur=m.group(1); continue
        m=re.match(r'^ ([0-9a-f]+) ((?:[0-9a-f]{2,8} ?){1,4})',l)
        if m and cur:
            off=int(m.group(1),16); hx=m.group(2).replace(' ','')
            b=bytes.fromhex(hx)
            if len(data[cur])<off: data[cur].extend(b'\0'*(off-len(data[cur])))
            data[cur][off:off+len(b)]=b
    rel=collections.defaultdict(list); cur=None
    for l in subprocess.run(['arm-none-eabi-objdump','-r',o],capture_output=True,text=True).stdout.split('\n'):
        m=re.match(r'^RELOCATION RECORDS FOR \[(\S+)\]:',l)
        if m: cur=m.group(1); continue
        m=re.match(r'^([0-9a-f]{8}) (\S+)\s+(\S+)$',l)
        if m and cur: rel[cur].append((int(m.group(1),16),m.group(2),m.group(3)))
    return syms,data,rel
def perlabel(syms,data,rel):
    out={}
    for sec,lst in syms.items():
        lst=sorted(lst); addrs=[a for a,_ in lst]
        def name_at(addr):
            best=None
            for a,n in lst:
                if a<=addr: best=(a,n)
                else: break
            return best
        bnd=sorted(set(addrs))+[len(data[sec])]
        for i,(a,n) in enumerate(lst):
            end=min(x for x in bnd if x>a) if any(x>a for x in bnd) else len(data[sec])
            b=bytearray(data[sec][a:end])
            rr=[]
            for off,typ,val in rel[sec]:
                if a<=off<end:
                    m=re.match(r'^(\S+?)(?:\+0x([0-9a-f]+))?$',val)
                    s=m.group(1); add=int(m.group(2) or '0',16)
                    if typ=='R_ARM_ABS32' and off+4<=end:
                        add+=int.from_bytes(data[sec][off:off+4],'little'); b[off-a:off-a+4]=b'\0\0\0\0'
                    if s==sec or s.startswith('.'):
                        t=name_at(add); s='%s+%d'%(t[1],add-t[0]) if t else val
                    elif add: s='%s+%d'%(s,add)
                    rr.append((off-a,typ,s))
            b=bytes(b)
            out[n]=(b,tuple(rr))
    return out
O=perlabel(*load('old')); N=perlabel(*load('new'))
same=diff=0; onlyO=[k for k in O if k not in N]; onlyN=[k for k in N if k not in O]
for k in O:
    if k in N:
        if O[k]==N[k]: same+=1
        else:
            diff+=1; print('DIFF',k,len(O[k][0]),len(N[k][0]))
print('same',same,'diff',diff); print('only old',onlyO[:40]); print('only new',onlyN[:40])
for k in sys.argv[2:]:
    print(k); print(' O',O[k][0].hex(),O[k][1]); print(' N',N[k][0].hex(),N[k][1])
