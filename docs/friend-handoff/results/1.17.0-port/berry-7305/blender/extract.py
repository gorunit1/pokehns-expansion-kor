import sys,re
src=open(sys.argv[1],encoding='utf-8').read().split('\n')
out=[]
def block(start_pat, end_pred):
    for i,l in enumerate(src):
        if re.match(start_pat,l):
            j=i
            while not end_pred(src[j]): j+=1
            return src[i:j+1]
    raise SystemExit('not found '+start_pat)
out+= [l for l in src if l.startswith('#define NUM_NPC_BERRIES')]
out+= block(r'^struct BlenderBerry$', lambda l: l.startswith('};'))
out+= block(r'^static const u8 sOpponentBerrySets', lambda l: l.startswith('};'))
out+= block(r'^static const u8 sBerryMasterBerries', lambda l: l.startswith('};'))
out+= block(r'^static void SetOpponentsBerryData\(', lambda l: l=='}')
print('\n'.join(out))
