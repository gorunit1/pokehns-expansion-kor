#include "global.h"
#include "test/battle.h"

// seq 161 #9168 scratch only (never committed): Korean output order of every berry / held-item activation path that
// #9168 touches. Expected output = HnS before #9168 (chunk-159-162/base). The item pop-up (CreateItemPopUp) is not a
// recorded test event before #10321 (seq 413); the trace (ITEM lines, tmp-161/kortests/tracecmp161.py) checks it.
// The held item animation id is the only intended difference:
//   before #9168  berries use B_ANIM_HELD_ITEM_EFFECT
//   after  #9168  berries use B_ANIM_HELD_ITEM_BERRY, other held items keep B_ANIM_HELD_ITEM_EFFECT
// Tests tagged [ANIM ADDED] (Fling / Bug Bite / Stuff Cheeks) also gain the berry animation after #9168 because
// HITMARKER_DISABLE_ANIMATION is no longer set around consumeberry. Tagged [RIPEN POPUP] = Berry Juice + Ripen.
// A space in MESSAGE matches one space or newline of the real text. Every battle ends with an idle TURN {} so the
// item pop-up task is freed before the battle ends (headless "task not freed", before #10321).

#ifdef B_ANIM_HELD_ITEM_BERRY
#define HNS9168_PORTED TRUE
#define HNS_BERRY_ANIM B_ANIM_HELD_ITEM_BERRY
#else
#define HNS9168_PORTED FALSE
#define HNS_BERRY_ANIM B_ANIM_HELD_ITEM_EFFECT
#endif

// [ANIM ADDED]: after #9168 the eaten berry plays its animation; before, HITMARKER_DISABLE_ANIMATION skipped it.
// run.sh with KEEP_NOANIM=1 defines HNS9168_KEEP_NOANIM (tree has seq161-9168-optional-keep-noanim.patch on top).
#if HNS9168_PORTED && !defined(HNS9168_KEEP_NOANIM)
#define EATEN_BERRY_ANIM(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, battler)
#else
#define EATEN_BERRY_ANIM(battler) NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, battler); }
#endif
// [ANIM ADDED]: the disliked-flavor confusion animation of a berry eaten by Bug Bite was skipped for the same reason.
#if HNS9168_PORTED && !defined(HNS9168_KEEP_NOANIM)
#define EATEN_CONFUSION_ANIM(battler) ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CONFUSION, battler)
#else
#define EATEN_CONFUSION_ANIM(battler) NONE_OF { ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CONFUSION, battler); }
#endif

// ---- status cure berries: BattleScript_BerryCureStatusRet ----

SINGLE_BATTLE_TEST("HNS9168 K01 status cure berries (Cheri, Rawst, Pecha, Chesto) after the status move")
{
    enum Item item;
    u32 move;
    PARAMETRIZE { item = ITEM_CHERI_BERRY; move = MOVE_THUNDER_WAVE; }
    PARAMETRIZE { item = ITEM_RAWST_BERRY; move = MOVE_WILL_O_WISP; }
    PARAMETRIZE { item = ITEM_PECHA_BERRY; move = MOVE_TOXIC; }
    PARAMETRIZE { item = ITEM_CHESTO_BERRY; move = MOVE_SPORE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, move); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        if (item == ITEM_CHERI_BERRY)
            MESSAGE("마자용은 버치열매로 마비가 풀렸다!");
        else if (item == ITEM_RAWST_BERRY)
            MESSAGE("마자용은 복분열매로 화상이 나았다!");
        else if (item == ITEM_PECHA_BERRY)
            MESSAGE("마자용은 복슝열매로 독이 해독됐다!");
        else
            MESSAGE("마자용은 유루열매로 눈을 떴다!");
        STATUS_ICON(player, none: TRUE);
    } THEN {
        EXPECT_EQ(player->status1, STATUS1_NONE);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K02 Aspear Berry thaws the holder on switch-in (opponent side)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(STATUS1_FREEZE); Item(ITEM_ASPEAR_BERRY); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 마자용은 배리열매로 얼음 상태가 나았다!");
    } THEN {
        EXPECT_EQ(opponent->status1, STATUS1_NONE);
    }
}

// ---- Lum Berry: HnS BattleScript_LumBerryCureStatusRet (per-status message loop) ----

