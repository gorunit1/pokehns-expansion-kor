#include "global.h"
#include "test/battle.h"

// seq 150 #9717 scratch only (never committed): Korean output order of end-of-turn Emergency Exit / Wimp Out.
// Expected output = HnS before #9717 (chunk-150-155/base = code 32b62b550f + chunk 1 patches = HEAD ab2ae3cae1).
// Tests named "... CHANGE EXPECTED" pin the HnS order that upstream #9717 changes on purpose:
//  - Emergency Exit / Wimp Out now activate right after the damage that triggers them (checked before every
//    end-turn handler), not only at the four old ENDTURN_EMERGENCY_EXIT_1~4 steps;
//  - the leaving mon skips later IsBattlerPresent() effects (except Grassy Terrain healing);
//  - its replacement comes in at the next ENDTURN_SEND_OUT_REPLACEMENTS_1~5 step.
// A FAIL there after the port is the recorded upstream change, not a regression. A space in MESSAGE matches one
// space or newline of the real text.

SINGLE_BATTLE_TEST("HNS9717 K1-01 Sandstorm, slower foe Emergency Exit: both hits, exit, replacement (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        MESSAGE("모래바람이 불기 시작했다!");
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        HP_BAR(player);
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        MESSAGE("모래바람이 상대 마자를 덮쳤다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-02 UPSTREAM ORDER CHANGE EXPECTED: Sandstorm, faster foe Emergency Exit waits for the player's hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        HP_BAR(opponent);
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        HP_BAR(player);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-03 UPSTREAM ORDER CHANGE EXPECTED: Sandstorm, faster player Wimp Out waits for the foe's hit")
{
    GIVEN {
        PLAYER(SPECIES_WIMPOD) { Ability(ABILITY_WIMP_OUT); MaxHP(263); HP(134); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 꼬시레를 덮쳤다!");
        HP_BAR(player);
        MESSAGE("모래바람이 상대 마자용을 덮쳤다!");
        HP_BAR(opponent);
        ABILITY_POPUP(player, ABILITY_WIMP_OUT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, player);
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-04 UPSTREAM ORDER CHANGE EXPECTED: both burned, faster foe Emergency Exit waits for the player's burn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        MESSAGE("마자용은 화상 데미지를 입고 있다!");
        HP_BAR(player);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-05 both burned, slower foe Emergency Exit: burns, exit, replacement (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(10); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 화상 데미지를 입고 있다!");
        HP_BAR(player);
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용은 화상 데미지를 입고 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K1-06 UPSTREAM CHANGE EXPECTED: Leech Seed, then poison and Salt Cure still hit before Emergency Exit")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_SALT_CURE, MOVE_EFFECT_SALT_CURE));
        ASSUME(GetMoveEffect(MOVE_LEECH_SEED) == EFFECT_LEECH_SEED);
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(160); Status1(STATUS1_POISON); Speed(40); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(30); }
        PLAYER(SPECIES_WYNAUT) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SALT_CURE, target: playerLeft);
               MOVE(opponentRight, MOVE_LEECH_SEED, target: playerLeft);
               SEND_OUT(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SALT_CURE, opponentLeft);
        HP_BAR(playerLeft);
        MESSAGE("갑주무사는 소금에 절여졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEECH_SEED, opponentRight);
        MESSAGE("갑주무사에게 씨앗을 심었다!");
        HP_BAR(playerLeft);
        HP_BAR(opponentRight);
        MESSAGE("씨뿌리기가 갑주무사의 체력을 빼앗는다!");
        MESSAGE("갑주무사는 독에 의한 데미지를 입고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, playerLeft);
        HP_BAR(playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SALT_CURE_DAMAGE, playerLeft);
        HP_BAR(playerLeft);
        MESSAGE("갑주무사는 소금절이의 데미지를 입고 있다.");
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, playerLeft);
        MESSAGE("가랏! 마자!");
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K1-07 UPSTREAM CHANGE EXPECTED: Sea of Fire with Air Balloon, no Grassy Terrain heal for the Emergency Exit mon")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FIRE_PLEDGE) == EFFECT_PLEDGE);
        ASSUME(GetMoveEffect(MOVE_GRASS_PLEDGE) == EFFECT_PLEDGE);
        ASSUME(GetMoveEffect(MOVE_GRASSY_TERRAIN) == EFFECT_GRASSY_TERRAIN);
        ASSUME(GetItemHoldEffect(ITEM_AIR_BALLOON) == HOLD_EFFECT_AIR_BALLOON);
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); Item(ITEM_AIR_BALLOON); Speed(40); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_FIRE_PLEDGE, target: playerRight);
               MOVE(opponentRight, MOVE_GRASS_PLEDGE, target: playerRight);
               MOVE(playerRight, MOVE_GRASSY_TERRAIN);
               SEND_OUT(playerLeft, 2); }
    } SCENE {
        MESSAGE("발밑에 풀이 무성해졌다!");
        MESSAGE("갑주무사는 불바다의 데미지를 입었다!");
        HP_BAR(playerLeft);
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        HP_BAR(playerRight);
        MESSAGE("마자용의 체력이 회복되었다!");
        HP_BAR(playerRight);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, playerLeft);
        MESSAGE("가랏! 마자!");
        NONE_OF { MESSAGE("갑주무사의 체력이 회복되었다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K1-08 UPSTREAM CHANGE EXPECTED: Sea of Fire, Grassy Terrain and Leftovers bring HP back above half, no Emergency Exit")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FIRE_PLEDGE) == EFFECT_PLEDGE);
        ASSUME(GetMoveEffect(MOVE_GRASS_PLEDGE) == EFFECT_PLEDGE);
        ASSUME(GetMoveEffect(MOVE_GRASSY_TERRAIN) == EFFECT_GRASSY_TERRAIN);
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Item(ITEM_LEFTOVERS); Speed(40); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_FIRE_PLEDGE, target: playerRight);
               MOVE(opponentRight, MOVE_GRASS_PLEDGE, target: playerRight);
               MOVE(playerRight, MOVE_GRASSY_TERRAIN); }
    } SCENE {
        MESSAGE("발밑에 풀이 무성해졌다!");
        MESSAGE("갑주무사는 불바다의 데미지를 입었다!");
        HP_BAR(playerLeft, damage: 32);
        MESSAGE("갑주무사의 체력이 회복되었다!");
        HP_BAR(playerLeft, damage: -16);
        MESSAGE("갑주무사는 먹다남은음식으로 인해 조금 회복했다.");
        HP_BAR(playerLeft, damage: -16);
        NONE_OF { ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT); }
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-09 Sandstorm then Sitrus Berry back above half: no Emergency Exit (same)")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_SITRUS_BERRY) == HOLD_EFFECT_RESTORE_PCT_HP);
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Item(ITEM_SITRUS_BERRY); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); }
        TURN {}
    } SCENE {
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        HP_BAR(opponent);
        MESSAGE("상대 갑주무사는 자뭉열매로 체력을 회복했다!");
        HP_BAR(opponent);
        NONE_OF { ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT); }
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-10 UPSTREAM ORDER CHANGE EXPECTED: Yawn puts the Emergency Exit mon (Leech Seed) to sleep before it leaves")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_YAWN) == EFFECT_YAWN);
        ASSUME(GetMoveEffect(MOVE_LEECH_SEED) == EFFECT_LEECH_SEED);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(200); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
        TURN { MOVE(player, MOVE_YAWN); }
        TURN { MOVE(player, MOVE_CELEBRATE); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 갑주무사에게 씨앗을 심었다!");
        MESSAGE("상대 갑주무사의 졸음을 유도했다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("씨뿌리기가 상대 갑주무사의 체력을 빼앗는다!");
        MESSAGE("상대 갑주무사는 잠들어 버렸다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-11 UPSTREAM CHANGE EXPECTED: Perish Song count of the Emergency Exit mon is shown before it leaves")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PERISH_SONG) == EFFECT_PERISH_SONG);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_PERISH_SONG); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        MESSAGE("상대 갑주무사의 멸망의 카운트가 3이 되었다!");
        MESSAGE("마자용의 멸망의 카운트가 3이 되었다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-12 UPSTREAM ORDER CHANGE EXPECTED: Octolock drops the Emergency Exit mon's stats before it leaves")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_OCTOLOCK) == EFFECT_OCTOLOCK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 갑주무사의 방어가 떨어졌다!");
        MESSAGE("상대 갑주무사의 특수방어가 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-13 UPSTREAM ORDER CHANGE EXPECTED: Taunt end text of the Emergency Exit mon comes before it leaves")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TAUNT) == EFFECT_TAUNT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(182); Status1(STATUS1_BURN); Speed(50); Moves(MOVE_CELEBRATE, MOVE_SCRATCH); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_TAUNT); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 갑주무사는 도발에 넘어가 버렸다!");
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        MESSAGE("상대 갑주무사는 도발의 효과가 풀렸다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K1-14 UPSTREAM ORDER CHANGE EXPECTED: two foe Emergency Exits in one Sandstorm, each leaves and is replaced in turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(40); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SANDSTORM); SEND_OUT(opponentLeft, 2); SEND_OUT(opponentRight, 3); }
    } SCENE {
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        HP_BAR(opponentLeft);
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        HP_BAR(opponentRight);
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        MESSAGE("모래바람이 마자를 덮쳤다!");
        ABILITY_POPUP(opponentLeft, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponentLeft);
        MESSAGE("2는 마자를 내보냈다!");
        ABILITY_POPUP(opponentRight, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponentRight);
        MESSAGE("2는 마자용을 내보냈다!");
    }
}

