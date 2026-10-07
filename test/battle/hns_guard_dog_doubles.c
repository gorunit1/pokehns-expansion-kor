#include "global.h"
#include "test/battle.h"

// HnS: in doubles, Intimidate still lowers the Attack of the other foe after it raised a Guard Dog holder's Attack,
// as in the main series and HnS before upstream #9730 (since #9730 the foe after a Guard Dog holder was skipped).
// HnS prints Korean text, so the checks use ability pop-ups, animations and stat stages, not MESSAGE().

DOUBLE_BATTLE_TEST("Guard Dog (HnS): the player's Intimidate reaches both foes, Guard Dog on either side")
{
    bool32 guardDogLeft;
    PARAMETRIZE { guardDogLeft = TRUE; }
    PARAMETRIZE { guardDogLeft = FALSE; }
    GIVEN {
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(guardDogLeft ? SPECIES_OKIDOGI : SPECIES_WYNAUT) { Ability(guardDogLeft ? ABILITY_GUARD_DOG : ABILITY_TELEPATHY); }
        OPPONENT(guardDogLeft ? SPECIES_WYNAUT : SPECIES_OKIDOGI) { Ability(guardDogLeft ? ABILITY_TELEPATHY : ABILITY_GUARD_DOG); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        if (guardDogLeft)
        {
            ABILITY_POPUP(opponentLeft, ABILITY_GUARD_DOG);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        }
        else
        {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
            ABILITY_POPUP(opponentRight, ABILITY_GUARD_DOG);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        }
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], guardDogLeft ? DEFAULT_STAT_STAGE + 1 : DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], guardDogLeft ? DEFAULT_STAT_STAGE - 1 : DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("Guard Dog (HnS): the foe's Intimidate reaches both of the player's Pokemon, Guard Dog on either side")
{
    bool32 guardDogLeft;
    PARAMETRIZE { guardDogLeft = TRUE; }
    PARAMETRIZE { guardDogLeft = FALSE; }
    GIVEN {
        PLAYER(guardDogLeft ? SPECIES_OKIDOGI : SPECIES_WYNAUT) { Ability(guardDogLeft ? ABILITY_GUARD_DOG : ABILITY_TELEPATHY); }
        PLAYER(guardDogLeft ? SPECIES_WYNAUT : SPECIES_OKIDOGI) { Ability(guardDogLeft ? ABILITY_TELEPATHY : ABILITY_GUARD_DOG); }
        OPPONENT(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_TELEPATHY); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponentLeft, ABILITY_INTIMIDATE);
    } THEN {
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], guardDogLeft ? DEFAULT_STAT_STAGE + 1 : DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(playerRight->statStages[STAT_ATK], guardDogLeft ? DEFAULT_STAT_STAGE - 1 : DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("Guard Dog (HnS): an Adrenaline Orb on the foe after a Guard Dog holder still activates")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_ADRENALINE_ORB) == HOLD_EFFECT_ADRENALINE_ORB);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_OKIDOGI) { Ability(ABILITY_GUARD_DOG); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponentLeft, ABILITY_GUARD_DOG);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
    } THEN {
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}
