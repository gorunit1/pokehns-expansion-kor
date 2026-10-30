// HnS 1.17.1 뒤 upstream 버그 수정 선택 이식 회귀 테스트(친구 요청 2026-10-30) — 한글 출력으로 확인.
// 이름이 모두 "HnS post-1.17.1"로 시작한다.
#include "global.h"
#include "test/battle.h"

// #10953: upstream 1.17.2 개발판은 #10803 뒤 반무적 기술·지오컨트롤이 날씨 속에서 두 턴 기술 날씨 assert에 걸린다.
// HnS(1.17.1 기반)는 그 전에 걸러 내므로 같은 장면이 그대로 진행돼야 한다.
SINGLE_BATTLE_TEST("HnS post-1.17.1 #10953: semi-invulnerable moves and Geomancy work in weather")
{
    enum Move weatherMove, move;
    PARAMETRIZE { weatherMove = MOVE_RAIN_DANCE;  move = MOVE_DIG; }
    PARAMETRIZE { weatherMove = MOVE_SUNNY_DAY;   move = MOVE_FLY; }
    PARAMETRIZE { weatherMove = MOVE_RAIN_DANCE;  move = MOVE_DIVE; }
    PARAMETRIZE { weatherMove = MOVE_SANDSTORM;   move = MOVE_BOUNCE; }
    PARAMETRIZE { weatherMove = MOVE_SNOWSCAPE;   move = MOVE_PHANTOM_FORCE; }
    PARAMETRIZE { weatherMove = MOVE_SUNNY_DAY;   move = MOVE_GEOMANCY; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, weatherMove); MOVE(opponent, move); }
        TURN { SKIP_TURN(opponent); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, weatherMove, player);
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        if (move != MOVE_GEOMANCY)
            HP_BAR(player);
    }
}

// #10972: 난기류는 비행 타입 쪽 약점만 줄인다. 다른 타입 때문에 효과가 굉장한 기술에는 문장이 나오지 않는다.
SINGLE_BATTLE_TEST("HnS post-1.17.1 #10972: Strong Winds message only for a Flying-type weakness")
{
    enum Move move;
    bool32 weakened;
    PARAMETRIZE { move = MOVE_DRAGON_PULSE; weakened = FALSE; } // 드래곤 약점(비행과 무관)
    PARAMETRIZE { move = MOVE_ROCK_THROW;   weakened = TRUE; }  // 비행 약점만
    PARAMETRIZE { move = MOVE_ICE_BEAM;     weakened = TRUE; }  // 드래곤·비행 둘 다 약점

    GIVEN {
        ASSUME(GetMoveType(MOVE_DRAGON_PULSE) == TYPE_DRAGON);
        ASSUME(GetMoveType(MOVE_ROCK_THROW) == TYPE_ROCK);
        ASSUME(GetMoveType(MOVE_ICE_BEAM) == TYPE_ICE);
        PLAYER(SPECIES_RAYQUAZA) { Moves(MOVE_DRAGON_ASCENT, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_MEGA); MOVE(opponent, move); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DELTA_STREAM);
        // 난기류 문장은 기술 애니메이션 앞(pre-anim 단계)에 나온다.
        if (weakened)
        {
            MESSAGE("수수께끼의 난기류가 공격을 약하게 만들었다!");
            ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        }
        else
        {
            NOT MESSAGE("수수께끼의 난기류가 공격을 약하게 만들었다!");
            ANIMATION(ANIM_TYPE_MOVE, move, opponent);
            EFFECTIVENESS_SE(player, SE_SUPER_EFFECTIVE);
        }
        HP_BAR(player);
    }
}

// #10974: 화염구슬·맹독구슬은 자기 자신에게 거는 것이라 신비의부적·플라워베일에 막히지 않는다. 화상·독 면역(타입·특성)은 그대로 막는다.
SINGLE_BATTLE_TEST("HnS post-1.17.1 #10974: Flame Orb and Toxic Orb still respect type and ability immunities")
{
    enum Item item;
    enum Species species;
    enum Ability ability;
    PARAMETRIZE { item = ITEM_FLAME_ORB; species = SPECIES_CHARMANDER; ability = ABILITY_BLAZE; }        // 불꽃 타입
    PARAMETRIZE { item = ITEM_FLAME_ORB; species = SPECIES_GOLDEEN;    ability = ABILITY_WATER_VEIL; }   // 수의베일
    PARAMETRIZE { item = ITEM_TOXIC_ORB; species = SPECIES_BULBASAUR;  ability = ABILITY_OVERGROW; }     // 독 타입
    PARAMETRIZE { item = ITEM_TOXIC_ORB; species = SPECIES_SNORLAX;    ability = ABILITY_IMMUNITY; }     // 면역

    GIVEN {
        ASSUME(gItemsInfo[ITEM_FLAME_ORB].holdEffect == HOLD_EFFECT_FLAME_ORB);
        ASSUME(gItemsInfo[ITEM_TOXIC_ORB].holdEffect == HOLD_EFFECT_TOXIC_ORB);
        PLAYER(species) { Ability(ability); Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SAFEGUARD); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SAFEGUARD, player);
        NONE_OF {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, player);
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, player);
        }
    } THEN {
        EXPECT_EQ(player->status1, STATUS1_NONE);
    }
}

// #10878 확인용(HnS는 볼이 볼 바꾸기 때문에 파티 메뉴형 도구라 USE_ITEM에 partyIndex가 필요하다 — upstream 테스트와 같은 장면).
// 1.17.1(이식 전 HnS)은 훔치는 순간 가방에 넣고, 잡으면 잡은 포켓몬의 원래 도구도 되돌려 같은 도구가 둘이 됐다.
// upstream #10878처럼 가방에는 배틀 끝에 넣고, 잡으면 그 포켓몬에게 돌려준다.
WILD_BATTLE_TEST("HnS post-1.17.1 #10878: a wild item stolen with Thief or Covet comes back if the mon is caught")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_THIEF; }
    PARAMETRIZE { move = MOVE_COVET; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_HYPER_POTION); }
    } WHEN {
        TURN { MOVE(player, move); }
        TURN { USE_ITEM(player, ITEM_MASTER_BALL, partyIndex: 0); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ITEM_STEAL, opponent);
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
        EXPECT_EQ(FALSE, CheckBagHasItem(ITEM_HYPER_POTION, 1));
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HELD_ITEM), ITEM_HYPER_POTION);
    }
}

WILD_BATTLE_TEST("HnS post-1.17.1 #10878: a wild item stolen with Thief goes to the Bag if the mon is not caught")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_HYPER_POTION); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THIEF, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ITEM_STEAL, opponent);
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(TRUE, CheckBagHasItem(ITEM_HYPER_POTION, 1));
    }
}

WILD_BATTLE_TEST("HnS post-1.17.1 #10878: an item tricked onto a wild mon is not duplicated if the mon is caught")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TRICK) == EFFECT_TRICK);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_SUPER_POTION); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_HYPER_POTION); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRICK); }
        TURN { USE_ITEM(player, ITEM_MASTER_BALL, partyIndex: 0); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK, player);
    } THEN {
        EXPECT_EQ(FALSE, CheckBagHasItem(ITEM_HYPER_POTION, 1));
        EXPECT_EQ(FALSE, CheckBagHasItem(ITEM_SUPER_POTION, 1));
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM), ITEM_SUPER_POTION);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HELD_ITEM), ITEM_HYPER_POTION);
    }
}