SINGLE_BATTLE_TEST("HNS9168 K03 Lum Berry cures paralysis")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_THUNDER_WAVE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER_WAVE, opponent);
        MESSAGE("마자용은 마비되어 기술이 나오기 어려워졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 리샘열매로 마비가 풀렸다!");
        NOT MESSAGE("마자용은 리샘열매로 혼란이 풀렸다!");
    } THEN {
        EXPECT_EQ(player->status1, STATUS1_NONE);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K04 Lum Berry cures confusion only (HnS pop-up R30)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, opponent);
        MESSAGE("마자용은 혼란에 빠졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 리샘열매로 혼란이 풀렸다!");
    } THEN {
        EXPECT(player->volatiles.confusionTurns == 0);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K05 Lum Berry received by Switcheroo cures paralysis and confusion (two messages)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Status1(STATUS1_PARALYSIS); Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LUM_BERRY); Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); MOVE(player, MOVE_CELEBRATE, WITH_RNG(RNG_CONFUSION, FALSE)); }
        TURN { MOVE(opponent, MOVE_SWITCHEROO); MOVE(player, MOVE_CELEBRATE, WITH_RNG(RNG_PARALYSIS, FALSE)); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWITCHEROO, opponent);
        MESSAGE("상대 마자는 서로의 도구를 교체했다!");
        MESSAGE("마자용은 리샘열매를 손에 넣었다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 리샘열매로 마비가 풀렸다!");
        MESSAGE("마자용은 리샘열매로 혼란이 풀렸다!");
        STATUS_ICON(player, none: TRUE);
    } THEN {
        EXPECT_EQ(player->status1, STATUS1_NONE);
        EXPECT(player->volatiles.confusionTurns == 0);
    }
}

// ---- Persim: BattleScript_BerryCureConfusionRet (HnS pop-up R28) ----

SINGLE_BATTLE_TEST("HNS9168 K06 Persim Berry cures confusion")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_PERSIM_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, opponent);
        MESSAGE("마자용은 혼란에 빠졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 시몬열매로 혼란이 풀렸다!");
    }
}

// ---- HP restore: BattleScript_ItemHealHP_RemoveItem -> RemoveBerry / RemoveItem ----