WILD_BATTLE_TEST("HNS9717 K1-15 Wild Emergency Exit flees at end of turn (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(10); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("야생 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_MON_TELEPORTED);
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-16 Player Emergency Exit at end of turn sends in an Intimidate mon (same)")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(10); }
        PLAYER(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { SEND_OUT(player, 1); }
        TURN {}
    } SCENE {
        MESSAGE("갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(player);
        ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, player);
        MESSAGE("가랏! 갸라도스!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K1-17 Emergency Exit at weather damage: the replacement gets the Wish heal of the same end of turn (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_WISH) == EFFECT_WISH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(100); Speed(10); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_WISH); }
        TURN { MOVE(player, MOVE_SANDSTORM); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 갑주무사는 희망사항을 썼다!");
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
        MESSAGE("상대 갑주무사의 희망사항이 이루어졌다!");
        HP_BAR(opponent);
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K1-18 UPSTREAM ORDER CHANGE EXPECTED: doubles, burned faster foe Emergency Exit, partner burn and player burn before replacement")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Status1(STATUS1_BURN); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(opponentLeft, 2); }
    } SCENE {
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponentLeft);
        MESSAGE("상대 마자용은 화상 데미지를 입고 있다!");
        HP_BAR(opponentRight);
        MESSAGE("마자용은 화상 데미지를 입고 있다!");
        HP_BAR(playerLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponentLeft);
        MESSAGE("2는 마자를 내보냈다!");
    }
}
