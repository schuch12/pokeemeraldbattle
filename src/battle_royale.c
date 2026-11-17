#include "global.h"
#include "battle_royale.h"
#include "battle.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "main.h"
#include "money.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "strings.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"

// ---------------------------------------------------------------------------
// Battle Royale: Battle Tower tiers.
//
// A tier is a run of BR_BATTLES_PER_TIER battles. Each opponent borrows the
// name, class, picture and AI of an existing trainer, but gets a random team
// built from the tier's species pool. The last battle of a tier is against
// its boss. The final (Champion) tier is the Elite Four and Wallace, in order.
// Everything is tuned per tier in the sTiers table below.
//
// NOTE: this file is partly generated; edit it directly from now on.
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
    struct BRTrainer boss; // opponent of the last battle
    bool8 fixedOrder;      // TRUE: trainers[] are fought in order and there is no separate boss
    const u16 *species;
    u8 numSpecies;
    u8 fixedIV;
    u8 monCount[BR_BATTLES_PER_TIER];
    u8 minLevel[BR_BATTLES_PER_TIER];
    u8 maxLevel[BR_BATTLES_PER_TIER];
    u16 winMoney[BR_BATTLES_PER_TIER];
    u32 clearBonus;
    u16 consolation; // paid to the player after a loss
    u16 lossFee;     // taken from the player after a loss
    u8 levelCap;     // soft cap while this tier is the next one to clear
};

EWRAM_DATA bool8 gBattleRoyaleTowerBattle = FALSE;
static EWRAM_DATA u8 sTextId = 0;
static EWRAM_DATA bool8 sIsBoss = FALSE;

static const struct BRTrainer sTier1Trainers[] =
{
    {TRAINER_CALVIN_1, OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_ALLEN, OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_BILLY, OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_JOSH, OBJ_EVENT_GFX_YOUNGSTER},
    {TRAINER_RICK, OBJ_EVENT_GFX_BUG_CATCHER},
    {TRAINER_JAMES_1, OBJ_EVENT_GFX_BUG_CATCHER},
    {TRAINER_JOSE, OBJ_EVENT_GFX_BUG_CATCHER},
    {TRAINER_TIANA, OBJ_EVENT_GFX_LASS},
    {TRAINER_HALEY_1, OBJ_EVENT_GFX_LASS},
    {TRAINER_WINSTON_1, OBJ_EVENT_GFX_RICH_BOY},
    {TRAINER_CINDY_1, OBJ_EVENT_GFX_WOMAN_2},
    {TRAINER_LOLA_1, OBJ_EVENT_GFX_TUBER_F},
    {TRAINER_DARIAN, OBJ_EVENT_GFX_FISHERMAN},
};

static const struct BRTrainer sTier2Trainers[] =
{
    {TRAINER_SAWYER_1, OBJ_EVENT_GFX_HIKER},
    {TRAINER_ELI, OBJ_EVENT_GFX_HIKER},
    {TRAINER_DREW, OBJ_EVENT_GFX_CAMPER},
    {TRAINER_BEAU, OBJ_EVENT_GFX_CAMPER},
    {TRAINER_AUTUMN, OBJ_EVENT_GFX_PICNICKER},
    {TRAINER_HEIDI, OBJ_EVENT_GFX_PICNICKER},
    {TRAINER_EDWARD, OBJ_EVENT_GFX_PSYCHIC_M},
    {TRAINER_ZANDER, OBJ_EVENT_GFX_BLACK_BELT},
    {TRAINER_TAKAO, OBJ_EVENT_GFX_BLACK_BELT},
    {TRAINER_MELISSA, OBJ_EVENT_GFX_BEAUTY},
    {TRAINER_WALTER_1, OBJ_EVENT_GFX_GENTLEMAN},
    {TRAINER_HUEY, OBJ_EVENT_GFX_SAILOR},
    {TRAINER_MIGUEL_1, OBJ_EVENT_GFX_POKEFAN_M},
    {TRAINER_VANESSA, OBJ_EVENT_GFX_POKEFAN_F},
    {TRAINER_STEVE_1, OBJ_EVENT_GFX_MANIAC},
    {TRAINER_YASU, OBJ_EVENT_GFX_NINJA_BOY},
};

