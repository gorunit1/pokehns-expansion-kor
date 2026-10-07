// seq 138.5 #8943 영역 D — 일반 세이브 왕복(실제 저장·불러오기 코드) 확인 테스트. 스크래치 사본 전용, 커밋하지 않는다.
// 이름이 "HNS8943 SAVE"로 시작한다(TESTS="HNS8943 SAVE"). 출력 줄은 verify/savetest/savetest.py가 읽는다.
//   MAKE: 결정적인 게임 상태(파티 6·박스 몬·박스 이름·플레이어 정보·플래그·옛/새 형식 녹화 기록)를 만들고
//         TrySavingData(SAVE_NORMAL)로 에뮬레이터 플래시에 저장한 뒤 플래시 32섹터를 "SAV" 줄로 출력한다.
//   LOAD: zz_hns8943_savimage.h(어떤 빌드가 만든 .sav, 또는 실기 .sav)를 플래시에 쓰고 LoadGameSave(SAVE_NORMAL)
//         (→ CopyPartyAndObjectsFromSave → LoadPlayerParty)로 읽은 결과를 "DUMP" 줄로 출력하고, 다시
//         TrySavingData(SAVE_NORMAL)로 저장한 플래시 섹터 해시를 "RESAVE" 줄로 출력한다.
#include "global.h"
#include "test/test.h"
#include "save.h"
#include "load_save.h"
#include "agb_flash.h"
#include "gba/flash_internal.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "recorded_battle.h"
#include "util.h"
#include "new_game.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"

#if __has_include("zz_hns8943_savimage.h")
#include "zz_hns8943_savimage.h"
#endif

static const char sHex[] = "0123456789abcdef";

static u32 Fnv(const void *data, u32 n)
{
    const u8 *p = data;
    u32 h = 2166136261u;
    for (u32 i = 0; i < n; i++)
        h = (h ^ p[i]) * 16777619u;
    return h;
}

static void PutHex32(char *dst, u32 v)
{
    for (s32 k = 7; k >= 0; k--, v >>= 4)
        dst[k] = sHex[v & 0xF];
}

// "<prefix> <hex bytes>" 한 줄 (n <= 100)
static void PrintHexLine(const char *prefix, const u8 *data, u32 n)
{
    char line[256];
    u32 j = 0;
    while (prefix[j] && j < 70)
    {
        line[j] = prefix[j];
        j++;
    }
    line[j++] = ' ';
    for (u32 i = 0; i < n && j < 250; i++)
    {
        line[j++] = sHex[data[i] >> 4];
        line[j++] = sHex[data[i] & 0xF];
    }
    line[j] = '\0';
    Test_MgbaPrintf("%s", line);
}

// "<prefix> v0 v1 ..." (u32 hex, 24개씩 줄을 나눔)
static void PrintWords(const char *prefix, const u32 *vals, u32 n)
{
    char line[256];
    for (u32 start = 0; start < n; start += 24)
    {
        u32 j = 0;
        while (prefix[j] && j < 70)
        {
            line[j] = prefix[j];
            j++;
        }
        line[j++] = '@';
        line[j++] = sHex[(start / 24) & 0xF];
        for (u32 i = start; i < n && i < start + 24; i++)
        {
            line[j++] = ' ';
            PutHex32(&line[j], vals[i]);
            j += 8;
        }
        line[j] = '\0';
        Test_MgbaPrintf("%s", line);
    }
}

#define NUM_FIELDS (MON_DATA_EVOLUTION_TRACKER + 1)

// 모든 MON_DATA 필드를 GetMonData/GetBoxMonData로 읽는다. 버퍼를 쓰는 필드(이름 등)는 버퍼 해시를 쓴다.
static void DumpDecoded(const char *prefix, struct Pokemon *mon, struct BoxPokemon *boxMon)
{
    u32 vals[NUM_FIELDS];
    u8 buf[64];
    for (u32 f = 0; f < NUM_FIELDS; f++)
    {
        u32 v;
        memset(buf, 0, sizeof(buf));
        if (f == MON_DATA_KNOWN_MOVES)
        {
            u16 moves[2] = {MOVE_FLAMETHROWER, MOVES_COUNT};
            memcpy(buf, moves, sizeof(moves));
        }
        if (mon != NULL)
            v = GetMonData(mon, f, buf);
        else
            v = GetBoxMonData(boxMon, f, buf);
        if (f == MON_DATA_NICKNAME || f == MON_DATA_NICKNAME10 || f == MON_DATA_OT_NAME)
            v ^= Fnv(buf, sizeof(buf));
        vals[f] = v;
    }
    PrintWords(prefix, vals, NUM_FIELDS);
}

