#ifndef GUARD_CONSTANTS_EVENT_OBJECTS_H
#define GUARD_CONSTANTS_EVENT_OBJECTS_H

#include "constants/global.h"
#include "constants/map_event_ids.h"

#define PLAYER_AVATAR_GFX_MALE_NORMAL     (IS_HNS ? OBJ_EVENT_GFX_GOLD_NORMAL_HNS     : IS_FRLG ? OBJ_EVENT_GFX_RED_NORMAL     : OBJ_EVENT_GFX_BRENDAN_NORMAL)
#define PLAYER_AVATAR_GFX_MALE_MACH_BIKE  (IS_HNS ? OBJ_EVENT_GFX_GOLD_MACH_BIKE_HNS : IS_FRLG ? OBJ_EVENT_GFX_RED_BIKE       : OBJ_EVENT_GFX_BRENDAN_MACH_BIKE)
#define PLAYER_AVATAR_GFX_MALE_ACRO_BIKE  (IS_HNS ? OBJ_EVENT_GFX_GOLD_ACRO_BIKE_HNS : IS_FRLG ? OBJ_EVENT_GFX_RED_BIKE       : OBJ_EVENT_GFX_BRENDAN_ACRO_BIKE)
#define PLAYER_AVATAR_GFX_MALE_SURFING    (IS_HNS ? OBJ_EVENT_GFX_GOLD_SURFING_HNS   : IS_FRLG ? OBJ_EVENT_GFX_RED_SURF       : OBJ_EVENT_GFX_BRENDAN_SURFING)
#define PLAYER_AVATAR_GFX_MALE_UNDERWATER (IS_HNS ? OBJ_EVENT_GFX_GOLD_UNDERWATER_HNS : IS_FRLG ? OBJ_EVENT_GFX_RED_SURF       : OBJ_EVENT_GFX_BRENDAN_UNDERWATER)
#define PLAYER_AVATAR_GFX_MALE_FIELD_MOVE (IS_HNS ? OBJ_EVENT_GFX_GOLD_FIELD_MOVE_HNS : IS_FRLG ? OBJ_EVENT_GFX_RED_FIELD_MOVE : OBJ_EVENT_GFX_BRENDAN_FIELD_MOVE)
#define PLAYER_AVATAR_GFX_MALE_FISHING    (IS_HNS ? OBJ_EVENT_GFX_GOLD_FISHING_HNS   : IS_FRLG ? OBJ_EVENT_GFX_RED_FISH       : OBJ_EVENT_GFX_BRENDAN_FISHING)
#define PLAYER_AVATAR_GFX_MALE_WATERING   (IS_HNS ? OBJ_EVENT_GFX_GOLD_WATERING_HNS  : IS_FRLG ? OBJ_EVENT_GFX_RED_FIELD_MOVE : OBJ_EVENT_GFX_BRENDAN_WATERING)
#define PLAYER_AVATAR_GFX_MALE_VSSEEKER   (IS_HNS ? OBJ_EVENT_GFX_GOLD_FIELD_MOVE_HNS : IS_FRLG ? OBJ_EVENT_GFX_RED_VS_SEEKER  : OBJ_EVENT_GFX_BRENDAN_FIELD_MOVE)
#define PLAYER_AVATAR_GFX_FEMALE_NORMAL     (IS_HNS ? OBJ_EVENT_GFX_KRIS_NORMAL_HNS     : IS_FRLG ? OBJ_EVENT_GFX_GREEN_NORMAL     : OBJ_EVENT_GFX_MAY_NORMAL)
#define PLAYER_AVATAR_GFX_FEMALE_MACH_BIKE  (IS_HNS ? OBJ_EVENT_GFX_KRIS_MACH_BIKE_HNS : IS_FRLG ? OBJ_EVENT_GFX_GREEN_BIKE       : OBJ_EVENT_GFX_MAY_MACH_BIKE)
#define PLAYER_AVATAR_GFX_FEMALE_ACRO_BIKE  (IS_HNS ? OBJ_EVENT_GFX_KRIS_ACRO_BIKE_HNS : IS_FRLG ? OBJ_EVENT_GFX_GREEN_BIKE       : OBJ_EVENT_GFX_MAY_ACRO_BIKE)
#define PLAYER_AVATAR_GFX_FEMALE_SURFING    (IS_HNS ? OBJ_EVENT_GFX_KRIS_SURFING_HNS   : IS_FRLG ? OBJ_EVENT_GFX_GREEN_SURF       : OBJ_EVENT_GFX_MAY_SURFING)
#define PLAYER_AVATAR_GFX_FEMALE_UNDERWATER (IS_HNS ? OBJ_EVENT_GFX_KRIS_UNDERWATER_HNS : IS_FRLG ? OBJ_EVENT_GFX_GREEN_SURF       : OBJ_EVENT_GFX_MAY_UNDERWATER)
#define PLAYER_AVATAR_GFX_FEMALE_FIELD_MOVE (IS_HNS ? OBJ_EVENT_GFX_KRIS_FIELD_MOVE_HNS : IS_FRLG ? OBJ_EVENT_GFX_GREEN_FIELD_MOVE : OBJ_EVENT_GFX_MAY_FIELD_MOVE)
#define PLAYER_AVATAR_GFX_FEMALE_FISHING    (IS_HNS ? OBJ_EVENT_GFX_KRIS_FISHING_HNS   : IS_FRLG ? OBJ_EVENT_GFX_GREEN_FISH       : OBJ_EVENT_GFX_MAY_FISHING)
#define PLAYER_AVATAR_GFX_FEMALE_WATERING   (IS_HNS ? OBJ_EVENT_GFX_KRIS_WATERING_HNS  : IS_FRLG ? OBJ_EVENT_GFX_GREEN_FIELD_MOVE : OBJ_EVENT_GFX_MAY_WATERING)
#define PLAYER_AVATAR_GFX_FEMALE_VSSEEKER   (IS_HNS ? OBJ_EVENT_GFX_KRIS_FIELD_MOVE_HNS : IS_FRLG ? OBJ_EVENT_GFX_GREEN_VS_SEEKER  : OBJ_EVENT_GFX_MAY_FIELD_MOVE)

