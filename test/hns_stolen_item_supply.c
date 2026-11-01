// 재확인 114 보강(친구 2026-11-01): 대기 목록(HNS_STOLEN_WILD_ITEMS_MAX = 8)에 들어오는 도구 공급 상한,
// 재생성 중복 방지, 목록 8개/9개 경계. 이름이 모두 "HnS stolen item supply:"로 시작한다.
// 각 테스트 이름 끝의 꼬리표:
//   (reachable)               정상 플레이에서 만들 수 있는 상황. 장면을 짧고 확실하게 만들려고 테스트가 행동·난수·
//                             일부 능력치·특성을 정하지만, 상황 자체는 실제 플레이에서 가능하다.
//   (reachable, randomizer)   랜덤화 설정(야생 포켓몬·특성)에서만 만들 수 있는 상황.
//   (data)                    정상 플레이 조건을 정하는 설정·데이터 확인.
//   (test-only)               정상 플레이에서 만들 수 없는 상태를 테스트가 직접 만든 경계 확인.
// 공급 상한의 근거: 도구 개체는 새로 생기지 않는다. 먹거나 던지면 "쓴 도구 기록"(usedHeldItem) 하나가 남고,
// 수확·줍기·리사이클은 그 기록을 지우면서 한 개를 되살린다. 훔치기는 기록을 남기지 않는다.
// 예외는 볼 줍기(던진 볼, 배틀당 한 번)뿐이다.
#include "global.h"
#include "test/battle.h"
#include "item.h"
#include "malloc.h"
#include "wild_encounter.h"
#include "battle_util.h"
#include "battle_script_commands.h"
#include "constants/items.h"
#include "constants/maps.h"

static u32 BagCount(enum Item item)
{
    u32 count = 0;
    struct BagPocket *bagPocket = &gBagPockets[GetItemPocket(item)];
    for (u32 i = 0; i < bagPocket->capacity; i++)
    {
        struct ItemSlot slot = BagPocket_GetSlotData(bagPocket, i);
        if (slot.itemId == item)
            count += slot.quantity;
    }
    return count;
}

