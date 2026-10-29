// HnS seq 507 #10334 회귀 테스트 — 콘테스트 부문별 switch 5종을 gContestCategoryInfo 표로 옮긴 뒤에도 값이 이식 전과
// 같은지(리본·그림 효과·결과 타이틀·문자열·TV 상태). 그림 효과는 HnS 보정(upstream은 .stdString을 돌려줌 — 귀여움만 값이
// 우연히 같다)이 쓰는 값.
// 부문 이름이 한글("근사함" 등)로 들어가는지도 본다. 이름이 모두 "HnS 10334:"로 시작한다.
#include "global.h"
#include "contest.h"
#include "graphics.h"
#include "image_processing_effects.h"
#include "script_menu.h"
#include "string_util.h"
#include "strings.h"
#include "test/test.h"
#include "constants/script_menu.h"
#include "constants/tv.h"

void CopyContestCategoryToStringVar(u8 varIdx, enum ContestCategories category);

struct HnsContestValues
{
    u8 ribbon;
    u8 imageEffect;
    const u16 *resultsTilemap;
    u8 stdString;
    const u8 *text;
    const u8 *korean;
    u8 tvShowState;
    u8 tvShowStateExciting;
};

// The values of the switch statements this PR removed (contest_painting.c, contest_util.c, tv.c).
static const struct HnsContestValues sOldValues[CONTEST_CATEGORIES_COUNT] =
{
    [CONTEST_CATEGORY_COOL]   = { MON_DATA_COOL_RIBBON,   IMAGE_EFFECT_OUTLINE_COLORED, gContestResultsTitle_Cool_Tilemap,   STDSTRING_COOL,   gText_Cool,   COMPOUND_STRING("근사함"),   CONTESTLIVE_STATE_COOL,      CONTESTLIVE_STATE_VERY_COOL },
    [CONTEST_CATEGORY_BEAUTY] = { MON_DATA_BEAUTY_RIBBON, IMAGE_EFFECT_SHIMMER,         gContestResultsTitle_Beauty_Tilemap, STDSTRING_BEAUTY, gText_Beauty, COMPOUND_STRING("아름다움"), CONTESTLIVE_STATE_BEAUTIFUL, CONTESTLIVE_STATE_VERY_BEAUTIFUL },
    [CONTEST_CATEGORY_CUTE]   = { MON_DATA_CUTE_RIBBON,   IMAGE_EFFECT_POINTILLISM,     gContestResultsTitle_Cute_Tilemap,   STDSTRING_CUTE,   gText_Cute,   COMPOUND_STRING("귀여움"),   CONTESTLIVE_STATE_CUTE,      CONTESTLIVE_STATE_VERY_CUTE },
    [CONTEST_CATEGORY_SMART]  = { MON_DATA_SMART_RIBBON,  IMAGE_EFFECT_CHARCOAL,        gContestResultsTitle_Smart_Tilemap,  STDSTRING_SMART,  gText_Smart,  COMPOUND_STRING("슬기로움"), CONTESTLIVE_STATE_SMART,     CONTESTLIVE_STATE_VERY_SMART },
    [CONTEST_CATEGORY_TOUGH]  = { MON_DATA_TOUGH_RIBBON,  IMAGE_EFFECT_GRAYSCALE_LIGHT, gContestResultsTitle_Tough_Tilemap,  STDSTRING_TOUGH,  gText_Tough,  COMPOUND_STRING("강인함"),   CONTESTLIVE_STATE_TOUGH,     CONTESTLIVE_STATE_VERY_TOUGH },
};

TEST("HnS 10334: contest category values match the removed switch statements")
{
    u32 category = 0;
    for (u32 i = 0; i < CONTEST_CATEGORIES_COUNT; i++)
    {
        PARAMETRIZE { category = i; }
    }
    const struct ContestCategory *info = &gContestCategoryInfo[category];
    const struct HnsContestValues *old = &sOldValues[category];
    EXPECT_EQ(info->ribbon, old->ribbon);
    EXPECT_EQ(info->imageEffect, old->imageEffect);
    EXPECT_EQ(info->resultsTilemap, old->resultsTilemap);
    EXPECT_EQ(info->stdString, old->stdString);
    EXPECT_EQ(info->text, old->text);
    EXPECT_EQ(info->tvShowState, old->tvShowState);
    EXPECT_EQ(info->tvShowStateExciting, old->tvShowStateExciting);
}

TEST("HnS 10334: the contest category string var is the Korean category name")
{
    u32 category = 0;
    for (u32 i = 0; i < CONTEST_CATEGORIES_COUNT; i++)
    {
        PARAMETRIZE { category = i; }
    }
    CopyContestCategoryToStringVar(0, category);
    EXPECT_EQ(StringCompare(gStringVar1, sOldValues[category].korean), 0);
    EXPECT_EQ(StringCompare(gContestCategoryInfo[category].text, sOldValues[category].korean), 0);
}
