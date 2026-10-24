// HnS 방어 성공 애니(B_ANIM_PROTECTED_ITSELF) 회귀 테스트 — seq 441 #10467 A안, 친구 결정(2026-10-24b).
// 방어 계열로 막으면 막은 포켓몬(BS_SCRIPTING)에게 방어막·흔들림 연출이 한 번 나온 뒤 기존 "몸을 지켰다" 문장.
// 이름이 모두 "HnS protect anim:"으로 시작한다(TESTS='HnS protect anim'). 문장은 보지 않는다(언어 무관).
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS protect anim: played once on the Pokemon that protected itself")
{
    enum Move move = MOVE_NONE;
    PARAMETRIZE { move = MOVE_PROTECT; }
    PARAMETRIZE { move = MOVE_DETECT; }
    PARAMETRIZE { move = MOVE_SPIKY_SHIELD; }
    PARAMETRIZE { move = MOVE_KINGS_SHIELD; }
    PARAMETRIZE { move = MOVE_BANEFUL_BUNKER; }
    PARAMETRIZE { move = MOVE_OBSTRUCT; }
    PARAMETRIZE { move = MOVE_SILK_TRAP; }
    PARAMETRIZE { move = MOVE_BURNING_BULWARK; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponent);
        }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
    }
}

// The contact effects still follow (Spiky Shield damage, King's Shield Attack drop).
SINGLE_BATTLE_TEST("HnS protect anim: contact effects still follow")
{
    enum Move move = MOVE_NONE;
    PARAMETRIZE { move = MOVE_SPIKY_SHIELD; }
    PARAMETRIZE { move = MOVE_KINGS_SHIELD; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
        if (move == MOVE_SPIKY_SHIELD)
            HP_BAR(opponent);
        else
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        if (move == MOVE_KINGS_SHIELD)
            EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE - (GetConfig(B_KINGS_SHIELD_LOWER_ATK) >= GEN_8 ? 1 : 2));
        else
            EXPECT_LT(opponent->hp, opponent->maxHP);
    }
}

// A multi-hit move is stopped once: one animation.
SINGLE_BATTLE_TEST("HnS protect anim: a multi-hit move shows it once")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_PROTECT); MOVE(opponent, MOVE_DOUBLE_KICK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
    }
}

SINGLE_BATTLE_TEST("HnS protect anim: not played when the attack isn't blocked")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponent);
        }
    }
}

// Doubles, spread move: each protecting foe gets it once, nobody else.
DOUBLE_BATTLE_TEST("HnS protect anim: each protecting target once against a spread move")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_DAZZLING_GLEAM) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {
            MOVE(opponentLeft, MOVE_PROTECT);
            MOVE(opponentRight, MOVE_PROTECT);
            MOVE(playerLeft, MOVE_DAZZLING_GLEAM);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, playerLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, playerRight);
        }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponentRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponentLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponentRight);
        }
    }
}

// Doubles, Wide Guard: the side guard shows it on each Pokémon it protects; the single-target hit on the
// unprotected ally is not blocked.
DOUBLE_BATTLE_TEST("HnS protect anim: Wide Guard shows it on each protected Pokemon")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_DAZZLING_GLEAM) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {
            MOVE(playerRight, MOVE_WIDE_GUARD);
            MOVE(opponentLeft, MOVE_DAZZLING_GLEAM);
            MOVE(opponentRight, MOVE_TACKLE, target: playerLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WIDE_GUARD, playerRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentRight);
        HP_BAR(playerLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, playerLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponentLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, opponentRight);
        }
    }
}

// Fling into Protect: the animation, then the item is still used up.
SINGLE_BATTLE_TEST("HnS protect anim: Fling into Protect still uses up the item")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FLING) == EFFECT_FLING);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RAZOR_CLAW); }
    } WHEN {
        TURN { MOVE(player, MOVE_PROTECT); MOVE(opponent, MOVE_FLING); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_PROTECTED_ITSELF, player);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}
