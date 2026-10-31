// HnS 너즐록 화이트아웃 보충 보호 장치(da0adeaa, 재확인 67, 친구 2026-10-14 유지 결정) 회귀 테스트.
// 배틀 없이 DoWhiteOut()을 직접 부른다. 이름이 모두 "Nuzlocke whiteout (HnS):"로 시작한다
// (TESTS='test/hns_nuzlocke_whiteout.c' 또는 TESTS='Nuzlocke whiteout'). HnS 맵·회복 장소 상수를 쓰므로 BUILD=hns에서만
// 컴파일된다(다른 빌드에서는 빈 파일). "HNS"로 시작하는 이름은 kortests(TESTS=HNS)와 섞이므로 쓰지 않는다.
//
// DoWhiteOut()에 들어가는 상태를 만든다:
//   - 일반 미러 패배·미러 재전 패배: HandleEndTurn_FinishBattle이 gPlayerPartyBackup(원래 파티)을 복구한 뒤
//     CB2_EndTrainerBattle/CB2_EndRematchBattle → CB2_WhiteOut → DoWhiteOut. 파티 = 산 원래 파티.
//   - 기권: 같은 함수에서 기절한 포켓몬이 지워지고(NuzlockeDeleteFaintedPartyPokemon) → CB2_WhiteOut → DoWhiteOut.
//     파티 = 남은 산 포켓몬.
//   - 전멸: 기절 포켓몬이 지워져 빈 파티(또는 삭제를 건너뛰는 배틀이면 기절한 채 남은 파티).
// 실제 배틀·CB2_WhiteOut(하드코어 세이브 삭제, 맵 다시 읽기)·저장/불러오기는 돌리지 않는다.
// DoWhiteOut 안의 RunScriptImmediately(EventScript_WhiteOut), HealPlayerParty, Overworld_ResetStateAfterWhiteOut,
// SetWarpDestinationToLastHealLocation, WarpIntoMap은 실제로 실행된다(세이브 블록의 위치·플래그 값으로 확인).
#include "global.h"
#include "test/test.h"

#if IS_HNS
#include "challenge_menu.h"
#include "event_data.h"
#include "heal_location.h"
#include "nuzlocke.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "constants/battle.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/heal_locations.h"
#include "constants/items.h"
#include "constants/map_groups.h"
#include "constants/moves.h"
#include "constants/species.h"

enum { NUZ_NORMAL, NUZ_EASY, NUZ_ENDED };

struct HnsNuzMonSnap
{
    u16 species;
    u32 personality;
    u32 otId;
    u8 ivs[NUM_STATS];
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u8 abilityNum;
    u16 moves[MAX_MON_MOVES];
    u16 heldItem;
    u8 level;
    u32 exp;
    bool8 isEgg;
};

static const u16 sPartySpecies[PARTY_SIZE] = { SPECIES_CYNDAQUIL, SPECIES_PIDGEY, SPECIES_GEODUDE, SPECIES_ZUBAT, SPECIES_ABRA, SPECIES_MAGIKARP };
static const u8 sPartyLevels[PARTY_SIZE] = { 14, 12, 11, 10, 9, 8 };
static const u16 sPartyItems[PARTY_SIZE] = { ITEM_ORAN_BERRY, ITEM_LEFTOVERS, ITEM_CHARCOAL, ITEM_HARD_STONE, ITEM_TWISTED_SPOON, ITEM_NONE };
static const u8 sPartyAbilityNums[PARTY_SIZE] = { 1, 0, 2, 1, 0, 1 };
static const u16 sPartyMoves[PARTY_SIZE][MAX_MON_MOVES] = {
    { MOVE_TACKLE, MOVE_EMBER, MOVE_LEER, MOVE_QUICK_ATTACK },
    { MOVE_GUST, MOVE_SAND_ATTACK, MOVE_QUICK_ATTACK, MOVE_NONE },
    { MOVE_ROCK_THROW, MOVE_DEFENSE_CURL, MOVE_NONE, MOVE_NONE },
    { MOVE_SUPERSONIC, MOVE_ASTONISH, MOVE_LEECH_LIFE, MOVE_BITE },
    { MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE },
    { MOVE_SPLASH, MOVE_TACKLE, MOVE_NONE, MOVE_NONE },
};
static const u8 sNick0[] = _("FIRE");
static const u8 sNick1[] = _("BIRDY");
static const u8 sNick2[] = _("ROCKO");
static const u8 sNick3[] = _("BAT");
static const u8 sNick4[] = _("ABBY");
static const u8 sNick5[] = _("KARP");
static const u8 *const sPartyNicknames[PARTY_SIZE] = { sNick0, sNick1, sNick2, sNick3, sNick4, sNick5 };

#define HNSNUZ_BOX_HASH_SIZE sizeof(gPokemonStoragePtr->boxes)

