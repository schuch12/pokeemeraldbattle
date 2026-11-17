# Handoff: Pokémon Battle Royale — Beta v0.1 → bug fixing

You are picking up a Pokémon Emerald romhack (pret/pokeemerald decomp) at its first beta.
All planned features are built. **Your job is to find and fix bugs in what exists, not to
add features**, unless Drew asks. Read `GAME.md` and `STATUS.md` in this folder first.

- Current ROM: `builds/BattleRoyale_Beta_v0.1.1.gba` (SHA-1 `0f0380f009c5b7b1d9f875416b93c409f771c894`). `BattleRoyale_Beta_v0.1.gba` is the first beta.
- Branch: `claude/beautiful-allen-khfrqu`
- Earlier milestones: `builds/phase1.gba` … `phase5.gba` (kept for reference)

## 1. Working with Drew

- Drew is a complete beginner: no coding, no command line. Explain in plain English.
  Never ask Drew to run commands. You build, test and commit.
- Deliver a build by committing the `.gba` to `builds/` (it is git-ignored, so use
  `git add -f`), pushing, and giving Drew the GitHub link to download.
- After each fix round: `make` must succeed, tell Drew which file to download and give
  3–5 concrete things to check. Update `STATUS.md`.
- Drew tests in mGBA, then on an EZ-Flash Omega (real GBA hardware).
- Say plainly what you tested and what you did not. Almost all testing so far was done
  with an automated emulator and an artificially boosted team, so **balance above Tier 2
  has never been played by a human**.
- A local Claude Code run of the test brief failed on Drew's machine; do not rely on
  Drew being able to run automated tests.

## 2. Setting up the build (fresh container)

```
apt-get update && apt-get install -y binutils-arm-none-eabi libpng-dev libmgba-dev build-essential
git clone --depth 1 https://github.com/pret/agbcc /home/user/agbcc
cd /home/user/agbcc && ./build.sh && ./install.sh /home/user/pokeemeraldbattle
cd /home/user/pokeemeraldbattle && make -j$(nproc)      # -> pokeemerald.gba
```

`tools/agbcc/` and `pokeemerald.gba` are not committed. The ROM no longer matches
`rom.sha1` (it is a hack), so use `make`, not `make compare`.

## 3. Hard constraints (do not break these)

- **Do not change `struct Pokemon` / `BoxPokemon`, the species list, or the save block
  layout.** Link trading with real Gen 3 cartridges must keep working.
- Keep the save type Flash 128K and the header game code `BPEE` (EZ-Flash and emulators
  use it to pick the save type).
- New persistent state must go in *unused* vars/flags only (see §5), never new save fields.
- New region-map sections are not allowed (met-location byte); the hub reuses
  `MAPSEC_BATTLE_FRONTIER`, renamed to `バトル・ロワイアル`.
- Script functions that set `gSpecialVar_Result` themselves and return `void` must be
  called with `special`, **not** `specialvar` (that bug already happened once).
- Text boxes fit about 36 characters per line. Japanese text needs `{JPN}…{ENG}`.

## 4. What the game is

New Game → short intro (gender, name) → hub town with three random level-5 starters and
₽3000. Hub has a Pokémon Center, Mart, Pokémon shop, practice grass and the Battle Tower.
The Tower has **Tiers 1–8 and a Champion tier**, five battles each, cleared in order.

