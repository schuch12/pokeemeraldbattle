#include "global.h"
#include "battle_royale.h"
#include "battle.h"
#include "battle_setup.h"
#include "event_data.h"
#include "money.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "strings.h"
#include "constants/event_objects.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"

// ---------------------------------------------------------------------------
// Battle Royale: Battle Tower tiers.
//
// A tier is a run of BR_BATTLES_PER_TIER battles. Each opponent borrows the
// name, class and picture of an existing trainer, but gets a random team
// built from the tier's species pool. Everything here is tuned per tier in
// the sTiers table below.
// ---------------------------------------------------------------------------

struct BRTrainer
{
    u16 trainerId; // existing trainer whose name/class/pic/AI is used
    u8 gfxId;      // overworld sprite
};

struct BRTier
{
    const struct BRTrainer *trainers;
    u8 numTrainers; // max 16 (bitmask in VAR_BR_USED_TRAINERS)
    const u16 *species;
    u8 numSpecies;
    u8 fixedIV;
    u8 monCount[BR_BATTLES_PER_TIER];
    u8 minLevel[BR_BATTLES_PER_TIER];
    u8 maxLevel[BR_BATTLES_PER_TIER];
    u16 winMoney[BR_BATTLES_PER_TIER];
    u16 clearBonus;
    u16 consolation;
    u8 levelCap; // soft cap: EXP is cut sharply at this level and above
};

EWRAM_DATA bool8 gBattleRoyaleTowerBattle = FALSE;
static EWRAM_DATA u8 sTextId = 0;

static const struct BRTrainer sTier1Trainers[] =
{
    {TRAINER_CALVIN_1,  OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_ALLEN,     OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_BILLY,     OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_JOSH,      OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_RICK,      OBJ_EVENT_GFX_BUG_CATCHER},
    {TRAINER_JAMES_1,   OBJ_EVENT_GFX_BUG_CATCHER},
    {TRAINER_JOSE,      OBJ_EVENT_GFX_BUG_CATCHER},
    {TRAINER_TIANA,     OBJ_EVENT_GFX_LASS},
    {TRAINER_HALEY_1,   OBJ_EVENT_GFX_LASS},
    {TRAINER_WINSTON_1, OBJ_EVENT_GFX_RICH_BOY},
    {TRAINER_CINDY_1,   OBJ_EVENT_GFX_WOMAN_2},
    {TRAINER_LOLA_1,    OBJ_EVENT_GFX_TUBER_F},
    {TRAINER_DARIAN,    OBJ_EVENT_GFX_FISHERMAN},
};

static const u16 sTier1Species[] =
{
    SPECIES_POOCHYENA,
    SPECIES_ZIGZAGOON,
    SPECIES_WURMPLE,
    SPECIES_LOTAD,
    SPECIES_SEEDOT,
    SPECIES_TAILLOW,
    SPECIES_WINGULL,
    SPECIES_SURSKIT,
    SPECIES_WHISMUR,
    SPECIES_SKITTY,
    SPECIES_AZURILL,
    SPECIES_NINCADA,
    SPECIES_SLAKOTH,
    SPECIES_ZUBAT,
    SPECIES_MARILL,
    SPECIES_GULPIN,
};

static const struct BRTrainer sTier2Trainers[] =
{
    {TRAINER_SAWYER_1,   OBJ_EVENT_GFX_HIKER},
    {TRAINER_ELI,        OBJ_EVENT_GFX_HIKER},
    {TRAINER_DREW,       OBJ_EVENT_GFX_CAMPER},
    {TRAINER_BEAU,       OBJ_EVENT_GFX_CAMPER},
    {TRAINER_AUTUMN,     OBJ_EVENT_GFX_PICNICKER},
    {TRAINER_HEIDI,      OBJ_EVENT_GFX_PICNICKER},
    {TRAINER_EDWARD,     OBJ_EVENT_GFX_PSYCHIC_M},
    {TRAINER_ZANDER,     OBJ_EVENT_GFX_BLACK_BELT},
    {TRAINER_TAKAO,      OBJ_EVENT_GFX_BLACK_BELT},
    {TRAINER_MELISSA,    OBJ_EVENT_GFX_BEAUTY},
    {TRAINER_WALTER_1,   OBJ_EVENT_GFX_GENTLEMAN},
    {TRAINER_HUEY,       OBJ_EVENT_GFX_SAILOR},
    {TRAINER_MIGUEL_1,   OBJ_EVENT_GFX_POKEFAN_M},
    {TRAINER_VANESSA,    OBJ_EVENT_GFX_POKEFAN_F},
    {TRAINER_STEVE_1,    OBJ_EVENT_GFX_MANIAC},
    {TRAINER_YASU,       OBJ_EVENT_GFX_NINJA_BOY},
};