// Values are assigned explicitly so that graphics ids stored in saves and map data keep their numbers.
enum
{
    OBJ_EVENT_GFX_BRENDAN_NORMAL                = 0,
    OBJ_EVENT_GFX_BRENDAN_MACH_BIKE             = 1,
    OBJ_EVENT_GFX_BRENDAN_SURFING               = 2,
    OBJ_EVENT_GFX_BRENDAN_FIELD_MOVE            = 3,
    OBJ_EVENT_GFX_QUINTY_PLUMP                  = 4,
    OBJ_EVENT_GFX_NINJA_BOY                     = 5,
    OBJ_EVENT_GFX_TWIN                          = 6,
    OBJ_EVENT_GFX_BOY_1                         = 7,
    OBJ_EVENT_GFX_GIRL_1                        = 8,
    OBJ_EVENT_GFX_BOY_2                         = 9,
    OBJ_EVENT_GFX_GIRL_2                        = 10,
    OBJ_EVENT_GFX_LITTLE_BOY                    = 11,
    OBJ_EVENT_GFX_LITTLE_GIRL                   = 12,
    OBJ_EVENT_GFX_BOY_3                         = 13,
    OBJ_EVENT_GFX_GIRL_3                        = 14,
    OBJ_EVENT_GFX_RICH_BOY                      = 15,
    OBJ_EVENT_GFX_WOMAN_1                       = 16,
    OBJ_EVENT_GFX_FAT_MAN                       = 17,
    OBJ_EVENT_GFX_POKEFAN_F                     = 18,
    OBJ_EVENT_GFX_MAN_1                         = 19,
    OBJ_EVENT_GFX_WOMAN_2                       = 20,
    OBJ_EVENT_GFX_EXPERT_M                      = 21,
    OBJ_EVENT_GFX_EXPERT_F                      = 22,
    OBJ_EVENT_GFX_MAN_2                         = 23,
    OBJ_EVENT_GFX_WOMAN_3                       = 24,
    OBJ_EVENT_GFX_POKEFAN_M                     = 25,
    OBJ_EVENT_GFX_WOMAN_4                       = 26,
    OBJ_EVENT_GFX_COOK                          = 27,
    OBJ_EVENT_GFX_LINK_RECEPTIONIST             = 28,
    OBJ_EVENT_GFX_OLD_MAN                       = 29,
    OBJ_EVENT_GFX_OLD_WOMAN                     = 30,
    OBJ_EVENT_GFX_CAMPER                        = 31,
    OBJ_EVENT_GFX_PICNICKER                     = 32,
    OBJ_EVENT_GFX_MAN_3                         = 33,
    OBJ_EVENT_GFX_WOMAN_5                       = 34,
    OBJ_EVENT_GFX_YOUNGSTER                     = 35,
    OBJ_EVENT_GFX_BUG_CATCHER                   = 36,
    OBJ_EVENT_GFX_PSYCHIC_M                     = 37,
    OBJ_EVENT_GFX_SCHOOL_KID_M                  = 38,
    OBJ_EVENT_GFX_MANIAC                        = 39,
    OBJ_EVENT_GFX_HEX_MANIAC                    = 40,
    OBJ_EVENT_GFX_RAYQUAZA_STILL                = 41,
    OBJ_EVENT_GFX_SWIMMER_M                     = 42,
    OBJ_EVENT_GFX_SWIMMER_F                     = 43,
    OBJ_EVENT_GFX_BLACK_BELT                    = 44,
    OBJ_EVENT_GFX_BEAUTY                        = 45,
    OBJ_EVENT_GFX_SCIENTIST_1                   = 46,
    OBJ_EVENT_GFX_LASS                          = 47,
    OBJ_EVENT_GFX_GENTLEMAN                     = 48,
    OBJ_EVENT_GFX_SAILOR                        = 49,
    OBJ_EVENT_GFX_FISHERMAN                     = 50,
    OBJ_EVENT_GFX_RUNNING_TRIATHLETE_M          = 51,
    OBJ_EVENT_GFX_RUNNING_TRIATHLETE_F          = 52,
    OBJ_EVENT_GFX_TUBER_F                       = 53,
    OBJ_EVENT_GFX_TUBER_M                       = 54,
    OBJ_EVENT_GFX_HIKER                         = 55,
    OBJ_EVENT_GFX_CYCLING_TRIATHLETE_M          = 56,
    OBJ_EVENT_GFX_CYCLING_TRIATHLETE_F          = 57,
    OBJ_EVENT_GFX_NURSE                         = 58,
    OBJ_EVENT_GFX_ITEM_BALL                     = 59,
    OBJ_EVENT_GFX_BERRY_TREE                    = 60,
    OBJ_EVENT_GFX_BERRY_TREE_EARLY_STAGES       = 61,
    OBJ_EVENT_GFX_BERRY_TREE_LATE_STAGES        = 62,
    OBJ_EVENT_GFX_BRENDAN_ACRO_BIKE             = 63,
    OBJ_EVENT_GFX_PROF_BIRCH                    = 64,
    OBJ_EVENT_GFX_MAN_4                         = 65,
    OBJ_EVENT_GFX_MAN_5                         = 66,
    OBJ_EVENT_GFX_REPORTER_M                    = 67,
    OBJ_EVENT_GFX_REPORTER_F                    = 68,
    OBJ_EVENT_GFX_BARD                          = 69,
    OBJ_EVENT_GFX_ANABEL                        = 70,
    OBJ_EVENT_GFX_TUCKER                        = 71,
    OBJ_EVENT_GFX_GRETA                         = 72,
    OBJ_EVENT_GFX_SPENSER                       = 73,
    OBJ_EVENT_GFX_NOLAND                        = 74,
    OBJ_EVENT_GFX_LUCY                          = 75,
    OBJ_EVENT_GFX_UNUSED_NATU_DOLL              = 76,
    OBJ_EVENT_GFX_UNUSED_MAGNEMITE_DOLL         = 77,
    OBJ_EVENT_GFX_UNUSED_SQUIRTLE_DOLL          = 78,
    OBJ_EVENT_GFX_UNUSED_WOOPER_DOLL            = 79,
    OBJ_EVENT_GFX_UNUSED_PIKACHU_DOLL           = 80,
    OBJ_EVENT_GFX_UNUSED_PORYGON2_DOLL          = 81,
    OBJ_EVENT_GFX_CUTTABLE_TREE                 = 82,
    OBJ_EVENT_GFX_MART_EMPLOYEE                 = 83,
    OBJ_EVENT_GFX_ROOFTOP_SALE_WOMAN            = 84,
    OBJ_EVENT_GFX_TEALA                         = 85,
    OBJ_EVENT_GFX_BREAKABLE_ROCK                = 86,
    OBJ_EVENT_GFX_PUSHABLE_BOULDER              = 87,
    OBJ_EVENT_GFX_MR_BRINEYS_BOAT               = 88,
    OBJ_EVENT_GFX_MAY_NORMAL                    = 89,
    OBJ_EVENT_GFX_MAY_MACH_BIKE                 = 90,
    OBJ_EVENT_GFX_MAY_ACRO_BIKE                 = 91,
    OBJ_EVENT_GFX_MAY_SURFING                   = 92,
    OBJ_EVENT_GFX_MAY_FIELD_MOVE                = 93,
    OBJ_EVENT_GFX_TRUCK                         = 94,
    OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX         = 95,
    OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY          = 96,
    OBJ_EVENT_GFX_BIRCHS_BAG                    = 97,
    OBJ_EVENT_GFX_ZIGZAGOON_1                   = 98,
    OBJ_EVENT_GFX_ARTIST                        = 99,
    OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL          = 100,
    OBJ_EVENT_GFX_RIVAL_BRENDAN_MACH_BIKE       = 101,
    OBJ_EVENT_GFX_RIVAL_BRENDAN_ACRO_BIKE       = 102,
    OBJ_EVENT_GFX_RIVAL_BRENDAN_SURFING         = 103,
    OBJ_EVENT_GFX_RIVAL_BRENDAN_FIELD_MOVE      = 104,
    OBJ_EVENT_GFX_RIVAL_MAY_NORMAL              = 105,
    OBJ_EVENT_GFX_RIVAL_MAY_MACH_BIKE           = 106,
    OBJ_EVENT_GFX_RIVAL_MAY_ACRO_BIKE           = 107,
    OBJ_EVENT_GFX_RIVAL_MAY_SURFING             = 108,
    OBJ_EVENT_GFX_RIVAL_MAY_FIELD_MOVE          = 109,
    OBJ_EVENT_GFX_CAMERAMAN                     = 110,
    OBJ_EVENT_GFX_BRENDAN_UNDERWATER            = 111,
    OBJ_EVENT_GFX_MAY_UNDERWATER                = 112,
    OBJ_EVENT_GFX_MOVING_BOX                    = 113,
    OBJ_EVENT_GFX_CABLE_CAR                     = 114,
    OBJ_EVENT_GFX_SCIENTIST_2                   = 115,
    OBJ_EVENT_GFX_DEVON_EMPLOYEE                = 116,
    OBJ_EVENT_GFX_AQUA_MEMBER_M                 = 117,
    OBJ_EVENT_GFX_AQUA_MEMBER_F                 = 118,
    OBJ_EVENT_GFX_MAGMA_MEMBER_M                = 119,
    OBJ_EVENT_GFX_MAGMA_MEMBER_F                = 120,
    OBJ_EVENT_GFX_SIDNEY                        = 121,
    OBJ_EVENT_GFX_PHOEBE                        = 122,
    OBJ_EVENT_GFX_GLACIA                        = 123,
    OBJ_EVENT_GFX_DRAKE                         = 124,
    OBJ_EVENT_GFX_ROXANNE                       = 125,
    OBJ_EVENT_GFX_BRAWLY                        = 126,
    OBJ_EVENT_GFX_WATTSON                       = 127,
    OBJ_EVENT_GFX_FLANNERY                      = 128,
    OBJ_EVENT_GFX_NORMAN                        = 129,
    OBJ_EVENT_GFX_WINONA                        = 130,
    OBJ_EVENT_GFX_LIZA                          = 131,
    OBJ_EVENT_GFX_TATE                          = 132,
    OBJ_EVENT_GFX_WALLACE                       = 133,
    OBJ_EVENT_GFX_STEVEN                        = 134,
    OBJ_EVENT_GFX_WALLY                         = 135,
    OBJ_EVENT_GFX_LITTLE_BOY_3                  = 136,
    OBJ_EVENT_GFX_BRENDAN_FISHING               = 137,
    OBJ_EVENT_GFX_MAY_FISHING                   = 138,
    OBJ_EVENT_GFX_HOT_SPRINGS_OLD_WOMAN         = 139,
    OBJ_EVENT_GFX_SS_TIDAL                      = 140,
    OBJ_EVENT_GFX_SUBMARINE_SHADOW              = 141,
    OBJ_EVENT_GFX_PICHU_DOLL                    = 142,
    OBJ_EVENT_GFX_PIKACHU_DOLL                  = 143,
    OBJ_EVENT_GFX_MARILL_DOLL                   = 144,
    OBJ_EVENT_GFX_TOGEPI_DOLL                   = 145,
    OBJ_EVENT_GFX_CYNDAQUIL_DOLL                = 146,
    OBJ_EVENT_GFX_CHIKORITA_DOLL                = 147,
    OBJ_EVENT_GFX_TOTODILE_DOLL                 = 148,
    OBJ_EVENT_GFX_JIGGLYPUFF_DOLL               = 149,
    OBJ_EVENT_GFX_MEOWTH_DOLL                   = 150,
    OBJ_EVENT_GFX_CLEFAIRY_DOLL                 = 151,
    OBJ_EVENT_GFX_DITTO_DOLL                    = 152,
    OBJ_EVENT_GFX_SMOOCHUM_DOLL                 = 153,
    OBJ_EVENT_GFX_TREECKO_DOLL                  = 154,
    OBJ_EVENT_GFX_TORCHIC_DOLL                  = 155,
    OBJ_EVENT_GFX_MUDKIP_DOLL                   = 156,
    OBJ_EVENT_GFX_DUSKULL_DOLL                  = 157,
    OBJ_EVENT_GFX_WYNAUT_DOLL                   = 158,
    OBJ_EVENT_GFX_BALTOY_DOLL                   = 159,
    OBJ_EVENT_GFX_KECLEON_DOLL                  = 160,
    OBJ_EVENT_GFX_AZURILL_DOLL                  = 161,
    OBJ_EVENT_GFX_SKITTY_DOLL                   = 162,
    OBJ_EVENT_GFX_SWABLU_DOLL                   = 163,
    OBJ_EVENT_GFX_GULPIN_DOLL                   = 164,
    OBJ_EVENT_GFX_LOTAD_DOLL                    = 165,
    OBJ_EVENT_GFX_SEEDOT_DOLL                   = 166,
    OBJ_EVENT_GFX_PIKA_CUSHION                  = 167,
    OBJ_EVENT_GFX_ROUND_CUSHION                 = 168,
    OBJ_EVENT_GFX_KISS_CUSHION                  = 169,
    OBJ_EVENT_GFX_ZIGZAG_CUSHION                = 170,
    OBJ_EVENT_GFX_SPIN_CUSHION                  = 171,
    OBJ_EVENT_GFX_DIAMOND_CUSHION               = 172,
    OBJ_EVENT_GFX_BALL_CUSHION                  = 173,
    OBJ_EVENT_GFX_GRASS_CUSHION                 = 174,
    OBJ_EVENT_GFX_FIRE_CUSHION                  = 175,
    OBJ_EVENT_GFX_WATER_CUSHION                 = 176,
    OBJ_EVENT_GFX_BIG_SNORLAX_DOLL              = 177,
    OBJ_EVENT_GFX_BIG_RHYDON_DOLL               = 178,
    OBJ_EVENT_GFX_BIG_LAPRAS_DOLL               = 179,
    OBJ_EVENT_GFX_BIG_VENUSAUR_DOLL             = 180,
    OBJ_EVENT_GFX_BIG_CHARIZARD_DOLL            = 181,
    OBJ_EVENT_GFX_BIG_BLASTOISE_DOLL            = 182,
    OBJ_EVENT_GFX_BIG_WAILMER_DOLL              = 183,
    OBJ_EVENT_GFX_BIG_REGIROCK_DOLL             = 184,
    OBJ_EVENT_GFX_BIG_REGICE_DOLL               = 185,
    OBJ_EVENT_GFX_BIG_REGISTEEL_DOLL            = 186,
    OBJ_EVENT_GFX_LATIAS                        = 187,
    OBJ_EVENT_GFX_LATIOS                        = 188,
    OBJ_EVENT_GFX_GAMEBOY_KID                   = 189,
    OBJ_EVENT_GFX_CONTEST_JUDGE                 = 190,
    OBJ_EVENT_GFX_BRENDAN_WATERING              = 191,
    OBJ_EVENT_GFX_MAY_WATERING                  = 192,
    OBJ_EVENT_GFX_BRENDAN_DECORATING            = 193,
    OBJ_EVENT_GFX_MAY_DECORATING                = 194,
    OBJ_EVENT_GFX_ARCHIE                        = 195,
    OBJ_EVENT_GFX_MAXIE                         = 196,
    OBJ_EVENT_GFX_KYOGRE_FRONT                  = 197,
    OBJ_EVENT_GFX_GROUDON_FRONT                 = 198,
    OBJ_EVENT_GFX_FOSSIL                        = 199,
    OBJ_EVENT_GFX_REGIROCK                      = 200,
    OBJ_EVENT_GFX_REGICE                        = 201,
    OBJ_EVENT_GFX_REGISTEEL                     = 202,
    OBJ_EVENT_GFX_SKITTY                        = 203,
    OBJ_EVENT_GFX_KECLEON                       = 204,
    OBJ_EVENT_GFX_KYOGRE_ASLEEP                 = 205,
    OBJ_EVENT_GFX_GROUDON_ASLEEP                = 206,
    OBJ_EVENT_GFX_RAYQUAZA                      = 207,
    OBJ_EVENT_GFX_ZIGZAGOON_2                   = 208,
    OBJ_EVENT_GFX_PIKACHU                       = 209,
    OBJ_EVENT_GFX_AZUMARILL                     = 210,
    OBJ_EVENT_GFX_WINGULL                       = 211,
    OBJ_EVENT_GFX_KECLEON_BRIDGE_SHADOW         = 212,
    OBJ_EVENT_GFX_TUBER_M_SWIMMING              = 213,
    OBJ_EVENT_GFX_AZURILL                       = 214,
    OBJ_EVENT_GFX_MOM                           = 215,
    OBJ_EVENT_GFX_LINK_BRENDAN                  = 216,
    OBJ_EVENT_GFX_LINK_MAY                      = 217,
    OBJ_EVENT_GFX_JUAN                          = 218,
    OBJ_EVENT_GFX_SCOTT                         = 219,
    OBJ_EVENT_GFX_POOCHYENA                     = 220,
    OBJ_EVENT_GFX_KYOGRE_SIDE                   = 221,
    OBJ_EVENT_GFX_GROUDON_SIDE                  = 222,
    OBJ_EVENT_GFX_MYSTERY_GIFT_MAN              = 223,
    OBJ_EVENT_GFX_TRICK_HOUSE_STATUE            = 224,
    OBJ_EVENT_GFX_KIRLIA                        = 225,
    OBJ_EVENT_GFX_DUSCLOPS                      = 226,
    OBJ_EVENT_GFX_UNION_ROOM_NURSE              = 227,
    OBJ_EVENT_GFX_SUDOWOODO                     = 228,
    OBJ_EVENT_GFX_MEW                           = 229,
    OBJ_EVENT_GFX_RED                           = 230,
    OBJ_EVENT_GFX_LEAF                          = 231,
    OBJ_EVENT_GFX_DEOXYS                        = 232,
    OBJ_EVENT_GFX_DEOXYS_TRIANGLE               = 233,
    OBJ_EVENT_GFX_BRANDON                       = 234,
    OBJ_EVENT_GFX_LINK_RS_BRENDAN               = 235,
    OBJ_EVENT_GFX_LINK_RS_MAY                   = 236,
    OBJ_EVENT_GFX_LUGIA                         = 237,
    OBJ_EVENT_GFX_HOOH                          = 238,
    OBJ_EVENT_GFX_POKE_BALL                     = 239,
    OBJ_EVENT_GFX_OW_MON                        = 240,
    OBJ_EVENT_GFX_LIGHT_SPRITE                  = 241,
    OBJ_EVENT_GFX_APRICORN_TREE                 = 242,