SINGLE_BATTLE_TEST("HNS9168 K07 Oran and Sitrus Berry restore HP after a hit")
{
    enum Item item;
    PARAMETRIZE { item = ITEM_ORAN_BERRY; }
    PARAMETRIZE { item = ITEM_SITRUS_BERRY; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(60); Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        HP_BAR(player, damage: 40);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        if (item == ITEM_ORAN_BERRY)
            MESSAGE("마자용은 오랭열매로 체력을 회복했다!");
        else
            MESSAGE("마자용은 자뭉열매로 체력을 회복했다!");
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K08 Berry Juice restores HP after a hit (not a berry: animation id unchanged)")
{
    GIVEN {
        ASSUME(GetItemPocket(ITEM_BERRY_JUICE) != POCKET_BERRIES);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(60); Item(ITEM_BERRY_JUICE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        HP_BAR(player, damage: 40);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 나무열매쥬스로 체력을 회복했다!");
        HP_BAR(player, damage: -20);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K09 Oran Berry with Ripen (ability pop-up, item pop-up, animation)")
{
    GIVEN {
        PLAYER(SPECIES_APPLIN) { Ability(ABILITY_RIPEN); MaxHP(100); HP(60); Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ABILITY_POPUP(player, ABILITY_RIPEN);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("과사삭벌레는 오랭열매로 체력을 회복했다!");
        HP_BAR(player, damage: -20);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K10 [RIPEN POPUP] Berry Juice held by a Ripen Pokemon")
{
    GIVEN {
        PLAYER(SPECIES_APPLIN) { Ability(ABILITY_RIPEN); MaxHP(100); HP(60); Item(ITEM_BERRY_JUICE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
#if HNS9168_PORTED
        NONE_OF { ABILITY_POPUP(player, ABILITY_RIPEN); }
#else
        ABILITY_POPUP(player, ABILITY_RIPEN);
#endif
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("과사삭벌레는 나무열매쥬스로 체력을 회복했다!");
        HP_BAR(player, damage: -20);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K11 Figy Berry, flavor not disliked")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Nature(NATURE_ADAMANT); MaxHP(100); HP(60); Item(ITEM_FIGY_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 무화열매로 체력을 회복했다!");
        HP_BAR(player);
        NOT MESSAGE("마자용은 혼란에 빠졌다!");
    } THEN {
        EXPECT(player->volatiles.confusionTurns == 0);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K12 Figy Berry, disliked flavor (BerryConfuseHeal, no item pop-up in HnS)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Nature(NATURE_BOLD); MaxHP(100); HP(60); Item(ITEM_FIGY_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 무화열매로 체력을 회복했다!");
        HP_BAR(player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CONFUSION, player);
        MESSAGE("마자용은 혼란에 빠졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K13 Enigma Berry after a super effective hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); Item(ITEM_ENIGMA_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_BITE, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITE, opponent);
        HP_BAR(player);
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 의문열매로 체력을 회복했다!");
        HP_BAR(player);
    }
}

// ---- PP: BattleScript_BerryPPHeal ----

SINGLE_BATTLE_TEST("HNS9168 K14 Leppa Berry restores PP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MovesWithPP({MOVE_CELEBRATE, 1}, {MOVE_SCRATCH, 10}); Item(ITEM_LEPPA_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 과사열매로 축하의 PP를 회복했다!");
        MESSAGE("마자용은 할퀴기를 썼다!");
    }
}

// ---- stat berries: BattleScript_ConsumableStatRaiseRet -> ConsumableBerryStatRaise ----

SINGLE_BATTLE_TEST("HNS9168 K15 pinch stat berries (Liechi, Ganlon, Salac, Petaya, Apicot)")
{
    enum Item item;
    PARAMETRIZE { item = ITEM_LIECHI_BERRY; }
    PARAMETRIZE { item = ITEM_GANLON_BERRY; }
    PARAMETRIZE { item = ITEM_SALAC_BERRY; }
    PARAMETRIZE { item = ITEM_PETAYA_BERRY; }
    PARAMETRIZE { item = ITEM_APICOT_BERRY; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(60); Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        if (item == ITEM_LIECHI_BERRY)
            MESSAGE("마자용은 치리열매로 공격이 올라갔다!");
        else if (item == ITEM_GANLON_BERRY)
            MESSAGE("마자용은 용아열매로 방어가 올라갔다!");
        else if (item == ITEM_SALAC_BERRY)
            MESSAGE("마자용은 캄라열매로 스피드가 올라갔다!");
        else if (item == ITEM_PETAYA_BERRY)
            MESSAGE("마자용은 야타비열매로 특수공격이 올라갔다!");
        else
            MESSAGE("마자용은 규살열매로 특수방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K16 Liechi Berry with Ripen")
{
    GIVEN {
        PLAYER(SPECIES_APPLIN) { Ability(ABILITY_RIPEN); MaxHP(100); HP(60); Item(ITEM_LIECHI_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ABILITY_POPUP(player, ABILITY_RIPEN);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("과사삭벌레는 치리열매로 공격이 크게 올라갔다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K17 Starf Berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(60); Item(ITEM_STARF_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 스타열매로 특수방어가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K18 Kee Berry (physical) and Maranga Berry (special)")
{
    enum Item item;
    u32 move;
    PARAMETRIZE { item = ITEM_KEE_BERRY; move = MOVE_SCRATCH; }
    PARAMETRIZE { item = ITEM_MARANGA_BERRY; move = MOVE_EMBER; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, move, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        HP_BAR(player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        if (item == ITEM_KEE_BERRY)
            MESSAGE("마자용은 악키열매로 방어가 올라갔다!");
        else
            MESSAGE("마자용은 타라프열매로 특수방어가 올라갔다!");
    }
}

// ---- not berries on the same label: ConsumableItemStatRaise keeps B_ANIM_HELD_ITEM_EFFECT ----

SINGLE_BATTLE_TEST("HNS9168 K19 Room Service under Trick Room (not a berry)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ROOM_SERVICE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TRICK_ROOM); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK_ROOM, opponent);
        MESSAGE("상대 마자는 시공을 뒤틀었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 룸서비스로 스피드가 떨어졌다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE - 1);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K20 Electric Seed when Electric Terrain starts (not a berry)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ELECTRIC_SEED); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ELECTRIC_TERRAIN, opponent);
        MESSAGE("발밑에 전기가 흐르기 시작했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 일렉트릭시드로 방어가 올라갔다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
    }
}

// ---- other berry scripts that only change the animation id ----

SINGLE_BATTLE_TEST("HNS9168 K21 Lansat Berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(60); Item(ITEM_LANSAT_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 랑사열매를 써서 의욕이 넘치기 시작했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K22 Micle Berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(60); Item(ITEM_MICLE_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 미클열매로 다음에 쓸 기술이 명중하기 쉬워졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K23 Custap Berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); MaxHP(160); HP(40); Item(ITEM_CUSTAP_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 애슈열매로 행동이 빨라졌다!");
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K24 Jaboca Berry (physical) and Rowap Berry (special)")
{
    enum Item item;
    u32 move;
    PARAMETRIZE { item = ITEM_JABOCA_BERRY; move = MOVE_SCRATCH; }
    PARAMETRIZE { item = ITEM_ROWAP_BERRY; move = MOVE_EMBER; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, move, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        HP_BAR(player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        HP_BAR(opponent);
        MESSAGE("상대 마자는 상처를 입었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K25 Colbur Berry weakens a super effective Bite")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_COLBUR_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_BITE, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 물기를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 입는 데미지를 마코열매가 약하게 했다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITE, opponent);
        HP_BAR(player);
        MESSAGE("효과가 굉장했다!");
    }
}

// ---- berries eaten by moves: HITMARKER_DISABLE_ANIMATION removed by #9168 ----

SINGLE_BATTLE_TEST("HNS9168 K26 [ANIM ADDED] Fling throws a berry that the target eats (Oran, Cheri, Liechi)")
{
    enum Item item;
    u32 status1;
    PARAMETRIZE { item = ITEM_ORAN_BERRY; status1 = STATUS1_NONE; }
    PARAMETRIZE { item = ITEM_CHERI_BERRY; status1 = STATUS1_PARALYSIS; }
    PARAMETRIZE { item = ITEM_LIECHI_BERRY; status1 = STATUS1_NONE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); Attack(1); }
        OPPONENT(SPECIES_WYNAUT) { Status1(status1); MaxHP(400); HP(399); }
    } WHEN {
        TURN { MOVE(player, MOVE_FLING); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_PARALYSIS, FALSE)); }
        TURN { MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_PARALYSIS, FALSE)); }
    } SCENE {
        if (item == ITEM_ORAN_BERRY)
            MESSAGE("마자용은 오랭열매를 내던졌다!");
        else if (item == ITEM_CHERI_BERRY)
            MESSAGE("마자용은 버치열매를 내던졌다!");
        else
            MESSAGE("마자용은 치리열매를 내던졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLING, player);
        HP_BAR(opponent);
        MESSAGE("효과가 굉장했다!");
        EATEN_BERRY_ANIM(opponent);
        if (item == ITEM_ORAN_BERRY) {
            MESSAGE("상대 마자는 오랭열매로 체력을 회복했다!");
            HP_BAR(opponent);
        } else if (item == ITEM_CHERI_BERRY) {
            MESSAGE("상대 마자는 버치열매로 마비가 풀렸다!");
        } else {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            MESSAGE("상대 마자는 치리열매로 공격이 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K27 [ANIM ADDED] Bug Bite eats the target's berry (Sitrus, Liechi)")
{
    enum Item item;
    PARAMETRIZE { item = ITEM_SITRUS_BERRY; }
    PARAMETRIZE { item = ITEM_LIECHI_BERRY; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(399); }
        OPPONENT(SPECIES_WYNAUT) { Item(item); }
    } WHEN {
        TURN { MOVE(player, MOVE_BUG_BITE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BUG_BITE, player);
        HP_BAR(opponent);
        MESSAGE("효과가 굉장했다!");
        if (item == ITEM_SITRUS_BERRY) {
            MESSAGE("마자용은 자뭉열매를 빼앗아 먹었다!");
            EATEN_BERRY_ANIM(player);
            MESSAGE("마자용은 자뭉열매로 체력을 회복했다!");
            HP_BAR(player);
        } else {
            MESSAGE("마자용은 치리열매를 빼앗아 먹었다!");
            EATEN_BERRY_ANIM(player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
            MESSAGE("마자용은 치리열매로 공격이 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K28 [ANIM ADDED] Bug Bite eats a disliked Figy Berry (confusion animation too)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Nature(NATURE_BOLD); MaxHP(400); HP(399); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_FIGY_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_BUG_BITE); }
        TURN { MOVE(player, MOVE_CELEBRATE, WITH_RNG(RNG_CONFUSION, FALSE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BUG_BITE, player);
        HP_BAR(opponent);
        MESSAGE("효과가 굉장했다!");
        MESSAGE("마자용은 무화열매를 빼앗아 먹었다!");
        EATEN_BERRY_ANIM(player);
        MESSAGE("마자용은 무화열매로 체력을 회복했다!");
        HP_BAR(player);
        EATEN_CONFUSION_ANIM(player);
        MESSAGE("마자용은 혼란에 빠졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K29 [ANIM ADDED] Stuff Cheeks (Oran, Liechi)")
{
    enum Item item;
    PARAMETRIZE { item = ITEM_ORAN_BERRY; }
    PARAMETRIZE { item = ITEM_LIECHI_BERRY; }
    GIVEN {
        PLAYER(SPECIES_SKWOVET) { MaxHP(400); HP(399); Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_STUFF_CHEEKS); }
        TURN {}
    } SCENE {
        MESSAGE("탐리스는 볼가득넣기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STUFF_CHEEKS, player);
        EATEN_BERRY_ANIM(player);
        if (item == ITEM_ORAN_BERRY) {
            MESSAGE("탐리스는 오랭열매로 체력을 회복했다!");
            HP_BAR(player);
        } else {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
            MESSAGE("탐리스는 치리열매로 공격이 올라갔다!");
            ABILITY_POPUP(player, ABILITY_CHEEK_POUCH);
        }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("탐리스의 방어가 크게 올라갔다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 2);
    }
}

// ---- abilities and moves that re-eat or force berries (animation id only) ----

SINGLE_BATTLE_TEST("HNS9168 K30 Cud Chew eats the Oran Berry again on the next turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TAUROS_PALDEA_COMBAT) { Ability(ABILITY_CUD_CHEW); MaxHP(100); HP(60); Item(ITEM_ORAN_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 켄타로스는 오랭열매로 체력을 회복했다!");
        HP_BAR(opponent, damage: -10);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
        ABILITY_POPUP(opponent, ABILITY_CUD_CHEW);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 켄타로스는 오랭열매로 체력을 회복했다!");
        HP_BAR(opponent, damage: -10);
    }
}

SINGLE_BATTLE_TEST("HNS9168 K31 Teatime: both sides eat their Liechi Berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIECHI_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LIECHI_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_TEATIME); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TEATIME, player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("마자용은 치리열매로 공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 마자는 치리열매로 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K32 Harvest restores the Sitrus Berry in sunlight")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_EXEGGUTOR) { Ability(ABILITY_HARVEST); MaxHP(500); HP(251); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_DRAGON_RAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 나시는 자뭉열매로 체력을 회복했다!");
        ABILITY_POPUP(opponent, ABILITY_HARVEST);
        MESSAGE("상대 나시는 자뭉열매를 수확했다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 나시는 자뭉열매로 체력을 회복했다!");
        ABILITY_POPUP(opponent, ABILITY_HARVEST);
        MESSAGE("상대 나시는 자뭉열매를 수확했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K33 Cheek Pouch after an Oran Berry")
{
    GIVEN {
        PLAYER(SPECIES_GREEDENT) { Ability(ABILITY_CHEEK_POUCH); MaxHP(60); HP(31); Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, player);
        MESSAGE("요씽리스는 오랭열매로 체력을 회복했다!");
        HP_BAR(player, damage: -10);
        ABILITY_POPUP(player, ABILITY_CHEEK_POUCH);
        HP_BAR(player, damage: -20);
    }
}

// ---- controls: held items that are not berries keep B_ANIM_HELD_ITEM_EFFECT; plain stat drop path ----

SINGLE_BATTLE_TEST("HNS9168 K34 control: Mental Herb, White Herb, Leftovers")
{
    enum Item item;
    u32 move;
    PARAMETRIZE { item = ITEM_MENTAL_HERB; move = MOVE_TAUNT; }
    PARAMETRIZE { item = ITEM_WHITE_HERB; move = MOVE_GROWL; }
    PARAMETRIZE { item = ITEM_LEFTOVERS; move = MOVE_DRAGON_RAGE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(80); Item(item); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, move); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        if (item == ITEM_MENTAL_HERB)
            MESSAGE("마자용은 도발에 넘어가 버렸다!");
        else if (item == ITEM_WHITE_HERB)
            MESSAGE("마자용의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        if (item == ITEM_MENTAL_HERB)
            MESSAGE("마자용은 도발의 효과가 풀렸다!");
        else if (item == ITEM_WHITE_HERB)
            MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        else
            MESSAGE("마자용은 먹다남은음식으로 인해 조금 회복했다.");
    }
}

SINGLE_BATTLE_TEST("HNS9168 K35 control: Growl without an item (BattleScript_StatDownDoAnim)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_GROWL); }
        TURN { MOVE(opponent, MOVE_GROWL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE - 2);
    }
}

DOUBLE_BATTLE_TEST("HNS9168 K36 doubles: two Sitrus Berries after a spread move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(100); HP(55); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { MaxHP(100); HP(55); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SURF); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SURF, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponentLeft);
        MESSAGE("상대 마자용은 자뭉열매로 체력을 회복했다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponentRight);
        MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!");
    }
}
