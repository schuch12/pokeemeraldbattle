# Pokémon Battle Royale

- Base: pret/pokeemerald, GBA, US English
- Workflow: romhack (decomp)
- Concept: hub town + Battle Tower tiers; buy, level and trade Pokémon; transit-friendly saves
- Testing: mGBA first, then EZ-Flash Omega on real hardware
- Save type: Flash 128K (same as vanilla Emerald)
- Constraint: keep Pokémon data structures and species list vanilla-compatible (link trading with real Gen 3 carts)

## Building
Needs `binutils-arm-none-eabi`, `libpng-dev`, and agbcc (https://github.com/pret/agbcc) installed into `tools/agbcc`.
Then `make` produces `pokeemerald.gba`. Playable builds for testing are copied to `builds/`. Current: `BattleRoyale_Beta_v0.1.gba`.
Full setup steps and a map of the code are in `HANDOFF_OPUS.md`.
