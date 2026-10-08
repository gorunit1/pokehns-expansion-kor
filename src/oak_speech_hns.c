#include "global.h"
#include "trainer_pokemon_sprites.h"
#include "bg.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "field_effect.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "international_string_util.h"
#include "main.h"
#include "main_menu.h"
#include "menu.h"
#include "list_menu.h"
#include "naming_screen.h"
#include "oak_speech_hns.h"
#include "challenge_menu.h"
#include "new_game.h"
#include "overworld.h"
#include "palette.h"
#include "pokeball.h"
#include "pokemon.h"
#include "random.h"
#include "save.h"
#include "scanline_effect.h"
#include "rtc.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"

#if IS_HNS

extern const u8 gText_Oak_LateNight[];
extern const u8 gText_Oak_Morning[];
extern const u8 gText_Oak_Day[];
extern const u8 gText_Oak_Evening[];
extern const u8 gText_Oak_Night[];
extern const u8 gText_Oak_Welcome[];
extern const u8 gText_Oak_MainSpeech[];
extern const u8 gText_Oak_AndYouAre[];
extern const u8 gText_Oak_BoyOrGirl[];
extern const u8 gText_Oak_WhatChallenge[];
extern const u8 gText_Oak_ChallengeSelected[];
extern const u8 gText_Oak_WhatsYourName[];
extern const u8 gText_Oak_SoItsPlayer[];
extern const u8 gText_Oak_YourePlayer[];
extern const u8 gText_Oak_AreYouReady[];

// SoulGold 스타일 동시 출력 좌표 및 밝기 설정
#define NEW_GAME_SPEECH_BOY_X      145
#define NEW_GAME_SPEECH_GIRL_X     205
#define NEW_GAME_SPEECH_PLAYER_Y   80
#define NEW_GAME_SPEECH_GENDER_DIM 8

static EWRAM_DATA bool8 sStartedPokeBallTask = 0;
static EWRAM_DATA u8 sOakSpeechPicTilemap[BG_SCREEN_SIZE / 2] = {0};

static u8 sHnsSpeechMainTaskId;

// Static function declarations
static void Task_NewGameHnsSpeech_TimeIntro_Init(u8); // 추가
static void Task_NewGameHnsSpeech_TimeIntro_Wait(u8); // 추가
static void Task_NewGameHnsSpeech_TimeIntro_FadeInOak(u8 taskId); // 추가
static void Task_NewGameHnsSpeech_Init(u8);
static void InitHnsSpeechScene(u8);
static void AddHnsSpeechObjects(u8);
static void LoadOakSpeechSceneGfx(void);
static void NewGameHnsSpeech_StartFadeInTarget1OutTarget2(u8, u8);
static void NewGameHnsSpeech_StartFadePlatformOut(u8, u8);
static void NewGameHnsSpeech_ClearWindow(u8);
static void Task_NewGameHnsSpeech_ThisIsAPokemon(u8);
static void Task_NewGameHnsSpeech_MainSpeech(u8);
static void NewGameHnsSpeech_WaitForThisIsPokemonText(struct TextPrinterTemplate *, u16);
static void Task_NewGameHnsSpeech_AndYouAre(u8);
static void Task_NewGameHnsSpeechSub_WaitForMon(u8);
static void Task_NewGameHnsSpeech_StartProfessorMonPlatformFade(u8);
static void NewGameHnsSpeech_StartFadeOutTarget1InTarget2(u8, u8);
static void NewGameHnsSpeech_StartFadePlatformIn(u8, u8);
static void Task_NewGameHnsSpeech_SlidePlatformAway(u8);
static void Task_NewGameHnsSpeech_StartPlayerFadeIn(u8);
static void Task_NewGameHnsSpeech_WaitForPlayerFadeIn(u8);
static void Task_NewGameHnsSpeech_BoyOrGirl(u8);
static void Task_NewGameHnsSpeech_WaitToShowGenderMenu(u8);
static void Task_NewGameHnsSpeech_ChooseGender(u8);
static void NewGameHnsSpeech_ShowGenderMenu(void);
static void NewGameHnsSpeech_ClearGenderWindow(u8, u8);
static void Task_NewGameHnsSpeech_ChallengeDisclaimer(u8);
static void Task_NewGameHnsSpeech_WaitDisclaimerText(u8);
static void Task_NewGameHnsSpeech_WaitPressDisclaimer(u8);
static void Task_NewGameHnsSpeech_ChallengeMenu(u8);
static void Task_NewGameHnsSpeech_WhatsYourName(u8);

// 동시 출력을 위한 새로운 함수들
static void NewGameHnsSpeech_UpdateGenderSprites(u8);
static void NewGameHnsSpeech_StartGenderConfirmation(u8);
static void Task_NewGameHnsSpeech_WaitForUnselectedGenderFadeOut(u8);
static void NewGameHnsSpeech_HideUnselectedGenderSprite(u8);
static void Task_NewGameHnsSpeech_WaitForPlayerFadeOutToGenderChoice(u8);

static void Task_NewGameHnsSpeech_WaitForWhatsYourNameToPrint(u8);
static void Task_NewGameHnsSpeech_WaitPressBeforeNameChoice(u8);
static void Task_NewGameHnsSpeech_StartNamingScreen(u8);
static void CB2_NewGameHnsSpeech_ReturnFromNamingScreen(void);
static void CB2_NewGameHnsSpeech_ReturnFromChallengeMenu(void);
static void Task_NewGameHnsSpeech_FadeOutToChallengeMenu(u8);
static void Task_NewGameHnsSpeech_ReturnFromChallengeMenuShowTextbox(u8);
static void Task_NewGameHnsSpeech_WaitForTextAfterChallengeMenu(u8);
static void Task_NewGameHnsSpeech_ReturnFromNamingScreenShowTextbox(u8);
static void Task_NewGameHnsSpeech_SoItsPlayerName(u8);
static void Task_NewGameHnsSpeech_CreateNameYesNo(u8);
static void Task_NewGameHnsSpeech_ProcessNameYesNoMenu(u8);
static void Task_NewGameHnsSpeech_SlidePlatformAway2(u8);
static void Task_NewGameHnsSpeech_ReshowProfessorMon(u8);
static void Task_NewGameHnsSpeech_WaitForYourePlayerText(u8);
static void Task_NewGameHnsSpeech_WaitForAreYouReadyText(u8);
static void Task_NewGameHnsSpeech_FadeInPlayer(u8);
static void Task_NewGameHnsSpeech_ShrinkPlayer(u8);
static void Task_NewGameHnsSpeech_WaitForPlayerShrink(u8);
static void Task_NewGameHnsSpeech_FadePlayerToWhite(u8);
static void Task_NewGameHnsSpeech_Cleanup(u8);
static void Task_NewGameHnsSpeechSub_InitPokeBall(u8);
static void Task_NewGameHnsSpeech_FadeOutTarget1InTarget2(u8);
static void Task_NewGameHnsSpeech_FadeInTarget1OutTarget2(u8);
static void Task_NewGameHnsSpeech_FadePlatformIn(u8);
static void Task_NewGameHnsSpeech_FadePlatformOut(u8);
static void SpriteCB_Null(struct Sprite *);
static void SpriteCB_MovePlayerDownWhileShrinking(struct Sprite *);
static u8 NewGameHnsSpeech_CreateMonSprite(u8, u8);
static s8 NewGameHnsSpeech_ProcessGenderMenuInput(void);
static void NewGameHnsSpeech_ClearGenderWindowTilemap(u8, u8, u8, u8, u8, u8);
static void LoadMainMenuWindowFrameTiles(u8, u16);
static void DrawMainMenuWindowBorder(const struct WindowTemplate *, u16);

