# Test instructions for Claude Code — Pokémon Battle Royale, `phase5.gba`

You are testing a Pokémon Emerald romhack on Drew's computer with mGBA. Drew is not a
programmer: do the work yourself, do not ask Drew to run commands, and explain anything
you need from Drew in plain English.

**Your deliverable is one file, `TEST_REPORT.md`, saved next to this file.** Drew will
upload it to the Claude session that builds the game. Format is at the bottom.

Do not modify the ROM. Do not try to fix anything. Report only.

## 0. Before you start

- The ROM is `phase5.gba` in this folder. Expected SHA-1:
  `d3ad8208a727ecbf243e334cfff550b5354d66ac`. If it differs, stop and say so in the report.
- Keep every save file you create (`.sav`) in this folder and list them in the report.
- Always start from **New Game** unless a test says otherwise. Delete or move any old
  `.sav` first so results are not mixed with earlier builds.

## 1. How to drive the game

Use whichever of these works on this machine, and say in the report which one you used.

1. **mGBA with Lua scripting** (mGBA 0.10 or newer, Tools → Scripting). A script can press
   buttons (`emu:addKey` / `emu:clearKey`, keys in `C.GBA_KEY`), read memory
   (`emu:read8/16/32`), run on every frame (`callbacks:add("frame", fn)`) and save
   screenshots (`emu:screenshot(path)`).
2. **The headless harness in `test-harness/run.c`** (Linux only; needs `libmgba-dev`).
   Build with `gcc -O2 -o run run.c -lmgba`, run with
   `SB1PTR=3005d8c ./run phase5.gba script.txt save.sav`. Script commands are listed in
   the comment-free source: `run N`, `press KEYS [frames]`, `mash KEY count gap`,
   `walk DIR tiles`, `pos`, `shot file.ppm`, `getmoney`, `name`, `peek addr len`,
   `mashuntil KEY addr gap`, `mashvar KEY varIndexHex gap`. It writes PPM screenshots.
3. **Desktop control of the mGBA window**, if neither of the above is available.

Blind timing is unreliable (text speed, fades). Wait on game state, not on frame counts:
read memory, or check a screenshot, before the next input.

### Memory map for this exact build

| What | Address | Notes |
|---|---|---|
| `gSaveBlock1Ptr` | `0x03005D8C` | pointer; **re-read it every time**, the block moves |
| `gSaveBlock2Ptr` | `0x03005D90` | pointer; re-read every time |
| Player X, Y | SaveBlock1 `+0x00`, `+0x02` | u16 each |
| Map group, map number | SaveBlock1 `+0x04`, `+0x05` | hub town = 0.57, shop = 26.47, Center = 26.53, Mart = 26.55, Tower lobby = 26.89, battle room = 26.90 |
| Money | SaveBlock1 `+0x490` | u32, XOR with the key at SaveBlock2 `+0xAC` |
| Script variables | SaveBlock1 `+0x139C + index*2` | u16 each |
| – run tier (0 = no run) | index `0xF7` | |
| – battles won this run | index `0xF8` | |
| – highest tier cleared | index `0xF9` | |
| Player name | SaveBlock2 `+0x00` | 8 bytes, `0xFF` terminated; `A` = `0xBB`, `a` = `0xD5` |
| `gPlayerPartyCount` | `0x020244E9` | u8 |
| `gPlayerParty` | `0x020244EC` | 100 bytes per Pokémon; level `+0x54`, HP `+0x56`, max HP `+0x58` |
| `gBattleMons` | `0x02024084` | in battle: player's active Pokémon first; HP `+0x28`, level `+0x2A` |
| `gBattleOutcome` | `0x0202433A` | 1 = won, 2 = lost |
| `gMain.callback2` | `0x030022C4` | `0x08085FF1` in the overworld, `0x0803849D` in battle |

### Map of the hub town (tile coordinates)

- New game start: (16, 18), facing down. Battle Tower door: (16, 14), straight up.
- Route to the south buildings from the start: down to (16, 33), right to (21, 33), down to
  (21, 42), left to (16, 42), down to (16, 52). The row y = 52 runs past all three doors.
