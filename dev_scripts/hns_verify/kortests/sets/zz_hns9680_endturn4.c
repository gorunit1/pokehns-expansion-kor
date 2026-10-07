#include "global.h"
#include "test/battle.h"

// seq 132 #9680 area D: end-of-turn output regression set, part 4 (form changes, switches, fainting, battle end,
// Dynamax end, trainer slides, stage order in one end of turn).
// Scratch only, never committed. Expected output = HnS before #9680 (HEAD 18f412e9ff, code 587f4e7cdc).
// Tests marked "UPSTREAM ORDER CHANGE EXPECTED" pin the HnS order that upstream #9680 changes on purpose
// (Dynamax end moves after fainted-mon handling and trainer slides). A FAIL there after the port is an upstream
// behaviour change to record (D4), not an HnS regression.
// A space in MESSAGE matches one space or newline of the real text.

SINGLE_BATTLE_TEST("HNS9680 4-01 Zen Mode (pop-up, animation, text) and Hunger Switch (animation only)")
{
    GIVEN {
        PLAYER(SPECIES_DARMANITAN_STANDARD) { Ability(ABILITY_ZEN_MODE); HP((GetMonData(&PLAYER_PARTY[0], MON_DATA_MAX_HP) / 2) + 1); }
        OPPONENT(SPECIES_MORPEKO_FULL_BELLY) { Ability(ABILITY_HUNGER_SWITCH); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("상대 모르페코는 할퀴기를 썼다!");
        HP_BAR(player);
        ABILITY_POPUP(player, ABILITY_ZEN_MODE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("달마모드 발동!");
        NONE_OF { ABILITY_POPUP(opponent, ABILITY_HUNGER_SWITCH); }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, opponent);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_DARMANITAN_ZEN);
        EXPECT_EQ(opponent->species, SPECIES_MORPEKO_HANGRY);
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-02 Power Construct at end of turn")
{
    GIVEN {
        PLAYER(SPECIES_ZYGARDE_50_POWER_CONSTRUCT) { Ability(ABILITY_POWER_CONSTRUCT); HP((GetMonData(&PLAYER_PARTY[0], MON_DATA_MAX_HP) / 2) + 1); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
        TURN {}
    } SCENE {
        HP_BAR(player);
        MESSAGE("많은 기척이 느껴진다...!");
        ABILITY_POPUP(player, ABILITY_POWER_CONSTRUCT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_POWER_CONSTRUCT, player);
        MESSAGE("지가르데는 퍼펙트폼으로 바뀌었다!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_ZYGARDE_COMPLETE);
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-03 Schooling and Shields Down revert at end of turn (pop-up, animation, no text)")
{
    u32 species;
    PARAMETRIZE { species = SPECIES_WISHIWASHI_SOLO; }
    PARAMETRIZE { species = SPECIES_MINIOR_CORE_RED; }
    GIVEN {
        if (species == SPECIES_WISHIWASHI_SOLO)
            PLAYER(SPECIES_WISHIWASHI_SOLO) { Level(20); HP(GetMonData(&PLAYER_PARTY[0], MON_DATA_MAX_HP) / 2); Ability(ABILITY_SCHOOLING); }
        else
            PLAYER(SPECIES_MINIOR_CORE_RED) { Ability(ABILITY_SHIELDS_DOWN); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("상대 마자는 분노의앞니를 썼다!");
        HP_BAR(player);
        ABILITY_POPUP(player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        NONE_OF { ABILITY_POPUP(player); }
    } THEN {
        EXPECT_EQ(player->species, species);
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-04 Emergency Exit at end of turn (foe): replacement sent out")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        MESSAGE("상대 갑주무사는 화상 데미지를 입고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
        HP_BAR(opponent, damage: 16);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, opponent);
        MESSAGE("2는 마자를 내보냈다!");
        MESSAGE("마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-05 Emergency Exit at end of turn (player): replacement sent out")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { SEND_OUT(player, 1); }
        TURN {}
    } SCENE {
        MESSAGE("갑주무사는 화상 데미지를 입고 있다!");
        HP_BAR(player, damage: 16);
        ABILITY_POPUP(player, ABILITY_EMERGENCY_EXIT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SLIDE_OFFSCREEN, player);
        MESSAGE("가랏! 마자용!");
        MESSAGE("마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-06 Eject Pack at end of turn after Octolock (foe)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_EJECT_PACK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 탈출팩 때문에 돌아간다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-07 Eject Pack at end of turn after Moody (player)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_MOODY); Item(ITEM_EJECT_PACK); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { SEND_OUT(player, 1); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_MOODY);
        MESSAGE("마자용의 특수방어가 크게 올라갔다!");
        MESSAGE("마자용의 특수공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 탈출팩 때문에 돌아간다!");
        MESSAGE("가랏! 마자!");
        MESSAGE("마자는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-08 Foe faints from poison: later end-turn effects, then replacement")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("마자용은 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-09 Player mon faints from poison: foe Speed Boost, then replacement")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SPEED_BOOST); }
    } WHEN {
        TURN { SEND_OUT(player, 1); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
        HP_BAR(player, hp: 0);
        MESSAGE("마자용은 쓰러졌다!");
        ABILITY_POPUP(opponent, ABILITY_SPEED_BOOST);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
        MESSAGE("가랏! 마자!");
        MESSAGE("상대 마자는 축하를 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9680 4-10 Two foes faint at end of turn (doubles): remaining effects, then both replacements")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Ability(ABILITY_SPEED_BOOST); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Item(ITEM_LEFTOVERS); HP(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); HP(1); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); HP(1); Status1(STATUS1_POISON); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { SEND_OUT(opponentLeft, 2); SEND_OUT(opponentRight, 3); }
        TURN {}
    } SCENE {
        MESSAGE("마자는 먹다남은음식으로 인해 조금 회복했다.");
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("상대 마자용은 화상 데미지를 입고 있다!");
        MESSAGE("상대 마자용은 쓰러졌다!");
        ABILITY_POPUP(playerLeft, ABILITY_SPEED_BOOST);
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("2는 마자를 내보냈다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("마자는 먹다남은음식으로 인해 조금 회복했다.");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-11 Last foe faints from poison: battle won, no later end-turn effects")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); }
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        NONE_OF { ABILITY_POPUP(player, ABILITY_SPEED_BOOST); }
        MESSAGE("2와의 승부에서 이겼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-12 Sandstorm knocks out the last foe: battle won before Leftovers and Speed Boost")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); Item(ITEM_LEFTOVERS); HP(100); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); }
    } SCENE {
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 상대 마자를 덮쳤다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        NONE_OF {
            MESSAGE("모래바람이 마자용을 덮쳤다!");
            MESSAGE("마자용은 먹다남은음식으로 인해 조금 회복했다.");
            ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        }
        MESSAGE("2와의 승부에서 이겼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-13 Dynamax ends after three turns")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, player);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player); }
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-14 UPSTREAM ORDER CHANGE EXPECTED: Dynamax ends before the fainted foe is replaced")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { MaxHP(80); HP(30); Status1(STATUS1_POISON); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("2는 마자용을 내보냈다!");
    }
}

