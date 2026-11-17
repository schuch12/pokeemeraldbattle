import sys,struct,json
import os; R=os.path.abspath(os.path.join(os.path.dirname(__file__),'../../../..'))+'/'
def load(n):
    l=[x for x in json.load(open(R+'data/layouts/layouts.json'))['layouts'] if x and x['id']==n][0]
    d=open(R+l['blockdata_filepath'],'rb').read()
    return l['width'],l['height'],list(struct.unpack('<%dH'%(len(d)//2),d))
if __name__=='__main__':
    n,x0,y0,x1,y1=sys.argv[1],*map(int,sys.argv[2:6])
    w,h,b=load(n)
    print('     '+' '.join('%4d'%x for x in range(x0,x1+1)))
    for y in range(y0,y1+1): print('%3d: '%y+' '.join('%04X'%b[y*w+x] for x in range(x0,x1+1)))