static void FillMon(struct Pokemon *mon, u32 i, enum Species species)
{
    u32 v;
    u8 nick[POKEMON_NAME_LENGTH + 1];
    CreateMon(mon, species, 5 + i * 9, 0x1000193u * (i + 1) ^ 0x5A5A0000u, OTID_STRUCT_PRESET(0x87654321u));
    v = ITEM_LEFTOVERS - i;                 SetMonData(mon, MON_DATA_HELD_ITEM, &v);
    v = MOVE_FLAMETHROWER + i;              SetMonData(mon, MON_DATA_MOVE1, &v);
    v = MOVE_SURF;                          SetMonData(mon, MON_DATA_MOVE2, &v);
    v = 3 + i;                              SetMonData(mon, MON_DATA_PP1, &v);
    v = 0x5A ^ i;                           SetMonData(mon, MON_DATA_PP_BONUSES, &v);
    v = 10 * i + 4;                         SetMonData(mon, MON_DATA_HP_EV, &v);
    v = 252 - i;                            SetMonData(mon, MON_DATA_SPEED_EV, &v);
    v = 70 + 30 * i;                        SetMonData(mon, MON_DATA_FRIENDSHIP, &v);
    v = (i == 2) ? 0x31 : 0;                SetMonData(mon, MON_DATA_POKERUS, &v);
    v = 0x10 + i;                           SetMonData(mon, MON_DATA_MET_LOCATION, &v);
    v = ITEM_MASTER_BALL + (i % 3);         SetMonData(mon, MON_DATA_POKEBALL, &v);
    v = i % 2;                              SetMonData(mon, MON_DATA_ABILITY_NUM, &v);
    v = TYPE_FIRE;                          SetMonData(mon, MON_DATA_TERA_TYPE, &v);
    v = (i == 4);                           SetMonData(mon, MON_DATA_CHAMPION_RIBBON, &v);
    v = i;                                  SetMonData(mon, MON_DATA_COOL_RIBBON, &v);
    for (u32 k = 0; k < POKEMON_NAME_LENGTH; k++)
        nick[k] = 0xBB + ((k + i) % 26);    // 영문 대문자 영역 바이트
    nick[POKEMON_NAME_LENGTH] = 0xFF;
    nick[3 + i] = 0xFF;
    SetMonData(mon, MON_DATA_NICKNAME, nick);
    CalculateMonStats(mon);
    v = (i == 1) ? STATUS1_BURN : 0;        SetMonData(mon, MON_DATA_STATUS, &v);
    v = GetMonData(mon, MON_DATA_MAX_HP) - i; SetMonData(mon, MON_DATA_HP, &v);
    if (i == 5)
    {
        v = TRUE;                           SetMonData(mon, MON_DATA_IS_EGG, &v);
    }
}

static void FillGameState(bool32 keepWorld)
{
    static const enum Species sParty[PARTY_SIZE] = {
        SPECIES_TYPHLOSION, SPECIES_FERALIGATR, SPECIES_PIKACHU, SPECIES_LUGIA, SPECIES_UNOWN_B, SPECIES_NIDORAN_F,
    };
    static const u8 sBoxPos[][2] = {{0, 0}, {0, 1}, {0, 29}, {5, 13}, {13, 0}, {13, 29}};
    struct Pokemon *tmp = AllocZeroed(sizeof(struct Pokemon));
    u32 i;

    if (keepWorld)
        goto party;
    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
        gSaveBlock2Ptr->playerName[i] = 0xBB + i;
    gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH] = 0xFF;
    gSaveBlock2Ptr->playerGender = FEMALE;
    gSaveBlock2Ptr->playerTrainerId[0] = 0x21;
    gSaveBlock2Ptr->playerTrainerId[1] = 0x43;
    gSaveBlock2Ptr->playerTrainerId[2] = 0x65;
    gSaveBlock2Ptr->playerTrainerId[3] = 0x87;
    gSaveBlock2Ptr->playTimeHours = 123;
    gSaveBlock2Ptr->playTimeMinutes = 45;
    for (i = 0; i < 64; i++)
        gSaveBlock1Ptr->flags[i] = (u8)(i * 37 + 11);
    for (i = 0; i < 32; i++)
        gSaveBlock1Ptr->vars[i] = (u16)(i * 4099 + 7);

