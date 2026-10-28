// HnS seq 491 #10630 회귀 테스트 — 회복봉인·금제가 타이머 하나로만 판정됨(중복 플래그 삭제). 막는 동작·끝나는 문장·배턴터치가
// 이식 전과 같은지. 이름이 모두 "HnS 10630:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10630: Heal Block stops Recover and wears off after five turns")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HEAL_BLOCK) == EFFECT_HEAL_BLOCK);
        ASSUME(B_HEAL_BLOCK_TIMER == 5);
        PLAYER(SPECIES_WOBBUFFET) { HP(50); MaxHP(100); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_HEAL_BLOCK); MOVE(player, MOVE_RECOVER); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 회복 동작을 봉인당했다!");
        MESSAGE("마자용은 회복봉인 때문에 HP회복을 쓸 수 없다!");
        NOT HP_BAR(player);
        MESSAGE("마자용의 회복봉인 효과가 사라졌다!");
    } THEN {
        EXPECT(player->volatiles.healBlockTimer == 0);
    }
}

SINGLE_BATTLE_TEST("HnS 10630: Embargo stops Leftovers and wears off after five turns")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_EMBARGO) == EFFECT_EMBARGO);
        ASSUME(B_EMBARGO_TIMER == 5);
        PLAYER(SPECIES_WOBBUFFET) { HP(50); MaxHP(100); Item(ITEM_LEFTOVERS); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_EMBARGO); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 도구를 쓸 수 없게 되었다!");
        NOT HP_BAR(player);
        MESSAGE("마자용은 도구를 쓸 수 있게 되었다!");
    } THEN {
        EXPECT(player->volatiles.embargoTimer == 0);
    }
}

// Heal Block blocks the selection of Recover: with Recover as its only move the replacement Struggles.
SINGLE_BATTLE_TEST("HnS 10630: Baton Pass passes Heal Block to the replacement")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { HP(50); MaxHP(100); Speed(1); Moves(MOVE_RECOVER); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_HEAL_BLOCK); MOVE(player, MOVE_BATON_PASS); SEND_OUT(player, 1); }
        TURN { MOVE(player, MOVE_RECOVER, allowed: FALSE); }
    } SCENE {
        MESSAGE("마자용은 회복 동작을 봉인당했다!");
        MESSAGE("마자용은 배턴터치를 썼다!");
        MESSAGE("마자는 발버둥을 썼다!");
    } THEN {
        EXPECT(player->volatiles.healBlockTimer != 0);
    }
}
