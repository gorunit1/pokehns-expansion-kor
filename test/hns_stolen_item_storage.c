// 재확인 114(친구 결정 2026-10-31): 도둑질·탐내기로 훔친 야생 도구 보관.
// 배틀 끝에 가방(배틀 피라미드에서는 피라미드 가방) → 안 되면 PC(안내 문장), 가방·PC 모두 자리가 없으면 훔치지 않음(안내 문장).
// 포획하면 그 포켓몬에게 돌아간다. 이름이 모두 "HnS stolen item storage:"로 시작한다.
#include "global.h"
#include "test/battle.h"
#include "item.h"
#include "string_util.h"
#include "text.h"
#include "constants/items.h"

#define STOLEN_ITEM  ITEM_HYPER_POTION // 고급상처약, 상처약 주머니
#define FILLER_ITEM  ITEM_POTION       // 상처약
#define MSG_WIDTH_PX BATTLE_MSG_MAX_WIDTH

// pocket의 앞쪽 칸을 filler 999개로 채우고 freeSlots칸만 비워 둔다(칸 순서는 가방 규칙과 무관)
static void FillPocket(enum Pocket pocket, u32 freeSlots, enum Item filler)
{
    struct BagPocket *bagPocket = &gBagPockets[pocket];
    for (u32 i = 0; i < bagPocket->capacity; i++)
        BagPocket_SetSlotItemIdAndCount(bagPocket, i, i < bagPocket->capacity - freeSlots ? filler : ITEM_NONE, MAX_BAG_ITEM_CAPACITY);
}

static void FillPC(u32 freeSlots, enum Item filler)
{
    for (u32 i = 0; i < PC_ITEMS_COUNT; i++)
    {
        gSaveBlock1Ptr->pcItems[i].itemId = i < PC_ITEMS_COUNT - freeSlots ? filler : ITEM_NONE;
        gSaveBlock1Ptr->pcItems[i].quantity = i < PC_ITEMS_COUNT - freeSlots ? MAX_BAG_ITEM_CAPACITY : 0;
    }
}

static u32 PCCount(enum Item item)
{
    u32 count = 0;
    for (u32 i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == item)
            count += gSaveBlock1Ptr->pcItems[i].quantity;
    }
    return count;
}

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

// 가방·PC 전체 상태 요약(값이 그대로인지 비교)
static u32 StorageHash(void)
{
    u32 hash = 0;
    const u8 *bytes = (const u8 *)&gSaveBlock1Ptr->bag;
    for (u32 i = 0; i < sizeof(gSaveBlock1Ptr->bag); i++)
        hash = hash * 31 + bytes[i];
    bytes = (const u8 *)gSaveBlock1Ptr->pcItems;
    for (u32 i = 0; i < sizeof(gSaveBlock1Ptr->pcItems); i++)
        hash = hash * 31 + bytes[i];
    return hash;
}

// ---- 1. 예약 계산(CanStoreItemsInBagOrPC) — 실제 추가 함수로 시험하고 되돌린다 ----
TEST("HnS stolen item storage: the reservation check counts pockets, stacks and slots, and changes nothing")
{
    ASSUME(GetItemPocket(STOLEN_ITEM) == POCKET_MEDICINE);
    ASSUME(GetItemPocket(FILLER_ITEM) == POCKET_MEDICINE);
    ASSUME(GetItemPocket(ITEM_ORAN_BERRY) == POCKET_BERRIES);
    enum Item two[] = { STOLEN_ITEM, ITEM_FULL_HEAL };     // 서로 다른 상처약 주머니 도구 두 개
    enum Item same[] = { STOLEN_ITEM, STOLEN_ITEM };       // 같은 도구 두 개(한 칸에 쌓임)
    enum Item other[] = { STOLEN_ITEM, ITEM_ORAN_BERRY };  // 주머니가 다름
    u32 hash;

    // 상처약 주머니 1칸, PC 0칸
    FillPocket(POCKET_MEDICINE, 1, FILLER_ITEM);
    FillPC(0, FILLER_ITEM);
    hash = StorageHash();
    EXPECT(CanStoreItemsInBagOrPC(two, 1));
    EXPECT(!CanStoreItemsInBagOrPC(two, 2));   // 마지막 칸을 두 예약이 같이 쓰지 않음
    EXPECT(CanStoreItemsInBagOrPC(same, 2));   // 같은 도구는 같은 칸에 쌓임
    EXPECT(CanStoreItemsInBagOrPC(other, 2));  // 나무열매 주머니는 비어 있음
    EXPECT_EQ(StorageHash(), hash);            // 시험한 뒤 가방·PC 그대로

    // 상처약 주머니 0칸, PC 1칸 → 하나는 PC, 둘째는 실패
    FillPocket(POCKET_MEDICINE, 0, FILLER_ITEM);
    FillPC(1, FILLER_ITEM);
    hash = StorageHash();
    EXPECT(CanStoreItemsInBagOrPC(two, 1));
    EXPECT(!CanStoreItemsInBagOrPC(two, 2));
    EXPECT(CanStoreItemsInBagOrPC(same, 2));   // PC에서도 같은 도구는 쌓임
    EXPECT_EQ(StorageHash(), hash);

    // 스택 잔량: 가방에 고급상처약 998개(주머니 나머지는 꽉), PC 0칸 → 1개는 들어가고 2개는 실패
    FillPocket(POCKET_MEDICINE, 0, FILLER_ITEM);
    BagPocket_SetSlotItemIdAndCount(&gBagPockets[POCKET_MEDICINE], 0, STOLEN_ITEM, MAX_BAG_ITEM_CAPACITY - 1);
    FillPC(0, FILLER_ITEM);
    hash = StorageHash();
    EXPECT(CanStoreItemsInBagOrPC(same, 1));
    EXPECT(!CanStoreItemsInBagOrPC(same, 2));
    // 스택 상한: 999개면 그 칸에 더 못 넣음
    BagPocket_SetSlotItemIdAndCount(&gBagPockets[POCKET_MEDICINE], 0, STOLEN_ITEM, MAX_BAG_ITEM_CAPACITY);
    EXPECT(!CanStoreItemsInBagOrPC(same, 1));
    EXPECT_EQ(BagCount(STOLEN_ITEM), MAX_BAG_ITEM_CAPACITY);
    (void)hash;
}