    // FRLG objects
    OBJ_EVENT_GFX_RED_NORMAL                    = 243,
    OBJ_EVENT_GFX_RED_BIKE                      = 244,
    OBJ_EVENT_GFX_RED_SURF                      = 245,
    OBJ_EVENT_GFX_RED_FIELD_MOVE                = 246,
    OBJ_EVENT_GFX_RED_FISH                      = 247,
    OBJ_EVENT_GFX_RED_VS_SEEKER                 = 248,
    OBJ_EVENT_GFX_RED_VS_SEEKER_BIKE            = 249,
    // 250 is unused
    OBJ_EVENT_GFX_GREEN_NORMAL                  = 251,
    OBJ_EVENT_GFX_GREEN_BIKE                    = 252,
    OBJ_EVENT_GFX_GREEN_SURF                    = 253,
    OBJ_EVENT_GFX_GREEN_FIELD_MOVE              = 254,
    OBJ_EVENT_GFX_GREEN_FISH                    = 255,
    OBJ_EVENT_GFX_GREEN_VS_SEEKER               = 256,
    OBJ_EVENT_GFX_GREEN_VS_SEEKER_BIKE          = 257,
    OBJ_EVENT_GFX_BOY                           = 258,
    OBJ_EVENT_GFX_CRUSH_GIRL                    = 259,
    OBJ_EVENT_GFX_MAN                           = 260,
    OBJ_EVENT_GFX_ROCKER                        = 261,
    OBJ_EVENT_GFX_BALDING_MAN                   = 262,
    OBJ_EVENT_GFX_OLD_MAN_1                     = 263,
    OBJ_EVENT_GFX_OLD_MAN_2                     = 264,
    OBJ_EVENT_GFX_OLD_MAN_LYING_DOWN            = 265,
    OBJ_EVENT_GFX_TUBER_M_WATER                 = 266,
    OBJ_EVENT_GFX_TUBER_M_LAND                  = 267,
    OBJ_EVENT_GFX_COOLTRAINER_M                 = 268,
    OBJ_EVENT_GFX_COOLTRAINER_F                 = 269,
    OBJ_EVENT_GFX_SWIMMER_M_WATER               = 270,
    OBJ_EVENT_GFX_SWIMMER_F_WATER               = 271,
    OBJ_EVENT_GFX_SWIMMER_M_LAND                = 272,
    OBJ_EVENT_GFX_SWIMMER_F_LAND                = 273,
    OBJ_EVENT_GFX_WORKER_M                      = 274,
    OBJ_EVENT_GFX_WORKER_F                      = 275,
    OBJ_EVENT_GFX_ROCKET_M                      = 276,
    OBJ_EVENT_GFX_ROCKET_F                      = 277,
    OBJ_EVENT_GFX_GBA_KID                       = 278,
    OBJ_EVENT_GFX_POKE_MANIAC_FRLG              = 279,
    OBJ_EVENT_GFX_BIKER                         = 280,
    OBJ_EVENT_GFX_BLACK_BELT_FRLG               = 281,
    OBJ_EVENT_GFX_SCIENTIST                     = 282,
    OBJ_EVENT_GFX_FISHER                        = 283,
    OBJ_EVENT_GFX_CHANNELER                     = 284,
    OBJ_EVENT_GFX_CHEF                          = 285,
    OBJ_EVENT_GFX_POLICEMAN                     = 286,
    OBJ_EVENT_GFX_CAPTAIN                       = 287,
    OBJ_EVENT_GFX_CABLE_CLUB_RECEPTIONIST       = 288,
    OBJ_EVENT_GFX_UNION_ROOM_RECEPTIONIST       = 289,
    OBJ_EVENT_GFX_CLERK                         = 290,
    OBJ_EVENT_GFX_MG_DELIVERYMAN                = 291,
    OBJ_EVENT_GFX_TRAINER_TOWER_DUDE            = 292,
    OBJ_EVENT_GFX_PROF_OAK                      = 293,
    OBJ_EVENT_GFX_BLUE                          = 294,
    OBJ_EVENT_GFX_BILL                          = 295,
    OBJ_EVENT_GFX_LANCE                         = 296,
    OBJ_EVENT_GFX_AGATHA                        = 297,
    OBJ_EVENT_GFX_DAISY                         = 298,
    OBJ_EVENT_GFX_LORELEI                       = 299,
    OBJ_EVENT_GFX_MR_FUJI                       = 300,
    OBJ_EVENT_GFX_BRUNO                         = 301,
    OBJ_EVENT_GFX_BROCK                         = 302,
    OBJ_EVENT_GFX_MISTY                         = 303,
    OBJ_EVENT_GFX_LT_SURGE                      = 304,
    OBJ_EVENT_GFX_ERIKA                         = 305,
    OBJ_EVENT_GFX_KOGA                          = 306,
    OBJ_EVENT_GFX_SABRINA                       = 307,
    OBJ_EVENT_GFX_BLAINE                        = 308,
    OBJ_EVENT_GFX_GIOVANNI                      = 309,
    OBJ_EVENT_GFX_CELIO                         = 310,
    OBJ_EVENT_GFX_TEACHY_TV_HOST                = 311,
    OBJ_EVENT_GFX_GYM_GUY                       = 312,
    OBJ_EVENT_GFX_TOWN_MAP                      = 313,
    OBJ_EVENT_GFX_POKEDEX                       = 314,
    OBJ_EVENT_GFX_LITTLE_BOY_FRLG               = 315,
    OBJ_EVENT_GFX_LITTLE_GIRL_FRLG              = 316,
    OBJ_EVENT_GFX_YOUNGSTER_FRLG                = 317,
    OBJ_EVENT_GFX_BUG_CATCHER_FRLG              = 318,
    OBJ_EVENT_GFX_LASS_FRLG                     = 319,
    OBJ_EVENT_GFX_WOMAN_1_FRLG                  = 320,
    OBJ_EVENT_GFX_FAT_MAN_FRLG                  = 321,
    OBJ_EVENT_GFX_WOMAN_2_FRLG                  = 322,
    OBJ_EVENT_GFX_BEAUTY_FRLG                   = 323,
    OBJ_EVENT_GFX_WOMAN_3_FRLG                  = 324,
    OBJ_EVENT_GFX_OLD_WOMAN_FRLG                = 325,
    OBJ_EVENT_GFX_CAMPER_FRLG                   = 326,
    OBJ_EVENT_GFX_PICNICKER_FRLG                = 327,
    OBJ_EVENT_GFX_MOM_FRLG                      = 328,
    OBJ_EVENT_GFX_TUBER_F_FRLG                  = 329,
    OBJ_EVENT_GFX_HIKER_FRLG                    = 330,
    OBJ_EVENT_GFX_GENTLEMAN_FRLG                = 331,
    OBJ_EVENT_GFX_SAILOR_FRLG                   = 332,
    OBJ_EVENT_GFX_NURSE_FRLG                    = 333,
    OBJ_EVENT_GFX_FOSSIL_FRLG                   = 334,
    OBJ_EVENT_GFX_RUBY                          = 335,
    OBJ_EVENT_GFX_SAPPHIRE                      = 336,
    OBJ_EVENT_GFX_OLD_AMBER                     = 337,
    OBJ_EVENT_GFX_GYM_SIGN                      = 338,
    OBJ_EVENT_GFX_SIGN                          = 339,
    OBJ_EVENT_GFX_TRAINER_TIPS                  = 340,
    OBJ_EVENT_GFX_CLIPBOARD                     = 341,
    OBJ_EVENT_GFX_METEORITE                     = 342,
    OBJ_EVENT_GFX_LAPRAS_DOLL                   = 343,
    OBJ_EVENT_GFX_SEAGALLOP                     = 344,
    OBJ_EVENT_GFX_SNORLAX                       = 345,
    OBJ_EVENT_GFX_SPEAROW                       = 346,
    OBJ_EVENT_GFX_CUBONE                        = 347,
    OBJ_EVENT_GFX_POLIWRATH                     = 348,
    OBJ_EVENT_GFX_CLEFAIRY                      = 349,
    OBJ_EVENT_GFX_PIDGEOT                       = 350,
    OBJ_EVENT_GFX_JIGGLYPUFF                    = 351,
    OBJ_EVENT_GFX_PIDGEY                        = 352,
    OBJ_EVENT_GFX_CHANSEY                       = 353,
    OBJ_EVENT_GFX_OMANYTE                       = 354,
    OBJ_EVENT_GFX_KANGASKHAN                    = 355,
    OBJ_EVENT_GFX_PIKACHU_FRLG                  = 356,
    OBJ_EVENT_GFX_PSYDUCK                       = 357,
    OBJ_EVENT_GFX_NIDORAN_F                     = 358,
    OBJ_EVENT_GFX_NIDORAN_M                     = 359,
    OBJ_EVENT_GFX_NIDORINO                      = 360,
    OBJ_EVENT_GFX_MEOWTH                        = 361,
    OBJ_EVENT_GFX_SEEL                          = 362,
    OBJ_EVENT_GFX_VOLTORB                       = 363,
    OBJ_EVENT_GFX_SLOWPOKE                      = 364,
    OBJ_EVENT_GFX_SLOWBRO                       = 365,
    OBJ_EVENT_GFX_MACHOP                        = 366,
    OBJ_EVENT_GFX_WIGGLYTUFF                    = 367,
    OBJ_EVENT_GFX_DODUO                         = 368,
    OBJ_EVENT_GFX_FEAROW                        = 369,
    OBJ_EVENT_GFX_MACHOKE                       = 370,
    OBJ_EVENT_GFX_LAPRAS                        = 371,
    OBJ_EVENT_GFX_ZAPDOS                        = 372,
    OBJ_EVENT_GFX_MOLTRES                       = 373,
    OBJ_EVENT_GFX_ARTICUNO                      = 374,
    OBJ_EVENT_GFX_MEWTWO                        = 375,
    OBJ_EVENT_GFX_ENTEI                         = 376,
    OBJ_EVENT_GFX_SUICUNE                       = 377,
    OBJ_EVENT_GFX_RAIKOU                        = 378,
    OBJ_EVENT_GFX_CELEBI                        = 379,
    OBJ_EVENT_GFX_KABUTO                        = 380,
    OBJ_EVENT_GFX_DEOXYS_D                      = 381,
    OBJ_EVENT_GFX_DEOXYS_A                      = 382,
    OBJ_EVENT_GFX_DEOXYS_N                      = 383,
    OBJ_EVENT_GFX_SS_ANNE                       = 384,
    OBJ_EVENT_GFX_PUSHABLE_BOULDER_FRLG         = 385,
    OBJ_EVENT_GFX_CUTTABLE_TREE_FRLG            = 386,
    OBJ_EVENT_GFX_BREAKABLE_ROCK_FRLG           = 387,
    // 388-397 are unused

