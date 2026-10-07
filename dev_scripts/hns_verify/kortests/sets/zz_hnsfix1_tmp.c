#include "global.h"
#include "test/battle.h"

// HnS fix 1 (2026-10-04) scratch tests: Eject item replacement + Magic Bounce / Magic Coat / Snatch (never committed).
// Copy into test/battle/ of a scratch copy and run: make check BUILD=hns TESTS="HNSFIX1 "
// No MESSAGE checks (HnS strings are Korean).

// T1 (= review A R3b): a Magic Bounce mon sent in by Eject Button bounces a status move in the same turn.
DOUBLE_BATTLE_TEST("HNSFIX1 T1 Eject Button replacement with Magic Bounce bounces in the same turn")
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
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ESPEON);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
    }
}

// T2: same with Eject Pack (stat drop from a status move).
DOUBLE_BATTLE_TEST("HNSFIX1 T2 Eject Pack replacement with Magic Bounce bounces in the same turn")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_PACK); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_SCARY_FACE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ESPEON);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
    }
}

// T3: the ejected slot had not acted yet. The replacement bounces, but still does not use the ejected mon's move.
DOUBLE_BATTLE_TEST("HNSFIX1 T3 Eject Button replacement bounces and still skips the slot's own action")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_EJECT_BUTTON); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ESPEON) { Speed(1); Ability(ABILITY_MAGIC_BOUNCE); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
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
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
    }
}

// T4 (= review A R3): Magic Coat user leaves by Eject Button; the replacement must not bounce with it.
DOUBLE_BATTLE_TEST("HNSFIX1 T4 Magic Coat is not inherited by an Eject Button replacement")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
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
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, opponentRight);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

// T5 (= review A R4): Snatch user leaves by Eject Button; the replacement must not snatch with it.
DOUBLE_BATTLE_TEST("HNSFIX1 T5 Snatch is not inherited by an Eject Button replacement")
{
    GIVEN {
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SNATCH);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponentRight);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

// T6: Magic Coat / Snatch user leaves by Eject Pack.
DOUBLE_BATTLE_TEST("HNSFIX1 T6a Magic Coat is not inherited by an Eject Pack replacement")
{
    u32 move = MOVE_MAGIC_COAT;
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_PACK); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, move);
            MOVE(opponentLeft, MOVE_LOW_SWEEP, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            if (move == MOVE_MAGIC_COAT)
                MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            else
                MOVE(opponentRight, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOW_SWEEP, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        if (move == MOVE_MAGIC_COAT) {
            EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
            EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        } else {
            EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
            EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        }
    }
}

DOUBLE_BATTLE_TEST("HNSFIX1 T6b Snatch is not inherited by an Eject Pack replacement")
{
    u32 move = MOVE_SNATCH;
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_PACK); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, move);
            MOVE(opponentLeft, MOVE_LOW_SWEEP, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            if (move == MOVE_MAGIC_COAT)
                MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            else
                MOVE(opponentRight, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOW_SWEEP, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        if (move == MOVE_MAGIC_COAT) {
            EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
            EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        } else {
            EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
            EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        }
    }
}

// T7: not an Eject item (no usedEjectItem): Emergency Exit. Before the fix the replacement bounced/snatched without an assert.
DOUBLE_BATTLE_TEST("HNSFIX1 T7a Magic Coat is not inherited by an Emergency Exit replacement")
{
    u32 move = MOVE_MAGIC_COAT;
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_GOLISOPOD) { Speed(100); Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(262); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, move);
            MOVE(opponentLeft, MOVE_SUPER_FANG, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            if (move == MOVE_MAGIC_COAT)
                MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            else
                MOVE(opponentRight, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        if (move == MOVE_MAGIC_COAT) {
            EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
            EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        } else {
            EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
            EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        }
    }
}

DOUBLE_BATTLE_TEST("HNSFIX1 T7b Snatch is not inherited by an Emergency Exit replacement")
{
    u32 move = MOVE_SNATCH;
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_GOLISOPOD) { Speed(100); Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(262); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, move);
            MOVE(opponentLeft, MOVE_SUPER_FANG, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            if (move == MOVE_MAGIC_COAT)
                MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            else
                MOVE(opponentRight, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        if (move == MOVE_MAGIC_COAT) {
            EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
            EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        } else {
            EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
            EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        }
    }
}

// T8: forced switch (Dragon Tail) of the Magic Coat user; the replacement must not bounce the later Roar.
DOUBLE_BATTLE_TEST("HNSFIX1 T8 Magic Coat is not inherited by a Dragon Tail replacement")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_ROAR));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
        OPPONENT(SPECIES_WYNAUT) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_MAGIC_COAT);
            MOVE(opponentLeft, MOVE_DRAGON_TAIL, target: playerLeft);
            MOVE(opponentRight, MOVE_ROAR, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROAR, opponentRight);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_ROAR, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WOBBUFFET);
        EXPECT_EQ(opponentRight->species, SPECIES_WOBBUFFET);
    }
}