// .rodata

static const u16 sOakSpeechBgPal[] = INCBIN_U16("graphics/oak_speech/oak_speech_bg.gbapal");
static const u32 sOakSpeechBgGfx[] = INCBIN_U32("graphics/oak_speech/oak_speech_bg.4bpp.smol");
static const u32 sOakSpeechBgMap[] = INCBIN_U32("graphics/oak_speech/oak_speech_bg.bin.smolTM");
static const u16 sOakSpeechOakPal[] = INCBIN_U16("graphics/oak_speech/oak/pal.gbapal");
static const u32 sOakSpeechOakGfx[] = INCBIN_U32("graphics/oak_speech/oak/pic.8bpp.smol");

#define OAK_SPEECH_PIC_WIDTH       8
#define OAK_SPEECH_PIC_HEIGHT      12
#define OAK_SPEECH_PIC_TILE_COUNT  (OAK_SPEECH_PIC_WIDTH * OAK_SPEECH_PIC_HEIGHT)
#define OAK_SPEECH_BLANK_TILE      OAK_SPEECH_PIC_TILE_COUNT
#define OAK_SPEECH_BG_PAL_NUM      3
#define OAK_SPEECH_BG_BACKDROP     14
#define OAK_SPEECH_BG_MAP_SIZE     (32 * 20)

static const struct BgTemplate sOakSpeechPicBgTemplate =
{
    .bg = 2,
    .charBaseIndex = 1,
    .mapBaseIndex = 29,
    .screenSize = 1,
    .paletteMode = 1,
    .priority = 0,
    .baseTile = 0
};

static const struct WindowTemplate sNewGameHnsSpeechTextWindows[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26, // HnS message box frame spans left - 2 .. left + width + 1 (src/menu.c)
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1
    },
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 5,
        .width = 6,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x6D
    },
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 2,
        .width = 9,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 0x85
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sHnsBgTemplate = {
    .bg = 0,
    .charBaseIndex = 3,
    .mapBaseIndex = 30,
    .screenSize = 0,
    .paletteMode = 0,
    .priority = 0,
    .baseTile = 0
};

static const struct BgTemplate sMainMenuBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 7,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    }
};

static const union AffineAnimCmd sSpriteAffineAnim_PlayerShrink[] = {
    AFFINEANIMCMD_FRAME(-2, -2, 0, 0x30),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sSpriteAffineAnimTable_PlayerShrink[] =
{
    sSpriteAffineAnim_PlayerShrink
};

static const struct MenuAction sMenuActions_Gender[] = {
    {gText_Boy, {NULL}},
    {gText_Girl, {NULL}}
};

static const u8 *const sMalePresetNames[] = {
    COMPOUND_STRING("심향"),
};

static const u8 *const sFemalePresetNames[] = {
    COMPOUND_STRING("크리스"),
};

#define NUM_PRESET_NAMES min(ARRAY_COUNT(sMalePresetNames), ARRAY_COUNT(sFemalePresetNames))

#define MAIN_MENU_BORDER_TILE   0x1D5
#define HNS_MENU_BASE_TILE_NUM 0xF3
#define HNS_DLG_BASE_TILE_NUM 0xFC

// Task data defines
#define tTimer        data[0]
#define tBG1HOFS      data[1]
#define tPlayerSpriteId   data[2]
#define tPlayerGender     data[3]
#define tMonSpriteId      data[5]
#define tGoldSpriteId     data[6]
#define tKrisSpriteId     data[7]
#define tIsDoneFadingSprites data[15]

static void CB2_HnsMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_HnsMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void StartNewGameSceneHns(void)
{
    u8 taskId;

    // A new game started over an existing save inherits that save's flags until
    // ClearSav1 runs at the end of the speech, so the intro would play GBS tracks.
    // Clearing it here also means GBS never drives NR50 during the speech; restore
    // the PSG master volume in case it was left attenuated on the way in.
    FlagClear(FLAG_SYS_GBS_ENABLED);
    RestorePSGMasterVolume();

    SetVBlankCallback(NULL);
    ResetTasks();
    taskId = CreateTask(Task_NewGameHnsSpeech_Init, 0);
    gTasks[taskId].tBG1HOFS = 0;
    gTasks[taskId].tPlayerSpriteId = SPRITE_NONE;
    gTasks[taskId].data[3] = 0xFF;
    gTasks[taskId].tTimer = 80;

    SetVBlankCallback(VBlankCB_HnsMenu);
    SetMainCallback2(CB2_HnsMenu);
}

static void Task_NewGameHnsSpeech_Init(u8 taskId)
{
    InitHnsSpeechScene(taskId);
    sStartedPokeBallTask = FALSE;
    PlayBGM(MUS_HG_NEW_GAME);
    gTasks[taskId].func = Task_NewGameHnsSpeech_ChallengeDisclaimer;
}

// Initial setup and challenge-menu return both precede gender/name selection.
static void InitHnsSpeechScene(u8 taskId)
{
    SetVBlankCallback(NULL);
    ResetBgsAndClearDma3BusyFlags(0);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetPaletteFade();
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    
    InitBgsFromTemplates(0, sMainMenuBgTemplates, ARRAY_COUNT(sMainMenuBgTemplates));
    InitBgFromTemplate(&sHnsBgTemplate);
    InitBgFromTemplate(&sOakSpeechPicBgTemplate);
    SetBgMode(1);
    SetBgTilemapBuffer(2, sOakSpeechPicTilemap);
    SetBgAffine(2, 0, 0, 0, 0, 256, 256, 0);

    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);

    LoadOakSpeechSceneGfx();

    ScanlineEffect_Stop();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetAllPicSprites();
    AddHnsSpeechObjects(taskId);

    gTasks[taskId].tBG1HOFS = 0;
    gTasks[taskId].tPlayerSpriteId = SPRITE_NONE;
    gTasks[taskId].tPlayerGender = 0xFF;
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);

    InitWindows(sNewGameHnsSpeechTextWindows);
    LoadMainMenuWindowFrameTiles(0, HNS_MENU_BASE_TILE_NUM);
    LoadMessageBoxGfx(0, HNS_DLG_BASE_TILE_NUM, BG_PLTT_ID(15));
    DrawDialogFrameWithCustomTile(0, TRUE, HNS_DLG_BASE_TILE_NUM);
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);

    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    ShowBg(0);
    ShowBg(1);
    HideBg(2);
    SetVBlankCallback(VBlankCB_HnsMenu);
}