static const struct BRTrainer sTier3Trainers[] =
{
    {TRAINER_ALBERTO, OBJ_EVENT_GFX_BOY_3},
    {TRAINER_PERRY, OBJ_EVENT_GFX_BOY_3},
    {TRAINER_KIRK, OBJ_EVENT_GFX_MAN_5},
    {TRAINER_SHAWN, OBJ_EVENT_GFX_MAN_5},
    {TRAINER_COLE, OBJ_EVENT_GFX_MAN_4},
    {TRAINER_JEFF, OBJ_EVENT_GFX_MAN_4},
    {TRAINER_JOCELYN, OBJ_EVENT_GFX_GIRL_3},
    {TRAINER_LAURA, OBJ_EVENT_GFX_GIRL_3},
    {TRAINER_JACLYN, OBJ_EVENT_GFX_LASS},
    {TRAINER_HANNAH, OBJ_EVENT_GFX_LASS},
    {TRAINER_ED, OBJ_EVENT_GFX_MANIAC},
    {TRAINER_HECTOR, OBJ_EVENT_GFX_MANIAC},
    {TRAINER_DUSTY_1, OBJ_EVENT_GFX_HIKER},
    {TRAINER_CHIP, OBJ_EVENT_GFX_HIKER},
    {TRAINER_DAISY, OBJ_EVENT_GFX_WOMAN_2},
    {TRAINER_MADELINE_1, OBJ_EVENT_GFX_WOMAN_5},
};

static const struct BRTrainer sTier4Trainers[] =
{
    {TRAINER_FREDRICK, OBJ_EVENT_GFX_EXPERT_M},
    {TRAINER_TIMOTHY_1, OBJ_EVENT_GFX_EXPERT_M},
    {TRAINER_MOLLIE, OBJ_EVENT_GFX_EXPERT_F},
    {TRAINER_SHELBY_1, OBJ_EVENT_GFX_EXPERT_F},
    {TRAINER_MARK, OBJ_EVENT_GFX_MANIAC},
    {TRAINER_MICAH, OBJ_EVENT_GFX_GENTLEMAN},
    {TRAINER_THOMAS, OBJ_EVENT_GFX_GENTLEMAN},
    {TRAINER_SHEILA, OBJ_EVENT_GFX_BEAUTY},
    {TRAINER_SHIRLEY, OBJ_EVENT_GFX_BEAUTY},
    {TRAINER_EDMOND, OBJ_EVENT_GFX_SAILOR},
    {TRAINER_ERNEST_1, OBJ_EVENT_GFX_SAILOR},
    {TRAINER_HITOSHI, OBJ_EVENT_GFX_BLACK_BELT},
    {TRAINER_TAKASHI, OBJ_EVENT_GFX_NINJA_BOY},
    {TRAINER_MARC, OBJ_EVENT_GFX_HIKER},
    {TRAINER_PRESTON, OBJ_EVENT_GFX_PSYCHIC_M},
    {TRAINER_VIRGIL, OBJ_EVENT_GFX_PSYCHIC_M},
};

static const struct BRTrainer sVeteranTrainers[] =
{
    {TRAINER_MARCEL, OBJ_EVENT_GFX_MAN_3},
    {TRAINER_FELIX, OBJ_EVENT_GFX_MAN_3},
    {TRAINER_RANDALL, OBJ_EVENT_GFX_MAN_3},
    {TRAINER_PARKER, OBJ_EVENT_GFX_MAN_3},
    {TRAINER_MARY, OBJ_EVENT_GFX_WOMAN_5},
    {TRAINER_ALEXIA, OBJ_EVENT_GFX_WOMAN_5},
    {TRAINER_JODY, OBJ_EVENT_GFX_WOMAN_5},
    {TRAINER_WENDY, OBJ_EVENT_GFX_WOMAN_5},
    {TRAINER_AARON, OBJ_EVENT_GFX_MAN_5},
    {TRAINER_NICOLAS_1, OBJ_EVENT_GFX_MAN_5},
    {TRAINER_LORENZO, OBJ_EVENT_GFX_CAMPER},
    {TRAINER_JENNA, OBJ_EVENT_GFX_PICNICKER},
    {TRAINER_LEAH, OBJ_EVENT_GFX_HEX_MANIAC},
    {TRAINER_PATRICIA, OBJ_EVENT_GFX_HEX_MANIAC},
    {TRAINER_DECLAN, OBJ_EVENT_GFX_SWIMMER_M},
    {TRAINER_BEVERLY, OBJ_EVENT_GFX_SWIMMER_F},
};

