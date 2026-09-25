#include "global.h"
#include "option_menu.h"
#include "bg.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "event_data.h"
#include "constants/flags.h"

// =============================================================================
// Tab definitions
// =============================================================================

enum {
    TAB_MAIN,
    TAB_BATTLE,
    TAB_SOUND,
    TAB_COUNT,
};

// =============================================================================
// Per-tab item enums
// =============================================================================

enum {
    ITEM_MAIN_TEXTSPEED,
    ITEM_MAIN_BATTLESCENE,
    ITEM_MAIN_BATTLESTYLE,
    ITEM_MAIN_BUTTONMODE,
    ITEM_MAIN_FOLLOWER,
    ITEM_MAIN_LARGE_FOLLOWER,
    ITEM_MAIN_AUTORUN,
    ITEM_MAIN_AUTORUN_SURF,
    ITEM_MAIN_FISHING,
    ITEM_MAIN_FASTER_JOY,
    ITEM_MAIN_UNIT_TYPE,
    ITEM_MAIN_MATCHCALL,
    ITEM_MAIN_FRAMETYPE,
    ITEM_MAIN_COUNT,
};

enum {
    ITEM_BATTLE_FAST_INTRO,
    ITEM_BATTLE_FAST_BATTLES,
    ITEM_BATTLE_NEW_BACKGROUNDS,
    ITEM_BATTLE_NEW_BATTLEUI,
    ITEM_BATTLE_BALL_PROMPT,
    ITEM_BATTLE_RUN_TYPE,
    ITEM_BATTLE_LR_RUN,
    ITEM_BATTLE_COUNT,
};

enum {
    ITEM_SOUND_SOUND,
    ITEM_SOUND_MUSIC,
    ITEM_SOUND_BIKE_MUSIC,
    ITEM_SOUND_SURF_MUSIC,
    ITEM_SOUND_COUNT,
};

#define MAX_ITEMS_PER_TAB 16
#define ITEMS_VISIBLE 5
#define Y_DIFF 16

// =============================================================================
// Menu item descriptor
// =============================================================================

struct OptionMenuItem
{
    const u8 *name;
    const u8 *const *descriptions;
    u8 numChoices;
    const u8 *const *choiceNames;
};

// =============================================================================
// EWRAM state
// =============================================================================

struct OptionMenuState
{
    u8 currentTab;
    u8 listTaskId;
    u8 arrowTaskId;
    u16 scrollOffset[TAB_COUNT];
    u16 selectedRow[TAB_COUNT];
    u8 selections[TAB_COUNT * MAX_ITEMS_PER_TAB];
};

static EWRAM_DATA struct OptionMenuState *sMenu = NULL;

// =============================================================================
// Window / BG templates
// =============================================================================

enum {
    WIN_TOPBAR,
    WIN_OPTIONS,
    WIN_DESCRIPTION,
};

static const struct WindowTemplate sWinTemplates[] = {
    [WIN_TOPBAR] = {
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2,
    },
    [WIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 26,
        .height = 10,
        .paletteNum = 1,
        .baseBlock = 62,
    },
    [WIN_DESCRIPTION] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 1,
        .baseBlock = 500,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
};

static const u16 sBgPal[] = {RGB(14, 20, 24)};
static const u16 sTextPal[] = INCBIN_U16("graphics/interface/option_menu_text_custom.gbapal");

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

#define TEXT_COLOR_OPTIONS_WHITE              1
#define TEXT_COLOR_OPTIONS_GRAY_FG            2
#define TEXT_COLOR_OPTIONS_GRAY_SHADOW        3
#define TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG      4
#define TEXT_COLOR_OPTIONS_ORANGE_FG          5
#define TEXT_COLOR_OPTIONS_ORANGE_SHADOW      6
#define TEXT_COLOR_OPTIONS_RED_FG             7
#define TEXT_COLOR_OPTIONS_RED_SHADOW         8
#define TEXT_COLOR_OPTIONS_GREEN_FG           9
#define TEXT_COLOR_OPTIONS_GREEN_SHADOW      10
#define TEXT_COLOR_OPTIONS_GREEN_DARK_FG     11
#define TEXT_COLOR_OPTIONS_GREEN_DARK_SHADOW 12
#define TEXT_COLOR_OPTIONS_RED_DARK_FG       13
#define TEXT_COLOR_OPTIONS_RED_DARK_SHADOW   14

// =============================================================================
// Choice strings
// =============================================================================

static const u8 *const sChoices_OnOff[] = {
    COMPOUND_STRING("ON"),
    COMPOUND_STRING("OFF"),
};

static const u8 *const sChoices_ShiftSet[] = {
    COMPOUND_STRING("교체"),
    COMPOUND_STRING("토너먼트"),
};

static const u8 *const sChoices_MonoStereo[] = {
    COMPOUND_STRING("모노"),
    COMPOUND_STRING("스테레오"),
};

static const u8 *const sChoices_ButtonMode[] = {
    COMPOUND_STRING("노말"),
    COMPOUND_STRING("LR"),
    COMPOUND_STRING("L=A"),
};