- Pokémon Center door (3, 51). Mart door (13, 51). Pokémon shop door (22, 51).
- Practice grass: x 18–26, y 54–57 (enter by walking down from (22, 52)).
- Center nurse: stand at (7, 4) facing up. Mart clerk: stand at (3, 3) facing left.
  Shop clerk: stand at (3, 4) facing up. Tower attendant: stand at (14, 6) facing up.

## 2. Tests

Play **honestly** (no memory writes) for tests A–F. They cover what has not been checked
yet. For each test record PASS, FAIL or NOT RUN, with a one-line reason and a screenshot
for every FAIL.

**A. Name entry.** New Game, pick a gender, then on the naming screen:
  1. Enter a single letter with the D-pad and A, confirm with START then A. In town, the
     welcome message and the Start menu must show that letter.
  2. New Game again; confirm the naming screen with **no letters entered**. Record what
     name the game gives (a preset such as REID is the known current behaviour).
  3. New Game again; press A rapidly from the moment the naming screen starts to fade in,
     then confirm. Record whether any letters were dropped. (Drew once got a preset name
     after choosing one letter; we could not reproduce it.)

**B. Shops.** With the starting ₽3000:
  1. Mart: buy one Potion. Money must drop by exactly 300 and the bag must hold it.
  2. Pokémon shop: buy Zigzagoon (₽1000) and answer **YES** to the nickname question, type
     a nickname, confirm. Check the party shows the nickname and the game returns to the
     shop normally.
  3. Try to buy Trapinch (₽3500) without enough money: polite refusal, no money lost.

**C. Blacking out in the grass.** Note your money. Enter the practice grass and lose a wild
  battle on purpose (let every Pokémon faint). Expected: you wake up outside the Pokémon
  Center with a healed team and **the same money as before**. Not expected: money halved, a
  freeze, or appearing anywhere outside the hub town.

**D. Honest difficulty.** Without training first, enter Tier 1 with the three starters and
  play sensibly. Record for each battle: opponent class and name, their Pokémon and levels,
  win or loss, and your team's levels afterwards. Repeat for three separate runs. Then
  train or shop as you see fit and attempt Tier 2 twice, recording the same things plus
  what you did to prepare. We want to know whether Tier 1 is a fair intro and Tier 2 is
  hard but beatable.

**E. Level cap and evolution.** Before clearing Tier 1, get one Pokémon to level 15 and
  note the EXP it gains from the next two battles (should be about a tenth of normal).
  Also confirm a normal level-up evolution happens (for example a wild-caught Wurmple at
  level 7, or a starter at level 16 after Tier 1 is cleared) and the game carries on
  afterwards.

**F. Save and quit.** In a Tier run, win one battle, choose SAVE AND QUIT, save in the
  lobby, close mGBA completely, reopen and Continue. Talk to the attendant: she must offer
  to continue the run at the right battle number. Continue and finish or lose the run.
  Separately, check RETIRE ends the run and keeps the prize money won so far.

**G. Soak test (optional, memory writes allowed).** Loop Tier 1 runs for at least 30
  minutes of emulated play, pressing A through battles. Report any freeze, graphical
  corruption, or a screen that stops responding to A and B for more than 30 seconds.

Anything else that looks wrong while you play — a typo, text running off the box, a door
that leads somewhere odd, an NPC blocking a path — add it under "Other findings".

## 3. `TEST_REPORT.md` format

```
# Test report — phase5.gba
- Date, operating system, mGBA version, how the game was driven (Lua / harness / desktop)
- ROM SHA-1 matched: yes / no

## Summary
One paragraph: what works, what is broken, what could not be tested.

## Results
| Test | Result | Notes |
|---|---|---|
| A1 | PASS / FAIL / NOT RUN | ... |
(one row per numbered item)

## Failures in detail
For each FAIL: exact steps and button presses, what was expected, what happened,
screenshot file name, and the save file that reproduces it if there is one.

## Difficulty log (test D)
The per-battle table for every run.

## Other findings

## Files
Screenshots and save files produced, all in this folder.
```

Report what you saw, not what you expect. If a test could not be run, say why.
