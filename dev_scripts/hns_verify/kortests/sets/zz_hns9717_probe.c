#include "global.h"
#include "test/battle.h"

// seq 150 #9717 scratch probe only (never committed): K3-04 TIMEOUT after the port.

SINGLE_BATTLE_TEST("HNS9717P P1 K3-04 with SEND_OUT(player, 1)")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); }
    } WHEN {
        TURN { SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 쓰러졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717P P2 foe Emergency Exit (poison, faster) and the player's last mon faints from poison")
{
    GIVEN {
        PLAYER(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("마자는 쓰러졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717P P3 faster player Emergency Exit (Sandstorm) and the last foe faints from Sandstorm")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Speed(10); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); }
    } SCENE {
        MESSAGE("상대 마자는 쓰러졌다!");
    }
}

WILD_BATTLE_TEST("HNS9717P P4 wild: faster player Emergency Exit (poison) and the wild mon faints from poison")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("야생 마자는 쓰러졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717P P6 move end (#9494, before #9717): Eject Button target leaves, then the attacker faints from Life Orb recoil")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(5); Item(ITEM_LIFE_ORB); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 쓰러졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717P P7 move end (#9494, before #9717): Emergency Exit target leaves, then the attacker faints from Life Orb recoil")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(5); Item(ITEM_LIFE_ORB); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); SEND_OUT(opponent, 1); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 쓰러졌다!");
    }
}