static const u8 *const sChoices_TextSpeed[] = {
    COMPOUND_STRING("느리게"),
    COMPOUND_STRING("보통"),
    COMPOUND_STRING("빠르게"),
    COMPOUND_STRING("더 빠르게"),
};

static const u8 *const sChoices_MetricImperial[] = {
    COMPOUND_STRING("미터법"),
    COMPOUND_STRING("야드파운드법"),
};

static const u8 *const sChoices_OldModern[] = {
    COMPOUND_STRING("기존"),
    COMPOUND_STRING("모던"),
};

static const u8 *const sChoices_Gen3Gen4[] = {
    COMPOUND_STRING("3세대"),
    COMPOUND_STRING("4세대"),
};

static const u8 *const sChoices_RunType[] = {
    COMPOUND_STRING("끈다"),
    COMPOUND_STRING("L+R+A"),
    COMPOUND_STRING("B{RIGHT_ARROW}A"),
    COMPOUND_STRING("B"),
};

static const u8 sText_TopBar_Left[]  = _("{L_BUTTON}");
static const u8 sText_TopBar_Right[] = _("{R_BUTTON}");

// =============================================================================
// Frame type — special: draws "TYPE  N" instead of choice text
// =============================================================================

// numChoices = 0 signals special handling; we use WINDOW_FRAMES_COUNT for cycling
#define FRAME_TYPE_SPECIAL 0

// =============================================================================
// Descriptions
// =============================================================================

static const u8 sText_Desc_TextSpeed[] = _("메시지의 속도를\n4종류 중에서 선택할 수 있습니다");
static const u8 *const sDesc_TextSpeed[] = {
    sText_Desc_TextSpeed,
    sText_Desc_TextSpeed,
    sText_Desc_TextSpeed,
    sText_Desc_TextSpeed,
};
static const u8 *const sDesc_BattleScene[] = {
    COMPOUND_STRING("전투에서 포켓몬의 등장\n애니메이션과 기술 애니메이션을 봅니다"),
    COMPOUND_STRING("전투에서 포켓몬의 등장\n애니메이션과 기술 애니메이션을 보지 않습니다"),
};
static const u8 *const sDesc_BattleStyle[] = {
    COMPOUND_STRING("상대의 포켓몬이 쓰려졌을 때\n포켓몬을 교체할 수 있습니다"),
    COMPOUND_STRING("상대의 포켓몬이 쓰러져도\n포켓몬을 교체할 수 없습니다"),
};
static const u8 *const sDesc_ButtonMode[] = {
    COMPOUND_STRING("모든 버튼을\n보통 때처럼 사용합니다 "),
    COMPOUND_STRING("일부 화면에서\nL과 R버튼을 좌우 버튼처럼 사용합니다"),
    COMPOUND_STRING("L버튼을 A버튼처럼 사용하여\n한 손으로 플레이할 수 있습니다"),
};
static const u8 *const sDesc_Follower[] = {
    COMPOUND_STRING("지닌 포켓몬 중 선두\n포켓몬을 데리고 걷습니다"),
    COMPOUND_STRING("혼자서 걷습니다"),
};
static const u8 *const sDesc_LargeFollower[] = {
    COMPOUND_STRING("거대한 포켓몬을 데리고 걷습니다\n그래픽 문제가 발생할 수 있습니다"),
    COMPOUND_STRING("거대한 포켓몬을 데리고 걷지 않습니다\n이 설정을 권장합니다"),
};
static const u8 *const sDesc_Autorun[] = {
    COMPOUND_STRING("B버튼을 누르지 않아도\n자동으로 대시합니다"),
    COMPOUND_STRING("B버튼을 누른 상태로 대시합니다"),
};
static const u8 *const sDesc_AutorunSurf[] = {
    COMPOUND_STRING("B버튼을 누르지 않아도\n자동으로 파도타기로 빠르게 대시합니다"),
    COMPOUND_STRING("B버튼을 누른 상태로\n파도타기로 빠르게 대시합니다"),
};
static const u8 *const sDesc_Fishing[] = {
    COMPOUND_STRING("낚시를 할 때 자동으로 낚아챕니다"),
    COMPOUND_STRING("낚시를 할 때 직접 낚아챕니다\n원래 방식대로 낚시를 즐기세요!"),
};
static const u8 *const sDesc_FasterJoy[] = {
    COMPOUND_STRING("간호순이 포켓몬을\n빠르게 회복시킵니다"),
    COMPOUND_STRING("간호순이 포켓몬을\n평범하게 회복시킵니다"),
};
static const u8 *const sDesc_UnitType[] = {
    COMPOUND_STRING("나무열매와 포켓몬의 무게와\n크기를 킬로그램과 미터로 표시합니다"),
    COMPOUND_STRING("나무열매와 포켓몬의 무게와\n크기를 파운드와 인치로 표시합니다"),
};
static const u8 *const sDesc_MatchCall[] = {
    COMPOUND_STRING("트레이너가 전화를 걸어와\n재대결을 신청하거나 정보를 제공합니다"),
    COMPOUND_STRING("전화를 받지 않습니다\n단, 특수한 이벤트 전화는 걸려옵니다"),
};
static const u8 *const sDesc_FrameType[] = {
    COMPOUND_STRING("윈도우의 디자인을\n선택할 수 있습니다"),
};
static const u8 *const sDesc_FastIntro[] = {
    COMPOUND_STRING("인트로 애니메이션을 생략하고\n빠르게 야생 포켓몬과 배틀합니다"),
    COMPOUND_STRING("인트로 애니메이션을 생략하지 않습니다"),
};
static const u8 *const sDesc_FastBattles[] = {
    COMPOUND_STRING("배틀 중 모든 딜레이를\n생략하여 빠르게 진행합니다"),
    COMPOUND_STRING("직접 딜레이를 생략합니다.\nA 또는 B버튼을 누르면 생략됩니다"),
};
static const u8 *const sDesc_NewBackgrounds[] = {
    COMPOUND_STRING("3세대의 기존\n배틀 배경을 사용합니다"),
    COMPOUND_STRING("HnS의 현대화된\n배틀 배경을 사용합니다"),
};
static const u8 *const sDesc_NewBattleUI[] = {
    COMPOUND_STRING("3세대의 기존\n배틀 UI를 사용합니다"),
    COMPOUND_STRING("4세대의 현대화된\n배틀 UI를 사용합니다"),
};
static const u8 *const sDesc_BallPrompt[] = {
    COMPOUND_STRING("배틀 중 {R_BUTTON}을 눌러 볼을 사용합니다\n{L_BUTTON}/{R_BUTTON}을 길게 눌러 볼을 교체합니다"),
    COMPOUND_STRING("몬스터볼을 바로\n사용하는 버튼을 비활성화합니다"),
};
static const u8 *const sDesc_RunType[] = {
    COMPOUND_STRING("빠른 도망가기를 사용하지 않습니다"),
    COMPOUND_STRING("배틀 시작 전\n{L_BUTTON}+{R_BUTTON}을 누른 채로 {A_BUTTON}를 눌러 도망갑니다"),
    COMPOUND_STRING("배틀 시작 후\n{B_BUTTON}를 누르면 도망간다로 커서가 이동합니다"),
    COMPOUND_STRING("배틀 시작 전\n{B_BUTTON}를 눌러 배틀에서 도망갑니다"),
};
static const u8 *const sDesc_LRRun[] = {
    COMPOUND_STRING("빠른 도망가기 프롬프트를 활성화합니다"),
    COMPOUND_STRING("해당 프롬프트를 비활성화합니다\n버튼 커맨드는 그대로 작동합니다"),
};
static const u8 *const sDesc_Sound[] = {
    COMPOUND_STRING("좌우 스피커에서 동일한 소리가 나옵니다\n실기 플레이 시 권장합니다"),
    COMPOUND_STRING("좌우 스피커에서 각각 다른 소리가 나옵니다\n이어폰 사용 시 권장합니다"),
};
static const u8 *const sDesc_Music[] = {
    COMPOUND_STRING("배경음악을 켭니다\n맵 이동 시 설정이 적용됩니다"),
    COMPOUND_STRING("배경음악을 끕니다\n맵 이동 시 설정이 적용됩니다"),
};
static const u8 *const sDesc_BikeMusic[] = {
    COMPOUND_STRING("자전거 배경음악을 켭니다"),
    COMPOUND_STRING("자전거 배경음악을 끕니다"),
};
static const u8 *const sDesc_SurfMusic[] = {
    COMPOUND_STRING("파도타기 배경음악을 켭니다"),
    COMPOUND_STRING("자전거 배경음악을 끕니다"),
};