    // HnS objects
    OBJ_EVENT_GFX_ATTENDANT_F_HNS               = 398,
    OBJ_EVENT_GFX_ATTENDANT_M_HNS               = 399,
    OBJ_EVENT_GFX_BALDING_MAN_HNS               = 400,
    OBJ_EVENT_GFX_BATTLE_GIRL_HNS               = 401,
    OBJ_EVENT_GFX_BATTLE_TOWER_TRAINER_DUDE_HNS = 402,
    OBJ_EVENT_GFX_BEAUTY_HNS                    = 403,
    OBJ_EVENT_GFX_BIKER_HNS                     = 404,
    OBJ_EVENT_GFX_BLACK_BELT_HNS                = 405,
    OBJ_EVENT_GFX_BOY_2_HNS                     = 406,
    OBJ_EVENT_GFX_BUG_CATCHER_HNS               = 407,
    OBJ_EVENT_GFX_BURGLAR_HNS                   = 408,
    OBJ_EVENT_GFX_CAMPER_HNS                    = 409,
    OBJ_EVENT_GFX_CAPTAIN_HNS                   = 410,
    OBJ_EVENT_GFX_CHANNELER_HNS                 = 411,
    OBJ_EVENT_GFX_COOK_HNS                      = 412,
    OBJ_EVENT_GFX_COOLTRAINER_F_HNS             = 413,
    OBJ_EVENT_GFX_COOLTRAINER_M_HNS             = 414,
    OBJ_EVENT_GFX_ENGINEER_HNS                  = 415,
    OBJ_EVENT_GFX_FAT_MAN_HNS                   = 416,
    OBJ_EVENT_GFX_FIREBREATHER_HNS              = 417,
    OBJ_EVENT_GFX_FISHERMAN_HNS                 = 418,
    OBJ_EVENT_GFX_GAMEBOY_KID_HNS               = 419,
    OBJ_EVENT_GFX_GENTLEMAN_HNS                 = 420,
    OBJ_EVENT_GFX_GIRL_1_HNS                    = 421,
    OBJ_EVENT_GFX_GYM_GUY_HNS                   = 422,
    OBJ_EVENT_GFX_HIKER_HNS                     = 423,
    OBJ_EVENT_GFX_JUGGLER_HNS                   = 424,
    OBJ_EVENT_GFX_LASS_HNS                      = 425,
    OBJ_EVENT_GFX_LINK_RECEPTIONIST_HNS         = 426,
    OBJ_EVENT_GFX_LITTLE_BOY_HNS                = 427,
    OBJ_EVENT_GFX_LITTLE_GIRL_HNS               = 428,
    OBJ_EVENT_GFX_MAN_HNS                       = 429,
    OBJ_EVENT_GFX_MART_EMPLOYEE_HNS             = 430,
    OBJ_EVENT_GFX_MOM_HNS                       = 431,
    OBJ_EVENT_GFX_MYSTERY_EVENT_DELIVERYMAN_HNS = 432,
    OBJ_EVENT_GFX_NURSE_HNS                     = 433,
    OBJ_EVENT_GFX_OFFICER_HNS                   = 434,
    OBJ_EVENT_GFX_OLD_MAN_HNS                   = 435,
    OBJ_EVENT_GFX_OLD_MAN_2_HNS                 = 436,
    OBJ_EVENT_GFX_OLD_WOMAN_HNS                 = 437,
    OBJ_EVENT_GFX_PICNICKER_HNS                 = 438,
    OBJ_EVENT_GFX_PSYCHIC_M_HNS                 = 439,
    OBJ_EVENT_GFX_ROCKER_HNS                    = 440,
    OBJ_EVENT_GFX_SAGE_HNS                      = 441,
    OBJ_EVENT_GFX_SAGE_ELDER_HNS                = 442,
    OBJ_EVENT_GFX_SAILOR_HNS                    = 443,
    OBJ_EVENT_GFX_SCIENTIST_F_HNS               = 444,
    OBJ_EVENT_GFX_SCIENTIST_M_HNS               = 445,
    OBJ_EVENT_GFX_SCOTT_HNS                     = 446,
    OBJ_EVENT_GFX_STEVEN_HNS                    = 447,
    OBJ_EVENT_GFX_SUPER_NERD_HNS                = 448,
    OBJ_EVENT_GFX_SWIMMER_F_HNS                 = 449,
    OBJ_EVENT_GFX_SWIMMER_F_LAND_HNS            = 450,
    OBJ_EVENT_GFX_SWIMMER_M_HNS                 = 451,
    OBJ_EVENT_GFX_TUBER_F_HNS                   = 452,
    OBJ_EVENT_GFX_TUBER_M_HNS                   = 453,
    OBJ_EVENT_GFX_TUBER_M_SWIMMING_HNS          = 454,
    OBJ_EVENT_GFX_TWIN_HNS                      = 455,
    OBJ_EVENT_GFX_WOMAN_1_HNS                   = 456,
    OBJ_EVENT_GFX_WOMAN_2_HNS                   = 457,
    OBJ_EVENT_GFX_WOMAN_3_HNS                   = 458,
    OBJ_EVENT_GFX_WORKER_F_HNS                  = 459,
    OBJ_EVENT_GFX_WORKER_M_HNS                  = 460,
    OBJ_EVENT_GFX_YOUNGSTER_HNS                 = 461,
    // HnS gym leaders
    OBJ_EVENT_GFX_BLAINE_HNS                    = 462,
    OBJ_EVENT_GFX_BLUE_HNS                      = 463,
    OBJ_EVENT_GFX_BROCK_HNS                     = 464,
    OBJ_EVENT_GFX_BUGSY_HNS                     = 465,
    OBJ_EVENT_GFX_CHUCK_HNS                     = 466,
    OBJ_EVENT_GFX_CLAIR_HNS                     = 467,
    OBJ_EVENT_GFX_ERIKA_HNS                     = 468,
    OBJ_EVENT_GFX_FALKNER_HNS                   = 469,
    OBJ_EVENT_GFX_JANINE_HNS                    = 470,
    OBJ_EVENT_GFX_JASMINE_HNS                   = 471,
    OBJ_EVENT_GFX_MISTY_HNS                     = 472,
    OBJ_EVENT_GFX_MORTY_HNS                     = 473,
    OBJ_EVENT_GFX_PRYCE_HNS                     = 474,
    OBJ_EVENT_GFX_SABRINA_HNS                   = 475,
    OBJ_EVENT_GFX_SURGE_HNS                     = 476,
    OBJ_EVENT_GFX_WHITNEY_HNS                   = 477,
    // HnS elite four
    OBJ_EVENT_GFX_BRUNO_HNS                     = 478,
    OBJ_EVENT_GFX_KAREN_HNS                     = 479,
    OBJ_EVENT_GFX_KOGA_HNS                      = 480,
    OBJ_EVENT_GFX_LANCE_HNS                     = 481,
    OBJ_EVENT_GFX_WILL_HNS                      = 482,
    // HnS rockets
    OBJ_EVENT_GFX_ARCHER_HNS                    = 483,
    OBJ_EVENT_GFX_ARIANA_HNS                    = 484,
    OBJ_EVENT_GFX_GIOVANNI_HNS                  = 485,
    OBJ_EVENT_GFX_PETREL_HNS                    = 486,
    OBJ_EVENT_GFX_PROTON_HNS                    = 487,
    OBJ_EVENT_GFX_ROCKET_F_HNS                  = 488,
    OBJ_EVENT_GFX_ROCKET_M_HNS                  = 489,
    // HnS frontier brains
    OBJ_EVENT_GFX_ANABEL_HNS                    = 490,
    OBJ_EVENT_GFX_BRANDON_HNS                   = 491,
    OBJ_EVENT_GFX_GRETA_HNS                     = 492,
    OBJ_EVENT_GFX_LUCY_HNS                      = 493,
    OBJ_EVENT_GFX_NOLAND_HNS                    = 494,
    OBJ_EVENT_GFX_SPENSER_HNS                   = 495,
    OBJ_EVENT_GFX_TUCKER_HNS                    = 496,
    // HnS special characters
    OBJ_EVENT_GFX_BILL_HNS                      = 497,
    OBJ_EVENT_GFX_ELM_HNS                       = 498,
    OBJ_EVENT_GFX_EUSINE_HNS                    = 499,
    OBJ_EVENT_GFX_KIMONO_HNS                    = 500,
    OBJ_EVENT_GFX_KURT_HNS                      = 501,
    OBJ_EVENT_GFX_KURT_LYING_DOWN_HNS           = 502,
    OBJ_EVENT_GFX_PROF_OAK_HNS                  = 503,
    OBJ_EVENT_GFX_RED_NORMAL_HNS                = 504,
    OBJ_EVENT_GFX_SILVER_HNS                    = 505,
    // HnS misc objects
    OBJ_EVENT_GFX_BIRTH_ISLAND_STONE_HNS        = 506,
    OBJ_EVENT_GFX_BREAKABLE_ROCK_HNS            = 507,
    OBJ_EVENT_GFX_CUTTABLE_TREE_HNS             = 508,
    OBJ_EVENT_GFX_FOSSIL_HNS                    = 509,
    OBJ_EVENT_GFX_ITEM_BALL_HNS                 = 510,
    OBJ_EVENT_GFX_LEGENDARY_SHADOW_HNS          = 511,
    OBJ_EVENT_GFX_NO_TAIL_SLOWPOKE_HNS          = 512,
    OBJ_EVENT_GFX_PUSHABLE_BOULDER_HNS          = 513,
    OBJ_EVENT_GFX_SHINY_GYARADOS_HNS            = 514,
    OBJ_EVENT_GFX_SLEEPING_SNORLAX_HNS          = 515,
    OBJ_EVENT_GFX_SS_AQUA_HNS                   = 516,
    OBJ_EVENT_GFX_SWIMMING_LAPRAS_HNS           = 517,
    OBJ_EVENT_GFX_TOWER_BEAM_HNS                = 518,
    OBJ_EVENT_GFX_TRAIN_EAST_HNS                = 519,
    OBJ_EVENT_GFX_TRAIN_WEST_HNS                = 520,
    OBJ_EVENT_GFX_WHIRLPOOL_HNS                 = 521,
    // HnS lights
    OBJ_EVENT_GFX_LIGHT_HNS                     = 522,
    OBJ_EVENT_GFX_MART_LIGHT_HNS                = 523,
    OBJ_EVENT_GFX_POKE_CENTER_LIGHT_HNS         = 524,
    OBJ_EVENT_GFX_SMALL_LIGHT_HNS               = 525,
    OBJ_EVENT_GFX_NURSE_CHANSEY_HNS             = 526,
    // HnS protagonists
    OBJ_EVENT_GFX_GOLD_NORMAL_HNS               = 527,
    OBJ_EVENT_GFX_GOLD_MACH_BIKE_HNS            = 528,
    OBJ_EVENT_GFX_GOLD_ACRO_BIKE_HNS            = 529,
    OBJ_EVENT_GFX_GOLD_SURFING_HNS              = 530,
    OBJ_EVENT_GFX_GOLD_UNDERWATER_HNS           = 531,
    OBJ_EVENT_GFX_GOLD_FIELD_MOVE_HNS           = 532,
    OBJ_EVENT_GFX_GOLD_FISHING_HNS              = 533,
    OBJ_EVENT_GFX_GOLD_WATERING_HNS             = 534,
    OBJ_EVENT_GFX_GOLD_DECORATING_HNS           = 535,
    OBJ_EVENT_GFX_KRIS_NORMAL_HNS               = 536,
    OBJ_EVENT_GFX_KRIS_MACH_BIKE_HNS            = 537,
    OBJ_EVENT_GFX_KRIS_ACRO_BIKE_HNS            = 538,
    OBJ_EVENT_GFX_KRIS_SURFING_HNS              = 539,
    OBJ_EVENT_GFX_KRIS_UNDERWATER_HNS           = 540,
    OBJ_EVENT_GFX_KRIS_FIELD_MOVE_HNS           = 541,
    OBJ_EVENT_GFX_KRIS_FISHING_HNS              = 542,
    OBJ_EVENT_GFX_KRIS_WATERING_HNS             = 543,
    OBJ_EVENT_GFX_KRIS_DECORATING_HNS           = 544,
    OBJ_EVENT_GFX_SKIER_F_HNS                   = 545,
    OBJ_EVENT_GFX_SKIER_M_HNS                   = 546,
    OBJ_EVENT_GFX_ALOLA_OAK_HNS                 = 547,
    NUM_OBJ_EVENT_GFX                           = 548,
};

