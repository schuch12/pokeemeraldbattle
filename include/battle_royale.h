#ifndef GUARD_BATTLE_ROYALE_H
#define GUARD_BATTLE_ROYALE_H

// Battle Royale: tier-based Battle Tower runs.
// Save data lives in previously unused script variables so the save layout stays vanilla.
#define VAR_BR_TIER            VAR_UNUSED_0x40F7 // tier of the run in progress (0 = no run)
#define VAR_BR_BATTLE_NUM      VAR_UNUSED_0x40F8 // battles won so far in the current run
#define VAR_BR_TIERS_CLEARED   VAR_UNUSED_0x40F9 // highest tier cleared so far
#define VAR_BR_USED_TRAINERS   VAR_UNUSED_0x40FA // bitmask of opponents already met this run

#define BR_NUM_TIERS          1
#define BR_BATTLES_PER_TIER   5

extern bool8 gBattleRoyaleTowerBattle;

u8 BattleRoyale_CreateEnemyParty(struct Pokemon *party);
u32 BattleRoyale_GetWinMoney(void);
const u8 *BattleRoyale_GetDefeatText(void);

#endif // GUARD_BATTLE_ROYALE_H
