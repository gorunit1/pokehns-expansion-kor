// HnS seq 475 #9777 회귀 테스트 — Pokémon Champions 배틀 메시지 이식 뒤 한글 출력.
// 도구 회복 문장 삭제(친구 답 Q3~Q5), 유폭·까칠한피부 문장 삭제, 방어 우회 문장 위치, 능력 상승 열매 팝업,
// 주눅구슬 문장 유지(친구 결정 7), 연속기 뒤 타격 빗나감. upstream 테스트는 영어 MESSAGE라 HnS에서 판정되지 않는다.
// 이름이 모두 "HnS 9777:"로 시작한다.
#include "global.h"
#include "test/battle.h"

// Q3: Leftovers shows its pop-up and the heal animation, without the old "restored a little" message.
SINGLE_BATTLE_TEST("HnS 9777: Leftovers heals with its pop-up and no message")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_LEFTOVERS) == HOLD_EFFECT_LEFTOVERS);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LEFTOVERS); HP(50); MaxHP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { }
    } SCENE {
        ITEM_POPUP(player, ITEM_LEFTOVERS);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player);
        NOT MESSAGE("마자용은 먹다남은음식으로 인해 조금 회복했다.");
    }
}

// Q5: Sitrus Berry: berry animation, heal animation and HP bar, without "restored its health".
SINGLE_BATTLE_TEST("HnS 9777: Sitrus Berry heals with its pop-up and no message")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_SITRUS_BERRY) == HOLD_EFFECT_RESTORE_PCT_HP);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponent);
        ITEM_POPUP(player, ITEM_SITRUS_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        HP_BAR(player);
        NOT MESSAGE("마자용은 자뭉열매로 체력을 회복했다!");
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

// Aftermath: pop-up and HP bar, without "…은(는) 상처를 입었다!".
SINGLE_BATTLE_TEST("HnS 9777: Aftermath hurts the attacker without a message")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_TACKLE));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_AFTERMATH); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        ABILITY_POPUP(opponent, ABILITY_AFTERMATH);
        HP_BAR(player);
        NOT MESSAGE("마자용은 상처를 입었다!");
    }
}

// Rough Skin: pop-up and HP bar, without "…은(는) 상처를 입었다!".
SINGLE_BATTLE_TEST("HnS 9777: Rough Skin hurts the attacker without a message")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_TACKLE));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player);
        NOT MESSAGE("마자용은 상처를 입었다!");
    }
}

// Spiky Shield keeps its message (the string is only renamed STRINGID_PKMNWASHURT).
SINGLE_BATTLE_TEST("HnS 9777: Spiky Shield still says the attacker was hurt")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_TACKLE));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPIKY_SHIELD); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPIKY_SHIELD, opponent);
        HP_BAR(player);
        MESSAGE("마자용은 상처를 입었다!");
    }
}

// Unseen Fist through Protect: the hit, then the pop-up and the "couldn't fully protect" line naming the target.
SINGLE_BATTLE_TEST("HnS 9777: Unseen Fist through Protect shows its pop-up and names the target")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_TACKLE));
        PLAYER(SPECIES_URSHIFU) { Ability(ABILITY_UNSEEN_FIST); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_PROTECT); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
        ABILITY_POPUP(player, ABILITY_UNSEEN_FIST);
        MESSAGE("상대 마자는 공격을 막아 내지 못하고 데미지를 입었다!");
    }
}

// Unseen Fist with a non-contact move does not get through Protect, so no pop-up or message.
SINGLE_BATTLE_TEST("HnS 9777: Unseen Fist with a non-contact move shows nothing against Protect")
{
    GIVEN {
        ASSUME(!MoveMakesContact(MOVE_WATER_GUN));
        PLAYER(SPECIES_URSHIFU) { Ability(ABILITY_UNSEEN_FIST); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_PROTECT); MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
            ABILITY_POPUP(player, ABILITY_UNSEEN_FIST);
            MESSAGE("상대 마자는 공격을 막아 내지 못하고 데미지를 입었다!");
        }
    }
}

// A stat berry now shows the item pop-up before its animation (friend reply 2026-10-05, Q6 plan A).
SINGLE_BATTLE_TEST("HnS 9777: a stat-raising berry shows the item pop-up")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_LIECHI_BERRY) == HOLD_EFFECT_ATTACK_UP);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIECHI_BERRY); HP(40); MaxHP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPER_FANG); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, opponent);
        ITEM_POPUP(player, ITEM_LIECHI_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        MESSAGE("마자용은 치리열매로 공격이 올라갔다!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
    }
}

