#include "global.h"
#include "test/battle.h"

// HnS: switches made during a dance move resolve before Dancer, and Dancer follows the battlers on the field after them
// (friend decisions 2026-10-06 and 2026-10-07). The Eject Button/Pack tests are in test/battle/ability/dancer.c.
// These are the same rule for Emergency Exit/Wimp Out: a Dancer sent in for its user dances in that turn, but does not
// take the action chosen for the slot. The user that leaves has Emergency Exit/Wimp Out, so it is never a Dancer itself.
// No English MESSAGE() (HnS prints Korean text): checks use pop-ups, animations, HP and stat stages.

DOUBLE_BATTLE_TEST("Dancer sent in by Emergency Exit or Wimp Out copies a later dance move of the turn but skips its own action")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_DRAGON_DANCE));
        ASSUME(!IsDanceMove(MOVE_SCRATCH));
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Speed(5); Ability(ABILITY_DANCER); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentLeft, MOVE_SCRATCH, target: playerLeft);
            SEND_OUT(playerLeft, 2);
            MOVE(opponentRight, MOVE_DRAGON_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponentLeft);
        ABILITY_POPUP(playerLeft, ability);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, opponentRight);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1); // +3 if it had used Swords Dance
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(playerRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(playerRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("Dancer sent in by Emergency Exit or Wimp Out copies the dance move that made the user switch out")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetMoveCategory(MOVE_AQUA_STEP) != DAMAGE_CATEGORY_STATUS);
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); }
        PLAYER(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(opponentLeft, MOVE_AQUA_STEP, target: playerLeft);
            SEND_OUT(playerLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentLeft);
        HP_BAR(playerLeft);
        ABILITY_POPUP(playerLeft, ability);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        HP_BAR(opponentLeft);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("Dancer sent in by a foe's Emergency Exit copies the dance move that made the user switch out")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_FIERY_DANCE));
        ASSUME(GetMoveCategory(MOVE_FIERY_DANCE) != DAMAGE_CATEGORY_STATUS);
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(40); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); }
        OPPONENT(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_FIERY_DANCE, target: opponentLeft);
            MOVE(opponentLeft, MOVE_SWORDS_DANCE);
            SEND_OUT(opponentLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIERY_DANCE, playerLeft);
        HP_BAR(opponentLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(opponentLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIERY_DANCE, opponentLeft);
        HP_BAR(playerLeft);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponentLeft);
    } THEN {
        EXPECT_EQ(opponentLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Dancer sent in by Emergency Exit or Wimp Out copies the dance move that made the user switch out (single battle)")
{
    u32 species;
    enum Ability ability;
    enum Move move;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; move = MOVE_AQUA_STEP; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT;       move = MOVE_AQUA_STEP; }
    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; move = MOVE_REVELATION_DANCE; } // friend's mGBA case
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(IsDanceMove(MOVE_REVELATION_DANCE));
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); SpDefense(400); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Speed(50); Ability(ABILITY_DANCER); Moves(MOVE_AQUA_STEP, MOVE_REVELATION_DANCE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); MOVE(opponent, move); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        HP_BAR(player);
        ABILITY_POPUP(player, ability);
        ABILITY_POPUP(player, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        HP_BAR(opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_ORICORIO);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + (move == MOVE_AQUA_STEP));
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

// HnS: the friend's mGBA case (2026-10-09) with a trainer AI choosing the foe's moves, singles and doubles.
AI_SINGLE_BATTLE_TEST("Dancer sent in by Emergency Exit copies the trainer AI's Revelation Dance that made the user switch out")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_REVELATION_DANCE));
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); SpDefense(400); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(50); Moves(MOVE_REVELATION_DANCE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_REVELATION_DANCE); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REVELATION_DANCE, opponent);
        ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(player, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REVELATION_DANCE, player);
        HP_BAR(opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_ORICORIO);
    }
}

AI_DOUBLE_BATTLE_TEST("Dancer sent in by Emergency Exit copies the trainer AI's Revelation Dance that made the user switch out (doubles)")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_REVELATION_DANCE));
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); SpDefense(400); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_HEATRAN) { Ability(ABILITY_FLASH_FIRE); Speed(20); Moves(MOVE_CELEBRATE); } // immune: the AI targets Golisopod
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(50); Moves(MOVE_REVELATION_DANCE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            EXPECT_MOVE(opponentLeft, MOVE_REVELATION_DANCE, target: playerLeft);
            EXPECT_MOVE(opponentRight, MOVE_CELEBRATE);
            SEND_OUT(playerLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REVELATION_DANCE, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REVELATION_DANCE, playerLeft);
        HP_BAR(opponentLeft);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
    }
}