// ---- 1. 최대 공급: 야생의 원래 도구 1 + 파티 6마리의 도구 6 (+ 볼 줍기 1) ----
// 파티 6마리가 자뭉열매를 지님. 야생 먹고자(줍기)가 분노의앞니로 HP를 깎아 자뭉열매를 먹게 하고, 턴 끝에 그 열매를 주움 →
// 도둑질로 훔침. 교체해 가며 6마리 모두 반복(7개). 볼 줍기 쪽은 선두가 멍파치(볼 줍기): 자기 열매를 빼앗긴 뒤 실패한 몬스터볼을
// 줍고, 나눔으로 야생에게 넘긴 뒤 다시 훔침(8개 = 목록 크기와 같음). 볼 줍기가 배틀당 한 번인 것은 기존 테스트
// "Ball Fetch only picks up the first failed ball, once per battle"(test/battle/ability/ball_fetch.c)와 gHasFetchedBall.
// 기대: 훔치기 7/8번 모두 성공("도구가 가득 차서" 없음), 배틀 끝에 모두 가방으로.
// 테스트 조건: 먹고자의 방어를 높여 둔다(도둑질 피해로 HP가 절반 아래가 되면 주운 자뭉열매를 훔치기 전에 먹어 장면이 달라짐).
// 야생 배틀 테스트에서는 MOVE(opponent, …) 지정이 적용되지 않고 야생이 가진 기술 중에서 고른다(임시 계측으로 확인) → 야생에게
// 분노의앞니 하나만 준다(PP 30 — 16턴 동안 매 턴 사용). 플레이어 포켓몬의 HP가 매 턴 절반으로 줄지만 쓰러지지 않는다.
WILD_BATTLE_TEST("HnS stolen item supply: 1 wild item + 6 party items = 7 (reachable), + 1 fetched Ball = 8 (reachable, randomizer)")
{
    u32 ballFetch;
    PARAMETRIZE { ballFetch = FALSE; }
    PARAMETRIZE { ballFetch = TRUE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUPER_FANG) == EFFECT_FIXED_PERCENT_DAMAGE);
        ASSUME(GetMoveEffect(MOVE_BESTOW) == EFFECT_BESTOW);
        ASSUME(GetItemPocket(ITEM_SITRUS_BERRY) == POCKET_BERRIES);
        if (ballFetch)
        {
            GIVE_PLAYER_ITEM(ITEM_POKE_BALL, 1);
            PLAYER(SPECIES_YAMPER) { Speed(1); HP(300); MaxHP(400); Ability(ABILITY_BALL_FETCH); Item(ITEM_SITRUS_BERRY); }
        }
        else
        {
            PLAYER(SPECIES_WOBBUFFET) { Speed(1); HP(300); MaxHP(400); Item(ITEM_SITRUS_BERRY); }
        }
        for (u32 i = 1; i < PARTY_SIZE; i++)
            PLAYER(SPECIES_WOBBUFFET) { Speed(1); HP(300); MaxHP(400); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_MUNCHLAX) { Speed(10); HP(999); MaxHP(999); Defense(999); Ability(ABILITY_PICKUP); Item(ITEM_NUGGET); MovesWithPP({MOVE_SUPER_FANG, 30}); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }  // 금구슬, 턴 끝 줍기(0번의 자뭉열매)
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }  // 0번의 자뭉열매
        if (ballFetch)
        {
            TURN { MOVE(opponent, MOVE_SUPER_FANG); USE_ITEM(player, ITEM_POKE_BALL, partyIndex: 0, WITH_RNG(RNG_BALLTHROW_SHAKE, MAX_u16)); } // 턴 끝 볼 줍기
            TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_BESTOW); }
            TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }  // 몬스터볼
        }
        for (u32 k = 1; k < PARTY_SIZE; k++)
        {
            TURN { SWITCH(player, k); MOVE(opponent, MOVE_SUPER_FANG); }  // k번이 자뭉열매를 먹음, 턴 끝 줍기
            TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }
        }
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_ROAR); } // 볼 줍기 쪽 16턴(테스트 러너 최대)
    } SCENE {
        if (ballFetch)
        {
            MESSAGE("멍파치는 야생 먹고자로부터 금구슬을 빼앗았다!");
            ABILITY_POPUP(opponent, ABILITY_PICKUP);
            MESSAGE("멍파치는 야생 먹고자로부터 자뭉열매를 빼앗았다!");
            ABILITY_POPUP(player, ABILITY_BALL_FETCH);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_BESTOW, player);
            MESSAGE("멍파치는 야생 먹고자로부터 몬스터볼을 빼앗았다!");
        }
        else
        {
            MESSAGE("마자용은 야생 먹고자로부터 금구슬을 빼앗았다!");
            ABILITY_POPUP(opponent, ABILITY_PICKUP);
            MESSAGE("마자용은 야생 먹고자로부터 자뭉열매를 빼앗았다!");
        }
        for (u32 k = 1; k < PARTY_SIZE; k++)
        {
            ABILITY_POPUP(opponent, ABILITY_PICKUP);
            MESSAGE("마자용은 야생 먹고자로부터 자뭉열매를 빼앗았다!");
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROAR, player);
        NONE_OF {
            MESSAGE("도구가 가득 차서 빼앗을 수 없다!");
            MESSAGE("자뭉열매를 PC로 보냈다!");
        }
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(BagCount(ITEM_NUGGET), 1);
        EXPECT_EQ(BagCount(ITEM_SITRUS_BERRY), PARTY_SIZE);
        EXPECT_EQ(BagCount(ITEM_POKE_BALL), ballFetch ? 1 : 0); // 1개 던짐 → 주워서 훔친 그 볼이 돌아옴
        for (u32 k = 0; k < PARTY_SIZE; k++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][k], MON_DATA_HELD_ITEM), ITEM_NONE); // 배틀 뒤 되돌림으로 복제되지 않음
    }
}