static u32 HnsNuz_BoxHash(void)
{
    const u8 *p = (const u8 *)gPokemonStoragePtr->boxes;
    u32 h = 2166136261u;
    for (u32 i = 0; i < HNSNUZ_BOX_HASH_SIZE; i++)
        h = (h ^ p[i]) * 16777619u;
    return h;
}

// 다음 테스트에 파티·박스를 남기지 않는다(ClearSaveBlocks는 파티·PC를 지우지 않는다)
static void HnsNuz_Cleanup(void)
{
    ZeroPlayerPartyMons();
    gPartiesCount[B_TRAINER_PLAYER] = 0;
    for (u32 b = 0; b < TOTAL_BOXES_COUNT; b++)
        for (u32 p = 0; p < IN_BOX_COUNT; p++)
            ZeroBoxMonAt(b, p);
}

static void HnsNuz_SetupState(u32 mode)
{
    struct ChallengeSettings *cs = &gSaveBlock3Ptr->challengeSettings;

    HnsNuz_Cleanup();

    // 미러(도둑미러 아님)와 함께 켠 상태. DoWhiteOut은 미러 설정을 읽지 않는다.
    cs->tx_Challenges_Mirror = TRUE;
    cs->tx_Challenges_Mirror_Thief = FALSE;
    cs->tx_Challenges_Nuzlocke = (mode == NUZ_NORMAL || mode == NUZ_ENDED);
    cs->tx_Nuzlocke_EasyMode = (mode == NUZ_EASY);
    cs->tx_Challenges_NuzlockeHardcore = FALSE;
    cs->tx_Challenges_PartyLimit = 0;
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_START_NUZLOCKE);
    if (mode == NUZ_ENDED)
        FlagSet(FLAG_END_NUZLOCKE);
    else
        FlagClear(FLAG_END_NUZLOCKE);

    // 화이트아웃 스크립트·상태 초기화·리스폰을 확인할 값
    FlagSet(FLAG_NO_WILD_CATCHING);     // EventScript_WhiteOut(IS_HNS)이 지운다
    FlagSet(FLAG_SYS_CYCLING_ROAD);     // Overworld_ResetStateAfterWhiteOut이 지운다
    SetLastHealLocationWarp(HEAL_LOCATION_VIOLET_CITY_HNS);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_VIOLET_CITY_GYM_HNS);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_VIOLET_CITY_GYM_HNS);
    gSaveBlock1Ptr->location.warpId = WARP_ID_NONE;
    gSaveBlock1Ptr->location.x = 3;
    gSaveBlock1Ptr->location.y = 4;
}

static void HnsNuz_ExpectMode(u32 mode)
{
    switch (mode)
    {
    case NUZ_NORMAL:
        EXPECT(IsNuzlockeActive());
        EXPECT(!IsNuzlockeEasyActive());
        break;
    case NUZ_EASY:
        EXPECT(!IsNuzlockeActive());
        EXPECT(IsNuzlockeEasyActive());
        break;
    case NUZ_ENDED:
        EXPECT(!IsNuzlockeActive());
        EXPECT(!IsNuzlockeEasyActive());
        break;
    }
}

static void HnsNuz_GivePartyMon(u32 slot, u32 src)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
    u32 personality = 0x1234567u + src * 0x01010101u;
    CreateMon(mon, sPartySpecies[src], sPartyLevels[src], personality, OTID_STRUCT_PLAYER_ID);
    for (u32 s = 0; s < NUM_STATS; s++)
    {
        u32 iv = (src * 5 + s * 7 + 3) % (MAX_PER_STAT_IVS + 1);
        SetMonData(mon, MON_DATA_HP_IV + s, &iv);
    }
    u32 abilityNum = sPartyAbilityNums[src];
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    for (u32 m = 0; m < MAX_MON_MOVES; m++)
    {
        u32 move = sPartyMoves[src][m];
        SetMonData(mon, MON_DATA_MOVE1 + m, &move);
    }
    u32 item = sPartyItems[src];
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    SetMonData(mon, MON_DATA_NICKNAME, sPartyNicknames[src]);
    CalculateMonStats(mon);
}

// 원래 파티 6마리. 일부는 다치거나 상태이상(살아 있음).
static void HnsNuz_GiveLivingParty(u32 count)
{
    for (u32 i = 0; i < count; i++)
        HnsNuz_GivePartyMon(i, i);
    if (count > 1)
    {
        u32 hp = 1;
        SetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP, &hp);
    }
    if (count > 3)
    {
        u32 status = STATUS1_POISON;
        SetMonData(&gParties[B_TRAINER_PLAYER][3], MON_DATA_STATUS, &status);
    }
    CalculatePlayerPartyCount();
}

static void HnsNuz_Faint(u32 slot)
{
    u32 hp = 0;
    SetMonData(&gParties[B_TRAINER_PLAYER][slot], MON_DATA_HP, &hp);
}

