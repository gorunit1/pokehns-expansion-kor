#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Screen Cleaner displays removed screens in the order Reflect, Light Screen, Aurora Veil")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_NINETALES_ALOLA) { Ability(ABILITY_SNOW_WARNING); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_LIGHT_SCREEN); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_AURORA_VEIL); MOVE(player, MOVE_CELEBRATE); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("Your team's Reflect wore off!");
        MESSAGE("Your team's Light Screen wore off!");
        MESSAGE("Your team's Aurora Veil wore off!");
    }
}