DOUBLE_BATTLE_TEST("Emergency Exit or Wimp Out user's replacement dances once only if it is a Dancer")
{
    u32 species, replacement;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; replacement = SPECIES_WYNAUT; }
    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; replacement = SPECIES_ORICORIO; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT;       replacement = SPECIES_WYNAUT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT;       replacement = SPECIES_ORICORIO; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetSpeciesAbility(SPECIES_ORICORIO, 0) == ABILITY_DANCER);
        ASSUME(GetSpeciesAbility(SPECIES_WYNAUT, 0) != ABILITY_DANCER);
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        PLAYER(replacement) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_AQUA_STEP, target: playerLeft); SEND_OUT(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentLeft);
        ABILITY_POPUP(playerLeft, ability);
        if (replacement == SPECIES_ORICORIO) {
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        }
        NONE_OF {
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_POSITION_PLAYER_LEFT], 2);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + (replacement == SPECIES_ORICORIO));
    }
}

// HnS: B_DANCER_ORDER GEN_7 keeps the HnS order from before #9988: slowest current unmodified Speed first, lower battler
// first on ties. Upstream's GEN_7 branch reads gBattlersByRawSpeed, which is only sorted on switch-ins.

DOUBLE_BATTLE_TEST("Dancer (Gen 7, HnS) uses the current unmodified Speed after Speed Swap")
{
    GIVEN {
        WITH_CONFIG(B_DANCER_ORDER, GEN_7);
        ASSUME(GetMoveEffect(MOVE_SPEED_SWAP) == EFFECT_SPEED_SWAP);
        ASSUME(IsDanceMove(MOVE_DRAGON_DANCE));
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(5); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SPEED_SWAP, target: opponentRight); }
        TURN { MOVE(playerRight, MOVE_DRAGON_DANCE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPEED_SWAP, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, playerRight);
        ABILITY_POPUP(opponentLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->speed, 30);
        EXPECT_EQ(opponentRight->speed, 5);
    }
}

DOUBLE_BATTLE_TEST("Dancer (Gen 7, HnS) uses the current unmodified Speed after Ally Switch")
{
    GIVEN {
        WITH_CONFIG(B_DANCER_ORDER, GEN_7);
        ASSUME(GetMoveEffect(MOVE_ALLY_SWITCH) == EFFECT_ALLY_SWITCH);
        ASSUME(IsDanceMove(MOVE_DRAGON_DANCE));
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(5); }
        PLAYER(SPECIES_ORICORIO_POM_POM) { Ability(ABILITY_DANCER); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
    } WHEN {
        TURN { MOVE(playerRight, MOVE_ALLY_SWITCH); }
        TURN { MOVE(opponentLeft, MOVE_DRAGON_DANCE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ALLY_SWITCH, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, opponentLeft);
        ABILITY_POPUP(playerRight, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, playerRight);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->speed, 30);
        EXPECT_EQ(playerRight->speed, 5);
    }
}

DOUBLE_BATTLE_TEST("Dancer (Gen 7, HnS) on a Speed tie: the lower battler dances first, also when sent in by Emergency Exit")
{
    GIVEN {
        WITH_CONFIG(B_DANCER_ORDER, GEN_7);
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Speed(10); Ability(ABILITY_DANCER); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_ORICORIO) { Speed(10); Ability(ABILITY_DANCER); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_AQUA_STEP, target: playerLeft); SEND_OUT(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentLeft);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        ABILITY_POPUP(opponentRight, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentRight);
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_POSITION_PLAYER_LEFT], 2);
    }
}

// HnS: chains and edge cases for the rule above (friend request 2026-10-09). Emergency Exit/Wimp Out mark the slot like
// Eject Button/Pack (dancerAfterEjectItem, read by MoveEndDancer). A slot dances at most once per action (TryDancer checks
// dancerUsedMove, which stays on the slot until the action ends) and the dance move user's slot never dances, so a chain
// of switches always ends.

