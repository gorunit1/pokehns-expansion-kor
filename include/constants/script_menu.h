#ifndef GUARD_SCRIPT_MENU_CONSTANTS_H
#define GUARD_SCRIPT_MENU_CONSTANTS_H

#define MULTICHOICE(name) {name, ARRAY_COUNT(name)}

#define MAX_MULTICHOICE_WIDTH 28

#define MULTI_B_PRESSED  127

// Multichoice Ids
enum
{
    MULTI_BRINEY_ON_DEWFORD                          = 0,
    MULTI_PC                                         = 1, // Exit only, populated by CreatePCMultichoice
    MULTI_ENTERINFO                                  = 2,
    MULTI_CONTEST_INFO                               = 3,
    MULTI_CONTEST_TYPE                               = 4,
    MULTI_BASE_PC_NO_REGISTRY                        = 5,
    MULTI_BASE_PC_WITH_REGISTRY                      = 6,
    MULTI_REGISTER_MENU                              = 7,
    MULTI_SSTIDAL_LILYCOVE                           = 8, // Exit only, populated by CreateLilycoveSSTidalMultichoice
    MULTI_UNUSED_9                                   = 9,
    MULTI_UNUSED_10                                  = 10,
    MULTI_FRONTIER_PASS_INFO                         = 11,
    MULTI_BIKE                                       = 12,
    MULTI_STATUS_INFO                                = 13,
    MULTI_BRINEY_OFF_DEWFORD                         = 14,
    MULTI_UNUSED_15                                  = 15,
    MULTI_VIEWED_PAINTINGS                           = 16,
    MULTI_YESNOINFO                                  = 17,
    MULTI_BATTLE_MODE                                = 18,
    MULTI_UNUSED_19                                  = 19,
    MULTI_YESNOINFO_2                                = 20,
    MULTI_UNUSED_21                                  = 21,
    MULTI_UNUSED_22                                  = 22,
    MULTI_CHALLENGEINFO                              = 23,
    MULTI_LEVEL_MODE                                 = 24,
    MULTI_MECHADOLL1_Q1                              = 25,
    MULTI_MECHADOLL1_Q2                              = 26,
    MULTI_MECHADOLL1_Q3                              = 27,
    MULTI_MECHADOLL2_Q1                              = 28,
    MULTI_MECHADOLL2_Q2                              = 29,
    MULTI_MECHADOLL2_Q3                              = 30,
    MULTI_MECHADOLL3_Q1                              = 31,
    MULTI_MECHADOLL3_Q2                              = 32,
    MULTI_MECHADOLL3_Q3                              = 33,
    MULTI_MECHADOLL4_Q1                              = 34,
    MULTI_MECHADOLL4_Q2                              = 35,
    MULTI_MECHADOLL4_Q3                              = 36,
    MULTI_MECHADOLL5_Q1                              = 37,
    MULTI_MECHADOLL5_Q2                              = 38,
    MULTI_MECHADOLL5_Q3                              = 39,
    MULTI_UNUSED_40                                  = 40,
    MULTI_UNUSED_41                                  = 41,
    MULTI_VENDING_MACHINE                            = 42,
    MULTI_MACH_BIKE_INFO                             = 43,
    MULTI_ACRO_BIKE_INFO                             = 44,
    MULTI_SATISFACTION                               = 45,
    MULTI_STERN_DEEPSEA                              = 46,
    MULTI_UNUSED_ASH_VENDOR                          = 47, // Replaced by scrollable multichoice
    MULTI_GAME_CORNER_DOLLS                          = 48,
    MULTI_GAME_CORNER_COINS                          = 49,
    MULTI_HOWS_FISHING                               = 50,
    MULTI_UNUSED_51                                  = 51,
    MULTI_SSTIDAL_SLATEPORT_WITH_BF                  = 52,
    MULTI_SSTIDAL_BATTLE_FRONTIER                    = 53,
    MULTI_RIGHTLEFT                                  = 54,
    MULTI_GAME_CORNER_TMS                            = 55,
    MULTI_SSTIDAL_SLATEPORT_NO_BF                    = 56,
    MULTI_FLOORS                                     = 57,
    MULTI_SHARDS_R                                   = 58,
    MULTI_SHARDS_Y                                   = 59,
    MULTI_SHARDS_RY                                  = 60,
    MULTI_SHARDS_B                                   = 61,
    MULTI_SHARDS_RB                                  = 62,
    MULTI_SHARDS_YB                                  = 63,
    MULTI_SHARDS_RYB                                 = 64,
    MULTI_SHARDS_G                                   = 65,
    MULTI_SHARDS_RG                                  = 66,
    MULTI_SHARDS_YG                                  = 67,
    MULTI_SHARDS_RYG                                 = 68,
    MULTI_SHARDS_BG                                  = 69,
    MULTI_SHARDS_RBG                                 = 70,
    MULTI_SHARDS_YBG                                 = 71,
    MULTI_SHARDS_RYBG                                = 72,
    MULTI_TOURNEY_WITH_RECORD                        = 73,
    MULTI_CABLE_CLUB_NO_RECORD_MIX                   = 74,
    MULTI_WIRELESS_NO_RECORD_BERRY                   = 75,
    MULTI_CABLE_CLUB_WITH_RECORD_MIX                 = 76,
    MULTI_WIRELESS_NO_BERRY                          = 77,
    MULTI_WIRELESS_NO_RECORD                         = 78,
    MULTI_WIRELESS_ALL_SERVICES                      = 79,
    MULTI_WIRELESS_MINIGAME                          = 80,
    MULTI_LINK_LEADER                                = 81,
    MULTI_CONTEST_RANK                               = 82,
    MULTI_FRONTIER_ITEM_CHOOSE                       = 83,
    MULTI_LINK_CONTEST_INFO                          = 84,
    MULTI_LINK_CONTEST_MODE                          = 85,
    MULTI_FORCED_START_MENU                          = 86,
    MULTI_FRONTIER_GAMBLER_BET                       = 87,
    MULTI_TENT                                       = 88,
    MULTI_UNUSED_SSTIDAL_1                           = 89, // These 4 were replaced by CreateLilycoveSSTidalMultichoice
    MULTI_UNUSED_SSTIDAL_2                           = 90,
    MULTI_UNUSED_SSTIDAL_3                           = 91,
    MULTI_UNUSED_SSTIDAL_4                           = 92,
    MULTI_FOSSIL                                     = 93,
    MULTI_YESNO                                      = 94,
    MULTI_FRONTIER_RULES                             = 95,
    MULTI_BATTLE_ARENA_RULES                         = 96,
    MULTI_BATTLE_TOWER_RULES                         = 97,
    MULTI_BATTLE_DOME_RULES                          = 98,
    MULTI_BATTLE_FACTORY_RULES                       = 99,
    MULTI_BATTLE_PALACE_RULES                        = 100,
    MULTI_BATTLE_PYRAMID_RULES                       = 101,
    MULTI_BATTLE_PIKE_RULES                          = 102,
    MULTI_GO_ON_RECORD_REST_RETIRE                   = 103,
    MULTI_GO_ON_REST_RETIRE                          = 104,
    MULTI_GO_ON_RECORD_RETIRE                        = 105,
    MULTI_GO_ON_RETIRE                               = 106,
    MULTI_TOURNEY_NO_RECORD                          = 107,
    MULTI_TV_LATI                                    = 108,
    MULTI_BATTLE_TOWER_FEELINGS                      = 109,
    MULTI_WHERES_RAYQUAZA                            = 110,
    MULTI_SLATEPORT_TENT_RULES                       = 111,
    MULTI_FALLARBOR_TENT_RULES                       = 112,
    MULTI_TAG_MATCH_TYPE                             = 113,
    MULTI_BERRY_PLOT                                 = 114,
    MULTI_BIKE_SHOP                                  = 115,
    MULTI_EEVEELUTIONS                               = 116,
    MULTI_ISLAND_23                                  = 117,
    MULTI_ISLAND_13                                  = 118,
    MULTI_ISLAND_12                                  = 119,
    MULTI_SEVII_NAVEL                                = 120,
    MULTI_SEVII_BIRTH                                = 121,
    MULTI_SEVII_NAVEL_BIRTH                          = 122,
    MULTI_SEAGALLOP_123                              = 123,
    MULTI_SEAGALLOP_V23                              = 124,
    MULTI_SEAGALLOP_V13                              = 125,
    MULTI_SEAGALLOP_V12                              = 126,
    MULTI_SEAGALLOP_VERMILION                        = 127,
    MULTI_GAME_CORNER_POKEMON_PRIZES                 = 128,
    MULTI_GAME_CORNER_TMPRIZES                       = 129,
    MULTI_GAME_CORNER_BATTLE_ITEM_PRIZES             = 130,
    MULTI_DEPT_STORE_ELEVATOR                        = 131,
    MULTI_GAME_CORNER_COIN_PURCHASE_COUNTER          = 132,
    MULTI_LINKED_DIRECT_UNION                        = 133,
    MULTI_CELADON_VENDING_MACHINE                    = 134,
    MULTI_THIRSTY_GIRL_FRESH_WATER                   = 135,
    MULTI_THIRSTY_GIRL_SODA_POP                      = 136,
    MULTI_THIRSTY_GIRL_FRESH_WATER_SODA_POP          = 137,
    MULTI_THIRSTY_GIRL_LEMONADE                      = 138,
    MULTI_THIRSTY_GIRL_FRESH_WATER_LEMONADE          = 139,
    MULTI_THIRSTY_GIRL_SODA_POP_LEMONADE             = 140,
    MULTI_THIRSTY_GIRL_FRESH_WATER_SODA_POP_LEMONADE = 141,
    MULTI_ROCKET_HIDEOUT_ELEVATOR                    = 142,
    MULTI_HELIX                                      = 143,
    MULTI_DOME                                       = 144,
    MULTI_AMBER                                      = 145,
    MULTI_HELIX_AMBER                                = 146,
    MULTI_DOME_AMBER                                 = 147,
    MULTI_MUSHROOMS                                  = 148,
    MULTI_ROOFTOP_B1F                                = 149,
    MULTI_TRAINER_TOWER_MODE                         = 150,
    MULTI_TRAINER_CARD_ICON_TINT                     = 151,
    MULTI_HOF_QUIT                                   = 152,
    MULTI_EGGS_QUIT                                  = 153,
    MULTI_VICTORIES_QUIT                             = 154,
    MULTI_HOF_EGGS_QUIT                              = 155,
    MULTI_HOF_VICTORIES_QUIT                         = 156,
    MULTI_EGGS_VICTORIES_QUIT                        = 157,
    MULTI_HOF_EGGS_VICTORIES_QUIT                    = 158,
    MULTI_DAYS_OF_WEEK                               = 159,
    MULTI_KURT_BALLS                                 = 160,
    MULTI_PRIZE_MONS                                 = 161,
    MULTI_7FLOORS                                    = 162,
    MULTI_GOLDSILVER                                 = 163,
    MULTI_ELDERQUIIZ1                                = 164,
    MULTI_ELDERQUIIZ2                                = 165,
    MULTI_ELDERQUIIZ3                                = 166,
    MULTI_ELDERQUIIZ4                                = 167,
    MULTI_ELDERQUIIZ5                                = 168,
    MULTI_OLIVINE_HARBOR                             = 169,
    MULTI_VERMILION_HARBOR                           = 170,
    MULTI_HOENN_STARTERS                             = 171,
    MULTI_5FLOORS                                    = 172,
    MULTI_MOM_MENU                                   = 173,
    MULTI_LINK_SERVICES_HNS                          = 174,
    MULTI_BATTLE_MODE_HNS                            = 175,
    MULTI_FOSSIL_HNS                                 = 176,
    MULTI_GAME_CORNER_DOLLS2                         = 177,
};

