#include "global.h"
#include "text.h"
#include "korean.h"
#include "korean_table.h"
#include "string_util.h"

// NOTE: 첫 항목에는 처리할 문자, 두번째 항목에는 종성 코드
static const u8 sSingleByteJongTable[][2] = 
{
    { CHAR_0, 21 },
    { CHAR_1,  8 },
    { CHAR_3, 16 },
    { CHAR_6,  1 },
    { CHAR_7,  8 },
    { CHAR_8,  8 },
};

bool8 IsKoreanGlyph(u16 glyph)
{
    return glyph >= 0x37 && glyph <= 0x41;
}

bool8 IsJaum(u8 glyph)
{
    return glyph >= 0x01 && glyph <= 0x13;
}

bool8 IsMoum(u8 glyph)
{
    return !IsJaum(glyph);
}

u8 GetJongCode(u16 ch)
{
    // Keyboard order: ㄱ ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ.
    // ㄸ/ㅃ/ㅉ have no final form; use the corresponding ㄷ/ㅂ/ㅈ final.
    static const u8 standaloneJaumJong[] =
    {
         1,  2,  4,  7,  7,  8, 16, 17, 17, 19,
        20, 21, 22, 22, 23, 24, 25, 26, 27,
    };
    u8 i, singleByteChar;

    if (ch >= 0x4101 && ch <= 0x4113)
        return standaloneJaumJong[ch - 0x4101];
    if (ch >= 0x4114 && ch <= 0x4128)
        return 0;

    if (IsKoreanGlyph((ch & 0xff00) >> 8))
    {
        return gConvertKoreanToUnicodeTable[ch - 0x3700] % 28;
    }
    else
    {
        singleByteChar = (u8)ch & 0x00ff;
        for (i = 0; i < ARRAY_COUNT(sSingleByteJongTable); i++)
        {
            if (singleByteChar == sSingleByteJongTable[i][0])
                return sSingleByteJongTable[i][1];
        }
    }

    return 0;
}

// Inspect an expanded output span, not the two raw bytes preceding end.
// Neither end nor bytes before start are read, including for partial tokens.
u8 GetStringJongCode(const u8 *start, const u8 *end)
{
    u16 lastChar = 0;
    bool8 japanese = FALSE;

    while (start < end)
    {
        u8 c = *start++;

        if (c == EOS)
            break;
        if (!japanese && IsKoreanGlyph(c))
        {
            if (start == end)
                return 0;
            lastChar = (c << 8) | *start++;
        }
        else if (c == EXT_CTRL_CODE_BEGIN)
        {
            u8 code, length;

            if (start == end)
                return 0;
            code = *start;
            length = GetExtCtrlCodeLength(code);
            // The renderer consumes a speaker ID, unlike the legacy length table.
            if (code == EXT_CTRL_CODE_SPEAKER)
                length = 2;
            if (length == 0 || end - start < length)
                return 0;
            if (code == EXT_CTRL_CODE_JPN)
                japanese = TRUE;
            else if (code == EXT_CTRL_CODE_ENG)
                japanese = FALSE;
            else if (code == EXT_CTRL_CODE_ESCAPE || code == EXT_CTRL_CODE_FILL_WINDOW)
                lastChar = 0;
            start += length;
        }
        else if (c == CHAR_DYNAMIC || c == PLACEHOLDER_BEGIN
              || c == CHAR_KEYPAD_ICON || c == CHAR_EXTRA_SYMBOL)
        {
            if (start == end)
                return 0;
            // An unresolved token or a symbol is not its one-byte argument.
            start++;
            lastChar = 0;
        }
        else if (c != CHAR_ZWS || japanese)
        {
            // Single-byte characters retain the existing digit/fallback policy.
            lastChar = c;
        }
    }

    return GetJongCode(lastChar);
}

u8 GetCho(u8 index)
{
    return index - 1;
}

u8 GetJung(u8 index)
{
    return index - 0x14;
}

u8 GetJong(u8 index)
{
    u8 i = 0;
    s8 ch;

    while ((ch = gConvertJongToChoTable[i]) != -1)
    {
        if (ch == index)
            return i;
        i++;
    }

    return -1;
}

u8 GetChoByHangul(u16 korean)
{
    u16 code = gConvertKoreanToUnicodeTable[korean - 0x3700];
    return ((code / 28) / 21) + 0x01;
}

u8 GetJungByHangul(u16 korean)
{
    u16 code = gConvertKoreanToUnicodeTable[korean - 0x3700];
    return ((code / 28) % 21) + 0x14;
}

u8 GetJongByHangul(u16 korean)
{
    return gConvertJongToChoTable[gConvertKoreanToUnicodeTable[korean - 0x3700] % 28];
}

