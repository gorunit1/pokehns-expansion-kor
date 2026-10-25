// HnS seq 481 #10326 회귀 테스트 — 필드(지형)를 상태 비트(gFieldStatuses)에서 gFieldTimers.terrain 값 하나와
// gBattleTerrainInfo 표(타입·씨앗·시작/끝 문장·배경)로 옮긴 뒤의 한글 출력과 동작. 이름이 모두 "HnS 10326:"으로 시작한다.
#include "global.h"
#include "test/battle.h"

// A terrain move: start line, then the end line after five turns.
SINGLE_BATTLE_TEST("HnS 10326: Electric Terrain starts and ends after five turns")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ELECTRIC_TERRAIN) == EFFECT_TERRAIN);
        ASSUME(GetMoveTerrainType(MOVE_ELECTRIC_TERRAIN) == B_TERRAIN_ELECTRIC);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); }
        TURN { }
        TURN { }
        TURN { }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ELECTRIC_TERRAIN, player);
        MESSAGE("발밑에 전기가 흐르기 시작했다!");
        MESSAGE("발밑의 전기가 사라졌다!");
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_NONE);
    }
}

// Using the terrain that is already up fails.
SINGLE_BATTLE_TEST("HnS 10326: Grassy Terrain fails when Grassy Terrain is already up")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); }
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); }
    } SCENE {
        MESSAGE("발밑에 풀이 무성해졌다!");
        MESSAGE("마자용은 그래스필드를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

// Hadron Engine entering on Electric Terrain set by a faster Electric Surge: the "in terrain" line, the terrain stays.
SINGLE_BATTLE_TEST("HnS 10326: Hadron Engine on existing Electric Terrain uses the terrain line")
{
    GIVEN {
        PLAYER(SPECIES_MIRAIDON) { Ability(ABILITY_HADRON_ENGINE); Speed(10); }
        OPPONENT(SPECIES_TAPU_KOKO) { Ability(ABILITY_ELECTRIC_SURGE); Speed(100); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_ELECTRIC_SURGE);
        MESSAGE("발밑에 전기가 흐르기 시작했다!");
        ABILITY_POPUP(player, ABILITY_HADRON_ENGINE);
        MESSAGE("미라이돈은 일렉트릭필드의 힘으로 미래 기관을 가동했다!!");
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_ELECTRIC);
    }
}

// Teraform Zero with rain but no terrain: the rain line only, no terrain-end line or background restore.
SINGLE_BATTLE_TEST("HnS 10326: Teraform Zero clears rain and does not run the terrain removal without a terrain")
{
    GIVEN {
        PLAYER(SPECIES_TERAPAGOS_TERASTAL);
        OPPONENT(SPECIES_KYOGRE) { Ability(ABILITY_DRIZZLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_TERA); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_TERAFORM_ZERO);
        MESSAGE("비가 그쳤다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, player);
            MESSAGE("발밑의 전기가 사라졌다!");
            MESSAGE("발밑의 안개가 사라졌다!");
            MESSAGE("발밑의 이상한 느낌이 사라졌다!");
            MESSAGE("발밑의 풀이 사라졌다!");
        }
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_NONE);
    }
}

// Teraform Zero with rain and a terrain: both end lines.
SINGLE_BATTLE_TEST("HnS 10326: Teraform Zero clears rain and Electric Terrain")
{
    GIVEN {
        PLAYER(SPECIES_TERAPAGOS_TERASTAL);
        OPPONENT(SPECIES_KYOGRE) { Ability(ABILITY_DRIZZLE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_TERA); }
    } SCENE {
        MESSAGE("발밑에 전기가 흐르기 시작했다!");
        ABILITY_POPUP(player, ABILITY_TERAFORM_ZERO);
        MESSAGE("비가 그쳤다!");
        MESSAGE("발밑의 전기가 사라졌다!");
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_NONE);
    }
}

// Misty Terrain stops a confusing status move (jumpifterrainaffected now takes a terrain byte).
SINGLE_BATTLE_TEST("HnS 10326: Misty Terrain protects a grounded target from Confuse Ray")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_CONFUSE_RAY) == EFFECT_CONFUSE);
        PLAYER(SPECIES_TAPU_FINI) { Ability(ABILITY_MISTY_SURGE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CONFUSE_RAY); }
    } SCENE {
        MESSAGE("발밑이 안개로 자욱해졌다!");
        MESSAGE("상대 마자용을 미스트필드가 지켜 주고 있다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, player);
    } THEN {
        EXPECT(!opponent->volatiles.confusionTimer);
    }
}

// Terrain seeds follow the table: Electric Seed raises Defense, Misty Seed Special Defense.
SINGLE_BATTLE_TEST("HnS 10326: Electric and Misty Seeds raise the stat from the terrain table")
{
    enum Ability ability;
    enum Item item;
    enum Stat stat;
    PARAMETRIZE { ability = ABILITY_ELECTRIC_SURGE; item = ITEM_ELECTRIC_SEED; stat = STAT_DEF; }
    PARAMETRIZE { ability = ABILITY_MISTY_SURGE;    item = ITEM_MISTY_SEED;    stat = STAT_SPDEF; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_TAPU_KOKO) { Ability(ability); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponent, ability);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[stat], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

// Terrain Pulse takes the terrain's type from the table: Electric on Electric Terrain, so a Ground type is immune.
SINGLE_BATTLE_TEST("HnS 10326: Terrain Pulse is Electric on Electric Terrain")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TERRAIN_PULSE) == EFFECT_TERRAIN_PULSE);
        PLAYER(SPECIES_TAPU_KOKO) { Ability(ABILITY_ELECTRIC_SURGE); }
        OPPONENT(SPECIES_SANDSHREW);
    } WHEN {
        TURN { MOVE(player, MOVE_TERRAIN_PULSE); }
    } SCENE {
        MESSAGE("발밑에 전기가 흐르기 시작했다!");
        MESSAGE("상대 모래두지에게는 효과가 없는 것 같다...");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TERRAIN_PULSE, player);
    }
}
