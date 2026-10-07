#include "global.h"
#include "test/battle.h"
#include "script.h"

// HnS: totem boosts (`settotemboost` in about 25 HnS boss battles) keep a stat animation and message for every boosted
// stat after the aura message, as before upstream #9730 (friend decision 2026-10-07 #5). Mirror Herb and Opportunist
// still don't react to them (as in upstream, which sets the stages silently).
// HnS prints Korean text, so the checks use animations, stat stages and held items instead of English MESSAGE().

void ScriptSetTotemBoost(struct ScriptContext *ctx);

// Same native as the `settotemboost` script macro: battler position, then Atk, Def, Speed, Sp. Atk, Sp. Def, Accuracy, Evasion
static void TotemBoost(u32 position, u32 atk, u32 def, u32 speed, u32 spAtk, u32 spDef, u32 acc)
{
    u16 args[8] = {position, atk, def, speed, spAtk, spDef, acc, 0};
    struct ScriptContext ctx = {0};
    ctx.scriptPtr = (const u8 *)args;
    ScriptSetTotemBoost(&ctx);
}

SINGLE_BATTLE_TEST("Totem boost (HnS): every boosted stat gets its own stat animation after the aura")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1, 1);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_ACC], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_EVASION], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Totem boost (HnS): +2 and +3 boosts are applied one stat at a time")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        TotemBoost(B_POSITION_OPPONENT_LEFT, 0, 3, 1, 0, 3, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 3);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 3);
    }
}

DOUBLE_BATTLE_TEST("Totem boost (HnS): the right foe's boost in a double battle")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        TotemBoost(B_POSITION_OPPONENT_RIGHT, 1, 1, 0, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponentLeft);
    } THEN {
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentRight->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Totem boost (HnS): a foe's Mirror Herb does not copy it")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_MIRROR_HERB) == HOLD_EFFECT_MIRROR_HERB);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); }
        OPPONENT(SPECIES_WYNAUT);
        TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 2, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, player);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(player->item, ITEM_MIRROR_HERB);
    }
}

SINGLE_BATTLE_TEST("Totem boost (HnS): a foe's Opportunist does not copy it")
{
    GIVEN {
        PLAYER(SPECIES_ESPATHRA) { Ability(ABILITY_OPPORTUNIST); }
        OPPONENT(SPECIES_WYNAUT);
        TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 2, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_OPPORTUNIST);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, player);
        }
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}