| Tier | Opponent levels | Team sizes | Boss (battle 5) | Level cap while uncleared | Win money (per battle) | Clear bonus | On loss |
|---|---|---|---|---|---|---|---|
| 1 | 3–8 | 1,2,2,2,3 | Roxanne | 15 | 200–600 | 1500 | +200 |
| 2 | 14–23 | 2,3,3,3,4 | Brawly | 25 | 600–1500 | 4000 | +500 |
| 3 | 22–31 | 3,3,3,4,4 | Wattson | 33 | 1000–2500 | 6000 | −500 |
| 4 | 30–39 | 3,3,4,4,5 | Flannery | 41 | 1500–4000 | 9000 | −1000 |
| 5 | 38–47 | 3,4,4,5,5 | Norman | 49 | 2500–5500 | 12000 | −2000 |
| 6 | 46–55 | 4,4,5,5,6 | Winona | 57 | 3500–8000 | 16000 | −3000 |
| 7 | 54–63 | 4,5,5,6,6 | Juan | 65 | 5000–10000 | 20000 | −4000 |
| 8 | 62–71 | 5,5,6,6,6 | Steven | 73 | 6000–13000 | 30000 | −5000 |
| Champion | 72–78 | 5,5,5,5,6 | Sidney, Phoebe, Glacia, Drake, Wallace in order | 80 | 10000 ×4, 20000 | 50000 | −8000 |

Rules implemented:

- **Opponents** borrow the name/class/picture/AI/items of a vanilla trainer but get a
  random team from the tier's species pool (default level-up moves).
- **Normal battles**: EXP, bag items and switching all allowed. Party healed before a run
  and between battles. A loss never whites out.
- **Soft level cap**: at or above the cap a Pokémon gets 10% EXP. The cap is that of the
  first uncleared tier; no cap once the Champion tier is cleared.
- **Traded Pokémon** (different OT) may disobey above the cap. Badges are not used.
- **Legendaries** (list `sBannedSpecies`) are refused at the lobby except for the Champion tier.
- **Loss fee** from Tier 3: pays `min(fee, money)`. **Game over** = whole team fainted with
  exactly ₽0 (a Tier 3+ loss, or a grass blackout). Game over shows a message and soft
  resets to the title; the save file is *not* erased (deliberate, cheap to change).
- **Save and quit** between battles: returns to the lobby with the run on hold and offers
  the normal save. The attendant resumes or abandons it. The player can roam, heal, shop
  and train while a run is on hold (accepted for now).
- **Pokémon shop**: standard clerk (6 species) and rare clerk (6 species, after Tier 2).
  Level sold = level cap − 10 (min 5); price = base × (tiers cleared + 1).
- **Mart** stock has three stages (start, after Tier 2, after Tier 5).
- **Practice grass**: 12 species, level 3–8 plus a bonus that grows with tiers cleared (lowered from 4–12 in v0.1.1 after Drew blacked out to a level 11 wild Pokémon with level 5 starters).
- **Running from wild Pokémon always succeeds** (`TryRunFromBattle` in `src/battle_util.c`), unless a trapping move or ability applies.
- **Link battles** (Cable Club Colosseum) open after Tier 4. Trading is always open.
- **Cheat menu**: talk to the Skitty in the Pokémon Center. Toggles: level limit off, all
  tiers open, EXP ×4. Actions: +₽100000, 10 Rare Candies, BALLS (5 each of Master, Ultra and Great Ball), heal party. Drew asked for this.

## 5. Where things live

