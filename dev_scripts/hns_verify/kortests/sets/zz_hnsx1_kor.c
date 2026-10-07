#include "global.h"
#include "test/battle.h"

#include "script.h"

// hnsfix-1007b X1 scratch Korean output tests (never committed). Prefix "HNSX1".
// Friend decisions 2026-10-07: #3 Adrenaline Orb / Weakness Policy at the limit, #5 totem stat messages,
// #7 Adrenaline Orb item name and Charge message. SCENE = expected output with the X1 patches;
// the traces (tools/trace.patch) are compared with the pre-#9730 base and the post copy.

void ScriptSetTotemBoost(struct ScriptContext *ctx);

static void HnsX1TotemBoost(u32 battler, u32 atk, u32 def, u32 speed, u32 spAtk, u32 spDef, u32 acc)
{
    u16 args[8] = {battler, atk, def, speed, spAtk, spDef, acc, 0};
    struct ScriptContext ctx = {0};
    ctx.scriptPtr = (const u8 *)args;
    ScriptSetTotemBoost(&ctx);
}

SINGLE_BATTLE_TEST("HNSX1 01 Charge at +6 Sp. Def")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_CHARGE); }
    } SCENE {
        MESSAGE("마자용은 충전을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARGE, player);
        MESSAGE("마자용의 특수방어는 더 올라가지 않는다!");
        MESSAGE("마자용은 충전을 시작했다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 02 Charge with Contrary")
{
    GIVEN {
        PLAYER(SPECIES_SPINDA) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARGE); }
    } SCENE {
        MESSAGE("얼루기는 충전을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("얼루기의 특수방어가 떨어졌다!");
        MESSAGE("얼루기는 충전을 시작했다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 03 Charge by the foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CHARGE); }
    } SCENE {
        MESSAGE("상대 마자는 충전을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수방어가 올라갔다!");
        MESSAGE("상대 마자는 충전을 시작했다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 04 Charge twice at +6 Sp. Def")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_CHARGE); }
        TURN { MOVE(player, MOVE_CHARGE); }
    } SCENE {
        MESSAGE("마자용은 충전을 썼다!");
        MESSAGE("마자용은 충전을 시작했다!");
        MESSAGE("마자용은 충전을 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 05 Adrenaline Orb after Defiant")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_MANKEY) { Ability(ABILITY_DEFIANT); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 망키의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_DEFIANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 망키의 공격이 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 망키는 주눅구슬로 스피드가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNSX1 06 Intimidate on two Adrenaline Orb holders, left one at +6 Speed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ADRENALINE_ORB); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_AGILITY); }
        TURN { MOVE(opponentLeft, MOVE_AGILITY); }
        TURN { MOVE(opponentLeft, MOVE_AGILITY); }
        TURN { SWITCH(playerLeft, 2); }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("상대 마자는 주눅구슬로 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 07 Weakness Policy with only Attack at +6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Item(ITEM_WEAKNESS_POLICY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        MESSAGE("마자용은 덩굴채찍을 썼다!");
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 꼬부기의 공격은 더 올라가지 않는다!");
        MESSAGE("상대 꼬부기는 약점보험으로 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 08 totem boost: the six stats of the HnS Alola/Kanto totems")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        HnsX1TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 1, 1, 1, 1, 1);
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
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 명중률이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 09 totem boost: +3 Defense and Sp. Def (Route 50 boss pattern)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        HnsX1TotemBoost(B_POSITION_OPPONENT_LEFT, 0, 3, 1, 0, 3, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        MESSAGE("상대 마자는 오라에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 매우 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수방어가 매우 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNSX1 10 totem boost vs the player's Mirror Herb")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); }
        OPPONENT(SPECIES_WYNAUT);
        HnsX1TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 2, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        MESSAGE("상대 마자는 오라에 둘러싸였다!");
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->item, ITEM_MIRROR_HERB);
    }
}