// ---- 2. 가방 여유 → 가방, PC 안내 없음 ----
WILD_BATTLE_TEST("HnS stolen item storage: room in the Bag -> the stolen item goes to the Bag, no PC message")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(STOLEN_ITEM); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THIEF, player);
        HP_BAR(opponent);
        MESSAGE("마자용은 야생 마자용으로부터 고급상처약을 빼앗았다!");
        NONE_OF {
            MESSAGE("고급상처약을 PC로 보냈다!");
            MESSAGE("도구가 가득 차서 빼앗을 수 없다!");
        }
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(player->item, ITEM_NONE);
        EXPECT_EQ(BagCount(STOLEN_ITEM), 1);
        EXPECT_EQ(PCCount(STOLEN_ITEM), 0);
    }
}

// 배틀 테스트는 배틀을 시작할 때 가방을 비우고 테스트 인벤토리(GIVE_PLAYER_ITEM, 10칸)만 다시 넣는다(ResetTestInventory).
// 그래서 배틀 장면의 "가방 실패"는 나무열매 주머니 규칙으로 만든다: 같은 열매가 999개 있으면 그 열매는 더 못 넣는다
// (BagPocket_AddItem — 나무열매·기술머신 주머니는 한 종류에 한 칸). 주머니 칸이 모두 찬 경우는 1절 TEST에서 확인.
// PC는 배틀 시작 때(매개변수 사이에도) 비우지 않으므로 GIVEN에서 매번 채우거나 비운다.
// PC 안내 문장은 배틀이 실제로 끝날 때(KO·울부짖기·패배 등) 나온다. 마지막 턴까지 그냥 끝나는 테스트는 녹화 배틀 종료
// (CB2_QuitRecordedBattle)로 끝나 문장 없이 지급된다(3b) — PC 안내를 확인하는 장면은 울부짖기 등으로 끝낸다.
#define BERRY_STOLEN ITEM_CHERI_BERRY // 버치열매(마비가 아니면 먹지 않음)

