#include "global.h"
#include "test/battle.h"

// seq 166 #9784 scratch only (never committed): Korean output order of Red Card / Eject Button / Eject Pack /
// White Herb / Mirror Herb / Magic Bounce / move-end item paths that #9784 reorders (raw Speed order, one
// MOVEEND_ITEM_ON_STAT_CHANGE step, MoveEndItemsEffectsAll reachable again).
// Expected output = HnS before #9784 (chunk-166-170/base). Names ending in "(same)" must not change.
// "CHANGE EXPECTED" pins the pre-#9784 order and is expected to FAIL after the port (intended upstream change).
// The HnS item pop-up (call BattleScript_ItemPopUp_*) is not a recorded test event; the trace diff (ITEM lines) checks it.
// A space in MESSAGE matches one space or newline of the real text.

// ---------------------------------------------------------------- K1 Red Card
SINGLE_BATTLE_TEST("HNS9784 K1-01 Red Card drags the attacker's replacement out (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_RED_CARD); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 레드카드를 마자용에게 꺼내 들었다!");
        MESSAGE("마자는 배틀에 끌려 나왔다!");
        MESSAGE("상대 마자는 축하를 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K1-02 Red Card: spread move, only the fastest of two holders (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); Speed(30); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_RED_CARD); Speed(20); }
        PLAYER(SPECIES_GYARADOS) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(40); }
        OPPONENT(SPECIES_WIMPOD) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_ROCK_SLIDE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        MESSAGE("마자용은 레드카드를 상대 마자용에게 꺼내 들었다!");
        MESSAGE("상대 꼬시레는 배틀에 끌려 나왔다!");
        NONE_OF { MESSAGE("마자는 레드카드를 상대 마자용에게 꺼내 들었다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K1-03 CHANGE EXPECTED: Red Card: raw-faster holder under Slow Start should activate (pre: effective-Speed holder)")
{
    GIVEN {
        PLAYER(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); Item(ITEM_RED_CARD); Speed(30); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_RED_CARD); Speed(20); }
        PLAYER(SPECIES_GYARADOS) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(40); }
        OPPONENT(SPECIES_WIMPOD) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_ROCK_SLIDE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerRight);
        MESSAGE("마자는 레드카드를 상대 마자용에게 꺼내 들었다!");
        MESSAGE("상대 꼬시레는 배틀에 끌려 나왔다!");
        NONE_OF { MESSAGE("레지기가스는 레드카드를 상대 마자용에게 꺼내 들었다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K1-04 CHANGE EXPECTED: faster Eject Button, then slower Red Card should also activate (pre: only Eject Button)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_GYARADOS) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); Item(ITEM_EJECT_BUTTON); }
        OPPONENT(SPECIES_WYNAUT) { Speed(25); Item(ITEM_RED_CARD); }
        OPPONENT(SPECIES_WIMPOD) { Speed(5); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("상대 마자용은 탈출버튼 때문에 돌아간다!");
        NONE_OF { MESSAGE("상대 마자는 레드카드를 마자용에게 꺼내 들었다!"); }
        MESSAGE("2는 꼬시레를 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K1-05 CHANGE EXPECTED: faster Red Card, then slower Eject Button should also activate (pre: only Red Card)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_GYARADOS) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(25); Item(ITEM_EJECT_BUTTON); }
        OPPONENT(SPECIES_WYNAUT) { Speed(30); Item(ITEM_RED_CARD); }
        OPPONENT(SPECIES_WIMPOD) { Speed(5); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("상대 마자는 레드카드를 마자용에게 꺼내 들었다!");
        MESSAGE("갸라도스는 배틀에 끌려 나왔다!");
        NONE_OF { MESSAGE("상대 마자용은 탈출버튼 때문에 돌아간다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9784 K1-06 Red Card before the attacker's Eject Pack (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_OVERHEAT); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_OVERHEAT, player);
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 레드카드를 마자용에게 꺼내 들었다!");
        MESSAGE("마자는 배틀에 끌려 나왔다!");
        NONE_OF { MESSAGE("마자용은 탈출팩 때문에 돌아간다!"); }
        MESSAGE("상대 마자용은 할퀴기를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K1-07 Red Card does not activate if the attacker faints from recoil (#9976 line kept) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_EDGE); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_EDGE, player);
        MESSAGE("마자용은 반동으로 데미지를 입었다!");
        MESSAGE("마자용은 쓰러졌다!");
        NONE_OF { MESSAGE("상대 마자용은 레드카드를 마자용에게 꺼내 들었다!"); }
        MESSAGE("상대 마자용은 축하를 썼다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K1-08 Red Card does not activate behind a Substitute (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 마자용을 대신하여 대타가 공격을 받았다!");
        NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent); }
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_RED_CARD);
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K1-09 Red Card: two attackers in one turn, each target's card activates (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(3); Item(ITEM_RED_CARD); }
        PLAYER(SPECIES_WYNAUT) { Speed(2); Item(ITEM_RED_CARD); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Speed(4); }
        OPPONENT(SPECIES_WIMPOD) { Speed(1); }
    } WHEN {
        TURN {
            MOVE(opponentLeft, MOVE_ROCK_SLIDE);
            MOVE(opponentRight, MOVE_SCRATCH, target: playerRight);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, opponentLeft);
        MESSAGE("마자용은 레드카드를 상대 마자용에게 꺼내 들었다!");
        MESSAGE("상대 꼬시레는 배틀에 끌려 나왔다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("마자는 레드카드를 상대 마자에게 꺼내 들었다!");
        MESSAGE("상대 마자용은 배틀에 끌려 나왔다!");
    }
}

// ---------------------------------------------------------------- K2 Eject Button
DOUBLE_BATTLE_TEST("HNS9784 K2-01 Eject Button: spread move, only the fastest of two holders (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); Speed(30); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 마자용은 탈출버튼 때문에 돌아간다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K2-02 CHANGE EXPECTED: Eject Button: raw-faster holder under Slow Start should activate (pre: effective-Speed holder)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); Item(ITEM_EJECT_BUTTON); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); Speed(30); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); SEND_OUT(opponentRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("상대 마자용은 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 레지기가스는 탈출버튼 때문에 돌아간다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K2-03 CHANGE EXPECTED: Eject Button: all four Speed tied, raw sort should pick the right foe (pre: left foe)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 마자용은 탈출버튼 때문에 돌아간다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9784 K2-04 Eject Button, then the attacker's Life Orb recoil, then the replacement (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIFE_ORB); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        OPPONENT(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 탈출버튼 때문에 돌아간다!");
        HP_BAR(player);
        MESSAGE("마자용의 생명이 조금 깎였다!");
        MESSAGE("2는 갸라도스를 내보냈다!");
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K2-05 Eject Button and Eject Pack hit by the same move: only Eject Button (same)")
{
    GIVEN {
        ASSUME(IsSoundMove(MOVE_SNARL));
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(40); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자의 특수공격이 떨어졌다!");
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!"); }
    }
}

// ---------------------------------------------------------------- K3 Eject Pack
DOUBLE_BATTLE_TEST("HNS9784 K3-01 Eject Pack: spread stat drop, only the fastest of two holders (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K3-02 CHANGE EXPECTED: Eject Pack: raw-faster holder under Slow Start should activate (pre: effective-Speed holder)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(40); }
        OPPONENT(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 레지기가스는 탈출팩 때문에 돌아간다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K3-03 CHANGE EXPECTED: faster Eject Pack should come before slower White Herb (pre: White Herb first)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K3-04 faster White Herb before slower Eject Pack (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(30); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentLeft);
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K3-05 CHANGE EXPECTED: user Throat Spray, then foes Eject Pack and White Herb by raw Speed (pre: White Herb, Throat Spray, Eject Pack)")
{
    GIVEN {
        ASSUME(IsSoundMove(MOVE_SNARL));
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_THROAT_SPRAY); Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("마자용은 목스프레이로 특수공격이 올라갔다!");
        MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K3-06 CHANGE EXPECTED: Weak Armor on U-turn: Eject Pack should not activate (pre: it fires at the U-turn replacement switch-in)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SKARMORY) { Ability(ABILITY_WEAK_ARMOR); Item(ITEM_EJECT_PACK); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_U_TURN); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, player);
        ABILITY_POPUP(opponent, ABILITY_WEAK_ARMOR);
        MESSAGE("상대 무장조의 방어가 떨어졌다!");
        MESSAGE("마자용은 1의 곁으로 돌아간다!");
        MESSAGE("가랏! 마자!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 무장조는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 마자용을 내보냈다!");
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_WOBBUFFET);
    }
}

SINGLE_BATTLE_TEST("HNS9784 K3-07 Weak Armor lowers Defense on a plain hit: Eject Pack activates (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SKARMORY) { Ability(ABILITY_WEAK_ARMOR); Item(ITEM_EJECT_PACK); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_WEAK_ARMOR);
        MESSAGE("상대 무장조의 스피드가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 무장조는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 마자용을 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K3-08 Parting Shot on an Eject Pack holder while the user can switch: no Eject Pack, also not at the replacement switch-in (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_PARTING_SHOT); SEND_OUT(player, 1); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, player);
    } THEN {
        EXPECT_EQ(opponent->item * 1000 + opponent->species, ITEM_EJECT_PACK * 1000 + SPECIES_WOBBUFFET); // pre: 509202
    }
}

// ---------------------------------------------------------------- K4 White Herb
SINGLE_BATTLE_TEST("HNS9784 K4-01 White Herb after the user's Close Combat (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_CLOSE_COMBAT); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLOSE_COMBAT, player);
        MESSAGE("마자용의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K4-02 CHANGE EXPECTED: Shell Smash: foe Opportunist should come before the user White Herb (pre: White Herb first)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(50); }
        OPPONENT(SPECIES_ESPATHRA) { Ability(ABILITY_OPPORTUNIST); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        ABILITY_POPUP(opponent, ABILITY_OPPORTUNIST);
        MESSAGE("상대 클레스퍼트라는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K4-03 CHANGE EXPECTED: Snarl: user Throat Spray should come before the foe White Herb (pre: White Herb first)")
{
    GIVEN {
        ASSUME(IsSoundMove(MOVE_SNARL));
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_THROAT_SPRAY); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SNARL); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, player);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 목스프레이로 특수공격이 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K4-04 CHANGE EXPECTED: two foes White Herb by raw Speed, right foe faster should go first (pre: battler order, left first)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(50); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("상대 마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K4-05 two foes' White Herb, left foe faster (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(30); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자용의 특수공격이 떨어졌다!");
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("상대 마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K4-06 White Herb on switch-in Intimidate (not move end) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); Speed(50); }
    } WHEN {
        TURN { SWITCH(opponent, 1); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        MESSAGE("마자용의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K4-07 Mega Evolution Intimidate on turn 1: both foes' White Herb in battler order (BS_EffectsAfterFormChange kept) (same)")
{
    GIVEN {
        PLAYER(SPECIES_MANECTRIC) { Item(ITEM_MANECTITE); Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(50); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("상대 마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

// ---------------------------------------------------------------- K5 Mirror Herb
SINGLE_BATTLE_TEST("HNS9784 K5-01 Mirror Herb copies the foe's Swords Dance (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("HNS9784 K5-02 CHANGE EXPECTED: Clangorous Soul: user Throat Spray should come before the foe Mirror Herb (pre: Mirror Herb first, reveal 708)")
{
    GIVEN {
        ASSUME(IsSoundMove(MOVE_CLANGOROUS_SOUL));
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_THROAT_SPRAY); Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CLANGOROUS_SOUL); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLANGOROUS_SOUL, opponent);
        MESSAGE("상대 마자용의 특수방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 목스프레이로 특수공격이 올라갔다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPATK] * 100 + opponent->statStages[STAT_SPATK], 708); // pre: Mirror Herb copies +1, then Throat Spray
    }
}

SINGLE_BATTLE_TEST("HNS9784 K5-07 CHANGE EXPECTED: Clangorous Soul with Throat Spray vs Mirror Herb, reveal only (pre 708 = Mirror Herb copies before Throat Spray; 1.17.0 808)")
{
    GIVEN {
        ASSUME(IsSoundMove(MOVE_CLANGOROUS_SOUL));
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_THROAT_SPRAY); Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CLANGOROUS_SOUL); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLANGOROUS_SOUL, opponent);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPATK] * 100 + opponent->statStages[STAT_SPATK], 708);
    }
}

SINGLE_BATTLE_TEST("HNS9784 K5-03 CHANGE EXPECTED: Shell Smash: faster foe Mirror Herb should come before the user White Herb (pre: White Herb first)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K5-04 Shell Smash: faster user's White Herb before the foe's Mirror Herb (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K5-05 CHANGE EXPECTED: two Mirror Herb holders by raw Speed, right one faster should go first (pre: battler order)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_MIRROR_HERB); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SWORDS_DANCE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        MESSAGE("마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerRight);
        MESSAGE("마자는 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K5-06 Opportunist before Mirror Herb on the same Swords Dance (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); Speed(30); }
        PLAYER(SPECIES_ESPATHRA) { Ability(ABILITY_OPPORTUNIST); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SWORDS_DANCE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponentLeft);
        ABILITY_POPUP(playerRight, ABILITY_OPPORTUNIST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        MESSAGE("마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
    }
}

// ---------------------------------------------------------------- K6 Magic Bounce order (MoveEndBouncedMove)
DOUBLE_BATTLE_TEST("HNS9784 K6-01 CHANGE EXPECTED: both foes bounce Growl, right foe faster should bounce first (pre: battler order)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_GROWL) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_ESPEON) { Ability(ABILITY_MAGIC_BOUNCE); Speed(30); }
        OPPONENT(SPECIES_HATTERENE) { Ability(ABILITY_MAGIC_BOUNCE); Speed(50); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GROWL); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponentLeft);
        ABILITY_POPUP(opponentRight, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponentRight);
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K6-02 both foes bounce Growl, left foe faster (same)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_GROWL) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_ESPEON) { Ability(ABILITY_MAGIC_BOUNCE); Speed(50); }
        OPPONENT(SPECIES_HATTERENE) { Ability(ABILITY_MAGIC_BOUNCE); Speed(30); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GROWL); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponentLeft);
        ABILITY_POPUP(opponentRight, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponentRight);
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K6-03 Stealth Rock on two Magic Bounce foes: fastest bounces (already raw Speed) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_ESPEON) { Ability(ABILITY_MAGIC_BOUNCE); Speed(30); }
        OPPONENT(SPECIES_HATTERENE) { Ability(ABILITY_MAGIC_BOUNCE); Speed(50); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_STEALTH_ROCK); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 스텔스록을 썼다!");
        ABILITY_POPUP(opponentRight, ABILITY_MAGIC_BOUNCE);
        MESSAGE("마자용의 스텔스록을 되받아쳤다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEALTH_ROCK, opponentRight);
        MESSAGE("우리 편의 주위에 뾰족한 바위가 떠다니기 시작했다!");
        NONE_OF { ABILITY_POPUP(opponentLeft, ABILITY_MAGIC_BOUNCE); }
    }
}

// ---------------------------------------------------------------- K7 MOVEEND_ITEMS_EFFECTS_ALL reachable again
SINGLE_BATTLE_TEST("HNS9784 K7-01 CHANGE EXPECTED: Magician steals a Sitrus Berry below half HP: should eat it in the same move end (pre: after the foe acts)")
{
    GIVEN {
        PLAYER(SPECIES_KLEFKI) { Ability(ABILITY_MAGICIAN); MaxHP(200); HP(60); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_SITRUS_BERRY); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(player, ABILITY_MAGICIAN);
        MESSAGE("클레피는 상대 마자용으로부터 자뭉열매를 빼앗았다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
        MESSAGE("클레피는 자뭉열매로 체력을 회복했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K7-02 burned Pickpocket steals a Lum Berry and eats it at once (BattleScript_Pickpocket activateitemeffects) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); Speed(50); }
        OPPONENT(SPECIES_SNEASEL) { Ability(ABILITY_PICKPOCKET); Status1(STATUS1_BURN); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 포푸니는 마자용으로부터 리샘열매를 빼앗았다!");
        MESSAGE("상대 포푸니는 리샘열매로 화상이 나았다!");
        MESSAGE("상대 포푸니는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K7-03 Sitrus Berry of the target dropping below half (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_SITRUS_BERRY); MaxHP(200); HP(110); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 마자용은 자뭉열매로 체력을 회복했다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9784 K7-04 Red Card drags in a poisoned Lum Berry holder (Lum eaten at the switch-in, before the foe acts) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_LUM_BERRY); Status1(STATUS1_POISON); Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 마자용은 레드카드를 마자용에게 꺼내 들었다!");
        MESSAGE("마자는 배틀에 끌려 나왔다!");
        MESSAGE("마자는 리샘열매로 독이 해독됐다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
    }
}

// ---------------------------------------------------------------- K8 misc
SINGLE_BATTLE_TEST("HNS9784 K8-01 Pain Split with Life Orb: no recoil (EFFECT_PAIN_SPLIT exception removed) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIFE_ORB); MaxHP(200); HP(50); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_PAIN_SPLIT); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PAIN_SPLIT, player);
        MESSAGE("서로의 체력을 나누어 가졌다!");
        NONE_OF { MESSAGE("마자용의 생명이 조금 깎였다!"); }
    } THEN {
        EXPECT_EQ(player->hp, 125);
    }
}

SINGLE_BATTLE_TEST("HNS9784 K8-02 Pain Split with Shell Bell: no heal (EFFECT_PAIN_SPLIT exception removed) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_SHELL_BELL); MaxHP(200); HP(50); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_PAIN_SPLIT); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PAIN_SPLIT, player);
        MESSAGE("서로의 체력을 나누어 가졌다!");
        NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player); }
    } THEN {
        EXPECT_EQ(player->hp, 125);
    }
}

SINGLE_BATTLE_TEST("HNS9784 K8-03 Throat Spray of a mon dragged in by Red Card does not activate (upstream new test) (same)")
{
    GIVEN {
        ASSUME(IsSoundMove(MOVE_HYPER_VOICE) == TRUE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_THROAT_SPRAY); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_VOICE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자용은 레드카드를 마자용에게 꺼내 들었다!");
        MESSAGE("마자는 배틀에 끌려 나왔다!");
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K8-04 RECHECK 3b FIXED (ee2da89a61): player Dancer sent in by Eject Button dances")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_DRAGON_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_EJECT_BUTTON); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_DRAGON_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        MESSAGE("마자용은 탈출버튼 때문에 돌아간다!");
        MESSAGE("가랏! 춤추새!");
        MESSAGE("상대 마자용은 용의춤을 썼다!");
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        MESSAGE("춤추새는 용의춤을 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK] * 100 + playerLeft->statStages[STAT_SPEED], 707); // before ee2da89a61: 606 (popup only, no dance)
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K8-05 RECHECK 3b FIXED (ee2da89a61): foe Dancer sent in by Eject Button dances")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(20); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); SEND_OUT(opponentLeft, 2);
               MOVE(playerRight, MOVE_SWORDS_DANCE); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 춤추새를 내보냈다!");
        MESSAGE("마자는 칼춤을 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_DANCER);
        MESSAGE("상대 춤추새는 칼춤을 썼다!");
    } THEN {
        EXPECT_EQ(opponentLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], 8); // before ee2da89a61: 6 (popup only, no dance)
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K8-06 RECHECK 3b control: Dancer already on the field dances (same)")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
    } WHEN {
        TURN { MOVE(playerRight, MOVE_SWORDS_DANCE); }
        TURN {}
    } SCENE {
        MESSAGE("마자는 칼춤을 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_DANCER);
        MESSAGE("상대 춤추새는 칼춤을 썼다!");
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], 8);
    }
}

// ---------------------------------------------------------------- K9 queued switch (#9494/#9717) + Eject Button
// HnS BattleScript_FutureAttackEnd runs only SET_VALUES..COLOR_CHANGE (see #10344 port note), so a Future Sight hit
// never reaches MOVEEND_CARD_BUTTON; the new HasAnyBattlerQueuedSwitch() check in TryEjectButton cannot show here.
SINGLE_BATTLE_TEST("HNS9784 K9-01 second Future Sight of the end turn hits the Eject Button holder while the foe Emergency Exit is queued: no Eject Button (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { SEND_OUT(opponent, 1); }
        TURN { }
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        NONE_OF { MESSAGE("마자용은 탈출버튼 때문에 돌아간다!"); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[0], 0);
        EXPECT_EQ(gBattlerPartyIndexes[1], 1);
        EXPECT_EQ(player->item, ITEM_EJECT_BUTTON);
    }
}

SINGLE_BATTLE_TEST("HNS9784 K9-02 Future Sight alone hits the Eject Button holder at end of turn: no Eject Button (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
        TURN { }
    } SCENE {
        MESSAGE("상대 마자용은 미래예지를 썼다!");
        NONE_OF { MESSAGE("마자용은 탈출버튼 때문에 돌아간다!"); }
    } THEN {
        EXPECT_EQ(player->item, ITEM_EJECT_BUTTON);
    }
}

DOUBLE_BATTLE_TEST("HNS9784 K9-03 Eject Button in move end, then Eject Pack of the slower foe waits (queued switch) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(40); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNARL); SEND_OUT(opponentLeft, 2); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, playerLeft);
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        NONE_OF { MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!"); }
        MESSAGE("2는 갑주무사를 내보냈다!");
    }
}