static void LoadOakSpeechSceneGfx(void)
{
    u32 i;
    u16 *bgTilemap = (u16 *)BG_SCREEN_ADDR(7);

    DecompressDataWithHeaderVram(sOakSpeechBgGfx, (void *)VRAM);
    DecompressDataWithHeaderVram(sOakSpeechBgMap, bgTilemap);
    DecompressDataWithHeaderVram(sOakSpeechOakGfx, (void *)BG_CHAR_ADDR(1));
    DmaFill32(3, 0, (void *)(BG_CHAR_ADDR(1) + OAK_SPEECH_BLANK_TILE * TILE_SIZE_8BPP), TILE_SIZE_8BPP);
    LoadPalette(sOakSpeechOakPal, BG_PLTT_ID(0), sizeof(sOakSpeechOakPal));
    LoadPalette(sOakSpeechBgPal, BG_PLTT_ID(OAK_SPEECH_BG_PAL_NUM), sizeof(sOakSpeechBgPal));
    LoadPalette(&sOakSpeechBgPal[OAK_SPEECH_BG_BACKDROP], BG_PLTT_ID(0), PLTT_SIZEOF(1));

    for (i = 0; i < OAK_SPEECH_BG_MAP_SIZE; i++)
        bgTilemap[i] |= OAK_SPEECH_BG_PAL_NUM << 12;

    for (i = 0; i < ARRAY_COUNT(sOakSpeechPicTilemap); i++)
        sOakSpeechPicTilemap[i] = OAK_SPEECH_BLANK_TILE;
    for (i = 0; i < OAK_SPEECH_PIC_TILE_COUNT; i++)
        sOakSpeechPicTilemap[(i / OAK_SPEECH_PIC_WIDTH + 2) * 32 + (i % OAK_SPEECH_PIC_WIDTH) + 11] = i;
    CopyBgTilemapBufferToVram(2);
}

static void Task_NewGameHnsSpeech_TimeIntro_Init(u8 taskId)
{
    const u8 *introText;

    if (gPaletteFade.active)
        return;

    // Only the RTC introduction uses a black scene; keep the textbox visible.
    HideBg(1);
    HideBg(2);
    gPlttBufferUnfaded[0] = RGB_BLACK;
    gPlttBufferFaded[0] = RGB_BLACK;

    // 현재 기기의 시간(RTC) 읽어오기
    RtcCalcLocalTime();

    // 5가지 시간대에 따른 대사 완벽 분기
    if (gLocalTime.hours >= 4 && gLocalTime.hours < 10) // 아침
        introText = gText_Oak_Morning;
    else if (gLocalTime.hours >= 10 && gLocalTime.hours < 17) // 낮
        introText = gText_Oak_Day;
    else if (gLocalTime.hours >= 17 && gLocalTime.hours < 20) // 저녁
        introText = gText_Oak_Evening;
    else if (gLocalTime.hours >= 20 && gLocalTime.hours <= 23) // 밤
        introText = gText_Oak_Night;
    else // 심야
        introText = gText_Oak_LateNight;

    // 대사 출력
    NewGameHnsSpeech_ClearWindow(0);
    StringExpandPlaceholders(gStringVar4, introText);
    AddTextPrinterForMessage(TRUE);

    gTasks[taskId].func = Task_NewGameHnsSpeech_TimeIntro_Wait;
}

static void Task_NewGameHnsSpeech_TimeIntro_Wait(u8 taskId)
{
    // A버튼 입력을 대기 (\p 기호로 인해 한 번만 누르면 자연스럽게 넘어갑니다)
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        // 1. 텍스트 박스는 끄지 않고 유지! 덮어씌웠던 검정색을 풀고, 원래 배경색(주황색) 복구 준비
        gPlttBufferUnfaded[0] = sOakSpeechBgPal[OAK_SPEECH_BG_BACKDROP];

        // 2. 숨겨뒀던 오박사와 배경 다시 켜기 (화면은 페이드 때문에 아직 어두움)
        ShowBg(1);
        ShowBg(2);

        PlayBGM(MUS_HG_ROUTE30); // 오박사 등장 브금 재생 시작!

        // 3. 텍스트 박스(15번 팔레트)를 제외한 나머지 모든 화면을 부드럽게 페이드인!
        // PALETTES_ALL에서 15번 팔레트를 비트 연산으로 제외시켜 텍스트만 둥둥 떠 있게 만듭니다.
        BeginNormalPaletteFade(PALETTES_ALL & ~(1 << 15), 0, 16, 0, RGB_BLACK);

        gTasks[taskId].func = Task_NewGameHnsSpeech_TimeIntro_FadeInOak;
    }
}

static void Task_NewGameHnsSpeech_TimeIntro_FadeInOak(u8 taskId)
{
    // 배경과 오박사 페이드인이 완전히 끝나면 실행
    if (!gPaletteFade.active)
    {
        // 유지되고 있던 시간대 대사를 이제 지우고, 새로운 오박사 환영 대사 출력
        NewGameHnsSpeech_ClearWindow(0);
        StringExpandPlaceholders(gStringVar4, gText_Oak_Welcome);
        AddTextPrinterForMessage(TRUE);
        
        // 원래 인트로 본편으로 자연스럽게 연결
        gTasks[taskId].func = Task_NewGameHnsSpeech_ThisIsAPokemon;
    }
}

static void Task_NewGameHnsSpeech_ThisIsAPokemon(u8 taskId)
{
    if (!gPaletteFade.active && !RunTextPrintersAndIsPrinter0Active())
    {
        gTasks[taskId].func = Task_NewGameHnsSpeech_MainSpeech;
        StringExpandPlaceholders(gStringVar4, gText_ThisIsAPokemon);
        AddTextPrinterWithCallbackForMessage(TRUE, NewGameHnsSpeech_WaitForThisIsPokemonText);
        sHnsSpeechMainTaskId = taskId;
    }
}

static void Task_NewGameHnsSpeech_MainSpeech(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        StringExpandPlaceholders(gStringVar4, gText_Oak_MainSpeech);
        AddTextPrinterForMessage(TRUE);
        gTasks[taskId].func = Task_NewGameHnsSpeech_AndYouAre;
    }
}

#define tState data[0]

static void Task_NewGameHnsSpeechSub_InitPokeBall(u8 taskId)
{
    u8 spriteId = gTasks[sHnsSpeechMainTaskId].tMonSpriteId;

    gSprites[spriteId].x = 80;
    gSprites[spriteId].y = 92;
    gSprites[spriteId].invisible = FALSE;
    gSprites[spriteId].data[0] = 0;

    CreatePokeballSpriteToReleaseMon(spriteId, gSprites[spriteId].oam.paletteNum, 104, 66, 0, 0, 32, PALETTES_BG, SPECIES_MARILL);
    gTasks[taskId].func = Task_NewGameHnsSpeechSub_WaitForMon;
    gTasks[sHnsSpeechMainTaskId].tTimer = 0;
}

static void Task_NewGameHnsSpeechSub_WaitForMon(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite *sprite = &gSprites[gTasks[sHnsSpeechMainTaskId].tMonSpriteId];

    switch (tState)
    {
    case 0:
        if (sprite->callback != SpriteCallbackDummy)
            return;
        sprite->oam.affineMode = ST_OAM_AFFINE_OFF;
        break;
    case 1:
        if (gTasks[sHnsSpeechMainTaskId].tTimer >= 96)
        {
            DestroyTask(taskId);
            if (gTasks[sHnsSpeechMainTaskId].tTimer < 0x4000)
                gTasks[sHnsSpeechMainTaskId].tTimer++;
        }
        return;
    }
    tState++;
    if (gTasks[sHnsSpeechMainTaskId].tTimer < 0x4000)
        gTasks[sHnsSpeechMainTaskId].tTimer++;
}

#undef tState

static void Task_NewGameHnsSpeech_AndYouAre(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        sStartedPokeBallTask = FALSE;
        StringExpandPlaceholders(gStringVar4, gText_Oak_AndYouAre);
        AddTextPrinterForMessage(TRUE);
        gTasks[taskId].func = Task_NewGameHnsSpeech_StartProfessorMonPlatformFade;
    }
}

