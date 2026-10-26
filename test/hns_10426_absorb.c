// HnS seq 484 #10426 회귀 테스트 — 흡수 기술이 MOVE_EFFECT_ABSORB 추가 효과로, 흡수·씨뿌리기·해감액 문장의 이름 토큰이
// B_DEF/B_ATK → B_SCR/B_EFF로 바뀐 뒤의 한글 출력. 이름이 모두 "HnS 10426:"으로 시작한다.
#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("HnS 10426: Absorb drains with the target's name")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_ABSORB, MOVE_EFFECT_ABSORB));
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ABSORB); }
    } SCENE {
        MESSAGE("마자용은 흡수를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ABSORB, player);
        HP_BAR(opponent);
        HP_BAR(player);
        MESSAGE("상대 마자로부터 체력을 흡수했다!");
    }
}

// The particle after a name ending in a final consonant: 마자용 → 으로부터.
SINGLE_BATTLE_TEST("HnS 10426: Giga Drain from the player's Pokemon picks the right particle")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_GIGA_DRAIN, MOVE_EFFECT_ABSORB));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_GIGA_DRAIN); }
    } SCENE {
        MESSAGE("상대 마자는 기가드레인을 썼다!");
        HP_BAR(player);
        HP_BAR(opponent);
        MESSAGE("마자용으로부터 체력을 흡수했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10426: Absorb against Liquid Ooze hurts the user with its line")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_ABSORB); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
        HP_BAR(player);
        MESSAGE("마자용은 해감액을 흡수했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10426: no absorb line at full HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ABSORB); }
    } SCENE {
        HP_BAR(opponent);
        NONE_OF {
            HP_BAR(player);
            MESSAGE("상대 마자로부터 체력을 흡수했다!");
        }
    }
}

SINGLE_BATTLE_TEST("HnS 10426: Dream Eater drains a sleeping target with its name")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DREAM_EATER) == EFFECT_DREAM_EATER);
        ASSUME(MoveHasAdditionalEffect(MOVE_DREAM_EATER, MOVE_EFFECT_ABSORB));
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP); }
    } WHEN {
        TURN { MOVE(player, MOVE_DREAM_EATER); }
    } SCENE {
        MESSAGE("마자용은 꿈먹기를 썼다!");
        HP_BAR(opponent);
        HP_BAR(player);
        MESSAGE("상대 마자로부터 체력을 흡수했다!");
    }
}

// HnS keeps Leech Life at 60 power; #10426 only moves its drain into the additional effect.
SINGLE_BATTLE_TEST("HnS 10426: Leech Life keeps 60 power and drains half")
{
    s16 damage;
    s16 healed;
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_LEECH_LIFE) == EFFECT_HIT);
        ASSUME(MoveHasAdditionalEffect(MOVE_LEECH_LIFE, MOVE_EFFECT_ABSORB));
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_LIFE); }
    } SCENE {
        MESSAGE("마자용은 흡혈을 썼다!");
        HP_BAR(opponent, captureDamage: &damage);
        HP_BAR(player, captureDamage: &healed);
        MESSAGE("상대 마자용으로부터 체력을 흡수했다!");
    } THEN {
        EXPECT_EQ(GetMovePower(MOVE_LEECH_LIFE), 60);
        EXPECT_MUL_EQ(damage, Q_4_12(-0.5), healed);
    }
}

// Big Root: 50% drain × 1.3.
SINGLE_BATTLE_TEST("HnS 10426: Big Root raises the drained HP")
{
    s16 damage;
    s16 healed;
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_BIG_ROOT) == HOLD_EFFECT_BIG_ROOT);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Item(ITEM_BIG_ROOT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_GIGA_DRAIN); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &damage);
        HP_BAR(player, captureDamage: &healed);
        MESSAGE("상대 마자용으로부터 체력을 흡수했다!");
    } THEN {
        EXPECT_MUL_EQ(damage, Q_4_12(-0.65), healed);
    }
}

