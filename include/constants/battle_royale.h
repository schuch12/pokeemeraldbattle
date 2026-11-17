#ifndef GUARD_CONSTANTS_BATTLE_ROYALE_H
#define GUARD_CONSTANTS_BATTLE_ROYALE_H

// Battle Royale: tier-based Battle Tower runs.
// Save data lives in previously unused script variables so the save layout stays vanilla.
#define VAR_BR_TIER            VAR_UNUSED_0x40F7 // tier of the run in progress (0 = no run)
#define VAR_BR_BATTLE_NUM      VAR_UNUSED_0x40F8 // battles won so far in the current run
#define VAR_BR_TIERS_CLEARED   VAR_UNUSED_0x40F9 // highest tier cleared so far
#define VAR_BR_USED_TRAINERS   VAR_UNUSED_0x40FA // bitmask of opponents already met this run

#define BR_NUM_TIERS          2
#define BR_BATTLES_PER_TIER   5
#define BR_OVER_CAP_EXP_PERCENT 10 // EXP kept by Pokemon at or above the level cap

#endif // GUARD_CONSTANTS_BATTLE_ROYALE_H
