#!/usr/bin/env python3
"""Builds the hub town layout by cropping the west strip of BattleFrontier_OutsideEast
and stamping a few changes. Run from the repo root. Safe to re-run."""
import struct, shutil, os
SRC = 'data/layouts/BattleFrontier_OutsideEast'
DST = 'data/layouts/BattleRoyaleTown'
SW, SH = 72, 72
W, H = 30, 64
d = open(SRC + '/map.bin', 'rb').read()
src = struct.unpack('<%dH' % (SW * SH), d)
m = [[src[y * SW + x] for x in range(W)] for y in range(H)]

SOLID = 0x0400
WALK = 0x3000
# Mart building replaces the small house (door ends up at 13,51)
mart = [[0x3028, 0x3029, 0x3029, 0x302B],
        [0x0430, 0x0431, 0x0432, 0x0433],
        [0x0438, 0x0439, 0x043A, 0x043B],
        [0x0460, 0x0441, 0x0442, 0x0443]]
for dy, row in enumerate(mart):
    for dx, v in enumerate(row):
        m[48 + dy][12 + dx] = v
# Tree wall along the east edge of the lower plaza
for y in range(40, 60):
    v = 0x04C6 if y % 2 == 0 else 0x0416
    m[y][29] = v
    if y < 48:
        m[y][28] = v
# Practice grass in the fenced garden
for y in range(54, 58):
    for x in range(18, 27):
        m[y][x] = WALK | 0x00D
# Doors that lead nowhere yet are made solid
for (x, y) in [(5, 8), (10, 28), (4, 44), (28, 7)]:
    m[y][x] |= SOLID
os.makedirs(DST, exist_ok=True)
open(DST + '/map.bin', 'wb').write(struct.pack('<%dH' % (W * H), *[v for r in m for v in r]))
shutil.copy(SRC + '/border.bin', DST + '/border.bin')
