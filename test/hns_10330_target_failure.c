// HnS seq 479 #10330 회귀 테스트 — 대상 실패 판정(CancelerTargetFailure)이 "대상마다 모든 검사"에서 "검사마다 모든 대상"
// (공중·땅속 → 사이코필드 → 방어 → 반사 → 특성·도구 → 상성 순)으로 바뀐 뒤의 한글 출력.
// upstream은 이 PR에서 쓰러진 대상을 거르는 분기를 지웠고 #10566(seq 486)에서야 다시 거른다 — HnS는 그 사이 쓰러진 대상을
// 이식 전처럼 거른다(재확인 97). 이름이 모두 "HnS 10330:"으로 시작한다.
#include "global.h"
#include "test/battle.h"

// A foe fainted earlier in the turn is not named by a spread move it would be immune to.
DOUBLE_BATTLE_TEST("HnS 10330: a spread move does not name a fainted immune foe")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        ASSUME(GetMoveType(MOVE_HYPER_VOICE) == TYPE_NORMAL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_GASTLY) { HP(1); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SHADOW_BALL, target: opponentLeft); MOVE(playerRight, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_BALL, playerLeft);
        MESSAGE("마자는 하이퍼보이스를 썼다!");
        NONE_OF {
            MESSAGE("상대 고오스에게는 효과가 없는 것 같다...");
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerRight);
        HP_BAR(opponentRight);
    }
}

// A fainted foe gets no HP bar from a spread move; the living foe is still hit.
DOUBLE_BATTLE_TEST("HnS 10330: a spread move skips a fainted foe and hits the other")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); MOVE(playerRight, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        HP_BAR(opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerRight);
        NOT HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    }
}

// Earthquake after a Levitate foe fainted: no Levitate popup or line for the fainted foe.
DOUBLE_BATTLE_TEST("HnS 10330: Earthquake does not show Levitate for a fainted foe")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_GASTLY) { Ability(ABILITY_LEVITATE); HP(1); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SHADOW_BALL, target: opponentLeft); MOVE(playerRight, MOVE_EARTHQUAKE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_BALL, playerLeft);
        MESSAGE("마자는 지진을 썼다!");
        NONE_OF {
            ABILITY_POPUP(opponentLeft, ABILITY_LEVITATE);
            MESSAGE("상대 고오스에게는 효과가 없는 것 같다...");
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerRight);
        HP_BAR(playerLeft);
        HP_BAR(opponentRight);
    }
}

// Order change: one foe flying (semi-invulnerable), the other protected. Every target's semi-invulnerability is
// checked before anyone's Protect, so the flying foe's line comes first whichever slot it is in.
DOUBLE_BATTLE_TEST("HnS 10330: a spread move names the flying foe before the protected foe")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(90); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(opponentRight, MOVE_FLY, target: playerLeft); MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        MESSAGE("마자용은 하이퍼보이스를 썼다!");
        MESSAGE("상대 마자용에게는 맞지 않았다!");
        MESSAGE("상대 마자는 공격으로부터 몸을 지켰다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
    }
}

// A move with hit: FALSE after a foe fainted: only the living targets get the miss line, not the fainted foe.
DOUBLE_BATTLE_TEST("HnS 10330: a missed spread move does not name a fainted foe")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Item(ITEM_BRIGHT_POWDER); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Item(ITEM_BRIGHT_POWDER); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); MOVE(playerRight, MOVE_EARTHQUAKE, hit: FALSE); }
    } SCENE {
        MESSAGE("마자용에게는 맞지 않았다!");
        NOT MESSAGE("상대 마자용에게는 맞지 않았다!");
        MESSAGE("상대 마자에게는 맞지 않았다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerRight);
    }
}

// A fainted Magic Bounce foe does not bounce a status move aimed at both foes.
DOUBLE_BATTLE_TEST("HnS 10330: a fainted Magic Bounce foe does not bounce Growl")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_GROWL) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_HATTERENE) { HP(1); Speed(2); Ability(ABILITY_MAGIC_BOUNCE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); MOVE(playerRight, MOVE_GROWL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        NOT ABILITY_POPUP(opponentLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, playerRight);
    } THEN {
        EXPECT_EQ(playerRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
    }
}