static void HnsNuz_GiveEgg(u32 slot, u16 species)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
    u32 isEgg = TRUE;
    CreateMon(mon, species, 5, 0xE9900000u + slot, OTID_STRUCT_PLAYER_ID);
    CalculateMonStats(mon);
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);
}

static void HnsNuz_PutBoxMon(u32 box, u32 pos, u16 species, u32 personality, bool32 dead, bool32 egg)
{
    struct Pokemon mon;
    CreateMon(&mon, species, 7, personality, OTID_STRUCT_PLAYER_ID);
    CalculateMonStats(&mon);   // CreateMon은 능력치를 계산하지 않는다(최대 HP 0)
    if (egg)
    {
        u32 isEgg = TRUE;
        SetMonData(&mon, MON_DATA_IS_EGG, &isEgg);
    }
    if (dead)
    {
        u32 hp = 0;
        SetMonData(&mon, MON_DATA_HP, &hp);
    }
    SetBoxMonAt(box, pos, &mon.box);
}

// 박스: (0,0) 쉬운 모드에서 PC로 간 기절 포켓몬, (0,1) 알, (0,2) 첫 산 포켓몬(보충 후보), (3,5) 산 포켓몬
#define BOX_CANDIDATE_PERSONALITY 0xB0C5A1u
static void HnsNuz_FillBoxWithCandidate(void)
{
    HnsNuz_PutBoxMon(0, 0, SPECIES_SPEAROW, 0xDEAD01u, TRUE, FALSE);
    HnsNuz_PutBoxMon(0, 1, SPECIES_TOGEPI, 0xE6601u, FALSE, TRUE);
    HnsNuz_PutBoxMon(0, 2, SPECIES_SENTRET, BOX_CANDIDATE_PERSONALITY, FALSE, FALSE);
    HnsNuz_PutBoxMon(3, 5, SPECIES_HOOTHOOT, 0xB0C5A2u, FALSE, FALSE);
    EXPECT_EQ(GetFirstAliveBoxPokemon(), 2);
}

// 보충 후보 없음: 기절 포켓몬과 알만 있다
static void HnsNuz_FillBoxWithoutCandidate(void)
{
    HnsNuz_PutBoxMon(0, 0, SPECIES_SPEAROW, 0xDEAD01u, TRUE, FALSE);
    HnsNuz_PutBoxMon(0, 1, SPECIES_TOGEPI, 0xE6601u, FALSE, TRUE);
    EXPECT_EQ(GetFirstAliveBoxPokemon(), IN_BOX_COUNT * TOTAL_BOXES_COUNT);
}

static void HnsNuz_Snap(struct HnsNuzMonSnap *snap)
{
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        snap[i].species = GetMonData(mon, MON_DATA_SPECIES);
        snap[i].personality = GetMonData(mon, MON_DATA_PERSONALITY);
        snap[i].otId = GetMonData(mon, MON_DATA_OT_ID);
        for (u32 s = 0; s < NUM_STATS; s++)
            snap[i].ivs[s] = GetMonData(mon, MON_DATA_HP_IV + s);
        GetMonData(mon, MON_DATA_NICKNAME, snap[i].nickname);
        snap[i].abilityNum = GetMonData(mon, MON_DATA_ABILITY_NUM);
        for (u32 m = 0; m < MAX_MON_MOVES; m++)
            snap[i].moves[m] = GetMonData(mon, MON_DATA_MOVE1 + m);
        snap[i].heldItem = GetMonData(mon, MON_DATA_HELD_ITEM);
        snap[i].level = GetMonData(mon, MON_DATA_LEVEL);
        snap[i].exp = GetMonData(mon, MON_DATA_EXP);
        snap[i].isEgg = GetMonData(mon, MON_DATA_IS_EGG);
    }
}

// 종·개체(성격값·트레이너 ID·개체값)·이름·특성·기술·도구·레벨·경험치·알 여부를 칸 순서대로 비교
static void HnsNuz_ExpectSlotSame(const struct HnsNuzMonSnap *a, const struct HnsNuzMonSnap *b)
{
    EXPECT_EQ(a->species, b->species);
    EXPECT_EQ(a->personality, b->personality);
    EXPECT_EQ(a->otId, b->otId);
    for (u32 s = 0; s < NUM_STATS; s++)
        EXPECT_EQ(a->ivs[s], b->ivs[s]);
    for (u32 c = 0; c < POKEMON_NAME_LENGTH + 1; c++)
    {
        EXPECT_EQ(a->nickname[c], b->nickname[c]);
        if (a->nickname[c] == EOS)
            break;
    }
    EXPECT_EQ(a->abilityNum, b->abilityNum);
    for (u32 m = 0; m < MAX_MON_MOVES; m++)
        EXPECT_EQ(a->moves[m], b->moves[m]);
    EXPECT_EQ(a->heldItem, b->heldItem);
    EXPECT_EQ(a->level, b->level);
    EXPECT_EQ(a->exp, b->exp);
    EXPECT_EQ(a->isEgg, b->isEgg);
}

