#include "global.h"
#include "test/test.h"
#include "battle.h"
#include "battle_message.h"
#include "korean.h"
#include "constants/battle_string_ids.h"
#include "constants/characters.h"

// HnS: Korean stat change messages put the degree word in front of the verb, and the template already has the
// space after the particle: "{B_BUFF1}{B_TXT_IGA} {B_BUFF2}<verb>!". battle_stat_change.c buffers B_BUFF2 as
// STRINGID_EMPTYSTRING3 for one stage, STATSHARPLY for +2 and STATHARSHLY / DRASTICALLY / SEVERELY for -2, +3, -3.
// gText_EmptyString3 used to be a space, which printed two spaces before the verb in every 1-stage message
// (friend decision 2026-10-07 #6: one space everywhere).
// HnS prints Korean text, so these checks count the spaces of the expanded second line instead of comparing text.

static const u8 *GetSecondLine(const u8 *str)
{
    while (*str != EOS)
    {
        if (IsKoreanGlyph(*str) || *str == PLACEHOLDER_BEGIN)
        {
            str += 2;
            continue;
        }
        if (*str == CHAR_NEWLINE)
            return str + 1;
        str++;
    }
    return NULL;
}

static void CountSpaces(const u8 *str, u32 *spaces, u32 *doubleSpaces)
{
    bool32 prevSpace = FALSE;

    *spaces = 0;
    *doubleSpaces = 0;
    while (*str != EOS)
    {
        if (IsKoreanGlyph(*str))
        {
            prevSpace = FALSE;
            str += 2;
            continue;
        }
        if (*str == CHAR_SPACE)
        {
            (*spaces)++;
            if (prevSpace)
                (*doubleSpaces)++;
            prevSpace = TRUE;
        }
        else
        {
            prevSpace = FALSE;
        }
        str++;
    }
}

TEST("HnS: stat change messages have single spaces for every degree")
{
    enum StringID stringId = 0;
    u16 degreeStringId = 0;
    u32 expectedSpaces = 0, spaces, doubleSpaces;
    const u8 *line;
    u8 str[64];

    // spaces on the second line: 1 stage "<stat><particle> <verb>!", +2 "... <sharply> <verb>!", -2/+3/-3 "... <very> <sharply> <verb>!"
    PARAMETRIZE { stringId = STRINGID_STATROSE;                degreeStringId = STRINGID_EMPTYSTRING3; expectedSpaces = 1; }
    PARAMETRIZE { stringId = STRINGID_STATROSE;                degreeStringId = STRINGID_STATSHARPLY;  expectedSpaces = 2; }
    PARAMETRIZE { stringId = STRINGID_STATROSE;                degreeStringId = STRINGID_DRASTICALLY;  expectedSpaces = 3; }
    PARAMETRIZE { stringId = STRINGID_STATFELL;                degreeStringId = STRINGID_EMPTYSTRING3; expectedSpaces = 1; }
    PARAMETRIZE { stringId = STRINGID_STATFELL;                degreeStringId = STRINGID_STATHARSHLY;  expectedSpaces = 3; }
    PARAMETRIZE { stringId = STRINGID_STATFELL;                degreeStringId = STRINGID_SEVERELY;     expectedSpaces = 3; }
    PARAMETRIZE { stringId = STRINGID_USINGITEMSTATOFPKMNROSE; degreeStringId = STRINGID_EMPTYSTRING3; expectedSpaces = 1; }
    PARAMETRIZE { stringId = STRINGID_USINGITEMSTATOFPKMNROSE; degreeStringId = STRINGID_STATSHARPLY;  expectedSpaces = 2; }
    PARAMETRIZE { stringId = STRINGID_USINGITEMSTATOFPKMNFELL; degreeStringId = STRINGID_EMPTYSTRING3; expectedSpaces = 1; }
    PARAMETRIZE { stringId = STRINGID_USINGITEMSTATOFPKMNFELL; degreeStringId = STRINGID_STATHARSHLY;  expectedSpaces = 3; }

    line = GetSecondLine(gBattleStringsTable[stringId]);
    EXPECT(line != NULL);
    PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
    PREPARE_STRING_BUFFER(gBattleTextBuff2, degreeStringId);
    BattleStringExpandPlaceholders(line, str, sizeof(str));
    CountSpaces(str, &spaces, &doubleSpaces);
    EXPECT_EQ(doubleSpaces, 0);
    EXPECT_EQ(spaces, expectedSpaces);
}