// ---- 2. 수확: 먹은 열매는 한 번만 다시 생기고, 훔친 열매는 다시 생기지 않음 ----
// 야생 아라리(수확)가 버치열매를 지님. 쾌청(수확 100%) → 전기자석파로 마비 → 버치열매를 먹고 턴 끝에 수확으로 다시 생김 →
// 도둑질로 훔침. 그 뒤 쾌청이 남아 있는 동안 수확이 다시 일어나지 않음. 야생은 리사이클만 가지고 매 턴 먼저 쓰는데 모두 실패
// (먹기 전·수확이 기록을 지운 뒤·훔친 뒤 — 훔치기는 "쓴 도구 기록"을 남기지 않음).
WILD_BATTLE_TEST("HnS stolen item supply: Harvest restores an eaten berry once and never the stolen one (reachable)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_RECYCLE) == EFFECT_RECYCLE);
        ASSUME(GetMoveEffect(MOVE_SUNNY_DAY) == EFFECT_WEATHER);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_EXEGGCUTE) { Speed(10); HP(999); MaxHP(999); Ability(ABILITY_HARVEST); Item(ITEM_CHERI_BERRY); Moves(MOVE_RECYCLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUNNY_DAY); MOVE(opponent, MOVE_RECYCLE); }
        TURN { MOVE(player, MOVE_THUNDER_WAVE, hit: TRUE); MOVE(opponent, MOVE_RECYCLE); } // 버치열매를 먹음, 턴 끝 수확
        TURN { MOVE(player, MOVE_THIEF); MOVE(opponent, MOVE_RECYCLE); }
        TURN { MOVE(player, MOVE_THIEF); MOVE(opponent, MOVE_RECYCLE); }  // 다시 생겼다면 여기서 또 훔침
        TURN { MOVE(player, MOVE_ROAR); MOVE(opponent, MOVE_RECYCLE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_RECYCLE, opponent);
        ABILITY_POPUP(opponent, ABILITY_HARVEST);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_RECYCLE, opponent);
        MESSAGE("마자용은 야생 아라리로부터 버치열매를 빼앗았다!");
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_HARVEST);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_RECYCLE, opponent);
            MESSAGE("마자용은 야생 아라리로부터 버치열매를 빼앗았다!");
        }
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(BagCount(ITEM_CHERI_BERRY), 1);
    }
}