| Area | Files |
|---|---|
| All tier data and Tower/shop/game-over logic | `src/battle_royale.c`, `include/battle_royale.h`, `include/constants/battle_royale.h` |
| Starting a Tower battle, no-whiteout end | `BattleRoyale_StartTowerBattle` in `src/battle_setup.c` |
| Enemy team override | top of `CreateNPCTrainerParty` in `src/battle_main.c` |
| Prize money override, EXP cap, EXP cheat | `GetTrainerMoneyToGive` and `Cmd_getexp` in `src/battle_script_commands.c` |
| Obedience | `IsMonDisobedient` in `src/battle_util.c` |
| Wild level bonus | `TryGenerateWildMon` in `src/wild_encounter.c`; table in `src/data/wild_encounters.json` (`MAP_BATTLE_ROYALE_TOWN`) |
| Blackout (no money loss, game-over flag) | `DoWhiteOut` in `src/overworld.c` |
| New game: warp, starters, flags | `src/new_game.c`; intro shortcut `Task_NewGameQuickStart` in `src/main_menu.c`; `CB2_NewGame` in `src/overworld.c` |
| Hub town | `data/maps/BattleRoyaleTown/`, layout built by `tools/make_hub.py` from BattleFrontier_OutsideEast |
| Tower lobby / battle room | `data/maps/BattleRoyale_TowerLobby/`, `data/maps/BattleRoyale_TowerBattleRoom/` |
| Center (incl. cheat menu on Skitty) | `data/maps/BattleFrontier_PokemonCenter_1F/scripts.inc` |
| Mart | `data/maps/BattleFrontier_Mart/scripts.inc` |
| Pokémon shop | end of `data/maps/BattleFrontier_Lounge5/scripts.inc` |
| Menus | `src/data/script_menu.h` (`MULTI_BR_*`), tier list `SCROLL_MULTI_BR_TIERS` in `src/field_specials.c` |
| Link battle gate | `CableClub_EventScript_Colosseum` in `data/scripts/cable_club.inc` |
| Item prices changed | berries, Moon Stone in `src/data/items.h` (search "Battle Royale") |

Every engine change is marked with a `Battle Royale` comment: `grep -rn "Battle Royale" src data include`.

Persistent state (all previously unused):

| Name | Var/flag | Meaning |
|---|---|---|
| `VAR_BR_TIER` | 0x40F7 | tier of the run in progress, 0 = none |
| `VAR_BR_BATTLE_NUM` | 0x40F8 | battles won in this run |
| `VAR_BR_TIERS_CLEARED` | 0x40F9 | highest tier cleared (9 = champion) |
| `VAR_BR_USED_TRAINERS` | 0x40FA | bitmask of opponents met this run |
| `VAR_BR_PAUSED` | 0x40FB | 1 = lobby should offer to save |
| `VAR_BR_GAME_OVER` | 0x40FC | 1 = hub shows game over |
| `VAR_LITTLEROOT_INTRO_STATE` | (vanilla) | 0 until the welcome message has shown |
| `FLAG_BR_CHEAT_*` | flags 0x20–0x22 | cheat switches |

## 6. Testing tools

`tools/test-harness/` holds a headless mGBA runner (`run.c`, needs `libmgba-dev`) and the
input scripts used so far. `./go.sh script_full.txt` plays the built ROM and writes
`sheet.png` (a contact sheet of screenshots).

Script commands: `run N` frames; `press KEYS [frames]`; `mash KEY count gap`;
`walk DIR tiles` (reads the player position, reports "blocked"); `pos`; `shot fN.ppm`;
`getmoney` (money and tier vars); `name`; `opts` (text fast, battle style SET, animations
off); `money N`; `setvar HEXINDEX value` (index = var − 0x4000); `peek`/`poke8`/`poke16`;
`mashuntil KEY addr gap` (until a 32-bit value changes); `mashvar KEY HEXINDEX gap` (until
a var changes; each press also re-applies the boosts in `buff2.txt` unless `BUFF=none`).

- `script_start.txt` — boot to the hub (prepend it to other scripts; the listed scripts already include it or a boot sequence).
- `script_full.txt` — clears all nine tiers with a boosted lead. Expected final line: `money 365000 … cleared=9`.
- `script_rules.txt` (`BUFF=none`) — locked tier, loss fee, Tower game over.
- `script_grass.txt` (`BUFF=none`) — grass blackout with and without money.
- `script_cheat.txt` — Skitty cheat menu. `script_balls_run.txt` (`BUFF=none`) — BALLS cheat and running from a wild battle.
- `script_p5a2.txt`, `script_p5b.txt`, `script_p5c.txt` — three sessions sharing one save: pause, save, reload, resume.
- `render.py LAYOUT_ID out.png [scale]` draws a map layout; `dump.py` prints block ids.

