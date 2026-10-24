// HnS 회복 반짝임(B_ANIM_SIMPLE_HEAL) 회귀 테스트 — seq 428 #10416 A안, 친구 결정(2026-10-24b).
// 공통 회복 처리(BattleScript_PresentHealTarget)에서 실제로 회복하는 포켓몬(BS_TARGET)에게 한 번만 나온다.
// 이 경로를 타는 기술 애니 안의 개별 반짝임(HealingEffect)은 지웠다 — 애니 안쪽 호출이라 테스트로는 보이지 않고, 정적 대조로 확인.
// 회복·태만함·회복지령(EFFECT_RESTORE_HP)은 이 경로가 아니어서(BattleScript_EffectRestoreHp) 기술 애니 안 반짝임 그대로다.
// 이름이 모두 "HnS heal sparkle:"로 시작한다(TESTS='HnS heal sparkle'). 문장은 보지 않는다(언어 무관).
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS heal sparkle: self-healing moves play it once on the user")
{
    enum Move move = MOVE_NONE;
    PARAMETRIZE { move = MOVE_SOFT_BOILED; }
    PARAMETRIZE { move = MOVE_MILK_DRINK; }
    PARAMETRIZE { move = MOVE_ROOST; }
    PARAMETRIZE { move = MOVE_MORNING_SUN; }
    PARAMETRIZE { move = MOVE_SYNTHESIS; }
    PARAMETRIZE { move = MOVE_MOONLIGHT; }
    PARAMETRIZE { move = MOVE_SHORE_UP; }
    PARAMETRIZE { move = MOVE_REST; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); MaxHP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
    }
}

SINGLE_BATTLE_TEST("HnS heal sparkle: not played when the move fails at full HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SOFT_BOILED); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SOFT_BOILED, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
            HP_BAR(player);
        }
    }
}

// Present heals its target, so the sparkle goes on the target, not the user.
SINGLE_BATTLE_TEST("HnS heal sparkle: Present's heal plays it on the healed target")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { HP(100); MaxHP(400); }
    } WHEN {
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 254)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PRESENT, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
        HP_BAR(opponent);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
    }
}

// In doubles the right-hand Pokémon healing itself gets it, not its partner or the foes.
DOUBLE_BATTLE_TEST("HnS heal sparkle: goes on the Pokemon that heals in a double battle")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET) { HP(100); MaxHP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_SOFT_BOILED); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SOFT_BOILED, playerRight);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, playerLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponentLeft);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponentRight);
        }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, playerRight);
        HP_BAR(playerRight);
    }
}

// Mega Sol path: ability pop-up after the move animation, then the sparkle once (no second move animation).
SINGLE_BATTLE_TEST("HnS heal sparkle: Mega Sol healing plays the move animation once, then the sparkle once")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SYNTHESIS) == EFFECT_SYNTHESIS);
        PLAYER(SPECIES_MEGANIUM_MEGA) { Ability(ABILITY_MEGA_SOL); HP(100); MaxHP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SYNTHESIS); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SYNTHESIS, player);
        ABILITY_POPUP(player, ABILITY_MEGA_SOL);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SYNTHESIS, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
    }
}

// Recover, Slack Off and Heal Order don't use the common heal script: their own animation has the sparkle,
// and the general one is not added on top.
SINGLE_BATTLE_TEST("HnS heal sparkle: Recover-type moves don't add the general one")
{
    enum Move move = MOVE_NONE;
    PARAMETRIZE { move = MOVE_RECOVER; }
    PARAMETRIZE { move = MOVE_SLACK_OFF; }
    PARAMETRIZE { move = MOVE_HEAL_ORDER; }
    GIVEN {
        ASSUME(GetMoveEffect(move) == EFFECT_RESTORE_HP);
        PLAYER(SPECIES_WOBBUFFET) { HP(100); MaxHP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player);
    }
}