// T9: two Magic Bounce mons, one of them sent in by Eject Button, bounce a move hitting both foes.
// Before the fix the first bounce was dropped with the second pending bit left over.
DOUBLE_BATTLE_TEST("HNSFIX1 T9 Leer at two Magic Bounce mons after Eject Button")
{
    bool32 eject;
    PARAMETRIZE { eject = FALSE; }
    PARAMETRIZE { eject = TRUE; }
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_LEER));
        ASSUME(GetMoveTarget(MOVE_LEER) == TARGET_BOTH);
        if (eject)
            PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        else
            PLAYER(SPECIES_ESPEON) { Speed(100); Ability(ABILITY_MAGIC_BOUNCE); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            if (eject)
                SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_LEER);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
        ABILITY_POPUP(playerRight, ABILITY_MAGIC_BOUNCE);
    } THEN {
        EXPECT_EQ(playerLeft->statStages[STAT_DEF], DEFAULT_STAT_STAGE);
        EXPECT_EQ(playerRight->statStages[STAT_DEF], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentLeft->statStages[STAT_DEF], DEFAULT_STAT_STAGE - 2);
        EXPECT_EQ(opponentRight->statStages[STAT_DEF], DEFAULT_STAT_STAGE - 2);
    }
}

// T10: faint path. FaintClearSetData already cleared both flags; the replacement next turn behaves normally.
DOUBLE_BATTLE_TEST("HNSFIX1 T10 Magic Coat user faints, replacement is unaffected")
{
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); HP(1); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_MAGIC_COAT);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            MOVE(opponentRight, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            SEND_OUT(playerLeft, 2);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

// T11: controls. Plain Magic Coat / Snatch / Magic Bounce in a double battle with an Eject Button holder that does not activate.
DOUBLE_BATTLE_TEST("HNSFIX1 T11 controls: Magic Coat, Snatch and Magic Bounce without a switch")
{
    u32 move;
    PARAMETRIZE { move = MOVE_MAGIC_COAT; }
    PARAMETRIZE { move = MOVE_SNATCH; }
    PARAMETRIZE { move = MOVE_CELEBRATE; }
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        if (move == MOVE_CELEBRATE)
            PLAYER(SPECIES_ESPEON) { Speed(100); Ability(ABILITY_MAGIC_BOUNCE); Item(ITEM_EJECT_BUTTON); }
        else
            PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, move);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            if (move == MOVE_SNATCH)
                MOVE(opponentRight, MOVE_SWORDS_DANCE);
            else
                MOVE(opponentRight, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        if (move == MOVE_SNATCH)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
        else
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, playerLeft);
    } THEN {
        if (move == MOVE_SNATCH) {
            EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
            EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        } else {
            EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
            EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
        }
    }
}

// T12: Baton Pass. Reachable only when the user gets Magic Coat / Snatch from Instruct and then uses Baton Pass in the same turn.
// Upstream #10338 (1.17.0) clears both on every switch-in including Baton Pass; the fix does the same.
DOUBLE_BATTLE_TEST("HNSFIX1 T12a Magic Coat from Instruct is not passed by Baton Pass")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_INSTRUCT) == EFFECT_INSTRUCT);
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        ASSUME(MoveCanBeBouncedBack(MOVE_SCARY_FACE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_MAGIC_COAT, MOVE_BATON_PASS); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Moves(MOVE_INSTRUCT, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_MAGIC_COAT);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerRight, MOVE_INSTRUCT, target: playerLeft);
            MOVE(playerLeft, MOVE_BATON_PASS);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentLeft, MOVE_SCARY_FACE, target: playerLeft);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_INSTRUCT, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, opponentLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 2);
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("HNSFIX1 T12b Snatch from Instruct is not passed by Baton Pass")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_INSTRUCT) == EFFECT_INSTRUCT);
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        ASSUME(MoveCanBeSnatched(MOVE_SWORDS_DANCE));
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_SNATCH, MOVE_BATON_PASS); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Moves(MOVE_INSTRUCT, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SNATCH);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerRight, MOVE_INSTRUCT, target: playerLeft);
            MOVE(playerLeft, MOVE_BATON_PASS);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_INSTRUCT, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponentLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_WYNAUT);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

// T13: a bounced move that switches (Parting Shot, still the pre-#9730 intermediate state in HnS) from an Eject Button replacement
// behaves like the same bounce without Eject Button (no assert). Opponents have no bench, so nobody switches.
DOUBLE_BATTLE_TEST("HNSFIX1 T13 Parting Shot bounced by an Eject Button replacement")
{
    bool32 eject;
    PARAMETRIZE { eject = FALSE; }
    PARAMETRIZE { eject = TRUE; }
    GIVEN {
        ASSUME(MoveCanBeBouncedBack(MOVE_PARTING_SHOT));
        if (eject)
            PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_EJECT_BUTTON); }
        else
            PLAYER(SPECIES_ESPEON) { Speed(100); Ability(ABILITY_MAGIC_BOUNCE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(SPECIES_ESPEON) { Speed(5); Ability(ABILITY_MAGIC_BOUNCE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft);
            if (eject)
                SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_PARTING_SHOT, target: playerLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_MAGIC_BOUNCE);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ESPEON);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPATK], DEFAULT_STAT_STAGE - 1);
    }
}
