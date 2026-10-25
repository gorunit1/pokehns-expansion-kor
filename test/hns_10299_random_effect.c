// HnS seq 475.6 #10299 회귀 테스트 — 트라이어택·페이탈클로의 무작위 추가 효과가 기술마다 둔 목록(MOVE_EFFECT_RANDOM_FROM_LIST)에서
// 고르게 된 뒤의 한글 상태이상 문장. 트라이어택 목록은 화상·마비·얼음(이전 코드는 화상·얼음·마비 순), 페이탈클로는 독·마비·잠듦.
// 이름이 모두 "HnS 10299:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10299: Tri Attack burns, paralyzes or freezes with the Korean line")
{
    u32 index;
    PARAMETRIZE { index = 0; }
    PARAMETRIZE { index = 1; }
    PARAMETRIZE { index = 2; }
    GIVEN {
        ASSUME(B_USE_FROSTBITE == FALSE);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TRI_ATTACK, WITH_RNG(RNG_RANDOM_FROM_LIST, index)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRI_ATTACK, player);
        HP_BAR(opponent);
        if (index == 0)
            MESSAGE("상대 마자용은 화상을 입었다!");
        else if (index == 1)
            MESSAGE("상대 마자용은 마비되어 기술이 나오기 어려워졌다!");
        else
            MESSAGE("상대 마자용은 얼어붙었다!");
    } THEN {
        if (index == 0)
            EXPECT(opponent->status1 & STATUS1_BURN);
        else if (index == 1)
            EXPECT(opponent->status1 & STATUS1_PARALYSIS);
        // a frozen foe may thaw when it tries to move this turn, so freeze is only checked by its line
    }
}

SINGLE_BATTLE_TEST("HnS 10299: Dire Claw poisons, paralyzes or puts to sleep with the Korean line")
{
    u32 index;
    PARAMETRIZE { index = 0; }
    PARAMETRIZE { index = 1; }
    PARAMETRIZE { index = 2; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_DIRE_CLAW, WITH_RNG(RNG_RANDOM_FROM_LIST, index)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DIRE_CLAW, player);
        HP_BAR(opponent);
        if (index == 0)
            MESSAGE("상대 마자용의 몸에 독이 퍼졌다!");
        else if (index == 1)
            MESSAGE("상대 마자용은 마비되어 기술이 나오기 어려워졌다!");
        else
            MESSAGE("상대 마자용은 잠들어 버렸다!");
    }
}

// A target that already has a status: no second status line.
SINGLE_BATTLE_TEST("HnS 10299: Tri Attack does not change an existing status")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRI_ATTACK, WITH_RNG(RNG_RANDOM_FROM_LIST, 0)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRI_ATTACK, player);
        HP_BAR(opponent);
        NOT MESSAGE("상대 마자용은 화상을 입었다!");
    } THEN {
        EXPECT(opponent->status1 & STATUS1_POISON);
        EXPECT(!(opponent->status1 & STATUS1_BURN));
    }
}
