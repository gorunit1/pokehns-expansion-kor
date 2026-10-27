// HnS seq 394 #10268 회귀 테스트 — 열매 도구 팝업(애슈·미클·자보·애터·랑사열매)이 지닌 포켓몬 위에 뜨는지, 방어를 다 막지
// 못한 문장의 대상 이름(B_SCR → B_DEF). 팝업 대기(waitabilitypopup·destroyitempopup)는 헤드리스 테스트에서 팝업 스프라이트를
// 만들지 않으므로 여기서 재지 않는다. 이름이 모두 "HnS 10268:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10268: the Custap Berry pop-up shows on its holder")
{
    u32 holderIsPlayer;
    PARAMETRIZE { holderIsPlayer = TRUE; }
    PARAMETRIZE { holderIsPlayer = FALSE; }
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_CUSTAP_BERRY) == HOLD_EFFECT_CUSTAP_BERRY);
        if (holderIsPlayer) {
            PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_CUSTAP_BERRY); HP(1); Speed(1); }
            OPPONENT(SPECIES_WYNAUT) { Speed(20); }
        } else {
            PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
            OPPONENT(SPECIES_WYNAUT) { Item(ITEM_CUSTAP_BERRY); HP(1); Speed(1); }
        }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        if (holderIsPlayer) {
            ITEM_POPUP(player, ITEM_CUSTAP_BERRY);
            MESSAGE("마자용은 애슈열매로 행동이 빨라졌다!");
            MESSAGE("마자용은 축하를 썼다!");
        } else {
            ITEM_POPUP(opponent, ITEM_CUSTAP_BERRY);
            MESSAGE("상대 마자는 애슈열매로 행동이 빨라졌다!");
            MESSAGE("상대 마자는 축하를 썼다!");
        }
    }
}

SINGLE_BATTLE_TEST("HnS 10268: the Micle Berry pop-up shows on its holder")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_MICLE_BERRY) == HOLD_EFFECT_MICLE_BERRY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_MICLE_BERRY); MaxHP(400); HP(101); Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        HP_BAR(opponent);
        ITEM_POPUP(opponent, ITEM_MICLE_BERRY);
        MESSAGE("상대 마자는 미클열매로 다음에 쓸 기술이 명중하기 쉬워졌다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10268: the Jaboca and Rowap Berry pop-ups show on their holder")
{
    enum Item item;
    enum Move move;
    PARAMETRIZE { item = ITEM_JABOCA_BERRY; move = MOVE_SCRATCH; }
    PARAMETRIZE { item = ITEM_ROWAP_BERRY; move = MOVE_WATER_GUN; }
    GIVEN {
        ASSUME(GetMoveCategory(MOVE_SCRATCH) == DAMAGE_CATEGORY_PHYSICAL);
        ASSUME(GetMoveCategory(MOVE_WATER_GUN) == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Item(item); Speed(1); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        HP_BAR(opponent);
        ITEM_POPUP(opponent, item);
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("HnS 10268: the Lansat Berry pop-up shows on its holder")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_LANSAT_BERRY) == HOLD_EFFECT_CRITICAL_UP);
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_LANSAT_BERRY); MaxHP(400); HP(101); Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        HP_BAR(opponent);
        ITEM_POPUP(opponent, ITEM_LANSAT_BERRY);
        MESSAGE("상대 마자는 랑사열매를 써서 의욕이 넘치기 시작했다!");
    }
}

// The "couldn't fully protect" line names the hit target through {B_DEF} (#10268), here the right-hand foe.
DOUBLE_BATTLE_TEST("HnS 10268: Unseen Fist through Protect names the target in a double battle")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_URSHIFU) { Ability(ABILITY_UNSEEN_FIST); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_PROTECT); MOVE(playerLeft, MOVE_SCRATCH, target: opponentRight); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        HP_BAR(opponentRight);
        ABILITY_POPUP(playerLeft, ABILITY_UNSEEN_FIST);
        MESSAGE("상대 마자는 공격을 막아 내지 못하고 데미지를 입었다!");
    }
}