SINGLE_BATTLE_TEST("HNSX1 11 totem boost on a Simple holder")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SIMPLE); }
        HnsX1TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 0, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
        MESSAGE("상대 마자는 오라에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("HNSX1 12 Adrenaline Orb with Contrary at -6 Speed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_SPINDA) { Ability(ABILITY_CONTRARY); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("상대 얼루기의 공격이 올라갔다!");
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_ADRENALINE_ORB);
    }
}

// Probes (no output assertion): what Mirror Herb / Opportunist do after a totem boost. THEN shows the item and stages.
SINGLE_BATTLE_TEST("HNSX1 13 probe: totem boost then the player's Mirror Herb")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); }
        OPPONENT(SPECIES_WYNAUT);
        HnsX1TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 2, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
    } THEN {
        EXPECT_EQ(player->item * 100 + player->statStages[STAT_ATK] * 10 + player->statStages[STAT_SPEED], ITEM_MIRROR_HERB * 100 + 66);
    }
}

SINGLE_BATTLE_TEST("HNSX1 14 probe: totem boost then the player's Opportunist")
{
    GIVEN {
        PLAYER(SPECIES_ESPATHRA) { Ability(ABILITY_OPPORTUNIST); }
        OPPONENT(SPECIES_WYNAUT);
        HnsX1TotemBoost(B_POSITION_OPPONENT_LEFT, 1, 0, 2, 0, 0, 0);
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TOTEM_FLARE, opponent);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK] * 10 + player->statStages[STAT_SPEED], 66);
    }
}

// Probe (out of scope, report only): Snowball at +6 Attack. Pre-#9730 HnS checked first (ONLY_CHECKING) and kept it.
SINGLE_BATTLE_TEST("HNSX1 15 probe: Snowball at +6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_SNOWBALL); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_ICE_SHARD); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_SHARD, player);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_SNOWBALL);
    }
}

// X6 (friend decision 2026-10-07 evening, main series): Intimidate blocked by Inner Focus (Gen 8+) still uses the
// Adrenaline Orb right after the block message, as before #9730 (this was a report-only probe of the post-#9730 output,
// where the orb stayed unused). Bulbapedia: the orb still activates when an Ability or Mist blocks Intimidate.
SINGLE_BATTLE_TEST("HNSX1 16 Intimidate blocked by Inner Focus vs Adrenaline Orb")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_INNER_FOCUS); Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_INNER_FOCUS);
        MESSAGE("상대 마자의 공격은 떨어지지 않는다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 주눅구슬로 스피드가 올라갔다!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

// X1-3b (added 2026-10-07): the other held stat items at the limit, and the normal case for the output.
SINGLE_BATTLE_TEST("HNSX1 17 Cell Battery at +6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_CELL_BATTERY); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER_SHOCK, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_CELL_BATTERY);
    }
}

SINGLE_BATTLE_TEST("HNSX1 18 Absorb Bulb at +6 Sp. Atk")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ABSORB_BULB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_NASTY_PLOT); }
        TURN { MOVE(opponent, MOVE_NASTY_PLOT); }
        TURN { MOVE(opponent, MOVE_NASTY_PLOT); }
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_ABSORB_BULB);
    }
}

SINGLE_BATTLE_TEST("HNSX1 19 Luminous Moss at +6 Sp. Def")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LUMINOUS_MOSS); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AMNESIA); }
        TURN { MOVE(opponent, MOVE_AMNESIA); }
        TURN { MOVE(opponent, MOVE_AMNESIA); }
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_LUMINOUS_MOSS);
    }
}

SINGLE_BATTLE_TEST("HNSX1 20 Snowball with Contrary at -6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SPINDA) { Ability(ABILITY_CONTRARY); Item(ITEM_SNOWBALL); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_ICE_SHARD); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_SHARD, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_SNOWBALL);
    }
}

SINGLE_BATTLE_TEST("HNSX1 21 Snowball at +4 Attack still activates")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_SNOWBALL); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_ICE_SHARD); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_SHARD, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 눈덩이로 공격이 올라갔다!");
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}
