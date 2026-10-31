// 재확인 89(1)(친구 결정 2026-10-31): 테스트 세이브의 "페어리 타입" 설정은 새 게임 기본값(1)과 같게 둔다
// (test/test_runner.c ClearSaveBlocks — 테스트마다 다시 설정). 페어리 없음(0) 모드가 필요한 테스트는 직접 0을 넣는다.
// 이름이 모두 "HnS fairy types"로 시작한다.
#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"

TEST("HnS fairy types: the test save starts with Fairy types on, like a new game")
{
    EXPECT_EQ((u32)gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types, 1);
    EXPECT_EQ(GetSpeciesType(SPECIES_WIGGLYTUFF, 1), TYPE_FAIRY);
    EXPECT_EQ(GetMoveType(MOVE_MOONBLAST), TYPE_FAIRY);
}

TEST("HnS fairy types: with Fairy types off, Wigglytuff is pure Normal and Fairy moves change type")
{
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types = 0;
    EXPECT_EQ(GetSpeciesType(SPECIES_WIGGLYTUFF, 0), TYPE_NORMAL);
    EXPECT_EQ(GetSpeciesType(SPECIES_WIGGLYTUFF, 1), TYPE_NORMAL);
    EXPECT_EQ(GetMoveType(MOVE_DAZZLING_GLEAM), TYPE_NORMAL); // sFairyMoveAltTypes (src/pokemon.c)
    EXPECT_EQ(GetMoveType(MOVE_MOONBLAST), TYPE_DARK);
}

// 실제 배틀: 페어리 있음이면 푸크린에게 드래곤애로우가 무효라 두 번 모두 옆 포켓몬에게 간다.
// 페어리 없음이면 푸크린(노말)과 옆 포켓몬에게 한 번씩 맞는다.
DOUBLE_BATTLE_TEST("HnS fairy types: Dragon Darts against Wigglytuff follows the Fairy types setting")
{
    u32 fairyTypes;
    PARAMETRIZE { fairyTypes = 1; }
    PARAMETRIZE { fairyTypes = 0; }
    GIVEN {
        gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types = fairyTypes;
        ASSUME(GetMoveTarget(MOVE_DRAGON_DARTS) == TARGET_SMART);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WIGGLYTUFF);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_DRAGON_DARTS, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DARTS, playerLeft);
        if (fairyTypes)
        {
            NOT HP_BAR(opponentLeft);
            HP_BAR(opponentRight);
            HP_BAR(opponentRight);
        }
        else
        {
            HP_BAR(opponentLeft);
            HP_BAR(opponentRight);
        }
    } THEN {
        if (fairyTypes)
            EXPECT_EQ(opponentLeft->hp, opponentLeft->maxHP);
        else
            EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
    }
}

// AI 평가: upstream 테스트 "AI sees Dragon Darts damage redirecting if one target is immune"와 같은 장면을 페어리 없음으로.
// 푸크린이 노말이라 드래곤애로우가 피해를 주므로 무효 감점(NO_DAMAGE_OR_FAILS)이 없고, 불꽃펀치(75)보다 약해
// 최고 피해 가점도 없다 → 기본 점수. 불꽃펀치는 페어리 있음일 때와 같이 최고 피해 기술.
AI_DOUBLE_BATTLE_TEST("HnS fairy types: with Fairy types off, the AI does not treat Wigglytuff as immune to Dragon Darts")
{
    GIVEN {
        gSaveBlock3Ptr->challengeSettings.tx_Mode_Fairy_Types = 0;
        ASSUME(GetMoveCategory(MOVE_DRAGON_DARTS) == DAMAGE_CATEGORY_PHYSICAL);
        ASSUME(GetMoveTarget(MOVE_DRAGON_DARTS) == TARGET_SMART);
        ASSUME(GetMovePower(MOVE_DRAGON_DARTS) == 50);
        ASSUME(GetMoveType(MOVE_FIRE_PUNCH) == TYPE_FIRE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WIGGLYTUFF) { Speed(4); Ability(ABILITY_CONTRARY); }
        PLAYER(SPECIES_DRATINI) { Speed(3); Level(60); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_DRAGON_DARTS, MOVE_FIRE_PUNCH); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_CELEBRATE); }
        TIE_BREAK_TARGET(TARGET_TIE_HI, 0);
        TIE_BREAK_SCORE(RNG_AI_SCORE_TIE_DOUBLES_MOVE, SCORE_TIE_LO, 0);
    } WHEN {
        TURN {
            SCORE_EQ_VAL(opponentLeft, MOVE_DRAGON_DARTS, AI_SCORE_DEFAULT, target: playerLeft);
            SCORE_EQ_VAL(opponentLeft, MOVE_FIRE_PUNCH, AI_SCORE_DEFAULT + BEST_DAMAGE_MOVE, target: playerLeft);
        }
    }
}
