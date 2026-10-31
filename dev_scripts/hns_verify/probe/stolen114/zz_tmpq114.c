#include "global.h"
#include "test/battle.h"
#include "item.h"

static u32 TmpBagCount(enum Item item)
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

// 같은 장면: test/hns_stolen_item_storage.c "the stolen item is given before the wild mon's dropped item", 지급 순서만 강제로 뺀 판
WILD_BATTLE_TEST("TMPQ forced final delivery failure")
{
    GIVEN {
        gSaveBlock3Ptr->challengeSettings.tx_Features_WildMonDropItems = 1;
        GIVE_PLAYER_ITEM(ITEM_SITRUS_BERRY, MAX_BAG_ITEM_CAPACITY - 1);
        for (u32 i = 0; i < PC_ITEMS_COUNT; i++) { gSaveBlock1Ptr->pcItems[i].itemId = ITEM_POTION; gSaveBlock1Ptr->pcItems[i].quantity = MAX_BAG_ITEM_CAPACITY; }
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); HP(300); MaxHP(400); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_ZIGZAGOON) { Speed(10); Ability(ABILITY_PICKUP); Item(ITEM_SITRUS_BERRY); Moves(MOVE_SUPER_FANG, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); MOVE(player, MOVE_THIEF); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_FISSURE, hit: TRUE); }
    } THEN {
        u32 pc = 0;
        for (u32 i = 0; i < PC_ITEMS_COUNT; i++) if (gSaveBlock1Ptr->pcItems[i].itemId == ITEM_SITRUS_BERRY) pc += gSaveBlock1Ptr->pcItems[i].quantity;
        Test_MgbaPrintf("TMPQ after: bagSitrus=%d pcSitrus=%d wildPartySlot0Item=%d playerMonItem=%d", TmpBagCount(ITEM_SITRUS_BERRY), pc, GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HELD_ITEM), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM));
    }
}
