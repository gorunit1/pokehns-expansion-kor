// HnS seq 477 #10311 회귀 테스트 — `end2` 삭제(`end`로 통합)와, 게으름·불복종(낮잠·잠든 채 무시)·대타출동꼬리 스크립트가
// `BattleScript_MoveEnd`(전체 MoveEnd)로 끝나게 된 뒤의 한글 출력과 다음 턴 진행. 이름이 모두 "HnS 10311:"로 시작한다.
#include "global.h"
#include "test/battle.h"

// Truant: the loafing turn shows the popup and the line, then the next turn attacks normally.
SINGLE_BATTLE_TEST("HnS 10311: Truant loafs on the second turn and attacks on the third")
{
    GIVEN {
        PLAYER(SPECIES_SLAKING) { Ability(ABILITY_TRUANT); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent);
        ABILITY_POPUP(player, ABILITY_TRUANT);
        MESSAGE("게을킹은 게으름을 피우고 있다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent);
    }
}

// Stomping Tantrum after a Truant loafing turn: the loafing turn now runs the whole move end (the timer is set there),
// so the next Stomping Tantrum is doubled, as after a full paralysis.
SINGLE_BATTLE_TEST("HnS 10311: Stomping Tantrum is doubled after a Truant loafing turn")
{
    s16 damage[2];
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_STOMPING_TANTRUM) == EFFECT_STOMPING_TANTRUM);
        PLAYER(SPECIES_SLAKING) { Ability(ABILITY_TRUANT); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
    } WHEN {
        TURN { MOVE(player, MOVE_STOMPING_TANTRUM); }
        TURN { MOVE(player, MOVE_STOMPING_TANTRUM); }
        TURN { MOVE(player, MOVE_STOMPING_TANTRUM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOMPING_TANTRUM, player);
        HP_BAR(opponent, captureDamage: &damage[0]);
        MESSAGE("게을킹은 게으름을 피우고 있다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOMPING_TANTRUM, player);
        HP_BAR(opponent, captureDamage: &damage[1]);
    } THEN {
        EXPECT_MUL_EQ(damage[0], Q_4_12(2.0), damage[1]);
    }
}

// Disobedience nap (third roll below D): the line, sleep, no attack; the opponent's move still follows.
SINGLE_BATTLE_TEST("HnS 10311: a disobedient mon naps and the turn goes on")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { OTName("Test"); Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_HNS_OBEDIENCE_ROLL3, 0)); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 낮잠을 자기 시작했다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        HP_BAR(player);
    } THEN {
        EXPECT(player->status1 & STATUS1_SLEEP);
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

// A sleeping disobedient mon ordered to use Snore: "ignored orders while asleep", no Snore; the opponent still moves.
SINGLE_BATTLE_TEST("HnS 10311: a sleeping disobedient mon ignores orders and the turn goes on")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SNORE) == EFFECT_SNORE);
        PLAYER(SPECIES_WOBBUFFET) { OTName("Test"); Status1(STATUS1_SLEEP_TURN(3)); Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SNORE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 잠든 채로 명령을 무시했다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SNORE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        HP_BAR(player);
    } THEN {
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

// Shed Tail: the line, the switch, and the substitute takes the next hit.
SINGLE_BATTLE_TEST("HnS 10311: Shed Tail switches out and the substitute takes the next hit")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SHED_TAIL) == EFFECT_SHED_TAIL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SHED_TAIL); MOVE(opponent, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHED_TAIL, player);
        HP_BAR(player);
        MESSAGE("마자용은 꼬리를 잘라 대타로 삼았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        NOT HP_BAR(player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(player->hp, player->maxHP);
    }
}