DOUBLE_BATTLE_TEST("Dancer sent in by Eject Button, Emergency Exit or Wimp Out dances the same way, slowest first with the Dancers already there (Gen 7, HnS)")
{
    u32 species, speed;
    enum Ability ability;
    enum Item item;

    PARAMETRIZE { species = SPECIES_WOBBUFFET; ability = ABILITY_TELEPATHY;      item = ITEM_EJECT_BUTTON; speed = 1; }
    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; item = ITEM_NONE;         speed = 1; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT;       item = ITEM_NONE;         speed = 1; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; ability = ABILITY_TELEPATHY;      item = ITEM_EJECT_BUTTON; speed = 30; }
    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; item = ITEM_NONE;         speed = 30; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT;       item = ITEM_NONE;         speed = 30; }
    GIVEN {
        WITH_CONFIG(B_DANCER_ORDER, GEN_7);
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetItemHoldEffect(ITEM_EJECT_BUTTON) == HOLD_EFFECT_EJECT_BUTTON);
        PLAYER(species) { Ability(ability); Item(item); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(20); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(speed); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_AQUA_STEP, target: playerLeft);
            MOVE(opponentRight, MOVE_CELEBRATE);
            SEND_OUT(playerLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentLeft);
        HP_BAR(playerLeft);
        if (item == ITEM_EJECT_BUTTON)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        else
            ABILITY_POPUP(playerLeft, ability);
        if (speed == 1) {
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
            HP_BAR(opponentLeft);
        }
        ABILITY_POPUP(opponentRight, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentRight);
        HP_BAR(playerLeft);
        ABILITY_POPUP(playerRight, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerRight);
        HP_BAR(opponentLeft);
        if (speed == 30) {
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
            HP_BAR(opponentLeft);
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        NONE_OF {
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerLeft);
        }
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        // Every battler moved or danced once: one Aqua Step each
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(playerLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(playerRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("Dancer sent in by Emergency Exit or Wimp Out after an ally's dance move copies it on that ally, the original user")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerRight, MOVE_AQUA_STEP, target: playerLeft);
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            SEND_OUT(playerLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerRight);
        HP_BAR(playerLeft);
        ABILITY_POPUP(playerLeft, ability);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        HP_BAR(playerRight);
        NONE_OF {
            HP_BAR(opponentLeft);
            HP_BAR(opponentRight);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        }
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(playerRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit on both sides: the Dancer sent in for the dance move's user doesn't copy the foe Dancer's copy (no loop)")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(133); Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_AQUA_STEP); MOVE(opponent, MOVE_SWORDS_DANCE); SEND_OUT(opponent, 1); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(opponent, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT);
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
            ABILITY_POPUP(opponent, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
        }
    } THEN {
        EXPECT_EQ(player->species, SPECIES_ORICORIO);
        EXPECT_EQ(opponent->species, SPECIES_ORICORIO);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("Dancer sent in by Emergency Exit after a Dancer's copy made the replacement switch out too dances once (chain ends)")
{
    GIVEN {
        WITH_CONFIG(B_DANCER_ORDER, GEN_7);
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SWORDS_DANCE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_AQUA_STEP, target: playerLeft);
            MOVE(opponentRight, MOVE_CELEBRATE);
            SEND_OUT(playerLeft, 2);
            SEND_OUT(playerLeft, 3);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentLeft);
        HP_BAR(playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(opponentRight, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentRight);
        HP_BAR(playerLeft);
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        HP_BAR(opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        NONE_OF {
            ABILITY_POPUP(playerLeft, ABILITY_DANCER);
            ABILITY_POPUP(opponentRight, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
        }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_POSITION_PLAYER_LEFT], 3);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit or Wimp Out user holding an Eject Button: Eject Button goes first and the Dancer sent in dances once")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetItemHoldEffect(ITEM_EJECT_BUTTON) == HOLD_EFFECT_EJECT_BUTTON);
        PLAYER(species) { Ability(ability); Item(ITEM_EJECT_BUTTON); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); MOVE(opponent, MOVE_AQUA_STEP); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ABILITY_POPUP(player, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
        HP_BAR(opponent);
        NONE_OF {
            ABILITY_POPUP(player, ability);
            ABILITY_POPUP(player, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        }
    } THEN {
        EXPECT_EQ(player->species, SPECIES_ORICORIO);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Dancer sent in by Emergency Exit or Wimp Out that faints on entry doesn't dance, nor does the Pokemon sent in for it")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetMoveEffect(MOVE_STEALTH_ROCK) == EFFECT_STEALTH_ROCK);
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); HP(1); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_STEALTH_ROCK, MOVE_AQUA_STEP); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_STEALTH_ROCK); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); MOVE(opponent, MOVE_AQUA_STEP); SEND_OUT(player, 1); SEND_OUT(player, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEALTH_ROCK, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        ABILITY_POPUP(player, ability);
        HP_BAR(player, hp: 0);
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_POSITION_PLAYER_LEFT], 2);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit or Wimp Out with no Pokemon left to switch in: no Dancer, and the user still takes its action")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); HP(0); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_AQUA_STEP); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_AQUA_STEP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        NONE_OF {
            ABILITY_POPUP(player, ability);
            ABILITY_POPUP(player, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    } THEN {
        EXPECT_EQ(player->species, species);
        EXPECT_EQ(gBattlerPartyIndexes[B_POSITION_PLAYER_LEFT], 0);
    }
}