// ---- 3. 가방 실패·PC 여유 → PC + 안내(배틀 끝) ----
WILD_BATTLE_TEST("HnS stolen item storage: no room in the Bag, room in the PC -> sent to the PC with a message")
{
    GIVEN {
        ASSUME(GetItemPocket(BERRY_STOLEN) == POCKET_BERRIES);
        ASSUME(GetMoveEffect(MOVE_ROAR) == EFFECT_ROAR);
        GIVE_PLAYER_ITEM(BERRY_STOLEN, MAX_BAG_ITEM_CAPACITY);
        FillPC(PC_ITEMS_COUNT, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(BERRY_STOLEN); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
        TURN { MOVE(player, MOVE_ROAR); }
    } SCENE {
        MESSAGE("마자용은 야생 마자용으로부터 버치열매를 빼앗았다!");
        MESSAGE("버치열매를 PC로 보냈다!");
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(BagCount(BERRY_STOLEN), MAX_BAG_ITEM_CAPACITY);
        EXPECT_EQ(PCCount(BERRY_STOLEN), 1);
    }
}

// ---- 3b. 녹화 배틀 종료(테스트가 턴을 다 쓴 경우)에서는 문장 없이 PC에 지급(게임에서는 녹화 배틀에만 해당) ----
WILD_BATTLE_TEST("HnS stolen item storage: a recorded battle that is quit gives the item to the PC without the message")
{
    GIVEN {
        GIVE_PLAYER_ITEM(BERRY_STOLEN, MAX_BAG_ITEM_CAPACITY);
        FillPC(PC_ITEMS_COUNT, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(BERRY_STOLEN); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        MESSAGE("마자용은 야생 마자용으로부터 버치열매를 빼앗았다!");
        NOT MESSAGE("버치열매를 PC로 보냈다!");
    } THEN {
        EXPECT_EQ(BagCount(BERRY_STOLEN), MAX_BAG_ITEM_CAPACITY);
        EXPECT_EQ(PCCount(BERRY_STOLEN), 1);
    }
}

// ---- 4. 가방·PC 모두 실패 → 훔치지 않음: 안내, 성공 문장 없음, 피해는 그대로, 상대가 계속 지님 ----
WILD_BATTLE_TEST("HnS stolen item storage: no room in the Bag or PC -> not stolen, the hit stands, the wild mon keeps it")
{
    GIVEN {
        GIVE_PLAYER_ITEM(BERRY_STOLEN, MAX_BAG_ITEM_CAPACITY);
        FillPC(0, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(BERRY_STOLEN); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THIEF, player);
        HP_BAR(opponent);
        MESSAGE("도구가 가득 차서 빼앗을 수 없다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ITEM_STEAL, opponent);
            MESSAGE("마자용은 야생 마자용으로부터 버치열매를 빼앗았다!");
            MESSAGE("버치열매를 PC로 보냈다!");
        }
    } THEN {
        EXPECT_EQ(opponent->item, BERRY_STOLEN);
        EXPECT_LT(opponent->hp, opponent->maxHP);
        EXPECT_EQ(BagCount(BERRY_STOLEN), MAX_BAG_ITEM_CAPACITY);
        EXPECT_EQ(PCCount(BERRY_STOLEN), 0);
        EXPECT_EQ(PCCount(FILLER_ITEM), PC_ITEMS_COUNT * MAX_BAG_ITEM_CAPACITY);
    }
}

// ---- 5. 같은 스택: 가방에 같은 열매 998개 → 가방(999), 999개(상한)면 PC ----
WILD_BATTLE_TEST("HnS stolen item storage: the item stacks onto the same item in the Bag, or goes to the PC at the stack limit")
{
    u32 bagStack;
    PARAMETRIZE { bagStack = MAX_BAG_ITEM_CAPACITY - 1; }
    PARAMETRIZE { bagStack = MAX_BAG_ITEM_CAPACITY; }
    GIVEN {
        GIVE_PLAYER_ITEM(BERRY_STOLEN, bagStack);
        FillPC(PC_ITEMS_COUNT, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(BERRY_STOLEN); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
        TURN { MOVE(player, MOVE_ROAR); }
    } SCENE {
        MESSAGE("마자용은 야생 마자용으로부터 버치열매를 빼앗았다!");
        if (bagStack == MAX_BAG_ITEM_CAPACITY)
            MESSAGE("버치열매를 PC로 보냈다!");
        else
            NOT MESSAGE("버치열매를 PC로 보냈다!");
    } THEN {
        EXPECT_EQ(BagCount(BERRY_STOLEN), MAX_BAG_ITEM_CAPACITY);
        EXPECT_EQ(PCCount(BERRY_STOLEN), bagStack == MAX_BAG_ITEM_CAPACITY ? 1 : 0);
    }
}

// ---- 6. 포획 → 포획 개체에게 돌아감(예약 해제), 가방·PC 그대로, PC 안내 없음 ----
WILD_BATTLE_TEST("HnS stolen item storage: catching the mon gives it the item back and frees the reservation")
{
    GIVEN {
        GIVE_PLAYER_ITEM(BERRY_STOLEN, MAX_BAG_ITEM_CAPACITY);   // 포획하지 않았다면 PC로 갔을 상태
        GIVE_PLAYER_ITEM(ITEM_MASTER_BALL, 1);                   // 테스트 인벤토리를 쓰면 USE_ITEM이 볼을 넣어 주지 않음
        FillPC(PC_ITEMS_COUNT, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(BERRY_STOLEN); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
        TURN { USE_ITEM(player, ITEM_MASTER_BALL, partyIndex: 0); }
    } SCENE {
        MESSAGE("마자용은 야생 마자용으로부터 버치열매를 빼앗았다!");
        NOT MESSAGE("버치열매를 PC로 보냈다!");
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HELD_ITEM), BERRY_STOLEN);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM), ITEM_NONE);
        EXPECT_EQ(PCCount(BERRY_STOLEN), 0);
        EXPECT_EQ(BagCount(BERRY_STOLEN), MAX_BAG_ITEM_CAPACITY);
    }
}

