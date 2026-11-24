#include "global.h"
#include "berry.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_control_avatar.h"
#include "fieldmap.h"
#include "item.h"
#include "item_menu.h"
#include "main.h"
#include "random.h"
#include "script_pokemon_util.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "constants/event_object_movement.h"
#include "constants/items.h"

static u8 BerryTreeGetNumStagesWatered(struct BerryTree *tree);
static u8 GetNumStagesWateredByBerryTreeId(u8 id);

#if IS_HNS
static u8 sLastPickedBerryType = BERRY_NONE;
#endif
static u8 CalcBerryYieldInternal(u16 max, u16 min, u8 water);
static u8 CalcBerryYield(struct BerryTree *tree);
static u32 GetBerryTreeAge(u8 id, u8 stage);
static u8 GetBerryCountByBerryTreeId(u8 id);
static u16 GetStageDurationByBerryType(u8);
static u8 GetDrainRateByBerryType(u8);
static u8 GetWaterBonusByBerryType(u8);
static u8 GetWeedingBonusByBerryType(u8);
static u8 GetPestsBonusByBerryType(u8);
static void SetTreeMutations(u8 id, u8 berry);
static u8 GetTreeMutationValue(u8 id);
static enum Species GetBerryPestSpecies(u8 berryId);
static void TryForWeeds(struct BerryTree *tree);
static void TryForPests(struct BerryTree *tree);
static void AddTreeBonus(struct BerryTree *tree, u8 bonus);

// Check include/config/overworld.h configs and throw an error if illegal
#if OW_BERRY_GROWTH_RATE < GEN_3 || (OW_BERRY_GROWTH_RATE > GEN_7 && OW_BERRY_GROWTH_RATE != GEN_6_ORAS)
#error "OW_BERRY_GROWTH_RATE must be between GEN_3 and GEN_7!"
#endif

#if OW_BERRY_YIELD_RATE < GEN_3 || (OW_BERRY_YIELD_RATE > GEN_6 && OW_BERRY_YIELD_RATE != GEN_6_ORAS)
#error "OW_BERRY_YIELD_RATE must be between GEN_3 and GEN_6!"
#elif OW_BERRY_YIELD_RATE == GEN_5
#error "OW_BERRY_YIELD_RATE can not be GEN_5!"
#endif

#if OW_BERRY_MOISTURE && OW_BERRY_DRAIN_RATE != GEN_4 && OW_BERRY_DRAIN_RATE != GEN_6_XY && OW_BERRY_DRAIN_RATE != GEN_6_ORAS
#error "OW_BERRY_DRAIN_RATE must be GEN_5, GEN_6_XY or GEN_6_ORAS!"
#endif

#if OW_BERRY_COLORS != GEN_6_XY && OW_BERRY_COLORS != GEN_6_ORAS
#error "OW_BERRY_COLORS must be GEN_6_XY or GEN_6_ORAS!"
#endif

// HnS: BERRY_ID_* values are stored in SaveBlock1.berryTrees[].berry, so FOREACH_BERRY
// (include/constants/berries.h) must keep the old ITEM_TO_BERRY numbering (item ID order).
// Upstream #7305 moved CHILAN after BABIRI and ROSELI after ROWAP, which would renumber 36-65.
#define HNS_ASSERT_BERRY_ID_ORDER(_berry) STATIC_ASSERT(BERRY_ID_##_berry == ITEM_##_berry##_BERRY - FIRST_BERRY_INDEX + 1, HnsBerryIdOrder_##_berry)
FOREACH_BERRY(HNS_ASSERT_BERRY_ID_ORDER)
#undef HNS_ASSERT_BERRY_ID_ORDER
STATIC_ASSERT(BERRY_ID_BELUE == 35 && BERRY_ID_CHILAN == 36 && BERRY_ID_OCCA == 37 && BERRY_ID_BABIRI == 52, HnsBerryIdSave_35_52)
STATIC_ASSERT(BERRY_ID_ROSELI == 53 && BERRY_ID_LIECHI == 54 && BERRY_ID_ENIGMA == 61 && BERRY_ID_ROWAP == 65, HnsBerryIdSave_53_65)
STATIC_ASSERT(BERRY_ID_KEE == 66 && BERRY_ID_MARANGA == 67 && BERRY_ID_ENGIMA_E_READER == 68 && NUM_BERRIES == 68, HnsBerryIdSave_66_68)

#define GROWTH_DURATION(g3, g4, g5, xy, oras, g7) OW_BERRY_GROWTH_RATE == GEN_3 ? g3 : OW_BERRY_GROWTH_RATE == GEN_4 ? g4 : OW_BERRY_GROWTH_RATE == GEN_5 ? g5 : OW_BERRY_GROWTH_RATE == GEN_6_XY ? xy : OW_BERRY_GROWTH_RATE == GEN_6_ORAS ? oras : g7
#define YIELD_RATE(g3, g4, xy, oras) OW_BERRY_YIELD_RATE == GEN_3 ? g3 : OW_BERRY_YIELD_RATE == GEN_4 ? g4 : OW_BERRY_YIELD_RATE == GEN_6_XY ? xy : oras

