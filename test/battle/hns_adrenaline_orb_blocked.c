#include "global.h"
#include "test/battle.h"

// HnS: an Adrenaline Orb still activates when an Ability, Mist or Flower Veil stops Intimidate, as in the main series
// (Pokemon Showdown adrenalineorb, Bulbapedia) and HnS before upstream #9730 for Inner Focus, Own Tempo, Oblivious and
// Scrappy (friend decision 2026-10-07). It does not activate behind a Substitute, at -6 Attack or at +6 Speed.
// HnS prints Korean text, so the checks use ability pop-ups, animations, stat stages and the held item, not MESSAGE().

ASSUMPTIONS
{
    ASSUME(GetItemHoldEffect(ITEM_ADRENALINE_ORB) == HOLD_EFFECT_ADRENALINE_ORB);
    ASSUME(GetItemHoldEffect(ITEM_CLEAR_AMULET) == HOLD_EFFECT_CLEAR_AMULET);
    ASSUME(GetMoveEffect(MOVE_MIST) == EFFECT_MIST);
    ASSUME(GetMoveEffect(MOVE_BELLY_DRUM) == EFFECT_BELLY_DRUM);
    ASSUME(GetMoveEffect(MOVE_TOPSY_TURVY) == EFFECT_TOPSY_TURVY);
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): used when an Ability stops Intimidate")
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; }
    PARAMETRIZE { ability = ABILITY_OWN_TEMPO; }
    PARAMETRIZE { ability = ABILITY_OBLIVIOUS; }
    PARAMETRIZE { ability = ABILITY_SCRAPPY; }
    PARAMETRIZE { ability = ABILITY_CLEAR_BODY; }
    PARAMETRIZE { ability = ABILITY_WHITE_SMOKE; }
    PARAMETRIZE { ability = ABILITY_FULL_METAL_BODY; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER; }
    GIVEN {
        WITH_CONFIG(B_UPDATED_INTIMIDATE, GEN_8);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ability); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ability);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): used when Mist stops Intimidate, also with Simple and Guard Dog")
{
    enum Species species;
    enum Ability ability;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; ability = ABILITY_TELEPATHY; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; ability = ABILITY_SIMPLE; }
    PARAMETRIZE { species = SPECIES_OKIDOGI; ability = ABILITY_GUARD_DOG; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(species) { Ability(ability); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_MIST); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponent);
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + (ability == ABILITY_SIMPLE ? 2 : 1));
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("Adrenaline Orb (HnS): used when Flower Veil stops Intimidate on a Grass-type holder")
{
    GIVEN {
        ASSUME(GetSpeciesType(SPECIES_ODDISH, 0) == TYPE_GRASS);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_COMFEY) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_ODDISH) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): a Mirror Armor holder uses it after reflecting Intimidate")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_CORVIKNIGHT) { Ability(ABILITY_MIRROR_ARMOR); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): a Guard Dog holder uses it once, also at +6 Attack")
{
    bool32 maxAttack;
    PARAMETRIZE { maxAttack = FALSE; }
    PARAMETRIZE { maxAttack = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_OKIDOGI) { Ability(ABILITY_GUARD_DOG); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        if (maxAttack)
            TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        if (maxAttack)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_GUARD_DOG);
        if (!maxAttack)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], maxAttack ? MAX_STAT_STAGE : DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): not used when Intimidate hits a Substitute")
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_TELEPATHY; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ability); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        NONE_OF {
            ABILITY_POPUP(opponent, ability);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

// 0: no blocker, 1: Clear Body, 2: Inner Focus, 3: Mist, 4: Mirror Armor, 5: Guard Dog
SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): not used at -6 Attack, whatever stops Intimidate")
{
    u32 blocker;
    enum Species species = SPECIES_WOBBUFFET;
    enum Ability ability = ABILITY_TELEPATHY;
    PARAMETRIZE { blocker = 0; }
    PARAMETRIZE { blocker = 1; ability = ABILITY_CLEAR_BODY; }
    PARAMETRIZE { blocker = 2; ability = ABILITY_INNER_FOCUS; }
    PARAMETRIZE { blocker = 3; }
    PARAMETRIZE { blocker = 4; species = SPECIES_CORVIKNIGHT; ability = ABILITY_MIRROR_ARMOR; }
    PARAMETRIZE { blocker = 5; species = SPECIES_OKIDOGI; ability = ABILITY_GUARD_DOG; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(species) { Ability(ability); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { MOVE(player, MOVE_TOPSY_TURVY); }
        if (blocker == 3)
            TURN { MOVE(opponent, MOVE_MIST); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOPSY_TURVY, player);
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MIN_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): used at -5 Attack when Clear Body stops Intimidate")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_HOWL, attack: 1);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_CLEAR_BODY); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { MOVE(player, MOVE_TOPSY_TURVY); }
        TURN { MOVE(opponent, MOVE_HOWL); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HOWL, opponent);
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MIN_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): not used at +6 Speed when something stops Intimidate")
{
    enum Ability ability;
    bool32 mist;
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; mist = FALSE; }
    PARAMETRIZE { ability = ABILITY_CLEAR_BODY; mist = FALSE; }
    PARAMETRIZE { ability = ABILITY_TELEPATHY; mist = TRUE; }
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_AGILITY, speed: 2);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ability); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        if (mist)
            TURN { MOVE(opponent, MOVE_MIST); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], MAX_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): a Contrary holder still uses it, but not at +6 Attack")
{
    bool32 maxAttack;
    PARAMETRIZE { maxAttack = FALSE; }
    PARAMETRIZE { maxAttack = TRUE; }
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_SWORDS_DANCE, attack: 2);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_SPINDA) { Ability(ABILITY_CONTRARY); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        if (maxAttack)
        {
            TURN { MOVE(opponent, MOVE_SWORDS_DANCE); } // Contrary: -2
            TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
            TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
            TURN { MOVE(player, MOVE_TOPSY_TURVY); }    // -6 -> +6
        }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        if (maxAttack)
        {
            NONE_OF {
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            }
        }
        else
        {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], maxAttack ? MAX_STAT_STAGE : DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], maxAttack ? DEFAULT_STAT_STAGE : DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponent->item, maxAttack ? ITEM_ADRENALINE_ORB : ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Adrenaline Orb (HnS): not used when Clear Body stops a stat drop other than Intimidate")
{
    GIVEN {
        ASSUME_STAT_CHANGE(MOVE_GROWL, attack: -1);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_SHED_SKIN); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_CLEAR_BODY); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

DOUBLE_BATTLE_TEST("Adrenaline Orb (HnS): two blocked holders each use it right after their own block")
{
    GIVEN {
        WITH_CONFIG(B_UPDATED_INTIMIDATE, GEN_8);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_CLEAR_BODY); Item(ITEM_ADRENALINE_ORB); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_INNER_FOCUS); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponentLeft, ABILITY_CLEAR_BODY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ABILITY_POPUP(opponentRight, ABILITY_INNER_FOCUS);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentLeft->item, ITEM_NONE);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("Adrenaline Orb (HnS): a partner's Clear Amulet does not touch the holder's orb")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_TELEPATHY); Item(ITEM_CLEAR_AMULET); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentLeft->item, ITEM_CLEAR_AMULET);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("Adrenaline Orb (HnS): Mist on the holders' side, both use it")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_TELEPATHY); Item(ITEM_ADRENALINE_ORB); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_MIST); }
        TURN { SWITCH(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentLeft->item, ITEM_NONE);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}
