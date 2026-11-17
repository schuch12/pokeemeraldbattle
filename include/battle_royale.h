#ifndef GUARD_BATTLE_ROYALE_H
#define GUARD_BATTLE_ROYALE_H

#include "constants/battle_royale.h"

extern bool8 gBattleRoyaleTowerBattle;

u8 BattleRoyale_CreateEnemyParty(struct Pokemon *party);
u32 BattleRoyale_GetWinMoney(void);
u8 BattleRoyale_GetLevelCap(void);
u8 BattleRoyale_GetWildLevelBonus(void);
void BattleRoyale_OnWhiteOut(void);
const u8 *BattleRoyale_GetDefeatText(void);

#endif // GUARD_BATTLE_ROYALE_H
