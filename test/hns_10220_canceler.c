// HnS seq 476 #10220 회귀 테스트 — 공격 전 효과(내던지기·폴터가이스트 문장)와 대미지 계산이 공격 취소 단계로 옮겨지고,
// 꿈먹기·트랩셸이 일반 공격 스크립트를 쓰게 된 뒤의 한글 출력과 순서. 이름이 모두 "HnS 10220:"으로 시작한다.
#include "global.h"
#include "test/battle.h"

// Dream Eater on an awake target: the "doesn't affect" line naming the target, no animation, no damage.
SINGLE_BATTLE_TEST("HnS 10220: Dream Eater does not affect an awake target")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DREAM_EATER) == EFFECT_DREAM_EATER);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_DREAM_EATER); }
    } SCENE {
        MESSAGE("마자용은 꿈먹기를 썼다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DREAM_EATER, player);
            HP_BAR(opponent);
        }
        MESSAGE("상대 마자에게는 효과가 없는 것 같다...");
    } THEN {
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

// Dream Eater on a sleeping target hits and drains.
SINGLE_BATTLE_TEST("HnS 10220: Dream Eater hits a sleeping target")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP); }
    } WHEN {
        TURN { MOVE(player, MOVE_DREAM_EATER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DREAM_EATER, player);
        HP_BAR(opponent);
        HP_BAR(player);
    }
}

// Shell Trap without being hit by a physical move: the failure line after the move name, no animation.
SINGLE_BATTLE_TEST("HnS 10220: Shell Trap fails when the user was not hit")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SHELL_TRAP) == EFFECT_SHELL_TRAP);
        PLAYER(SPECIES_TURTONATOR);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_TRAP); }
    } SCENE {
        MESSAGE("폭거북스는 트랩셸을 썼다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, player);
        MESSAGE("폭거북스의 트랩셸은 불발로 끝났다!");
    }
}

// Shell Trap after a physical hit: it goes off and damages the foe.
SINGLE_BATTLE_TEST("HnS 10220: Shell Trap goes off after a physical hit")
{
    GIVEN {
        PLAYER(SPECIES_TURTONATOR);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_TRAP); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, player);
        HP_BAR(opponent);
    }
}

// Present with a damaging roll goes through the attack animation and HP bar.
SINGLE_BATTLE_TEST("HnS 10220: Present deals damage on a damaging roll")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PRESENT) == EFFECT_PRESENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 1)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PRESENT, player);
        HP_BAR(opponent);
    } THEN {
        EXPECT_LT(opponent->hp, opponent->maxHP);
    }
}

// A Gem: its line comes before the move animation (from the damage calculation step).
SINGLE_BATTLE_TEST("HnS 10220: a Gem's message comes before the move animation")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_TACKLE) == TYPE_NORMAL);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_NORMAL_GEM); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        MESSAGE("마자용은 몸통박치기를 썼다!");
        MESSAGE("노말주얼은 몸통박치기의 위력을 강하게 했다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

// Fling: the pre-attack "flung" line, then the hit.
SINGLE_BATTLE_TEST("HnS 10220: Fling names the flung item before the hit")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FLING) == EFFECT_FLING);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FLING); }
    } SCENE {
        MESSAGE("마자용은 내던지기를 썼다!");
        MESSAGE("마자용은 먹다남은음식을 내던졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLING, player);
        HP_BAR(opponent);
    }
}

// Poltergeist: the pre-attack line naming the target's item, then the hit.
SINGLE_BATTLE_TEST("HnS 10220: Poltergeist names the target's item before the hit")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_POLTERGEIST) == EFFECT_POLTERGEIST);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN { MOVE(player, MOVE_POLTERGEIST); }
    } SCENE {
        MESSAGE("마자용은 폴터가이스트를 썼다!");
        MESSAGE("먹다남은음식이 상대 마자에게 덤벼들었다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POLTERGEIST, player);
        HP_BAR(opponent);
    }
}

// A spread move calculates damage for each target once (both foes take damage).
DOUBLE_BATTLE_TEST("HnS 10220: a spread move damages each foe")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    }
}
