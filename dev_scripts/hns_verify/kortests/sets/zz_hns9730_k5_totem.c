#include "global.h"
#include "test/battle.h"

#include "script.h"

// seq 181 #9730 area E Korean stat-change output set K5: HnS totem boss boosts (scratch only, never committed).
// HnS map scripts queue them with `settotemboost` = callnative ScriptSetTotemBoost (about 25 boss battles).
// The test calls the same native with a fake script context, so it works with the pre-#9730 queue layout
// (bit stat-1, 0x80 flag) and the post-#9730 one (bit stat). Evasion is never used (post-#9730 index 7 is
// out of statChanges[7], see part-C risk 3).

void ScriptSetTotemBoost(struct ScriptContext *ctx);

static void HnsTotemBoost(u32 battler, u32 atk, u32 def, u32 speed, u32 spAtk, u32 spDef, u32 acc)
{
    u16 args[8] = {battler, atk, def, speed, spAtk, spDef, acc, 0};
    struct ScriptContext ctx = {0};
    ctx.scriptPtr = (const u8 *)args;
    ScriptSetTotemBoost(&ctx);
}

SINGLE_BATTLE_TEST("HNS9730 K5-01 totem boost: foe Attack +1")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        HnsTotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 0, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        MESSAGE("상대 마자는 오라에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K5-02 totem boost: foe Defense +2 and Sp. Def +2")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        HnsTotemBoost(B_POSITION_OPPONENT_LEFT, 0, 2, 0, 0, 2, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        MESSAGE("상대 마자는 오라에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수방어가 크게 올라갔다!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(opponent->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K5-03 totem boost: all five stats +1")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        HnsTotemBoost(B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        MESSAGE("상대 마자는 오라에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수방어가 올라갔다!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K5-04 totem boost in doubles: right foe Speed +1")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        HnsTotemBoost(B_POSITION_OPPONENT_RIGHT, 0, 0, 1, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponentRight);
        MESSAGE("상대 마자용은 오라에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 스피드가 올라갔다!");
    } THEN {
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}
