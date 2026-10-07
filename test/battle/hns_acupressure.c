#include "global.h"
#include "test/battle.h"

// HnS: Acupressure is changed by the target's Contrary (-2) and Simple (+4), as in the main series and HnS before
// upstream #9730 (upstream 1.17.0/master always queue +2).
// The stat is random, so the checks count the changed stat stages. HnS prints Korean text, so there is no MESSAGE().

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_ACUPRESSURE) == EFFECT_ACUPRESSURE);
}

static u32 CountStagesNotAt(struct BattlePokemon *mon, u32 stage)
{
    u32 count = 0;
    for (enum Stat stat = STAT_ATK; stat < NUM_BATTLE_STATS; stat++)
    {
        if (mon->statStages[stat] != stage)
            count++;
    }
    return count;
}

static s32 GetChangedStageDelta(struct BattlePokemon *mon)
{
    for (enum Stat stat = STAT_ATK; stat < NUM_BATTLE_STATS; stat++)
    {
        if (mon->statStages[stat] != DEFAULT_STAT_STAGE)
            return mon->statStages[stat] - DEFAULT_STAT_STAGE;
    }
    return 0;
}

DOUBLE_BATTLE_TEST("Acupressure (HnS): raises a random stat by 2 on the user or an ally without Contrary or Simple")
{
    bool32 onAlly;
    PARAMETRIZE { onAlly = FALSE; }
    PARAMETRIZE { onAlly = TRUE; }
    GIVEN {
        PLAYER(SPECIES_SHUCKLE) { Ability(ABILITY_STURDY); }
        PLAYER(SPECIES_SHUCKLE) { Ability(ABILITY_STURDY); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: onAlly ? playerRight : playerLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, onAlly ? playerRight : playerLeft);
    } THEN {
        struct BattlePokemon *target = onAlly ? playerRight : playerLeft;
        EXPECT_EQ(CountStagesNotAt(target, DEFAULT_STAT_STAGE), 1);
        EXPECT_EQ(GetChangedStageDelta(target), 2);
        EXPECT_EQ(CountStagesNotAt(onAlly ? playerLeft : playerRight, DEFAULT_STAT_STAGE), 0);
    }
}

DOUBLE_BATTLE_TEST("Acupressure (HnS): lowers a random stat by 2 on a Contrary user or ally")
{
    bool32 onAlly;
    PARAMETRIZE { onAlly = FALSE; }
    PARAMETRIZE { onAlly = TRUE; }
    GIVEN {
        PLAYER(SPECIES_SHUCKLE) { Ability(onAlly ? ABILITY_STURDY : ABILITY_CONTRARY); }
        PLAYER(SPECIES_SHUCKLE) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: onAlly ? playerRight : playerLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, onAlly ? playerRight : playerLeft);
    } THEN {
        struct BattlePokemon *target = onAlly ? playerRight : playerLeft;
        EXPECT_EQ(CountStagesNotAt(target, DEFAULT_STAT_STAGE), 1);
        EXPECT_EQ(GetChangedStageDelta(target), -2);
        EXPECT_EQ(CountStagesNotAt(onAlly ? playerLeft : playerRight, DEFAULT_STAT_STAGE), 0);
    }
}

DOUBLE_BATTLE_TEST("Acupressure (HnS): raises a random stat by 4 on a Simple user or ally")
{
    bool32 onAlly;
    PARAMETRIZE { onAlly = FALSE; }
    PARAMETRIZE { onAlly = TRUE; }
    GIVEN {
        PLAYER(SPECIES_BIBAREL) { Ability(onAlly ? ABILITY_UNAWARE : ABILITY_SIMPLE); }
        PLAYER(SPECIES_BIBAREL) { Ability(ABILITY_SIMPLE); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: onAlly ? playerRight : playerLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, onAlly ? playerRight : playerLeft);
    } THEN {
        struct BattlePokemon *target = onAlly ? playerRight : playerLeft;
        EXPECT_EQ(CountStagesNotAt(target, DEFAULT_STAT_STAGE), 1);
        EXPECT_EQ(GetChangedStageDelta(target), 4);
        EXPECT_EQ(CountStagesNotAt(onAlly ? playerLeft : playerRight, DEFAULT_STAT_STAGE), 0);
    }
}

// Every use changes a stat that can still change, so the number of uses to the limit doesn't depend on the RNG:
// 7 stats x 3 uses of 2 stages, or 7 stats x 2 uses of 4 stages (the second one is cut to 2 at +4).
// Both player battlers use it on the left one so that the uses fit in MAX_TURNS.
DOUBLE_BATTLE_TEST("Acupressure (HnS): reaches the limit on every stat in 21 uses (14 with Simple), then fails")
{
    u32 species, ability, uses, limit;
    PARAMETRIZE { species = SPECIES_SHUCKLE; ability = ABILITY_STURDY;   uses = 21; limit = MAX_STAT_STAGE; }
    PARAMETRIZE { species = SPECIES_SHUCKLE; ability = ABILITY_CONTRARY; uses = 21; limit = MIN_STAT_STAGE; }
    PARAMETRIZE { species = SPECIES_BIBAREL; ability = ABILITY_SIMPLE;   uses = 14; limit = MAX_STAT_STAGE; }
    GIVEN {
        PLAYER(species) { Ability(ability); Speed(50); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
    } WHEN {
        for (u32 j = 0; j < (uses + 2) / 2; j++) // uses + 1 Acupressures, the last one has nothing left to change
        {
            if (2 * j + 1 < uses + 1)
                TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerLeft); MOVE(playerRight, MOVE_ACUPRESSURE, target: playerLeft); }
            else
                TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerLeft); }
        }
    } SCENE {
        for (u32 j = 0; j < uses; j++)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerRight);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        }
    } THEN {
        EXPECT_EQ(CountStagesNotAt(playerLeft, limit), 0);
        EXPECT_EQ(CountStagesNotAt(playerRight, DEFAULT_STAT_STAGE), 0);
    }
}

DOUBLE_BATTLE_TEST("Acupressure (HnS): lowers a stat of a Contrary Pokemon that is at +6 everywhere")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SKILL_SWAP) == EFFECT_SKILL_SWAP);
        PLAYER(SPECIES_SHUCKLE) { Ability(ABILITY_STURDY); Speed(50); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(40); }
        OPPONENT(SPECIES_SHUCKLE) { Ability(ABILITY_CONTRARY); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(20); }
    } WHEN {
        for (u32 j = 0; j < 10; j++) // 21 uses without Contrary: +6 everywhere
            TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerLeft); MOVE(playerRight, MOVE_ACUPRESSURE, target: playerLeft); }
        TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerLeft); }
        TURN { MOVE(opponentLeft, MOVE_SKILL_SWAP, target: playerLeft); }
        TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->ability, ABILITY_CONTRARY);
        EXPECT_EQ(CountStagesNotAt(playerLeft, MAX_STAT_STAGE), 1);
        EXPECT_EQ(CountStagesNotAt(playerLeft, MAX_STAT_STAGE - 2), 6); // the other 6 of the 7 stats stay at +6
    }
}