static const u16 sTier2Species[] =
{
    SPECIES_MIGHTYENA,
    SPECIES_LINOONE,
    SPECIES_BEAUTIFLY,
    SPECIES_DUSTOX,
    SPECIES_LOMBRE,
    SPECIES_NUZLEAF,
    SPECIES_SWELLOW,
    SPECIES_PELIPPER,
    SPECIES_KIRLIA,
    SPECIES_MASQUERAIN,
    SPECIES_BRELOOM,
    SPECIES_LOUDRED,
    SPECIES_VIGOROTH,
    SPECIES_GOLBAT,
    SPECIES_MAKUHITA,
    SPECIES_NOSEPASS,
    SPECIES_ARON,
    SPECIES_MEDITITE,
    SPECIES_ELECTRIKE,
    SPECIES_ROSELIA,
    SPECIES_CARVANHA,
    SPECIES_NUMEL,
    SPECIES_GEODUDE,
    SPECIES_MAGNEMITE,
    SPECIES_MACHOP,
    SPECIES_SABLEYE,
    SPECIES_MAWILE,
    SPECIES_SPOINK,
    SPECIES_CORPHISH,
    SPECIES_GRAVELER,
};

static const struct BRTier sTiers[BR_NUM_TIERS] =
{
    // Tier 1: an easy introduction
    {
        .trainers = sTier1Trainers,
        .numTrainers = ARRAY_COUNT(sTier1Trainers),
        .species = sTier1Species,
        .numSpecies = ARRAY_COUNT(sTier1Species),
        .fixedIV = 0,
        .monCount = {1, 2, 2, 2, 3},
        .minLevel = {3, 3, 4, 5, 6},
        .maxLevel = {4, 4, 5, 6, 8},
        .winMoney = {200, 300, 400, 500, 600},
        .clearBonus = 1500,
        .consolation = 200,
        .levelCap = 15,
    },
    // Tier 2: noticeably harder. Evolved Pokemon, bigger teams, better stats.
    {
        .trainers = sTier2Trainers,
        .numTrainers = ARRAY_COUNT(sTier2Trainers),
        .species = sTier2Species,
        .numSpecies = ARRAY_COUNT(sTier2Species),
        .fixedIV = 12,
        .monCount = {2, 3, 3, 3, 4},
        .minLevel = {14, 15, 17, 18, 20},
        .maxLevel = {16, 17, 19, 21, 23},
        .winMoney = {600, 800, 1000, 1200, 1500},
        .clearBonus = 4000,
        .consolation = 500,
        .levelCap = 25,
    },
};

extern const u8 BattleRoyale_Text_Intro0[];
extern const u8 BattleRoyale_Text_Intro1[];
extern const u8 BattleRoyale_Text_Intro2[];
extern const u8 BattleRoyale_Text_Intro3[];
extern const u8 BattleRoyale_Text_Intro4[];
extern const u8 BattleRoyale_Text_Intro5[];
extern const u8 BattleRoyale_Text_Defeat0[];
extern const u8 BattleRoyale_Text_Defeat1[];
extern const u8 BattleRoyale_Text_Defeat2[];
extern const u8 BattleRoyale_Text_Defeat3[];
extern const u8 BattleRoyale_Text_Defeat4[];
extern const u8 BattleRoyale_Text_Defeat5[];

static const u8 *const sIntroTexts[] =
{
    BattleRoyale_Text_Intro0,
    BattleRoyale_Text_Intro1,
    BattleRoyale_Text_Intro2,
    BattleRoyale_Text_Intro3,
    BattleRoyale_Text_Intro4,
    BattleRoyale_Text_Intro5,
};

static const u8 *const sDefeatTexts[] =
{
    BattleRoyale_Text_Defeat0,
    BattleRoyale_Text_Defeat1,
    BattleRoyale_Text_Defeat2,
    BattleRoyale_Text_Defeat3,
    BattleRoyale_Text_Defeat4,
    BattleRoyale_Text_Defeat5,
};

static const struct BRTier *GetCurrentTier(void)
{
    u16 tier = VarGet(VAR_BR_TIER);

    if (tier == 0 || tier > BR_NUM_TIERS)
        tier = 1;
    return &sTiers[tier - 1];
}

static u8 GetBattleNum(void)
{
    u16 num = VarGet(VAR_BR_BATTLE_NUM);

    if (num >= BR_BATTLES_PER_TIER)
        num = BR_BATTLES_PER_TIER - 1;
    return num;
}

static void EndRun(void)
{
    VarSet(VAR_BR_TIER, 0);
    VarSet(VAR_BR_BATTLE_NUM, 0);
    VarSet(VAR_BR_USED_TRAINERS, 0);
    VarSet(VAR_BR_PAUSED, 0);
}

// The soft level cap follows progress: it is the cap of the first tier the
// player has not cleared yet (or of the last tier once all are cleared).
u8 BattleRoyale_GetLevelCap(void)
{
    u16 cleared = VarGet(VAR_BR_TIERS_CLEARED);

    if (cleared >= BR_NUM_TIERS)
        cleared = BR_NUM_TIERS - 1;
    return sTiers[cleared].levelCap;
}

