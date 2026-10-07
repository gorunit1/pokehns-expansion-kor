#include "global.h"
#include "test/battle.h"

// Observation only (never committed): Dancer replacement sent in by Eject Button reacting to a plain dance move.
DOUBLE_BATTLE_TEST("HNSFIXOBS O1 Dancer replacement after Eject Button and a plain dance move")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_DRAGON_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_EJECT_BUTTON); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_DRAGON_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, opponentRight);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        // reveal: ATK*100 + SPEED of the Dancer replacement (6 = default)
        EXPECT_EQ(playerLeft->statStages[STAT_ATK] * 100 + playerLeft->statStages[STAT_SPEED], 0);
    }
}