// FRLG equivalents

// #define OBJ_EVENT_GFX_MEW OBJ_EVENT_GFX_NINJA_BOY
// #define OBJ_EVENT_GFX_LUGIA OBJ_EVENT_GFX_NINJA_BOY



// NOTE: The maximum amount of object events has been expanded from 255 to 65535.
// Since dynamic graphics ids still require at least 16 free values, the actual limit
// is 65519, but even considering follower Pokémon, this should be more than enough :)


// These are dynamic object gfx ids.
// They correspond with the values of the VAR_OBJ_GFX_ID_X vars.
// More info about them in include/constants/vars.h
#define OBJ_EVENT_GFX_VARS   (NUM_OBJ_EVENT_GFX + 1)
#define OBJ_EVENT_GFX_VAR_0  (OBJ_EVENT_GFX_VARS + 0x0)
#define OBJ_EVENT_GFX_VAR_1  (OBJ_EVENT_GFX_VARS + 0x1)
#define OBJ_EVENT_GFX_VAR_2  (OBJ_EVENT_GFX_VARS + 0x2)
#define OBJ_EVENT_GFX_VAR_3  (OBJ_EVENT_GFX_VARS + 0x3)
#define OBJ_EVENT_GFX_VAR_4  (OBJ_EVENT_GFX_VARS + 0x4)
#define OBJ_EVENT_GFX_VAR_5  (OBJ_EVENT_GFX_VARS + 0x5)
#define OBJ_EVENT_GFX_VAR_6  (OBJ_EVENT_GFX_VARS + 0x6)
#define OBJ_EVENT_GFX_VAR_7  (OBJ_EVENT_GFX_VARS + 0x7)
#define OBJ_EVENT_GFX_VAR_8  (OBJ_EVENT_GFX_VARS + 0x8)
#define OBJ_EVENT_GFX_VAR_9  (OBJ_EVENT_GFX_VARS + 0x9)
#define OBJ_EVENT_GFX_VAR_A  (OBJ_EVENT_GFX_VARS + 0xA)
#define OBJ_EVENT_GFX_VAR_B  (OBJ_EVENT_GFX_VARS + 0xB)
#define OBJ_EVENT_GFX_VAR_C  (OBJ_EVENT_GFX_VARS + 0xC)
#define OBJ_EVENT_GFX_VAR_D  (OBJ_EVENT_GFX_VARS + 0xD)
#define OBJ_EVENT_GFX_VAR_E  (OBJ_EVENT_GFX_VARS + 0xE)
#define OBJ_EVENT_GFX_VAR_F  (OBJ_EVENT_GFX_VARS + 0xF)