static void HnsNuz_ExpectPartySame(const struct HnsNuzMonSnap *before, u32 count)
{
    struct HnsNuzMonSnap after[PARTY_SIZE];
    HnsNuz_Snap(after);
    EXPECT_EQ(CalculatePlayerPartyCount(), count);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        HnsNuz_ExpectSlotSame(&before[i], &after[i]);
}

// 정상 화이트아웃: 회복(HP·상태이상), 스크립트·상태 초기화, 마지막 회복 장소로 리스폰
static void HnsNuz_ExpectNormalWhiteoutEffects(void)
{
    struct WarpData expected;
    SetWhiteoutRespawnWarpAndHealerNPC(&expected);

    for (u32 i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_IS_EGG))
            continue;
        EXPECT_EQ(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
        EXPECT_EQ(GetMonData(mon, MON_DATA_STATUS), STATUS1_NONE);
    }
    EXPECT(!FlagGet(FLAG_NO_WILD_CATCHING));
    EXPECT(!FlagGet(FLAG_SYS_CYCLING_ROAD));
    EXPECT_EQ(gSaveBlock1Ptr->location.mapGroup, expected.mapGroup);
    EXPECT_EQ(gSaveBlock1Ptr->location.mapNum, expected.mapNum);
    EXPECT(!(gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_VIOLET_CITY_GYM_HNS)
          && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_VIOLET_CITY_GYM_HNS)));
    if (expected.warpId == WARP_ID_NONE && expected.x >= 0 && expected.y >= 0)
    {
        EXPECT_EQ(gSaveBlock1Ptr->pos.x, expected.x);
        EXPECT_EQ(gSaveBlock1Ptr->pos.y, expected.y);
    }
}

// 케이스 1·3·4·6 공통: 산 파티(count마리) + 보충 후보가 있는 박스 → 보충 없음, 파티·박스 그대로
static void HnsNuz_LivingPartyWithCandidate(u32 mode, u32 count)
{
    struct HnsNuzMonSnap before[PARTY_SIZE];
    HnsNuz_SetupState(mode);
    HnsNuz_GiveLivingParty(count);
    HnsNuz_FillBoxWithCandidate();
    HnsNuz_ExpectMode(mode);
    HnsNuz_Snap(before);
    u32 boxHash = HnsNuz_BoxHash();

    DoWhiteOut();

    HnsNuz_ExpectPartySame(before, count);
    EXPECT_EQ(HnsNuz_BoxHash(), boxHash);
    EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_SPECIES), SPECIES_SENTRET);
    EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_PERSONALITY), BOX_CANDIDATE_PERSONALITY);
    EXPECT_EQ(GetBoxMonDataAt(3, 5, MON_DATA_SPECIES), SPECIES_HOOTHOOT);
    EXPECT_EQ(GetFirstAliveBoxPokemon(), 2);
    HnsNuz_ExpectNormalWhiteoutEffects();
}

// 케이스 2: 산 파티 + 보충 후보 없는 박스 → 꼬렛 없음, 파티·박스 그대로
static void HnsNuz_LivingPartyWithoutCandidate(u32 mode)
{
    struct HnsNuzMonSnap before[PARTY_SIZE];
    HnsNuz_SetupState(mode);
    HnsNuz_GiveLivingParty(PARTY_SIZE);
    HnsNuz_FillBoxWithoutCandidate();
    HnsNuz_ExpectMode(mode);
    HnsNuz_Snap(before);
    u32 boxHash = HnsNuz_BoxHash();

    DoWhiteOut();

    HnsNuz_ExpectPartySame(before, PARTY_SIZE);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT_NE(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), SPECIES_RATTATA);
    EXPECT_EQ(HnsNuz_BoxHash(), boxHash);
    HnsNuz_ExpectNormalWhiteoutEffects();
}

