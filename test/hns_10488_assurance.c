// HnS seq 485 #10488 회귀 테스트 — 승부굳히기 위력 2배 판정. 스스로 깎은 HP(배북·대타출동·아픔나누기·혼란 자해)는
// 2배 조건이 아니고, 아이스페이스가 막아도 2배가 된다. 이름이 모두 "HnS 10488:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10488: Assurance does not double after the target's own Belly Drum")
{
    s16 hits[2];
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ASSURANCE) == EFFECT_ASSURANCE);
        ASSUME(GetMoveEffect(MOVE_BELLY_DRUM) == EFFECT_BELLY_DRUM);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ASSURANCE); }
        TURN { MOVE(player, MOVE_BELLY_DRUM); MOVE(opponent, MOVE_ASSURANCE); }
    } SCENE {
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[0]);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[1]);
    } THEN {
        EXPECT_EQ(hits[0], hits[1]);
    }
}

// The target gives HP away with Pain Split (equal HP would change nothing, as in upstream's test).
SINGLE_BATTLE_TEST("HnS 10488: Assurance does not double after Pain Split")
{
    s16 hits[2];
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PAIN_SPLIT) == EFFECT_PAIN_SPLIT);
        PLAYER(SPECIES_WOBBUFFET) { HP(800); MaxHP(1000); }
        OPPONENT(SPECIES_WYNAUT) { HP(200); MaxHP(1000); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ASSURANCE); }
        TURN { MOVE(player, MOVE_PAIN_SPLIT); MOVE(opponent, MOVE_ASSURANCE); }
    } SCENE {
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[0]);
        MESSAGE("서로의 체력을 나누어 가졌다!");
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[1]);
    } THEN {
        EXPECT_EQ(hits[0], hits[1]);
    }
}

SINGLE_BATTLE_TEST("HnS 10488: Assurance does not double after the target makes a Substitute")
{
    s16 hits[2];
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_HOPPIP) { Ability(ABILITY_INFILTRATOR); Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ASSURANCE); }
        TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, MOVE_ASSURANCE); }
    } SCENE {
        MESSAGE("상대 통통코는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[0]);
        MESSAGE("마자용의 대타가 나타났다");
        MESSAGE("상대 통통코는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[1]);
    } THEN {
        EXPECT_EQ(hits[0], hits[1]);
    }
}

SINGLE_BATTLE_TEST("HnS 10488: Assurance does not double after the target hurts itself in confusion")
{
    s16 hits[2];
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_CONFUSE_RAY) == EFFECT_CONFUSE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ASSURANCE); }
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
        TURN { MOVE(player, MOVE_CELEBRATE, WITH_RNG(RNG_CONFUSION, TRUE)); MOVE(opponent, MOVE_ASSURANCE); }
    } SCENE {
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[0]);
        MESSAGE("마자용은 혼란에 빠졌다!");
        MESSAGE("영문도 모른 채 자신을 공격했다!");
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[1]);
    } THEN {
        EXPECT_EQ(hits[0], hits[1]);
    }
}

// Damage that an opponent causes still doubles it, as before (Life Orb recoil on the target).
SINGLE_BATTLE_TEST("HnS 10488: Assurance doubles after the target loses HP to its Life Orb")
{
    s16 hits[2];
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_LIFE_ORB) == HOLD_EFFECT_LIFE_ORB);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIFE_ORB); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ASSURANCE); }
        TURN { MOVE(player, MOVE_POUND); MOVE(opponent, MOVE_ASSURANCE); }
    } SCENE {
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[0]);
        MESSAGE("마자용은 막치기를 썼다!");
        HP_BAR(opponent);
        HP_BAR(player);
        MESSAGE("상대 마자는 승부굳히기를 썼다!");
        HP_BAR(player, captureDamage: &hits[1]);
    } THEN {
        EXPECT_MUL_EQ(hits[0], Q_4_12(2.0), hits[1]);
    }
}

DOUBLE_BATTLE_TEST("HnS 10488: Assurance doubles after Ice Face blocks a hit")
{
    s16 hits[2];
    GIVEN {
        ASSUME(GetMoveWeatherType(MOVE_HAIL) == BATTLE_WEATHER_HAIL);
        PLAYER(SPECIES_EISCUE) { Speed(1); Ability(ABILITY_ICE_FACE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(9); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_POUND, target: playerLeft); }
        TURN { MOVE(opponentLeft, MOVE_ASSURANCE, target: playerLeft); MOVE(playerLeft, MOVE_HAIL); }
        TURN { MOVE(opponentRight, MOVE_POUND, target: playerLeft); MOVE(opponentLeft, MOVE_ASSURANCE, target: playerLeft); }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_ICE_FACE);
        MESSAGE("상대 마자용은 승부굳히기를 썼다!");
        HP_BAR(playerLeft, captureDamage: &hits[0]);
        MESSAGE("빙큐보는 싸라기눈을 썼다!");
        ABILITY_POPUP(playerLeft, ABILITY_ICE_FACE);
        MESSAGE("상대 마자는 막치기를 썼다!");
        ABILITY_POPUP(playerLeft, ABILITY_ICE_FACE);
        MESSAGE("상대 마자용은 승부굳히기를 썼다!");
        HP_BAR(playerLeft, captureDamage: &hits[1]);
    } THEN {
        EXPECT_MUL_EQ(hits[0], Q_4_12(2.0), hits[1]);
    }
}
