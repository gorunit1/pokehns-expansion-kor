#include "global.h"
#include "test/battle.h"

// HnS: upstream #10217 clears originalBattlerPartyId only at the end of a move, so a switch made
// outside a move carried over into the switched-in battler's next move (see HandleAction_UseMove).

SINGLE_BATTLE_TEST("HnS: Life Orb recoil applies on the first move after both sides switch")
{
    GIVEN {
        ASSUME(gItemsInfo[ITEM_LIFE_ORB].holdEffect == HOLD_EFFECT_LIFE_ORB);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
        HP_BAR(player);
    } THEN {
        EXPECT_LT(player->hp, player->maxHP);
    }
}

SINGLE_BATTLE_TEST("HnS: U-turn switches the user out on the first move after both sides switch")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_U_TURN) == EFFECT_HIT_ESCAPE);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_U_TURN); SEND_OUT(player, 0); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, player);
        HP_BAR(opponent);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WOBBUFFET);
    }
}