party:
    ZeroPlayerPartyMons();
    for (i = 0; i < PARTY_SIZE; i++)
        FillMon(&gPlayerParty[i], i, sParty[i]);
    gPlayerPartyCount = PARTY_SIZE;

    for (i = 0; i < ARRAY_COUNT(sBoxPos); i++)
    {
        FillMon(tmp, i + 7, sParty[(i + 2) % PARTY_SIZE]);
        SetBoxMonAt(sBoxPos[i][0], sBoxPos[i][1], &tmp->box);
    }
    for (i = 0; i < TOTAL_BOXES_COUNT && !keepWorld; i++)
    {
        for (u32 k = 0; k < BOX_NAME_LENGTH; k++)
            gPokemonStoragePtr->boxNames[i][k] = (k < 3 + (i % 5)) ? 0xBB + ((i + k) % 26) : 0xFF;
        gPokemonStoragePtr->boxNames[i][BOX_NAME_LENGTH] = 0xFF;
        gPokemonStoragePtr->boxWallpapers[i] = (i * 5) % 16;
    }
    if (!keepWorld)
        gPokemonStoragePtr->currentBox = 5;
    Free(tmp);
}

static void WriteSyntheticRecordedBattle(void)
{
    // 이 빌드의 RecordedBattleSave 형식으로 '유효한' 기록 하나(내용은 무의미한 무늬)
    struct RecordedBattleSave *rb = AllocZeroed(SECTOR_SIZE);
    u8 *p = (u8 *)rb;
    for (u32 i = 0; i < sizeof(*rb) - 4; i++)
        p[i] = (u8)(i * 7 + 3);
    rb->battleFlags = BATTLE_TYPE_TRAINER;
    rb->checksum = CalcByteArraySum(p, sizeof(*rb) - 4);
    Test_MgbaPrintf("MAKE recorded_battle_write=%d size=%d", TryWriteSpecialSaveSector(SECTOR_ID_RECORDED_BATTLE, p), (s32)sizeof(*rb));
    Free(rb);
}

static void DumpFlash(const char *tag)
{
    u8 *buf = Alloc(SECTOR_SIZE);
    char prefix[32];
    for (u32 s = 0; s < SECTORS_COUNT; s++)
    {
        ReadFlash(s, 0, buf, SECTOR_SIZE);
        for (u32 o = 0; o < SECTOR_SIZE; o += 64)
        {
            u32 k;
            for (k = 0; k < 64 && buf[o + k] == 0xFF; k++)
                ;
            if (k == 64)
                continue;
            // prefix: "<tag> <s> <o>"
            u32 j = 0;
            while (tag[j])
            {
                prefix[j] = tag[j];
                j++;
            }
            prefix[j++] = ' ';
            prefix[j++] = sHex[s >> 4];
            prefix[j++] = sHex[s & 0xF];
            prefix[j++] = ' ';
            PutHex32(&prefix[j], o);
            j += 8;
            prefix[j] = '\0';
            PrintHexLine(prefix, &buf[o], 64);
        }
    }
    Free(buf);
}

static void PrintFlashHashes(const char *tag)
{
    u8 *buf = Alloc(SECTOR_SIZE);
    u32 h[SECTORS_COUNT];
    for (u32 s = 0; s < SECTORS_COUNT; s++)
    {
        ReadFlash(s, 0, buf, SECTOR_SIZE);
        h[s] = Fnv(buf, SECTOR_SIZE);
    }
    PrintWords(tag, h, SECTORS_COUNT);
    Free(buf);
}

TEST("HNS8943 SAVE MAKE")
{
    u32 st;
    FillGameState(FALSE);
    for (u32 s = 0; s < SECTORS_COUNT; s++)
        EraseFlashSector(s);
    st = TrySavingData(SAVE_NORMAL);
    Test_MgbaPrintf("MAKE save_status=%d damaged=%d counter=%d", st, gDamagedSaveSectors, gSaveCounter);
    WriteSyntheticRecordedBattle();
    DumpFlash("SAV");
    EXPECT_EQ(st, SAVE_STATUS_OK);
}

// 새 게임 초기화(NewGameInitData: 플래그·변수·가방·돈·PC 도구·열매 나무·박스 초기화 등) 뒤 파티·박스 몬을 넣고 저장
TEST("HNS8943 SAVE NEWGAME")
{
    u32 st;
    NewGameInitData();
    FillGameState(TRUE);
    for (u32 s = 0; s < SECTORS_COUNT; s++)
        EraseFlashSector(s);
    st = TrySavingData(SAVE_NORMAL);
    Test_MgbaPrintf("NEWGAME save_status=%d damaged=%d counter=%d", st, gDamagedSaveSectors, gSaveCounter);
    DumpFlash("SAVN");
    EXPECT_EQ(st, SAVE_STATUS_OK);
}

