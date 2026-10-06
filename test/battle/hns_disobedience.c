#include "global.h"
#include "test/battle.h"

// HnS: disobedience self-hit (CancelerObedience DISOBEYS_HITS_SELF) must hurt like a confusion self-hit.
// Battle tests are recorded link battles, so these rely on the TESTING hooks in GetAttackerObedienceForAction:
// - a player mon with another OT name (OTName) above the badge level can disobey (no badges: level 10)
// - its roll defaults to 0xFFFF, so it disobeys; WITH_RNG(RNG_HNS_OBEDIENCE, 0) makes it obey for that move
// - the third roll ((rnd >> 16) & 255) is always 0, so a mon that can fall asleep naps instead of hitting itself;
//   these mons have Insomnia or a status
// HnS prints Korean text, so the checks use HP instead of English MESSAGE().

SINGLE_BATTLE_TEST("Disobedience: hitting itself deals the same damage as a confusion self-hit", s16 damage)
{
    bool32 disobeys;

    PARAMETRIZE { disobeys = FALSE; }
    PARAMETRIZE { disobeys = TRUE; }
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); Speed(1); if (disobeys) OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, disobeys ? MOVE_CELEBRATE : MOVE_CONFUSE_RAY); MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_CONFUSION, TRUE)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
            HP_BAR(opponent);
        }
    } FINALLY {
        EXPECT_GT(results[0].damage, 0);
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Disobedience: Focus Sash leaves 1 HP when the mon hits itself at full HP")
{
    GIVEN {
        ASSUME(GetItemHoldEffect(ITEM_FOCUS_SASH) == HOLD_EFFECT_FOCUS_SASH);
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); MaxHP(10); HP(10); Item(ITEM_FOCUS_SASH); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, hp: 1);
    } THEN {
        EXPECT_EQ(player->hp, 1);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Disobedience: a mon that hits itself at low HP faints")
{
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); HP(1); OTName("Test"); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } SCENE {
        HP_BAR(player, hp: 0);
        NOT HP_BAR(opponent);
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_0][0], MON_DATA_HP), 0);
    }
}

SINGLE_BATTLE_TEST("Disobedience: Magic Guard does not prevent the self-hit, as with confusion", s16 damage)
{
    bool32 disobeys;

    PARAMETRIZE { disobeys = FALSE; }
    PARAMETRIZE { disobeys = TRUE; }
    GIVEN {
        PLAYER(SPECIES_CLEFABLE) { Ability(ABILITY_MAGIC_GUARD); Status1(STATUS1_POISON); Speed(1); if (disobeys) OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, disobeys ? MOVE_CELEBRATE : MOVE_CONFUSE_RAY); MOVE(player, MOVE_POUND, WITH_RNG(RNG_CONFUSION, TRUE)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
            HP_BAR(opponent);
        }
    } FINALLY {
        EXPECT_GT(results[0].damage, 0);
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Disobedience: the self-hit goes past the user's Substitute")
{
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE, WITH_RNG(RNG_HNS_OBEDIENCE, 0)); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        HP_BAR(player);
        HP_BAR(player);
        NONE_OF {
            SUB_HIT(player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
            HP_BAR(opponent);
        }
    } THEN {
        EXPECT(player->volatiles.substituteHP > 0);
        EXPECT_LT(player->hp, player->maxHP - player->maxHP / 4);
    }
}

SINGLE_BATTLE_TEST("Disobedience: a mon from another trainer obeys with the eighth badge")
{
    GIVEN {
        FLAG_SET(FLAG_BADGE08_GET);
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent);
        NOT HP_BAR(player);
    }
}
