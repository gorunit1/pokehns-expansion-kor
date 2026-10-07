#include "global.h"
#include "test/battle.h"
// seq 161 #9168: berries use B_ANIM_HELD_ITEM_BERRY after the port (scratch test, updated 2026-10-05)
#ifdef B_ANIM_HELD_ITEM_BERRY
#define HNS_BERRY_ANIM B_ANIM_HELD_ITEM_BERRY
#else
#define HNS_BERRY_ANIM B_ANIM_HELD_ITEM_EFFECT
#endif


// seq 132 #9680 area D: end-of-turn output regression set, part 1 (HnS-specific items and abilities).
// Scratch only, never committed. Expected output = HnS before #9680 (HEAD 18f412e9ff, code 587f4e7cdc).
// A space in MESSAGE matches one space or newline of the real text.
// Item pop-ups (BattleScript_ItemPopUp_*) are not visible to SCENE; see trace.patch / part-D.md.

SINGLE_BATTLE_TEST("HNS9680 1-01 Toxic Orb: HnS message at end of turn, poison damage next turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_TOXIC_ORB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, player);
        MESSAGE("마자용은 맹독구슬 때문에 맹독에 중독됐다!");
        STATUS_ICON(player, badPoison: TRUE);
        NONE_OF { MESSAGE("마자용의 몸에 맹독이 퍼졌다!"); }
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, player);
        HP_BAR(player, damage: 30);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-02 Flame Orb: HnS message at end of turn, burn damage next turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_FLAME_ORB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, player);
        MESSAGE("마자용은 화염구슬 때문에 화상을 입었다!");
        STATUS_ICON(player, burn: TRUE);
        NONE_OF { MESSAGE("마자용은 화상을 입었다!"); }
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용은 화상 데미지를 입고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, player);
        HP_BAR(player, damage: 30);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-03 Poison Heal (poison, Toxic Orb): pop-up, status and heal animations, HP bar, no text")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_POISON_HEAL); Item(ITEM_TOXIC_ORB); HP(100); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_POISON_HEAL); Status1(STATUS1_POISON); HP(100); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        // turn 1: foe Poison Heal, then player's Toxic Orb
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_POISON_HEAL);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
        HP_BAR(opponent, damage: -37);
        NONE_OF { MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!"); }
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, player);
        MESSAGE("마자용은 맹독구슬 때문에 맹독에 중독됐다!");
        STATUS_ICON(player, badPoison: TRUE);
        // turn 2: player first (faster), then foe
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_POISON_HEAL);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player, damage: -61);
        NONE_OF { MESSAGE("마자용은 독에 의한 데미지를 입고 있다!"); }
        ABILITY_POPUP(opponent, ABILITY_POISON_HEAL);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
        HP_BAR(opponent, damage: -37);
        NONE_OF { MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-04 Ice Body (hail, snow): pop-up, heal animation, HP bar, no text")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_ICE_BODY); HP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        MESSAGE("눈이 내리기 시작했다!");
        if (move == MOVE_HAIL) {
            MESSAGE("눈이 내리고 있다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HAIL_CONTINUES);
        } else {
            MESSAGE("눈이 내리고 있다.");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SNOW_CONTINUES);
        }
        ABILITY_POPUP(player, ABILITY_ICE_BODY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player, damage: -30);
        NONE_OF { MESSAGE("마자용's 아이스바디 healed it a little bit!"); }
        if (move == MOVE_HAIL) {
            MESSAGE("싸라기눈이 상대 마자를 덮쳤다!");
            HP_BAR(opponent, damage: 18);
        }
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-05 Solar Power and Dry Skin in sun: pop-up then HP bar, no text")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SOLAR_POWER); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DRY_SKIN); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
    } SCENE {
        MESSAGE("햇살이 강해졌다!");
        MESSAGE("햇살이 강하다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUN_CONTINUES);
        ABILITY_POPUP(player, ABILITY_SOLAR_POWER);
        HP_BAR(player, damage: 61);
        ABILITY_POPUP(opponent, ABILITY_DRY_SKIN);
        HP_BAR(opponent, damage: 37);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-06 Rain Dish and Dry Skin in rain: pop-up, heal animation, HP bar, no text")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_RAIN_DISH); HP(100); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DRY_SKIN); HP(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        MESSAGE("비가 내리기 시작했다!");
        MESSAGE("비가 내리고 있다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES);
        ABILITY_POPUP(player, ABILITY_RAIN_DISH);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player, damage: -30);
        ABILITY_POPUP(opponent, ABILITY_DRY_SKIN);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
        HP_BAR(opponent, damage: -37);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-07 Leftovers heal and Black Sludge damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LEFTOVERS); HP(100); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_BLACK_SLUDGE); }
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 먹다남은음식으로 인해 조금 회복했다.");
        HP_BAR(player, damage: -30);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, opponent);
        HP_BAR(opponent, damage: 37);
        MESSAGE("상대 마자는 검은오물 때문에! 데미지를 입었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-08 Black Sludge heals a Poison type, Sticky Barb damage")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { Item(ITEM_BLACK_SLUDGE); HP(100); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_STICKY_BARB); }
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("아보는 검은오물로 인해 조금 회복했다.");
        HP_BAR(player, damage: -11);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, opponent);
        HP_BAR(opponent, damage: 37);
        MESSAGE("상대 마자는 끈적끈적바늘 때문에! 데미지를 입었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-09 White Herb end-of-turn path after Moody lowers a stat")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_MOODY); Item(ITEM_WHITE_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_MOODY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수방어가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("마자용은 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_MOODY);
        MESSAGE("마자용의 특수공격이 떨어졌다!");
        NONE_OF { MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!"); }
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-10 White Herb end-of-turn path after Octolock lowers stats")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); }
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 문어굳히기 때문에 도망칠 수 없게 되었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_DEF], DEFAULT_STAT_STAGE - 1);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-11 Shed Skin and Hydration cure messages")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SHED_SKIN); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_HYDRATION); Status1(STATUS1_PARALYSIS); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        MESSAGE("비가 내리고 있다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES);
        ABILITY_POPUP(player, ABILITY_SHED_SKIN);
        MESSAGE("마자용의 화상이 나았다!");
        STATUS_ICON(player, none: TRUE);
        ABILITY_POPUP(opponent, ABILITY_HYDRATION);
        MESSAGE("상대 마자의 몸저림이 풀렸다!");
        STATUS_ICON(opponent, none: TRUE);
    }
}

