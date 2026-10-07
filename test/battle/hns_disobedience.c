#include "global.h"
#include "test/battle.h"

// HnS: disobedience self-hit (CancelerObedience DISOBEYS_HITS_SELF) must hurt like a confusion self-hit, and the
// nap / hit itself / loaf roll must follow Emerald's odds (upstream's third roll was always 0).
// Battle tests are recorded link battles, so these rely on the TESTING hooks in GetAttackerObedienceForAction:
// - a player mon with another OT name (OTName) above the badge level can disobey (no badges: level 10)
// - its first two rolls default to 0xFFFF, so it disobeys and does not use another move;
//   WITH_RNG(RNG_HNS_OBEDIENCE, 0) makes it obey for that move
// - the third roll R3 is RNG_HNS_OBEDIENCE_ROLL3 (default 255). With D = level - 10: R3 < D naps if the mon can fall
//   asleep, otherwise R3 < 2 * D hits itself, otherwise it loafs. The self-hit tests set R3 to 0 and use mons that
//   cannot fall asleep (Insomnia or a status), so they hit themselves.
// HnS prints Korean text, so the checks use HP and status instead of English MESSAGE().

SINGLE_BATTLE_TEST("Disobedience: hitting itself deals the same damage as a confusion self-hit", s16 damage)
{
    bool32 disobeys;

    PARAMETRIZE { disobeys = FALSE; }
    PARAMETRIZE { disobeys = TRUE; }
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); Speed(1); if (disobeys) OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, disobeys ? MOVE_CELEBRATE : MOVE_CONFUSE_RAY); MOVE(player, MOVE_SCRATCH, WITH_RNG(disobeys ? RNG_HNS_OBEDIENCE_ROLL3 : RNG_CONFUSION, disobeys ? 0 : TRUE)); }
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
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_HNS_OBEDIENCE_ROLL3, 0)); }
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
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_HNS_OBEDIENCE_ROLL3, 0)); SEND_OUT(player, 1); }
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
        TURN { MOVE(opponent, disobeys ? MOVE_CELEBRATE : MOVE_CONFUSE_RAY); MOVE(player, MOVE_POUND, WITH_RNG(disobeys ? RNG_HNS_OBEDIENCE_ROLL3 : RNG_CONFUSION, disobeys ? 0 : TRUE)); }
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
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_HNS_OBEDIENCE_ROLL3, 0)); }
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

// HnS: the third roll (Emerald: an independent Random() & 255; Bulbapedia "Obedience", Generation III and IV).
// Level 100 with no badges: D = 90, so R3 0-89 naps (or hits itself if the mon cannot fall asleep), 90-179 hits
// itself and 180-255 loafs. The probability tests are not parametrized: a failed THEN leaves runThen set, so the next
// parameter would run THEN before its battle.
#define HNS_DISOBEY_NAPS(battler)  ((battler)->status1 & STATUS1_SLEEP)
#define HNS_DISOBEY_HURT(battler)  ((battler)->hp < (battler)->maxHP)

SINGLE_BATTLE_TEST("Disobedience: a mon that can fall asleep naps with probability D/256")
{
    PASSES_RANDOMLY(90, 256, RNG_HNS_OBEDIENCE_ROLL3);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(HNS_DISOBEY_NAPS(player));
        EXPECT(!HNS_DISOBEY_HURT(player));
    }
}

SINGLE_BATTLE_TEST("Disobedience: a mon that can fall asleep hits itself with probability D/256")
{
    PASSES_RANDOMLY(90, 256, RNG_HNS_OBEDIENCE_ROLL3);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(!HNS_DISOBEY_NAPS(player));
        EXPECT(HNS_DISOBEY_HURT(player));
    }
}

SINGLE_BATTLE_TEST("Disobedience: a mon with Insomnia hits itself with probability 2D/256")
{
    PASSES_RANDOMLY(180, 256, RNG_HNS_OBEDIENCE_ROLL3);
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(!HNS_DISOBEY_NAPS(player));
        EXPECT(HNS_DISOBEY_HURT(player));
    }
}

SINGLE_BATTLE_TEST("Disobedience: a poisoned mon hits itself with probability 2D/256")
{
    PASSES_RANDOMLY(180, 256, RNG_HNS_OBEDIENCE_ROLL3);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Status1(STATUS1_POISON); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT_EQ(player->status1, STATUS1_POISON);
        EXPECT_LT(player->hp, player->maxHP - player->maxHP / 8); // more than the poison damage
    }
}

SINGLE_BATTLE_TEST("Disobedience: a mon that can fall asleep loafs around with probability (256 - 2D)/256")
{
    PASSES_RANDOMLY(76, 256, RNG_HNS_OBEDIENCE_ROLL3);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(!HNS_DISOBEY_NAPS(player));
        EXPECT(!HNS_DISOBEY_HURT(player));
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

SINGLE_BATTLE_TEST("Disobedience: a mon with Insomnia loafs around with probability (256 - 2D)/256")
{
    PASSES_RANDOMLY(76, 256, RNG_HNS_OBEDIENCE_ROLL3);
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(!HNS_DISOBEY_NAPS(player));
        EXPECT(!HNS_DISOBEY_HURT(player));
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}

SINGLE_BATTLE_TEST("Disobedience: the third roll's nap, self-hit and loaf boundaries are D and 2D")
{
    enum { NAPS, HITS_ITSELF, LOAFS } expected;
    u32 roll, ability;

    // Level 30 with no badges: D = 20
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; roll = 0;   expected = NAPS; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; roll = 19;  expected = NAPS; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; roll = 20;  expected = HITS_ITSELF; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; roll = 39;  expected = HITS_ITSELF; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; roll = 40;  expected = LOAFS; }
    PARAMETRIZE { ability = ABILITY_INNER_FOCUS; roll = 255; expected = LOAFS; }
    PARAMETRIZE { ability = ABILITY_INSOMNIA;    roll = 0;   expected = HITS_ITSELF; }
    PARAMETRIZE { ability = ABILITY_INSOMNIA;    roll = 19;  expected = HITS_ITSELF; }
    PARAMETRIZE { ability = ABILITY_INSOMNIA;    roll = 39;  expected = HITS_ITSELF; }
    PARAMETRIZE { ability = ABILITY_INSOMNIA;    roll = 40;  expected = LOAFS; }
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Level(30); Ability(ability); OTName("Test"); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_HNS_OBEDIENCE_ROLL3, roll)); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT_EQ(!!HNS_DISOBEY_NAPS(player), expected == NAPS);
        EXPECT_EQ(HNS_DISOBEY_HURT(player), expected == HITS_ITSELF);
        EXPECT_EQ(opponent->hp, opponent->maxHP);
    }
}
