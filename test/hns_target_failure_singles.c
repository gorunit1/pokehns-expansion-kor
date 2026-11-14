// QA b4048-24(친구 2026-11-14): 싱글 배틀에서 대상별 무효 문장이 3번 나오고 가운데 문장의 이름이 틀리던 문제.
// 원인: CancelerTargetFailure의 대상 반복이 MAX_BATTLERS_COUNT(4)까지 돌아, 싱글에 없는 2·3번 칸도 "잠들지 않은 대상"으로
// 검사됨. upstream #10467이 gBattlersCount로 고친 부분을 seq 441 이식 때 빠뜨렸다. 이름이 모두 "HnS target failure:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS target failure: Dream Eater on an awake target prints the no-effect message once, with the target's name")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DREAM_EATER) == EFFECT_DREAM_EATER);
        PLAYER(SPECIES_GARCHOMP) { MovesWithPP({MOVE_DREAM_EATER, 5}, {MOVE_CELEBRATE, 40}); }
        OPPONENT(SPECIES_GYARADOS);
    } WHEN {
        TURN { MOVE(player, MOVE_DREAM_EATER); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 갸라도스에게는 효과가 없는 것 같다...");
        NONE_OF {
            MESSAGE("상대 갸라도스에게는 효과가 없는 것 같다...");
            MESSAGE("상대 한카리아스에게는 효과가 없는 것 같다...");
            MESSAGE("한카리아스에게는 효과가 없는 것 같다...");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DREAM_EATER, player);
            HP_BAR(opponent);
            HP_BAR(player);
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player); // 다음 턴으로 정상 진행
    } THEN {
        EXPECT_EQ(opponent->hp, opponent->maxHP);
        EXPECT_EQ(player->hp, player->maxHP);
        EXPECT_EQ(opponent->status1, STATUS1_NONE);
        EXPECT_EQ(player->pp[0], 4);
    }
}

SINGLE_BATTLE_TEST("HnS target failure: Dream Eater still hits and drains from a sleeping target")
{
    GIVEN {
        PLAYER(SPECIES_GARCHOMP) { HP(1); }
        OPPONENT(SPECIES_GYARADOS) { Status1(STATUS1_SLEEP_TURN(3)); }
    } WHEN {
        TURN { MOVE(player, MOVE_DREAM_EATER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DREAM_EATER, player);
        HP_BAR(opponent);
        HP_BAR(player);
        NONE_OF {
            MESSAGE("상대 갸라도스에게는 효과가 없는 것 같다...");
            MESSAGE("상대 한카리아스에게는 효과가 없는 것 같다...");
            MESSAGE("한카리아스에게는 효과가 없는 것 같다...");
        }
    } THEN {
        EXPECT_LT(opponent->hp, opponent->maxHP);
        EXPECT_GT(player->hp, 1);
    }
}

// 같은 공통 처리: 죽기살기(대상 HP가 사용자 이하면 무효)
SINGLE_BATTLE_TEST("HnS target failure: Endeavor that cannot lower HP prints the no-effect message once")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ENDEAVOR) == EFFECT_ENDEAVOR);
        PLAYER(SPECIES_GARCHOMP);
        OPPONENT(SPECIES_GYARADOS) { HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_ENDEAVOR); }
    } SCENE {
        MESSAGE("상대 갸라도스에게는 효과가 없는 것 같다...");
        NONE_OF {
            MESSAGE("상대 갸라도스에게는 효과가 없는 것 같다...");
            MESSAGE("상대 한카리아스에게는 효과가 없는 것 같다...");
            MESSAGE("한카리아스에게는 효과가 없는 것 같다...");
            HP_BAR(opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->hp, 1);
    }
}
