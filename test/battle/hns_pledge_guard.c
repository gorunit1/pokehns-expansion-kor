#include "global.h"
#include "test/battle.h"

// HnS: pledge combo guard (CancelerPledgeAttack, TurnValuesCleanUp).
// Since #9918 the move after a waiting pledge user always becomes the combined move. When the partner does not
// really use its pledge (Encore, Z-Move, Max Move, disobedience, replaced mid-turn) or the waiting move was called
// by another move (Instruct, Sleep Talk, Metronome, Me First), GetPledgeComboMove/GetPledgeResultMove got a move
// that is not a pledge: assertf (crash screen in `make hns`, INVALID here), then a wrong combined move. Before #9918
// the first group went on without a combo and the second group froze. Upstream 1.17.0 and master have the same code.
// TurnValuesCleanUp clears a wait whose partner never moved (it did so before #9918), so it cannot reach the next turn.
// HnS prints Korean text, so these tests check animations instead of English MESSAGE().

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_FIRE_PLEDGE) == EFFECT_PLEDGE);
    ASSUME(GetMoveEffect(MOVE_GRASS_PLEDGE) == EFFECT_PLEDGE);
    ASSUME(GetMoveEffect(MOVE_WATER_PLEDGE) == EFFECT_PLEDGE);
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the waiting user's partner is Encored into another move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_FIRE_PLEDGE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_GRASS_PLEDGE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); Moves(MOVE_ENCORE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(opponentLeft, MOVE_ENCORE, target: playerRight); MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentRight); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentRight); MOVE(opponentRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ENCORE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentRight);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the partner uses its pledge as a Z-Move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Item(ITEM_GRASSIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BLOOM_DOOM, playerRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the partner Dynamaxes with its pledge")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_OVERGROWTH, playerRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the partner was replaced by Eject Button before acting")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_FIRE_PLEDGE); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Item(ITEM_EJECT_BUTTON); Moves(MOVE_GRASS_PLEDGE); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_WATER_PLEDGE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_TACKLE, target: playerRight); MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); MOVE(opponentRight, MOVE_CELEBRATE); SEND_OUT(playerRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, opponentRight);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_PLEDGE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAINBOW, playerLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: a wait left over at the end of the turn does not turn the next pledge into a combined move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(3); Moves(MOVE_FIRE_PLEDGE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(2); Item(ITEM_EJECT_BUTTON); Moves(MOVE_GRASS_PLEDGE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(2); Moves(MOVE_WATER_PLEDGE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); Moves(MOVE_TACKLE, MOVE_FIRE_PLEDGE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(4); Moves(MOVE_CELEBRATE, MOVE_GRASS_PLEDGE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_TACKLE, target: playerRight); MOVE(opponentRight, MOVE_CELEBRATE); MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); SEND_OUT(playerRight, 2); }
        TURN { MOVE(opponentLeft, MOVE_FIRE_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_GRASS_PLEDGE, target: playerLeft); MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, playerLeft);
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when a disobedient partner uses another move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_FIRE_PLEDGE); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Level(50); OTName("Test"); Moves(MOVE_GRASS_PLEDGE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft, WITH_RNG(RNG_HNS_OBEDIENCE, 0x00FF)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: Instruct makes the user repeat its pledge out of turn; its own next move is not a combined move")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_INSTRUCT) == EFFECT_INSTRUCT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_FIRE_PLEDGE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_GRASS_PLEDGE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); Moves(MOVE_INSTRUCT, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(opponentLeft, MOVE_INSTRUCT, target: playerLeft); MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); MOVE(opponentRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_INSTRUCT, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, playerRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SWAMP, playerLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SWAMP, opponentLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the first pledge was called by Sleep Talk")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SLEEP_TALK) == EFFECT_SLEEP_TALK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_FIRE_PLEDGE); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_GRASS_PLEDGE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SLEEP_TALK, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLEEP_TALK, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the first pledge was called by Metronome")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_METRONOME) == EFFECT_METRONOME);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_METRONOME); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_GRASS_PLEDGE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_FIRE_PLEDGE)); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}

DOUBLE_BATTLE_TEST("Pledge guard: no combined move when the first pledge was copied by Me First")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ME_FIRST) == EFFECT_ME_FIRST);
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); Moves(MOVE_ME_FIRST); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_GRASS_PLEDGE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_FIRE_PLEDGE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ME_FIRST, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); MOVE(opponentLeft, MOVE_FIRE_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ME_FIRST, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, opponentLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        }
    }
}