#ifdef HNS8943_SAVIMAGE
static void DumpLoadedState(void)
{
    char prefix[24];
    u32 w[8];
    w[0] = gPlayerPartyCount;
    w[1] = Fnv(gPlayerParty, sizeof(struct Pokemon) * PARTY_SIZE);
    w[2] = Fnv(gSaveBlock1Ptr, sizeof(struct SaveBlock1));
    w[3] = Fnv(gSaveBlock2Ptr, sizeof(struct SaveBlock2));
    w[4] = Fnv(gSaveBlock3Ptr, sizeof(struct SaveBlock3));
    w[5] = Fnv(gPokemonStoragePtr, sizeof(struct PokemonStorage));
    w[6] = gPokemonStoragePtr->currentBox;
    w[7] = CalculatePlayerPartyCount();
    PrintWords("DUMP STATE count,partyHash,sb1,sb2,sb3,storage,curBox,calcCount", w, 8);
    PrintHexLine("DUMP PLAYER name", gSaveBlock2Ptr->playerName, PLAYER_NAME_LENGTH + 1);
    PrintHexLine("DUMP PLAYER gender,id", &gSaveBlock2Ptr->playerGender, 1 + 1 + TRAINER_ID_LENGTH);
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        memcpy(prefix, "DUMP PRAW x", 12);
        prefix[10] = '0' + i;
        PrintHexLine(prefix, (const u8 *)&gPlayerParty[i], sizeof(struct Pokemon));
        memcpy(prefix, "DUMP PDEC x", 12);
        prefix[10] = '0' + i;
        DumpDecoded(prefix, &gPlayerParty[i], NULL);
    }
    for (u32 b = 0; b < TOTAL_BOXES_COUNT; b++)
    {
        PrintHexLine("DUMP BOXNAME", gPokemonStoragePtr->boxNames[b], BOX_NAME_LENGTH + 1);
        for (u32 s = 0; s < IN_BOX_COUNT; s++)
        {
            if (GetBoxMonDataAt(b, s, MON_DATA_SPECIES) == SPECIES_NONE)
                continue;
            memcpy(prefix, "DUMP BRAW bb ss", 16);
            prefix[10] = sHex[b >> 4]; prefix[11] = sHex[b & 0xF];
            prefix[13] = '0' + s / 10; prefix[14] = '0' + s % 10;
            PrintHexLine(prefix, (const u8 *)GetBoxedMonPtr(b, s), sizeof(struct BoxPokemon));
            prefix[6] = 'D'; prefix[7] = 'E'; prefix[8] = 'C';
            DumpDecoded(prefix, NULL, GetBoxedMonPtr(b, s));
        }
    }
    PrintHexLine("DUMP WALLPAPERS", gPokemonStoragePtr->boxWallpapers, TOTAL_BOXES_COUNT);
    PrintHexLine("DUMP FLAGS0", gSaveBlock1Ptr->flags, 64);
}

static void ProgramImageAndLoad(void)
{
    u8 *buf = Alloc(SECTOR_SIZE);
    u32 st;
    for (u32 s = 0; s < SECTORS_COUNT; s++)
    {
        u32 k;
        for (k = 0; k < ARRAY_COUNT(sSavImageSectors) && sSavImageSectors[k] != s; k++)
            ;
        if (k == ARRAY_COUNT(sSavImageSectors))
        {
            EraseFlashSector(s);    // 이미지에 없는 섹터는 지운다(같은 프로세스의 앞 테스트가 남긴 내용 제거)
            continue;
        }
        memcpy(buf, sSavImageData[k], SECTOR_SIZE);
        if (ProgramFlashSectorAndVerify(s, buf) != 0)
            Test_MgbaPrintf("LOAD program_failed sector=%d", s);
    }
    Free(buf);
    ZeroPlayerPartyMons();
    gPlayerPartyCount = 0;
    st = LoadGameSave(SAVE_NORMAL);
    Test_MgbaPrintf("DUMP LOADSTATUS %d %d", st, gSaveFileStatus);
}

TEST("HNS8943 SAVE LOAD")
{
    Test_MgbaPrintf("LOAD image=%s sectors=%d", HNS8943_SAVIMAGE_NAME, (s32)ARRAY_COUNT(sSavImageSectors));
    ProgramImageAndLoad();
    DumpLoadedState();
    Test_MgbaPrintf("DUMP RECORDED_BATTLE_VALID %d (A안: 이식 전 기록이면 이식 전 1, 이식 후 0)", CanCopyRecordedBattleSaveData());
}

TEST("HNS8943 SAVE RESAVE")
{
    u32 st;
    ProgramImageAndLoad();
    st = TrySavingData(SAVE_NORMAL);
    Test_MgbaPrintf("DUMP RESAVE_STATUS %d %d %d", st, gDamagedSaveSectors, gSaveCounter);
    PrintFlashHashes("DUMP RESAVE_SECTOR_FNV");
    EXPECT_EQ(st, SAVE_STATUS_OK);
}
#endif