// ---- 3. 플레이어가 쓴 도구 하나를 두 쪽이 되살리려 할 때: 한 번만 ----
// 플레이어 쪽이 자연의은혜로 자기 자뭉열매를 씀(쓴 도구 기록이 남음). 야생의 줍기와 플레이어의 리사이클/수확 중 먼저 처리된
// 쪽만 되살림.
//   PICKUP_THEN_RECYCLE: 턴 끝에 야생 줍기가 가져간 뒤 리사이클 실패 → 훔쳐서 가방 1개
//   PICKUP_THEN_HARVEST: 줍기(빠름)가 가져간 뒤 쾌청에서도 수확 없음 → 훔쳐서 가방 1개
//   HARVEST_THEN_PICKUP: 수확(빠름)이 먼저 되살림 → 줍기 없음, 훔칠 것 없음(플레이어가 지님)
// 어느 쪽이든 가방 + 플레이어가 지닌 자뭉열매 = 1개.
// 야생 쪽은 줍기를 준 마자용(축하만 씀): 도구를 지정하지 않은 야생 포켓몬은 배틀 시작 때 종 데이터의 도구를 무작위로 받는데
// (SetWildMonHeldItem — 지그제구리는 상처약·기력의조각), 마자용은 종 도구가 없어 빈손으로 시작한다. 실제 게임에서도 줍기 야생
// 포켓몬이 빈손인 경우는 흔하다(지그제구리 45%).
enum { PICKUP_THEN_RECYCLE, PICKUP_THEN_HARVEST, HARVEST_THEN_PICKUP };
WILD_BATTLE_TEST("HnS stolen item supply: one used item is restored once, by Pickup or by its owner's Recycle or Harvest (reachable)")
{
    u32 order;
    PARAMETRIZE { order = PICKUP_THEN_RECYCLE; }
    PARAMETRIZE { order = PICKUP_THEN_HARVEST; }
    PARAMETRIZE { order = HARVEST_THEN_PICKUP; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_NATURAL_GIFT) == EFFECT_NATURAL_GIFT);
        ASSUME(GetMoveEffect(MOVE_RECYCLE) == EFFECT_RECYCLE);
        if (order == PICKUP_THEN_RECYCLE)
            PLAYER(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_SITRUS_BERRY); }
        else
            PLAYER(SPECIES_EXEGGCUTE) { Speed(order == HARVEST_THEN_PICKUP ? 20 : 1); Ability(ABILITY_HARVEST); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); HP(999); MaxHP(999); Ability(ABILITY_PICKUP); Moves(MOVE_CELEBRATE); }
    } WHEN {
        if (order == PICKUP_THEN_RECYCLE)
        {
            TURN { MOVE(player, MOVE_NATURAL_GIFT); MOVE(opponent, MOVE_CELEBRATE); } // 턴 끝 줍기
            TURN { MOVE(player, MOVE_RECYCLE); MOVE(opponent, MOVE_CELEBRATE); }
            TURN { MOVE(player, MOVE_THIEF); MOVE(opponent, MOVE_CELEBRATE); }
            TURN { MOVE(player, MOVE_RECYCLE); MOVE(opponent, MOVE_CELEBRATE); }
        }
        else
        {
            TURN { MOVE(player, MOVE_SUNNY_DAY); MOVE(opponent, MOVE_CELEBRATE); }
            TURN { MOVE(player, MOVE_NATURAL_GIFT); MOVE(opponent, MOVE_CELEBRATE); } // 턴 끝 줍기·수확(빠른 쪽 먼저)
            TURN { MOVE(player, MOVE_THIEF); MOVE(opponent, MOVE_CELEBRATE); }
            TURN { MOVE(player, MOVE_THIEF); MOVE(opponent, MOVE_CELEBRATE); }
        }
        TURN { MOVE(player, MOVE_ROAR); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player);
        switch (order)
        {
        case PICKUP_THEN_RECYCLE:
            ABILITY_POPUP(opponent, ABILITY_PICKUP);
            NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_RECYCLE, player);
            MESSAGE("마자용은 야생 마자용으로부터 자뭉열매를 빼앗았다!");
            NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_RECYCLE, player);
            break;
        case PICKUP_THEN_HARVEST:
            ABILITY_POPUP(opponent, ABILITY_PICKUP);
            NOT ABILITY_POPUP(player, ABILITY_HARVEST);
            MESSAGE("아라리는 야생 마자용으로부터 자뭉열매를 빼앗았다!");
            NOT ABILITY_POPUP(player, ABILITY_HARVEST);
            break;
        case HARVEST_THEN_PICKUP:
            ABILITY_POPUP(player, ABILITY_HARVEST);
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_PICKUP);
                MESSAGE("아라리는 야생 마자용으로부터 자뭉열매를 빼앗았다!");
                ABILITY_POPUP(player, ABILITY_HARVEST);
            }
            break;
        }
    } THEN {
        bool32 restoredByPickup = (order != HARVEST_THEN_PICKUP);
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(BagCount(ITEM_SITRUS_BERRY), restoredByPickup ? 1 : 0);
        EXPECT_EQ(player->item, restoredByPickup ? ITEM_NONE : ITEM_SITRUS_BERRY);
    }
}

