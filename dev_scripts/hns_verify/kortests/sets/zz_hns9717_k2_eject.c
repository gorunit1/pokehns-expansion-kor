#include "global.h"
#include "test/battle.h"

// seq 150 #9717 scratch only (never committed): Korean output of Eject Button / Eject Pack paths whose scripts are
// renamed or merged by #9717 (BattleScript_EjectItemActivates, BattleScript_EjectPackActivates_SendReplacement).
// Expected output = HnS before #9717 (chunk-150-155/base). The HnS item pop-up (call BattleScript_ItemPopUp_Scripting)
// is not a recorded test event; the trace diff (ITEM lines) checks it. A space in MESSAGE matches one space or
// newline of the real text.

SINGLE_BATTLE_TEST("HNS9717 K2-01 Eject Pack at end of turn after Octolock (foe), replacement right after (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_OCTOLOCK) == EFFECT_OCTOLOCK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_WOBBUFFET);
        EXPECT_EQ(gBattleMons[B_POSITION_OPPONENT_LEFT].item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K2-02 Eject Pack at end of turn: only the fastest of two holders (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_OCTOLOCK) == EFFECT_OCTOLOCK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_GOLISOPOD) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_OCTOLOCK, target: opponentLeft); MOVE(playerRight, MOVE_OCTOLOCK, target: opponentRight); SEND_OUT(opponentRight, 2); }
    } SCENE {
        MESSAGE("상대 마자용의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        MESSAGE("상대 마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갑주무사를 내보냈다!");
        NONE_OF { MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-03 Eject Pack at end of turn: the replacement's Intimidate triggers the player's Eject Pack (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_OCTOLOCK) == EFFECT_OCTOLOCK);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); SEND_OUT(opponent, 1); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 갸라도스를 내보냈다!");
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        MESSAGE("마자용의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("가랏! 마자!");
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K2-04 UPSTREAM ORDER CHANGE EXPECTED: Emergency Exit (burn) and Eject Pack (Octolock) in the same end of turn")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_OCTOLOCK) == EFFECT_OCTOLOCK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WIMPOD) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_OCTOLOCK, target: opponentRight); SEND_OUT(opponentLeft, 2); SEND_OUT(opponentRight, 3); }
    } SCENE {
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(opponentLeft);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        ABILITY_POPUP(opponentLeft, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponentLeft);
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 꼬시레를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-05 Eject Button after a hit (move end), replacement before the next action (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 마자용을 내보냈다!");
        NONE_OF { MESSAGE("상대 마자는 축하를 썼다!"); }
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_WOBBUFFET);
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-06 Eject Pack after a self stat drop (move end) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_OVERHEAT); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 오버히트를 썼다!");
        HP_BAR(opponent);
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("가랏! 마자!");
        MESSAGE("상대 마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-07 Eject Pack while rooted by Ingrain still switches (#9946 case) (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_INGRAIN) == EFFECT_INGRAIN);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_INGRAIN); }
        TURN { MOVE(player, MOVE_OVERHEAT); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 뿌리를 뻗었다!");
        MESSAGE("마자용은 오버히트를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-08 Eject Pack on a foe switch-in Intimidate (switch-in path, EjectPackActivates_SendReplacement) (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); Speed(50); }
    } WHEN {
        TURN { SWITCH(opponent, 1); SEND_OUT(player, 1); }
        TURN {}
    } SCENE {
        MESSAGE("2는 갸라도스를 내보냈다!");
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        MESSAGE("마자용의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("가랏! 마자!");
        MESSAGE("마자는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-09 Eject Pack at end of turn with nothing to switch in does not activate (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_OCTOLOCK) == EFFECT_OCTOLOCK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
            MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        }
        MESSAGE("상대 마자는 축하를 썼다!");
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_EJECT_PACK);
    }
}

DOUBLE_BATTLE_TEST("HNS9717 K2-10 RECHECK 3b related: foe Dancer replacement sent in by Eject Button copies a later dance of the turn (same)")
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
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!");
        MESSAGE("2는 춤추새를 내보냈다!");
        MESSAGE("마자는 칼춤을 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_DANCER);
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-11 Eject Button holder with nothing to switch in does not activate (same)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_BUTTON); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        HP_BAR(opponent);
        NONE_OF { MESSAGE("상대 마자는 탈출버튼 때문에 돌아간다!"); }
        MESSAGE("상대 마자는 축하를 썼다!");
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_EJECT_BUTTON);
    }
}

SINGLE_BATTLE_TEST("HNS9717 K2-12 Eject Pack while trapped by Mean Look still switches (move end) (same)")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_MEAN_LOOK) == EFFECT_MEAN_LOOK);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_MEAN_LOOK); }
        TURN { MOVE(player, MOVE_GROWL); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("상대 마자는 이제 도망칠 수 없다!");
        MESSAGE("마자용은 울음소리를 썼다!");
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 마자용을 내보냈다!");
    }
}
