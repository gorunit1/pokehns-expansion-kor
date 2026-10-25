// HnS seq 470 #9939 회귀 테스트(빗나감은 명중률 100 미만에서만 hit: FALSE로 고정되므로 반짝가루를 들린다) — 데미지 기술의 명중 판정이 공격 취소 단계(CancelerAccuracyCheck)로 옮겨진 뒤
// 한글 빗나감 문장과 이름(D1: 모든 빗나감은 "…에게는 맞지 않았다!"). upstream 테스트는 영어 MESSAGE라 HnS에서 판정되지 않는다.
// 이름이 모두 "HnS 9939:"로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 9939: a damaging move that misses names the target")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_BRIGHTPOWDER); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, hit: FALSE); }
    } SCENE {
        MESSAGE("마자용은 몸통박치기를 썼다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

SINGLE_BATTLE_TEST("HnS 9939: a status move that misses names the target")
{
    GIVEN {
        ASSUME(GetMovePower(MOVE_HYPNOSIS) == 0);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_HYPNOSIS, hit: FALSE); }
    } SCENE {
        MESSAGE("마자용은 최면술을 썼다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPNOSIS, player);
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

// Each Pokémon that avoids the spread move is named, in the order ally, left foe, right foe.
DOUBLE_BATTLE_TEST("HnS 9939: a spread move that misses everyone names each target in order")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); Item(ITEM_BRIGHTPOWDER); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_BRIGHTPOWDER); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); Item(ITEM_BRIGHTPOWDER); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE, hit: FALSE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerLeft);
        MESSAGE("마자에게는 맞지 않았다!");
        MESSAGE("상대 마자용에게는 맞지 않았다!");
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

// Sturdy against a one-hit KO move: pop-up and the "doesn't affect" message naming the holder.
SINGLE_BATTLE_TEST("HnS 9939: Sturdy stops a one-hit KO move")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FISSURE) == EFFECT_OHKO);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_STURDY); }
    } WHEN {
        TURN { MOVE(player, MOVE_FISSURE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        ABILITY_POPUP(opponent, ABILITY_STURDY);
        MESSAGE("상대 마자에게는 효과가 없는 것 같다...");
    } THEN {
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

// Leech Seed on a target that is already seeded uses the same miss message (the old "…은(는) 공격을 피했다!" is gone).
SINGLE_BATTLE_TEST("HnS 9939: Leech Seed on an already seeded target uses the miss message")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEECH_SEED, player);
        MESSAGE("마자용은 씨뿌리기를 썼다!");
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

// Stomping Tantrum doubles after the previous one missed.
SINGLE_BATTLE_TEST("HnS 9939: Stomping Tantrum doubles after a miss")
{
    s16 damage[2];
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_BRIGHTPOWDER); }
    } WHEN {
        TURN { MOVE(player, MOVE_STOMPING_TANTRUM); }
        TURN { MOVE(player, MOVE_STOMPING_TANTRUM, hit: FALSE); }
        TURN { MOVE(player, MOVE_STOMPING_TANTRUM); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &damage[0]);
        MESSAGE("상대 마자에게는 맞지 않았다!");
        HP_BAR(opponent, captureDamage: &damage[1]);
    } THEN {
        EXPECT_MUL_EQ(damage[0], Q_4_12(2.0), damage[1]);
    }
}
