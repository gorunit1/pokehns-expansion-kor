#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("TMP37 A skip on: Solar Power")
{
    GIVEN {
        PLAYER(SPECIES_CHARMANDER) { Ability(ABILITY_SOLAR_POWER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SOLAR_POWER);
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("TMP37 B skip off: Solar Power")
{
    GIVEN {
        gSaveBlock3Ptr->challengeSettings.fastBattle = 1;
        PLAYER(SPECIES_CHARMANDER) { Ability(ABILITY_SOLAR_POWER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SOLAR_POWER);
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("TMP37 C skip on: Leftovers item pop-up")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        HP_BAR(player);
    }
}

DOUBLE_BATTLE_TEST("TMP37 D skip on: two Intimidate pop-ups in a double")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponentLeft, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponentRight, ABILITY_INTIMIDATE);
    }
}

SINGLE_BATTLE_TEST("TMP37 E skip on: pause without a pop-up (But it failed)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SNORE); }
    } SCENE {
        MESSAGE("마자용은 코골기를 썼다!");
    }
}

SINGLE_BATTLE_TEST("TMP37 F skip on: ability pop-up of the other side already in place")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SNORE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
    }
}
