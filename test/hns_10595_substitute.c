// HnS seq 488 #10595 회귀 테스트 — 대타출동 판정과 변화기 명중 판정이 기술 처리 단계로 옮겨진 뒤의 한글 출력과 순서.
// 이름이 모두 "HnS 10595:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10595: Leech Seed fails against a Substitute")
{
    GIVEN {
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); }
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } SCENE {
        MESSAGE("마자는 씨뿌리기를 썼다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_LEECH_SEED, player);
        MESSAGE("그러나 실패하고 말았다!");
    }
}

// Corrosive Gas used to print the "no effect" line here; the Substitute check now ends the move with "But it failed!".
SINGLE_BATTLE_TEST("HnS 10595: Corrosive Gas fails against a Substitute")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_CORROSIVE_GAS) == EFFECT_CORROSIVE_GAS);
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ORAN_BERRY); Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_CORROSIVE_GAS); }
    } SCENE {
        MESSAGE("마자는 부식가스를 썼다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_CORROSIVE_GAS, player);
            MESSAGE("상대 마자용에게는 효과가 없는 것 같다...");
        }
        MESSAGE("그러나 실패하고 말았다!");
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_ORAN_BERRY);
    }
}

// The accuracy check now comes before the "already asleep" check (as in the main series).
SINGLE_BATTLE_TEST("HnS 10595: a missed Hypnosis on a sleeping target prints the miss line")
{
    GIVEN {
        ASSUME(GetMoveAccuracy(MOVE_HYPNOSIS) < 100);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPNOSIS, hit: FALSE); }
    } SCENE {
        MESSAGE("마자용은 최면술을 썼다!");
        NOT MESSAGE("상대 마자는 이미 잠들어 있다");
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10595: a missed Supersonic on a confused target prints the miss line")
{
    GIVEN {
        ASSUME(GetMoveAccuracy(MOVE_SUPERSONIC) < 100);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_CONFUSE_RAY); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_CONFUSION, FALSE)); }
        TURN { MOVE(player, MOVE_SUPERSONIC, hit: FALSE); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_CONFUSION, FALSE)); }
    } SCENE {
        MESSAGE("상대 마자는 혼란에 빠졌다!");
        MESSAGE("마자용은 초음파를 썼다!");
        NOT MESSAGE("상대 마자는 이미 혼란에 빠져 있다");
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

DOUBLE_BATTLE_TEST("HnS 10595: Teeter Dance does not confuse a foe behind a Substitute when the other foe protects")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TEETER_DANCE) == EFFECT_CONFUSE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_OWN_TEMPO); Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(opponentRight, MOVE_SUBSTITUTE); MOVE(playerLeft, MOVE_TEETER_DANCE); }
    } SCENE {
        MESSAGE("마자용은 흔들흔들댄스를 썼다!");
        NOT MESSAGE("상대 마자는 혼란에 빠졌다!");
    } THEN {
        EXPECT(opponentRight->volatiles.confusionTimer == 0);
    }
}

// HnS fix: entry hazards are set on the foes' side even if the foe is behind a Substitute (upstream #10595 made them fail).
SINGLE_BATTLE_TEST("HnS 10595: entry hazards are set against a foe behind a Substitute")
{
    u32 move;
    PARAMETRIZE { move = MOVE_STEALTH_ROCK; }
    PARAMETRIZE { move = MOVE_SPIKES; }
    PARAMETRIZE { move = MOVE_TOXIC_SPIKES; }
    PARAMETRIZE { move = MOVE_STICKY_WEB; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, move); }
    } SCENE {
        MESSAGE("마자용의 대타가 나타났다");
        NOT MESSAGE("그러나 실패하고 말았다!");
        if (move == MOVE_STEALTH_ROCK)
            MESSAGE("우리 편의 주위에 뾰족한 바위가 떠다니기 시작했다!");
        else if (move == MOVE_SPIKES)
            MESSAGE("우리 편의 발밑에 압정이 뿌려졌다!");
        else if (move == MOVE_TOXIC_SPIKES)
            MESSAGE("우리 편의 발밑에 독압정이 뿌려졌다!");
        else
            MESSAGE("우리 편 발밑에 끈적끈적네트가 펼쳐졌다!");
    }
}
