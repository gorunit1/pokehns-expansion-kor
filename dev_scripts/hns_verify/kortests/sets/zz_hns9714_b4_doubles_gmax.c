#include "global.h"
#include "test/battle.h"

// seq 139 #9714 area B: Korean output regression set, part 4 (doubles Defog with right-side users,
// the attacker after Defog, G-Max Wind Rage = MOVE_EFFECT_DEFOG). Scratch only, never committed.
// Expected output = HnS before #9714 (code f3a58f9939).
// A space in MESSAGE matches one space or newline of the real text.

DOUBLE_BATTLE_TEST("HNS9714 4-01 Doubles: right-side Defog users (player and foe), screens, hazards both sides, later movers keep their names")
{
    u32 user;
    PARAMETRIZE { user = 0; } // playerRight
    PARAMETRIZE { user = 1; } // opponentRight
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(8); }
        PLAYER(SPECIES_WYNAUT) { Speed(user == 0 ? 6 : 1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(7); }
        OPPONENT(SPECIES_WYNAUT) { Speed(user == 0 ? 1 : 6); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SPIKES); MOVE(opponentLeft, MOVE_STEALTH_ROCK); }
        TURN { MOVE(playerLeft, MOVE_REFLECT); MOVE(opponentLeft, MOVE_LIGHT_SCREEN); }
        if (user == 0)
            TURN { MOVE(playerRight, MOVE_DEFOG, target: opponentRight); }
        else
            TURN { MOVE(opponentRight, MOVE_DEFOG, target: playerRight); }
    } SCENE {
        if (user == 0) {
            MESSAGE("마자용은 축하를 썼다!");
            MESSAGE("상대 마자용은 축하를 썼다!");
            MESSAGE("마자는 안개제거를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, playerRight);
            MESSAGE("상대 마자의 회피율이 떨어졌다!");
            MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
            MESSAGE("상대의 빛의장막이 없어졌다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
            MESSAGE("상대 마자는 축하를 썼다!");
        } else {
            MESSAGE("마자용은 축하를 썼다!");
            MESSAGE("상대 마자용은 축하를 썼다!");
            MESSAGE("상대 마자는 안개제거를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, opponentRight);
            MESSAGE("마자의 회피율이 떨어졌다!");
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
            MESSAGE("마자는 축하를 썼다!");
        }
    }
}

DOUBLE_BATTLE_TEST("HNS9714 4-02 UPSTREAM CHANGE EXPECTED: right-side player Defog against a Substitute with only terrain to clear")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SUBSTITUTE); MOVE(opponentRight, MOVE_REFLECT); }
        TURN { MOVE(playerLeft, MOVE_ELECTRIC_TERRAIN); MOVE(playerRight, MOVE_DEFOG, target: opponentLeft); }
    } SCENE {
        MESSAGE("마자는 안개제거를 썼다!");
        // Before #9714 the check pass returns on the terrain with gBattlerAttacker = side 0 = playerLeft:
        // the Defog animation starts from playerLeft, the rest of the move runs with playerLeft as attacker.
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, playerLeft);
        NONE_OF { MESSAGE("상대 마자용의 회피율이 떨어졌다!"); }
        MESSAGE("발밑의 전기가 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, playerLeft);
        MESSAGE("상대의 리플렉터가 없어졌다!");
    } THEN {
        // Before #9714 the move end also runs with playerLeft as the attacker: playerLeft's last move becomes
        // Defog too (it used Electric Terrain).
        EXPECT_EQ(gLastMoves[B_POSITION_PLAYER_LEFT], MOVE_DEFOG);
        EXPECT_EQ(gLastMoves[B_POSITION_PLAYER_RIGHT], MOVE_DEFOG);
    }
}

SINGLE_BATTLE_TEST("HNS9714 4-03 UPSTREAM CHANGE EXPECTED: G-Max Wind Rage against foe-side screens (and the user's own)")
{
    u32 mode;
    PARAMETRIZE { mode = 0; } // Reflect on the foe side only
    PARAMETRIZE { mode = 1; } // Reflect on both sides
    GIVEN {
        PLAYER(SPECIES_CORVIKNIGHT) { GigantamaxFactor(TRUE); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        if (mode == 0)
            TURN { MOVE(opponent, MOVE_REFLECT); }
        else
            TURN { MOVE(opponent, MOVE_REFLECT); MOVE(player, MOVE_REFLECT); }
        TURN { MOVE(player, MOVE_PECK, gimmick: GIMMICK_DYNAMAX); }
        TURN {}
    } SCENE {
        // Before #9714 BattleScript_MoveEffectDefog runs TryDefogClear(gEffectBattler = the TARGET): screens are
        // cleared on every side except the target's, i.e. the user's own side; the foe's Reflect stays.
        MESSAGE("아머까오는 거다이풍격을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_G_MAX_WIND_RAGE, player);
        HP_BAR(opponent);
        if (mode == 1)
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        NONE_OF { MESSAGE("상대의 리플렉터가 없어졌다!"); }
        MESSAGE("아머까오는 다이월을 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9714 4-04 G-Max Wind Rage removes hazards on both sides and terrain")
{
    GIVEN {
        PLAYER(SPECIES_CORVIKNIGHT) { GigantamaxFactor(TRUE); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPIKES); MOVE(player, MOVE_STEALTH_ROCK); }
        TURN { MOVE(opponent, MOVE_PSYCHIC_TERRAIN); MOVE(player, MOVE_PECK, gimmick: GIMMICK_DYNAMAX); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_G_MAX_WIND_RAGE, player);
        HP_BAR(opponent);
        MESSAGE("우리 편 발밑의 압정이 사라졌다!");
        MESSAGE("발밑의 이상한 느낌이 사라졌다!");
        MESSAGE("상대 주변의 스텔스록이 사라졌다!");
    }
}
