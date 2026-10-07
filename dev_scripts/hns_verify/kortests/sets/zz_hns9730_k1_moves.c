#include "global.h"
#include "test/battle.h"

// seq 181 #9730 area E Korean stat-change output set K1: stat changes from moves (scratch only, never committed).
// A space in MESSAGE matches exactly one space, newline or prompt; 1-stage messages have a single space since
// hnsfix-1007b X2 (gText_EmptyString3 is "" again, so {B_TXT_IGA} + " " + {B_BUFF2} gives one space).

SINGLE_BATTLE_TEST("HNS9730 K1-01 self +1: Harden")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_HARDEN); }
    } SCENE {
        MESSAGE("마자용은 단단해지기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HARDEN, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-02 self +2: Swords Dance (sharply = 크게 in front)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-03 self +3: Cotton Guard (drastically = 매우 크게 in front)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_COTTON_GUARD); }
    } SCENE {
        MESSAGE("마자용은 코튼가드를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COTTON_GUARD, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 매우 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-04 foe -1: Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-05 foe -2: Screech")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCREECH); }
    } SCENE {
        MESSAGE("마자용은 싫은소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCREECH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-06 foe -4 with Simple: Charm (severely)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SIMPLE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
    } SCENE {
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-07 opponent uses Growl on the player")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_GROWL); }
    } SCENE {
        MESSAGE("상대 마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-08 opponent self +2: Swords Dance")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("상대 마자는 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-09 self at +6: Swords Dance won't go higher")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        MESSAGE("마자용의 공격은 더 올라가지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-10 foe at -6: Growl won't go lower")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 울음소리를 썼다!");
        MESSAGE("상대 마자의 공격은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-11 Dragon Dance: two stats up")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_DANCE); }
    } SCENE {
        MESSAGE("마자용은 용의춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-12 Quiver Dance: three stats up")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_QUIVER_DANCE); }
    } SCENE {
        MESSAGE("마자용은 나비춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_QUIVER_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-13 Shell Smash: two down, three sharply up")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
    } SCENE {
        MESSAGE("마자용은 껍질깨기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
        MESSAGE("마자용의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자용의 특수공격이 크게 올라갔다!");
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-14 Dragon Dance with both stats at +6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
        TURN { MOVE(player, MOVE_AGILITY); }
        TURN { MOVE(player, MOVE_AGILITY); }
        TURN { MOVE(player, MOVE_AGILITY); }
        TURN { MOVE(player, MOVE_DRAGON_DANCE); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용은 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        MESSAGE("마자용은 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        MESSAGE("마자용은 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        MESSAGE("마자용은 용의춤을 썼다!");
        MESSAGE("마자용의 능력은 더 올라가지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-15 Dragon Dance with Attack at +6 only")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
        TURN { MOVE(player, MOVE_DRAGON_DANCE); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용은 용의춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-16 Contrary: Swords Dance lowers")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-17 Contrary: Leaf Storm raises Sp. Atk")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_LEAF_STORM); }
    } SCENE {
        MESSAGE("마자용은 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-18 Contrary foe: Growl raises its Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-19 Contrary at -6: Swords Dance won't go lower")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        MESSAGE("마자용의 공격은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-20 Contrary: Dragon Dance at -6 both")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN { MOVE(player, MOVE_DRAGON_DANCE); }
    } SCENE {
        MESSAGE("마자용은 껍질깨기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("마자용은 껍질깨기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("마자용은 껍질깨기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("마자용은 용의춤을 썼다!");
        MESSAGE("마자용의 능력은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-21 Simple: Harden +2")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SIMPLE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_HARDEN); }
    } SCENE {
        MESSAGE("마자용은 단단해지기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HARDEN, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-22 self-lowering: Close Combat")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CLOSE_COMBAT); }
    } SCENE {
        MESSAGE("마자용은 인파이트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLOSE_COMBAT, player);
        MESSAGE("효과가 별로인 듯하다.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
        MESSAGE("마자용의 특수방어가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-23 self-lowering: Leaf Storm -2")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_LEAF_STORM); }
    } SCENE {
        MESSAGE("마자용은 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-24 self-lowering: Superpower by the foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUPERPOWER); }
    } SCENE {
        MESSAGE("상대 마자는 엄청난힘을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPERPOWER, opponent);
        MESSAGE("효과가 별로인 듯하다.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("상대 마자의 방어가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-25 self-lowering at -6: Leaf Storm x4")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(500); MaxHP(500); }
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEAF_STORM); }
        TURN { MOVE(player, MOVE_LEAF_STORM); }
        TURN { MOVE(player, MOVE_LEAF_STORM); }
        TURN { MOVE(player, MOVE_LEAF_STORM); }
    } SCENE {
        MESSAGE("마자용은 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, player);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-26 secondary 100%: Mud-Slap lowers accuracy")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_MUD_SLAP); }
    } SCENE {
        MESSAGE("마자용은 진흙뿌리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MUD_SLAP, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 명중률이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-27 secondary chance: Bubble Beam lowers Speed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BUBBLE_BEAM, secondaryEffect: TRUE); }
    } SCENE {
        MESSAGE("마자용은 거품광선을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BUBBLE_BEAM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-28 secondary at -6: Rock Tomb when Speed is at -6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCARY_FACE); }
        TURN { MOVE(player, MOVE_SCARY_FACE); }
        TURN { MOVE(player, MOVE_SCARY_FACE); }
        TURN { MOVE(player, MOVE_ROCK_TOMB); }
    } SCENE {
        MESSAGE("마자용은 겁나는얼굴을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("마자용은 겁나는얼굴을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("마자용은 겁나는얼굴을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("마자용은 암석봉인을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_TOMB, player);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-29 self-raising secondary: Flame Charge")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FLAME_CHARGE); }
    } SCENE {
        MESSAGE("마자용은 니트로차지를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLAME_CHARGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-30 all stats up: Ominous Wind secondary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_OMINOUS_WIND, secondaryEffect: TRUE); }
    } SCENE {
        MESSAGE("마자용은 괴상한바람을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_OMINOUS_WIND, player);
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("마자용의 특수공격이 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-31 all stats up: No Retreat")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_NO_RETREAT); }
    } SCENE {
        MESSAGE("마자용은 배수의진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NO_RETREAT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("마자용의 특수공격이 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        MESSAGE("마자용은 배수의 진을 쳐서 도망칠 수 없게 되었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-32 Clangorous Soul")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CLANGOROUS_SOUL); }
    } SCENE {
        MESSAGE("마자용은 소울비트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLANGOROUS_SOUL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("마자용의 특수공격이 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-33 Belly Drum message")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-34 Belly Drum with Contrary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-35 Tickle: two stats down")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_TICKLE); }
    } SCENE {
        MESSAGE("마자용은 간지르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TICKLE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("상대 마자의 방어가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-36 Noble Roar with both stats at -6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_NOBLE_ROAR); }
    } SCENE {
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 괴전파를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EERIE_IMPULSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 괴전파를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EERIE_IMPULSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 괴전파를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EERIE_IMPULSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 부르짖기를 썼다!");
        MESSAGE("상대 마자의 능력은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-37 Memento")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_MEMENTO); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 추억의선물을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEMENTO, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("상대 마자의 특수공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 쓰러졌다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-38 Curse (non-Ghost)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CURSE); }
    } SCENE {
        MESSAGE("마자용은 저주를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CURSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-39 Swagger")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWAGGER); }
    } SCENE {
        MESSAGE("마자용은 뽐내기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWAGGER, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("상대 마자는 혼란에 빠졌다!");
        MESSAGE("상대 마자는 혼란에 빠져 있다!");
        MESSAGE("영문도 모른 채 자신을 공격했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-40 Flatter")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FLATTER); }
    } SCENE {
        MESSAGE("마자용은 부추기기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLATTER, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 올라갔다!");
        MESSAGE("상대 마자는 혼란에 빠졌다!");
        MESSAGE("상대 마자는 혼란에 빠져 있다!");
        MESSAGE("영문도 모른 채 자신을 공격했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-41 Strength Sap")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH_SAP); }
    } SCENE {
        MESSAGE("마자용은 힘흡수를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRENGTH_SAP, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("상대 마자로부터 체력을 흡수했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-42 Strength Sap at -6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_STRENGTH_SAP); }
    } SCENE {
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 힘흡수를 썼다!");
        MESSAGE("상대 마자의 공격은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-43 Fell Stinger KO: Attack drastically")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FELL_STINGER); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 마지막일침을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FELL_STINGER, player);
        MESSAGE("효과가 굉장했다!");
        MESSAGE("상대 마자는 쓰러졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 올라갔다!");
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-44 Defog lowers evasion")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("마자용은 안개제거를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 회피율이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-45 Defog at -6 evasion, nothing to clear")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWEET_SCENT); }
        TURN { MOVE(player, MOVE_SWEET_SCENT); }
        TURN { MOVE(player, MOVE_SWEET_SCENT); }
        TURN { MOVE(player, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("마자용은 달콤한향기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWEET_SCENT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 회피율이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 달콤한향기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWEET_SCENT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 회피율이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 달콤한향기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWEET_SCENT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 회피율이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 안개제거를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-46 Venom Drench on a poisoned foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_VENOM_DRENCH); }
    } SCENE {
        MESSAGE("마자용은 베놈트랩을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VENOM_DRENCH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("상대 마자의 특수공격이 떨어졌다!");
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-47 Toxic Thread")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_TOXIC_THREAD); }
    } SCENE {
        MESSAGE("마자용은 독실을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC_THREAD, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        MESSAGE("상대 마자의 몸에 독이 퍼졌다!");
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-48 Tar Shot")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_TAR_SHOT); }
    } SCENE {
        MESSAGE("마자용은 타르샷을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAR_SHOT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        MESSAGE("상대 마자는 불꽃에 약해졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-49 Autotomize (became nimble)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_AUTOTOMIZE); }
    } SCENE {
        MESSAGE("마자용은 바디퍼지를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AUTOTOMIZE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        MESSAGE("마자용의 몸이 가벼워졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-50 Autotomize by the foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_AUTOTOMIZE); }
    } SCENE {
        MESSAGE("상대 마자는 바디퍼지를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AUTOTOMIZE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        MESSAGE("상대 마자의 몸이 가벼워졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-51 Charge")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARGE); }
    } SCENE {
        MESSAGE("마자용은 충전을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARGE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수방어가 올라갔다!");
        MESSAGE("마자용은 충전을 시작했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-52 Stockpile")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_STOCKPILE); }
    } SCENE {
        MESSAGE("마자용은 비축하기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOCKPILE, player);
        MESSAGE("마자용은 1개 비축했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-53 Minimize and Defense Curl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_MINIMIZE); MOVE(opponent, MOVE_DEFENSE_CURL); }
    } SCENE {
        MESSAGE("마자용은 작아지기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MINIMIZE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 회피율이 크게 올라갔다!");
        MESSAGE("상대 마자는 웅크리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFENSE_CURL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-54 Growth in sun")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); MOVE(player, MOVE_GROWTH); }
    } SCENE {
        MESSAGE("상대 마자는 쾌청을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        MESSAGE("햇살이 강해졌다!");
        MESSAGE("마자용은 성장을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWTH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자용의 특수공격이 크게 올라갔다!");
        MESSAGE("햇살이 강하다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-55 Spicy Extract")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SPICY_EXTRACT); }
    } SCENE {
        MESSAGE("마자용은 하바네로엑기스를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPICY_EXTRACT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-56 Acupressure")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ACUPRESSURE, target: player); }
    } SCENE {
        MESSAGE("마자용은 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-57 Fillet Away")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FILLET_AWAY); }
    } SCENE {
        MESSAGE("마자용은 제살깎기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FILLET_AWAY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자용의 특수공격이 크게 올라갔다!");
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-58 Shift Gear")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHIFT_GEAR); }
    } SCENE {
        MESSAGE("마자용은 기어체인지를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHIFT_GEAR, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 크게 올라갔다!");
        MESSAGE("마자용의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-59 V-create")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_V_CREATE); }
    } SCENE {
        MESSAGE("마자용은 V제너레이트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_V_CREATE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
        MESSAGE("마자용의 특수방어가 떨어졌다!");
        MESSAGE("마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-60 Spin Out (Speed -2 self)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPIN_OUT); }
    } SCENE {
        MESSAGE("마자용은 휠스핀을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPIN_OUT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-61 Psych Up copies foe stat changes")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); MOVE(player, MOVE_PSYCH_UP); }
    } SCENE {
        MESSAGE("상대 마자는 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 자기암시를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYCH_UP, player);
        MESSAGE("마자용은 상대 마자의 능력 변화를 복사했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-62 Mist blocks Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MIST); MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("상대 마자는 흰안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponent);
        MESSAGE("상대는 흰안개에 둘러싸였다!");
        MESSAGE("마자용은 울음소리를 썼다!");
        MESSAGE("상대 마자를 흰안개가 지켜주고 있다");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-63 Substitute blocks Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SCARY_FACE); }
    } SCENE {
        MESSAGE("상대 마자는 대타출동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        MESSAGE("상대 마자의 대타가 나타났다");
        MESSAGE("마자용은 겁나는얼굴을 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-64 Aurora Veil in snow")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SNOWSCAPE); MOVE(player, MOVE_AURORA_VEIL); }
    } SCENE {
        MESSAGE("상대 마자는 설경을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNOWSCAPE, opponent);
        MESSAGE("눈이 내리기 시작했다!");
        MESSAGE("마자용은 오로라베일을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AURORA_VEIL, player);
        MESSAGE("우리 편은 오로라베일로 물리공격과 특수공격에 강해졌다!");
        MESSAGE("눈이 내리고 있다.");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-65 Lash Out after Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(20); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_GROWL); MOVE(player, MOVE_LASH_OUT); }
    } SCENE {
        MESSAGE("상대 마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("마자용은 분풀이를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LASH_OUT, player);
        MESSAGE("효과가 굉장했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-66 Octolock end turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); }
    } SCENE {
        MESSAGE("마자용은 문어굳히기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_OCTOLOCK, player);
        MESSAGE("상대 마자는 문어굳히기 때문에 도망칠 수 없게 되었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-67 Syrup Bomb end turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_SYRUP_BOMB); }
    } SCENE {
        MESSAGE("마자용은 시럽봄을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SYRUP_BOMB, player);
        MESSAGE("상대 마자는 물엿범벅이 되었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-68 Topsy-Turvy and Haze (no stat message)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); MOVE(player, MOVE_TOPSY_TURVY); }
        TURN { MOVE(player, MOVE_HAZE); }
    } SCENE {
        MESSAGE("상대 마자는 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 뒤집어엎기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOPSY_TURVY, player);
        MESSAGE("상대 마자는 능력 변화가 뒤집혔다!");
        MESSAGE("마자용은 흑안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HAZE, player);
        MESSAGE("모든 상태가 원래대로 되돌아왔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-69 Stuff Cheeks")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_STUFF_CHEEKS); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 볼가득넣기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STUFF_CHEEKS, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-70 Power-Up Punch at +6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
        TURN { MOVE(player, MOVE_POWER_UP_PUNCH); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용은 그로우펀치를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POWER_UP_PUNCH, player);
        MESSAGE("효과가 별로인 듯하다.");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-71 Belly Drum at low HP fails")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(40); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-72 Belly Drum at +6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 배북을 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-73 Clangorous Soul at low HP fails")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(300); HP(90); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CLANGOROUS_SOUL); }
    } SCENE {
        MESSAGE("마자용은 소울비트를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-74 clamps: Swords Dance at +5, Charm at -5, Cotton Guard at +4")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); MOVE(opponent, MOVE_COTTON_GUARD); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); MOVE(opponent, MOVE_COTTON_GUARD); }
        TURN { MOVE(player, MOVE_HOWL); MOVE(opponent, MOVE_COTTON_GUARD); }
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_GROWL); }
        TURN { MOVE(player, MOVE_CHARM); }
    } SCENE {
        // (setup events before this point omitted: MAX_QUEUED_EVENTS = 30)
        MESSAGE("상대 마자는 코튼가드를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COTTON_GUARD, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 매우 크게 올라갔다!");
        MESSAGE("마자용은 멀리짖기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HOWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("상대 마자는 코튼가드를 썼다!");
        MESSAGE("상대 마자의 방어는 더 올라가지 않는다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-75 Spit Up drops the Stockpile boosts")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_STOCKPILE); }
        TURN { MOVE(player, MOVE_SPIT_UP); }
    } SCENE {
        MESSAGE("마자용은 비축하기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOCKPILE, player);
        MESSAGE("마자용은 1개 비축했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        MESSAGE("마자용은 토해내기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPIT_UP, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수방어가 떨어졌다!");
        MESSAGE("마자용은 비축해 두었던 효과가 사라졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-76 Contrary: Close Combat and Superpower raise")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CONTRARY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_CLOSE_COMBAT); MOVE(opponent, MOVE_SUPERPOWER); }
    } SCENE {
        MESSAGE("마자용은 인파이트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLOSE_COMBAT, player);
        MESSAGE("효과가 별로인 듯하다.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        MESSAGE("상대 마자는 엄청난힘을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPERPOWER, opponent);
        MESSAGE("효과가 별로인 듯하다.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-77 Strength Sap vs Contrary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH_SAP); }
    } SCENE {
        MESSAGE("마자용은 힘흡수를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRENGTH_SAP, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자로부터 체력을 흡수했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-78 Growth outside sun, Defense Curl at +6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_GROWTH); MOVE(opponent, MOVE_COTTON_GUARD); }
        TURN { MOVE(opponent, MOVE_COTTON_GUARD); }
        TURN { MOVE(opponent, MOVE_DEFENSE_CURL); }
    } SCENE {
        MESSAGE("마자용은 성장을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWTH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 특수공격이 올라갔다!");
        MESSAGE("상대 마자는 코튼가드를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COTTON_GUARD, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 매우 크게 올라갔다!");
        MESSAGE("상대 마자는 코튼가드를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COTTON_GUARD, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 매우 크게 올라갔다!");
        MESSAGE("상대 마자는 웅크리기를 썼다!");
        MESSAGE("상대 마자의 방어는 더 올라가지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-79 Memento vs Clear Body")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CLEAR_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_MEMENTO); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 추억의선물을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEMENTO, player);
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        MESSAGE("상대 마자는 클리어바디의 효과로 능력이 떨어지지 않는다!");
        MESSAGE("마자용은 쓰러졌다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K1-80 Tickle and Mud-Slap through a Substitute")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_TICKLE); }
        TURN { MOVE(player, MOVE_MUD_SLAP); }
    } SCENE {
        MESSAGE("상대 마자는 대타출동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        MESSAGE("상대 마자의 대타가 나타났다");
        MESSAGE("마자용은 간지르기를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
        MESSAGE("마자용은 진흙뿌리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MUD_SLAP, player);
        MESSAGE("상대 마자를 대신하여 대타가 공격을 받았다!");
    }
}