static void Task_NewGameHnsSpeech_StartProfessorMonPlatformFade(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        gSprites[gTasks[taskId].tMonSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;
        NewGameHnsSpeech_StartFadeOutTarget1InTarget2(taskId, 2);
        NewGameHnsSpeech_StartFadePlatformIn(taskId, 1);
        gTasks[taskId].tTimer = 64;
        gTasks[taskId].func = Task_NewGameHnsSpeech_SlidePlatformAway;
    }
}

static void Task_NewGameHnsSpeech_SlidePlatformAway(u8 taskId)
{
    if (gTasks[taskId].tBG1HOFS != -60)
    {
        gTasks[taskId].tBG1HOFS -= 2;
        SetGpuReg(REG_OFFSET_BG1HOFS, gTasks[taskId].tBG1HOFS);
    }
    else
    {
        gTasks[taskId].tBG1HOFS = -60;
        gTasks[taskId].func = Task_NewGameHnsSpeech_StartPlayerFadeIn;
    }
}

// 두 주인공이 동시에 나타나게 하는 코드
static void Task_NewGameHnsSpeech_StartPlayerFadeIn(u8 taskId)
{
    if (gTasks[taskId].tIsDoneFadingSprites)
    {
        HideBg(2);
        gSprites[gTasks[taskId].tMonSpriteId].invisible = TRUE;
        if (gTasks[taskId].tTimer)
        {
            gTasks[taskId].tTimer--;
        }
        else
        {
            u8 goldSpriteId = gTasks[taskId].tGoldSpriteId;
            u8 krisSpriteId = gTasks[taskId].tKrisSpriteId;

            gSprites[goldSpriteId].x = NEW_GAME_SPEECH_BOY_X;
            gSprites[goldSpriteId].y = NEW_GAME_SPEECH_PLAYER_Y;
            gSprites[goldSpriteId].invisible = FALSE;
            gSprites[goldSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;

            gSprites[krisSpriteId].x = NEW_GAME_SPEECH_GIRL_X;
            gSprites[krisSpriteId].y = NEW_GAME_SPEECH_PLAYER_Y;
            gSprites[krisSpriteId].invisible = FALSE;
            gSprites[krisSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;

            gTasks[taskId].tPlayerSpriteId = goldSpriteId;
            gTasks[taskId].tPlayerGender = MALE;
            NewGameHnsSpeech_StartFadeInTarget1OutTarget2(taskId, 2);
            NewGameHnsSpeech_StartFadePlatformOut(taskId, 1);
            gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForPlayerFadeIn;
        }
    }
}

static void Task_NewGameHnsSpeech_WaitForPlayerFadeIn(u8 taskId)
{
    if (gTasks[taskId].tIsDoneFadingSprites)
    {
        gSprites[gTasks[taskId].tGoldSpriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
        gSprites[gTasks[taskId].tKrisSpriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
        NewGameHnsSpeech_UpdateGenderSprites(taskId);
        gTasks[taskId].func = Task_NewGameHnsSpeech_BoyOrGirl;
    }
}

static void Task_NewGameHnsSpeech_BoyOrGirl(u8 taskId)
{
    NewGameHnsSpeech_ClearWindow(0);
    StringExpandPlaceholders(gStringVar4, gText_Oak_BoyOrGirl);
    AddTextPrinterForMessage(TRUE);
    gTasks[taskId].func = Task_NewGameHnsSpeech_WaitToShowGenderMenu;
}

static void Task_NewGameHnsSpeech_WaitToShowGenderMenu(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        NewGameHnsSpeech_ShowGenderMenu();
        gTasks[taskId].func = Task_NewGameHnsSpeech_ChooseGender;
    }
}

// 선택된 성별에 맞춰 스프라이트 밝기 조절하기
static void NewGameHnsSpeech_UpdateGenderSprites(u8 taskId)
{
    u8 goldSpriteId = gTasks[taskId].tGoldSpriteId;
    u8 krisSpriteId = gTasks[taskId].tKrisSpriteId;
    bool8 isMale = gTasks[taskId].tPlayerGender == MALE;

    BlendPalettes(1u << (16 + gSprites[goldSpriteId].oam.paletteNum),
                  isMale ? 0 : NEW_GAME_SPEECH_GENDER_DIM, RGB_BLACK);
    BlendPalettes(1u << (16 + gSprites[krisSpriteId].oam.paletteNum),
                  isMale ? NEW_GAME_SPEECH_GENDER_DIM : 0, RGB_BLACK);
    gTasks[taskId].tPlayerSpriteId = isMale ? goldSpriteId : krisSpriteId;
}

// 방향키로 성별 고르기 및 확정
static void Task_NewGameHnsSpeech_ChooseGender(u8 taskId)
{
    enum Gender gender = NewGameHnsSpeech_ProcessGenderMenuInput();
    enum Gender highlightedGender;

    switch (gender)
    {
    case MALE:
    case FEMALE:
        PlaySE(SE_SELECT);
        gSaveBlock2Ptr->playerGender = gender;
        gTasks[taskId].tPlayerGender = gender;
        NewGameHnsSpeech_StartGenderConfirmation(taskId);
        return;
    default:
        break;
    }

    highlightedGender = Menu_GetCursorPos();
    if (highlightedGender != gTasks[taskId].tPlayerGender)
    {
        gTasks[taskId].tPlayerGender = highlightedGender;
        NewGameHnsSpeech_UpdateGenderSprites(taskId);
    }
}

// 선택 완료 시, 선택받지 못한 스프라이트를 지우기 위해 페이드아웃
static void NewGameHnsSpeech_StartGenderConfirmation(u8 taskId)
{
    u8 unselectedSpriteId;

    NewGameHnsSpeech_UpdateGenderSprites(taskId);
    if (gTasks[taskId].tPlayerGender == MALE)
        unselectedSpriteId = gTasks[taskId].tKrisSpriteId;
    else
        unselectedSpriteId = gTasks[taskId].tGoldSpriteId;

    gSprites[unselectedSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;
    NewGameHnsSpeech_StartFadeOutTarget1InTarget2(taskId, 2);
    gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForUnselectedGenderFadeOut;
}

// 선택한 주인공을 가운데로 스르륵 이동시키기
static void Task_NewGameHnsSpeech_WaitForUnselectedGenderFadeOut(u8 taskId)
{
    struct Sprite *selectedSprite = &gSprites[gTasks[taskId].tPlayerSpriteId];

    if (selectedSprite->x > 120)
    {
        selectedSprite->x -= 2;
        if (selectedSprite->x < 120) selectedSprite->x = 120;
    }
    else if (selectedSprite->x < 120)
    {
        selectedSprite->x += 2;
        if (selectedSprite->x > 120) selectedSprite->x = 120;
    }

    if (gTasks[taskId].tIsDoneFadingSprites)
    {
        NewGameHnsSpeech_HideUnselectedGenderSprite(taskId);
        NewGameHnsSpeech_ClearGenderWindow(1, 1);
        gTasks[taskId].func = Task_NewGameHnsSpeech_WhatsYourName;
    }
}

static void NewGameHnsSpeech_HideUnselectedGenderSprite(u8 taskId)
{
    u8 goldSpriteId = gTasks[taskId].tGoldSpriteId;
    u8 krisSpriteId = gTasks[taskId].tKrisSpriteId;
    bool8 isMale = gTasks[taskId].tPlayerGender == MALE;

    BlendPalettes(1u << (16 + gSprites[goldSpriteId].oam.paletteNum), 0, RGB_BLACK);
    BlendPalettes(1u << (16 + gSprites[krisSpriteId].oam.paletteNum), 0, RGB_BLACK);
    gSprites[goldSpriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
    gSprites[krisSpriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
    gSprites[goldSpriteId].invisible = !isMale;
    gSprites[krisSpriteId].invisible = isMale;
    gTasks[taskId].tPlayerSpriteId = isMale ? goldSpriteId : krisSpriteId;
    gSprites[gTasks[taskId].tPlayerSpriteId].x = 120;
}

static void Task_NewGameHnsSpeech_ChallengeDisclaimer(u8 taskId)
{
    static const u8 sText_Disclaimer[] = _("먼저 어느 정도의 도전을\n원하는지 알려주십시오\p{COLOR RED}챌린지 설정은 게임을 시작한 뒤\nPC에서 언제든 변경할 수 있습니다\p단, 게임 시작 이후에는\n너즐록, 랜더마이저, 난이도, 챌린지 설정의 강도를\l더 낮출 수만 있으며\l더 높일 수는 없습니다");
    if (gPaletteFade.active)
        return;

    NewGameHnsSpeech_ClearWindow(0);
    StringCopy(gStringVar4, sText_Disclaimer);
    AddTextPrinterWithCustomSpeedForMessage(FALSE, 2);
    gTasks[taskId].func = Task_NewGameHnsSpeech_WaitDisclaimerText;
}

static void Task_NewGameHnsSpeech_WaitDisclaimerText(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitPressDisclaimer;
}

static void Task_NewGameHnsSpeech_WaitPressDisclaimer(u8 taskId)
{
    if ((JOY_NEW(A_BUTTON)) || (JOY_NEW(B_BUTTON)))
        gTasks[taskId].func = Task_NewGameHnsSpeech_ChallengeMenu;
}

static void Task_NewGameHnsSpeech_ChallengeMenu(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_NewGameHnsSpeech_FadeOutToChallengeMenu;
}

static void Task_NewGameHnsSpeech_FadeOutToChallengeMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        struct ChallengeSettings savedOptions;
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        savedOptions = gSaveBlock3Ptr->challengeSettings;
        memset(&gSaveBlock3Ptr->challengeSettings, 0, sizeof(struct ChallengeSettings));
        SetDefaultChallengeSettings();
        // Restore options-menu fields so pre-new-game changes survive
        gSaveBlock3Ptr->challengeSettings.followerEnable     = savedOptions.followerEnable;
        gSaveBlock3Ptr->challengeSettings.followerLargeEnable= savedOptions.followerLargeEnable;
        gSaveBlock3Ptr->challengeSettings.autoRun            = savedOptions.autoRun;
        gSaveBlock3Ptr->challengeSettings.autorunSurf        = savedOptions.autorunSurf;
        gSaveBlock3Ptr->challengeSettings.fishing            = savedOptions.fishing;
        gSaveBlock3Ptr->challengeSettings.evenFasterJoy      = savedOptions.evenFasterJoy;
        gSaveBlock3Ptr->challengeSettings.unitSystem         = savedOptions.unitSystem;
        gSaveBlock3Ptr->challengeSettings.disableMatchCall   = savedOptions.disableMatchCall;
        gSaveBlock3Ptr->challengeSettings.fastIntro          = savedOptions.fastIntro;
        gSaveBlock3Ptr->challengeSettings.fastBattle         = savedOptions.fastBattle;
        gSaveBlock3Ptr->challengeSettings.newBackgrounds     = savedOptions.newBackgrounds;
        gSaveBlock3Ptr->challengeSettings.newBattleUI        = savedOptions.newBattleUI;
        gSaveBlock3Ptr->challengeSettings.ballPrompt         = savedOptions.ballPrompt;
        gSaveBlock3Ptr->challengeSettings.lrToRun            = savedOptions.lrToRun;
        gSaveBlock3Ptr->challengeSettings.runType            = savedOptions.runType;
        gSaveBlock3Ptr->challengeSettings.musicOnOff         = savedOptions.musicOnOff;
        gSaveBlock3Ptr->challengeSettings.bikeMusic          = savedOptions.bikeMusic;
        gSaveBlock3Ptr->challengeSettings.surfMusic          = savedOptions.surfMusic;
        ChallengeMenu_SetInitialSetup(TRUE);
        gMain.savedCallback = CB2_NewGameHnsSpeech_ReturnFromChallengeMenu;
        SetMainCallback2(CB2_InitChallengeMenu);
    }
}

static void Task_NewGameHnsSpeech_WhatsYourName(u8 taskId)
{
    NewGameHnsSpeech_ClearWindow(0);
    StringExpandPlaceholders(gStringVar4, gText_Oak_WhatsYourName);
    AddTextPrinterForMessage(TRUE);
    gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForWhatsYourNameToPrint;
}

static void Task_NewGameHnsSpeech_WaitForWhatsYourNameToPrint(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitPressBeforeNameChoice;
}

static void Task_NewGameHnsSpeech_WaitPressBeforeNameChoice(u8 taskId)
{
    if ((JOY_NEW(A_BUTTON)) || (JOY_NEW(B_BUTTON)))
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_NewGameHnsSpeech_StartNamingScreen;
    }
}

static void NewGameHnsSpeech_SetDefaultPlayerName(u8 nameId)
{
    const u8 *name;
    u8 i;

    if (gSaveBlock2Ptr->playerGender == MALE)
        name = sMalePresetNames[nameId];
    else
        name = sFemalePresetNames[nameId];
    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
        gSaveBlock2Ptr->playerName[i] = name[i];
    gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH] = EOS;
}

static void Task_NewGameHnsSpeech_StartNamingScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeAllWindowBuffers();
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        NewGameHnsSpeech_SetDefaultPlayerName(Random() % NUM_PRESET_NAMES);
        DestroyTask(taskId);
        DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, 0, CB2_NewGameHnsSpeech_ReturnFromNamingScreen);
    }
}

