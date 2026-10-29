// HnS seq 504 #9518 회귀 테스트 — 도감 두 파일 통합 뒤 HGSS 도감이 공통 함수(울음소리 화면 이름, 성도 도감 수)를 쓴다.
// 공통 PrintCryScreenSpeciesName은 이름을 60 px에 맞춰 글꼴을 줄이는(WrapFontIdToFit) 원래 도감 형태라, 한글 이름이 모두
// 기본 글꼴 그대로인지(HGSS 이식 전과 같은 표시) 확인한다. 이름이 모두 "HnS 9518:"로 시작한다.
#include "global.h"
#include "pokedex.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"

TEST("HnS 9518: every species name keeps FONT_NORMAL on the cry screen")
{
    u32 i, widest = 0;
    for (i = 1; i < NUM_SPECIES; i++)
    {
        if (!IsSpeciesEnabled(i))
            continue;
        const u8 *name = GetSpeciesName(i);
        u32 width = GetStringWidth(FONT_NORMAL, name, 0);
        if (width > widest)
            widest = width;
        EXPECT_EQ(GetFontIdToFit(name, FONT_NORMAL, 0, 60), FONT_NORMAL);
    }
    EXPECT_LE(widest, 60);
}

TEST("HnS 9518: the regional Pokédex count is the Johto count")
{
    GetSetPokedexFlag(JohtoToNationalOrder(1), FLAG_SET_SEEN);
    GetSetPokedexFlag(JohtoToNationalOrder(1), FLAG_SET_CAUGHT);
    EXPECT_GE(GetJohtoPokedexCount(FLAG_GET_CAUGHT), 1);
    EXPECT_EQ(GetRegionalPokedexCount(FLAG_GET_SEEN), GetJohtoPokedexCount(FLAG_GET_SEEN));
    EXPECT_EQ(GetRegionalPokedexCount(FLAG_GET_CAUGHT), GetJohtoPokedexCount(FLAG_GET_CAUGHT));
}