// Friend decision 7 kept: the Adrenaline Orb message still names the item (the #9777 pop-up comes first).
SINGLE_BATTLE_TEST("HnS 9777: Adrenaline Orb keeps the item name in its message")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_ADRENALINE_ORB) == HOLD_EFFECT_ADRENALINE_ORB);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ADRENALINE_ORB); }
        OPPONENT(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        ITEM_POPUP(player, ITEM_ADRENALINE_ORB);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 주눅구슬로 스피드가 올라갔다!");
    }
}

// Grassy Terrain: the heal animation plays before the heal message.
SINGLE_BATTLE_TEST("HnS 9777: Grassy Terrain heal plays the heal animation")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(50); MaxHP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_TERRAIN, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, player);
        MESSAGE("마자용의 체력이 회복되었다!");
    }
}

// Korean versions of the upstream test/battle/effectiveness.c (English MESSAGE there): the four effectiveness levels.
SINGLE_BATTLE_TEST("HnS 9777: effectiveness messages in a single battle")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_POUND; }
    PARAMETRIZE { move = MOVE_ROCK_THROW; }
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    PARAMETRIZE { move = MOVE_KARATE_CHOP; }
    GIVEN {
        ASSUME(GetMoveType(MOVE_POUND) == TYPE_NORMAL);
        ASSUME(GetMoveType(MOVE_ROCK_THROW) == TYPE_ROCK);
        ASSUME(GetMoveType(MOVE_WATER_GUN) == TYPE_WATER);
        ASSUME(GetMoveType(MOVE_KARATE_CHOP) == TYPE_FIGHTING);
        PLAYER(SPECIES_AGGRON);
        OPPONENT(SPECIES_AGGRON);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        switch (move)
        {
        case MOVE_POUND:
            MESSAGE("효과가 매우 별로인 듯하다.");
            break;
        case MOVE_ROCK_THROW:
            MESSAGE("효과가 별로인 듯하다.");
            break;
        case MOVE_WATER_GUN:
            MESSAGE("효과가 굉장했다!");
            break;
        default:
            MESSAGE("효과가 매우 굉장했다!!");
            break;
        }
    }
}

// Spread moves that hit both foes the same way: one line naming both.
DOUBLE_BATTLE_TEST("HnS 9777: effectiveness messages for both foes in a double battle")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_BOOMBURST; }
    PARAMETRIZE { move = MOVE_SYNCHRONOISE; }
    PARAMETRIZE { move = MOVE_SURF; }
    PARAMETRIZE { move = MOVE_EARTHQUAKE; }
    GIVEN {
        ASSUME(GetMoveTarget(move) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_AGGRON) { Attack(1); SpAttack(1); }
        PLAYER(SPECIES_AGGRON) { Attack(1); SpAttack(1); }
        OPPONENT(SPECIES_AGGRON) { Attack(1); SpAttack(1); }
        OPPONENT(SPECIES_AGGRON) { Attack(1); SpAttack(1); }
    } WHEN {
        TURN { MOVE(playerLeft, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, playerLeft);
        switch (move)
        {
        case MOVE_BOOMBURST:
            MESSAGE("상대 보스로라와 보스로라에게 효과가 매우 별로였다.");
            break;
        case MOVE_SYNCHRONOISE:
            MESSAGE("상대 보스로라와 보스로라에게 효과가 별로였다.");
            break;
        case MOVE_SURF:
            MESSAGE("상대 보스로라와 보스로라에게 효과가 굉장했다!");
            break;
        default:
            MESSAGE("상대 보스로라와 보스로라에게 효과가 매우 굉장했다!!");
            break;
        }
    }
}

// Critical hits from a spread move name each target; a single-target move keeps the plain line
// (Korean version of the upstream "Critical hits mention each affected target").
DOUBLE_BATTLE_TEST("HnS 9777: critical hits from a spread move name each target")
{
    GIVEN {
        WITH_CONFIG(B_CRIT_CHANCE, GEN_9);
        ASSUME(GetSpeciesBaseSpeed(SPECIES_WOBBUFFET) == 33);
        PLAYER(SPECIES_DRAGONITE) { Item(ITEM_SCOPE_LENS); SpAttack(1); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_DRAGON_CHEER); MOVE(playerLeft, MOVE_SURF); }
        TURN { MOVE(playerLeft, MOVE_SURF); }
        TURN { MOVE(playerLeft, MOVE_DAZZLING_GLEAM); }
    } SCENE {
        MESSAGE("상대 마자의 급소에 맞았다!");
        MESSAGE("마자용의 급소에 맞았다!");
        MESSAGE("상대 마자용의 급소에 맞았다!");
        MESSAGE("마자용의 급소에 맞았다!");
        MESSAGE("상대 마자용의 급소에 맞았다!");
        MESSAGE("급소에 맞았다!");
    }
}