// =============================================================================
// Tab item tables
// =============================================================================

static const struct OptionMenuItem sTabItems_Main[] = {
    [ITEM_MAIN_TEXTSPEED] = {
        .name         = COMPOUND_STRING("이야기의 속도"),
        .descriptions = sDesc_TextSpeed,
        .numChoices   = 4,
        .choiceNames  = sChoices_TextSpeed,
    },
    [ITEM_MAIN_BATTLESCENE] = {
        .name         = COMPOUND_STRING("배틀 애니메이션"),
        .descriptions = sDesc_BattleScene,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_BATTLESTYLE] = {
        .name         = COMPOUND_STRING("시합 룰"),
        .descriptions = sDesc_BattleStyle,
        .numChoices   = 2,
        .choiceNames  = sChoices_ShiftSet,
    },
    [ITEM_MAIN_BUTTONMODE] = {
        .name         = COMPOUND_STRING("버튼 모드"),
        .descriptions = sDesc_ButtonMode,
        .numChoices   = 3,
        .choiceNames  = sChoices_ButtonMode,
    },
    [ITEM_MAIN_FOLLOWER] = {
        .name         = COMPOUND_STRING("동행 포켓몬"),
        .descriptions = sDesc_Follower,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_LARGE_FOLLOWER] = {
        .name         = COMPOUND_STRING("거대한 동행 포켓몬"),
        .descriptions = sDesc_LargeFollower,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_AUTORUN] = {
        .name         = COMPOUND_STRING("자동 대시"),
        .descriptions = sDesc_Autorun,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_AUTORUN_SURF] = {
        .name         = COMPOUND_STRING("자동 대시 (파도타기)"),
        .descriptions = sDesc_AutorunSurf,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_FISHING] = {
        .name         = COMPOUND_STRING("빠른 낚시"),
        .descriptions = sDesc_Fishing,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_FASTER_JOY] = {
        .name         = COMPOUND_STRING("빠른 간호순 회복"),
        .descriptions = sDesc_FasterJoy,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_UNIT_TYPE] = {
        .name         = COMPOUND_STRING("단위계"),
        .descriptions = sDesc_UnitType,
        .numChoices   = 2,
        .choiceNames  = sChoices_MetricImperial,
    },
    [ITEM_MAIN_MATCHCALL] = {
        .name         = COMPOUND_STRING("전화번호 등록"),
        .descriptions = sDesc_MatchCall,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_MAIN_FRAMETYPE] = {
        .name         = COMPOUND_STRING("윈도우"),
        .descriptions = sDesc_FrameType,
        .numChoices   = FRAME_TYPE_SPECIAL,
        .choiceNames  = NULL,
    },
};

