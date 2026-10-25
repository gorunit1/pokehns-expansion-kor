// HnS seq 482 #10362 + 483 #10431 + 493 #10714 회귀 테스트 — 공격 애니·효과음·피격·HP 바·대미지 반영을 스크립트
// (attackanimation … datahpupdate)에서 공격 취소 단계로 옮긴 뒤의 한글 출력과 순서. 이름이 모두 "HnS 10362:"로 시작한다.
#include "global.h"
#include "test/battle.h"

// Friend decision Q9 (2026-10-04): the resist berry popup and animation come before the attack animation,
// the "weakened the damage" line after the hit (#10431).
SINGLE_BATTLE_TEST("HnS 10362: a resist berry animates before the attack and its line follows the hit")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_BITE) == TYPE_DARK);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_COLBUR_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_BITE, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } SCENE {
        MESSAGE("상대 마자는 물기를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITE, opponent);
        HP_BAR(player);
        MESSAGE("효과가 굉장했다!");
        MESSAGE("마자용은 입는 데미지를 마코열매가 약하게 했다!");
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

// A single-target move in a double battle: only the target's substitute line, not the partner's.
DOUBLE_BATTLE_TEST("HnS 10362: a substitute on the untargeted foe does not show the substitute line")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(20); }
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_SUBSTITUTE); }
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponentRight);
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        HP_BAR(opponentLeft);
        NONE_OF {
            MESSAGE("상대 마자를 대신하여 대타가 공격을 받았다!");
        }
    }
}

// The targeted substitute still takes the hit with its line.
SINGLE_BATTLE_TEST("HnS 10362: a substitute takes the hit with its line and no HP bar")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(20); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 마자를 대신하여 대타가 공격을 받았다!");
        NOT HP_BAR(opponent);
    }
}

// Future Sight landing: the line, then the move's own hit animation (two-turn branch), then the HP bar.
SINGLE_BATTLE_TEST("HnS 10362: Future Sight lands with its hit animation and HP bar")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FUTURE_SIGHT, player);
        MESSAGE("마자용은 미래의 공격을 예지했다!");
        MESSAGE("상대 마자는 미래예지 공격을 받았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FUTURE_SIGHT, player);
        HP_BAR(opponent);
    }
}

// Strong winds: the weakened-attack line before the attack animation.
SINGLE_BATTLE_TEST("HnS 10362: the strong winds line comes before the attack animation")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_ICE_BEAM) == TYPE_ICE);
        PLAYER(SPECIES_RAYQUAZA) { Ability(ABILITY_DELTA_STREAM); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ICE_BEAM, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } SCENE {
        MESSAGE("상대 마자는 냉동빔을 썼다!");
        MESSAGE("수수께끼의 난기류가 공격을 약하게 만들었다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_BEAM, opponent);
        HP_BAR(player);
    }
}

// Endeavor against a target with no more HP than the user: the "doesn't affect" line, no animation.
SINGLE_BATTLE_TEST("HnS 10362: Endeavor does not affect a target with less HP than the user")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ENDEAVOR) == EFFECT_ENDEAVOR);
        PLAYER(SPECIES_WOBBUFFET) { HP(200); }
        OPPONENT(SPECIES_WYNAUT) { HP(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_ENDEAVOR); }
    } SCENE {
        MESSAGE("마자용은 죽기살기를 썼다!");
        MESSAGE("상대 마자에게는 효과가 없는 것 같다...");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_ENDEAVOR, player);
    } THEN {
        EXPECT_EQ(opponent->hp, 50);
    }
}

// A spread move hitting both foes: one animation, both HP bars, then each result line.
DOUBLE_BATTLE_TEST("HnS 10362: a spread move plays one animation and both HP bars")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
    }
}
