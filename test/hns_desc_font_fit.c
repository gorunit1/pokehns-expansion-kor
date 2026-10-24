// HnS 설명 글꼴 폭 맞춤 회귀 테스트 — seq 444 #10521 A안, 친구 결정(2026-10-24b).
// 창보다 넓은 설명만 더 좁은 글꼴로 찍는다(GetFontIdToFit / WrapFontIdToFit). 대상 수는 현재 문자열 기준이라 고정하지 않는다.
// 이름이 모두 "HnS desc font:"으로 시작한다(TESTS='HnS desc font').
#include "global.h"
#include "move.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/abilities.h"

// HnS Korean glyphs: FONT_NARROWER is wider than FONT_NARROW, so it never helps a Korean line fit.
TEST("HnS desc font: Korean glyph widths per font")
{
    static const u8 sHangul[] = _("가");
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, sHangul, 0), 8);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sHangul, 0), 8);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sHangul, 0), 11);
}

// A Korean description that fits keeps the normal font (summary ability window, 144 px).
TEST("HnS desc font: Aura Guard's Korean description keeps FONT_NORMAL")
{
    const u8 *desc = gAbilitiesInfo[ABILITY_AURA_GUARD].description;
    EXPECT_LE(GetStringWidth(FONT_NORMAL, desc, 0), 144);
    EXPECT_EQ(GetFontIdToFit(desc, FONT_NORMAL, 0, 144), FONT_NORMAL);
}

// The Move Relearner description window (128 px, FONT_NARROW) had no upstream fit test.
TEST("HnS desc font: move descriptions fit on the Move Relearner screen")
{
    u32 i;
    const u32 widthPx = 128;
    enum Move move = MOVE_NONE;
    for (i = 1; i < MOVES_COUNT_ALL; i++)
    {
        PARAMETRIZE_LABEL("%S", GetMoveDescription(i)) { move = i; }
    }
    u32 fontId = GetFontIdToFit(GetMoveDescription(move), FONT_NARROW, 0, widthPx);
    EXPECT_LE(GetStringWidth(fontId, GetMoveDescription(move), 0), widthPx);
    if (GetStringWidth(FONT_NARROW, GetMoveDescription(move), 0) <= widthPx)
        EXPECT_EQ(fontId, FONT_NARROW);
}

// Battle move info: the font change wraps only the description, after the Korean header line,
// and the header bytes stay as they were.
TEST("HnS desc font: battle move info wraps only an overflowing description")
{
    static const u8 sHeader[] = _("위력: 40{CLEAR_TO 72}명중: 100\n");
    static const u8 sLongDesc[] = _("WWWWWWWWWWWWWWWWWWWWWWWWWW");
    static const u8 sShortDesc[] = _("짧은 설명");
    u8 buffer[128];
    u32 headerLength = StringLength(sHeader);
    bool32 isLong = FALSE;
    PARAMETRIZE { isLong = FALSE; }
    PARAMETRIZE { isLong = TRUE; }

    u8 *descStart = StringCopy(buffer, sHeader);
    u8 *descEnd = StringCopy(descStart, isLong ? sLongDesc : sShortDesc);
    u32 descLength = descEnd - descStart;
    u8 *end = WrapFontIdToFit(descStart, descEnd, FONT_NORMAL, 144);

    EXPECT_EQ(memcmp(buffer, sHeader, headerLength), 0);
    if (isLong)
    {
        ASSUME(GetStringWidth(FONT_NORMAL, sLongDesc, 0) > 144);
        u32 fontId = GetFontIdToFit(sLongDesc, FONT_NORMAL, 0, 144);
        EXPECT_NE(fontId, FONT_NORMAL);
        EXPECT_EQ(descStart[0], EXT_CTRL_CODE_BEGIN);
        EXPECT_EQ(descStart[1], EXT_CTRL_CODE_FONT);
        EXPECT_EQ(descStart[2], fontId);
        EXPECT_EQ(memcmp(&descStart[3], sLongDesc, descLength), 0);
        EXPECT_EQ(descStart[3 + descLength], EXT_CTRL_CODE_BEGIN);
        EXPECT_EQ(descStart[4 + descLength], EXT_CTRL_CODE_FONT);
        EXPECT_EQ(descStart[5 + descLength], FONT_NORMAL);
        EXPECT_EQ(*end, EOS);
        EXPECT_EQ(end - descStart, descLength + 6);
    }
    else
    {
        EXPECT_EQ(end, descEnd);
        EXPECT_EQ(memcmp(descStart, sShortDesc, descLength + 1), 0);
    }
}