const struct Berry gBerries[NUM_BERRIES + 1] =
{
    [BERRY_ID_NONE] = {
        .info = {
            .name = _("?????"),
            .description1 = COMPOUND_STRING("????????"),
            .description2 = COMPOUND_STRING("????????"),
        },
        .berryPic = gBerryPic_Enigma,
        .berryPal =  gBerryPalette_Enigma,
        .berryTreePicTable = gPicTable_DurinBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Durin,
    },
    [BERRY_ID_CHERI] =
    {
        .info = {
            .name = _("버치"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_RED,
            .size = 20,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("새빨간 열매는 아주 맵고 자극적이다."),
            .description2 = COMPOUND_STRING("매우 귀여운 꽃이 핀다."),
            .growthDuration = GROWTH_DURATION(12, 12, 18, 24, 16, 24),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FIRE,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  20,
        .berryPic = gBerryPic_Cheri,
        .berryPal =  gBerryPalette_Cheri,
        .berryTreePicTable = gPicTable_CheriBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Cheri,
    },

    [BERRY_ID_CHESTO] =
    {
        .info = {
            .name = _("유루"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 80,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("겉과 속 모두 딱딱하고 떫지만"),
            .description2 = COMPOUND_STRING("충분히 다 먹을 수 있다."),
            .growthDuration = GROWTH_DURATION(12, 12, 18, 24, 16, 24),
            .spicy = 0,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_WATER,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  20,
        .berryPic = gBerryPic_Chesto,
        .berryPal = gBerryPalette_Chesto,
        .berryTreePicTable = gPicTable_ChestoBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Chesto,
    },

    [BERRY_ID_PECHA] =
    {
        .info = {
            .name = _("복슝"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = BERRY_COLOR_PINK,
            .size = 40,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("속에 빈 공간이 있어 먹을 부분은 적지만"),
            .description2 = COMPOUND_STRING("매우 달고 맛있다."),
            .growthDuration = GROWTH_DURATION(12, 12, 18, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 4,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ELECTRIC,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  20,
        .berryPic = gBerryPic_Pecha,
        .berryPal =  gBerryPalette_Pecha,
        .berryTreePicTable = gPicTable_PechaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Pecha,
    },

    [BERRY_ID_RAWST] =
    {
        .info = {
            .name = _("복분"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 32,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("잎사귀가 길고 둥글게 말린 것은"),
            .description2 = COMPOUND_STRING("약간 쓴맛이 강하다."),
            .growthDuration = GROWTH_DURATION(12, 12, 18, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 10,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_GRASS,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  20,
        .berryPic = gBerryPic_Rawst,
        .berryPal =  gBerryPalette_Rawst,
        .berryTreePicTable = gPicTable_RawstBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Rawst,
    },

    [BERRY_ID_ASPEAR] =
    {
        .info = {
            .name = _("배리"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_YELLOW,
            .size = 50,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("껍질은 딱딱하지만 속에는 즙이 많다."),
            .description2 = COMPOUND_STRING("상큼한 신맛이 특징이다."),
            .growthDuration = GROWTH_DURATION(12, 12, 18, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 10,
            .smoothness = 25,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ICE,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  20,
        .berryPic = gBerryPic_Aspear,
        .berryPal = gBerryPalette_Aspear,
        .berryTreePicTable = gPicTable_AspearBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Aspear,
    },

    [BERRY_ID_LEPPA] =
    {
        .info = {
            .name = _("과사"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_RED,
            .size = 28,
            .maxYield = YIELD_RATE(6, 5, 15, 22),
            .minYield = YIELD_RATE(5, 2, 2, 2),
            .description1 = COMPOUND_STRING("버치 등보다 조금 성장이 늦다."),
            .description2 = COMPOUND_STRING("열매가 작을수록 맛있다."),
            .growthDuration = GROWTH_DURATION(16, 16, 24, 24, 16, 24),
            .spicy = 10,
            .dry = 0,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 15,
            .waterBonus = 15,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FIGHTING,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Leppa,
        .berryPal =  gBerryPalette_Leppa,
        .berryTreePicTable = gPicTable_LeppaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Leppa,
    },

    [BERRY_ID_ORAN] =
    {
        .info = {
            .name = _("오랭"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_BLUE,
            .size = 35,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("자연의 은혜가 하나로 모여"),
            .description2 = COMPOUND_STRING("여러 맛이 입안에 펼쳐진다."),
            .growthDuration = GROWTH_DURATION(12, 16, 24, 24, 16, 24),
            .spicy = 10,
            .dry = 10,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 4,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_POISON,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Oran,
        .berryPal =   gBerryPalette_Oran,
        .berryTreePicTable = gPicTable_OranBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Oran,
    },

    [BERRY_ID_PERSIM] =
    {
        .info = {
            .name = _("시몬"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_PINK,
            .size = 47,
            .maxYield = YIELD_RATE(6, 5, 15, 20),
            .minYield = YIELD_RATE(5, 2, 4, 4),
            .description1 = COMPOUND_STRING("태양 에너지를 흡수할수록"),
            .description2 = COMPOUND_STRING("더 선명한 색으로 성장한다."),
            .growthDuration = GROWTH_DURATION(12, 16, 24, 24, 16, 24),
            .spicy = 10,
            .dry = 10,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 15,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_GROUND,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Persim,
        .berryPal = gBerryPalette_Persim,
        .berryTreePicTable = gPicTable_PersimBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Persim,
    },

    [BERRY_ID_LUM] =
    {
        .info = {
            .name = _("리샘"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 34,
            .maxYield = YIELD_RATE(6, 5, 20, 18),
            .minYield = YIELD_RATE(5, 2, 3, 2),
            .description1 = COMPOUND_STRING("포켓몬에 좋은 성분을 천천히 비축해"),
            .description2 = COMPOUND_STRING("자라기까지 시간이 걸린다."),
            .growthDuration = GROWTH_DURATION(48, 48, 72, 48, 32, 48),
            .spicy = 10,
            .dry = 10,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 8,
            .waterBonus = 12,
            .weedsBonus = 1,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FLYING,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Lum,
        .berryPal =    gBerryPalette_Lum,
        .berryTreePicTable = gPicTable_LumBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Lum,
    },

    [BERRY_ID_SITRUS] =
    {
        .info = {
            .name = _("자뭉"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_YELLOW,
            .size = 95,
            .maxYield = YIELD_RATE(6, 5, 20, 27),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("오랭열매와 같은 종류에서 갈라졌다."),
            .description2 = COMPOUND_STRING("오랭보다 크고 둥글다."),
            .growthDuration = GROWTH_DURATION(24, 32, 48, 48, 32, 48),
            .spicy = 10,
            .dry = 10,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 7,
            .waterBonus = 12,
            .weedsBonus = 1,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_PSYCHIC,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Sitrus,
        .berryPal = gBerryPalette_Sitrus,
        .berryTreePicTable = gPicTable_SitrusBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Sitrus,
    },

    [BERRY_ID_FIGY] =
    {
        .info = {
            .name = _("무화"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_RED,
            .size = 100,
            .maxYield = YIELD_RATE(6, 5, 15, 15),
            .minYield = YIELD_RATE(5, 1, 3, 3),
            .description1 = COMPOUND_STRING("베어 먹은 듯한 둥근 모양이지만"),
            .description2 = COMPOUND_STRING("속에는 매운 성분이 가득하다."),
            .growthDuration = GROWTH_DURATION(24, 20, 30, 24, 16, 24),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 10,
            .waterBonus = 15,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_BUG,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Figy,
        .berryPal =   gBerryPalette_Figy,
        .berryTreePicTable = gPicTable_FigyBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Figy,
    },

    [BERRY_ID_WIKI] =
    {
        .info = {
            .name = _("위키"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 115,
            .maxYield = YIELD_RATE(6, 5, 15, 15),
            .minYield = YIELD_RATE(5, 1, 3, 3),
            .description1 = COMPOUND_STRING("번식 범위를 넓히기 위해"),
            .description2 = COMPOUND_STRING("포켓몬이 쥐기 쉽게 울퉁불퉁하다."),
            .growthDuration = GROWTH_DURATION(24, 20, 30, 24, 16, 24),
            .spicy = 0,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 10,
            .waterBonus = 15,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ROCK,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Wiki,
        .berryPal =   gBerryPalette_Wiki,
        .berryTreePicTable = gPicTable_WikiBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Wiki,
    },

    [BERRY_ID_MAGO] =
    {
        .info = {
            .name = _("마고"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_PINK,
            .size = 126,
            .maxYield = YIELD_RATE(6, 5, 15, 15),
            .minYield = YIELD_RATE(5, 1, 3, 3),
            .description1 = COMPOUND_STRING("자랄수록 열매가 점점 구부러진다."),
            .description2 = COMPOUND_STRING("많이 구부러질수록 달다."),
            .growthDuration = GROWTH_DURATION(24, 20, 30, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 10,
            .waterBonus = 15,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_GHOST,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Mago,
        .berryPal =   gBerryPalette_Mago,
        .berryTreePicTable = gPicTable_MagoBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Mago,
    },

    [BERRY_ID_AGUAV] =
    {
        .info = {
            .name = _("아바"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 64,
            .maxYield = YIELD_RATE(6, 5, 15, 15),
            .minYield = YIELD_RATE(5, 1, 3, 3),
            .description1 = COMPOUND_STRING("꽃이 작아 햇빛을 잘 흡수하지 못한다."),
            .description2 = COMPOUND_STRING("꼭지에 가까운 쪽이 쓰다."),
            .growthDuration = GROWTH_DURATION(24, 20, 30, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 10,
            .sour = 0,
            .smoothness = 25,
            .drainRate = 10,
            .waterBonus = 15,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_DRAGON,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Aguav,
        .berryPal =  gBerryPalette_Aguav,
        .berryTreePicTable = gPicTable_AguavBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Aguav,
    },

    [BERRY_ID_IAPAPA] =
    {
        .info = {
            .name = _("파야"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 223,
            .maxYield = YIELD_RATE(6, 5, 15, 15),
            .minYield = YIELD_RATE(5, 1, 3, 3),
            .description1 = COMPOUND_STRING("매우 크고 신 나무열매다."),
            .description2 = COMPOUND_STRING("즙 많은 과육이 신맛을 더한다."),
            .growthDuration = GROWTH_DURATION(24, 20, 30, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 10,
            .smoothness = 25,
            .drainRate = 10,
            .waterBonus = 15,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_DARK,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Iapapa,
        .berryPal = gBerryPalette_Iapapa,
        .berryTreePicTable = gPicTable_IapapaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Iapapa,
    },

    [BERRY_ID_RAZZ] =
    {
        .info = {
            .name = _("라즈"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_RED,
            .size = 120,
            .maxYield = YIELD_RATE(6, 10, 15, 20),
            .minYield = YIELD_RATE(5, 2, 3, 4),
            .description1 = COMPOUND_STRING("주위의 빨간 낱알은 약간 맵고"),
            .description2 = COMPOUND_STRING("중앙 부분은 떫다."),
            .growthDuration = GROWTH_DURATION(4, 8, 12, 24, 16, 24),
            .spicy = 10,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 20,
            .drainRate = 35,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_STEEL,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  70,
        .berryPic = gBerryPic_Razz,
        .berryPal =   gBerryPalette_Razz,
        .berryTreePicTable = gPicTable_RazzBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Razz,
    },

    [BERRY_ID_BLUK] =
    {
        .info = {
            .name = _("블리"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 108,
            .maxYield = YIELD_RATE(6, 10, 15, 20),
            .minYield = YIELD_RATE(5, 2, 3, 4),
            .description1 = COMPOUND_STRING("부드러운 껍질의 파란 낱알을 먹으면"),
            .description2 = COMPOUND_STRING("입속을 까맣게 물들인다."),
            .growthDuration = GROWTH_DURATION(4, 8, 12, 24, 16, 24),
            .spicy = 0,
            .dry = 10,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 20,
            .drainRate = 35,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FIRE,
        .naturalGiftPower = 90,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  70,
        .berryPic = gBerryPic_Bluk,
        .berryPal =   gBerryPalette_Bluk,
        .berryTreePicTable = gPicTable_RazzBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Razz,
    },

    [BERRY_ID_NANAB] =
    {
        .info = {
            .name = _("나나"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_PINK,
            .size = 77,
            .maxYield = YIELD_RATE(6, 10, 15, 20),
            .minYield = YIELD_RATE(5, 2, 3, 4),
            .description1 = COMPOUND_STRING("달고 쓴 나무열매로"),
            .description2 = COMPOUND_STRING("세상에서 7번째로 발견되었다."),
            .growthDuration = GROWTH_DURATION(4, 8, 12, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 10,
            .bitter = 10,
            .sour = 0,
            .smoothness = 20,
            .drainRate = 35,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_WATER,
        .naturalGiftPower = 90,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  70,
        .berryPic = gBerryPic_Nanab,
        .berryPal =  gBerryPalette_Nanab,
        .berryTreePicTable = gPicTable_MagoBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Mago,
    },

    [BERRY_ID_WEPEAR] =
    {
        .info = {
            .name = _("서배"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 74,
            .maxYield = YIELD_RATE(6, 10, 15, 20),
            .minYield = YIELD_RATE(5, 2, 3, 4),
            .description1 = COMPOUND_STRING("쓴맛과 신맛이 섞여 소화에 좋다."),
            .description2 = COMPOUND_STRING("하얗고 아름다운 꽃이 핀다."),
            .growthDuration = GROWTH_DURATION(4, 8, 12, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 35,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ELECTRIC,
        .naturalGiftPower = 90,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  70,
        .berryPic = gBerryPic_Wepear,
        .berryPal = gBerryPalette_Wepear,
        .berryTreePicTable = gPicTable_WepearBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Wepear,
    },

    [BERRY_ID_PINAP] =
    {
        .info = {
            .name = _("파인"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_YELLOW,
            .size = 80,
            .maxYield = YIELD_RATE(6, 10, 15, 20),
            .minYield = YIELD_RATE(5, 2, 3, 4),
            .description1 = COMPOUND_STRING("신 껍질을 벗긴 매운 열매를"),
            .description2 = COMPOUND_STRING("갈아 으깨면 약이 된다고 한다."),
            .growthDuration = GROWTH_DURATION(4, 8, 12, 24, 16, 24),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 35,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_GRASS,
        .naturalGiftPower = 90,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  70,
        .berryPic = gBerryPic_Pinap,
        .berryPal =  gBerryPalette_Pinap,
        .berryTreePicTable = gPicTable_IapapaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Iapapa,
    },

    [BERRY_ID_POMEG] =
    {
        .info = {
            .name = _("유석"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_RED,
            .size = 135,
            .maxYield = YIELD_RATE(11, 5, 20, 26),
            .minYield = YIELD_RATE(10, 1, 1, 2),
            .description1 = COMPOUND_STRING("껍질을 벗기면 과육이 많이 나온다."),
            .description2 = COMPOUND_STRING("조금 매콤달콤한 어른스러운 맛이다."),
            .growthDuration = GROWTH_DURATION(12, 32, 48, 48, 32, 48),
            .spicy = 10,
            .dry = 0,
            .sweet = 10,
            .bitter = 10,
            .sour = 0,
            .smoothness = 20,
            .drainRate = 8,
            .waterBonus = 5,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ICE,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 100,
        .berryCrushPowder = 100,
        .berryPic = gBerryPic_Pomeg,
        .berryPal =  gBerryPalette_Pomeg,
        .berryTreePicTable = gPicTable_PomegBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Pomeg,
    },

    [BERRY_ID_KELPSY] =
    {
        .info = {
            .name = _("시마"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_BLUE,
            .size = 150,
            .maxYield = YIELD_RATE(11, 5, 20, 26),
            .minYield = YIELD_RATE(10, 1, 1, 2),
            .description1 = COMPOUND_STRING("그대로 먹거나 바싹 졸이면"),
            .description2 = COMPOUND_STRING("진액을 요리 조미료로도 쓴다."),
            .growthDuration = GROWTH_DURATION(12, 32, 48, 48, 32, 48),
            .spicy = 0,
            .dry = 10,
            .sweet = 0,
            .bitter = 10,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 8,
            .waterBonus = 5,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FIGHTING,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 100,
        .berryCrushPowder = 100,
        .berryPic = gBerryPic_Kelpsy,
        .berryPal = gBerryPalette_Kelpsy,
        .berryTreePicTable = gPicTable_KelpsyBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Kelpsy,
    },

    [BERRY_ID_QUALOT] =
    {
        .info = {
            .name = _("파비"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_YELLOW : BERRY_COLOR_PINK,
            .size = 110,
            .maxYield = YIELD_RATE(11, 5, 20, 26),
            .minYield = YIELD_RATE(10, 1, 1, 2),
            .description1 = COMPOUND_STRING("비가 잦고 습한 땅에서도"),
            .description2 = COMPOUND_STRING("쑥쑥 잘 자란다."),
            .growthDuration = GROWTH_DURATION(12, 32, 48, 48, 32, 48),
            .spicy = 10,
            .dry = 0,
            .sweet = 10,
            .bitter = 0,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 8,
            .waterBonus = 5,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_POISON,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 100,
        .berryCrushPowder = 100,
        .berryPic = gBerryPic_Qualot,
        .berryPal = gBerryPalette_Qualot,
        .berryTreePicTable = gPicTable_WepearBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Wepear,
    },

    [BERRY_ID_HONDEW] =
    {
        .info = {
            .name = _("로매"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 162,
            .maxYield = YIELD_RATE(11, 5, 20, 26),
            .minYield = YIELD_RATE(10, 1, 1, 2),
            .description1 = COMPOUND_STRING("조금 희귀하고 고급스러워"),
            .description2 = COMPOUND_STRING("선물로 받으면 좋아한다."),
            .growthDuration = GROWTH_DURATION(12, 32, 48, 48, 32, 48),
            .spicy = 10,
            .dry = 10,
            .sweet = 0,
            .bitter = 10,
            .sour = 0,
            .smoothness = 20,
            .drainRate = 8,
            .waterBonus = 5,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_GROUND,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 100,
        .berryCrushPowder = 100,
        .berryPic = gBerryPic_Hondew,
        .berryPal = gBerryPalette_Hondew,
        .berryTreePicTable = gPicTable_HondewBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Hondew,
    },

    [BERRY_ID_GREPA] =
    {
        .info = {
            .name = _("또뽀"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 149,
            .maxYield = YIELD_RATE(11, 5, 20, 26),
            .minYield = YIELD_RATE(10, 1, 1, 2),
            .description1 = COMPOUND_STRING("매우 부드러워 덥석 베어 물면"),
            .description2 = COMPOUND_STRING("단맛과 신맛이 입안에 퍼진다."),
            .growthDuration = GROWTH_DURATION(12, 32, 48, 48, 32, 48),
            .spicy = 0,
            .dry = 10,
            .sweet = 10,
            .bitter = 0,
            .sour = 10,
            .smoothness = 20,
            .drainRate = 8,
            .waterBonus = 5,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FLYING,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 100,
        .berryCrushPowder = 100,
        .berryPic = gBerryPic_Grepa,
        .berryPal =  gBerryPalette_Grepa,
        .berryTreePicTable = gPicTable_GrepaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Grepa,
    },

    [BERRY_ID_TAMATO] =
    {
        .info = {
            .name = _("토망"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_RED,
            .size = 200,
            .maxYield = YIELD_RATE(11, 5, 20, 26),
            .minYield = YIELD_RATE(10, 1, 1, 2),
            .description1 = COMPOUND_STRING("크고 매운 나무열매로"),
            .description2 = COMPOUND_STRING("추운 계절에 먹으면 몸이 따뜻해진다."),
            .growthDuration = GROWTH_DURATION(24, 32, 48, 48, 32, 48),
            .spicy = 20,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 8,
            .waterBonus = 5,
            .weedsBonus = 3,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_PSYCHIC,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 150,
        .berryPic = gBerryPic_Tamato,
        .berryPal = gBerryPalette_Tamato,
        .berryTreePicTable = gPicTable_TamatoBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Tamato,
    },

    [BERRY_ID_CORNN] =
    {
        .info = {
            .name = _("수숙"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 75,
            .maxYield = YIELD_RATE(6, 10, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("떫은맛이 굉장히 강해"),
            .description2 = COMPOUND_STRING("한두 개로는 참맛을 모른다."),
            .growthDuration = GROWTH_DURATION(24, 24, 36, 24, 16, 24),
            .spicy = 0,
            .dry = 20,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 10,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_BUG,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 150,
        .berryPic = gBerryPic_Cornn,
        .berryPal =  gBerryPalette_Cornn,
        .berryTreePicTable = gPicTable_CornnBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Cornn,
    },

    [BERRY_ID_MAGOST] =
    {
        .info = {
            .name = _("고스티"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_PINK,
            .size = 140,
            .maxYield = YIELD_RATE(6, 10, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("황홀한 단맛 뒤에 은은한 쓴맛이 난다."),
            .description2 = COMPOUND_STRING("포켓몬도 매우 만족하는 맛이다."),
            .growthDuration = GROWTH_DURATION(24, 24, 36, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 20,
            .bitter = 10,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 10,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ROCK,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 150,
        .berryPic = gBerryPic_Magost,
        .berryPal = gBerryPalette_Magost,
        .berryTreePicTable = gPicTable_PomegBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Pomeg,
    },

    [BERRY_ID_RABUTA] =
    {
        .info = {
            .name = _("라부탐"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_GREEN,
            .size = 226,
            .maxYield = YIELD_RATE(6, 10, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("쓰지만 껍질째로 먹으면"),
            .description2 = COMPOUND_STRING("표면의 털이 위장 속을 깨끗이 한다."),
            .growthDuration = GROWTH_DURATION(24, 24, 36, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 20,
            .sour = 10,
            .smoothness = 30,
            .drainRate = 10,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_GHOST,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 150,
        .berryPic = gBerryPic_Rabuta,
        .berryPal = gBerryPalette_Rabuta,
        .berryTreePicTable = gPicTable_RabutaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Rabuta,
    },

    [BERRY_ID_NOMEL] =
    {
        .info = {
            .name = _("노멜"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_YELLOW,
            .size = 285,
            .maxYield = YIELD_RATE(6, 10, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("전체적으로 꽤 시지만"),
            .description2 = COMPOUND_STRING("뾰족한 끝에 신맛이 특히 강하다."),
            .growthDuration = GROWTH_DURATION(24, 24, 36, 24, 16, 24),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 20,
            .smoothness = 30,
            .drainRate = 10,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_DRAGON,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 150,
        .berryPic = gBerryPic_Nomel,
        .berryPal =  gBerryPalette_Nomel,
        .berryTreePicTable = gPicTable_NomelBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Nomel,
    },

    [BERRY_ID_SPELON] =
    {
        .info = {
            .name = _("메호키"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_RED : BERRY_COLOR_PINK,
            .size = 133,
            .maxYield = YIELD_RATE(6, 15, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("불꽃타입이 아닌 포켓몬도"),
            .description2 = COMPOUND_STRING("먹으면 불꽃을 토할 정도로 맵다."),
            .growthDuration = GROWTH_DURATION(72, 60, 90, 24, 16, 24),
            .spicy = 40,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 70,
            .drainRate = 8,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_DARK,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Spelon,
        .berryPal = gBerryPalette_Spelon,
        .berryTreePicTable = gPicTable_SpelonBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Spelon,
    },

    [BERRY_ID_PAMTRE] =
    {
        .info = {
            .name = _("자야"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 244,
            .maxYield = YIELD_RATE(6, 15, 15, 15),
            .minYield = YIELD_RATE(5, 3, 3, 3),
            .description1 = COMPOUND_STRING("먼바다에서 떠내려왔지만"),
            .description2 = COMPOUND_STRING("지금은 성도지방에서도 재배된다."),
            .growthDuration = GROWTH_DURATION(72, 60, 90, 24, 16, 24),
            .spicy = 0,
            .dry = 40,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 70,
            .drainRate = 8,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_STEEL,
        .naturalGiftPower = 90,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Pamtre,
        .berryPal = gBerryPalette_Pamtre,
        .berryTreePicTable = gPicTable_PamtreBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Pamtre,
    },

    [BERRY_ID_WATMEL] =
    {
        .info = {
            .name = _("슈박"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PINK : BERRY_COLOR_GREEN,
            .size = 250,
            .maxYield = YIELD_RATE(6, 15, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("한없이 달콤한 자연의 은혜다."),
            .description2 = COMPOUND_STRING("20인치를 넘는 것도 발견된다."),
            .growthDuration = GROWTH_DURATION(72, 60, 90, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 40,
            .bitter = 10,
            .sour = 0,
            .smoothness = 70,
            .drainRate = 8,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_FIRE,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Watmel,
        .berryPal = gBerryPalette_Watmel,
        .berryTreePicTable = gPicTable_RabutaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Rabuta,
    },

    [BERRY_ID_DURIN] =
    {
        .info = {
            .name = _("두리"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 280,
            .maxYield = YIELD_RATE(6, 15, 15, 15),
            .minYield = YIELD_RATE(5, 3, 3, 3),
            .description1 = COMPOUND_STRING("말도 못 하게 쓴 나무열매로"),
            .description2 = COMPOUND_STRING("한입 먹으면 딸꾹질도 멈춘다."),
            .growthDuration = GROWTH_DURATION(72, 60, 90, 24, 16, 24),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 40,
            .sour = 10,
            .smoothness = 70,
            .drainRate = 8,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_WATER,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Durin,
        .berryPal =  gBerryPalette_Durin,
        .berryTreePicTable = gPicTable_DurinBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Durin,
    },

    [BERRY_ID_BELUE] =
    {
        .info = {
            .name = _("루베"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 300,
            .maxYield = YIELD_RATE(6, 15, 15, 15),
            .minYield = YIELD_RATE(5, 2, 3, 3),
            .description1 = COMPOUND_STRING("먹음직스러운 빛깔이지만"),
            .description2 = COMPOUND_STRING("먹어 보면 굉장히 시다."),
            .growthDuration = GROWTH_DURATION(72, 60, 90, 24, 16, 24),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 40,
            .smoothness = 70,
            .drainRate = 8,
            .waterBonus = 10,
            .weedsBonus = 2,
            .pestsBonus = 6,
        },
        .naturalGiftType = TYPE_ELECTRIC,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Belue,
        .berryPal =  gBerryPalette_Belue,
        .berryTreePicTable = gPicTable_HondewBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Hondew,
    },

    [BERRY_ID_OCCA] =
    {
        .info = {
            .name = _("오카"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_RED,
            .size = 90,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("예전에는 열대 지방에서 많이 재배됐다."),
            .description2 = COMPOUND_STRING("작열하는 매운맛을 자랑한다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 15,
            .dry = 0,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_FIRE,
        .naturalGiftPower = 80,
        .berryCrushDifficulty = 100,
        .berryCrushPowder = 100,
        .berryPic = gBerryPic_Occa,
        .berryPal = gBerryPalette_Occa,
        .berryTreePicTable = gPicTable_OccaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Occa,
    },

    [BERRY_ID_PASSHO] =
    {
        .info = {
            .name = _("꼬시개"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_BLUE,
            .size = 33,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("열매 속에 작은 공기 방울이 많아"),
            .description2 = COMPOUND_STRING("물에 둥둥 뜬다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 15,
            .sweet = 0,
            .bitter = 10,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_WATER,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Passho,
        .berryPal = gBerryPalette_Passho,
        .berryTreePicTable = gPicTable_CornnBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Cornn,
    },

    [BERRY_ID_WACAN] =
    {
        .info = {
            .name = _("초나"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 250,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("땅에 떨어진 번개 에너지를 받아"),
            .description2 = COMPOUND_STRING("크고 풍성하게 자란다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 0,
            .sweet = 15,
            .bitter = 0,
            .sour = 10,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_ELECTRIC,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Wacan,
        .berryPal = gBerryPalette_Wacan,
        .berryTreePicTable = gPicTable_RazzBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Razz,
    },

    [BERRY_ID_RINDO] =
    {
        .info = {
            .name = _("린드"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_GREEN,
            .size = 156,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("식물 특유의 풋내가 날 때도 있지만"),
            .description2 = COMPOUND_STRING("섬유질이 많고 몸에 좋다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 15,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_GRASS,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Rindo,
        .berryPal = gBerryPalette_Rindo,
        .berryTreePicTable = gPicTable_TamatoBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Tamato,
    },

    [BERRY_ID_YACHE] =
    {
        .info = {
            .name = _("플카"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_BLUE,
            .size = 135,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("떫은맛과 신맛이 잘 어우러져"),
            .description2 = COMPOUND_STRING("산뜻하며 차게 하면 더 맛있다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 15,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_ICE,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Yache,
        .berryPal = gBerryPalette_Yache,
        .berryTreePicTable = gPicTable_YacheBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Yache,
    },

    [BERRY_ID_CHOPLE] =
    {
        .info = {
            .name = _("로플"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_RED,
            .size = 77,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("과육에는 열을 내는 성분이 있어"),
            .description2 = COMPOUND_STRING("차가운 마음도 따뜻하게 한다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 15,
            .dry = 0,
            .sweet = 0,
            .bitter = 10,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_FIGHTING,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Chople,
        .berryPal = gBerryPalette_Chople,
        .berryTreePicTable = gPicTable_ChopleBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Chople,
    },

    [BERRY_ID_KEBIA] =
    {
        .info = {
            .name = _("으름"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 90,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("겉은 선명한 초록색이지만"),
            .description2 = COMPOUND_STRING("속에는 떫고 까만 열매가 가득하다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 15,
            .sweet = 0,
            .bitter = 0,
            .sour = 10,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_POISON,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Kebia,
        .berryPal = gBerryPalette_Kebia,
        .berryTreePicTable = gPicTable_KebiaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Kebia,
    },

    [BERRY_ID_SHUCA] =
    {
        .info = {
            .name = _("슈캐"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 42,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("단맛 가득한 과육 속에서"),
            .description2 = COMPOUND_STRING("단단하고 향기로운 풍미가 느껴진다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 10,
            .dry = 0,
            .sweet = 15,
            .bitter = 0,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_GROUND,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  20,
        .berryPic = gBerryPic_Shuca,
        .berryPal = gBerryPalette_Shuca,
        .berryTreePicTable = gPicTable_ShucaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Shuca,
    },

    [BERRY_ID_COBA] =
    {
        .info = {
            .name = _("바코"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_BLUE,
            .size = 278,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("멀리 있던 두 나무열매가 바람에 만나"),
            .description2 = COMPOUND_STRING("열매를 맺어 신종이 됐다고 한다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 10,
            .sweet = 0,
            .bitter = 15,
            .sour = 0,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_FLYING,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Coba,
        .berryPal = gBerryPalette_Coba,
        .berryTreePicTable = gPicTable_RawstBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Rawst,
    },

    [BERRY_ID_PAYAPA] =
    {
        .info = {
            .name = _("야파"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_RED,
            .size = 252,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("사람이 다가가면 끝이 둥글게 부풀어"),
            .description2 = COMPOUND_STRING("마음을 읽는다고 전해진다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 0,
            .sweet = 10,
            .bitter = 0,
            .sour = 15,
            .smoothness = 30,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_PSYCHIC,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Payapa,
        .berryPal = gBerryPalette_Payapa,
        .berryTreePicTable = gPicTable_PayapaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Payapa,
    },

    [BERRY_ID_TANGA] =
    {
        .info = {
            .name = _("리체"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = BERRY_COLOR_GREEN,
            .size = 42,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("끝의 실 같은 꽃잎을 바람에 흔들어"),
            .description2 = COMPOUND_STRING("벌레포켓몬을 불러들인다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 20,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 10,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_BUG,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Tanga,
        .berryPal = gBerryPalette_Tanga,
        .berryTreePicTable = gPicTable_TangaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Tanga,
    },

    [BERRY_ID_CHARTI] =
    {
        .info = {
            .name = _("루미"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 28,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("떫은맛이 강해 피클에 자주 쓰인다."),
            .description2 = COMPOUND_STRING("그대로 먹어도 자극적이다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 10,
            .dry = 20,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_ROCK,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Charti,
        .berryPal = gBerryPalette_Charti,
        .berryTreePicTable = gPicTable_LansatBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Lansat,
    },

    [BERRY_ID_KASIB] =
    {
        .info = {
            .name = _("수불"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_PINK,
            .size = 144,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("옛날부터 이상한 힘이 있다고 전해져"),
            .description2 = COMPOUND_STRING("말려서 부적으로 쓰기도 한다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 10,
            .sweet = 20,
            .bitter = 0,
            .sour = 0,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_GHOST,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Kasib,
        .berryPal = gBerryPalette_Kasib,
        .berryTreePicTable = gPicTable_KasibBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Kasib,
    },

    [BERRY_ID_HABAN] =
    {
        .info = {
            .name = _("하반"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_RED,
            .size = 23,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("많이 모아 졸이면 쓴맛이 사라져"),
            .description2 = COMPOUND_STRING("잼 재료로도 좋다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 0,
            .sweet = 10,
            .bitter = 20,
            .sour = 0,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_DRAGON,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  50,
        .berryCrushPowder =  30,
        .berryPic = gBerryPic_Haban,
        .berryPal = gBerryPalette_Haban,
        .berryTreePicTable = gPicTable_HabanBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Haban,
    },

    [BERRY_ID_COLBUR] =
    {
        .info = {
            .name = _("마코"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_PINK,
            .size = 39,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("표면의 가시로 포켓몬에 들러붙어"),
            .description2 = COMPOUND_STRING("먼 땅까지 운반된다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 10,
            .sour = 20,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_DARK,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Colbur,
        .berryPal = gBerryPalette_Colbur,
        .berryTreePicTable = gPicTable_ColburBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Colbur,
    },

    [BERRY_ID_BABIRI] =
    {
        .info = {
            .name = _("바리비"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 265,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("매우 단단하고 자극이 강한 맛이라"),
            .description2 = COMPOUND_STRING("옛날에는 약 재료로 썼다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 25,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_STEEL,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Babiri,
        .berryPal = gBerryPalette_Babiri,
        .berryTreePicTable = gPicTable_LiechiBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Liechi,
    },

    [BERRY_ID_CHILAN] =
    {
        .info = {
            .name = _("카리"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 34,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("속을 파내 말린 뒤 구멍에 불면"),
            .description2 = COMPOUND_STRING("뭐라 말할 수 없는 소리가 난다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 25,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_NORMAL,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  80,
        .berryCrushPowder =  70,
        .berryPic = gBerryPic_Chilan,
        .berryPal = gBerryPalette_Chilan,
        .berryTreePicTable = gPicTable_GrepaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Grepa,
    },

    [BERRY_ID_LIECHI] =
    {
        .info = {
            .name = _("치리"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_RED : BERRY_COLOR_YELLOW,
            .size = 111,
            .maxYield = YIELD_RATE(6, 5, 10, 13),
            .minYield = YIELD_RATE(5, 1, 1, 2),
            .description1 = COMPOUND_STRING("수수께끼의 치리열매에는 바다의 힘이"),
            .description2 = COMPOUND_STRING("깃들었다는 소문이 있다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 40,
            .dry = 0,
            .sweet = 40,
            .bitter = 0,
            .sour = 10,
            .smoothness = 80,
            .drainRate = 4,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_GRASS,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 180,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Liechi,
        .berryPal = gBerryPalette_Liechi,
        .berryTreePicTable = gPicTable_LiechiBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Liechi,
    },

    [BERRY_ID_GANLON] =
    {
        .info = {
            .name = _("용아"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_BLUE,
            .size = 33,
            .maxYield = YIELD_RATE(6, 5, 10, 13),
            .minYield = YIELD_RATE(5, 1, 1, 2),
            .description1 = COMPOUND_STRING("수수께끼의 용아열매에는 육지의 힘이"),
            .description2 = COMPOUND_STRING("깃들었다는 소문이 있다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 0,
            .dry = 40,
            .sweet = 0,
            .bitter = 40,
            .sour = 0,
            .smoothness = 80,
            .drainRate = 4,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_ICE,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 180,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Ganlon,
        .berryPal = gBerryPalette_Ganlon,
        .berryTreePicTable = gPicTable_HondewBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Hondew,
    },

    [BERRY_ID_SALAC] =
    {
        .info = {
            .name = _("캄라"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 95,
            .maxYield = YIELD_RATE(6, 5, 10, 13),
            .minYield = YIELD_RATE(5, 1, 1, 2),
            .description1 = COMPOUND_STRING("수수께끼의 캄라열매에는 하늘의 힘이"),
            .description2 = COMPOUND_STRING("깃들었다는 소문이 있다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 0,
            .dry = 0,
            .sweet = 40,
            .bitter = 0,
            .sour = 40,
            .smoothness = 80,
            .drainRate = 4,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_FIGHTING,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 180,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Salac,
        .berryPal =  gBerryPalette_Salac,
        .berryTreePicTable = gPicTable_AguavBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Aguav,
    },

    [BERRY_ID_PETAYA] =
    {
        .info = {
            .name = _("야타비"),
            .firmness = BERRY_FIRMNESS_VERY_HARD,
            .color = BERRY_COLOR_PINK,
            .size = 237,
            .maxYield = YIELD_RATE(6, 5, 10, 13),
            .minYield = YIELD_RATE(5, 1, 1, 2),
            .description1 = COMPOUND_STRING("수수께끼의 야타비열매에는 모든 생명체의 힘이"),
            .description2 = COMPOUND_STRING("깃들었다고 전해진다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 40,
            .dry = 0,
            .sweet = 0,
            .bitter = 40,
            .sour = 0,
            .smoothness = 80,
            .drainRate = 4,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_POISON,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 180,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Petaya,
        .berryPal = gBerryPalette_Petaya,
        .berryTreePicTable = gPicTable_PomegBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Pomeg,
    },

    [BERRY_ID_APICOT] =
    {
        .info = {
            .name = _("규살"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = BERRY_COLOR_BLUE,
            .size = 75,
            .maxYield = YIELD_RATE(6, 5, 10, 13),
            .minYield = YIELD_RATE(5, 1, 1, 2),
            .description1 = COMPOUND_STRING("이상하고 이상한 나무열매다."),
            .description2 = COMPOUND_STRING("무슨 일이 생길지는 알 수 없다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 0,
            .dry = 40,
            .sweet = 0,
            .bitter = 0,
            .sour = 40,
            .smoothness = 80,
            .drainRate = 4,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_GROUND,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 180,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Apicot,
        .berryPal = gBerryPalette_Apicot,
        .berryTreePicTable = gPicTable_GrepaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Grepa,
    },

    [BERRY_ID_LANSAT] =
    {
        .info = {
            .name = _("랑사"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_RED : BERRY_COLOR_PINK,
            .size = 97,
            .maxYield = YIELD_RATE(6, 5, 5, 7),
            .minYield = YIELD_RATE(5, 1, 1, 1),
            .description1 = COMPOUND_STRING("전설의 열매라고 전해진다."),
            .description2 = COMPOUND_STRING("가지고 있으면 매우 행복해진다고 한다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 120, 48, 72),
            .spicy = 10,
            .dry = 10,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 30,
            .drainRate = 4,
            .waterBonus = 1,
            .weedsBonus = 0,
            .pestsBonus = 1,
        },
        .naturalGiftType = TYPE_FLYING,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 200,
        .berryCrushPowder = 750,
        .berryPic = gBerryPic_Lansat,
        .berryPal = gBerryPalette_Lansat,
        .berryTreePicTable = gPicTable_LansatBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Lansat,
    },

    [BERRY_ID_STARF] =
    {
        .info = {
            .name = _("스타"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_GREEN,
            .size = 153,
            .maxYield = YIELD_RATE(6, 5, 5, 7),
            .minYield = YIELD_RATE(5, 1, 1, 1),
            .description1 = COMPOUND_STRING("너무나도 힘이 강해서"),
            .description2 = COMPOUND_STRING("세상 끝에 버려진 환상의 열매다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 120, 48, 72),
            .spicy = 10,
            .dry = 10,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 30,
            .drainRate = 4,
            .waterBonus = 1,
            .weedsBonus = 0,
            .pestsBonus = 1,
        },
        .naturalGiftType = TYPE_PSYCHIC,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 200,
        .berryCrushPowder = 750,
        .berryPic = gBerryPic_Starf,
        .berryPal =  gBerryPalette_Starf,
        .berryTreePicTable = gPicTable_CornnBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Cornn,
    },

    [BERRY_ID_ENIGMA] =
    {
        .info = {
            .name = _("의문"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PURPLE : BERRY_COLOR_YELLOW,
            .size = 155,
            .maxYield = YIELD_RATE(6, 5, 5, 13),
            .minYield = YIELD_RATE(5, 1, 1, 1),
            .description1 = COMPOUND_STRING("정체불명의 나무열매다."),
            .description2 = COMPOUND_STRING("밤하늘에 뜬 별의 힘을 지닌 듯하다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 72, 48, 72),
            .spicy = 40,
            .dry = 10,
            .sweet = 0,
            .bitter = 0,
            .sour = 0,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 0,
        },
        .naturalGiftType = TYPE_BUG,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 150,
        .berryCrushPowder = 200,
        .berryPic = gBerryPic_Enigma,
        .berryPal = gBerryPalette_Enigma,
        .berryTreePicTable = gPicTable_DurinBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Durin,
    },

    [BERRY_ID_MICLE] =
    {
        .info = {
            .name = _("미클"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_GREEN,
            .size = 41,
            .maxYield = YIELD_RATE(2, 5, 5, 13),
            .minYield = YIELD_RATE(1, 1, 1, 1),
            .description1 = COMPOUND_STRING("함께 먹은 것을 달게 느끼게 한다."),
            .description2 = COMPOUND_STRING("이 열매 자체는 매우 떫다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 72, 48, 72),
            .spicy = 0,
            .dry = 40,
            .sweet = 10,
            .bitter = 0,
            .sour = 0,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 0,
        },
        .naturalGiftType = TYPE_ROCK,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Micle,
        .berryPal = gBerryPalette_Micle,
        .berryTreePicTable = gPicTable_MicleBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Micle,
    },

    [BERRY_ID_CUSTAP] =
    {
        .info = {
            .name = _("애슈"),
            .firmness = BERRY_FIRMNESS_SUPER_HARD,
            .color = BERRY_COLOR_RED,
            .size = 267,
            .maxYield = YIELD_RATE(2, 5, 5, 13),
            .minYield = YIELD_RATE(1, 1, 1, 1),
            .description1 = COMPOUND_STRING("표면은 단단한 껍질에 둘러싸여 있지만"),
            .description2 = COMPOUND_STRING("속은 달고 부드러운 크림 같다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 72, 48, 72),
            .spicy = 0,
            .dry = 0,
            .sweet = 40,
            .bitter = 10,
            .sour = 0,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 0,
        },
        .naturalGiftType = TYPE_GHOST,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 200,
        .berryCrushPowder = 750,
        .berryPic = gBerryPic_Custap,
        .berryPal = gBerryPalette_Custap,
        .berryTreePicTable = gPicTable_CustapBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Custap,
    },

    [BERRY_ID_JABOCA] =
    {
        .info = {
            .name = _("자보"),
            .firmness = BERRY_FIRMNESS_SOFT,
            .color = BERRY_COLOR_YELLOW,
            .size = 33,
            .maxYield = YIELD_RATE(2, 5, 5, 13),
            .minYield = YIELD_RATE(1, 1, 1, 1),
            .description1 = COMPOUND_STRING("작은 낱알이 모여 있는 나무열매다."),
            .description2 = COMPOUND_STRING("거칠게 다루면 튀며 리드미컬한 소리가 난다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 72, 48, 72),
            .spicy = 0,
            .dry = 0,
            .sweet = 0,
            .bitter = 40,
            .sour = 10,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 0,
        },
        .naturalGiftType = TYPE_DRAGON,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Jaboca,
        .berryPal = gBerryPalette_Jaboca,
        .berryTreePicTable = gPicTable_JabocaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Jaboca,
    },

    [BERRY_ID_ROWAP] =
    {
        .info = {
            .name = _("애터"),
            .firmness = BERRY_FIRMNESS_VERY_SOFT,
            .color = BERRY_COLOR_BLUE,
            .size = 52,
            .maxYield = YIELD_RATE(2, 5, 5, 13),
            .minYield = YIELD_RATE(1, 1, 1, 1),
            .description1 = COMPOUND_STRING("열매를 떼면 하나하나가 팽이 같은 모양이라"),
            .description2 = COMPOUND_STRING("옛날에는 이것을 가지고 놀았다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 72, 48, 72),
            .spicy = 10,
            .dry = 0,
            .sweet = 0,
            .bitter = 0,
            .sour = 40,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 0,
        },
        .naturalGiftType = TYPE_DARK,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 130,
        .berryCrushPowder = 250,
        .berryPic = gBerryPic_Rowap,
        .berryPal = gBerryPalette_Rowap,
        .berryTreePicTable = gPicTable_RowapBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Rowap,
    },

    [BERRY_ID_ROSELI] =
    {
        .info = {
            .name = _("로셀"),
            .firmness = BERRY_FIRMNESS_HARD,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_PINK : BERRY_COLOR_RED,
            .size = 35,
            .maxYield = YIELD_RATE(5, 5, 20, 10),
            .minYield = YIELD_RATE(2, 1, 3, 2),
            .description1 = COMPOUND_STRING("진한 단맛 속에서 느껴지는 약간의 쓴맛은"),
            .description2 = COMPOUND_STRING("식재료에 포인트를 주기에 적절하다."),
            .growthDuration = GROWTH_DURATION(84, 72, 108, 48, 32, 48),
            .spicy = 0,
            .dry = 0,
            .sweet = 25,
            .bitter = 10,
            .sour = 0,
            .smoothness = 35,
            .drainRate = 6,
            .waterBonus = 10,
            .weedsBonus = 1,
            .pestsBonus = 4,
        },
        .naturalGiftType = TYPE_FAIRY,
        .naturalGiftPower = 80,
        .berryCrushDifficulty =  60,
        .berryCrushPowder =  50,
        .berryPic = gBerryPic_Roseli,
        .berryPal = gBerryPalette_Roseli,
        .berryTreePicTable = gPicTable_RoseliBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Roseli,
    },

    [BERRY_ID_KEE] =
    {
        .info = {
            .name = _("악키"),
            .firmness = BERRY_FIRMNESS_UNKNOWN,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_YELLOW : BERRY_COLOR_PINK,
            .size = 0,
            .maxYield = YIELD_RATE(2, 5, 10, 13),
            .minYield = YIELD_RATE(1, 1, 1, 2),
            .description1 = COMPOUND_STRING("지옥과도 같은 매운맛에 이어서"),
            .description2 = COMPOUND_STRING("강렬한 떫은맛이 몰아친다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 30,
            .dry = 30,
            .sweet = 10,
            .bitter = 10,
            .sour = 10,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_FAIRY,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Kee,
        .berryPal = gBerryPalette_Kee,
        .berryTreePicTable = gPicTable_PechaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Pecha,
    },

    [BERRY_ID_MARANGA] =
    {
        .info = {
            .name = _("타라프"), // "Maranga" is too long
            .firmness = BERRY_FIRMNESS_UNKNOWN,
            .color = OW_BERRY_COLORS == GEN_6_XY ? BERRY_COLOR_BLUE : BERRY_COLOR_YELLOW,
            .size = 0,
            .maxYield = YIELD_RATE(2, 5, 10, 13),
            .minYield = YIELD_RATE(1, 1, 1, 2),
            .description1 = COMPOUND_STRING("겉 부분은 매우 쓰지만"),
            .description2 = COMPOUND_STRING("속은 달아서 쥬스 같다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 96, 48, 72),
            .spicy = 10,
            .dry = 10,
            .sweet = 30,
            .bitter = 30,
            .sour = 10,
            .smoothness = 60,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 2,
        },
        .naturalGiftType = TYPE_DARK,
        .naturalGiftPower = 100,
        .berryCrushDifficulty = 160,
        .berryCrushPowder = 500,
        .berryPic = gBerryPic_Maranga,
        .berryPal = gBerryPalette_Maranga,
        .berryTreePicTable = gPicTable_OccaBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Occa,
    },

    [BERRY_ID_ENGIMA_E_READER] =
    {
        .info = {
            .name = _("의문"),
            .firmness = BERRY_FIRMNESS_UNKNOWN,
            .color = BERRY_COLOR_PURPLE,
            .size = 0,
            .maxYield = YIELD_RATE(2, 5, 5, 13),
            .minYield = YIELD_RATE(1, 1, 1, 1),
            .description1 = COMPOUND_STRING("정체불명의 나무열매다."),
            .description2 = COMPOUND_STRING("밤하늘에 뜬 별의 힘을 지닌 듯하다."),
            .growthDuration = GROWTH_DURATION(96, 96, 144, 72, 48, 72),
            .spicy = 40,
            .dry = 40,
            .sweet = 40,
            .bitter = 40,
            .sour = 40,
            .smoothness = 40,
            .drainRate = 7,
            .waterBonus = 2,
            .weedsBonus = 0,
            .pestsBonus = 0,
        },
        .berryCrushDifficulty = 150,
        .berryCrushPowder = 200,
        .berryPic = gBerryPic_Enigma,
        .berryPal = gBerryPalette_Enigma,
        .berryTreePicTable = gPicTable_DurinBerryTree,
        .berryTreePaletteSlotTable = gBerryTreePaletteSlotTable_Durin,
    },
};

const struct BerryTree gBlankBerryTree = {};

void SetEnigmaBerry(u8 *src)
{
#if FREE_ENIGMA_BERRY == FALSE
    u32 i;
    u8 *dest = (u8 *)&gSaveBlock1Ptr->enigmaBerry;

    for (i = 0; i < sizeof(gSaveBlock1Ptr->enigmaBerry); i++)
        dest[i] = src[i];
#endif //FREE_ENIGMA_BERRY
}

#if FREE_ENIGMA_BERRY == FALSE
static u32 GetEnigmaBerryChecksum(struct EnigmaBerry *enigmaBerry)
{
    u32 i;
    u32 checksum;
    u8 *dest;

    dest = (u8 *)enigmaBerry;
    checksum = 0;
    for (i = 0; i < sizeof(gSaveBlock1Ptr->enigmaBerry) - sizeof(gSaveBlock1Ptr->enigmaBerry.checksum); i++)
        checksum += dest[i];

    return checksum;
}
#endif //FREE_ENIGMA_BERRY

bool32 IsEnigmaBerryValid(void)
{
#if FREE_ENIGMA_BERRY == FALSE
    if (!gSaveBlock1Ptr->enigmaBerry.berry.growthDuration)
        return FALSE;
    if (!gSaveBlock1Ptr->enigmaBerry.berry.maxYield)
        return FALSE;
    if (GetEnigmaBerryChecksum(&gSaveBlock1Ptr->enigmaBerry) != gSaveBlock1Ptr->enigmaBerry.checksum)
        return FALSE;
    return TRUE;
#else
    return FALSE;
#endif //FREE_ENIGMA_BERRY
}

const struct BerryInfo *GetBerryInfo(enum BerryId berry)
{
    if (berry == BERRY_ID_ENGIMA_E_READER && IsEnigmaBerryValid())
    {
    #if FREE_ENIGMA_BERRY == FALSE
        return (struct BerryInfo *)(&gSaveBlock1Ptr->enigmaBerry.berry);
    #else
        return &gBerries[BERRY_ID_NONE].info;    //never reached, but will appease the compiler gods
    #endif //FREE_ENIGMA_BERRY
    }
    else
    {
        if (berry == BERRY_ID_NONE || berry > NUM_BERRIES)
            berry = 1;
        return &gBerries[berry].info;
    }
}

struct BerryTree *GetBerryTreeInfo(u8 id)
{
    return &gSaveBlock1Ptr->berryTrees[id];
}

bool32 ObjectEventInteractionWaterBerryTree(void)
{
    struct BerryTree *tree = GetBerryTreeInfo(GetObjectEventBerryTreeId(gSelectedObjectEvent));

    if (OW_BERRY_MOISTURE)
    {
        tree->moistureLevel = 100;
        if (OW_BERRY_ALWAYS_WATERABLE)
        {
            return TRUE;
        }
        AddTreeBonus(tree, GetWaterBonusByBerryType(tree->berry));
    }

    switch (tree->stage)
    {
    case BERRY_STAGE_PLANTED:
        tree->watered |= (1 << 0);
        break;
    case BERRY_STAGE_SPROUTED:
        tree->watered |= (1 << 1);
        break;
    case BERRY_STAGE_TALLER:
    case BERRY_STAGE_TRUNK:
    case BERRY_STAGE_BUDDING:
        tree->watered |= (1 << 2);
        break;
    case BERRY_STAGE_FLOWERING:
        tree->watered |= (1 << 3);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

bool8 IsPlayerFacingEmptyBerryTreePatch(void)
{
    if (GetObjectEventScriptPointerPlayerFacing() == BerryTreeScript
     && GetStageByBerryTreeId(GetObjectEventBerryTreeId(gSelectedObjectEvent)) == BERRY_STAGE_NO_BERRY)
        return TRUE;
    else
        return FALSE;
}

bool8 TryToWaterBerryTree(void)
{
    if (GetObjectEventScriptPointerPlayerFacing() != BerryTreeScript)
        return FALSE;
    else
        return ObjectEventInteractionWaterBerryTree();
}

void ClearBerryTrees(void)
{
    int i;

    for (i = 0; i < BERRY_TREES_COUNT; i++)
        gSaveBlock1Ptr->berryTrees[i] = gBlankBerryTree;
}

bool32 BerryTreeGrow(struct BerryTree *tree)
{
    if (tree->stopGrowth)
        return FALSE;

    switch (tree->stage)
    {
    case BERRY_STAGE_NO_BERRY:
        return FALSE;
#if IS_HNS
    case BERRY_STAGE_PLANTED:
    case BERRY_STAGE_SPROUTED:
    case BERRY_STAGE_TALLER:
    case BERRY_STAGE_FLOWERING:
        tree->berryYield = CalcBerryYield(tree);
        tree->stage = BERRY_STAGE_BERRIES;
        break;
    case BERRY_STAGE_BERRIES:
        break;
#else
    case BERRY_STAGE_FLOWERING:
        tree->berryYield = CalcBerryYield(tree);
    case BERRY_STAGE_PLANTED:
    case BERRY_STAGE_SPROUTED:
    case BERRY_STAGE_TRUNK:
        tree->stage++;
        break;
    case BERRY_STAGE_TALLER:
        if (OW_BERRY_SIX_STAGES)
            tree->stage = BERRY_STAGE_TRUNK;
        else
            tree->stage++;
        break;
    case BERRY_STAGE_BUDDING:
        tree->berryYield = CalcBerryYield(tree);
        tree->stage = BERRY_STAGE_BERRIES;
        break;
    case BERRY_STAGE_BERRIES:
        if (OW_BERRY_IMMORTAL)
            break;
        tree->watered = 0;
        tree->berryYield = 0;
        tree->stage = BERRY_STAGE_SPROUTED;
        tree->moistureLevel = 100;
        if (++tree->regrowthCount == ((tree->mulch == ITEM_TO_MULCH(ITEM_GOOEY_MULCH)) ? 15 : 10))
            *tree = gBlankBerryTree;
        break;
#endif
    }
    return TRUE;
}

static u16 GetMulchAffectedGrowthRate(u16 berryDuration, u8 mulch, u8 stage)
{
    if (stage == BERRY_STAGE_BERRIES)
        return berryDuration;
    if (mulch == ITEM_TO_MULCH(ITEM_GROWTH_MULCH))
        return berryDuration / 4 * 3;
    if (mulch == ITEM_TO_MULCH(ITEM_DAMP_MULCH))
        return berryDuration / 2 * 3;
    return berryDuration;
}

void BerryTreeTimeUpdate(s32 minutes)
{
    int i;
    struct BerryTree *tree;

    for (i = 0; i < BERRY_TREES_COUNT; i++)
    {
        tree = &gSaveBlock1Ptr->berryTrees[i];

#if IS_HNS
        if (tree->berry && tree->stage && !tree->stopGrowth && tree->stage != BERRY_STAGE_BERRIES)
        {
            s32 time = minutes;

            while (time != 0)
            {
                if (tree->minutesUntilNextStage > time)
                {
                    tree->minutesUntilNextStage -= time;
                    break;
                }
                time -= tree->minutesUntilNextStage;
                tree->minutesUntilNextStage = GetStageDurationByBerryType(tree->berry);
                if (!BerryTreeGrow(tree))
                    break;
                if (tree->stage == BERRY_STAGE_BERRIES)
                    tree->minutesUntilNextStage *= 4;
            }
        }
#else
        {
        u32 drainVal;

        if (tree->berry && tree->stage && !tree->stopGrowth && (!OW_BERRY_IMMORTAL || tree->stage != BERRY_STAGE_BERRIES))
        {
            if ((!OW_BERRY_IMMORTAL) && (minutes >= GetStageDurationByBerryType(tree->berry) * 71))
            {
                *tree = gBlankBerryTree;
            }
            else
            {
                s32 time = minutes;

                // Check moisture gradient, pests and weeds
                while (time > 0 && tree->stage != BERRY_STAGE_BERRIES)
                {
                    tree->moistureClock += 1;
                    time -= 1;
                    if (tree->moistureClock % 60 == 0)
                    {
                        if (OW_BERRY_MOISTURE)
                        {
                            drainVal = (OW_BERRY_DRAIN_RATE == GEN_4) ? GetDrainRateByBerryType(tree->berry) : (OW_BERRY_DRAIN_RATE == GEN_6_XY) ? 4 : 25;
                            if (OW_BERRY_MULCH_USAGE)
                            {
                                if (tree->mulch == ITEM_TO_MULCH(ITEM_GROWTH_MULCH))
                                    drainVal *= 2;
                                if (tree->mulch == ITEM_TO_MULCH(ITEM_DAMP_MULCH))
                                    drainVal /= 2;
                                if (tree->mulch == ITEM_TO_MULCH(ITEM_BOOST_MULCH) || tree->mulch == ITEM_TO_MULCH(ITEM_AMAZE_MULCH))
                                    drainVal = 25;
                            }
                            if (OW_BERRY_ALWAYS_WATERABLE && tree->moistureLevel == 0)
                            {
                                if (tree->berryYield > GetBerryInfo(tree->berry)->minYield + GetBerryInfo(tree->berry)->maxYield / 5)
                                    tree->berryYield -= GetBerryInfo(tree->berry)->maxYield / 5;
                                else
                                    tree->berryYield = GetBerryInfo(tree->berry)->minYield;
                            }
                            else if (tree->moistureLevel <= drainVal)
                                tree->moistureLevel = 0;
                            else
                                tree->moistureLevel -= drainVal;
                            if (OW_BERRY_DRAIN_RATE == GEN_6_XY && tree->moistureLevel <= 4) // Without variable drain rate (and without mulches), this needs to trigger after 24 hours, hence the extra check
                                tree->moistureLevel = 0;
                        }
                        if (tree->moistureClock == 120)
                        {
                            TryForWeeds(tree);
                            TryForPests(tree);
                            tree->moistureClock = 0;
                        }
                    }
                }

                // Check Berry growth
                time = minutes;

                while (time != 0)
                {
                    if (tree->minutesUntilNextStage > time)
                    {
                        tree->minutesUntilNextStage -= time;
                        break;
                    }
                    time -= tree->minutesUntilNextStage;
                    tree->minutesUntilNextStage = GetMulchAffectedGrowthRate(GetStageDurationByBerryType(tree->berry), tree->mulch, tree->stage);
                    if (!BerryTreeGrow(tree))
                        break;
                    if (tree->stage == BERRY_STAGE_BERRIES)
                        tree->minutesUntilNextStage = GetStageDurationByBerryType(tree->berry) * ((tree->mulch == ITEM_TO_MULCH(ITEM_STABLE_MULCH)) ? 6 : 4);
                }
            }
        }
        }
#endif
    }
}

void PlantBerryTree(u8 id, enum BerryId berry, u8 stage, bool8 allowGrowth)
{
    struct BerryTree *tree = GetBerryTreeInfo(id);

    tree->berry = berry;
    tree->stage = stage;
#if IS_HNS
    tree->minutesUntilNextStage = GetStageDurationByBerryType(berry);
    if (stage == BERRY_STAGE_BERRIES)
    {
        tree->berryYield = CalcBerryYield(tree);
        tree->minutesUntilNextStage *= 4;
    }
#else
    tree->minutesUntilNextStage = GetMulchAffectedGrowthRate(GetStageDurationByBerryType(berry), tree->mulch, stage);
    tree->moistureLevel = 100;
    if (OW_BERRY_ALWAYS_WATERABLE)
    {
        u32 berryTreeAge = GetBerryTreeAge(berry, stage);
        if (GetBerryInfo(berry)->maxYield - berryTreeAge * GetBerryInfo(berry)->maxYield / 5 < GetBerryInfo(berry)->minYield)
            tree->berryYield = GetBerryInfo(berry)->minYield;
        else
            tree->berryYield = GetBerryInfo(berry)->maxYield - berryTreeAge * GetBerryInfo(berry)->maxYield / 5;
    }
    else if (stage == BERRY_STAGE_BERRIES)
    {
        tree->berryYield = CalcBerryYield(tree);
        tree->minutesUntilNextStage *= ((tree->mulch == ITEM_TO_MULCH(ITEM_STABLE_MULCH)) ? 6 : 4);
    }
    SetTreeMutations(id, berry);
#endif

    if (!allowGrowth)
        tree->stopGrowth = TRUE;
}

void RemoveBerryTree(u8 id)
{
    gSaveBlock1Ptr->berryTrees[id] = gBlankBerryTree;
}

u8 GetBerryTypeByBerryTreeId(u8 id)
{
    return gSaveBlock1Ptr->berryTrees[id].berry;
}

u8 GetStageByBerryTreeId(u8 id)
{
    return gSaveBlock1Ptr->berryTrees[id].stage;
}

u8 GetMulchByBerryTreeId(u8 id)
{
    return gSaveBlock1Ptr->berryTrees[id].mulch;
}

void GetBerryNameByBerryType(u8 berry, u8 *string)
{
    memcpy(string, GetBerryInfo(berry)->name, BERRY_NAME_LENGTH);
    string[BERRY_NAME_LENGTH] = EOS;
}

void AllowBerryTreeGrowth(u8 id)
{
    GetBerryTreeInfo(id)->stopGrowth = FALSE;
}

static u8 BerryTreeGetNumStagesWatered(struct BerryTree *tree)
{
    u8 count = 0;

    if (tree->watered & (1 << 0))
        count++;
    if (tree->watered & (1 << 1))
        count++;
    if (tree->watered & (1 << 2))
        count++;
    if (tree->watered & (1 << 3))
        count++;
    return count;
}

static u8 GetNumStagesWateredByBerryTreeId(u8 id)
{
    return BerryTreeGetNumStagesWatered(GetBerryTreeInfo(id));
}

// Berries can be watered at 4 stages of growth. The distribution is largely
// even but slightly prefers middle berry yields, since it uniformly draws from
// a subset of the total yield range.
static u8 CalcBerryYieldInternal(u16 max, u16 min, u8 water)
{
    u32 randMin;
    u32 randMax;
    u32 rand;
    u32 extraYield;

    if (water == 0 || OW_BERRY_MOISTURE)
    {
        return min;
    }
    else
    {
        randMin = (max - min) * (water - 1);
        randMax = (max - min) * (water);
        rand = randMin + Random() % (randMax - randMin + 1);

        // Round upwards
        if ((rand % NUM_WATER_STAGES) >= NUM_WATER_STAGES / 2)
            extraYield = rand / NUM_WATER_STAGES + 1;
        else
            extraYield = rand / NUM_WATER_STAGES;
        return extraYield + min;
    }
}

static u8 CalcBerryYield(struct BerryTree *tree)
{
    const struct BerryInfo *berryInfo = GetBerryInfo(tree->berry);
    u8 min = tree->berryYield;
    u8 max = berryInfo->maxYield;
    u8 result;
    if (OW_BERRY_MULCH_USAGE && (tree->mulch == ITEM_TO_MULCH(ITEM_RICH_MULCH) || tree->mulch == ITEM_TO_MULCH(ITEM_AMAZE_MULCH)))
        min += 2;
    if (!(OW_BERRY_MOISTURE && OW_BERRY_ALWAYS_WATERABLE))
        min += berryInfo->minYield;
    if (min >= max)
        result = max;
    else
        result = CalcBerryYieldInternal(max, min, BerryTreeGetNumStagesWatered(tree));

    return result;
}

static u32 GetBerryTreeAge(u8 id, u8 stage)
{
    if (stage == BERRY_STAGE_TRUNK)
        stage = 5;
    else if (stage == BERRY_STAGE_BUDDING)
        stage = 6;
    else if (stage > 0)
        stage -= 1;
    return GetBerryInfo(id)->growthDuration * stage / (OW_BERRY_SIX_STAGES ? 6 : 4);
}

static u8 GetBerryCountByBerryTreeId(u8 id)
{
    return gSaveBlock1Ptr->berryTrees[id].berryYield;
}

static u16 GetStageDurationByBerryType(u8 berry)
{
#if IS_HNS
    if (berry == BERRY_ID_LUM || berry == BERRY_ID_SITRUS)
        return 12 * 2;
    return 3 * 2;
#else
    return GetBerryInfo(berry)->growthDuration * 60 / (OW_BERRY_SIX_STAGES ? 6 : 4);
#endif
}

static u8 GetDrainRateByBerryType(u8 berry)
{
    return GetBerryInfo(berry)->drainRate;
}

static u8 GetWaterBonusByBerryType(u8 berry)
{
    return GetBerryInfo(berry)->waterBonus;
}

static u8 GetWeedingBonusByBerryType(u8 berry)
{
    u8 bonus = GetBerryInfo(berry)->weedsBonus;
    return (bonus == 0) ? 1 : bonus * 5;
}

static u8 GetPestsBonusByBerryType(u8 berry)
{
    u8 bonus = GetBerryInfo(berry)->pestsBonus;
    return (bonus == 0) ? 2 : bonus * 5;
}

bool8 CanWaterBerryPlot(void)
{
    if (!OW_BERRY_MOISTURE || OW_BERRY_ALWAYS_WATERABLE)
        return TRUE;
    return (gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(gSelectedObjectEvent)].moistureLevel == 0);
}

void ObjectEventInteractionGetBerryTreeData(void)
{
    u8 id;
    u8 berry;
    u8 localId;
    u8 group;
    u8 num;

    id = GetObjectEventBerryTreeId(gSelectedObjectEvent);
    berry = GetBerryTypeByBerryTreeId(id);
    AllowBerryTreeGrowth(id);
    localId = gSpecialVar_LastTalked;
    num = gSaveBlock1Ptr->location.mapNum;
    group = gSaveBlock1Ptr->location.mapGroup;
    if (IsBerryTreeSparkling(localId, num, group))
        gSpecialVar_0x8004 = BERRY_STAGE_SPARKLING;
    else
        gSpecialVar_0x8004 = GetStageByBerryTreeId(id);
    gSpecialVar_0x8005 = GetNumStagesWateredByBerryTreeId(id);
    gSpecialVar_0x8006 = GetBerryCountByBerryTreeId(id);
    CopyItemNameHandlePlural(BerryTypeToItemId(berry), gStringVar1, gSpecialVar_0x8006);
}

void ObjectEventInteractionGetBerryName(void)
{
    u8 berryType = GetBerryTypeByBerryTreeId(GetObjectEventBerryTreeId(gSelectedObjectEvent));
    GetBerryNameByBerryType(berryType, gStringVar1);
}

void ObjectEventInteractionGetBerryCountString(void)
{
    u8 treeId = GetObjectEventBerryTreeId(gSelectedObjectEvent);
    u8 berry = GetBerryTypeByBerryTreeId(treeId);
    u8 count = GetBerryCountByBerryTreeId(treeId);

    // The strings for growing Berries all refer to a singular berry plant.
    // This ensures that text about planting a Berry and the growing Berry reads correctly.
    if (GetStageByBerryTreeId(treeId) != BERRY_STAGE_BERRIES)
        count = 1;

    gSpecialVar_0x8006 = BerryTypeToItemId(berry);
    CopyItemNameHandlePlural(gSpecialVar_0x8006, gStringVar1, count);
    berry = GetTreeMutationValue(treeId);
    if (berry > 0)
    {
        count = 1;
        CopyItemNameHandlePlural(BerryTypeToItemId(berry), gStringVar3, count);
        gSpecialVar_Result = TRUE;
    }
    else
        gSpecialVar_Result = FALSE;
}

void Bag_ChooseBerry(void)
{
    SetMainCallback2(CB2_ChooseBerry);
}

void Bag_ChooseMulch(void)
{
    SetMainCallback2(CB2_ChooseMulch);
}

void ObjectEventInteractionPlantBerryTree(void)
{
#if IS_HNS
    u8 berry = sLastPickedBerryType;
    if (berry == BERRY_NONE)
        berry = BERRY_ID_CHERI;
    PlantBerryTree(GetObjectEventBerryTreeId(gSelectedObjectEvent), berry, BERRY_STAGE_SPROUTED, TRUE);
#else
    PlantBerryTree(GetObjectEventBerryTreeId(gSelectedObjectEvent), ItemIdToBerryType(gSpecialVar_ItemId), BERRY_STAGE_PLANTED, TRUE);
#endif
    ObjectEventInteractionGetBerryTreeData();
}

void ObjectEventInteractionApplyMulch(void)
{
    u8 mulch = ITEM_TO_MULCH(gSpecialVar_ItemId);

    gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(gSelectedObjectEvent)].mulch = mulch;
    StringExpandPlaceholders(gStringVar1, gItemsInfo[gSpecialVar_ItemId].name);
}

void ObjectEventInteractionPickBerryTree(void)
{
    u8 id = GetObjectEventBerryTreeId(gSelectedObjectEvent);
    u8 berry = GetBerryTypeByBerryTreeId(id);

#if IS_HNS
    gSpecialVar_0x8004 = AddBagItem(BerryTypeToItemId(berry), GetBerryCountByBerryTreeId(id));
    sLastPickedBerryType = berry;
#else
    u8 mutation = GetTreeMutationValue(id);

    if (!OW_BERRY_MUTATIONS || mutation == 0)
    {
        gSpecialVar_0x8004 = AddBagItem(BerryTypeToItemId(berry), GetBerryCountByBerryTreeId(id));
        return;
    }
    gSpecialVar_0x8004 = (CheckBagHasSpace(BerryTypeToItemId(berry), GetBerryCountByBerryTreeId(id)) && CheckBagHasSpace(BerryTypeToItemId(mutation), 1)) + 2;
    if (gSpecialVar_0x8004 == 3)
    {
        AddBagItem(BerryTypeToItemId(berry), GetBerryCountByBerryTreeId(id));
        AddBagItem(BerryTypeToItemId(mutation), 1);
    }
#endif
}

void ObjectEventInteractionRemoveBerryTree(void)
{
    RemoveBerryTree(GetObjectEventBerryTreeId(gSelectedObjectEvent));
    SetBerryTreeJustPicked(gSpecialVar_LastTalked, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
}

void ObjectEventInteractionPullBerryWeed(void)
{
    struct BerryTree *tree = GetBerryTreeInfo(GetObjectEventBerryTreeId(gSelectedObjectEvent));
    tree->weeds = FALSE;
    AddTreeBonus(tree, GetWeedingBonusByBerryType(tree->berry));
}

void ObjectEventInteractionClearBerryPests(void)
{
    struct BerryTree *tree = GetBerryTreeInfo(GetObjectEventBerryTreeId(gSelectedObjectEvent));
    tree->pests = FALSE;
    AddTreeBonus(tree, GetPestsBonusByBerryType(tree->berry));
}

bool8 PlayerHasBerries(void)
{
    return IsBagPocketNonEmpty(POCKET_BERRIES);
}

bool8 ObjectEventInteractionBerryHasWeed(void)
{
    return gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(gSelectedObjectEvent)].weeds;
}

bool8 ObjectEventInteractionBerryHasPests(void)
{
    enum Species species;
    if (!OW_BERRY_PESTS || !gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(gSelectedObjectEvent)].pests)
        return FALSE;
    species = GetBerryPestSpecies(gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(gSelectedObjectEvent)].berry);
    if (species == SPECIES_NONE)
        return FALSE;
    CreateScriptedWildMon(species, 14 + Random() % 3, ITEM_NONE);
    gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(gSelectedObjectEvent)].pests = FALSE;
    return TRUE;
}

// Berry tree growth is frozen at their initial stage (usually, fully grown) until the player has seen the tree
// For all berry trees on screen, allow normal growth
void SetBerryTreesSeen(void)
{
    u16 cam_left;
    u16 cam_top;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    int i;

    GetCameraCoords(&cam_left, &cam_top);
    left = cam_left;
    top = cam_top + 3;
    right = cam_left + 14;
    bottom = top + 8;
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].active && gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            s16 x = gObjectEvents[i].currentCoords.x;
            s16 y = gObjectEvents[i].currentCoords.y;
            if (left <= x && x <= right && top <= y && y <= bottom)
                AllowBerryTreeGrowth(gObjectEvents[i].trainerRange_berryTreeId);
        }
    }
}

bool8 PlayerHasMulch(void)
{
    if (!OW_BERRY_MULCH_USAGE)
        return FALSE;
    if (CheckBagHasItem(ITEM_GROWTH_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_DAMP_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_STABLE_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_GOOEY_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_RICH_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_SURPRISE_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_BOOST_MULCH, 1))
        return TRUE;
    if (CheckBagHasItem(ITEM_AMAZE_MULCH, 1))
        return TRUE;
    return FALSE;
}

#if OW_BERRY_MUTATIONS == TRUE
static const u8 sBerryMutations[][3] = {
    {BERRY_ID_IAPAPA, BERRY_ID_MAGO,   BERRY_ID_POMEG},
    {BERRY_ID_CHESTO, BERRY_ID_PERSIM, BERRY_ID_KELPSY},
    {BERRY_ID_ORAN,   BERRY_ID_PECHA,  BERRY_ID_QUALOT},
    {BERRY_ID_CHESTO, BERRY_ID_PERSIM, BERRY_ID_KELPSY},
    {BERRY_ID_ASPEAR, BERRY_ID_LEPPA,  BERRY_ID_HONDEW},
    {BERRY_ID_AGUAV,  BERRY_ID_FIGY,   BERRY_ID_GREPA},
    {BERRY_ID_LUM,    BERRY_ID_SITRUS, BERRY_ID_TAMATO},
    {BERRY_ID_HONDEW, BERRY_ID_YACHE,  BERRY_ID_LIECHI},
    {BERRY_ID_QUALOT, BERRY_ID_TANGA,  BERRY_ID_GANLON},
    {BERRY_ID_GREPA,  BERRY_ID_ROSELI, BERRY_ID_SALAC},
    {BERRY_ID_POMEG,  BERRY_ID_KASIB,  BERRY_ID_PETAYA},
    {BERRY_ID_KELPSY, BERRY_ID_WACAN,  BERRY_ID_APICOT},
    {BERRY_ID_GANLON, BERRY_ID_LIECHI, BERRY_ID_KEE},
    {BERRY_ID_SALAC,  BERRY_ID_PETAYA, BERRY_ID_MARANGA},
    // Up to one more Mutation can be added here for a total of 15 (only 4 bits are allocated)
};

static u8 GetMutationOutcome(u8 berry1, u8 berry2)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(sBerryMutations); i++)
    {
        if ((sBerryMutations[i][0] == berry1 && sBerryMutations[i][1] == berry2)
          ||(sBerryMutations[i][0] == berry2 && sBerryMutations[i][1] == berry1))
            return (i + 1);
    }
    return 0;
}

static u8 TryForMutation(u8 berryTreeId, u8 berry)
{
    u8 i, j, mulch;
    s16 x1, x2, y1, y2;

    // Get location of current tree
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].trainerRange_berryTreeId == berryTreeId && gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
            break;
    }
    if (i == OBJECT_EVENTS_COUNT)
        return 0;

    x1 = gObjectEvents[i].currentCoords.x;
    y1 = gObjectEvents[i].currentCoords.y;

    mulch = GetMulchByBerryTreeId(GetObjectEventBerryTreeId(i));

    // Try mutation for each adjacent tree
    for (j = 0; j < OBJECT_EVENTS_COUNT; j++)
    {
        if (gObjectEvents[j].active && gObjectEvents[j].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH && GetStageByBerryTreeId(GetObjectEventBerryTreeId(j)) != BERRY_STAGE_NO_BERRY && j != i)
        {
            x2 = gObjectEvents[j].currentCoords.x;
            y2 = gObjectEvents[j].currentCoords.y;
            u32 rate = OW_BERRY_MUTATION_CHANCE;

            if (mulch == ITEM_TO_MULCH(ITEM_SURPRISE_MULCH) || mulch == ITEM_TO_MULCH(ITEM_AMAZE_MULCH))
                rate *= 2;

            if (Random() % 100 < rate && (
                (x1 == x2 && y1 == y2 - 1) ||
                (x1 == x2 && y1 == y2 + 1) ||
                (x1 == x2 - 1 && y1 == y2) ||
                (x1 == x2 + 1 && y1 == y2)))
                return GetMutationOutcome(berry, gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(j)].berry);
        }
    }
    return 0;
}
#endif

struct TreeMutationBitfield {
  u8 a: 2;
  u8 b: 2;
  u8 unused: 4;
};

union TreeMutation {
  u8 value;
  struct TreeMutationBitfield asField;
};

static u8 GetTreeMutationValue(u8 id)
{
#if OW_BERRY_MUTATIONS
    struct BerryTree *tree = GetBerryTreeInfo(id);
    union TreeMutation myMutation;
    if (tree->stopGrowth) // Pre-generated trees shouldn't have mutations
        return 0;
    myMutation.asField.a = tree->mutationA;
    myMutation.asField.b = tree->mutationB;
    myMutation.asField.unused = 0;
    if (myMutation.value == 0) // no mutation
        return 0;
    return sBerryMutations[myMutation.value - 1][2];
#else
    return 0;
#endif
}

static void SetTreeMutations(u8 id, u8 berry)
{
#if OW_BERRY_MUTATIONS == TRUE
    struct BerryTree *tree = GetBerryTreeInfo(id);
    union TreeMutation myMutation;

    myMutation.value = TryForMutation(id, berry);
    tree->mutationA = myMutation.asField.a;
    tree->mutationB = myMutation.asField.b;
#endif
}

static enum Species GetBerryPestSpecies(u8 berryId)
{
#if OW_BERRY_PESTS == TRUE
    const struct BerryInfo *berryInfo = GetBerryInfo(berryId);
    switch (berryInfo->color)
    {
    case BERRY_COLOR_RED:
        return P_FAMILY_LEDYBA ? SPECIES_LEDYBA : SPECIES_NONE;
        break;
    case BERRY_COLOR_BLUE:
        return P_FAMILY_VOLBEAT_ILLUMISE ? SPECIES_VOLBEAT : SPECIES_NONE;
        break;
    case BERRY_COLOR_PURPLE:
        return P_FAMILY_VOLBEAT_ILLUMISE ? SPECIES_ILLUMISE : SPECIES_NONE;
        break;
    case BERRY_COLOR_GREEN:
        return P_FAMILY_BURMY ? SPECIES_BURMY_PLANT : SPECIES_NONE;
        break;
    case BERRY_COLOR_YELLOW:
        return P_FAMILY_COMBEE ? SPECIES_COMBEE : SPECIES_NONE;
        break;
    case BERRY_COLOR_PINK:
        return P_FAMILY_SCATTERBUG ? SPECIES_SPEWPA : SPECIES_NONE;
        break;
    }
#endif
    return SPECIES_NONE;
}

#define BERRY_WEEDS_CHANCE 15
#define BERRY_PESTS_CHANCE 15

static void TryForWeeds(struct BerryTree *tree)
{
    if (!OW_BERRY_WEEDS)
        return;
    if (tree->weeds == TRUE)
        return;
    if (Random() % 100 < BERRY_WEEDS_CHANCE)
        tree->weeds = TRUE;
}

static void TryForPests(struct BerryTree *tree)
{
    if (!OW_BERRY_WEEDS)
        return;
    if (tree->pests == TRUE)
        return;
    if (Random() % 100 < BERRY_PESTS_CHANCE && tree->stage > BERRY_STAGE_PLANTED)
        tree->pests = TRUE;
}

static void AddTreeBonus(struct BerryTree *tree, u8 bonus)
{
    if (OW_BERRY_MOISTURE) // use watered field to save track of intermediate bonuses
    {
        tree->watered += bonus;
        while (tree->watered > 10)
        {
            tree->watered -= 10;
            bonus = tree->berryYield + 1;
            if (bonus > GetBerryInfo(tree->berry)->maxYield)
                bonus = GetBerryInfo(tree->berry)->maxYield;
            tree->berryYield = bonus;
        }
    }
    else
    {
        bonus = tree->berryYield + bonus / 10;
        if (bonus > GetBerryInfo(tree->berry)->maxYield)
            bonus = GetBerryInfo(tree->berry)->maxYield;
        tree->berryYield = bonus;
    }
}

#if IS_HNS
void Berry_Ready(void)
{
    struct BerryTree *tree = GetBerryTreeInfo(GetObjectEventBerryTreeId(gSelectedObjectEvent));
    if (tree->stage != BERRY_STAGE_NO_BERRY)
    {
        tree->stage = BERRY_STAGE_FLOWERING;
        BerryTreeGrow(tree);
    }
}
#endif