// special: starts a run of the tier in VAR_0x8004
void BattleRoyale_StartRun(void)
{
    VarSet(VAR_BR_TIER, gSpecialVar_0x8004);
    VarSet(VAR_BR_BATTLE_NUM, 0);
    VarSet(VAR_BR_USED_TRAINERS, 0);
}

// special: picks the next opponent. Sets their overworld sprite, puts their
// intro line in STR_VAR_4 and the battle number in STR_VAR_1.
void BattleRoyale_SetUpOpponent(void)
{
    const struct BRTier *tier = GetCurrentTier();
    u16 used = VarGet(VAR_BR_USED_TRAINERS);
    u8 id;

    if (used == (1 << tier->numTrainers) - 1)
        used = 0;
    do
    {
        id = Random() % tier->numTrainers;
    } while (used & (1 << id));
    VarSet(VAR_BR_USED_TRAINERS, used | (1 << id));

    gTrainerBattleOpponent_A = tier->trainers[id].trainerId;
    VarSet(VAR_OBJ_GFX_ID_0, tier->trainers[id].gfxId);
    sTextId = Random() % ARRAY_COUNT(sIntroTexts);
    StringExpandPlaceholders(gStringVar4, sIntroTexts[sTextId]);
    ConvertIntToDecimalStringN(gStringVar1, GetBattleNum() + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
}

const u8 *BattleRoyale_GetDefeatText(void)
{
    return sDefeatTexts[sTextId % ARRAY_COUNT(sDefeatTexts)];
}

// Called by the battle engine instead of loading the trainer's normal team.
u8 BattleRoyale_CreateEnemyParty(struct Pokemon *party)
{
    const struct BRTier *tier = GetCurrentTier();
    u8 battleNum = GetBattleNum();
    u8 count = tier->monCount[battleNum];
    u16 chosen[PARTY_SIZE];
    u8 i, j;

    ZeroEnemyPartyMons();
    for (i = 0; i < count; i++)
    {
        u16 species;
        u8 level;

        // no duplicate species on one team
        do
        {
            species = tier->species[Random() % tier->numSpecies];
            for (j = 0; j < i; j++)
            {
                if (chosen[j] == species)
                    break;
            }
        } while (j != i);
        chosen[i] = species;

        level = tier->minLevel[battleNum] + Random() % (tier->maxLevel[battleNum] - tier->minLevel[battleNum] + 1);
        CreateMon(&party[i], species, level, tier->fixedIV, FALSE, 0, OT_ID_RANDOM_NO_SHINY, 0);
    }
    return count;
}

u32 BattleRoyale_GetWinMoney(void)
{
    return GetCurrentTier()->winMoney[GetBattleNum()];
}

// special: call after a win. VAR_RESULT is TRUE if that was the last battle
// of the tier; the clear bonus is then paid and written to STR_VAR_1.
void BattleRoyale_OnWin(void)
{
    const struct BRTier *tier = GetCurrentTier();
    u16 battleNum = VarGet(VAR_BR_BATTLE_NUM) + 1;

    VarSet(VAR_BR_BATTLE_NUM, battleNum);
    if (battleNum >= BR_BATTLES_PER_TIER)
    {
        u16 tierNum = VarGet(VAR_BR_TIER);

        AddMoney(&gSaveBlock1Ptr->money, tier->clearBonus);
        ConvertIntToDecimalStringN(gStringVar1, tier->clearBonus, STR_CONV_MODE_LEFT_ALIGN, 5);
        ConvertIntToDecimalStringN(gStringVar2, tierNum, STR_CONV_MODE_LEFT_ALIGN, 1);
        if (VarGet(VAR_BR_TIERS_CLEARED) < tierNum)
            VarSet(VAR_BR_TIERS_CLEARED, tierNum);
        EndRun();
        gSpecialVar_Result = TRUE;
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, battleNum + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
        gSpecialVar_Result = FALSE;
    }
}

// special: call after a loss. Pays the consolation prize (written to STR_VAR_1)
// and ends the run.
void BattleRoyale_OnLoss(void)
{
    const struct BRTier *tier = GetCurrentTier();

    AddMoney(&gSaveBlock1Ptr->money, tier->consolation);
    ConvertIntToDecimalStringN(gStringVar1, tier->consolation, STR_CONV_MODE_LEFT_ALIGN, 5);
    EndRun();
}

// special: the player chose to stop between battles
void BattleRoyale_Retire(void)
{
    EndRun();
}

// special: writes the paused run's tier to STR_VAR_1 and the number of the
// next battle to STR_VAR_2
void BattleRoyale_BufferRunInfo(void)
{
    ConvertIntToDecimalStringN(gStringVar1, VarGet(VAR_BR_TIER), STR_CONV_MODE_LEFT_ALIGN, 1);
    ConvertIntToDecimalStringN(gStringVar2, GetBattleNum() + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
}
