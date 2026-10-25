// HnS 설명 글꼴 폭 맞춤 회귀 테스트 — seq 444 #10521 A안(친구 결정 2026-10-24b) + 재확인 91 A안(2026-10-25).
// 창보다 넓은 설명만, 실제로 더 좁게 재지는 글꼴로 찍는다(GetDescriptionFontIdToFit / WrapDescriptionFontIdToFit —
// 설명 화면 5곳 전용). 한글 글리프는 FONT_NARROWER에서 더 넓으므로 넘치는 한글 설명이 더 넓어지면 안 된다.
// 대상 수는 현재 문자열 기준이라 고정하지 않는다. 이름이 모두 "HnS desc font:"으로 시작한다(TESTS='HnS desc font').
#include "global.h"
#include "move.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/abilities.h"
#include "contest_effect.h"

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
    EXPECT_EQ(GetDescriptionFontIdToFit(desc, FONT_NORMAL, 0, 144), FONT_NORMAL);
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
    u32 fontId = GetDescriptionFontIdToFit(GetMoveDescription(move), FONT_NARROW, 0, widthPx);
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
    u8 *end = WrapDescriptionFontIdToFit(descStart, descEnd, FONT_NORMAL, 144);

    EXPECT_EQ(memcmp(buffer, sHeader, headerLength), 0);
    if (isLong)
    {
        ASSUME(GetStringWidth(FONT_NORMAL, sLongDesc, 0) > 144);
        u32 fontId = GetDescriptionFontIdToFit(sLongDesc, FONT_NORMAL, 0, 144);
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

// RECHECK 91 plan A. Test strings are built from glyphs whose widths are fixed above:
// "W" 6/5/4 px, space 3/3/3 px, Hangul 8/8/11 px in FONT_NORMAL/FONT_NARROW/FONT_NARROWER.
static const u8 sShortEnglish[] = _("Tackle");
static const u8 sKorean20[] = _("가나다라마바사아자차카타파하가나다라마바"); // 160/160/220
static const u8 sKorean17[] = _("가나다라마바사아자차카타파하가나다"); // 136/136/187
static const u8 sMixedEnglishHeavy[] = _("가WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"); // 188/158/131
static const u8 sMixedKoreanHeavy[] = _("가나다라마바사아자차카타파하가나WWWW"); // 152/148/192
static const u8 sTwoLinesFit[] = _("가나다라마바사아자차카타파하가나\nWWWWWWWWWWWWWWWWWWWWWWWWWW"); // max line 156/130/176
static const u8 sTwoLinesOverflow[] = _("가나다라마바사아자차카타파하\nWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"); // max line 180/150/154
static const u8 sEnglish40[] = _("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"); // 240/200/160

TEST("HnS desc font: test string widths")
{
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, sKorean20, 0), 160);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sKorean20, 0), 160);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sKorean20, 0), 220);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sKorean17, 0), 136);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sKorean17, 0), 187);
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, sMixedEnglishHeavy, 0), 188);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sMixedEnglishHeavy, 0), 158);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sMixedEnglishHeavy, 0), 131);
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, sMixedKoreanHeavy, 0), 152);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sMixedKoreanHeavy, 0), 148);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sMixedKoreanHeavy, 0), 192);
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, sTwoLinesFit, 0), 156);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sTwoLinesFit, 0), 130);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sTwoLinesFit, 0), 176);
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, sTwoLinesOverflow, 0), 180);
    EXPECT_EQ(GetStringWidth(FONT_NARROW, sTwoLinesOverflow, 0), 150);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sTwoLinesOverflow, 0), 154);
    EXPECT_EQ(GetStringWidth(FONT_NARROWER, sEnglish40, 0), 160);
}

TEST("HnS desc font: a short English description keeps the base font")
{
    EXPECT_EQ(GetDescriptionFontIdToFit(sShortEnglish, FONT_NORMAL, 0, 144), FONT_NORMAL);
    EXPECT_EQ(GetDescriptionFontIdToFit(sShortEnglish, FONT_NARROW, 0, 128), FONT_NARROW);
}

// With today's strings and the screens' widths, the description fit picks the same font as GetFontIdToFit did,
// so the English descriptions #10521 fitted keep their font (summary ability 144, summary move and contest 160,
// Move Relearner 128 from FONT_NARROW, battle move info 144).
TEST("HnS desc font: today's descriptions keep the font GetFontIdToFit picked")
{
    u32 i;
    for (i = 1; i < MOVES_COUNT_ALL; i++)
    {
        const u8 *desc = GetMoveDescription(i);
        const u8 *contest = gContestEffects[GetMoveContestEffect(i)].description;
        EXPECT_EQ(GetDescriptionFontIdToFit(desc, FONT_NARROW, 0, 128), GetFontIdToFit(desc, FONT_NARROW, 0, 128));
        EXPECT_EQ(GetDescriptionFontIdToFit(desc, FONT_NORMAL, 0, 160), GetFontIdToFit(desc, FONT_NORMAL, 0, 160));
        EXPECT_EQ(GetDescriptionFontIdToFit(desc, FONT_NORMAL, 0, 144), GetFontIdToFit(desc, FONT_NORMAL, 0, 144));
        EXPECT_EQ(GetDescriptionFontIdToFit(contest, FONT_NORMAL, 0, 160), GetFontIdToFit(contest, FONT_NORMAL, 0, 160));
    }
    for (i = 1; i < ABILITIES_COUNT; i++)
    {
        const u8 *desc = gAbilitiesInfo[i].description;
        EXPECT_EQ(GetDescriptionFontIdToFit(desc, FONT_NORMAL, 0, 144), GetFontIdToFit(desc, FONT_NORMAL, 0, 144));
    }
}