static void Task_NewGameHnsSpeech_SoItsPlayerName(u8 taskId)
{
    NewGameHnsSpeech_ClearWindow(0);
    StringExpandPlaceholders(gStringVar4, gText_Oak_SoItsPlayer);
    AddTextPrinterForMessage(TRUE);
    gTasks[taskId].func = Task_NewGameHnsSpeech_CreateNameYesNo;
}

static void Task_NewGameHnsSpeech_CreateNameYesNo(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        CreateYesNoMenuParameterized(2, 1, HNS_MENU_BASE_TILE_NUM, 0xDF, 2, 15);
        gTasks[taskId].func = Task_NewGameHnsSpeech_ProcessNameYesNoMenu;
    }
}

// "아니오" 선택 시 다시 동시 출력 화면으로 돌아가기
static void Task_NewGameHnsSpeech_ProcessNameYesNoMenu(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        PlaySE(SE_SELECT);
        gSprites[gTasks[taskId].tPlayerSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;
        NewGameHnsSpeech_StartFadeOutTarget1InTarget2(taskId, 2);
        NewGameHnsSpeech_StartFadePlatformIn(taskId, 1);
        gTasks[taskId].func = Task_NewGameHnsSpeech_SlidePlatformAway2;
        break;
    case MENU_B_PRESSED:
    case 1:
        PlaySE(SE_SELECT);
        gSprites[gTasks[taskId].tPlayerSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;
        NewGameHnsSpeech_StartFadeOutTarget1InTarget2(taskId, 2);
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForPlayerFadeOutToGenderChoice;
    }
}

static void Task_NewGameHnsSpeech_WaitForPlayerFadeOutToGenderChoice(u8 taskId)
{
    if (gTasks[taskId].tIsDoneFadingSprites)
    {
        u8 goldSpriteId = gTasks[taskId].tGoldSpriteId;
        u8 krisSpriteId = gTasks[taskId].tKrisSpriteId;

        gSprites[goldSpriteId].x = NEW_GAME_SPEECH_BOY_X;
        gSprites[goldSpriteId].y = NEW_GAME_SPEECH_PLAYER_Y;
        gSprites[goldSpriteId].invisible = FALSE;
        gSprites[goldSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;

        gSprites[krisSpriteId].x = NEW_GAME_SPEECH_GIRL_X;
        gSprites[krisSpriteId].y = NEW_GAME_SPEECH_PLAYER_Y;
        gSprites[krisSpriteId].invisible = FALSE;
        gSprites[krisSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;

        gTasks[taskId].tPlayerGender = gSaveBlock2Ptr->playerGender;
        gTasks[taskId].tPlayerSpriteId = gTasks[taskId].tPlayerGender == MALE ? goldSpriteId : krisSpriteId;
        NewGameHnsSpeech_StartFadeInTarget1OutTarget2(taskId, 2);
        
        NewGameHnsSpeech_ClearWindow(0);
        StringExpandPlaceholders(gStringVar4, gText_Oak_BoyOrGirl);
        AddTextPrinterForMessage(TRUE);
        
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForPlayerFadeIn;
    }
}

static void Task_NewGameHnsSpeech_SlidePlatformAway2(u8 taskId)
{
    if (gTasks[taskId].tBG1HOFS)
    {
        gTasks[taskId].tBG1HOFS += 2;
        SetGpuReg(REG_OFFSET_BG1HOFS, gTasks[taskId].tBG1HOFS);
    }
    else
    {
        gTasks[taskId].func = Task_NewGameHnsSpeech_ReshowProfessorMon;
    }
}

static void Task_NewGameHnsSpeech_ReshowProfessorMon(u8 taskId)
{
    u8 spriteId;

    if (gTasks[taskId].tIsDoneFadingSprites)
    {
        gSprites[gTasks[taskId].tGoldSpriteId].invisible = TRUE;
        gSprites[gTasks[taskId].tKrisSpriteId].invisible = TRUE;

        spriteId = gTasks[taskId].tMonSpriteId;
        gSprites[spriteId].x = 80;
        gSprites[spriteId].y = 92;
        gSprites[spriteId].invisible = FALSE;
        gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;

        NewGameHnsSpeech_StartFadeInTarget1OutTarget2(taskId, 2);
        
        NewGameHnsSpeech_ClearWindow(0);
        StringExpandPlaceholders(gStringVar4, gText_Oak_YourePlayer);
        AddTextPrinterForMessage(TRUE);
        
        // Oak is BG2, not an OBJ sprite. Reveal it only after the fade and
        // textbox are ready, so an intervening VBlank cannot show the old state.
        ShowBg(2);
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForYourePlayerText;
    }
}

static void Task_NewGameHnsSpeech_WaitForYourePlayerText(u8 taskId)
{
    if (gTasks[taskId].tIsDoneFadingSprites)
    {
        gSprites[gTasks[taskId].tMonSpriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
        
        // "심향이로구나?" 텍스트가 끝나고 A버튼을 누르면
        if (!RunTextPrintersAndIsPrinter0Active()) 
        {
            NewGameHnsSpeech_ClearWindow(0);
            StringExpandPlaceholders(gStringVar4, gText_Oak_AreYouReady); // "준비는 되었는가?" 출력
            AddTextPrinterForMessage(TRUE);
            gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForAreYouReadyText;
        }
    }
}

static void Task_NewGameHnsSpeech_WaitForAreYouReadyText(u8 taskId)
{
    // "준비는 되었는가?" 텍스트가 끝나고 A버튼을 누르면 오박사와 포켓몬이 먼저 퇴장!
    if (!RunTextPrintersAndIsPrinter0Active()) 
    {
        gSprites[gTasks[taskId].tMonSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;
        NewGameHnsSpeech_StartFadeOutTarget1InTarget2(taskId, 2);
        gTasks[taskId].tTimer = 30; // 약간의 딜레이
        gTasks[taskId].func = Task_NewGameHnsSpeech_FadeInPlayer;
    }
}

static void Task_NewGameHnsSpeech_FadeInPlayer(u8 taskId)
{
    u8 spriteId;

    // 오박사와 포켓몬의 퇴장이 완전히 끝난 후에 주인공 등장!
    if (gTasks[taskId].tIsDoneFadingSprites) 
    {
        HideBg(2); 
        gSprites[gTasks[taskId].tMonSpriteId].invisible = TRUE;

        if (gTasks[taskId].tTimer)
        {
            gTasks[taskId].tTimer--;
            return;
        }

        if (gSaveBlock2Ptr->playerGender != MALE)
            spriteId = gTasks[taskId].tKrisSpriteId;
        else
            spriteId = gTasks[taskId].tGoldSpriteId;

        gSprites[spriteId].x = 120;
        gSprites[spriteId].y = NEW_GAME_SPEECH_PLAYER_Y; // 바닥에 맞게 주인공 Y좌표 80으로 고정
        gSprites[spriteId].invisible = FALSE;
        gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
        gTasks[taskId].tPlayerSpriteId = spriteId;

        NewGameHnsSpeech_StartFadeInTarget1OutTarget2(taskId, 2);
        gTasks[taskId].func = Task_NewGameHnsSpeech_ShrinkPlayer;
    }
}

static void Task_NewGameHnsSpeech_ShrinkPlayer(u8 taskId)
{
    u8 spriteId;

    if (gTasks[taskId].tIsDoneFadingSprites) // 주인공 페이드인이 완전히 끝나면 화면 전환 시작
    {
        gSprites[gTasks[taskId].tPlayerSpriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
        
        spriteId = gTasks[taskId].tPlayerSpriteId;
        gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
        gSprites[spriteId].affineAnims = sSpriteAffineAnimTable_PlayerShrink;
        InitSpriteAffineAnim(&gSprites[spriteId]);
        StartSpriteAffineAnim(&gSprites[spriteId], 0);
        gSprites[spriteId].callback = SpriteCB_MovePlayerDownWhileShrinking;
        BeginNormalPaletteFade(PALETTES_BG, 0, 0, 16, RGB_BLACK);
        FadeOutBGM(4);
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForPlayerShrink;
    }
}

static void Task_NewGameHnsSpeech_WaitForPlayerShrink(u8 taskId)
{
    u8 spriteId = gTasks[taskId].tPlayerSpriteId;

    if (gSprites[spriteId].affineAnimEnded)
        gTasks[taskId].func = Task_NewGameHnsSpeech_FadePlayerToWhite;
}

static void Task_NewGameHnsSpeech_FadePlayerToWhite(u8 taskId)
{
    u8 spriteId;

    if (!gPaletteFade.active)
    {
        spriteId = gTasks[taskId].tPlayerSpriteId;
        gSprites[spriteId].callback = SpriteCB_Null;
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        BeginNormalPaletteFade(PALETTES_OBJECTS, 0, 0, 16, RGB_WHITEALPHA);
        gTasks[taskId].func = Task_NewGameHnsSpeech_Cleanup;
    }
}

static void Task_NewGameHnsSpeech_Cleanup(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeAllWindowBuffers();
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        ResetAllPicSprites();
        SetMainCallback2(CB2_NewGame);
        DestroyTask(taskId);
    }
}

static void CB2_NewGameHnsSpeech_ReturnFromNamingScreen(void)
{
    u8 taskId;
    u8 spriteId;
    u16 savedIme;

    ResetBgsAndClearDma3BusyFlags(0);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    InitBgsFromTemplates(0, sMainMenuBgTemplates, ARRAY_COUNT(sMainMenuBgTemplates));
    
    InitBgFromTemplate(&sHnsBgTemplate);
    InitBgFromTemplate(&sOakSpeechPicBgTemplate);
    SetBgMode(1);
    SetBgTilemapBuffer(2, sOakSpeechPicTilemap);
    SetBgAffine(2, 0, 0, 0, 0, 256, 256, 0);

    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetPaletteFade();
    
    LoadOakSpeechSceneGfx();

    ResetTasks();
    taskId = CreateTask(Task_NewGameHnsSpeech_ReturnFromNamingScreenShowTextbox, 0);
    gTasks[taskId].tTimer = 5;
    gTasks[taskId].tBG1HOFS = -60;
    ScanlineEffect_Stop();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetAllPicSprites();
    AddHnsSpeechObjects(taskId);
    if (gSaveBlock2Ptr->playerGender != MALE)
    {
        gTasks[taskId].tPlayerGender = FEMALE;
        spriteId = gTasks[taskId].tKrisSpriteId;
    }
    else
    {
        gTasks[taskId].tPlayerGender = MALE;
        spriteId = gTasks[taskId].tGoldSpriteId;
    }
    gSprites[spriteId].x = 120;
    gSprites[spriteId].y = NEW_GAME_SPEECH_PLAYER_Y;
    gSprites[spriteId].invisible = FALSE;
    gTasks[taskId].tPlayerSpriteId = spriteId;
    SetGpuReg(REG_OFFSET_BG1HOFS, -60);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    ShowBg(0);
    ShowBg(1);
    HideBg(2);
    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE |= 1;
    REG_IME = savedIme;
    SetVBlankCallback(VBlankCB_HnsMenu);
    SetMainCallback2(CB2_HnsMenu);
    InitWindows(sNewGameHnsSpeechTextWindows);
    LoadMainMenuWindowFrameTiles(0, HNS_MENU_BASE_TILE_NUM);
    LoadMessageBoxGfx(0, HNS_DLG_BASE_TILE_NUM, BG_PLTT_ID(15));
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
}

static void CB2_NewGameHnsSpeech_ReturnFromChallengeMenu(void)
{
    u8 taskId;

    ResetTasks();
    taskId = CreateTask(Task_NewGameHnsSpeech_ReturnFromChallengeMenuShowTextbox, 0);
    InitHnsSpeechScene(taskId);
    SetMainCallback2(CB2_HnsMenu);
}

static void Task_NewGameHnsSpeech_ReturnFromChallengeMenuShowTextbox(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DrawDialogFrameWithCustomTile(0, TRUE, HNS_DLG_BASE_TILE_NUM);
        StringExpandPlaceholders(gStringVar4, gText_Oak_ChallengeSelected);
        AddTextPrinterForMessage(TRUE);
        gTasks[taskId].func = Task_NewGameHnsSpeech_WaitForTextAfterChallengeMenu;
    }
}

static void Task_NewGameHnsSpeech_WaitForTextAfterChallengeMenu(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active() && ((JOY_NEW(A_BUTTON)) || (JOY_NEW(B_BUTTON))))
    {
        NewGameHnsSpeech_ClearWindow(0);
        BeginNormalPaletteFade(PALETTES_ALL & ~(1 << 15), 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_NewGameHnsSpeech_TimeIntro_Init;
    }
}

static void SpriteCB_Null(struct Sprite *sprite)
{
}

static void SpriteCB_MovePlayerDownWhileShrinking(struct Sprite *sprite)
{
    u32 y;

    y = (sprite->y << 16) + sprite->data[0] + 0xC000;
    sprite->y = y >> 16;
    sprite->data[0] = y;
}

static u8 NewGameHnsSpeech_CreateMonSprite(u8 x, u8 y)
{
    return CreateMonPicSprite_Affine(SPECIES_MARILL, FALSE, 0, MON_PIC_AFFINE_FRONT, x, y, 14, TAG_NONE);
}

static void AddHnsSpeechObjects(u8 taskId)
{
    u8 monSpriteId;
    u8 goldSpriteId;
    u8 krisSpriteId;

    monSpriteId = NewGameHnsSpeech_CreateMonSprite(80, 92);
    gSprites[monSpriteId].callback = SpriteCB_Null;
    gSprites[monSpriteId].oam.priority = 0;
    gSprites[monSpriteId].invisible = TRUE;
    gTasks[taskId].tMonSpriteId = monSpriteId;
    goldSpriteId = CreateTrainerSprite(FacilityClassToPicIndex(FACILITY_CLASS_GOLD_HNS), 120, NEW_GAME_SPEECH_PLAYER_Y, 0, NULL);
    gSprites[goldSpriteId].callback = SpriteCB_Null;
    gSprites[goldSpriteId].invisible = TRUE;
    gSprites[goldSpriteId].oam.priority = 0;
    gTasks[taskId].tGoldSpriteId = goldSpriteId;
    krisSpriteId = CreateTrainerSprite(FacilityClassToPicIndex(FACILITY_CLASS_KRIS_HNS), 120, NEW_GAME_SPEECH_PLAYER_Y, 0, NULL);
    gSprites[krisSpriteId].callback = SpriteCB_Null;
    gSprites[krisSpriteId].invisible = TRUE;
    gSprites[krisSpriteId].oam.priority = 0;
    gTasks[taskId].tKrisSpriteId = krisSpriteId;
}

#undef tPlayerSpriteId
#undef tBG1HOFS
#undef tPlayerGender
#undef tMonSpriteId
#undef tGoldSpriteId
#undef tKrisSpriteId

#define tMainTask data[0]
#define tAlphaCoeff1 data[1]
#define tAlphaCoeff2 data[2]
#define tDelay data[3]
#define tDelayTimer data[4]

static void Task_NewGameHnsSpeech_FadeOutTarget1InTarget2(u8 taskId)
{
    int alphaCoeff2;

    if (gTasks[taskId].tAlphaCoeff1 == 0)
    {
        gTasks[gTasks[taskId].tMainTask].tIsDoneFadingSprites = TRUE;
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tAlphaCoeff1--;
        gTasks[taskId].tAlphaCoeff2++;
        alphaCoeff2 = gTasks[taskId].tAlphaCoeff2 << 8;
        SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].tAlphaCoeff1 + alphaCoeff2);
    }
}

static void NewGameHnsSpeech_StartFadeOutTarget1InTarget2(u8 taskId, u8 delay)
{
    u8 taskId2;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    gTasks[taskId].tIsDoneFadingSprites = 0;
    taskId2 = CreateTask(Task_NewGameHnsSpeech_FadeOutTarget1InTarget2, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tAlphaCoeff1 = 16;
    gTasks[taskId2].tAlphaCoeff2 = 0;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

static void Task_NewGameHnsSpeech_FadeInTarget1OutTarget2(u8 taskId)
{
    int alphaCoeff2;

    if (gTasks[taskId].tAlphaCoeff1 == 16)
    {
        gTasks[gTasks[taskId].tMainTask].tIsDoneFadingSprites = TRUE;
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tAlphaCoeff1++;
        gTasks[taskId].tAlphaCoeff2--;
        alphaCoeff2 = gTasks[taskId].tAlphaCoeff2 << 8;
        SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].tAlphaCoeff1 + alphaCoeff2);
    }
}

static void NewGameHnsSpeech_StartFadeInTarget1OutTarget2(u8 taskId, u8 delay)
{
    u8 taskId2;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    gTasks[taskId].tIsDoneFadingSprites = 0;
    taskId2 = CreateTask(Task_NewGameHnsSpeech_FadeInTarget1OutTarget2, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tAlphaCoeff1 = 0;
    gTasks[taskId2].tAlphaCoeff2 = 16;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

#undef tMainTask
#undef tAlphaCoeff1
#undef tAlphaCoeff2
#undef tDelay
#undef tDelayTimer

#undef tIsDoneFadingSprites

#define tMainTask data[0]
#define tPalIndex data[1]
#define tDelayBefore data[2]
#define tDelay data[3]
#define tDelayTimer data[4]

static void Task_NewGameHnsSpeech_FadePlatformIn(u8 taskId)
{
    if (gTasks[taskId].tDelayBefore)
    {
        gTasks[taskId].tDelayBefore--;
    }
    else if (gTasks[taskId].tPalIndex == 8)
    {
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tPalIndex++;
    }
}

static void NewGameHnsSpeech_StartFadePlatformIn(u8 taskId, u8 delay)
{
    u8 taskId2;

    taskId2 = CreateTask(Task_NewGameHnsSpeech_FadePlatformIn, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tPalIndex = 0;
    gTasks[taskId2].tDelayBefore = 8;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

static void Task_NewGameHnsSpeech_FadePlatformOut(u8 taskId)
{
    if (gTasks[taskId].tDelayBefore)
    {
        gTasks[taskId].tDelayBefore--;
    }
    else if (gTasks[taskId].tPalIndex == 0)
    {
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tPalIndex--;
    }
}

static void NewGameHnsSpeech_StartFadePlatformOut(u8 taskId, u8 delay)
{
    u8 taskId2;

    taskId2 = CreateTask(Task_NewGameHnsSpeech_FadePlatformOut, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tPalIndex = 8;
    gTasks[taskId2].tDelayBefore = 8;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

#undef tMainTask
#undef tPalIndex
#undef tDelayBefore
#undef tDelay
#undef tDelayTimer

static void NewGameHnsSpeech_ShowGenderMenu(void)
{
    DrawMainMenuWindowBorder(&sNewGameHnsSpeechTextWindows[1], HNS_MENU_BASE_TILE_NUM);
    FillWindowPixelBuffer(1, PIXEL_FILL(1));
    PrintMenuTable(1, ARRAY_COUNT(sMenuActions_Gender), sMenuActions_Gender);
    InitMenuInUpperLeftCornerNormal(1, ARRAY_COUNT(sMenuActions_Gender), 0);
    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_FULL);
}

static s8 NewGameHnsSpeech_ProcessGenderMenuInput(void)
{
    return Menu_ProcessInputNoWrap();
}

static void NewGameHnsSpeech_ClearGenderWindowTilemap(u8 bg, u8 x, u8 y, u8 width, u8 height, u8 unused)
{
    FillBgTilemapBufferRect(bg, 0, x + 255, y + 255, width + 2, height + 2, 2);
}

static void NewGameHnsSpeech_ClearGenderWindow(u8 windowId, bool8 copyToVram)
{
    CallWindowFunction(windowId, NewGameHnsSpeech_ClearGenderWindowTilemap);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ClearWindowTilemap(windowId);
    if (copyToVram == TRUE)
        CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void NewGameHnsSpeech_ClearWindow(u8 windowId)
{
    u8 bgColor = GetFontAttribute(FONT_NORMAL, FONTATTR_COLOR_BACKGROUND);
    u8 maxCharWidth = GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_WIDTH);
    u8 maxCharHeight = GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT);
    u8 winWidth = GetWindowAttribute(windowId, WINDOW_WIDTH);
    u8 winHeight = GetWindowAttribute(windowId, WINDOW_HEIGHT);

    FillWindowPixelRect(windowId, bgColor, 0, 0, maxCharWidth * winWidth, maxCharHeight * winHeight);
    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void NewGameHnsSpeech_WaitForThisIsPokemonText(struct TextPrinterTemplate *printer, u16 renderCmd)
{
    if (*(printer->currentChar - 2) == EXT_CTRL_CODE_PAUSE && !sStartedPokeBallTask)
    {
        sStartedPokeBallTask = TRUE;
        CreateTask(Task_NewGameHnsSpeechSub_InitPokeBall, 0);
    }
}

static void Task_NewGameHnsSpeech_ReturnFromNamingScreenShowTextbox(u8 taskId)
{
    if (gTasks[taskId].tTimer-- <= 0)
    {
        DrawDialogFrameWithCustomTile(0, TRUE, HNS_DLG_BASE_TILE_NUM);
        gTasks[taskId].func = Task_NewGameHnsSpeech_SoItsPlayerName;
    }
}

// 회색 표준 테두리(SoulGold 스타일)를 불러오기 위해 0번 윈도우 프레임을 고정으로 불러옵니다.
static void LoadMainMenuWindowFrameTiles(u8 bgId, u16 tileOffset)
{
    LoadBgTiles(bgId, GetWindowFrameTilesPal(0)->tiles, 0x120, tileOffset);
    LoadPalette(GetWindowFrameTilesPal(0)->pal, BG_PLTT_ID(2), PLTT_SIZE_4BPP);
}

static void DrawMainMenuWindowBorder(const struct WindowTemplate *template, u16 baseTileNum)
{
    u16 r9 = 1 + baseTileNum;
    u16 r10 = 2 + baseTileNum;
    u16 sp18 = 3 + baseTileNum;
    u16 spC = 5 + baseTileNum;
    u16 sp10 = 6 + baseTileNum;
    u16 sp14 = 7 + baseTileNum;
    u16 r6 = 8 + baseTileNum;

    FillBgTilemapBufferRect(template->bg, baseTileNum, template->tilemapLeft - 1, template->tilemapTop - 1, 1, 1, 2);
    FillBgTilemapBufferRect(template->bg, r9, template->tilemapLeft, template->tilemapTop - 1, template->width, 1, 2);
    FillBgTilemapBufferRect(template->bg, r10, template->tilemapLeft + template->width, template->tilemapTop - 1, 1, 1, 2);
    FillBgTilemapBufferRect(template->bg, sp18, template->tilemapLeft - 1, template->tilemapTop, 1, template->height, 2);
    FillBgTilemapBufferRect(template->bg, spC, template->tilemapLeft + template->width, template->tilemapTop, 1, template->height, 2);
    FillBgTilemapBufferRect(template->bg, sp10, template->tilemapLeft - 1, template->tilemapTop + template->height, 1, 1, 2);
    FillBgTilemapBufferRect(template->bg, sp14, template->tilemapLeft, template->tilemapTop + template->height, template->width, 1, 2);
    FillBgTilemapBufferRect(template->bg, r6, template->tilemapLeft + template->width, template->tilemapTop + template->height, 1, 1, 2);
    CopyBgTilemapBufferToVram(template->bg);
}

#undef tTimer

#endif // IS_HNS
