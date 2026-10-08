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

    PARAMETRIZE { species = SPECIES_GOLISOPOD; ability = ABILITY_EMERGENCY_EXIT; }
    PARAMETRIZE { species = SPECIES_WIMPOD;    ability = ABILITY_WIMP_OUT; }
    GIVEN {
        ASSUME(IsDanceMove(MOVE_AQUA_STEP));
        PLAYER(species) { Ability(ability); MaxHP(263); HP(132); Speed(1); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        PLAYER(SPECIES_ORICORIO) { Speed(1); Ability(ABILITY_DANCER); Moves(MOVE_SWORDS_DANCE, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); MOVE(opponent, MOVE_AQUA_STEP); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, opponent);
        HP_BAR(player);
        ABILITY_POPUP(player, ability);
        ABILITY_POPUP(player, ABILITY_DANCER);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_STEP, player);
        HP_BAR(opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_ORICORIO);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
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