static const struct OptionMenuItem sTabItems_Battle[] = {
    [ITEM_BATTLE_FAST_INTRO] = {
        .name         = COMPOUND_STRING("인트로 생략"),
        .descriptions = sDesc_FastIntro,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_BATTLE_FAST_BATTLES] = {
        .name         = COMPOUND_STRING("배틀 딜레이 생략"),
        .descriptions = sDesc_FastBattles,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_BATTLE_NEW_BACKGROUNDS] = {
        .name         = COMPOUND_STRING("배틀 배경"),
        .descriptions = sDesc_NewBackgrounds,
        .numChoices   = 2,
        .choiceNames  = sChoices_OldModern,
    },
    [ITEM_BATTLE_NEW_BATTLEUI] = {
        .name         = COMPOUND_STRING("배틀 UI"),
        .descriptions = sDesc_NewBattleUI,
        .numChoices   = 2,
        .choiceNames  = sChoices_Gen3Gen4,
    },
    [ITEM_BATTLE_BALL_PROMPT] = {
        .name         = COMPOUND_STRING("몬스터볼 프롬프트"),
        .descriptions = sDesc_BallPrompt,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_BATTLE_RUN_TYPE] = {
        .name         = COMPOUND_STRING("빠른 도망가기"),
        .descriptions = sDesc_RunType,
        .numChoices   = 4,
        .choiceNames  = sChoices_RunType,
    },
    [ITEM_BATTLE_LR_RUN] = {
        .name         = COMPOUND_STRING("도망가기 프롬프트"),
        .descriptions = sDesc_LRRun,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
};

static const struct OptionMenuItem sTabItems_Sound[] = {
    [ITEM_SOUND_SOUND] = {
        .name         = COMPOUND_STRING("사운드"),
        .descriptions = sDesc_Sound,
        .numChoices   = 2,
        .choiceNames  = sChoices_MonoStereo,
    },
    [ITEM_SOUND_MUSIC] = {
        .name         = COMPOUND_STRING("배경음악"),
        .descriptions = sDesc_Music,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_SOUND_BIKE_MUSIC] = {
        .name         = COMPOUND_STRING("자전거 배경음악"),
        .descriptions = sDesc_BikeMusic,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
    [ITEM_SOUND_SURF_MUSIC] = {
        .name         = COMPOUND_STRING("파도타기 배경음악"),
        .descriptions = sDesc_SurfMusic,
        .numChoices   = 2,
        .choiceNames  = sChoices_OnOff,
    },
};

// =============================================================================
// Per-tab metadata
// =============================================================================

struct TabDef
{
    const u8 *tabName;
    const struct OptionMenuItem *items;
    u8 count;
};

static const struct TabDef sTabs[TAB_COUNT] = {
    [TAB_MAIN]   = { COMPOUND_STRING("설정"),        sTabItems_Main,   ITEM_MAIN_COUNT },
    [TAB_BATTLE] = { COMPOUND_STRING("배틀 설정"), sTabItems_Battle, ITEM_BATTLE_COUNT },
    [TAB_SOUND]  = { COMPOUND_STRING("사운드"),          sTabItems_Sound,  ITEM_SOUND_COUNT },
};

// =============================================================================
// Forward declarations
// =============================================================================

static void MainCB2(void);
static void VBlankCB(void);
static void Task_FadeIn(u8 taskId);
static void Task_ProcessInput(u8 taskId);
static void Task_Save(u8 taskId);
static void Task_FadeOut(u8 taskId);
static void InitListMenu(void);
static void DestroyCurrentListMenu(void);
static void DrawTopBar(void);
static void DrawDescription(void);
static void DrawBgFrames(void);
static void OptionMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void OptionMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y);

static u8 *GetSelectionPtr(u8 tab, u8 itemIndex);

static EWRAM_DATA struct ListMenuItem sListItems[MAX_ITEMS_PER_TAB] = {0};

// =============================================================================
// Helpers
// =============================================================================

static u8 *GetSelectionPtr(u8 tab, u8 itemIndex)
{
    return &sMenu->selections[tab * MAX_ITEMS_PER_TAB + itemIndex];
}

static u8 GetCurrentTabItemCount(void)
{
    return sTabs[sMenu->currentTab].count;
}

static const struct OptionMenuItem *GetCurrentTabItems(void)
{
    return sTabs[sMenu->currentTab].items;
}

// =============================================================================
// Conditions
// =============================================================================

static bool8 CheckConditions(u8 tab, u8 itemIndex)
{
    if (tab == TAB_BATTLE && itemIndex == ITEM_BATTLE_LR_RUN)
    {
        u8 runType = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_RUN_TYPE);
        return runType == 1 || runType == 3;
    }
    return TRUE;
}