// 케이스 5: 전멸 뒤(기절 포켓몬 삭제됨 = 빈 파티) + 보충 후보 → 첫 산 박스 포켓몬이 1번 자리로
static void HnsNuz_WipedPartyRefillFromBox(u32 mode, bool32 faintedLeftInParty)
{
    HnsNuz_SetupState(mode);
    if (faintedLeftInParty)
    {
        // 삭제를 건너뛰는 배틀 종류(파트너·시설 등): 기절한 채 남은 파티 + 알 하나
        HnsNuz_GiveLivingParty(3);
        HnsNuz_Faint(0);
        HnsNuz_Faint(1);
        HnsNuz_Faint(2);
        HnsNuz_GiveEgg(3, SPECIES_TOGEPI);
        CalculatePlayerPartyCount();
    }
    HnsNuz_FillBoxWithCandidate();
    HnsNuz_ExpectMode(mode);
    EXPECT_EQ(CountPartyAliveNonEggMonsExcept(PARTY_SIZE), 0);
    struct HnsNuzMonSnap before[PARTY_SIZE];
    HnsNuz_Snap(before);

    DoWhiteOut();

    EXPECT_EQ(GetBoxMonDataAt(0, 0, MON_DATA_SPECIES), SPECIES_SPEAROW);    // 기절 박스 포켓몬은 건너뜀
    EXPECT_EQ(GetBoxMonDataAt(0, 1, MON_DATA_SPECIES), SPECIES_TOGEPI);     // 알도 건너뜀
    EXPECT_EQ(GetBoxMonDataAt(3, 5, MON_DATA_SPECIES), SPECIES_HOOTHOOT);   // 다음 후보는 그대로
    if (faintedLeftInParty)
    {
        // 재확인 70 (b)(친구 2026-10-30): 기절 삭제를 건너뛴 배틀(파트너)은 기절 포켓몬이 파티에 남고
        // HealPlayerParty가 회복시킨다 → 보충하지 않는다. 이전에는 1번 자리의 기절 포켓몬이 덮어쓰였다.
        HnsNuz_ExpectPartySame(before, 4);
        EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_SPECIES), SPECIES_SENTRET);   // 후보는 박스에 그대로
        EXPECT_EQ(CountPartyAliveNonEggMonsExcept(PARTY_SIZE), 3);
    }
    else
    {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_SENTRET);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PERSONALITY), BOX_CANDIDATE_PERSONALITY);
        EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_SPECIES), SPECIES_NONE);       // 박스에서 빠짐
        for (u32 i = 1; i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), SPECIES_NONE);
        EXPECT_EQ(CountPartyAliveNonEggMonsExcept(PARTY_SIZE), 1);
    }
    HnsNuz_ExpectNormalWhiteoutEffects();
}

// 케이스 5: 전멸 뒤 빈 파티 + 보충 후보 없음 → 꼬렛 1마리
static void HnsNuz_WipedPartyRattata(u32 mode)
{
    HnsNuz_SetupState(mode);
    HnsNuz_FillBoxWithoutCandidate();
    HnsNuz_ExpectMode(mode);
    u32 boxHash = HnsNuz_BoxHash();

    DoWhiteOut();

    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_RATTATA);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MAX_HP));
    for (u32 i = 1; i < PARTY_SIZE; i++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), SPECIES_NONE);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(HnsNuz_BoxHash(), boxHash);
    HnsNuz_ExpectNormalWhiteoutEffects();
}

// 케이스 7: 전멸 뒤 기절 포켓몬이 지워지고 알 2개만 남음(산 비알 포켓몬 0) → 알을 산 포켓몬으로 세지 않고 보충.
// (이전 배치 "기절 포켓몬 1번 칸 + 알"은 재확인 70 (b) 뒤 보충하지 않는 경우라 N5b·E5b로 옮김.)
// 2026-10-31 친구 결정(재확인 70 (a) 원본 복원): 박스 포켓몬은 0번 칸에 바로 들어가 그 칸의 알(토게피)을 덮어쓴다.
// 2026-10-30의 "알을 뒤로 밀어 보존" 기대는 그 결정으로 바뀜.
static void HnsNuz_EggsDoNotCount(u32 mode)
{
    HnsNuz_SetupState(mode);
    HnsNuz_GiveEgg(0, SPECIES_TOGEPI);
    HnsNuz_GiveEgg(1, SPECIES_PICHU);
    CalculatePlayerPartyCount();
    HnsNuz_FillBoxWithCandidate();
    HnsNuz_ExpectMode(mode);
    EXPECT_EQ(CountPartyAliveNonEggMonsExcept(PARTY_SIZE), 0);

    DoWhiteOut();

    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_SENTRET);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PERSONALITY), BOX_CANDIDATE_PERSONALITY);
    EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_SPECIES), SPECIES_NONE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG), FALSE);       // 0번 칸 알(토게피)은 덮어써짐
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_IS_EGG), TRUE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), SPECIES_PICHU); // 1번 칸 알은 그 자리 그대로
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_SPECIES), SPECIES_NONE);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 2);
    HnsNuz_ExpectNormalWhiteoutEffects();
}