#define MULTI_NONE 255

// Lilycove SS Tidal Multichoice Selections
#define SSTIDAL_SELECTION_SLATEPORT        0
#define SSTIDAL_SELECTION_BATTLE_FRONTIER  1
#define SSTIDAL_SELECTION_SOUTHERN_ISLAND  2
#define SSTIDAL_SELECTION_NAVEL_ROCK       3
#define SSTIDAL_SELECTION_BIRTH_ISLAND     4
#define SSTIDAL_SELECTION_FARAWAY_ISLAND   5
#define SSTIDAL_SELECTION_EXIT             6
#define SSTIDAL_SELECTION_COUNT            7

// Std String Ids
#define STDSTRING_COOL             0
#define STDSTRING_BEAUTY           1
#define STDSTRING_CUTE             2
#define STDSTRING_SMART            3
#define STDSTRING_TOUGH            4
#define STDSTRING_NORMAL           5
#define STDSTRING_SUPER            6
#define STDSTRING_HYPER            7
#define STDSTRING_MASTER           8
#define STDSTRING_COOL2            9
#define STDSTRING_BEAUTY2          10
#define STDSTRING_CUTE2            11
#define STDSTRING_SMART2           12
#define STDSTRING_TOUGH2           13
#define STDSTRING_ITEMS            14
#define STDSTRING_KEYITEMS         15
#define STDSTRING_POKEBALLS        16
#define STDSTRING_TMHMS            17
#define STDSTRING_BERRIES          18
#define STDSTRING_SINGLE           19
#define STDSTRING_DOUBLE           20
#define STDSTRING_MULTI            21
#define STDSTRING_MULTI_LINK       22
#define STDSTRING_BATTLE_TOWER     23
#define STDSTRING_BATTLE_DOME      24
#define STDSTRING_BATTLE_FACTORY   25
#define STDSTRING_BATTLE_PALACE    26
#define STDSTRING_BATTLE_ARENA     27
#define STDSTRING_BATTLE_PIKE      28
#define STDSTRING_BATTLE_PYRAMID   29
#define STDSTRING_BOULDER_BADGE    30
#define STDSTRING_CASCADE_BADGE    31
#define STDSTRING_THUNDER_BADGE    32
#define STDSTRING_RAINBOW_BADGE    33
#define STDSTRING_SOUL_BADGE       34
#define STDSTRING_MARSH_BADGE      35
#define STDSTRING_VOLCANO_BADGE    36
#define STDSTRING_EARTH_BADGE      37
#define STDSTRING_COINS            38
#define STDSTRING_MEDICINE         39
#if I_COMBINE_BAG_POCKETS == FALSE
#define STDSTRING_BATTLE_ITEMS     40
#define STDSTRING_TREASURES        41
#endif

// Dynamic Multichoice Callbacks

#define DYN_MULTICHOICE_CB_DEBUG      0
#define DYN_MULTICHOICE_CB_SHOW_ITEM  1
#define DYN_MULTICHOICE_CB_NONE       255

#endif //GUARD_SCRIPT_MENU_CONSTANTS_H
