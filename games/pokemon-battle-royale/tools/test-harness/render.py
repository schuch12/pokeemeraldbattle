import sys, struct, json
from PIL import Image, ImageDraw
import os; R=os.path.abspath(os.path.join(os.path.dirname(__file__),'../../../..'))+'/'
def load_ts(path):
    img=Image.open(R+path+'/tiles.png'); px=img.load(); w,h=img.size
    mt=open(R+path+'/metatiles.bin','rb').read()
    pals=[]
    for i in range(13):
        try:
            ls=open(R+path+'/palettes/%02d.pal'%i).read().split('\n')[3:19]
            pals.append([tuple(map(int,l.split())) for l in ls])
        except Exception: pals.append([(0,0,0)]*16)
    return dict(px=px,w=w,h=h,mt=struct.unpack('<%dH'%(len(mt)//2),mt),pals=pals)
def render(layout_id, out, scale=1, grid=True, blocks=None, size=None):
    ls=json.load(open(R+'data/layouts/layouts.json'))['layouts']
    l=[x for x in ls if x and x['id']==layout_id][0]
    names={'gTileset_General':'primary/general'}
    import re
    def tsdir(n):
        if n in names: return names[n]
        s=re.sub(r'(?<!^)(?=[A-Z])','_',n.replace('gTileset_','')).lower()
        import os
        for d in ('primary','secondary'):
            if os.path.isdir(R+'data/tilesets/%s/%s'%(d,s)): return d+'/'+s
        raise Exception(s)
    P=load_ts('data/tilesets/'+tsdir(l['primary_tileset'])); S=load_ts('data/tilesets/'+tsdir(l['secondary_tileset']))
    W,H=size or (l['width'],l['height'])
    if blocks is None:
        d=open(R+l['blockdata_filepath'],'rb').read(); blocks=struct.unpack('<%dH'%(W*H),d)
    im=Image.new('RGB',(W*16,H*16))
    ip=im.load()
    for i,b in enumerate(blocks):
        m=b&0x3ff; ts=P if m<0x200 else S; mi=m if m<0x200 else m-0x200
        bx,by=(i%W)*16,(i//W)*16
        for k in range(8):
            if mi*8+k>=len(ts['mt']): continue
            t=ts['mt'][mi*8+k]; ti=t&0x3ff; hf=t>>10&1; vf=t>>11&1; pal=t>>12
            src=P if ti<512 else S; tj=ti if ti<512 else ti-512
            pl=(P if pal<6 else S)['pals'][pal] if pal<13 else [(0,0,0)]*16
            tx,ty=(tj%(src['w']//8))*8,(tj//(src['w']//8))*8
            if ty>=src['h']: continue
            ox,oy=bx+(k%2)*8,by+((k%4)//2)*8
            for y in range(8):
                for x in range(8):
                    c=src['px'][tx+(7-x if hf else x),ty+(7-y if vf else y)]
                    if c==0 and k>=4: continue
                    ip[ox+x,oy+y]=pl[c]
    dr=ImageDraw.Draw(im)
    for i,b in enumerate(blocks):
        if (b>>10)&3 and grid: 
            bx,by=(i%W)*16,(i//W)*16; dr.point([(bx+7,by+7),(bx+8,by+8),(bx+7,by+8),(bx+8,by+7)],fill=(255,0,0))
    if grid:
        for x in range(0,W,5): dr.line([(x*16,0),(x*16,H*16)],fill=(255,255,0)); dr.text((x*16+1,0),str(x),fill=(255,255,255))
        for y in range(0,H,5): dr.line([(0,y*16),(W*16,y*16)],fill=(255,255,0)); dr.text((0,y*16+1),str(y),fill=(255,255,255))
    if scale!=1: im=im.resize((int(W*16*scale),int(H*16*scale)))
    im.save(out)
if __name__=='__main__': render(sys.argv[1],sys.argv[2],float(sys.argv[3]) if len(sys.argv)>3 else 1)
