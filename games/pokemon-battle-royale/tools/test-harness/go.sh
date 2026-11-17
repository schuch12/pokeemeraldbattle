#!/bin/bash
# usage: ./go.sh script.txt [save.sav]
# Runs the ROM headlessly with the given input script and builds sheet.png from the
# screenshots (s1.ppm, s2.ppm, ...) the script took. Run from this directory.
# BUFF=none ./go.sh ...  turns off the stat boost that `mashvar` applies (see buff2.txt).
cd "$(dirname "$0")"
ROOT=../../../..
[ -x ./run ] || gcc -O2 -o run run.c -lmgba || exit 1
rm -f s*.ppm
export SB1PTR=$(grep -w gSaveBlock1Ptr $ROOT/pokeemerald.map | awk '{print $1}')
./run $ROOT/pokeemerald.gba "$1" "${2:-t.sav}"
python3 - <<'P'
from PIL import Image; import glob,re
fs=sorted(glob.glob('s*.ppm'),key=lambda f:int(re.findall(r'\d+',f)[0]))
if fs:
    c=3; r=(len(fs)+c-1)//c
    o=Image.new('RGB',(240*c,160*r))
    for i,f in enumerate(fs): o.paste(Image.open(f),((i%c)*240,(i//c)*160))
    o.save('sheet.png')
P