DOUBLE_BATTLE_TEST("HNS9680 1-12 Healer: HnS (SV/Champions) cure text")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_HEALER); }
        PLAYER(SPECIES_WYNAUT) { Status1(STATUS1_POISON); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(playerLeft, ABILITY_HEALER);
        STATUS_ICON(playerRight, none: TRUE);
        MESSAGE("마자는 치유되었다!");
        NONE_OF { MESSAGE("마자는 독에 의한 데미지를 입고 있다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-13 Speed Boost and Moody")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MOODY); }
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 올라갔다!");
        ABILITY_POPUP(opponent, ABILITY_MOODY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수방어가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-14 Harvest, then Cud Chew next turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_HARVEST); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CUD_CHEW); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); MOVE(opponent, MOVE_SUPER_FANG); }
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 자뭉열매로 체력을 회복했다!");
        HP_BAR(player, damage: -30);
        ABILITY_POPUP(player, ABILITY_HARVEST);
        MESSAGE("마자용은 자뭉열매를 수확했다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_CUD_CHEW);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!");
        HP_BAR(opponent, damage: -30);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-15 Pickup")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_PICKUP); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); }
    } SCENE {
        MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_PICKUP);
        MESSAGE("마자용은 자뭉열매를 주워 왔다!");
    } THEN {
        EXPECT_EQ(player->item, ITEM_SITRUS_BERRY);
    }
}

DOUBLE_BATTLE_TEST("HNS9680 1-16 Bad Dreams: one pop-up per turn for two sleeping foes, two turns")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_BAD_DREAMS); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP_TURN(5)); }
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP_TURN(5)); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 쿨쿨 잠들어 있다");
        ABILITY_POPUP(playerLeft, ABILITY_BAD_DREAMS);
        MESSAGE("상대 마자용은 나이트메어에 시달리고 있다!");
        HP_BAR(opponentLeft, damage: 61);
        NONE_OF { ABILITY_POPUP(playerLeft, ABILITY_BAD_DREAMS); }
        MESSAGE("상대 마자는 나이트메어에 시달리고 있다!");
        HP_BAR(opponentRight, damage: 37);
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 쿨쿨 잠들어 있다");
        ABILITY_POPUP(playerLeft, ABILITY_BAD_DREAMS);
        MESSAGE("상대 마자용은 나이트메어에 시달리고 있다!");
        HP_BAR(opponentLeft, damage: 61);
        NONE_OF { ABILITY_POPUP(playerLeft, ABILITY_BAD_DREAMS); }
        MESSAGE("상대 마자는 나이트메어에 시달리고 있다!");
        HP_BAR(opponentRight, damage: 37);
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-17 Slow Start ends after five turns")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SLOW_START); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SLOW_START);
        MESSAGE("마자용은 컨디션이 좋아지지 않는다!");
        NONE_OF { MESSAGE("마자용은 컨디션을 회복했다!"); }
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        ABILITY_POPUP(player, ABILITY_SLOW_START);
        MESSAGE("마자용은 컨디션을 회복했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 1-18 Bad Dreams knocks out a sleeping foe, replacement, next turn pop-up again")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_BAD_DREAMS); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_SLEEP_TURN(5)); }
        OPPONENT(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP_TURN(5)); }
    } WHEN {
        TURN { SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_BAD_DREAMS);
        MESSAGE("상대 마자는 나이트메어에 시달리고 있다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("상대 마자용은 쿨쿨 잠들어 있다");
        ABILITY_POPUP(player, ABILITY_BAD_DREAMS);
        MESSAGE("상대 마자용은 나이트메어에 시달리고 있다!");
        HP_BAR(opponent, damage: 61);
    }
}
