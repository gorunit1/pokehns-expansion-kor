#include "global.h"
#include "test/battle.h"

// Review of HnS fix commits 2026-10-04 (scratch only, never committed).
// Run: cp into <copy>/test/battle/ && make check BUILD=hns TESTS="HNSREV "

// M1: Magnet Rise ends on the player while the opponent's Disable ends earlier in the same end turn
// (HandleEndTurnDisable leaves gBattleScripting.battler = opponent).
SINGLE_BATTLE_TEST("HNSREV M1 Magnet Rise end names the player after the opponent's Disable ends")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_MAGNET_RISE, MOVE_DISABLE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_CELEBRATE, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_MAGNET_RISE); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_DISABLE); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        MESSAGE("마자용은 전자력으로 떠올랐다!");
        MESSAGE("상대 마자의 사슬묶기가 풀렸다!");
        NONE_OF { MESSAGE("상대 마자는 전자부유의 효과가 풀렸다!"); }
        MESSAGE("마자용은 전자부유의 효과가 풀렸다!");
    }
}

// M2: roles swapped. Opponent's Magnet Rise ends after the player's Disable ends (stale battler = player).
SINGLE_BATTLE_TEST("HNSREV M2 Magnet Rise end names the opponent after the player's Disable ends")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_CELEBRATE, MOVE_SPLASH); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_MAGNET_RISE, MOVE_DISABLE, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_MAGNET_RISE); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_DISABLE); MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        MESSAGE("상대 마자는 전자력으로 떠올랐다!");
        MESSAGE("마자용의 사슬묶기가 풀렸다!");
        NONE_OF { MESSAGE("마자용은 전자부유의 효과가 풀렸다!"); }
        MESSAGE("상대 마자는 전자부유의 효과가 풀렸다!");
    }
}

// X1: Magic Coat user leaves by Eject Button; the replacement has Magic Bounce itself -> it still bounces (ability).
DOUBLE_BATTLE_TEST("HNSREV X1 Magic Coat user ejected, Magic Bounce replacement bounces by its ability")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_MAGIC_COAT);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ESPEON);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
    }
}

// X2: the same slot is ejected twice in one turn, with a bounce in between. The slot's own action is still skipped
// once, and the last replacement acts normally next turn.
DOUBLE_BATTLE_TEST("HNSREV X2 Two Eject Buttons in one slot with a bounce in between")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_EJECT_BUTTON); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(30); }
        PLAYER(SPECIES_ESPEON) { Speed(1); Ability(ABILITY_MAGIC_BOUNCE); Item(ITEM_EJECT_BUTTON); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 3);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, playerRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
    }
}

// X3: the slot already acted before the Eject Button; the replacement bounces (usedEjectItem stays set until turn end)
// and acts normally next turn.
DOUBLE_BATTLE_TEST("HNSREV X3 Replacement bounces after its slot acted and acts normally next turn")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
        }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 4);
    }
}

// X4: the bounced move triggers the original attacker's Eject Pack while the bouncer itself is an Eject Button replacement.
DOUBLE_BATTLE_TEST("HNSREV X4 Bounced Scary Face ejects the attacker by Eject Pack (bouncer came by Eject Button)")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); Item(ITEM_EJECT_PACK); }
        OPPONENT(SPECIES_WYNAUT) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            SEND_OUT(opponentRight, 2);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(opponentRight->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

// X5: a Snatch user that is NOT switched still snatches after its ally's Eject Button switch in the same turn
// (SwitchInClearSetData clears only the switching slot).
DOUBLE_BATTLE_TEST("HNSREV X5 Snatch survives an ally's Eject Button switch in the same turn")
{
    GIVEN {
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(90); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SNATCH);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerRight);
            SEND_OUT(playerRight, 2);
            MOVE(opponentRight, MOVE_SWORDS_DANCE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerRight->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

// X6: Baton Pass still passes stat stages (only Magic Coat / Snatch are dropped).
DOUBLE_BATTLE_TEST("HNSREV X6 Baton Pass still passes stat stages and Magnet Rise")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SWORDS_DANCE); }
        TURN { MOVE(playerLeft, MOVE_MAGNET_RISE); }
        TURN { MOVE(playerLeft, MOVE_BATON_PASS); SEND_OUT(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        EXPECT_NE((u32)gBattleMons[0].volatiles.magnetRiseTimer, 0);
    }
}