// ---- 1. 너즐록 + 일반 미러 패배(원래 파티 복구, 살아 있음), 박스에 산 포켓몬 있음 ----
TEST("Nuzlocke whiteout (HnS): N1 normal Nuzlocke: lost Mirror battle, living party, box candidate -> no refill, party and box kept")
{
    HnsNuz_LivingPartyWithCandidate(NUZ_NORMAL, PARTY_SIZE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E1 easy Nuzlocke: lost Mirror battle, living party, box candidate -> no refill, party and box kept")
{
    HnsNuz_LivingPartyWithCandidate(NUZ_EASY, PARTY_SIZE);
    HnsNuz_Cleanup();
}

// ---- 2. 같은 조건, 박스에 보충 후보 없음 ----
TEST("Nuzlocke whiteout (HnS): N2 normal Nuzlocke: lost Mirror battle, living party, no box candidate -> no Rattata, party kept")
{
    HnsNuz_LivingPartyWithoutCandidate(NUZ_NORMAL);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E2 easy Nuzlocke: lost Mirror battle, living party, no box candidate -> no Rattata, party kept")
{
    HnsNuz_LivingPartyWithoutCandidate(NUZ_EASY);
    HnsNuz_Cleanup();
}

// ---- 6. 기권(기절 포켓몬 삭제 뒤 산 포켓몬 4마리)·미러 재전 패배(원래 파티 복구) ----
TEST("Nuzlocke whiteout (HnS): N6 normal Nuzlocke: forfeit with 4 living mons left -> not overwritten")
{
    HnsNuz_LivingPartyWithCandidate(NUZ_NORMAL, 4);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E6 easy Nuzlocke: forfeit with 4 living mons left -> not overwritten")
{
    HnsNuz_LivingPartyWithCandidate(NUZ_EASY, 4);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N6R normal Nuzlocke: lost Mirror rematch, restored 1-mon party -> not overwritten")
{
    HnsNuz_LivingPartyWithCandidate(NUZ_NORMAL, 1);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E6R easy Nuzlocke: lost Mirror rematch, restored 1-mon party -> not overwritten")
{
    HnsNuz_LivingPartyWithCandidate(NUZ_EASY, 1);
    HnsNuz_Cleanup();
}

// ---- 5. 전멸 → 기존 보충 ----
TEST("Nuzlocke whiteout (HnS): N5a normal Nuzlocke: wiped (fainted deleted) -> first living box mon to slot 0")
{
    HnsNuz_WipedPartyRefillFromBox(NUZ_NORMAL, FALSE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E5a easy Nuzlocke: wiped (fainted boxed) -> first living box mon to slot 0")
{
    HnsNuz_WipedPartyRefillFromBox(NUZ_EASY, FALSE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N5b normal Nuzlocke: wiped, fainted mons left in party (partner battle) -> no refill, party revived")
{
    HnsNuz_WipedPartyRefillFromBox(NUZ_NORMAL, TRUE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E5b easy Nuzlocke: wiped, fainted mons left in party (partner battle) -> no refill, party revived")
{
    HnsNuz_WipedPartyRefillFromBox(NUZ_EASY, TRUE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N5c normal Nuzlocke: wiped, no box candidate -> Rattata")
{
    HnsNuz_WipedPartyRattata(NUZ_NORMAL);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E5c easy Nuzlocke: wiped, no box candidate -> Rattata")
{
    HnsNuz_WipedPartyRattata(NUZ_EASY);
    HnsNuz_Cleanup();
}

// ---- 7. 알은 산 전투 가능 포켓몬으로 세지 않는다 ----
TEST("Nuzlocke whiteout (HnS): N7 normal Nuzlocke: wiped, only eggs left -> refill happens, the egg in slot 0 is overwritten")
{
    HnsNuz_EggsDoNotCount(NUZ_NORMAL);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E7 easy Nuzlocke: wiped, only eggs left -> refill happens, the egg in slot 0 is overwritten")
{
    HnsNuz_EggsDoNotCount(NUZ_EASY);
    HnsNuz_Cleanup();
}

// ---- 8~11. 재확인 70 (a): 알만 남은 전멸 — 원본 HnS 보충(친구 결정 2026-10-31) ----
// 박스 후보가 있으면 0번 칸에 바로(그 칸의 알은 덮어써짐, 나머지 알은 제자리), 없으면 파티를 비우고 꼬렛 Lv1(알 모두 사라짐).
// 보충 포켓몬은 회복되고 마릿수가 맞다. 파티 제한·PC 포화와 상관없이 보충한다.
// 2026-10-30 결정(알을 뒤로 밀어 보존, 제한이 꽉 차면 마지막 알을 PC로, PC 포화면 보충 중단)을 기대하던 테스트를
// 이 결정에 맞게 바꿨다(이름도 바뀜).
#define HNSNUZ_EGG_PERSONALITY(slot) (0xE9900000u + (slot))

static void HnsNuz_GiveEggs(u32 count)
{
    static const u16 eggSpecies[] = { SPECIES_TOGEPI, SPECIES_PICHU, SPECIES_CLEFFA, SPECIES_IGGLYBUFF, SPECIES_SMOOCHUM, SPECIES_ELEKID };
    for (u32 i = 0; i < count; i++)
        HnsNuz_GiveEgg(i, eggSpecies[i]);
    CalculatePlayerPartyCount();
}

// 알 i(원래 i번 칸)가 지금 slot 칸에 있다
static void HnsNuz_ExpectEggAt(u32 slot, u32 i)
{
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][slot], MON_DATA_IS_EGG), TRUE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][slot], MON_DATA_PERSONALITY), HNSNUZ_EGG_PERSONALITY(i));
}

static bool32 HnsNuz_BoxHasPersonality(u32 personality)
{
    for (u32 b = 0; b < TOTAL_BOXES_COUNT; b++)
        for (u32 p = 0; p < IN_BOX_COUNT; p++)
            if (GetBoxMonDataAt(b, p, MON_DATA_SPECIES) != SPECIES_NONE && GetBoxMonDataAt(b, p, MON_DATA_PERSONALITY) == personality)
                return TRUE;
    return FALSE;
}

// limit = tx_Challenges_PartyLimit(빼는 칸 수), eggs = 남은 알 수, candidate = 박스 보충 후보 유무
static void HnsNuz_EggsOnlyRefill(u32 mode, u32 limit, u32 eggs, bool32 candidate)
{
    HnsNuz_SetupState(mode);
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_PartyLimit = limit;
    HnsNuz_GiveEggs(eggs);
    if (candidate)
    {
        HnsNuz_FillBoxWithCandidate();
        // 박스의 보충 후보는 다친 채(보충 직후 회복되는지 — 재확인 70 문제 ②)
        u32 hp = 1;
        SetBoxMonDataAt(0, 2, MON_DATA_HP, &hp);
    }
    else
    {
        HnsNuz_FillBoxWithoutCandidate();
    }
    HnsNuz_ExpectMode(mode);
    u32 boxHash = HnsNuz_BoxHash();

    DoWhiteOut();

    // 보충 포켓몬이 0번 칸, HP 가득(회복됨)
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][0];
    EXPECT_EQ(GetMonData(lead, MON_DATA_SPECIES), candidate ? SPECIES_SENTRET : SPECIES_RATTATA);
    EXPECT_EQ(GetMonData(lead, MON_DATA_IS_EGG), FALSE);
    EXPECT_EQ(GetMonData(lead, MON_DATA_HP), GetMonData(lead, MON_DATA_MAX_HP));
    if (candidate)
    {
        EXPECT_EQ(GetMonData(lead, MON_DATA_PERSONALITY), BOX_CANDIDATE_PERSONALITY);
        EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_SPECIES), SPECIES_NONE); // 박스에서 빠짐
        // 0번 칸 알은 덮어써지고 나머지 알은 제자리
        for (u32 i = 1; i < eggs; i++)
            HnsNuz_ExpectEggAt(i, i);
        for (u32 i = (eggs > 1 ? eggs : 1); i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), SPECIES_NONE);
        EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], eggs > 1 ? eggs : 1);
    }
    else
    {
        EXPECT_EQ(GetMonData(lead, MON_DATA_LEVEL), 1);
        // 파티를 비우고 꼬렛만 — 알은 모두 사라짐
        for (u32 i = 1; i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), SPECIES_NONE);
        EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
        EXPECT_EQ(HnsNuz_BoxHash(), boxHash); // PC 그대로(알을 PC로 보내지 않음)
    }
    // 알을 PC로 보내지 않는다
    for (u32 i = 0; i < eggs; i++)
        EXPECT(!HnsNuz_BoxHasPersonality(HNSNUZ_EGG_PERSONALITY(i)));
    EXPECT_EQ(CountPartyAliveNonEggMonsExcept(PARTY_SIZE), 1);
    HnsNuz_ExpectNormalWhiteoutEffects();
}

TEST("Nuzlocke whiteout (HnS): N8 normal Nuzlocke: only eggs, box candidate -> candidate to slot 0 healed, slot 0 egg overwritten")
{
    HnsNuz_EggsOnlyRefill(NUZ_NORMAL, 0, 3, TRUE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E8 easy Nuzlocke: only eggs, box candidate -> candidate to slot 0 healed, slot 0 egg overwritten")
{
    HnsNuz_EggsOnlyRefill(NUZ_EASY, 0, 3, TRUE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N9 normal Nuzlocke: only eggs, no box candidate -> party cleared, Rattata Lv1")
{
    HnsNuz_EggsOnlyRefill(NUZ_NORMAL, 0, 2, FALSE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E9 easy Nuzlocke: only eggs, no box candidate -> party cleared, Rattata Lv1")
{
    HnsNuz_EggsOnlyRefill(NUZ_EASY, 0, 2, FALSE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N10 normal Nuzlocke: eggs fill the party (6), box candidate -> candidate to slot 0, other eggs kept")
{
    HnsNuz_EggsOnlyRefill(NUZ_NORMAL, 0, PARTY_SIZE, TRUE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N10L normal Nuzlocke: eggs fill party limit 3, box candidate -> candidate to slot 0, no egg to PC")
{
    HnsNuz_EggsOnlyRefill(NUZ_NORMAL, 3, 3, TRUE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E10L easy Nuzlocke: one egg, party limit 5, no box candidate -> party cleared, Rattata Lv1")
{
    HnsNuz_EggsOnlyRefill(NUZ_EASY, 5, 1, FALSE);
    HnsNuz_Cleanup();
}

// PC가 꽉 차도 보충은 멈추지 않는다(원본 HnS): 후보가 없으면 파티를 비우고 꼬렛, PC는 그대로.
// 2026-10-30에는 "아무것도 지우지 않고 보충 안 함"을 기대했다 — 2026-10-31 결정으로 바뀜.
static void HnsNuz_FillPCWithDead(void)
{
    struct Pokemon dead;
    CreateMon(&dead, SPECIES_SPEAROW, 7, 0xDEAD02u, OTID_STRUCT_PLAYER_ID);
    CalculateMonStats(&dead);
    u32 hp = 0;
    SetMonData(&dead, MON_DATA_HP, &hp);
    for (u32 b = 0; b < TOTAL_BOXES_COUNT; b++)
        for (u32 p = 0; p < IN_BOX_COUNT; p++)
            SetBoxMonAt(b, p, &dead.box);
}

static void HnsNuz_PCFull(u32 mode, bool32 candidate)
{
    HnsNuz_SetupState(mode);
    HnsNuz_GiveEggs(PARTY_SIZE);
    HnsNuz_FillPCWithDead();
    if (candidate)
        HnsNuz_PutBoxMon(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1, SPECIES_SENTRET, BOX_CANDIDATE_PERSONALITY, FALSE, FALSE); // 마지막 칸에 산 포켓몬 하나
    EXPECT_EQ(GetFirstAliveBoxPokemon(), candidate ? (IN_BOX_COUNT * TOTAL_BOXES_COUNT - 1) : IN_BOX_COUNT * TOTAL_BOXES_COUNT);
    u32 boxHash = HnsNuz_BoxHash();

    DoWhiteOut();

    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][0];
    EXPECT_EQ(GetMonData(lead, MON_DATA_SPECIES), candidate ? SPECIES_SENTRET : SPECIES_RATTATA);
    EXPECT_EQ(GetMonData(lead, MON_DATA_HP), GetMonData(lead, MON_DATA_MAX_HP));
    if (candidate)
    {
        EXPECT_EQ(GetBoxMonDataAt(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1, MON_DATA_SPECIES), SPECIES_NONE);
        for (u32 i = 1; i < PARTY_SIZE; i++)
            HnsNuz_ExpectEggAt(i, i);
        EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], PARTY_SIZE);
    }
    else
    {
        for (u32 i = 1; i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), SPECIES_NONE);
        EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
        EXPECT_EQ(HnsNuz_BoxHash(), boxHash); // 묘지(기절 포켓몬)는 그대로 — 지워서 자리를 만들지 않음
    }
    HnsNuz_ExpectNormalWhiteoutEffects();
}

TEST("Nuzlocke whiteout (HnS): N11 normal Nuzlocke: eggs fill the party, PC full, no box candidate -> party cleared, Rattata, PC kept")
{
    HnsNuz_PCFull(NUZ_NORMAL, FALSE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): E11 easy Nuzlocke: eggs fill the party, PC full, no box candidate -> party cleared, Rattata, PC kept")
{
    HnsNuz_PCFull(NUZ_EASY, FALSE);
    HnsNuz_Cleanup();
}

TEST("Nuzlocke whiteout (HnS): N11C normal Nuzlocke: eggs fill the party, PC full with one living mon -> it goes to slot 0")
{
    HnsNuz_PCFull(NUZ_NORMAL, TRUE);
    HnsNuz_Cleanup();
}

// ---- 대조: 너즐록 꺼짐(FLAG_END_NUZLOCKE) → 전멸이어도 보충 없음, 정상 화이트아웃 ----
TEST("Nuzlocke whiteout (HnS): C1 Nuzlocke ended: fainted party, box candidate -> no refill, normal whiteout")
{
    struct HnsNuzMonSnap before[PARTY_SIZE];
    HnsNuz_SetupState(NUZ_ENDED);
    HnsNuz_GiveLivingParty(PARTY_SIZE);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        HnsNuz_Faint(i);
    HnsNuz_FillBoxWithCandidate();
    HnsNuz_ExpectMode(NUZ_ENDED);
    HnsNuz_Snap(before);

    DoWhiteOut();

    HnsNuz_ExpectPartySame(before, PARTY_SIZE);
    EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_SPECIES), SPECIES_SENTRET);
    HnsNuz_ExpectNormalWhiteoutEffects();
    HnsNuz_Cleanup();
}

#endif // IS_HNS