// =============================================================================
// Callbacks
// =============================================================================

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

// =============================================================================
// Row highlight
// =============================================================================

static void HighlightRow(void)
{
    u16 scrollOffset, selectedRow;
    ListMenuGetScrollAndRow(sMenu->listTaskId, &scrollOffset, &selectedRow);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(Y_DIFF, 224));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(selectedRow * Y_DIFF + 24, selectedRow * Y_DIFF + 40));
}

// =============================================================================
// Right-side choice drawing
// =============================================================================

static void DrawRightSideChoiceText(const u8 *text, int x, int y, bool8 chosen, bool8 active)
{
    u8 color[3];
    color[0] = TEXT_COLOR_TRANSPARENT;
    if (active)
    {
        color[1] = chosen ? TEXT_COLOR_OPTIONS_RED_FG : TEXT_COLOR_OPTIONS_GRAY_FG;
        color[2] = chosen ? TEXT_COLOR_OPTIONS_RED_SHADOW : TEXT_COLOR_OPTIONS_GRAY_SHADOW;
    }
    else
    {
        color[1] = chosen ? TEXT_COLOR_OPTIONS_RED_DARK_FG : TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG;
        color[2] = chosen ? TEXT_COLOR_OPTIONS_RED_DARK_SHADOW : TEXT_COLOR_OPTIONS_GRAY_SHADOW;
    }
    AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, x, y, 0, 0, color, TEXT_SKIP_DRAW, text);
}

static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int widthLeft = GetStringWidth(FONT_NORMAL, txt1, 0);
    int widthMid = GetStringWidth(FONT_NORMAL, txt2, 0);
    int widthRight = GetStringWidth(FONT_NORMAL, txt3, 0);
    widthMid -= (198 - 104);
    return (widthLeft - widthMid - widthRight) / 2 + 104;
}

static void DrawChoices_Two(const u8 *const *strings, int selection, int y, bool8 active)
{
    DrawRightSideChoiceText(strings[0], 104, y + 1, selection == 0, active);
    DrawRightSideChoiceText(strings[1], GetStringRightAlignXOffset(FONT_NORMAL, strings[1], 198), y + 1, selection == 1, active);
}

static void DrawChoices_Three(const u8 *const *strings, int selection, int y, bool8 active)
{
    int xMid = GetMiddleX(strings[0], strings[1], strings[2]);
    DrawRightSideChoiceText(strings[0], 104, y + 1, selection == 0, active);
    DrawRightSideChoiceText(strings[1], xMid, y + 1, selection == 1, active);
    DrawRightSideChoiceText(strings[2], GetStringRightAlignXOffset(FONT_NORMAL, strings[2], 198), y + 1, selection == 2, active);
}

static void DrawChoices_Four(const u8 *const *strings, int selection, int y, bool8 active)
{
    static const u8 orders[][3] = { {0, 1, 2}, {0, 1, 2}, {1, 2, 3}, {1, 2, 3} };
    const u8 *order = orders[selection];
    int xMid = GetMiddleX(strings[order[0]], strings[order[1]], strings[order[2]]);
    DrawRightSideChoiceText(strings[order[0]], 104, y + 1, selection == order[0], active);
    DrawRightSideChoiceText(strings[order[1]], xMid, y + 1, selection == order[1], active);
    DrawRightSideChoiceText(strings[order[2]], GetStringRightAlignXOffset(FONT_NORMAL, strings[order[2]], 198), y + 1, selection == order[2], active);
}

// Frame type: draws "TYPE  N" on the right side
static void DrawFrameTypeChoice(u8 selection, int y, bool8 active)
{
    u8 text[16];
    u8 n = selection + 1;
    u8 i = 0;

    if (n / 10 != 0)
    {
        text[i++] = n / 10 + CHAR_0;
        text[i++] = n % 10 + CHAR_0;
    }
    else
    {
        text[i++] = n % 10 + CHAR_0;
        text[i++] = CHAR_SPACER;
    }
    text[i] = EOS;

    DrawRightSideChoiceText(COMPOUND_STRING("타입"), 104, y + 1, FALSE, active);
    DrawRightSideChoiceText(text, 128, y + 1, TRUE, active);
}

// =============================================================================
// ListMenu callbacks
// =============================================================================

static void OptionMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (!onInit)
        PlaySE(SE_SELECT);

    ListMenuGetScrollAndRow(sMenu->listTaskId, &sMenu->scrollOffset[sMenu->currentTab], &sMenu->selectedRow[sMenu->currentTab]);
    HighlightRow();
    DrawDescription();
}