// A spread drain move names each target in its own line.
DOUBLE_BATTLE_TEST("HnS 10426: a spread drain move names each target")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_PARABOLIC_CHARGE, MOVE_EFFECT_ABSORB));
        ASSUME(GetMoveTarget(MOVE_PARABOLIC_CHARGE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_PARABOLIC_CHARGE); }
    } SCENE {
        HP_BAR(opponentLeft);
        HP_BAR(playerRight);
        HP_BAR(opponentRight);
        MESSAGE("상대 마자용으로부터 체력을 흡수했다!");
        MESSAGE("마자용으로부터 체력을 흡수했다!");
        MESSAGE("상대 마자로부터 체력을 흡수했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10426: Leech Seed names the drained Pokemon on both sides")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { HP(100); Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); MOVE(opponent, MOVE_LEECH_SEED); }
    } SCENE {
        MESSAGE("상대 마자에게 씨앗을 심었다!");
        MESSAGE("마자용에게 씨앗을 심었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_LEECH_SEED_DRAIN, player);
        MESSAGE("씨뿌리기가 마자용의 체력을 빼앗는다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_LEECH_SEED_DRAIN, opponent);
        MESSAGE("씨뿌리기가 상대 마자의 체력을 빼앗는다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10426: Leech Seed on a Liquid Ooze Pokemon names the seeder")
{
    GIVEN {
        PLAYER(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_LEECH_SEED); }
    } SCENE {
        MESSAGE("왕눈해에게 씨앗을 심었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_LEECH_SEED_DRAIN, player);
        HP_BAR(player);
        ABILITY_POPUP(player, ABILITY_LIQUID_OOZE);
        HP_BAR(opponent);
        MESSAGE("상대 마자는 해감액을 흡수했다!");
    }
}

SINGLE_BATTLE_TEST("HnS 10426: Strength Sap names the target")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_STRENGTH_SAP) == EFFECT_STRENGTH_SAP);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_STRENGTH_SAP); }
    } SCENE {
        MESSAGE("상대 마자는 힘흡수를 썼다!");
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("마자용으로부터 체력을 흡수했다!");
    }
}

// Clear Body stops the Attack drop, so no stat change sets the scripting battler before the drain line.
SINGLE_BATTLE_TEST("HnS 10426: Strength Sap against Clear Body still names the target")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_STRENGTH_SAP) == EFFECT_STRENGTH_SAP);
        PLAYER(SPECIES_BELDUM) { Ability(ABILITY_CLEAR_BODY); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_STRENGTH_SAP); }
    } SCENE {
        MESSAGE("상대 마자는 힘흡수를 썼다!");
        ABILITY_POPUP(player, ABILITY_CLEAR_BODY);
        HP_BAR(opponent);
        MESSAGE("메탕으로부터 체력을 흡수했다!");
    }
}

// The seeder's Magic Guard blocks the Liquid Ooze damage (upstream #10426 left gEffectBattler unset here;
// HnS takes the line upstream master added in merge 5340d262a4).
SINGLE_BATTLE_TEST("HnS 10426: Magic Guard blocks Liquid Ooze from Leech Seed")
{
    GIVEN {
        PLAYER(SPECIES_CLEFFA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } SCENE {
        MESSAGE("상대 왕눈해에게 씨앗을 심었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_LEECH_SEED_DRAIN, opponent);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
        NONE_OF {
            HP_BAR(player);
            MESSAGE("삐는 해감액을 흡수했다!");
        }
    } THEN {
        EXPECT_EQ(player->hp, player->maxHP);
    }
}

// Matcha Gotcha: the drain now comes from the additional effect list, ahead of the burn (#10426 —
// before, the drain ran at move end after the burn line).
SINGLE_BATTLE_TEST("HnS 10426: Matcha Gotcha drains before its burn")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_MATCHA_GOTCHA, MOVE_EFFECT_ABSORB));
        ASSUME(MoveHasAdditionalEffect(MOVE_MATCHA_GOTCHA, MOVE_EFFECT_BURN));
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_MATCHA_GOTCHA, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); }
    } SCENE {
        MESSAGE("마자용은 휘적휘적포를 썼다!");
        HP_BAR(opponent);
        HP_BAR(player);
        MESSAGE("상대 마자용으로부터 체력을 흡수했다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
        MESSAGE("상대 마자용은 화상을 입었다!");
    }
}
