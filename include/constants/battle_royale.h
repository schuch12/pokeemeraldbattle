#ifndef GUARD_CONSTANTS_BATTLE_ROYALE_H
#define GUARD_CONSTANTS_BATTLE_ROYALE_H

// Battle Royale: tier-based Battle Tower runs.
// Save data lives in previously unused script variables so the save layout stays vanilla.
#define VAR_BR_TIER            VAR_UNUSED_0x40F7 // tier of the run in progress (0 = no run)
#define VAR_BR_BATTLE_NUM      VAR_UNUSED_0x40F8 // battles won so far in the current run
#define VAR_BR_TIERS_CLEARED   VAR_UNUSED_0x40F9 // highest tier cleared so far
#define VAR_BR_USED_TRAINERS   VAR_UNUSED_0x40FA // bitmask of opponents already met this run
#define VAR_BR_PAUSED          VAR_UNUSED_0x40FB // 1 = just left a run to save; lobby offers to save
#define VAR_BR_GAME_OVER       VAR_UNUSED_0x40FC // 1 = blacked out with no money; hub shows game over

// Cheat switches, toggled by talking to the SKITTY in the Pokemon Center
#define FLAG_BR_CHEAT_NO_LEVEL_CAP  FLAG_UNUSED_0x020 // no EXP cut, traded Pokemon always obey
#define FLAG_BR_CHEAT_ALL_TIERS     FLAG_UNUSED_0x021 // every tier can be entered
#define FLAG_BR_CHEAT_FAST_EXP      FLAG_UNUSED_0x022 // EXP x4
#define BR_CHEAT_EXP_MULTIPLIER 4

#define BR_NUM_TIERS          9 // tiers 1-8 plus the Champion tier
#define BR_CHAMPION_TIER      BR_NUM_TIERS
#define BR_BATTLES_PER_TIER   5
#define BR_OVER_CAP_EXP_PERCENT 10 // EXP kept by Pokemon at or above the level cap
#define BR_LINK_BATTLE_TIERS  4 // tiers that must be cleared before link battles open
#define BR_RARE_SHOP_TIERS    2 // tiers that must be cleared before the rare clerk sells
#define BR_SHOP_MONS_PER_LIST 6

// results of BattleRoyale_OnLoss
#define BR_LOSS_CONSOLATION 0
#define BR_LOSS_FEE         1
#define BR_LOSS_NOTHING     2
#define BR_LOSS_GAME_OVER   3

// results of BattleRoyale_BuyShopMon
#define BR_SHOP_NO_MONEY 0
#define BR_SHOP_NO_ROOM  1
#define BR_SHOP_TO_PARTY 2
#define BR_SHOP_TO_PC    3

#endif // GUARD_CONSTANTS_BATTLE_ROYALE_H