static void OptionMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    const struct OptionMenuItem *items = GetCurrentTabItems();

    if (itemId >= GetCurrentTabItemCount())
        return;

    bool8 active = CheckConditions(sMenu->currentTab, itemId);

    if (!active)
    {
        u8 color[3] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG, TEXT_COLOR_OPTIONS_GRAY_SHADOW };
        AddTextPrinterParameterized4(windowId, FONT_NORMAL, 8, y + 1, 0, 0, color, TEXT_SKIP_DRAW, items[itemId].name);
    }

    // Frame type is special — no choiceNames array
    if (sMenu->currentTab == TAB_MAIN && itemId == ITEM_MAIN_FRAMETYPE)
    {
        DrawFrameTypeChoice(*GetSelectionPtr(sMenu->currentTab, itemId), y, active);
        return;
    }

    if (items[itemId].numChoices == 0 || items[itemId].choiceNames == NULL)
        return;

    u8 sel = *GetSelectionPtr(sMenu->currentTab, itemId);
    if (sel >= items[itemId].numChoices)
        sel = 0;

    switch (items[itemId].numChoices)
    {
    case 2:
        DrawChoices_Two(items[itemId].choiceNames, sel, y, active);
        break;
    case 3:
        DrawChoices_Three(items[itemId].choiceNames, sel, y, active);
        break;
    case 4:
        DrawChoices_Four(items[itemId].choiceNames, sel, y, active);
        break;
    }
}

// =============================================================================
// List menu init / teardown
// =============================================================================

static void InitListMenu(void)
{
    struct ListMenuTemplate template;
    const struct OptionMenuItem *items = GetCurrentTabItems();
    u8 count = GetCurrentTabItemCount();

    for (u8 i = 0; i < count; i++)
    {
        sListItems[i].name = items[i].name;
        sListItems[i].id = i;
    }

    memset(&template, 0, sizeof(template));
    template.items = sListItems;
    template.moveCursorFunc = OptionMenu_MoveCursorFunc;
    template.itemPrintFunc = OptionMenu_ItemPrintFunc;
    template.totalItems = count;
    template.maxShowed = (count < ITEMS_VISIBLE) ? count : ITEMS_VISIBLE;
    template.windowId = WIN_OPTIONS;
    template.header_X = 0;
    template.item_X = 8;
    template.cursor_X = 0;
    template.upText_Y = 1;
    template.cursorPal = TEXT_COLOR_OPTIONS_ORANGE_FG;
    template.fillValue = 1;
    template.cursorShadowPal = TEXT_COLOR_OPTIONS_ORANGE_SHADOW;
    template.lettersSpacing = 0;
    template.itemVerticalPadding = 0;
    template.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    template.fontId = FONT_NORMAL;
    template.cursorKind = CURSOR_INVISIBLE;

    sMenu->listTaskId = ListMenuInit(&template,
        sMenu->scrollOffset[sMenu->currentTab],
        sMenu->selectedRow[sMenu->currentTab]);

    HighlightRow();

    if (count > ITEMS_VISIBLE)
    {
        sMenu->arrowTaskId = AddScrollIndicatorArrowPairParameterized(
            SCROLL_ARROW_UP, 240 / 2, 20, 110,
            count - ITEMS_VISIBLE,
            110, 110,
            &sMenu->scrollOffset[sMenu->currentTab]);
    }
    else
    {
        sMenu->arrowTaskId = TASK_NONE;
    }
}

static void DestroyCurrentListMenu(void)
{
    DestroyListMenuTask(sMenu->listTaskId,
        &sMenu->scrollOffset[sMenu->currentTab],
        &sMenu->selectedRow[sMenu->currentTab]);

    if (sMenu->arrowTaskId != TASK_NONE)
    {
        RemoveScrollIndicatorArrowPair(sMenu->arrowTaskId);
        sMenu->arrowTaskId = TASK_NONE;
    }
}

// =============================================================================
// Drawing
// =============================================================================

static void DrawTopBar(void)
{
    const u8 color[3] = { TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_OPTIONS_WHITE, TEXT_COLOR_OPTIONS_GRAY_FG };
    const u8 *tabName = sTabs[sMenu->currentTab].tabName;
    int width = GetStringWidth(0L, tabName, 0) / 2;

    FillWindowPixelBuffer(WIN_TOPBAR, PIXEL_FILL(15));

    AddTextPrinterParameterized3(WIN_TOPBAR, 0, 120 - width, 1, color, 0, tabName);

    if (sMenu->currentTab > 0)
        AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 2, 1, color, 0, sText_TopBar_Left);
    if (sMenu->currentTab < TAB_COUNT - 1)
        AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 222, 1, color, 0, sText_TopBar_Right);

    PutWindowTilemap(WIN_TOPBAR);
    CopyWindowToVram(WIN_TOPBAR, COPYWIN_FULL);
}