// The case RECHECK 91 asked about: an overflowing Korean description must not switch to the 11 px font.
// It keeps its font and is still too wide (reported as not fitting, not as fitting).
TEST("HnS desc font: a long Korean description never gets wider")
{
    EXPECT_EQ(GetFontIdToFit(sKorean20, FONT_NORMAL, 0, 144), FONT_NARROWER); // the shared function, still used elsewhere
    EXPECT_EQ(GetDescriptionFontIdToFit(sKorean20, FONT_NORMAL, 0, 144), FONT_NORMAL);
    EXPECT_GT(GetStringWidth(FONT_NORMAL, sKorean20, 0), 144);
    EXPECT_EQ(GetFontIdToFit(sKorean17, FONT_NARROW, 0, 128), FONT_NARROWER);
    EXPECT_EQ(GetDescriptionFontIdToFit(sKorean17, FONT_NARROW, 0, 128), FONT_NARROW);
    EXPECT_GT(GetStringWidth(FONT_NARROW, sKorean17, 0), 128);
}

// Mixed Korean and English: the choice follows the measured width, not the font's place in the chain.
TEST("HnS desc font: mixed Korean and English descriptions follow the measured width")
{
    // Mostly English: only FONT_NARROWER fits (131 <= 144).
    EXPECT_EQ(GetDescriptionFontIdToFit(sMixedEnglishHeavy, FONT_NORMAL, 0, 144), FONT_NARROWER);
    // Mostly Korean: FONT_NARROW is narrower (148 < 152) but still too wide; FONT_NARROWER would be 192.
    u32 fontId = GetDescriptionFontIdToFit(sMixedKoreanHeavy, FONT_NORMAL, 0, 144);
    EXPECT_EQ(fontId, FONT_NARROW);
    EXPECT_GT(GetStringWidth(fontId, sMixedKoreanHeavy, 0), 144);
    EXPECT_EQ(GetFontIdToFit(sMixedKoreanHeavy, FONT_NORMAL, 0, 144), FONT_NARROWER);
}

// Several lines: the widest line decides, in each font.
TEST("HnS desc font: a multi-line description is measured by its widest line")
{
    // Korean line 128, English line 156 -> 130 in FONT_NARROW: fits.
    EXPECT_EQ(GetDescriptionFontIdToFit(sTwoLinesFit, FONT_NORMAL, 0, 144), FONT_NARROW);
    // English line 180/150/120, Korean line 112/112/154: FONT_NARROW is the narrowest (150) and still too wide.
    // GetFontIdToFit would go on to FONT_NARROWER and make the Korean line 154 px.
    u32 fontId = GetDescriptionFontIdToFit(sTwoLinesOverflow, FONT_NORMAL, 0, 144);
    EXPECT_EQ(fontId, FONT_NARROW);
    EXPECT_GT(GetStringWidth(fontId, sTwoLinesOverflow, 0), 144);
    EXPECT_EQ(GetFontIdToFit(sTwoLinesOverflow, FONT_NORMAL, 0, 144), FONT_NARROWER);
}

// No font fits: the narrowest-measuring font is used and the text still overflows - never counted as fitting.
TEST("HnS desc font: a description no font fits is still reported as overflowing")
{
    u32 fontId = GetDescriptionFontIdToFit(sEnglish40, FONT_NORMAL, 0, 144);
    EXPECT_EQ(fontId, FONT_NARROWER);
    EXPECT_GT(GetStringWidth(fontId, sEnglish40, 0), 144);
    fontId = GetDescriptionFontIdToFit(sKorean20, FONT_NORMAL, 0, 144);
    EXPECT_GT(GetStringWidth(fontId, sKorean20, 0), 144);
}

// Battle move info: an overflowing Korean description is not wrapped in a font code at all (the window font,
// FONT_NARROW, is as narrow as it gets for Hangul), and the Korean header line stays as it was.
TEST("HnS desc font: battle move info does not wrap an overflowing Korean description")
{
    static const u8 sHeader[] = _("위력: 40{CLEAR_TO 72}명중: 100\n");
    u8 buffer[128];
    u32 headerLength = StringLength(sHeader);
    u8 *descStart = StringCopy(buffer, sHeader);
    u8 *descEnd = StringCopy(descStart, sKorean20);
    u32 descLength = descEnd - descStart;
    u8 *end = WrapDescriptionFontIdToFit(descStart, descEnd, FONT_NORMAL, 144);

    EXPECT_EQ(end, descEnd);
    EXPECT_EQ(memcmp(buffer, sHeader, headerLength), 0);
    EXPECT_EQ(memcmp(descStart, sKorean20, descLength + 1), 0);
}