// Don't use (1u << 15) to avoid conflict with BLEND_IMMUNE_FLAG.
#define OBJ_EVENT_MON               (1u << 14)
#define OBJ_EVENT_GFX_MON_BASE     OBJ_EVENT_MON
#define OBJ_EVENT_MON_SHINY         (1u << 13)
#define OBJ_EVENT_MON_FEMALE        (1u << 12)
#define OBJ_EVENT_MON_SPECIES_MASK  (~(7u << 12))

// Used to call a specific species' follower graphics. Useful for static encounters.
#define OBJ_EVENT_GFX_SPECIES(name)                 (SPECIES_##name + OBJ_EVENT_MON)
#define OBJ_EVENT_GFX_SPECIES_SHINY(name)           (SPECIES_##name + OBJ_EVENT_MON + OBJ_EVENT_MON_SHINY)
#define OBJ_EVENT_GFX_SPECIES_FEMALE(name)          (SPECIES_##name + OBJ_EVENT_MON + OBJ_EVENT_MON_FEMALE)
#define OBJ_EVENT_GFX_SPECIES_SHINY_FEMALE(name)    (SPECIES_##name + OBJ_EVENT_MON + OBJ_EVENT_MON_SHINY + OBJ_EVENT_MON_FEMALE)

#define OW_SPECIES(x) ((x)->graphicsId & OBJ_EVENT_MON_SPECIES_MASK)
#define OW_SHINY(x) ((x)->graphicsId & OBJ_EVENT_MON_SHINY)
#define OW_FEMALE(x) ((x)->graphicsId & OBJ_EVENT_MON_FEMALE)

