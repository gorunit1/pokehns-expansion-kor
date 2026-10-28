// HnS seq 492 #10640 회귀 테스트 — 문장 대기 표시(MSG_DISPLAY)를 상수·도우미 함수로 바꾼 뒤에도 급소·변환자재·상성 문장의
// 순서가 같은지. 이름이 모두 "HnS 10640:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10640: a critical hit prints its line before the effectiveness line")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_WATER_GUN) == TYPE_WATER);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHARMANDER);
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN, criticalHit: TRUE); }
    } SCENE {
        MESSAGE("마자용은 물대포를 썼다!");
        HP_BAR(opponent);
        MESSAGE("급소에 맞았다!");
        MESSAGE("효과가 굉장했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10640: Protean prints the type change before the move animation")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_WATER_GUN) == TYPE_WATER);
        PLAYER(SPECIES_FROAKIE) { Ability(ABILITY_PROTEAN); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        MESSAGE("개구마르는 몸통박치기를 썼다!");
        ABILITY_POPUP(player, ABILITY_PROTEAN);
        MESSAGE("개구마르는 노말타입이 됐다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
    }
}
