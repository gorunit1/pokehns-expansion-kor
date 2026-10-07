#include "global.h"
#include "test/battle.h"

// seq 150 #9717 scratch only (never committed): end-of-turn Emergency Exit together with fainting / battle end.
// Expected output = HnS before #9717 (chunk-150-155/base). "... CHANGE EXPECTED" = upstream #9717 changes it on
// purpose (see zz_hns9717_k1_ee.c). A space in MESSAGE matches one space or newline of the real text.

DOUBLE_BATTLE_TEST("HNS9717 K3-01 Foe faints from poison, other foe Emergency Exit from burn: EE replacement, then fainted replacement (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(opponentRight, 2); SEND_OUT(opponentLeft, 3); }
    } SCENE {
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponentLeft, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponentRight);
        ABILITY_POPUP(opponentRight, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponentRight);
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("2는 꼬시레를 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K3-02 Player Emergency Exit (burn) while the partner faints from poison (same)")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(40); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WIMPOD) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(playerLeft, 2); SEND_OUT(playerRight, 3); }
    } SCENE {
        MESSAGE("마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(playerRight, hp: 0);
        MESSAGE("마자는 쓰러졌다!");
        MESSAGE("갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, playerLeft);
        MESSAGE("가랏! 마자용!");
        MESSAGE("가랏! 꼬시레!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K3-03 Emergency Exit foe at weather damage: replacement comes in before the player's Leftovers (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LEFTOVERS); HP(100); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        MESSAGE("모래바람이 상대 갑주무사를 덮쳤다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        MESSAGE("2는 마자를 내보냈다!");
        MESSAGE("마자용은 먹다남은음식으로 인해 조금 회복했다.");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K3-04 UPSTREAM CHANGE EXPECTED (softlock without the HnS KO-animation fix): faster player Emergency Exit (poison) and the last foe faints from poison")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("갑주무사는 독에 의한 데미지를 입고 있다!");
        HP_BAR(player);
        NONE_OF { ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT); }
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        NONE_OF { ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT); }
        MESSAGE("2와의 승부에서 이겼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K3-05 slower player Emergency Exit (poison): the last foe faints first, no Emergency Exit (same)")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(50); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        NONE_OF { ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT); }
        MESSAGE("2와의 승부에서 이겼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K3-06 Emergency Exit foe with a fainted-and-replaced player mon in the same end of turn (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Status1(STATUS1_POISON); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(opponent, 1); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
        HP_BAR(player, hp: 0);
        MESSAGE("마자용은 쓰러졌다!");
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K3-07 UPSTREAM CHANGE EXPECTED (softlock without the HnS KO-animation fix): faster foe Emergency Exit (poison) and the player's last mon faints from poison")
{
    GIVEN {
        PLAYER(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("상대 갑주무사는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent);
        NONE_OF { ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT); }
        MESSAGE("마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(player, hp: 0);
        MESSAGE("마자는 쓰러졌다!");
        NONE_OF { ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT); }
        MESSAGE("2와의 승부에서 졌다!");
    }
}