// Whether Object Event is an OW Pokémon
#define IS_OW_MON_OBJ(obj) ((obj)->graphicsId & OBJ_EVENT_MON)

#define SHADOW_SIZE_S       0
#define SHADOW_SIZE_M       1
#define SHADOW_SIZE_L       2
#define SHADOW_SIZE_NONE    3   // Originally SHADOW_SIZE_XL, which went unused due to shadowSize in ObjectEventGraphicsInfo being only 2 bits.

#define SHADOW_SIZE_XL_BATTLE_ONLY  SHADOW_SIZE_NONE    // Battle-only definition for XL shadow size.

#define F_INANIMATE                        (1 << 6)
#define F_DISABLE_REFLECTION_PALETTE_LOAD  (1 << 7)

#define TRACKS_NONE       0
#define TRACKS_FOOT       1
#define TRACKS_BIKE_TIRE  2
#define TRACKS_SLITHER    3
#define TRACKS_SPOT       4
#define TRACKS_BUG        5

#define LIGHT_TYPE_BALL                 0
#define LIGHT_TYPE_PKMN_CENTER_SIGN     1
#define LIGHT_TYPE_POKE_MART_SIGN       2
#define LIGHT_TYPE_SMALL_LAMP           3
#define LIGHT_TYPE_LIGHTHOUSE           4
#define LIGHT_TYPE_BATTLE_FRONTIER_ARCH 5

#define FIRST_DECORATION_SPRITE_GFX OBJ_EVENT_GFX_PICHU_DOLL

#define OBJ_KIND_NORMAL 0
#define OBJ_KIND_CLONE  255 // Exclusive to FRLG


// Each object event template gets an ID that can be used to refer to it in scripts and elsewhere.
// This is referred to as the "local id" (and it's really just 1 + its index in the templates array).
// There are a few special IDs reserved for objects that don't have templates in the map data -- one for the player
// in regular offline play, five for linked players while playing Berry Blender, and one for an invisible object that
// can be spawned for the camera to track instead of the player. Additionally, the value 0 is reserved as an "empty" indicator.
#define LOCALID_NONE                              0
#define LOCALID_CAMERA                          127
#define LOCALID_BERRY_BLENDER_PLAYER_END        240 // This will use 5 (MAX_RFU_PLAYERS) IDs ending at 240, i.e. 236-240
#define LOCALID_OW_ENCOUNTER_END                252 // This will use 4 (OWE_SPAWNS_MAX) IDs ending at 252, i.e. 249-252
#define LOCALID_FOLLOWING_POKEMON               254
#define LOCALID_PLAYER                          255
#define OBJ_EVENT_ID_FOLLOWER                   0xFE
#define OBJ_EVENT_ID_NPC_FOLLOWER               0xFD

#define IS_LOCALID_GENERATED_OWE(localId)       (localId <= LOCALID_OW_ENCOUNTER_END \
                                                 && localId > (LOCALID_OW_ENCOUNTER_END - OWE_SPAWNS_MAX))

// Aliases for old names. "object event id" normally refers to an index into gObjectEvents, which these are not.
// Used for link player OWs in CreateLinkPlayerSprite
#define OBJ_EVENT_ID_CAMERA LOCALID_CAMERA
#define OBJ_EVENT_ID_PLAYER LOCALID_PLAYER
#define OBJ_EVENT_ID_DYNAMIC_BASE 0xF0

// Moved from src/event_object_movement.c so that they're accesible from other files.
#define OBJ_EVENT_PAL_TAG_BRENDAN                 0x1100
#define OBJ_EVENT_PAL_TAG_BRENDAN_REFLECTION      0x1101
#define OBJ_EVENT_PAL_TAG_BRIDGE_REFLECTION       0x1102
#define OBJ_EVENT_PAL_TAG_NPC_1                   0x1103
#define OBJ_EVENT_PAL_TAG_NPC_2                   0x1104
#define OBJ_EVENT_PAL_TAG_NPC_3                   0x1105
#define OBJ_EVENT_PAL_TAG_NPC_4                   0x1106
#define OBJ_EVENT_PAL_TAG_NPC_1_REFLECTION        0x1107
#define OBJ_EVENT_PAL_TAG_NPC_2_REFLECTION        0x1108
#define OBJ_EVENT_PAL_TAG_NPC_3_REFLECTION        0x1109
#define OBJ_EVENT_PAL_TAG_NPC_4_REFLECTION        0x110A
#define OBJ_EVENT_PAL_TAG_QUINTY_PLUMP            0x110B
#define OBJ_EVENT_PAL_TAG_QUINTY_PLUMP_REFLECTION 0x110C
#define OBJ_EVENT_PAL_TAG_TRUCK                   0x110D
#define OBJ_EVENT_PAL_TAG_VIGOROTH                0x110E
#define OBJ_EVENT_PAL_TAG_ZIGZAGOON               0x110F
#define OBJ_EVENT_PAL_TAG_MAY                     0x1110
#define OBJ_EVENT_PAL_TAG_MAY_REFLECTION          0x1111
#define OBJ_EVENT_PAL_TAG_MOVING_BOX              0x1112
#define OBJ_EVENT_PAL_TAG_CABLE_CAR               0x1113
#define OBJ_EVENT_PAL_TAG_SSTIDAL                 0x1114
#define OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER       0x1115
#define OBJ_EVENT_PAL_TAG_KYOGRE                  0x1116
#define OBJ_EVENT_PAL_TAG_KYOGRE_REFLECTION       0x1117
#define OBJ_EVENT_PAL_TAG_GROUDON                 0x1118
#define OBJ_EVENT_PAL_TAG_GROUDON_REFLECTION      0x1119
#define OBJ_EVENT_PAL_TAG_UNUSED                  0x111A
#define OBJ_EVENT_PAL_TAG_SUBMARINE_SHADOW        0x111B
#define OBJ_EVENT_PAL_TAG_POOCHYENA               0x111C
#define OBJ_EVENT_PAL_TAG_RED_LEAF                0x111D
#define OBJ_EVENT_PAL_TAG_DEOXYS                  0x111E
#define OBJ_EVENT_PAL_TAG_BIRTH_ISLAND_STONE      0x111F
#define OBJ_EVENT_PAL_TAG_HO_OH                   0x1120
#define OBJ_EVENT_PAL_TAG_LUGIA                   0x1121
#define OBJ_EVENT_PAL_TAG_RS_BRENDAN              0x1122
#define OBJ_EVENT_PAL_TAG_RS_MAY                  0x1123
#define OBJ_EVENT_PAL_TAG_DYNAMIC                 0x1124
#define OBJ_EVENT_PAL_TAG_PLAYER_RED              0x1125
#define OBJ_EVENT_PAL_TAG_PLAYER_RED_REFLECTION   0x1126
#define OBJ_EVENT_PAL_TAG_PLAYER_GREEN            0x1127
#define OBJ_EVENT_PAL_TAG_PLAYER_GREEN_REFLECTION 0x1128
#define OBJ_EVENT_PAL_TAG_NPC_BLUE                0x1129
#define OBJ_EVENT_PAL_TAG_NPC_PINK                0x112A
#define OBJ_EVENT_PAL_TAG_NPC_GREEN               0x112B
#define OBJ_EVENT_PAL_TAG_NPC_WHITE               0x112C
#define OBJ_EVENT_PAL_TAG_NPC_BLUE_REFLECTION     0x112D
#define OBJ_EVENT_PAL_TAG_NPC_PINK_REFLECTION     0x112E
#define OBJ_EVENT_PAL_TAG_NPC_GREEN_REFLECTION    0x112F
#define OBJ_EVENT_PAL_TAG_NPC_WHITE_REFLECTION    0x1130
#define OBJ_EVENT_PAL_TAG_METEORITE               0x1131
#define OBJ_EVENT_PAL_TAG_SEAGALLOP               0x1132
#define OBJ_EVENT_PAL_TAG_SS_ANNE                 0x1133

