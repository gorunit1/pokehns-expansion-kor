#include "global.h"
#include "test/battle.h"

// HnS: Stockpile counts the Def/Sp. Def stages it changed in either direction, so Spit Up and Swallow give a Contrary
// user back the stages Stockpile lowered, as in the main series and HnS before upstream #9730 (upstream 1.17.0/master
// only count raises, so the Contrary user keeps the drops). HnS prints Korean text, so there is no MESSAGE().

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_STOCKPILE) == EFFECT_STOCKPILE);
    ASSUME(GetMoveEffect(MOVE_SPIT_UP) == EFFECT_SPIT_UP);
    ASSUME(GetMoveEffect(MOVE_SWALLOW) == EFFECT_SWALLOW);
}

SINGLE_BATTLE_TEST("Stockpile (HnS): Spit Up and Swallow give back the Def and Sp. Def a Contrary user lost")
{
    u32 move, count;
    PARAMETRIZE { move = MOVE_SPIT_UP; count = 1; }
    PARAMETRIZE { move = MOVE_SPIT_UP; count = 3; }
    PARAMETRIZE { move = MOVE_SWALLOW; count = 2; }
    GIVEN {
        PLAYER(SPECIES_SHUCKLE) { Ability(ABILITY_CONTRARY); HP(100); MaxHP(400); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(900); MaxHP(900); }
    } WHEN {
        for (u32 j = 0; j < count; j++)
            TURN { MOVE(player, MOVE_STOCKPILE); }
        TURN { MOVE(player, move); }
    } SCENE {
        for (u32 j = 0; j < count; j++)
        {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_STOCKPILE, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        }
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Stockpile (HnS): Spit Up takes back the doubled Def and Sp. Def of a Simple user")
{
    GIVEN {
        PLAYER(SPECIES_BIBAREL) { Ability(ABILITY_SIMPLE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(900); MaxHP(900); }
    } WHEN {
        TURN { MOVE(player, MOVE_STOCKPILE); }
        TURN { MOVE(player, MOVE_SPIT_UP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOCKPILE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPIT_UP, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE);
    }
}