AI_SINGLE_BATTLE_TEST("Dancer sent in by the foe trainer AI's Emergency Exit copies the player's Revelation Dance on the player")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_REVELATION_DANCE));
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(50); Moves(MOVE_REVELATION_DANCE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(132); SpDefense(400); Speed(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_REVELATION_DANCE); EXPECT_MOVE(opponent, MOVE_CELEBRATE); EXPECT_SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REVELATION_DANCE, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(opponent, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REVELATION_DANCE, opponent);
        HP_BAR(player);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_ORICORIO);
    }
}

SINGLE_BATTLE_TEST("Dancer sent in for the dance move's user by its own Emergency Exit (Life Orb) doesn't copy that dance")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetItemHoldEffect(ITEM_LIFE_ORB) == HOLD_EFFECT_LIFE_ORB);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); Item(ITEM_LIFE_ORB); MaxHP(263); HP(140); Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_AQUA_STEP); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        HP_BAR(opponent); // Life Orb
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_ORICORIO);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Dance move triggering the target's Eject Button and its user's Emergency Exit (Life Orb): one switch at a time, the chain ends")
{
    u32 species;

    PARAMETRIZE { species = SPECIES_ORICORIO; }
    PARAMETRIZE { species = SPECIES_WYNAUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetItemHoldEffect(ITEM_EJECT_BUTTON) == HOLD_EFFECT_EJECT_BUTTON);
        ASSUME(GetItemHoldEffect(ITEM_LIFE_ORB) == HOLD_EFFECT_LIFE_ORB);
        ASSUME(GetSpeciesAbility(SPECIES_ORICORIO, 0) == ABILITY_DANCER);
        ASSUME(GetSpeciesAbility(SPECIES_WYNAUT, 0) != ABILITY_DANCER);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_TELEPATHY); Item(ITEM_EJECT_BUTTON); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(species) { Speed(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); Item(ITEM_LIFE_ORB); MaxHP(263); HP(140); Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_CELEBRATE);
            MOVE(opponent, MOVE_AQUA_STEP);
            SEND_OUT(player, 1);
            if (species == SPECIES_ORICORIO)
                SEND_OUT(opponent, 1);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        HP_BAR(opponent); // Life Orb; the queued Eject Button switch holds Emergency Exit back
        if (species == SPECIES_ORICORIO) {
            // The Dancer sent in by Eject Button copies the dance on the user; Emergency Exit then activates
            ABILITY_POPUP(player, ABILITY_DANCER);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
            HP_BAR(opponent);
            ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        }
        NONE_OF { // the Dancer sent in for the dance move's user doesn't dance; nothing activates twice
            ABILITY_POPUP(opponent, ABILITY_DANCER);
            ABILITY_POPUP(player, ABILITY_DANCER);
            ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        }
    } THEN {
        EXPECT_EQ(player->species, species);
        EXPECT_EQ(opponent->species, species == SPECIES_ORICORIO ? SPECIES_ORICORIO : SPECIES_GOLISOPOD);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + (species != SPECIES_ORICORIO));
    }
}

DOUBLE_BATTLE_TEST("Dancer sent in by Emergency Exit copies an attacking dance on the other foe if the dance move's user fainted")
{
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        ASSUME(GetItemHoldEffect(ITEM_ROCKY_HELMET) == HOLD_EFFECT_ROCKY_HELMET);
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); Item(ITEM_ROCKY_HELMET); MaxHP(263); HP(132); Speed(1); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(5); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Ability(ABILITY_DANCER); Speed(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(50); Moves(MOVE_AQUA_STEP, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(40); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(40); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_AQUA_STEP, target: playerLeft); SEND_OUT(playerLeft, 2); SEND_OUT(opponentLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponentLeft);
        HP_BAR(playerLeft);
        HP_BAR(opponentLeft, hp: 0); // Rocky Helmet
        ABILITY_POPUP(playerLeft, ABILITY_EMERGENCY_EXIT);
        ABILITY_POPUP(playerLeft, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, playerLeft);
        HP_BAR(opponentRight);
        NOT HP_BAR(playerRight);
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_ORICORIO);
        EXPECT_EQ(playerLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}