static void DrawDescription(void)
{
    const u8 color[3] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_OPTIONS_GRAY_FG, TEXT_COLOR_OPTIONS_GRAY_SHADOW };
    u16 scrollOffset, selectedRow;
    ListMenuGetScrollAndRow(sMenu->listTaskId, &scrollOffset, &selectedRow);
    u8 itemIndex = scrollOffset + selectedRow;

    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));

    const struct OptionMenuItem *items = GetCurrentTabItems();
    if (itemIndex < GetCurrentTabItemCount() && items[itemIndex].descriptions != NULL)
    {
        u8 sel = *GetSelectionPtr(sMenu->currentTab, itemIndex);
        u8 descIndex = sel;

        // For items with only 1 description (text speed, frame type), always use index 0
        if (items[itemIndex].numChoices <= 1 || (sMenu->currentTab == TAB_MAIN && itemIndex == ITEM_MAIN_FRAMETYPE))
            descIndex = 0;
        else if (descIndex >= items[itemIndex].numChoices)
            descIndex = 0;

        const u8 *desc = items[itemIndex].descriptions[descIndex];
        if (desc != NULL)
        {
            AddTextPrinterParameterized4(WIN_DESCRIPTION, FONT_NORMAL,
                8, 1, 0, 0, color, TEXT_SKIP_DRAW, desc);
        }
    }

    CopyWindowToVram(WIN_DESCRIPTION, COPYWIN_FULL);
}

static void DrawBgFrames(void)
{
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  2, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  3,  1, 16,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  3,  1, 16,  7);

    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 13,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 13, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 13,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2, 14, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28, 14,  1,  1,  7);

    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}

// =============================================================================
// Tab switching
// =============================================================================

static void SwitchTab(s8 direction)
{
    s8 newTab = sMenu->currentTab + direction;

    if (newTab < 0 || newTab >= TAB_COUNT)
        return;

    DestroyCurrentListMenu();
    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));

    sMenu->currentTab = newTab;

    InitListMenu();
    DrawTopBar();
    DrawDescription();
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);

    PlaySE(SE_SELECT);
}

// =============================================================================
// Input: cycle selection left/right
// =============================================================================

static void ProcessLeftRight(void)
{
    u16 scrollOffset, selectedRow;
    ListMenuGetScrollAndRow(sMenu->listTaskId, &scrollOffset, &selectedRow);
    u8 itemIndex = scrollOffset + selectedRow;

    const struct OptionMenuItem *items = GetCurrentTabItems();
    if (itemIndex >= GetCurrentTabItemCount())
        return;
    if (!CheckConditions(sMenu->currentTab, itemIndex))
        return;

    // Frame type special handling
    if (sMenu->currentTab == TAB_MAIN && itemIndex == ITEM_MAIN_FRAMETYPE)
    {
        u8 *sel = GetSelectionPtr(sMenu->currentTab, itemIndex);
        u8 prev = *sel;

        if (JOY_NEW(DPAD_RIGHT))
        {
            if (*sel < WINDOW_FRAMES_COUNT - 1)
                (*sel)++;
            else
                *sel = 0;
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            if (*sel > 0)
                (*sel)--;
            else
                *sel = WINDOW_FRAMES_COUNT - 1;
        }

        if (*sel != prev)
        {
            LoadBgTiles(1, GetWindowFrameTilesPal(*sel)->tiles, 0x120, 0x1A2);
            LoadPalette(GetWindowFrameTilesPal(*sel)->pal, 0x70, 0x20);
            PlaySE(SE_SELECT);
            RedrawListMenu(sMenu->listTaskId);
            HighlightRow();
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
            DrawDescription();
        }
        return;
    }

    if (items[itemIndex].numChoices == 0)
        return;

    u8 *sel = GetSelectionPtr(sMenu->currentTab, itemIndex);
    u8 prev = *sel;

    if (JOY_NEW(DPAD_RIGHT))
    {
        if (*sel < items[itemIndex].numChoices - 1)
            (*sel)++;
        else
            *sel = 0;
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (*sel > 0)
            (*sel)--;
        else
            *sel = items[itemIndex].numChoices - 1;
    }

    if (*sel != prev)
    {
        // Sound setting side effect
        if (sMenu->currentTab == TAB_SOUND && itemIndex == ITEM_SOUND_SOUND)
            SetPokemonCryStereo(*sel);

        // Quick Run → disable LR Run when not applicable
        if (sMenu->currentTab == TAB_BATTLE && itemIndex == ITEM_BATTLE_RUN_TYPE)
        {
            if (*sel == 0 || *sel == 2)
                *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_LR_RUN) = 1; // OFF
        }

        PlaySE(SE_SELECT);
        RedrawListMenu(sMenu->listTaskId);
        HighlightRow();
        CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        DrawDescription();
    }
}

// =============================================================================
// Tasks
// =============================================================================

static void Task_FadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ProcessInput;
}

static void Task_ProcessInput(u8 taskId)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        ProcessLeftRight();
        return;
    }

    if (JOY_NEW(L_BUTTON))
    {
        SwitchTab(-1);
        return;
    }

    if (JOY_NEW(R_BUTTON))
    {
        SwitchTab(+1);
        return;
    }

    if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_Save;
        return;
    }

    ListMenu_ProcessInput(sMenu->listTaskId);
}

