// HnS seq 471.5 #10144 회귀 테스트 — 타입 무효를 공격 취소 단계에서 대상마다 판정(BattleScript_DoesntAffectScripting).
// 더블에서 두 상대가 모두 무효면 이전의 "X와 Y에게는 효과가 없는 것 같다…" 한 줄(STRINGID_ITDOESNTAFFECTTWOFOES, upstream 삭제) 대신
// 대상마다 "…에게는 효과가 없는 것 같다..."가 기술 애니 전에 나온다. 이름이 모두 "HnS 10144:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10144: a move the target is immune to names the target before any animation")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GASTLY);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        MESSAGE("마자용은 몸통박치기를 썼다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        MESSAGE("상대 고오스에게는 효과가 없는 것 같다...");
    }
}

DOUBLE_BATTLE_TEST("HnS 10144: a spread move both foes are immune to names each foe")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        ASSUME(GetMoveType(MOVE_HYPER_VOICE) == TYPE_NORMAL);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_GASTLY);
        OPPONENT(SPECIES_HAUNTER);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        MESSAGE("마자용은 하이퍼보이스를 썼다!");
        MESSAGE("상대 고오스에게는 효과가 없는 것 같다...");
        MESSAGE("상대 고우스트에게는 효과가 없는 것 같다...");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
    }
}

// Only one foe immune: that foe is named, the other is hit.
DOUBLE_BATTLE_TEST("HnS 10144: a spread move one foe is immune to names that foe and hits the other")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_GASTLY);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        MESSAGE("상대 고오스에게는 효과가 없는 것 같다...");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentRight);
        NOT HP_BAR(opponentLeft);
    }
}