// HnS palette tags
#define OBJ_EVENT_PAL_TAG_NPC_1_HNS               0x1170
#define OBJ_EVENT_PAL_TAG_NPC_2_HNS               0x1171
#define OBJ_EVENT_PAL_TAG_NPC_3_HNS               0x1172
#define OBJ_EVENT_PAL_TAG_NPC_4_HNS               0x1173
#define OBJ_EVENT_PAL_TAG_BIRTH_ISLAND_STONE_HNS  0x1174
#define OBJ_EVENT_PAL_TAG_BUGSY_HNS               0x1175
#define OBJ_EVENT_PAL_TAG_CHUCK_HNS               0x1176
#define OBJ_EVENT_PAL_TAG_CLAIR_HNS               0x1177
#define OBJ_EVENT_PAL_TAG_ELM_HNS                 0x1178
#define OBJ_EVENT_PAL_TAG_EUSINE_HNS              0x1179
#define OBJ_EVENT_PAL_TAG_FALKNER_HNS             0x117A
#define OBJ_EVENT_PAL_TAG_JANINE_HNS              0x117B
#define OBJ_EVENT_PAL_TAG_JASMINE_HNS             0x117C
#define OBJ_EVENT_PAL_TAG_KAREN_HNS               0x117D
#define OBJ_EVENT_PAL_TAG_KIMONO_HNS              0x117E
#define OBJ_EVENT_PAL_TAG_LANCE_HNS               0x117F
#define OBJ_EVENT_PAL_TAG_LAPRAS_HNS              0x1180
#define OBJ_EVENT_PAL_TAG_LEGENDARY_SHADOW_HNS    0x1181
#define OBJ_EVENT_PAL_TAG_LIGHT_HNS               0x1182
#define OBJ_EVENT_PAL_TAG_LIGHT_2_HNS             0x1183
#define OBJ_EVENT_PAL_TAG_MORTY_HNS               0x1184
#define OBJ_EVENT_PAL_TAG_PRYCE_HNS               0x1185
#define OBJ_EVENT_PAL_TAG_RED_HNS                 0x1186
#define OBJ_EVENT_PAL_TAG_ROCKET_1_HNS            0x1187
#define OBJ_EVENT_PAL_TAG_ROCKET_2_HNS            0x1188
#define OBJ_EVENT_PAL_TAG_ROCKET_3_HNS            0x1189
#define OBJ_EVENT_PAL_TAG_ROCKET_4_HNS            0x118A
#define OBJ_EVENT_PAL_TAG_SAGE_HNS                0x118B
#define OBJ_EVENT_PAL_TAG_SCIENTIST_F_HNS         0x118C
#define OBJ_EVENT_PAL_TAG_SHINY_GYARADOS_HNS      0x118D
#define OBJ_EVENT_PAL_TAG_SILVER_HNS              0x118E
#define OBJ_EVENT_PAL_TAG_SLOWPOKE_HNS            0x118F
#define OBJ_EVENT_PAL_TAG_SNORLAX_HNS             0x1190
#define OBJ_EVENT_PAL_TAG_SSAQUA_HNS              0x1191
#define OBJ_EVENT_PAL_TAG_STEVEN_HNS              0x1192
#define OBJ_EVENT_PAL_TAG_TOWER_BEAM_HNS          0x1193
#define OBJ_EVENT_PAL_TAG_TRAIN_HNS               0x1194
#define OBJ_EVENT_PAL_TAG_WHIRLPOOL_HNS           0x1195
#define OBJ_EVENT_PAL_TAG_WHITNEY_HNS             0x1196
#define OBJ_EVENT_PAL_TAG_WILL_HNS                0x1197
#define OBJ_EVENT_PAL_TAG_GOLD_HNS                0x1198
#define OBJ_EVENT_PAL_TAG_GOLD_REFLECTION_HNS     0x1199
#define OBJ_EVENT_PAL_TAG_KRIS_HNS                0x119A
#define OBJ_EVENT_PAL_TAG_KRIS_REFLECTION_HNS     0x119B
#define OBJ_EVENT_PAL_TAG_ALOLA_OAK_HNS           0x119C

#if OW_FOLLOWERS_POKEBALLS
// Vanilla
#define OBJ_EVENT_PAL_TAG_BALL_MASTER             0x1150
#define OBJ_EVENT_PAL_TAG_BALL_ULTRA              0x1151
#define OBJ_EVENT_PAL_TAG_BALL_GREAT              0x1152
#define OBJ_EVENT_PAL_TAG_BALL_SAFARI             0x1153
#define OBJ_EVENT_PAL_TAG_BALL_NET                0x1154
#define OBJ_EVENT_PAL_TAG_BALL_DIVE               0x1155
#define OBJ_EVENT_PAL_TAG_BALL_NEST               0x1156
#define OBJ_EVENT_PAL_TAG_BALL_REPEAT             0x1157
#define OBJ_EVENT_PAL_TAG_BALL_TIMER              0x1158
#define OBJ_EVENT_PAL_TAG_BALL_LUXURY             0x1159
#define OBJ_EVENT_PAL_TAG_BALL_PREMIER            0x115A
// Gen IV/Sinnoh
#define OBJ_EVENT_PAL_TAG_BALL_DUSK               0x115B
#define OBJ_EVENT_PAL_TAG_BALL_HEAL               0x115C
#define OBJ_EVENT_PAL_TAG_BALL_QUICK              0x115D
#define OBJ_EVENT_PAL_TAG_BALL_CHERISH            0x115E
#define OBJ_EVENT_PAL_TAG_BALL_PARK               0x115F
// Gen II/Johto Apricorns
#define OBJ_EVENT_PAL_TAG_BALL_FAST               0x1160
#define OBJ_EVENT_PAL_TAG_BALL_LEVEL              0x1161
#define OBJ_EVENT_PAL_TAG_BALL_LURE               0x1162
#define OBJ_EVENT_PAL_TAG_BALL_HEAVY              0x1163
#define OBJ_EVENT_PAL_TAG_BALL_LOVE               0x1164
#define OBJ_EVENT_PAL_TAG_BALL_FRIEND             0x1165
#define OBJ_EVENT_PAL_TAG_BALL_MOON               0x1166
#define OBJ_EVENT_PAL_TAG_BALL_SPORT              0x1167
// Gen V
#define OBJ_EVENT_PAL_TAG_BALL_DREAM              0x1168
// Gen VII
#define OBJ_EVENT_PAL_TAG_BALL_BEAST              0x1169
// Gen VIII
#define OBJ_EVENT_PAL_TAG_BALL_STRANGE            0x116A
#endif //OW_FOLLOWERS_POKEBALLS
// Used as a placeholder follower graphic
#define OBJ_EVENT_PAL_TAG_SUBSTITUTE              0x7611
#define OBJ_EVENT_PAL_TAG_LIGHT                   0x8001
#define OBJ_EVENT_PAL_TAG_LIGHT_2                 0x8002
#define OBJ_EVENT_PAL_TAG_EMOTES                  0x8003
#define OBJ_EVENT_PAL_TAG_NEON_LIGHT              0x8004
// Not a real OW palette tag; used for the white flash applied to followers
#define OBJ_EVENT_PAL_TAG_WHITE                   (OBJ_EVENT_PAL_TAG_NONE - 1)
#define OBJ_EVENT_PAL_TAG_NONE                    0x11FF

// This + localId is used as the tileTag
// for compressed graphicsInfos
// '(C)ompressed (E)vent'
#define COMP_OW_TILE_TAG_BASE 0xCE00

#endif  // GUARD_CONSTANTS_EVENT_OBJECTS_H