AI_SINGLE_BATTLE_TEST("HNS9680 4-15 Trainer slide at end of turn comes after Speed Boost")
{
    GIVEN {
        FLAG_SET(TESTING_FLAG_TRAINER_SLIDES);
        VAR_SET(TESTING_VAR_TRAINER_SLIDES, TRAINER_SLIDE_LAST_HALF_HP);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); MaxHP(160); HP(81); Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, damage: 20);
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("Trainer A: Enemy last Mon has < 51% HP.{PAUSE_UNTIL_PRESS}");
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        NONE_OF { MESSAGE("Trainer A: Enemy last Mon has < 51% HP.{PAUSE_UNTIL_PRESS}"); }
    }
}

AI_SINGLE_BATTLE_TEST("HNS9680 4-16 UPSTREAM ORDER CHANGE EXPECTED: Dynamax end before the trainer slide of the same turn")
{
    GIVEN {
        FLAG_SET(TESTING_FLAG_TRAINER_SLIDES);
        VAR_SET(TESTING_VAR_TRAINER_SLIDES, TRAINER_SLIDE_LAST_HALF_HP);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); MaxHP(160); HP(121); Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        HP_BAR(opponent, hp: 61);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Trainer A: Enemy last Mon has < 51% HP.{PAUSE_UNTIL_PRESS}");
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9680 4-17 Stage order in one end of turn (doubles): weather, abilities, items, seed, status, Speed Boost, orbs")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); HP(100); Item(ITEM_LEFTOVERS); Status1(STATUS1_POISON); Ability(ABILITY_SPEED_BOOST); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); HP(100); Ability(ABILITY_RAIN_DISH); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_TOXIC_ORB); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); HP(100); Ability(ABILITY_ICE_BODY); Item(ITEM_FLAME_ORB); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_RAIN_DANCE); MOVE(playerRight, MOVE_LEECH_SEED, target: opponentLeft); MOVE(opponentLeft, MOVE_REFLECT); }
        TURN {}
    } SCENE {
        MESSAGE("상대는 리플렉터로 물리공격에 강해졌다!");
        MESSAGE("비가 내리고 있다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES);
        ABILITY_POPUP(playerRight, ABILITY_RAIN_DISH);
        HP_BAR(playerRight, damage: -18);
        MESSAGE("마자용은 먹다남은음식으로 인해 조금 회복했다.");
        HP_BAR(playerLeft, damage: -30);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_LEECH_SEED_DRAIN, opponentLeft);
        HP_BAR(opponentLeft, damage: 61);
        HP_BAR(playerRight, damage: -61);
        MESSAGE("씨뿌리기가 상대 마자용의 체력을 빼앗는다!");
        MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
        MESSAGE("마자는 화상 데미지를 입고 있다!");
        ABILITY_POPUP(playerLeft, ABILITY_SPEED_BOOST);
        MESSAGE("상대 마자용은 맹독구슬 때문에 맹독에 중독됐다!");
        MESSAGE("상대 마자는 화염구슬 때문에 화상을 입었다!");
        // turn 2: the new statuses now deal damage in speed order
        MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        MESSAGE("마자는 화상 데미지를 입고 있다!");
        MESSAGE("상대 마자는 화상 데미지를 입고 있다!");
        ABILITY_POPUP(playerLeft, ABILITY_SPEED_BOOST);
    }
}

SINGLE_BATTLE_TEST("HNS9680 4-18 UPSTREAM CHANGE EXPECTED: last foe faints at end of turn while the player is Dynamaxed, no Dynamax end animation")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, player);
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자는 쓰러졌다!");
        NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player); }
        MESSAGE("2와의 승부에서 이겼다!");
    }
}
