# Status

- Phase: **Beta v0.1** — all planned features built. Next step is bug fixing and balance (see `HANDOFF_OPUS.md`).
- Last working build: `builds/BattleRoyale_Beta_v0.1.gba`
- Next: Drew plays the beta; fix what he finds; tune Tiers 3–9.
- Known issues and the "never tested" list: `HANDOFF_OPUS.md` sections 7 and 8.

## Decisions (approved by Drew)
- Hub town name: バトル・ロワイアル. Title: Pokémon Battle Royale.
- Starter pool (level 5, random): Treecko, Torchic, Mudkip, Ralts, Shroomish, Aron, Electrike, Machop, Abra, Taillow, Eevee.
- Pokémon shop (level 5): Zigzagoon ₽1000, Wingull ₽1500, Makuhita ₽2000, Geodude ₽2000, Gastly ₽3000, Trapinch ₽3500.
- Losing a tier run costs nothing extra for now (penalty maybe later).
- New game gives 3 different random starters from the pool (Drew found 1 too few).
- Center/Mart bystander lines are Drew's six quotes (npc_quotes.md).
- Pokémon evolve normally. Plan: sell evolution stones in the Mart (Phase 2) so Eevee etc. can evolve.

- Beta decisions made without Drew (he asked for an unattended build): boss on battle 5 of each tier (gym leaders, Steven), Champion tier = Elite Four + Wallace, loss fees from Tier 3, game over = team fainted with ₽0 (soft reset, save kept), link battles after Tier 4, shop levels/prices scale with progress, rare clerk after Tier 2, three Mart stock stages, wild level bonus, Skitty cheat menu (Drew's request).

## Technical notes
See `HANDOFF_OPUS.md` section 5 for the file map and persistent state.

## Phase log
- Phase 0: toolchain installed, vanilla build matched rom.sha1.
- Phase 1: shortened intro (boy/girl + name only), hub town, random starter, National Dex and running shoes from the start. Tested in a headless emulator: new game, menus, save and continue.
- Phase 2: Center heals, Mart sells balls/potions/status heals/berries/4 evolution stones, shop sells 6 Pokémon (party or PC). Tested in a headless emulator, including a full party and too little money.
- Phase 3: Tier 1 (5 battles, random opponents and teams, lv 3–8). Wins pay ₽200/300/400/500/600, clear bonus ₽1500, loss pays ₽200. Tested: full clear, forced loss.
- Phase 4: Tier 2 (lv 14–23, evolved Pokémon, teams of 2–4; wins ₽600/800/1000/1200/1500, bonus ₽4000, loss ₽500), tier menu with Tier 2 locked until Tier 1 is cleared, level cap, practice grass, 3 starters, Drew's NPC quotes. Tested with a boosted team: Tier 1 clear → Tier 2 clear, money and progress correct; level stalls at 15 in the grass.
- Phase 5: save and quit between battles, resume or abandon from the lobby. Full run-through tested over three emulator sessions with a boosted team: Tier 1 battle 1 → save → reload → clear Tier 1 → Tier 2 two battles → save → reload → clear Tier 2. Final money ₽15600 as expected.
- Beta v0.1: Tiers 3–8 and Champion tier, bosses, loss fees, bankruptcy game over, obedience by tier, legendary ban, link-battle gate, scaling shop and wild levels, staged Mart stock, cheat menu. Tested: all nine tiers cleared in order with a boosted team (final money ₽365000), tier lock, loss fee, both game-over paths, grass blackout, shop scaling, rare clerk, cheat menu, save/quit/resume.
