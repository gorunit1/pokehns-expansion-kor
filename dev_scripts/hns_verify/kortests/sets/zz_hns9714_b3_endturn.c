#include "global.h"
#include "test/battle.h"

// seq 139 #9714 area B: Korean output regression set, part 3 (end-of-turn side status and terrain expiry).
// Scratch only, never committed. Expected output = HnS before #9714 (code f3a58f9939).
// A space in MESSAGE matches one space or newline of the real text.
// Friend request A changes the Tailwind START text: only the Tailwind END text is expected here.
// Friend request B changes the Grassy Terrain heal text: every mon stays at full HP so it never prints.

SINGLE_BATTLE_TEST("HNS9714 3-01 Player-side Reflect, Light Screen, Mist, Aurora Veil, Safeguard, Tailwind, Lucky Chant expire")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_REFLECT; }
    PARAMETRIZE { move = MOVE_LIGHT_SCREEN; }
    PARAMETRIZE { move = MOVE_MIST; }
    PARAMETRIZE { move = MOVE_AURORA_VEIL; }
    PARAMETRIZE { move = MOVE_SAFEGUARD; }
    PARAMETRIZE { move = MOVE_TAILWIND; }
    PARAMETRIZE { move = MOVE_LUCKY_CHANT; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SNOWSCAPE); }
        TURN { MOVE(player, move); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        if (move == MOVE_REFLECT)
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        else if (move == MOVE_LIGHT_SCREEN)
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
        else if (move == MOVE_MIST)
            MESSAGE("우리 편을 감싸던 흰안개가 없어졌다!");
        else if (move == MOVE_AURORA_VEIL)
            MESSAGE("우리 편의 오로라베일이 없어졌다!");
        else if (move == MOVE_SAFEGUARD)
            MESSAGE("우리 편을 감싸던 신비의 베일이 없어졌다!");
        else if (move == MOVE_TAILWIND)
            MESSAGE("우리 편의 순풍이 멈췄다!");
        else
            MESSAGE("우리 편의 주술이 풀렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9714 3-02 Foe-side Reflect, Light Screen, Mist, Aurora Veil, Safeguard, Tailwind, Lucky Chant expire")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_REFLECT; }
    PARAMETRIZE { move = MOVE_LIGHT_SCREEN; }
    PARAMETRIZE { move = MOVE_MIST; }
    PARAMETRIZE { move = MOVE_AURORA_VEIL; }
    PARAMETRIZE { move = MOVE_SAFEGUARD; }
    PARAMETRIZE { move = MOVE_TAILWIND; }
    PARAMETRIZE { move = MOVE_LUCKY_CHANT; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SNOWSCAPE); }
        TURN { MOVE(opponent, move); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        if (move == MOVE_REFLECT)
            MESSAGE("상대의 리플렉터가 없어졌다!");
        else if (move == MOVE_LIGHT_SCREEN)
            MESSAGE("상대의 빛의장막이 없어졌다!");
        else if (move == MOVE_MIST)
            MESSAGE("상대를 감싸던 흰안개가 없어졌다!");
        else if (move == MOVE_AURORA_VEIL)
            MESSAGE("상대의 오로라베일이 없어졌다!");
        else if (move == MOVE_SAFEGUARD)
            MESSAGE("상대를 감싸던 신비의 베일이 없어졌다!");
        else if (move == MOVE_TAILWIND)
            MESSAGE("상대의 순풍이 멈췄다!");
        else
            MESSAGE("상대의 주술이 풀렸다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9714 3-03 Doubles: both sides expire in the same end of turn, player side first, block order per side")
{
    u32 mode;
    PARAMETRIZE { mode = 0; } // Reflect, Light Screen
    PARAMETRIZE { mode = 1; } // Safeguard, Mist
    PARAMETRIZE { mode = 2; } // Tailwind, Lucky Chant, Aurora Veil
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (mode == 0) {
            TURN { MOVE(playerLeft, MOVE_REFLECT); MOVE(playerRight, MOVE_LIGHT_SCREEN); MOVE(opponentLeft, MOVE_REFLECT); MOVE(opponentRight, MOVE_LIGHT_SCREEN); }
        } else if (mode == 1) {
            TURN { MOVE(playerLeft, MOVE_SAFEGUARD); MOVE(playerRight, MOVE_MIST); MOVE(opponentLeft, MOVE_SAFEGUARD); MOVE(opponentRight, MOVE_MIST); }
        } else {
            TURN { MOVE(playerLeft, MOVE_SNOWSCAPE); }
            TURN { MOVE(playerLeft, MOVE_AURORA_VEIL); MOVE(playerRight, MOVE_LUCKY_CHANT); MOVE(opponentLeft, MOVE_AURORA_VEIL); MOVE(opponentRight, MOVE_LUCKY_CHANT); }
            TURN { MOVE(playerLeft, MOVE_TAILWIND); MOVE(opponentLeft, MOVE_TAILWIND); }
        }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (mode == 0) {
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("상대의 빛의장막이 없어졌다!");
        } else if (mode == 1) {
            MESSAGE("우리 편을 감싸던 신비의 베일이 없어졌다!");
            MESSAGE("우리 편을 감싸던 흰안개가 없어졌다!");
            MESSAGE("상대를 감싸던 신비의 베일이 없어졌다!");
            MESSAGE("상대를 감싸던 흰안개가 없어졌다!");
        } else {
            MESSAGE("우리 편의 순풍이 멈췄다!");
            MESSAGE("우리 편의 주술이 풀렸다!");
            MESSAGE("우리 편의 오로라베일이 없어졌다!");
            MESSAGE("상대의 순풍이 멈췄다!");
            MESSAGE("상대의 주술이 풀렸다!");
            MESSAGE("상대의 오로라베일이 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNS9714 3-04 Pledge fields end: sea of fire, rainbow, swamp on the player side, the foe side, both")
{
    // Sea of fire end text names the side of the FASTEST battler (gBattlerAttacker = gBattlerByTurnOrder[0] at every
    // end-turn block call), not the side whose fire ends: RECHECK_BEFORE_COMPLETION.md #6, fixed upstream in seq 385
    // #10214. The four blocks #9714 changes (Reflect, Light Screen, Mist, Aurora Veil) do not touch it, because the
    // end-turn loop rewrites gBattlerAttacker before every block call. side 3 = both sides, foe faster.
    u32 field, side;
    PARAMETRIZE { field = 0; side = 0; }
    PARAMETRIZE { field = 0; side = 1; }
    PARAMETRIZE { field = 0; side = 2; }
    PARAMETRIZE { field = 0; side = 3; }
    PARAMETRIZE { field = 1; side = 0; }
    PARAMETRIZE { field = 1; side = 1; }
    PARAMETRIZE { field = 1; side = 2; }
    PARAMETRIZE { field = 1; side = 3; }
    PARAMETRIZE { field = 2; side = 0; }
    PARAMETRIZE { field = 2; side = 1; }
    PARAMETRIZE { field = 2; side = 2; }
    PARAMETRIZE { field = 2; side = 3; }
    if (field == 0) {
        if (side != 1) SetStartingStatus(STARTING_STATUS_SEA_OF_FIRE_PLAYER_TEMPORARY);
        if (side != 0) SetStartingStatus(STARTING_STATUS_SEA_OF_FIRE_OPPONENT_TEMPORARY);
    } else if (field == 1) {
        if (side != 1) SetStartingStatus(STARTING_STATUS_RAINBOW_PLAYER_TEMPORARY);
        if (side != 0) SetStartingStatus(STARTING_STATUS_RAINBOW_OPPONENT_TEMPORARY);
    } else {
        if (side != 1) SetStartingStatus(STARTING_STATUS_SWAMP_PLAYER_TEMPORARY);
        if (side != 0) SetStartingStatus(STARTING_STATUS_SWAMP_OPPONENT_TEMPORARY);
    }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(side == 3 ? 1 : 2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(side == 3 ? 2 : 1); }
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (field == 0) {
            if (side == 0) {
                MESSAGE("우리 편 주변의 불바다가 사라졌다!");
                NONE_OF { MESSAGE("상대 주변의 불바다가 사라졌다!"); }
            } else if (side == 1) {
                MESSAGE("우리 편 주변의 불바다가 사라졌다!"); // known wrong side (RECHECK #6)
                NONE_OF { MESSAGE("상대 주변의 불바다가 사라졌다!"); }
            } else if (side == 2) {
                MESSAGE("우리 편 주변의 불바다가 사라졌다!");
                MESSAGE("우리 편 주변의 불바다가 사라졌다!"); // known wrong side (RECHECK #6)
            } else {
                MESSAGE("상대 주변의 불바다가 사라졌다!"); // known wrong side (RECHECK #6)
                MESSAGE("상대 주변의 불바다가 사라졌다!");
            }
        } else if (field == 1) {
            if (side == 0) {
                MESSAGE("우리 편 하늘에서 무지개가 사라졌다!");
            } else if (side == 1) {
                MESSAGE("상대 하늘에서 무지개가 사라졌다!");
            } else {
                MESSAGE("우리 편 하늘에서 무지개가 사라졌다!");
                MESSAGE("상대 하늘에서 무지개가 사라졌다!");
            }
        } else {
            if (side == 0) {
                MESSAGE("우리 편 주변의 습지초원이 사라졌다!");
            } else if (side == 1) {
                MESSAGE("상대 주변의 습지초원이 사라졌다!");
            } else {
                MESSAGE("우리 편 주변의 습지초원이 사라졌다!");
                MESSAGE("상대 주변의 습지초원이 사라졌다!");
            }
        }
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9714 3-05 Terrain ends at end of turn (set by player or foe): text and background animation")
{
    enum Move move;
    u32 user;
    PARAMETRIZE { move = MOVE_ELECTRIC_TERRAIN; user = 0; }
    PARAMETRIZE { move = MOVE_MISTY_TERRAIN;    user = 0; }
    PARAMETRIZE { move = MOVE_PSYCHIC_TERRAIN;  user = 0; }
    PARAMETRIZE { move = MOVE_GRASSY_TERRAIN;   user = 0; }
    PARAMETRIZE { move = MOVE_ELECTRIC_TERRAIN; user = 1; }
    PARAMETRIZE { move = MOVE_MISTY_TERRAIN;    user = 1; }
    PARAMETRIZE { move = MOVE_PSYCHIC_TERRAIN;  user = 1; }
    PARAMETRIZE { move = MOVE_GRASSY_TERRAIN;   user = 1; }
    PARAMETRIZE { move = MOVE_ELECTRIC_TERRAIN; user = 2; } // foe faster: background animation from the foe
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(user == 2 ? 1 : 2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(user == 2 ? 2 : 1); }
    } WHEN {
        if (user == 0)
            TURN { MOVE(player, move); }
        else
            TURN { MOVE(opponent, move); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (move == MOVE_ELECTRIC_TERRAIN)
            MESSAGE("발밑의 전기가 사라졌다!");
        else if (move == MOVE_MISTY_TERRAIN)
            MESSAGE("발밑의 안개가 사라졌다!");
        else if (move == MOVE_PSYCHIC_TERRAIN)
            MESSAGE("발밑의 이상한 느낌이 사라졌다!");
        else
            MESSAGE("발밑의 풀이 사라졌다!");
        if (user == 2)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, opponent);
        else
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, player);
    }
}

SINGLE_BATTLE_TEST("HNS9714 3-06 Electric Terrain ends at end of turn: Booster Energy activates Quark Drive")
{
    GIVEN {
        PLAYER(SPECIES_IRON_MOTH) { Attack(100); Defense(100); Speed(100); SpAttack(110); SpDefense(100); Ability(ABILITY_QUARK_DRIVE); Item(ITEM_BOOSTER_ENERGY); }
        OPPONENT(SPECIES_TAPU_KOKO) { Speed(1); Ability(ABILITY_ELECTRIC_SURGE); }
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_ELECTRIC_SURGE);
        ABILITY_POPUP(player, ABILITY_QUARK_DRIVE);
        MESSAGE("발밑의 전기가 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ABILITY_POPUP(player, ABILITY_QUARK_DRIVE);
    }
}
