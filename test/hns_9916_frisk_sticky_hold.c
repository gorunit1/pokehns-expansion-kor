// HnS seq 469 #9916 회귀 테스트 — 통찰 문장 이름 토큰(B_ATK/B_DEF → B_EFF/B_SCR, 정책 74)과
// 점착이 도둑질을 막는 스크립트 변경(BattleScript_NoItemSteal → BattleScript_StickyHoldActivatesRet)을 한글 출력으로 확인한다.
// upstream 테스트는 영어 MESSAGE라 HnS에서 판정되지 않는다. 이름이 모두 "HnS 9916:"으로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 9916: Frisk names the user and the frisked foe (player's Frisk)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_FRISK); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FRISK);
        MESSAGE("마자용은 상대 마자의 먹다남은음식을 통찰했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 9916: Frisk names the user and the frisked foe (opponent's Frisk)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_FRISK); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_FRISK);
        MESSAGE("상대 마자는 마자용의 먹다남은음식을 통찰했다!");
    }
}

// Doubles: one pop-up, then one message per foe holding an item, each naming that foe.
DOUBLE_BATTLE_TEST("HnS 9916: Frisk in a double battle shows one pop-up and names each foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_FRISK); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_FRISK);
        MESSAGE("마자용은 상대 마자의 먹다남은음식을 통찰했다!");
        NOT ABILITY_POPUP(playerLeft, ABILITY_FRISK);
        MESSAGE("마자용은 상대 마자용의 먹다남은음식을 통찰했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 9916: Frisk does nothing when no foe holds an item")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_FRISK); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { }
    } SCENE {
        NOT ABILITY_POPUP(player, ABILITY_FRISK);
    }
}

// Sticky Hold blocking Thief: the holder's pop-up and the same message as before, and the item stays.
SINGLE_BATTLE_TEST("HnS 9916: Sticky Hold stops Thief with its pop-up and message")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_STICKY_HOLD); Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THIEF, player);
        ABILITY_POPUP(opponent, ABILITY_STICKY_HOLD);
        MESSAGE("상대 마자에게는 효과가 없는 것 같다...");
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_LEFTOVERS);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}
