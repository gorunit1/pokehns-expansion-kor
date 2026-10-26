// HnS seq 475.5 #10151 + 475.8 #10282 회귀 테스트 — Champions 배틀 메커니즘(friend decision 2026-10-26, RECHECK 99
// plan A: the new configs follow HnS GEN_LATEST, which is GEN_CHAMPIONS in HnS). 이름이 모두 "HnS 10151:"로 시작한다.
#include "global.h"
#include "test/battle.h"

// Champions BTL_STRID_SET_GeppuFail: Belch can be selected without a Berry and fails with the official line.
SINGLE_BATTLE_TEST("HnS 10151: Belch without an eaten Berry fails with the Champions line")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_BELCH) == EFFECT_BELCH);
        ASSUME(GetConfig(B_BELCH_SELECTABLE) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELCH); }
    } SCENE {
        MESSAGE("마자용은 트림을 썼다!");
        MESSAGE("마자용은 나무열매를 먹지 않아서 기술을 쓸 수 없다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_BELCH, player);
    }
}

// BTL_STRID_SET_GeppuFail_E: the opposing Pokemon's line starts with 상대.
SINGLE_BATTLE_TEST("HnS 10151: an opposing Belch without an eaten Berry names the opposing Pokemon")
{
    GIVEN {
        ASSUME(GetConfig(B_BELCH_SELECTABLE) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELCH); }
    } SCENE {
        MESSAGE("상대 마자는 트림을 썼다!");
        MESSAGE("상대 마자는 나무열매를 먹지 않아서 기술을 쓸 수 없다!");
    }
}

// Fake Out cannot be selected after the user's first turn: with Fake Out as its only move the user Struggles
// (as upstream's Last Resort Champions test; allowed: FALSE alone does not check that the selection is refused).
SINGLE_BATTLE_TEST("HnS 10151: Fake Out cannot be selected after the first turn")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FAKE_OUT) == EFFECT_FIRST_TURN_ONLY);
        ASSUME(GetConfig(B_FIRST_TURN_MOVE) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_FAKE_OUT); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FAKE_OUT); }
        TURN { MOVE(player, MOVE_FAKE_OUT, allowed: FALSE); }
    } SCENE {
        MESSAGE("마자용은 속이기를 썼다!");
        MESSAGE("마자용은 발버둥을 썼다!");
    }
}

// HnS fix (upstream #10151 sets freezeTurns to 2 when a move freezes): a Pokemon frozen by a move stays frozen
// while the thaw roll fails, and thaws on its third frozen action at the latest.
SINGLE_BATTLE_TEST("HnS 10151: a Pokemon frozen by a move stays frozen for two actions and thaws on the third")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_ICE_BEAM, MOVE_EFFECT_FREEZE_OR_FROSTBITE));
        ASSUME(GetConfig(B_FREEZE_TURNS) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_BEAM, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_FROZEN, FALSE)); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_FROZEN, FALSE)); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_FROZEN, FALSE)); }
    } SCENE {
        MESSAGE("상대 마자는 얼어붙었다!");
        MESSAGE("상대 마자는 얼어버려서 움직이지 않는다!");
        MESSAGE("상대 마자는 얼어버려서 움직이지 않는다!");
        MESSAGE("상대 마자의 얼음이 녹았다!");
        MESSAGE("상대 마자는 축하를 썼다!");
    }
}

// Champions paralysis: 1 in 8 to be fully paralyzed (weighted roll, #10282).
SINGLE_BATTLE_TEST("HnS 10151: paralysis stops the move 1 time in 8")
{
    PASSES_RANDOMLY(1, 8, RNG_PARALYSIS);
    GIVEN {
        ASSUME(GetConfig(B_PARALYSIS_CHANCE) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET) { Status1(STATUS1_PARALYSIS); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자용은 몸이 저려서 움직일 수 없다");
    }
}

// Champions Salt Cure: 1/16 of max HP at the end of the turn (1/8 for Water and Steel).
SINGLE_BATTLE_TEST("HnS 10151: Salt Cure deals 1/16 at the end of the turn")
{
    s16 damage;
    GIVEN {
        ASSUME(GetConfig(B_SALT_CURE_DAMAGE) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(320); MaxHP(320); }
    } WHEN {
        TURN { MOVE(player, MOVE_SALT_CURE); }
    } SCENE {
        MESSAGE("상대 마자는 소금에 절여졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SALT_CURE_DAMAGE, opponent);
        HP_BAR(opponent, captureDamage: &damage);
        MESSAGE("상대 마자는 소금절이의 데미지를 입고 있다.");
    } THEN {
        EXPECT_EQ(damage, 320 / 16);
    }
}

// The brokeProtect argument (#10151 data, #10426 field): Feint keeps its own line, Phantom Force now prints the
// "broke through" line instead of the Feint line.
SINGLE_BATTLE_TEST("HnS 10151: Feint and Phantom Force through Protect print their own lines")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_FEINT; }
    PARAMETRIZE { move = MOVE_PHANTOM_FORCE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(20); }
    } WHEN {
        if (move == MOVE_FEINT) {
            TURN { MOVE(opponent, MOVE_PROTECT); MOVE(player, MOVE_FEINT); }
        } else {
            TURN { MOVE(player, MOVE_PHANTOM_FORCE); }
            TURN { MOVE(opponent, MOVE_PROTECT); SKIP_TURN(player); }
        }
    } SCENE {
        if (move == MOVE_FEINT) {
            MESSAGE("상대 마자는 페인트에 걸렸다!");
            NOT MESSAGE("상대 마자의 방어를 깨뜨렸다!");
        } else {
            MESSAGE("상대 마자의 방어를 깨뜨렸다!");
            NOT MESSAGE("상대 마자는 페인트에 걸렸다!");
        }
    }
}

// Champions faint timing: Knock Off still removes the item when the user faints to Rocky Helmet.
SINGLE_BATTLE_TEST("HnS 10151: Knock Off removes the item even if the user faints to Rocky Helmet")
{
    GIVEN {
        ASSUME(GetConfig(B_FAINT_MOVE_EFFECT_TIMING) >= GEN_CHAMPIONS);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ROCKY_HELMET); }
    } WHEN {
        TURN { MOVE(player, MOVE_KNOCK_OFF); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_KNOCK_OFF, player);
        MESSAGE("마자용은 쓰러졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ITEM_KNOCKOFF);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}
