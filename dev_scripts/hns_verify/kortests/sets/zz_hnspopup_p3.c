#include "global.h"
#include "test/battle.h"

// Friend request C (P3): optional item pop-up patches, order regression set.
// Scratch only, never committed. Run on scratch copies with chunk-132/D-tests/trace.patch applied:
// item pop-ups are not visible to SCENE before upstream #10321 (seq 413), so pop-up position is checked from
// the trace (TR:I lines) by tmp-P3/check_popups.py. SCENE only pins events that must not move.
// Every battle ends with one idle TURN {} without item activations: before #10321 CreateItemPopUp() also runs
// in headless tests, and a battle that ends right after a pop-up can fail with "task not freed".
//   H = p3-opt-healhp (R20 Shell Bell, R25 Leftovers / Black Sludge heal, R26 control)
//   B = p3-opt-airballoon-in (R09), P = p3-opt-airballoon-pop (R10)
//   S = p3-opt-seed (R02 seeds; R01 Room Service, R19 Kee, R35 pinch berries, R37 Starf share the label)
//   D = p3-opt-destinyknot (R42; R41 control)

SINGLE_BATTLE_TEST("HNSPOPUP H01 Leftovers end of turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(150); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        HP_BAR(player, damage: -10);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP H02 Shell Bell after a hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP H03 Black Sludge heals a Poison type")
{
    GIVEN {
        PLAYER(SPECIES_GRIMER) { MaxHP(160); HP(150); Item(ITEM_BLACK_SLUDGE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        HP_BAR(player, damage: -10);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP H04 control: Black Sludge hurts a non-Poison type (no pop-up in HnS)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_BLACK_SLUDGE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player);
    }
}

DOUBLE_BATTLE_TEST("HNSPOPUP H05 Leftovers on two battlers, end of turn order")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(150); Item(ITEM_LEFTOVERS); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(160); HP(150); Item(ITEM_LEFTOVERS); Speed(40); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
        HP_BAR(opponentRight, damage: -10);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        HP_BAR(playerLeft, damage: -10);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP B01 Air Balloon at battle start")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_AIR_BALLOON); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("마자용은 풍선 때문에 떠 있다!");
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP B02 Air Balloon on switch-in")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_AIR_BALLOON); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN {}
    } SCENE {
        MESSAGE("마자는 풍선 때문에 떠 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNSPOPUP B03 Air Balloon on both sides at battle start")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_AIR_BALLOON); Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_AIR_BALLOON); Speed(40); }
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 마자용은 풍선 때문에 떠 있다!");
        MESSAGE("마자용은 풍선 때문에 떠 있다!");
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP P01 Air Balloon pops")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_AIR_BALLOON); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        MESSAGE("마자용의 풍선이 터졌다!");
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("HNSPOPUP P02 Air Balloon pops on two targets of a spread move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_AIR_BALLOON); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_AIR_BALLOON); }
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_HYPER_VOICE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, opponentLeft);
        MESSAGE("마자용의 풍선이 터졌다!");
        MESSAGE("마자의 풍선이 터졌다!");
    } THEN {
        EXPECT_EQ(playerLeft->item, ITEM_NONE);
        EXPECT_EQ(playerRight->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP P03 Air Balloon pops when the holder faints")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Item(ITEM_AIR_BALLOON); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); SEND_OUT(player, 1); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        MESSAGE("마자용의 풍선이 터졌다!");
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S01 Electric Seed on Electric Terrain")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ELECTRIC_SEED); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ELECTRIC_TERRAIN, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S02 Grassy Seed on switch-in into Grassy Terrain")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_GRASSY_SEED); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_GRASSY_TERRAIN); }
        TURN { SWITCH(player, 1); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("HNSPOPUP S03 Psychic Seed on two battlers")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_PSYCHIC_SEED); }
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_PSYCHIC_SEED); }
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PSYCHIC_TERRAIN); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYCHIC_TERRAIN, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerRight);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S04 Misty Seed after the foe's Misty Surge at battle start (ability pop-up first)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MISTY_SEED); }
        OPPONENT(SPECIES_TAPU_FINI) { Ability(ABILITY_MISTY_SURGE); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_MISTY_SURGE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S05 side effect: Salac Berry (R35)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(26); Item(ITEM_SALAC_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S06 side effect: Ripen Liechi Berry (ability pop-up, then item pop-up)")
{
    GIVEN {
        PLAYER(SPECIES_APPLETUN) { Ability(ABILITY_RIPEN); MaxHP(100); HP(26); Item(ITEM_LIECHI_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponent);
        ABILITY_POPUP(player, ABILITY_RIPEN);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S07 side effect: Room Service on Trick Room (R01)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ROOM_SERVICE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TRICK_ROOM); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK_ROOM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 1);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S08 side effect: Kee Berry (R19)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_KEE_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP S09 control: Electric Seed at +6 Defense does not activate")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ELECTRIC_SEED); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_IRON_DEFENSE); }
        TURN { MOVE(player, MOVE_IRON_DEFENSE); }
        TURN { MOVE(player, MOVE_IRON_DEFENSE); MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        TURN {}
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT_EQ(player->item, ITEM_ELECTRIC_SEED);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP D01 Destiny Knot held by the target of Attract")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_MALE); }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); Item(ITEM_DESTINY_KNOT); }
    } WHEN {
        TURN { MOVE(player, MOVE_ATTRACT); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ATTRACT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("마자용은 빨간실 때문에 헤롱헤롱해졌다!");
    } THEN {
        EXPECT(player->volatiles.infatuation);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP D02 control: Destiny Knot held by the attacker that touched Cute Charm (R41)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_MALE); Item(ITEM_DESTINY_KNOT); }
        OPPONENT(SPECIES_CLEFAIRY) { Gender(MON_FEMALE); Ability(ABILITY_CUTE_CHARM); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_CUTE_CHARM);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
    } THEN {
        EXPECT(opponent->volatiles.infatuation);
    }
}

DOUBLE_BATTLE_TEST("HNSPOPUP D03 Destiny Knot held by the right-hand foe targeted by Attract")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_MALE); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); Item(ITEM_DESTINY_KNOT); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ATTRACT, target: opponentRight); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ATTRACT, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponentRight);
    } THEN {
        EXPECT(playerLeft->volatiles.infatuation);
    }
}