static const struct BRTrainer sChampionTrainers[] =
{
    {TRAINER_SIDNEY, OBJ_EVENT_GFX_SIDNEY},
    {TRAINER_PHOEBE, OBJ_EVENT_GFX_PHOEBE},
    {TRAINER_GLACIA, OBJ_EVENT_GFX_GLACIA},
    {TRAINER_DRAKE, OBJ_EVENT_GFX_DRAKE},
    {TRAINER_WALLACE, OBJ_EVENT_GFX_WALLACE},
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

static const u16 sTier3Species[] =
{
    SPECIES_MIGHTYENA,
    SPECIES_LINOONE,
    SPECIES_SWELLOW,
    SPECIES_PELIPPER,
    SPECIES_KIRLIA,
    SPECIES_BRELOOM,
    SPECIES_LOUDRED,
    SPECIES_VIGOROTH,
    SPECIES_NINJASK,
    SPECIES_HARIYAMA,
    SPECIES_NOSEPASS,
    SPECIES_MANECTRIC,
    SPECIES_PLUSLE,
    SPECIES_MINUN,
    SPECIES_ROSELIA,
    SPECIES_SWALOT,
    SPECIES_NUMEL,
    SPECIES_TORKOAL,
    SPECIES_SPINDA,
    SPECIES_ZANGOOSE,
    SPECIES_SEVIPER,
    SPECIES_LUNATONE,
    SPECIES_SOLROCK,
    SPECIES_CORPHISH,
    SPECIES_KECLEON,
    SPECIES_GOLBAT,
    SPECIES_GRAVELER,
    SPECIES_MACHOKE,
    SPECIES_KADABRA,
    SPECIES_SANDSLASH,
    SPECIES_MAWILE,
    SPECIES_SABLEYE,
    SPECIES_LOMBRE,
    SPECIES_NUZLEAF,
};

static const u16 sTier4Species[] =
{
    SPECIES_SHARPEDO,
    SPECIES_CAMERUPT,
    SPECIES_GRUMPIG,
    SPECIES_LAIRON,
    SPECIES_VIBRAVA,
    SPECIES_CACTURNE,
    SPECIES_ALTARIA,
    SPECIES_WHISCASH,
    SPECIES_CRAWDAUNT,
    SPECIES_CLAYDOL,
    SPECIES_BANETTE,
    SPECIES_DUSCLOPS,
    SPECIES_TROPIUS,
    SPECIES_CHIMECHO,
    SPECIES_ABSOL,
    SPECIES_SEALEO,
    SPECIES_MEDICHAM,
    SPECIES_MAGNETON,
    SPECIES_ELECTRODE,
    SPECIES_WEEZING,
    SPECIES_MUK,
    SPECIES_GOLDUCK,
    SPECIES_DODRIO,
    SPECIES_HARIYAMA,
    SPECIES_MANECTRIC,
    SPECIES_SWELLOW,
    SPECIES_BRELOOM,
    SPECIES_NINJASK,
    SPECIES_ZANGOOSE,
    SPECIES_SEVIPER,
    SPECIES_HAUNTER,
    SPECIES_RAICHU,
    SPECIES_NINETALES,
    SPECIES_HERACROSS,
    SPECIES_XATU,
    SPECIES_GIRAFARIG,
    SPECIES_SKARMORY,
    SPECIES_PINSIR,
};

static const u16 sTier5Species[] =
{
    SPECIES_SCEPTILE,
    SPECIES_BLAZIKEN,
    SPECIES_SWAMPERT,
    SPECIES_GARDEVOIR,
    SPECIES_SLAKING,
    SPECIES_LUDICOLO,
    SPECIES_SHIFTRY,
    SPECIES_HARIYAMA,
    SPECIES_EXPLOUD,
    SPECIES_AGGRON,
    SPECIES_MEDICHAM,
    SPECIES_MANECTRIC,
    SPECIES_SHARPEDO,
    SPECIES_WAILORD,
    SPECIES_CAMERUPT,
    SPECIES_GRUMPIG,
    SPECIES_FLYGON,
    SPECIES_CACTURNE,
    SPECIES_ALTARIA,
    SPECIES_WHISCASH,
    SPECIES_CRAWDAUNT,
    SPECIES_CLAYDOL,
    SPECIES_MILOTIC,
    SPECIES_BANETTE,
    SPECIES_DUSCLOPS,
    SPECIES_TROPIUS,
    SPECIES_ABSOL,
    SPECIES_GLALIE,
    SPECIES_WALREIN,
    SPECIES_CRADILY,
    SPECIES_ARMALDO,
    SPECIES_CROBAT,
    SPECIES_GOLEM,
    SPECIES_MACHAMP,
    SPECIES_ALAKAZAM,
    SPECIES_GENGAR,
    SPECIES_WEEZING,
    SPECIES_STARMIE,
    SPECIES_GYARADOS,
    SPECIES_HERACROSS,
    SPECIES_SKARMORY,
    SPECIES_RHYDON,
    SPECIES_DONPHAN,
};

static const u16 sTopSpecies[] =
{
    SPECIES_SALAMENCE,
    SPECIES_METAGROSS,
    SPECIES_TYRANITAR,
    SPECIES_DRAGONITE,
    SPECIES_SNORLAX,
    SPECIES_LAPRAS,
    SPECIES_ARCANINE,
    SPECIES_GYARADOS,
    SPECIES_MILOTIC,
    SPECIES_GENGAR,
    SPECIES_ALAKAZAM,
    SPECIES_MACHAMP,
    SPECIES_STARMIE,
    SPECIES_BLISSEY,
    SPECIES_SCIZOR,
    SPECIES_HERACROSS,
    SPECIES_SKARMORY,
    SPECIES_SLAKING,
    SPECIES_AGGRON,
    SPECIES_FLYGON,
    SPECIES_WALREIN,
    SPECIES_SWAMPERT,
    SPECIES_BLAZIKEN,
    SPECIES_SCEPTILE,
    SPECIES_GARDEVOIR,
    SPECIES_KINGDRA,
    SPECIES_UMBREON,
    SPECIES_ESPEON,
    SPECIES_HOUNDOOM,
    SPECIES_CROBAT,
    SPECIES_EXEGGUTOR,
    SPECIES_RHYDON,
    SPECIES_LUDICOLO,
    SPECIES_VAPOREON,
    SPECIES_JOLTEON,
};

static const struct BRTier sTiers[BR_NUM_TIERS] =
{
    // Tier 1
    {
        .trainers = sTier1Trainers,
        .numTrainers = ARRAY_COUNT(sTier1Trainers),
        .boss = {TRAINER_ROXANNE_1, OBJ_EVENT_GFX_ROXANNE},
        .fixedOrder = FALSE,
        .species = sTier1Species,
        .numSpecies = ARRAY_COUNT(sTier1Species),
        .fixedIV = 0,
        .monCount = {1, 2, 2, 2, 3},
        .minLevel = {3, 3, 4, 5, 6},
        .maxLevel = {4, 4, 5, 6, 8},
        .winMoney = {200, 300, 400, 500, 600},
        .clearBonus = 1500,
        .consolation = 200,
        .lossFee = 0,
        .levelCap = 15,
    },
    // Tier 2
    {
        .trainers = sTier2Trainers,
        .numTrainers = ARRAY_COUNT(sTier2Trainers),
        .boss = {TRAINER_BRAWLY_1, OBJ_EVENT_GFX_BRAWLY},
        .fixedOrder = FALSE,
        .species = sTier2Species,
        .numSpecies = ARRAY_COUNT(sTier2Species),
        .fixedIV = 12,
        .monCount = {2, 3, 3, 3, 4},
        .minLevel = {14, 15, 17, 18, 20},
        .maxLevel = {16, 17, 19, 21, 23},
        .winMoney = {600, 800, 1000, 1200, 1500},
        .clearBonus = 4000,
        .consolation = 500,
        .lossFee = 0,
        .levelCap = 25,
    },
    // Tier 3
    {
        .trainers = sTier3Trainers,
        .numTrainers = ARRAY_COUNT(sTier3Trainers),
        .boss = {TRAINER_WATTSON_1, OBJ_EVENT_GFX_WATTSON},
        .fixedOrder = FALSE,
        .species = sTier3Species,
        .numSpecies = ARRAY_COUNT(sTier3Species),
        .fixedIV = 16,
        .monCount = {3, 3, 3, 4, 4},
        .minLevel = {22, 23, 25, 26, 28},
        .maxLevel = {24, 25, 27, 29, 31},
        .winMoney = {1000, 1300, 1600, 2000, 2500},
        .clearBonus = 6000,
        .consolation = 0,
        .lossFee = 500,
        .levelCap = 33,
    },
    // Tier 4
    {
        .trainers = sTier4Trainers,
        .numTrainers = ARRAY_COUNT(sTier4Trainers),
        .boss = {TRAINER_FLANNERY_1, OBJ_EVENT_GFX_FLANNERY},
        .fixedOrder = FALSE,
        .species = sTier4Species,
        .numSpecies = ARRAY_COUNT(sTier4Species),
        .fixedIV = 20,
        .monCount = {3, 3, 4, 4, 5},
        .minLevel = {30, 31, 33, 34, 36},
        .maxLevel = {32, 33, 35, 37, 39},
        .winMoney = {1500, 2000, 2500, 3000, 4000},
        .clearBonus = 9000,
        .consolation = 0,
        .lossFee = 1000,
        .levelCap = 41,
    },
    // Tier 5
    {
        .trainers = sVeteranTrainers,
        .numTrainers = ARRAY_COUNT(sVeteranTrainers),
        .boss = {TRAINER_NORMAN_1, OBJ_EVENT_GFX_NORMAN},
        .fixedOrder = FALSE,
        .species = sTier5Species,
        .numSpecies = ARRAY_COUNT(sTier5Species),
        .fixedIV = 24,
        .monCount = {3, 4, 4, 5, 5},
        .minLevel = {38, 39, 41, 42, 44},
        .maxLevel = {40, 41, 43, 45, 47},
        .winMoney = {2500, 3000, 3500, 4000, 5500},
        .clearBonus = 12000,
        .consolation = 0,
        .lossFee = 2000,
        .levelCap = 49,
    },
    // Tier 6
    {
        .trainers = sVeteranTrainers,
        .numTrainers = ARRAY_COUNT(sVeteranTrainers),
        .boss = {TRAINER_WINONA_1, OBJ_EVENT_GFX_WINONA},
        .fixedOrder = FALSE,
        .species = sTopSpecies,
        .numSpecies = ARRAY_COUNT(sTopSpecies),
        .fixedIV = 28,
        .monCount = {4, 4, 5, 5, 6},
        .minLevel = {46, 47, 49, 50, 52},
        .maxLevel = {48, 49, 51, 53, 55},
        .winMoney = {3500, 4000, 5000, 6000, 8000},
        .clearBonus = 16000,
        .consolation = 0,
        .lossFee = 3000,
        .levelCap = 57,
    },
    // Tier 7
    {
        .trainers = sVeteranTrainers,
        .numTrainers = ARRAY_COUNT(sVeteranTrainers),
        .boss = {TRAINER_JUAN_1, OBJ_EVENT_GFX_JUAN},
        .fixedOrder = FALSE,
        .species = sTopSpecies,
        .numSpecies = ARRAY_COUNT(sTopSpecies),
        .fixedIV = 31,
        .monCount = {4, 5, 5, 6, 6},
        .minLevel = {54, 55, 57, 58, 60},
        .maxLevel = {56, 57, 59, 61, 63},
        .winMoney = {5000, 6000, 7000, 8000, 10000},
        .clearBonus = 20000,
        .consolation = 0,
        .lossFee = 4000,
        .levelCap = 65,
    },
    // Tier 8
    {
        .trainers = sVeteranTrainers,
        .numTrainers = ARRAY_COUNT(sVeteranTrainers),
        .boss = {TRAINER_STEVEN, OBJ_EVENT_GFX_STEVEN},
        .fixedOrder = FALSE,
        .species = sTopSpecies,
        .numSpecies = ARRAY_COUNT(sTopSpecies),
        .fixedIV = 31,
        .monCount = {5, 5, 6, 6, 6},
        .minLevel = {62, 63, 65, 66, 68},
        .maxLevel = {64, 65, 67, 69, 71},
        .winMoney = {6000, 7000, 8000, 10000, 13000},
        .clearBonus = 30000,
        .consolation = 0,
        .lossFee = 5000,
        .levelCap = 73,
    },
    // Champion tier
    {
        .trainers = sChampionTrainers,
        .numTrainers = ARRAY_COUNT(sChampionTrainers),
        .fixedOrder = TRUE,
        .species = sTopSpecies,
        .numSpecies = ARRAY_COUNT(sTopSpecies),
        .fixedIV = 31,
        .monCount = {5, 5, 5, 5, 6},
        .minLevel = {72, 73, 74, 75, 76},
        .maxLevel = {74, 75, 76, 77, 78},
        .winMoney = {10000, 10000, 10000, 10000, 20000},
        .clearBonus = 50000,
        .consolation = 0,
        .lossFee = 8000,
        .levelCap = 80,
    },
};

// Levels added to wild Pokemon in the hub grass, by number of tiers cleared
static const u8 sWildLevelBonus[BR_NUM_TIERS + 1] = {0, 8, 17, 25, 33, 41, 49, 57, 64, 70};

// Pokemon that may only enter the Champion tier
static const u16 sBannedSpecies[] =
{
    SPECIES_ARTICUNO, SPECIES_ZAPDOS, SPECIES_MOLTRES, SPECIES_MEWTWO, SPECIES_MEW,
    SPECIES_RAIKOU, SPECIES_ENTEI, SPECIES_SUICUNE, SPECIES_LUGIA, SPECIES_HO_OH, SPECIES_CELEBI,
    SPECIES_REGIROCK, SPECIES_REGICE, SPECIES_REGISTEEL, SPECIES_LATIAS, SPECIES_LATIOS,
    SPECIES_KYOGRE, SPECIES_GROUDON, SPECIES_RAYQUAZA, SPECIES_JIRACHI, SPECIES_DEOXYS,
};

struct BRShopMon
{
    u16 species;
    u16 basePrice; // multiplied by (tiers cleared + 1)
};

// Pokemon shop stock. Index 0: standard clerk, index 1: rare clerk.
static const struct BRShopMon sShopMons[2][BR_SHOP_MONS_PER_LIST] =
{
    {
        {SPECIES_ZIGZAGOON, 1000},
        {SPECIES_WINGULL,   1500},
        {SPECIES_MAKUHITA,  2000},
        {SPECIES_GEODUDE,   2000},
        {SPECIES_GASTLY,    3000},
        {SPECIES_TRAPINCH,  3500},
    },
    {
        {SPECIES_HERACROSS, 3000},
        {SPECIES_SKARMORY,  3000},
        {SPECIES_DRATINI,   4000},
        {SPECIES_LARVITAR,  4000},
        {SPECIES_BAGON,     4000},
        {SPECIES_BELDUM,    4000},
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
extern const u8 BattleRoyale_Text_BossIntro[];
extern const u8 BattleRoyale_Text_BossDefeat[];
extern const u8 BattleRoyale_Text_ChampionIntro[];
extern const u8 BattleRoyale_Text_ChampionDefeat[];
extern const u8 BattleRoyale_Text_TierName[];
extern const u8 BattleRoyale_Text_ChampionTierName[];

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

static u16 GetCurrentTierNum(void)
{
    u16 tier = VarGet(VAR_BR_TIER);

    if (tier == 0 || tier > BR_NUM_TIERS)
        tier = 1;
    return tier;
}

static const struct BRTier *GetCurrentTier(void)
{
    return &sTiers[GetCurrentTierNum() - 1];
}

static u8 GetBattleNum(void)
{
    u16 num = VarGet(VAR_BR_BATTLE_NUM);

    if (num >= BR_BATTLES_PER_TIER)
        num = BR_BATTLES_PER_TIER - 1;
    return num;
}

static u16 GetTiersCleared(void)
{
    u16 cleared = VarGet(VAR_BR_TIERS_CLEARED);

    if (cleared > BR_NUM_TIERS)
        cleared = BR_NUM_TIERS;
    return cleared;
}

static void EndRun(void)
{
    VarSet(VAR_BR_TIER, 0);
    VarSet(VAR_BR_BATTLE_NUM, 0);
    VarSet(VAR_BR_USED_TRAINERS, 0);
    VarSet(VAR_BR_PAUSED, 0);
}

// Writes "TIER n" or "CHAMPION TIER" to dest
static void BufferTierName(u8 *dest, u16 tierNum)
{
    if (tierNum >= BR_NUM_TIERS)
    {
        StringCopy(dest, BattleRoyale_Text_ChampionTierName);
    }
    else
    {
        dest = StringCopy(dest, BattleRoyale_Text_TierName);
        ConvertIntToDecimalStringN(dest, tierNum, STR_CONV_MODE_LEFT_ALIGN, 1);
    }
}

// The soft level cap follows progress: it is the cap of the first tier the
// player has not cleared yet. Once every tier is cleared there is no cap.
// Traded Pokemon above this level disobey (see IsMonDisobedient).
u8 BattleRoyale_GetLevelCap(void)
{
    u16 cleared = GetTiersCleared();

    if (cleared >= BR_NUM_TIERS || FlagGet(FLAG_BR_CHEAT_NO_LEVEL_CAP))
        return MAX_LEVEL;
    return sTiers[cleared].levelCap;
}

// Wild Pokemon in the hub grass get stronger as tiers are cleared
u8 BattleRoyale_GetWildLevelBonus(void)
{
    return sWildLevelBonus[GetTiersCleared()];
}

// special: VAR_RESULT = number of the highest tier the player may enter
void BattleRoyale_GetHighestUnlockedTier(void)
{
    u16 tier = GetTiersCleared() + 1;

    if (tier > BR_NUM_TIERS || FlagGet(FLAG_BR_CHEAT_ALL_TIERS))
        tier = BR_NUM_TIERS;
    gSpecialVar_Result = tier;
}

// special: describes the tier in VAR_0x8004. STR_VAR_1 = name, STR_VAR_2 =
// highest opponent level, STR_VAR_3 = loss fee. VAR_RESULT = TRUE if losing
// costs money.
void BattleRoyale_BufferTierInfo(void)
{
    u16 tierNum = gSpecialVar_0x8004;
    const struct BRTier *tier;

    if (tierNum == 0 || tierNum > BR_NUM_TIERS)
        tierNum = 1;
    tier = &sTiers[tierNum - 1];
    BufferTierName(gStringVar1, tierNum);
    ConvertIntToDecimalStringN(gStringVar2, tier->maxLevel[BR_BATTLES_PER_TIER - 1], STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, tier->lossFee, STR_CONV_MODE_LEFT_ALIGN, 5);
    gSpecialVar_Result = (tier->lossFee != 0);
}

// special: legendary Pokemon may only enter the Champion tier. Checks the
// party against the tier in VAR_0x8004. VAR_RESULT = TRUE if the party is
// allowed; otherwise the offending species name is in STR_VAR_1.
void BattleRoyale_CheckPartyAllowed(void)
{
    u32 i, j;

    gSpecialVar_Result = TRUE;
    if (gSpecialVar_0x8004 >= BR_NUM_TIERS)
        return;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);

        for (j = 0; j < ARRAY_COUNT(sBannedSpecies); j++)
        {
            if (species == sBannedSpecies[j])
            {
                StringCopy(gStringVar1, gSpeciesNames[species]);
                gSpecialVar_Result = FALSE;
                return;
            }
        }
    }
}

// special: starts a run of the tier in VAR_0x8004
void BattleRoyale_StartRun(void)
{
    VarSet(VAR_BR_TIER, gSpecialVar_0x8004);
    VarSet(VAR_BR_BATTLE_NUM, 0);
    VarSet(VAR_BR_USED_TRAINERS, 0);
    VarSet(VAR_BR_PAUSED, 0);
}

// special: picks the next opponent. Sets their overworld sprite, puts their
// intro line in STR_VAR_4 and the battle number in STR_VAR_1.
void BattleRoyale_SetUpOpponent(void)
{
    const struct BRTier *tier = GetCurrentTier();
    u8 battleNum = GetBattleNum();
    const struct BRTrainer *trainer;
    const u8 *intro;

    sIsBoss = FALSE;
    sTextId = Random() % ARRAY_COUNT(sIntroTexts);
    intro = sIntroTexts[sTextId];

    if (tier->fixedOrder)
    {
        trainer = &tier->trainers[battleNum % tier->numTrainers];
        if (battleNum == BR_BATTLES_PER_TIER - 1)
        {
            sIsBoss = TRUE;
            intro = BattleRoyale_Text_ChampionIntro;
        }
    }
    else if (battleNum == BR_BATTLES_PER_TIER - 1 && tier->boss.trainerId != TRAINER_NONE)
    {
        trainer = &tier->boss;
        sIsBoss = TRUE;
        intro = BattleRoyale_Text_BossIntro;
    }
    else
    {
        u16 used = VarGet(VAR_BR_USED_TRAINERS);
        u16 all = (tier->numTrainers >= 16) ? 0xFFFF : (1 << tier->numTrainers) - 1;
        u8 id;

        if ((used & all) == all)
            used = 0;
        do
        {
            id = Random() % tier->numTrainers;
        } while (used & (1 << id));
        VarSet(VAR_BR_USED_TRAINERS, used | (1 << id));
        trainer = &tier->trainers[id];
    }

    gTrainerBattleOpponent_A = trainer->trainerId;
    VarSet(VAR_OBJ_GFX_ID_0, trainer->gfxId);
    StringExpandPlaceholders(gStringVar4, intro);
    ConvertIntToDecimalStringN(gStringVar1, battleNum + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
}

const u8 *BattleRoyale_GetDefeatText(void)
{
    if (sIsBoss)
    {
        if (GetCurrentTier()->fixedOrder)
            return BattleRoyale_Text_ChampionDefeat;
        return BattleRoyale_Text_BossDefeat;
    }
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

    if (count > PARTY_SIZE)
        count = PARTY_SIZE;
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

// special: call after a win. VAR_RESULT: 0 = more battles to go (next battle
// number in STR_VAR_1), 1 = tier cleared, 2 = Champion tier cleared. On a
// clear the bonus is paid and written to STR_VAR_1, the tier name to STR_VAR_2.
void BattleRoyale_OnWin(void)
{
    const struct BRTier *tier = GetCurrentTier();
    u16 battleNum = VarGet(VAR_BR_BATTLE_NUM) + 1;

    VarSet(VAR_BR_BATTLE_NUM, battleNum);
    if (battleNum >= BR_BATTLES_PER_TIER)
    {
        u16 tierNum = GetCurrentTierNum();

        AddMoney(&gSaveBlock1Ptr->money, tier->clearBonus);
        ConvertIntToDecimalStringN(gStringVar1, tier->clearBonus, STR_CONV_MODE_LEFT_ALIGN, 6);
        BufferTierName(gStringVar2, tierNum);
        if (VarGet(VAR_BR_TIERS_CLEARED) < tierNum)
            VarSet(VAR_BR_TIERS_CLEARED, tierNum);
        EndRun();
        gSpecialVar_Result = (tierNum >= BR_NUM_TIERS) ? 2 : 1;
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, battleNum + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
        gSpecialVar_Result = 0;
    }
}

// special: call after a loss; ends the run. VAR_RESULT:
//  BR_LOSS_CONSOLATION - a consolation prize was paid (amount in STR_VAR_1)
//  BR_LOSS_FEE         - a fee was taken (amount actually paid in STR_VAR_1)
//  BR_LOSS_NOTHING     - nothing paid either way
//  BR_LOSS_GAME_OVER   - the tier has a fee and the player has no money at all
void BattleRoyale_OnLoss(void)
{
    const struct BRTier *tier = GetCurrentTier();
    u32 money = GetMoney(&gSaveBlock1Ptr->money);

    EndRun();
    if (tier->lossFee != 0)
    {
        u32 fee = tier->lossFee;

        if (money == 0)
        {
            gSpecialVar_Result = BR_LOSS_GAME_OVER;
            return;
        }
        if (fee > money)
            fee = money;
        RemoveMoney(&gSaveBlock1Ptr->money, fee);
        ConvertIntToDecimalStringN(gStringVar1, fee, STR_CONV_MODE_LEFT_ALIGN, 5);
        gSpecialVar_Result = BR_LOSS_FEE;
    }
    else if (tier->consolation != 0)
    {
        AddMoney(&gSaveBlock1Ptr->money, tier->consolation);
        ConvertIntToDecimalStringN(gStringVar1, tier->consolation, STR_CONV_MODE_LEFT_ALIGN, 5);
        gSpecialVar_Result = BR_LOSS_CONSOLATION;
    }
    else
    {
        gSpecialVar_Result = BR_LOSS_NOTHING;
    }
}

// special: the player chose to stop between battles
void BattleRoyale_Retire(void)
{
    EndRun();
}

// special: writes the paused run's tier name to STR_VAR_1 and the number of
// the next battle to STR_VAR_2. Also puts the tier number in VAR_0x8004.
void BattleRoyale_BufferRunInfo(void)
{
    gSpecialVar_0x8004 = GetCurrentTierNum();
    BufferTierName(gStringVar1, gSpecialVar_0x8004);
    ConvertIntToDecimalStringN(gStringVar2, GetBattleNum() + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
}

// Called when the whole party faints outside the Tower. With no money left,
// that is a game over (shown by the hub town's map script).
void BattleRoyale_OnWhiteOut(void)
{
    if (GetMoney(&gSaveBlock1Ptr->money) == 0)
        VarSet(VAR_BR_GAME_OVER, 1);
}

// special: back to the title screen. The save file is left untouched.
void BattleRoyale_GameOver(void)
{
    VarSet(VAR_BR_GAME_OVER, 0);
    DoSoftReset();
}

// ---------------------------------------------------------------------------
// Pokemon shop. Stock is sold at a level that follows the level cap, and
// prices rise with every tier cleared.
// ---------------------------------------------------------------------------

static u8 GetShopLevel(void)
{
    u16 cleared = GetTiersCleared();
    u8 cap;

    // follows real progress, not the level-cap cheat
    if (cleared >= BR_NUM_TIERS)
        cleared = BR_NUM_TIERS - 1;
    cap = sTiers[cleared].levelCap;
    if (cap < 15)
        return 5;
    return cap - 10;
}

static const struct BRShopMon *GetShopMon(void)
{
    return &sShopMons[gSpecialVar_0x8005 % 2][gSpecialVar_0x8004 % BR_SHOP_MONS_PER_LIST];
}

static u32 GetShopPrice(const struct BRShopMon *mon)
{
    return mon->basePrice * (GetTiersCleared() + 1);
}

// special: VAR_0x8005 = list, VAR_0x8004 = item. Writes the species name to
// STR_VAR_1, its level to STR_VAR_2 and its price to STR_VAR_3.
void BattleRoyale_BufferShopOffer(void)
{
    const struct BRShopMon *mon = GetShopMon();

    StringCopy(gStringVar1, gSpeciesNames[mon->species]);
    ConvertIntToDecimalStringN(gStringVar2, GetShopLevel(), STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, GetShopPrice(mon), STR_CONV_MODE_LEFT_ALIGN, 6);
}

// special: buys the Pokemon chosen with VAR_0x8005 / VAR_0x8004. VAR_RESULT:
//  BR_SHOP_NO_MONEY, BR_SHOP_NO_ROOM, BR_SHOP_TO_PARTY or BR_SHOP_TO_PC.
// The species is left in VAR_TEMP_TRANSFERRED_SPECIES.
void BattleRoyale_BuyShopMon(void)
{
    const struct BRShopMon *mon = GetShopMon();
    u32 price = GetShopPrice(mon);
    u8 result;

    if (!IsEnoughMoney(&gSaveBlock1Ptr->money, price))
    {
        gSpecialVar_Result = BR_SHOP_NO_MONEY;
        return;
    }
    VarSet(VAR_TEMP_TRANSFERRED_SPECIES, mon->species);
    result = ScriptGiveMon(mon->species, GetShopLevel(), ITEM_NONE, 0, 0, 0);
    if (result == MON_CANT_GIVE)
    {
        gSpecialVar_Result = BR_SHOP_NO_ROOM;
        return;
    }
    RemoveMoney(&gSaveBlock1Ptr->money, price);
    gSpecialVar_Result = (result == MON_GIVEN_TO_PC) ? BR_SHOP_TO_PC : BR_SHOP_TO_PARTY;
}