// ---- 4. 정상 플레이 조건(설정·데이터) ----
static bool32 SpeciesHasAbility(enum Species species, enum Ability ability)
{
    for (u32 i = 0; i < NUM_ABILITY_SLOTS; i++)
    {
        if (GetSpeciesAbility(species, i) == ability)
            return TRUE;
    }
    return FALSE;
}

static u32 CountWildInfoWithAbility(const struct WildPokemonInfo *info, u32 slots, enum Ability ability)
{
    u32 count = 0;
    if (info == NULL || info->wildPokemon == NULL)
        return 0;
    for (u32 i = 0; i < slots; i++)
    {
        if (SpeciesHasAbility(info->wildPokemon[i].species, ability))
            count++;
    }
    return count;
}

static u32 CountWildHeadersWithAbility(const struct WildPokemonHeader *headers, enum Ability ability, u32 *slotsScanned)
{
    u32 count = 0;
    for (u32 h = 0; headers[h].mapGroup != MAP_GROUP(MAP_UNDEFINED); h++)
    {
        for (u32 t = 0; t < TIMES_OF_DAY_COUNT; t++)
        {
            const struct WildEncounterTypes *types = &headers[h].encounterTypes[t];
            count += CountWildInfoWithAbility(types->landMonsInfo, NUM_LAND_MONS_ENCOUNTER_SLOTS, ability);
            count += CountWildInfoWithAbility(types->waterMonsInfo, NUM_WATER_MONS_ENCOUNTER_SLOTS, ability);
            count += CountWildInfoWithAbility(types->rockSmashMonsInfo, NUM_ROCK_SMASH_MONS_ENCOUNTER_SLOTS, ability);
            count += CountWildInfoWithAbility(types->fishingMonsInfo, NUM_FISHING_MONS_ENCOUNTER_SLOTS, ability);
            count += CountWildInfoWithAbility(types->hiddenMonsInfo, NUM_HIDDEN_MONS_ENCOUNTER_SLOTS, ability);
            if (types->landMonsInfo != NULL)
                (*slotsScanned)++;
        }
    }
    return count;
}

// 랜덤화를 끈 기본 데이터에는 볼 줍기를 가질 수 있는 야생 포켓몬이 없다 → 기본 설정의 싱글 최대는 7(1절 앞쪽).
// 줍기를 가질 수 있는 야생 포켓몬은 있다(1절의 공급 경로가 기본 설정에서도 가능).
TEST("HnS stolen item supply: no wild mon can have Ball Fetch without the randomizer, and Pickup is available (data)")
{
    u32 slotsScanned = 0;
    EXPECT_EQ(CountWildHeadersWithAbility(gWildMonHeaders, ABILITY_BALL_FETCH, &slotsScanned), 0);
    EXPECT_GT(slotsScanned, 0); // 검사가 실제 데이터를 돌았는지
    EXPECT_EQ(CountWildHeadersWithAbility(gBattlePyramidWildMonHeaders, ABILITY_BALL_FETCH, &slotsScanned), 0);
    EXPECT_EQ(CountWildHeadersWithAbility(gBattlePikeWildMonHeaders, ABILITY_BALL_FETCH, &slotsScanned), 0);
    EXPECT(!SpeciesHasAbility(gWildFeebas.species, ABILITY_BALL_FETCH));
    EXPECT_GT(CountWildHeadersWithAbility(gWildMonHeaders, ABILITY_PICKUP, &slotsScanned), 0);
}

// 무작위 야생 더블배틀은 꺼져 있다(야생 더블은 이벤트 7곳뿐 — 모두 포획 금지라 볼을 던질 수 없고 볼 줍기도 없음:
// 2 + 6 = 8. 이벤트 스크립트 확인은 dev_scripts/hns_verify/check_wild_doubles.py). 목록 크기는 8.
TEST("HnS stolen item supply: random wild doubles are off and the waiting list holds 8 (data)")
{
    EXPECT_EQ(WE_DOUBLE_WILD_CHANCE, 0);
    EXPECT_EQ(WE_FLAG_FORCE_DOUBLE_WILD, 0);
    EXPECT_EQ(FNPC_FLAG_PARTNER_WILD_BATTLES, 0);
    EXPECT_EQ(HNS_STOLEN_WILD_ITEMS_MAX, 8);
}