u8 GetJongIndexByHangul(u16 korean)
{
    return gConvertKoreanToUnicodeTable[korean - 0x3700] % 28;
}

u8 ConvertJongToCho(u8 jong)
{
    return gConvertJongToChoTable[jong];
}

u16 ConvertKoreanToUnicode(u16 korean)
{
    return gConvertKoreanToUnicodeTable[korean - 0x3700];
}

u16 ConvertUnicodeToKorean(u16 code)
{
    return gConvertUnicodeToKoreanTable[code];
}

u16 AssembleHangul(u8 cho, u8 jung, u8 jong)
{    
    return ConvertUnicodeToKorean(AssembleHangulToUnicode(cho, jung, jong));
}

u16 AssembleHangulToUnicode(u8 cho, u8 jung, u8 jong)
{    
    return (((cho * 21) + jung) * 28) + jong;
}

u8 JoinJaum(u8 ch, u8 ch2)
{
    switch (ch)
    {
    // ㄱ
    case 0x01:
        // ㅅ
        if (ch2 == 0x0a)
            return 3;
        break;
    // ㄴ
    case 0x03:
        switch (ch2)
        {
        // ㅈ
        case 0x0d:
            return 5;
        // ㅎ
        case 0x13:
            return 6;
        }
        break;
    // ㄹ
    case 0x06:
        switch (ch2)
        {
        // ㄱ
        case 0x01:
            return 9;
        // ㅁ
        case 0x07:
            return 10;
        // ㅂ
        case 0x08:
            return 11;
        // ㅅ
        case 0x0a:
            return 12;
        // ㅌ
        case 0x11:
            return 13;
        // ㅍ
        case 0x12:
            return 14;
        // ㅎ
        case 0x13:
            return 15;
        }
        break;
    // ㅂ
    case 0x08:
        if (ch2 == 0x0a)
            return 18;
        break;
    }

    // 조합불가
    return 0;
}

u8 JoinMoum(u8 ch, u8 ch2)
{
    switch (ch)
    {
    // ㅗ
    case 0x1c:
        switch (ch2)
        {
        // ㅏ
        case 0x14:
            return 0x1d;
        // ㅐ
        case 0x15:
            return 0x1e;
        // ㅣ
        case 0x28:
            return 0x1f;
        }
        break;

    // ㅜ
    case 0x21:
        switch (ch2)
        {
        // ㅓ
        case 0x18:
            return 0x22;
        // ㅔ
        case 0x19:
            return 0x23;
        // ㅣ
        case 0x28:
            return 0x24;
        }
        break;

    // ㅡ
    case 0x26:
        // ㅣ
        if (ch2 == 0x28)
            return 0x27;
        break;
    }

    // 조합불가
    return 0;
}

u8 SplitJung(u8 ch, u8 index)
{
    if (index == 0)
    {
        if (ch >= 9 && ch <= 11)
        {
            // ㅘ, ㅙ, ㅚ
            return 8;
        }
        else if (ch >= 14 && ch <= 16)
        {
            // ㅝ, ㅞ, ㅟ
            return 13;
        }
        else if (ch == 19)
        {
            // ㅢ
            return 18;
        }
    }
    else
    {
        switch (ch)
        {
        // ㅘ
        case 9:
            return 0;
        // ㅙ
        case 10:
            return 1;
        // ㅚ, ㅟ, ㅢ
        case 11:
        case 16:
        case 19:
            return 20;
        // ㅝ
        case 14:
            return 4;
        // ㅞ
        case 15:
            return 5;
        }
    }

    return 0;
}

u8 SplitJong(u8 ch, u8 index)
{
    if (index == 0)
    {
        if (ch == 3)
        {
            // ㄳ
            return 1;
        }
        else if (ch >= 5 && ch <= 6)
        {
            // ㄵ, ㄶ
            return 4;
        }
        else if (ch >= 9 && ch <= 15)
        {
            // ㄺ, ㄻ, ㄼ, ㄽ, ㄾ, ㄿ, ㅀ
            return 8;
        }
        else if (ch == 18)
        {
            // ㅄ
            return 17;
        }
    }
    else
    {
        switch (ch)
        {
        // ㄳ
        case 3:
            return 19;
        // ㄵ
        case 5:
            return 22;
        // ㄶ
        case 6:
            return 27;
        // ㄺ
        case 9:
            return 1;
        // ㄻ
        case 10:
            return 16;
        // ㄼ
        case 11:
            return 17;
        // ㄽ
        case 12:
            return 19;
        // ㄾ
        case 13:
            return 25;
        // ㄿ
        case 14:
            return 26;
        // ㅀ
        case 15:
            return 27;
        // ㅄ
        case 18:
            return 19;
        }
    }

    return 0;
}
