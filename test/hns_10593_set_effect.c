// HnS seq 490 #10593 회귀 테스트 — SetMoveEffect가 함수 표(src/battle_set_effect.c)로 옮겨진 뒤에도 HnS가 바꾼 줄의 출력이
// 그대로인지(방벽 해제 문장, 파트너 판정 GetPartnerBattler, 점착). 이름이 모두 "HnS 10593:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10593: Brick Break names each screen it breaks")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_BRICK_BREAK) == EFFECT_HIT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_LIGHT_SCREEN); MOVE(player, MOVE_BRICK_BREAK); }
    } SCENE {
        MESSAGE("마자용은 깨뜨리다를 썼다!");
        MESSAGE("상대의 리플렉터가 없어졌다!");
        MESSAGE("상대의 빛의장막이 없어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HnS 10593: Flame Burst hurts the target's partner")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_FLAME_BURST, MOVE_EFFECT_FLAME_BURST));
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FLAME_BURST, target: opponentLeft); }
    } SCENE {
        MESSAGE("마자용은 불꽃튀기기를 썼다!");
        HP_BAR(opponentLeft);
        MESSAGE("분출하는 불꽃이 상대 마자에게 명중했다!");
        HP_BAR(opponentRight);
    }
}

SINGLE_BATTLE_TEST("HnS 10593: Incinerate does not burn a Sticky Hold Pokemon's Berry")
{
    u32 ability;
    PARAMETRIZE { ability = ABILITY_STICKY_HOLD; }
    PARAMETRIZE { ability = ABILITY_LIQUID_OOZE; }
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_INCINERATE, MOVE_EFFECT_INCINERATE));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GULPIN) { Ability(ability); Item(ITEM_ORAN_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_INCINERATE); }
    } SCENE {
        MESSAGE("마자용은 불태우기를 썼다!");
        // 2026-10-30 재확인 47(친구 요청): 도구 이름 뒤 조사 "오랭열매는" → "를", 2026-10-31(친구 결정) 공식 본문 "…이(가) 불타 없어졌다!"(SV 7709)
        if (ability == ABILITY_STICKY_HOLD)
            NOT MESSAGE("상대 꼴깍몬의 오랭열매가 불타 없어졌다!");
        else
            MESSAGE("상대 꼴깍몬의 오랭열매가 불타 없어졌다!");
    } THEN {
        if (ability == ABILITY_STICKY_HOLD)
            EXPECT_EQ(opponent->item, ITEM_ORAN_BERRY);
        else
            EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}
