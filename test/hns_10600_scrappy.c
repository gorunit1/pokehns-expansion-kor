// HnS seq 489 #10600 회귀 테스트 — 기술의 상성 배율을 첫 판정 때 저장해 대미지 계산에 다시 씀. 다단 기술 도중 배짱이 미라로
// 바뀌어도 고스트에게 두 번째 타격이 들어가고, 상성 문장도 그대로. 이름이 모두 "HnS 10600:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10600: Scrappy Double Hit keeps hitting a Ghost after Mummy replaces the ability")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_DOUBLE_HIT) == TYPE_NORMAL);
        ASSUME(GetMoveStrikeCount(MOVE_DOUBLE_HIT) == 2);
        PLAYER(SPECIES_KANGASKHAN) { Ability(ABILITY_SCRAPPY); }
        OPPONENT(SPECIES_COFAGRIGUS) { Ability(ABILITY_MUMMY); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_HIT); }
    } SCENE {
        MESSAGE("캥카는 더블어택을 썼다!");
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_MUMMY);
        MESSAGE("캥카는 미라가 되었다!");
        HP_BAR(opponent);
        MESSAGE("2번 맞았다!");
    } THEN {
        EXPECT_EQ(player->ability, ABILITY_MUMMY);
    }
}

// The stored multiplier is per target: a spread move still prints each foe's own effectiveness line.
DOUBLE_BATTLE_TEST("HnS 10600: a spread move prints each target's effectiveness")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_EARTHQUAKE) == TYPE_GROUND);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_PIKACHU);
        OPPONENT(SPECIES_CHIKORITA);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE); }
    } SCENE {
        MESSAGE("마자용은 지진을 썼다!");
        HP_BAR(opponentLeft);
        HP_BAR(playerRight);
        HP_BAR(opponentRight);
        MESSAGE("상대 피카츄에게 효과가 굉장했다!");
        MESSAGE("상대 치코리타에게 효과가 별로인 듯하다.");
    }
}
