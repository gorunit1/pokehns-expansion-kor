// HnS AI 테라스탈 설정(B_AI_TERASTALLIZATION) 회귀 테스트 — 친구 결정(2026-10-24b).
// FALSE(HnS 기본): AI가 조종하는 트레이너(상대·인게임 파트너)는 테라 타입 지정·스마트 테라와 무관하게 테라스탈 불가.
// TRUE: 기존 조건과 AI 판단대로. 플레이어 쪽 조건은 설정과 무관.
// 이름이 모두 "HnS AI Tera:"로 시작한다(TESTS='HnS AI Tera'). 문장은 보지 않고 테라 애니·AI 선택만 본다(언어 무관).
// 테스트 러너는 테라 사용 의도를 TeraType()/gimmick 입력에서 받는다(ShouldTrainerBattlerUseGimmick의 TESTING 경로).
// 새 제한은 그보다 앞의 공통 판정(CanTerastallize)에 있어 이 우회로 건너뛰지 않는다.
#include "global.h"
#include "test/battle.h"

TEST("HnS AI Tera: B_AI_TERASTALLIZATION is FALSE in HnS")
{
    EXPECT_EQ(GetConfig(B_AI_TERASTALLIZATION), FALSE);
}

// A set Tera Type, default AI (Terastallizes whenever it can).
AI_SINGLE_BATTLE_TEST("HnS AI Tera: AI opponent with a set Tera Type Terastallizes only when allowed")
{
    u32 allowed = FALSE;
    PARAMETRIZE { allowed = FALSE; }
    PARAMETRIZE { allowed = TRUE; }
    GIVEN {
        WITH_CONFIG(B_AI_TERASTALLIZATION, allowed);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE); TeraType(TYPE_NORMAL); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_TACKLE, gimmick: allowed ? GIMMICK_TERA : GIMMICK_NONE); }
    } SCENE {
        if (allowed)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
        else
            NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
    }
}

// Smart Tera AI: same position as the upstream "AI will tera if it enables a ko" test.
AI_SINGLE_BATTLE_TEST("HnS AI Tera: Smart Tera AI opponent Terastallizes only when allowed")
{
    u32 allowed = FALSE;
    PARAMETRIZE { allowed = FALSE; }
    PARAMETRIZE { allowed = TRUE; }
    GIVEN {
        WITH_CONFIG(B_AI_TERASTALLIZATION, allowed);
        ASSUME(GetMovePower(MOVE_SEED_BOMB) == 80);
        ASSUME(GetMovePower(MOVE_AQUA_TAIL) == 90);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_SMART_TERA);
        PLAYER(SPECIES_WOBBUFFET) { HP(47); Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_AQUA_TAIL, MOVE_SEED_BOMB); TeraType(TYPE_GRASS); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(100); TeraType(TYPE_FIRE); }
    } WHEN {
        if (allowed)
            TURN { EXPECT_MOVE(opponent, MOVE_SEED_BOMB, gimmick: GIMMICK_TERA); SEND_OUT(player, 1); }
        else
            TURN { }
    } SCENE {
        if (allowed)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
        else
            NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
    }
}

// The in-game partner (like Lance) is AI-controlled too.
AI_TWO_VS_ONE_BATTLE_TEST("HnS AI Tera: AI partner Terastallizes only when allowed")
{
    u32 allowed = FALSE;
    PARAMETRIZE { allowed = FALSE; }
    PARAMETRIZE { allowed = TRUE; }
    GIVEN {
        WITH_CONFIG(B_AI_TERASTALLIZATION, allowed);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE); TeraType(TYPE_NORMAL); }
        OPPONENT_A(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            EXPECT_MOVE(playerRight, MOVE_TACKLE, gimmick: allowed ? GIMMICK_TERA : GIMMICK_NONE);
            EXPECT_MOVE(opponentLeft, MOVE_CELEBRATE);
            EXPECT_MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        if (allowed)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, playerRight);
        else
            NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, playerRight);
    }
}

// Battlers the AI doesn't control are not blocked: in non-AI battle tests the opponent's input comes from the
// test script through the recorded controller (like a link opponent), so it Terastallizes whatever the setting.
SINGLE_BATTLE_TEST("HnS AI Tera: an opponent not controlled by the AI is not blocked")
{
    u32 allowed = FALSE;
    PARAMETRIZE { allowed = FALSE; }
    PARAMETRIZE { allowed = TRUE; }
    GIVEN {
        WITH_CONFIG(B_AI_TERASTALLIZATION, allowed);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { TeraType(TYPE_NORMAL); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE, gimmick: GIMMICK_TERA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
    }
}

// The player is never blocked by this setting (the Tera Orb checks are skipped in tests, as before).
SINGLE_BATTLE_TEST("HnS AI Tera: the player Terastallizes whatever the setting")
{
    u32 allowed = FALSE;
    PARAMETRIZE { allowed = FALSE; }
    PARAMETRIZE { allowed = TRUE; }
    GIVEN {
        WITH_CONFIG(B_AI_TERASTALLIZATION, allowed);
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, gimmick: GIMMICK_TERA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
    }
}

// Mega Evolution (and the other gimmicks) of an AI opponent are not affected by the setting.
AI_SINGLE_BATTLE_TEST("HnS AI Tera: AI opponent still Mega Evolves when Tera is not allowed")
{
    GIVEN {
        WITH_CONFIG(B_AI_TERASTALLIZATION, FALSE);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VENUSAUR) { Item(ITEM_VENUSAURITE); Moves(MOVE_TACKLE); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_TACKLE, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
    }
}