// ---- 7. 종료 경로: KO(승리 스크립트 경로), 울부짖기로 야생 포켓몬이 떠남, 플레이어 패배 → 가방, 가방 실패면 PC ----
WILD_BATTLE_TEST("HnS stolen item storage: the item is given whichever way the battle ends")
{
    u32 ending, bagFull;
    PARAMETRIZE { ending = 0; bagFull = FALSE; } // KO
    PARAMETRIZE { ending = 0; bagFull = TRUE; }
    PARAMETRIZE { ending = 1; bagFull = FALSE; } // 울부짖기(야생 포켓몬이 떠남)
    PARAMETRIZE { ending = 1; bagFull = TRUE; }
    PARAMETRIZE { ending = 2; bagFull = FALSE; } // 플레이어 패배
    PARAMETRIZE { ending = 2; bagFull = TRUE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ROAR) == EFFECT_ROAR);
        FillPC(PC_ITEMS_COUNT, FILLER_ITEM); // PC는 매개변수 사이에 비워지지 않음
        if (bagFull)
            GIVE_PLAYER_ITEM(BERRY_STOLEN, MAX_BAG_ITEM_CAPACITY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); if (ending == 2) HP(1); }
        if (ending == 2) // 플레이어 패배: 도둑질한 그 턴에 상대 몸통박치기로 쓰러짐(한 턴으로 끝냄)
            OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Item(BERRY_STOLEN); Moves(MOVE_TACKLE); }
        else
            OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Item(BERRY_STOLEN); Moves(MOVE_CELEBRATE); }
    } WHEN {
        if (ending == 2)
        {
            TURN { MOVE(player, MOVE_THIEF); MOVE(opponent, MOVE_TACKLE); }
        }
        else
        {
            TURN { MOVE(player, MOVE_THIEF); }
            if (ending == 0)
                TURN { MOVE(player, MOVE_FISSURE, hit: TRUE); }
            else
                TURN { MOVE(player, MOVE_ROAR); }
        }
    } SCENE {
        MESSAGE("마자용은 야생 마자용으로부터 버치열매를 빼앗았다!");
        if (bagFull)
            MESSAGE("버치열매를 PC로 보냈다!");
        else
            NOT MESSAGE("버치열매를 PC로 보냈다!");
    } THEN {
        if (ending == 0)
            EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
        else if (ending == 2)
            EXPECT_EQ(gBattleOutcome, B_OUTCOME_LOST);
        EXPECT_EQ(BagCount(BERRY_STOLEN), bagFull ? MAX_BAG_ITEM_CAPACITY : 1);
        EXPECT_EQ(PCCount(BERRY_STOLEN), bagFull ? 1 : 0);
    }
}