static void Task_Save(u8 taskId)
{
    // SaveBlock2 — original options
    gSaveBlock2Ptr->optionsTextSpeed        = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_TEXTSPEED);
    gSaveBlock2Ptr->optionsBattleSceneOff   = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_BATTLESCENE);
    gSaveBlock2Ptr->optionsBattleStyle      = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_BATTLESTYLE);
    gSaveBlock2Ptr->optionsButtonMode       = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_BUTTONMODE);
    gSaveBlock2Ptr->optionsWindowFrameType  = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FRAMETYPE);
    gSaveBlock2Ptr->optionsSound            = *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_SOUND);

    // SaveBlock3 — HnS options plus
    struct ChallengeSettings *cs = &gSaveBlock3Ptr->challengeSettings;
    cs->followerEnable     = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FOLLOWER);
    cs->followerLargeEnable= *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_LARGE_FOLLOWER);
    cs->autoRun            = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_AUTORUN);
    cs->autorunSurf        = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_AUTORUN_SURF);
    cs->fishing            = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FISHING);
    cs->evenFasterJoy      = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FASTER_JOY);
    if (cs->evenFasterJoy == 0)
        FlagSet(FLAG_EVEN_FASTER_JOY);
    else
        FlagClear(FLAG_EVEN_FASTER_JOY);
    cs->unitSystem         = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_UNIT_TYPE);
    cs->disableMatchCall   = *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_MATCHCALL);

    cs->fastIntro          = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_FAST_INTRO);
    cs->fastBattle         = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_FAST_BATTLES);
    cs->newBackgrounds     = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_NEW_BACKGROUNDS);
    cs->newBattleUI        = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_NEW_BATTLEUI);
    cs->ballPrompt         = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_BALL_PROMPT);
    cs->lrToRun            = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_LR_RUN);
    cs->runType            = *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_RUN_TYPE);

    cs->musicOnOff         = *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_MUSIC);
    cs->bikeMusic          = *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_BIKE_MUSIC);
    cs->surfMusic          = *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_SURF_MUSIC);

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_FadeOut;
}

static void Task_FadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyCurrentListMenu();
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        FREE_AND_SET_NULL(sMenu);
        SetMainCallback2(gMain.savedCallback);
    }
}

// =============================================================================
// Init
// =============================================================================

void CB2_InitOptionMenu(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        ResetBgPositions();
        InitWindows(sWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, 224));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(24, 104));
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN1_BG0 | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sBgPal, 0, sizeof(sBgPal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sTextPal, 16, sizeof(sTextPal));
        gMain.state++;
        break;
    case 6:
    {
        sMenu = AllocZeroed(sizeof(struct OptionMenuState));
        sMenu->currentTab = TAB_MAIN;
        sMenu->arrowTaskId = TASK_NONE;

        // Load SaveBlock2 options
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_TEXTSPEED)    = gSaveBlock2Ptr->optionsTextSpeed;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_BATTLESCENE)  = gSaveBlock2Ptr->optionsBattleSceneOff;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_BATTLESTYLE)  = gSaveBlock2Ptr->optionsBattleStyle;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_BUTTONMODE)   = gSaveBlock2Ptr->optionsButtonMode;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FRAMETYPE)    = gSaveBlock2Ptr->optionsWindowFrameType;
        *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_SOUND)      = gSaveBlock2Ptr->optionsSound;

        // Load SaveBlock3 HnS options
        struct ChallengeSettings *cs = &gSaveBlock3Ptr->challengeSettings;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FOLLOWER)       = cs->followerEnable;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_LARGE_FOLLOWER) = cs->followerLargeEnable;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_AUTORUN)        = cs->autoRun;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_AUTORUN_SURF)   = cs->autorunSurf;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FISHING)        = cs->fishing;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_FASTER_JOY)     = cs->evenFasterJoy;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_UNIT_TYPE)      = cs->unitSystem;
        *GetSelectionPtr(TAB_MAIN, ITEM_MAIN_MATCHCALL)    = cs->disableMatchCall;

        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_FAST_INTRO)      = cs->fastIntro;
        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_FAST_BATTLES)    = cs->fastBattle;
        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_NEW_BACKGROUNDS) = cs->newBackgrounds;
        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_NEW_BATTLEUI)    = cs->newBattleUI;
        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_BALL_PROMPT)     = cs->ballPrompt;
        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_LR_RUN)          = cs->lrToRun;
        *GetSelectionPtr(TAB_BATTLE, ITEM_BATTLE_RUN_TYPE)        = cs->runType;

        *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_MUSIC)      = cs->musicOnOff;
        *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_BIKE_MUSIC) = cs->bikeMusic;
        *GetSelectionPtr(TAB_SOUND, ITEM_SOUND_SURF_MUSIC) = cs->surfMusic;

        gMain.state++;
        break;
    }
    case 7:
        PutWindowTilemap(WIN_TOPBAR);
        PutWindowTilemap(WIN_OPTIONS);
        PutWindowTilemap(WIN_DESCRIPTION);
        DrawTopBar();
        DrawBgFrames();
        InitListMenu();
        DrawDescription();
        CopyWindowToVram(WIN_TOPBAR, COPYWIN_FULL);
        CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
        CopyWindowToVram(WIN_DESCRIPTION, COPYWIN_FULL);
        gMain.state++;
        break;
    case 8:
        CreateTask(Task_FadeIn, 0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        break;
    }
}