// ---- 5. 목록 8개/9개 경계(테스트 전용 상태) ----
// 정상 플레이에서는 한 배틀에 훔칠 수 있는 도구가 8개를 넘지 않으므로(1~4절), 9번째는 테스트가 목록을 직접 채워서 만든다.
// 가방에 자리가 넉넉해도 9번째는 기록할 칸이 없어 막히고(공간 판단과 별개), 8개는 배틀 끝에 모두 지급된다.
// (목록 크기에 맞춰 센다 — 크기가 8인지는 4절 (data) 테스트가 확인.)
static const enum Item sBoundaryItems[] = {
    ITEM_POTION, ITEM_SUPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL,
    ITEM_ORAN_BERRY, ITEM_CHERI_BERRY, ITEM_NUGGET, ITEM_POKE_BALL,
    ITEM_ANTIDOTE, // 9번째(목록 크기 8일 때)
};
STATIC_ASSERT(ARRAY_COUNT(sBoundaryItems) >= HNS_STOLEN_WILD_ITEMS_MAX + 1, StolenSupplyBoundaryItems);

TEST("HnS stolen item supply: the 8th steal fits, the 9th is refused with room left, and all 8 are given (test-only)")
{
    const enum Item *items = sBoundaryItems;
    struct BattleStruct *savedBattleStruct = gBattleStruct;

    ClearBag();
    for (u32 i = 0; i < PC_ITEMS_COUNT; i++)
    {
        gSaveBlock1Ptr->pcItems[i].itemId = ITEM_NONE;
        gSaveBlock1Ptr->pcItems[i].quantity = 0;
    }
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));

    for (u32 i = 0; i < HNS_STOLEN_WILD_ITEMS_MAX; i++)
    {
        EXPECT(CanStoreStolenWildItem(items[i]));
        // StealTargetItem의 기록과 같은 형태
        gBattleStruct->hnsStolenWildItems[i].item = items[i];
        gBattleStruct->hnsStolenWildItems[i].trainer = B_TRAINER_OPPONENT_A;
        gBattleStruct->hnsStolenWildItems[i].partyIndex = 0;
        gBattleStruct->hnsStolenWildItemCount++;
    }
    EXPECT_EQ(gBattleStruct->hnsStolenWildItemCount, HNS_STOLEN_WILD_ITEMS_MAX);
    EXPECT(CheckBagHasSpace(items[HNS_STOLEN_WILD_ITEMS_MAX], 1));
    EXPECT(CanStoreItemsInBagOrPC(items, HNS_STOLEN_WILD_ITEMS_MAX + 1)); // 공간으로는 9개 모두 들어감
    EXPECT(!CanStoreStolenWildItem(items[HNS_STOLEN_WILD_ITEMS_MAX]));    // 9번째: 기록할 칸이 없음
    EXPECT(!CanStoreStolenWildItem(items[0]));                            // 이미 있는 도구여도 같음
    EXPECT_EQ(gBattleStruct->hnsStolenWildItemCount, HNS_STOLEN_WILD_ITEMS_MAX);

    EXPECT(!TryGiveNextStolenWildItem(FALSE)); // 모두 가방 → 안내 스크립트 없음
    EXPECT_EQ(gBattleStruct->hnsStolenWildItemCount, 0);
    for (u32 i = 0; i < HNS_STOLEN_WILD_ITEMS_MAX; i++)
        EXPECT_EQ(BagCount(items[i]), 1);
    EXPECT_EQ(BagCount(items[HNS_STOLEN_WILD_ITEMS_MAX]), 0);

    Free(gBattleStruct);
    gBattleStruct = savedBattleStruct;
}