// ---- 8. 같은 배틀에서 두 번 훔치기(줍기로 상대가 자뭉열매를 다시 지님) ----
// 플레이어 마자용이 자뭉열매를 먹은 뒤(분노의앞니) 도둑질로 버치열매를 훔치고, 줍기 지그제구리가 자뭉열매를 주움 → 다시 도둑질.
// 두 열매 모두 가방에 999개라 PC로만 갈 수 있음. PC 빈 칸 2 → 둘 다 PC(이름·순서대로 안내), 1 → 둘째는 막힘
// (첫째가 예약한 마지막 칸을 둘째가 다시 쓰지 않음). 가방에 자뭉열매가 없으면 둘째는 가방.
WILD_BATTLE_TEST("HnS stolen item storage: a second steal while one is waiting cannot use the same last slot")
{
    u32 pcSlots, sitrusInBag;
    PARAMETRIZE { pcSlots = 2; sitrusInBag = TRUE; }   // 둘 다 PC
    PARAMETRIZE { pcSlots = 1; sitrusInBag = TRUE; }   // 둘째는 막힘
    PARAMETRIZE { pcSlots = 1; sitrusInBag = FALSE; }  // 둘째는 가방
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUPER_FANG) == EFFECT_FIXED_PERCENT_DAMAGE);
        ASSUME(GetItemPocket(ITEM_SITRUS_BERRY) == POCKET_BERRIES);
        GIVE_PLAYER_ITEM(BERRY_STOLEN, MAX_BAG_ITEM_CAPACITY);
        if (sitrusInBag)
            GIVE_PLAYER_ITEM(ITEM_SITRUS_BERRY, MAX_BAG_ITEM_CAPACITY);
        FillPC(pcSlots, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); HP(300); MaxHP(400); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_ZIGZAGOON) { Speed(10); Ability(ABILITY_PICKUP); Item(BERRY_STOLEN); Moves(MOVE_SUPER_FANG, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_THIEF); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_ROAR); }
    } SCENE {
        MESSAGE("마자용은 야생 지그제구리로부터 버치열매를 빼앗았다!");
        ABILITY_POPUP(opponent, ABILITY_PICKUP);
        if (pcSlots == 1 && sitrusInBag)
        {
            MESSAGE("도구가 가득 차서 빼앗을 수 없다!");
            NOT MESSAGE("마자용은 야생 지그제구리로부터 자뭉열매를 빼앗았다!");
            MESSAGE("버치열매를 PC로 보냈다!");
        }
        else
        {
            MESSAGE("마자용은 야생 지그제구리로부터 자뭉열매를 빼앗았다!");
            MESSAGE("버치열매를 PC로 보냈다!");
            if (sitrusInBag)
                MESSAGE("자뭉열매를 PC로 보냈다!");
            else
                NOT MESSAGE("자뭉열매를 PC로 보냈다!");
        }
    } THEN {
        EXPECT_EQ(PCCount(BERRY_STOLEN), 1);
        if (pcSlots == 2)
            EXPECT_EQ(PCCount(ITEM_SITRUS_BERRY), 1);
        else if (!sitrusInBag)
            EXPECT_EQ(BagCount(ITEM_SITRUS_BERRY), 1);
        else
            EXPECT_EQ(opponent->item, ITEM_SITRUS_BERRY);
    }
}

// ---- 8b. 승리 끝의 야생 도구 드롭(HnS 기능)과 같은 자리를 다툴 때: 훔친 도구가 먼저 ----
// 가방에 자뭉열매 998개, PC 0칸. 훔친 자뭉열매가 마지막 1개 자리를 예약. 줍기로 지그제구리가 다시 자뭉열매를 지닌 채 쓰러지면
// 드롭 기능이 그 열매를 가방에 넣으려 한다. 훔친 열매를 먼저 주므로 가방 999개는 훔친 열매, 드롭은 기존 "가방이 가득" 처리.
WILD_BATTLE_TEST("HnS stolen item storage: the stolen item is given before the wild mon's dropped item")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUPER_FANG) == EFFECT_FIXED_PERCENT_DAMAGE);
        gSaveBlock3Ptr->challengeSettings.tx_Features_WildMonDropItems = 1;
        GIVE_PLAYER_ITEM(ITEM_SITRUS_BERRY, MAX_BAG_ITEM_CAPACITY - 1);
        FillPC(0, FILLER_ITEM);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); HP(300); MaxHP(400); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_ZIGZAGOON) { Speed(10); Ability(ABILITY_PICKUP); Item(ITEM_SITRUS_BERRY); Moves(MOVE_SUPER_FANG, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_FISSURE, hit: TRUE); }
    } SCENE {
        MESSAGE("마자용은 야생 지그제구리로부터 자뭉열매를 빼앗았다!");
        ABILITY_POPUP(opponent, ABILITY_PICKUP);
        MESSAGE("야생 지그제구리는 쓰러졌다!");
        MESSAGE("가방이 가득 차서 아이템을 주울 수 없습니다!");
        NONE_OF {
            MESSAGE("자뭉열매를 PC로 보냈다!");
            MESSAGE("도구가 가득 차서 빼앗을 수 없다!");
        }
    } THEN {
        EXPECT_EQ(BagCount(ITEM_SITRUS_BERRY), MAX_BAG_ITEM_CAPACITY);
        EXPECT_EQ(PCCount(ITEM_SITRUS_BERRY), 0);
    }
}

// ---- 9. 안내 문장 폭(문장 창 208 px, FONT_NORMAL) ----
TEST("HnS stolen item storage: both new messages fit the battle message window")
{
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("가나다라마바사아자차을"), 0), MSG_WIDTH_PX); // 10글자 도구 이름 + 조사
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("PC로 보냈다!"), 0), MSG_WIDTH_PX);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("도구가 가득 차서"), 0), MSG_WIDTH_PX);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("빼앗을 수 없다!"), 0), MSG_WIDTH_PX);
}
