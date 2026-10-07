#include "global.h"
#include "test/battle.h"

// HnS: Adrenaline Orb and Weakness Policy are not used up when the stats they change are already at the limit
// (+6, or -6 with Contrary), as in the main series and HnS before upstream #9730 (friend decision 2026-10-07 #3).
// HnS prints Korean text, so the checks use animations, stat stages and the held item instead of English MESSAGE().

ASSUMPTIONS
{
    ASSUME(GetItemHoldEffect(ITEM_ADRENALINE_ORB) == HOLD_EFFECT_ADRENALINE_ORB);
    ASSUME(GetItemHoldEffect(ITEM_WEAKNESS_POLICY) == HOLD_EFFECT_WEAKNESS_POLICY);
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): not used up when Intimidate hits a holder at +6 Speed")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_AGILITY, speed: 2);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], MAX_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): still used when Speed is below +6")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_AGILITY, speed: 2);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 5);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): not used up when a Contrary holder is at -6 Speed")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_AGILITY, speed: 2);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_SPINDA) { Ability(ABILITY_CONTRARY); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], MIN_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

DOUBLE_BATTLE_TEST("Adrenaline Orb (HnS): Intimidate on two holders uses only the orb of the one whose Speed can rise")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_AGILITY, speed: 2);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ADRENALINE_ORB); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_AGILITY); }
        TURN { MOVE(opponentLeft, MOVE_AGILITY); }
        TURN { MOVE(opponentLeft, MOVE_AGILITY); }
        TURN { SWITCH(playerLeft, 2); }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], MAX_STAT_STAGE);
        EXPECT_EQ(opponentLeft->item, ITEM_ADRENALINE_ORB);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Weakness Policy (HnS): not used up when Attack and Sp. Atk are both +6")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_SHELL_SMASH, attack: 2, spAtk: 2);
        ASSUME(GetMoveType(MOVE_VINE_WHIP) == TYPE_GRASS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Item(ITEM_WEAKNESS_POLICY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SHELL_SMASH); }
        TURN { MOVE(opponent, MOVE_SHELL_SMASH); }
        TURN { MOVE(opponent, MOVE_SHELL_SMASH); }
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        HP_BAR(opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MAX_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPATK], MAX_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_WEAKNESS_POLICY);
    }
}

SINGLE_BATTLE_TEST("Weakness Policy (HnS): still used when only Attack is +6")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_SWORDS_DANCE, attack: 2);
        ASSUME(GetMoveType(MOVE_VINE_WHIP) == TYPE_GRASS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Item(ITEM_WEAKNESS_POLICY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MAX_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Weakness Policy (HnS): not used up when a Contrary holder has Attack and Sp. Atk both at -6")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_SHELL_SMASH, attack: 2, spAtk: 2);
        ASSUME(GetMoveType(MOVE_VINE_WHIP) == TYPE_GRASS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Ability(ABILITY_CONTRARY); Item(ITEM_WEAKNESS_POLICY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SHELL_SMASH); }
        TURN { MOVE(opponent, MOVE_SHELL_SMASH); }
        TURN { MOVE(opponent, MOVE_SHELL_SMASH); }
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        HP_BAR(opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MIN_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPATK], MIN_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_WEAKNESS_POLICY);
    }
}

SINGLE_BATTLE_TEST("Weakness Policy (HnS): still used by a Contrary holder with only Attack at -6")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_SWORDS_DANCE, attack: 2);
        ASSUME(GetMoveType(MOVE_VINE_WHIP) == TYPE_GRASS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Ability(ABILITY_CONTRARY); Item(ITEM_WEAKNESS_POLICY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MIN_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGE - 2);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}