Lessons learned: fixed frame counts are fragile, so wait on state (`mashvar`,
`mashuntil`, `walk`). The save blocks move in RAM, so re-read `gSaveBlock1Ptr` every
time. **`buff.txt`, `buff2.txt` and `weak.txt` contain hard-coded RAM addresses**
(`gPlayerParty` 0x020244EC, `gBattleMons` 0x02024084); if you add or remove EWRAM
variables, look the symbols up in `pokeemerald.map` and regenerate them.

## 7. What has and has not been tested

Tested in the headless emulator on the beta build:
- New game, naming, three starters, menus, save and continue.
- Center heal, Mart (first stock list), shop purchases to party and PC, not-enough-money,
  shop level/price scaling, rare clerk lock and purchase.
- All nine tiers cleared in order (boosted team); money and progress correct.
- Tier lock message, tier info text, loss consolation, loss fee, Tower game over.
- Grass encounters, level stall at the cap, blackout respawn, grass game over.
- Save and quit, reload, resume, in Tier 1 and Tier 2.
- Cheat menu: level limit toggle text, money, Rare Candies.

Confirmed by Drew on earlier builds: quotes fit, Tier 2 lock, Tier 1–2 difficulty "good".

**Never tested — look here first:**
1. Real hardware / EZ-Flash Omega saving.
2. Link trade and link battle, including the Tier 4 gate and trading with a real cartridge.
3. Traded-Pokémon disobedience (needs a traded Pokémon).
4. The legendary ban (needs a legendary; code path is simple).
5. Honest balance of Tiers 3–9: levels, team sizes, fees, prize money, shop prices, wild
   level bonus. Expect tuning. All numbers are in `sTiers`, `sWildLevelBonus`, `sShopMons`.
6. Mart stock lists 2 and 3 on screen (they build; contents unchecked in game).
7. Shop nickname "YES" path, and buying when the party is full on the rare clerk.
8. Cheat toggles "all tiers open" and "EXP ×4" in play (only the menu was exercised).
9. Evolution during a Tower run (evolution scene between battles), and move-learning
   prompts there.
10. Abandoning a paused run from the lobby; RETIRE between battles.
11. Champion clear message on screen (the run reached `cleared=9`; text not screenshotted).

## 8. Known issues and loose ends

- **Blank name → preset name.** Confirming the naming screen with no letters gives a
  preset such as REID (vanilla behaviour; the confirmation step was cut with Birch's
  speech). Drew hit this once. Suggested fix: reopen the naming screen when the result is
  empty, or restore a short "So it's NAME?" yes/no.
- **Player faces down after returning to the lobby**, so talking again needs a turn first.
- **Battle-loss text** still says the player "whited out" inside the battle before the
  Tower script takes over.
- Opponent movesets are default level-up moves, so high-tier teams are weaker than their
  levels suggest; bosses carry their vanilla items (Potions, Full Restores).
- Normal-type-only teams can stall against Ghost types (Sableye in Tier 2–3).
- A run on hold lets the player train and shop mid-run; legendaries are re-checked on resume.
- Hub map is a cropped Battle Frontier: unused doors are solid, edges end at water, and a
  few cliff areas are unreachable. Title screen, credits and Pokédex text are vanilla.
- Mart and Center bystanders use Drew's six quotes (`npc_quotes` in the conversation);
  the Center 2F Cable Club staff are vanilla.
- Cheat flags persist in the save. There is no "cheats used" marker.
- Union Room (wireless) battles are not gated, only the Cable Club Colosseum.
- `FLAG_HIDE_BATTLE_TOWER_OPPONENT` is reused for the opponent in the battle room.

## 9. Suggested first session

1. Set up the build (§2), run `tools/test-harness/go.sh script_full.txt` and confirm
   `cleared=9`. That proves your environment matches.
2. Ask Drew what he has seen since the beta; fix reported bugs first.
3. Then work through the "never tested" list in §7 using cheats (`setvar f9 N`, the Skitty
   menu) to reach each state quickly, and screenshot every text box added in this build.
4. Tune Tiers 3–9 only with Drew's feedback from real play.
