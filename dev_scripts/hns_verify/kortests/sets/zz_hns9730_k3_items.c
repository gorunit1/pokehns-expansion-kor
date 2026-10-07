#include "global.h"
#include "test/battle.h"

// seq 181 #9730 area E Korean stat-change output set K3: held items (scratch only, never committed).
// Item pop-ups are not visible to SCENE before upstream #10321; compare them in the decoded trace (TR:I lines).
// Battles end with an idle TURN {} so a pop-up is never the last event ("task not freed").

SINGLE_BATTLE_TEST("HNS9730 K3-01 White Herb after Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-02 White Herb after the user's Close Combat")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CLOSE_COMBAT); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 인파이트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLOSE_COMBAT, player);
        MESSAGE("효과가 별로인 듯하다.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
        MESSAGE("마자용의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-03 White Herb after Shell Smash")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
        TURN {}
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
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-04 Mirror Herb copies the foe's Swords Dance")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_MIRROR_HERB); }
    } WHEN {
        TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-05 Mirror Herb copies Dragon Dance")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_DANCE); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 용의춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자의 스피드가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-06 Liechi Berry at low HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIECHI_BERRY); MaxHP(100); HP(20); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 치리열매로 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-07 Weakness Policy")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_VINE_WHIP) == TYPE_GRASS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Item(ITEM_WEAKNESS_POLICY); }
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 덩굴채찍을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 꼬부기는 약점보험으로 공격이 크게 올라갔다!");
        MESSAGE("상대 꼬부기는 약점보험으로 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-08 Kee Berry on a physical hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_KEE_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 악키열매로 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-09 Throat Spray")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_THROAT_SPRAY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 목스프레이로 특수공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-10 Blunder Policy")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_BLUNDER_POLICY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE, hit: FALSE); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 스톤샤워를 썼다!");
        MESSAGE("상대 마자에게는 맞지 않았다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 허탕보험으로 스피드가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-11 Cell Battery")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_CELL_BATTERY); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 전기쇼크를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER_SHOCK, player);
        MESSAGE("상대 마자는 마비되어 기술이 나오기 어려워졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 충전지로 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-12 Electric Seed on Electric Terrain")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ELECTRIC_SEED); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 일렉트릭필드를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ELECTRIC_TERRAIN, opponent);
        MESSAGE("발밑에 전기가 흐르기 시작했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 일렉트릭시드로 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-13 Room Service on Trick Room")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_ROOM_SERVICE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TRICK_ROOM); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 트릭룸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK_ROOM, opponent);
        MESSAGE("상대 마자는 시공을 뒤틀었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 룸서비스로 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-14 Adrenaline Orb at max Speed does not activate")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { SWITCH(player, 1); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        MESSAGE("상대 마자는 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        MESSAGE("상대 마자는 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        MESSAGE("마자용 이제 됐어! 돌아와!");
        MESSAGE("가랏! 마자!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-15 Clear Amulet vs Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_CLEAR_AMULET); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        MESSAGE("상대 마자는 클리어참의 효과로 능력이 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-16 Maranga Berry on a special hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_MARANGA_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 물대포를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 타라프열매로 특수방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-17 Snowball")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_SNOWBALL); }
    } WHEN {
        TURN { MOVE(player, MOVE_POWDER_SNOW); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 눈싸라기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POWDER_SNOW, player);
        MESSAGE("상대 마자는 얼어붙었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 눈덩이로 공격이 올라갔다!");
        MESSAGE("상대 마자의 얼음이 녹았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-18 Weakness Policy holder with Contrary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Item(ITEM_WEAKNESS_POLICY); Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 덩굴채찍을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 꼬부기의 공격이 매우 크게 떨어졌다!");
        MESSAGE("상대 꼬부기의 특수공격이 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-19 Liechi Berry with Ripen")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIECHI_BERRY); Ability(ABILITY_RIPEN); MaxHP(100); HP(20); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_RIPEN);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 치리열매로 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-20 Mirror Herb holder at +6 copies nothing new")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MIRROR_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_GROWL); }
        TURN { MOVE(opponent, MOVE_HOWL); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("상대 마자는 멀리짖기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HOWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 흉내허브를 써서 상대의 능력 변화를 흉내 냈다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-21 X Attack from the bag")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { USE_ITEM(player, ITEM_X_ATTACK); }
    } SCENE {
        MESSAGE("1는 플러스파워를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-22 X Attack at +6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BELLY_DRUM); }
        TURN { USE_ITEM(player, ITEM_X_ATTACK); }
    } SCENE {
        MESSAGE("마자용은 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("1는 플러스파워를 썼다!");
        MESSAGE("마자용의 공격은 더 올라가지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-23 Dire Hit from the bag")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { USE_ITEM(player, ITEM_DIRE_HIT); }
    } SCENE {
        MESSAGE("1는 크리티컬커터를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FOCUS_ENERGY, player);
        MESSAGE("마자용은 크리티컬커터를 써서 의욕이 넘치기 시작했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-24 Weakness Policy at +6 Attack and Sp. Atk")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SQUIRTLE) { Item(ITEM_WEAKNESS_POLICY); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { MOVE(opponent, MOVE_TAIL_GLOW); }
        TURN { MOVE(opponent, MOVE_TAIL_GLOW); }
        TURN { MOVE(player, MOVE_VINE_WHIP); }
        TURN {}
    } SCENE {
        MESSAGE("상대 꼬부기는 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 꼬부기는 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("상대 꼬부기는 반딧불을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAIL_GLOW, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 꼬부기의 특수공격이 매우 크게 올라갔다!");
        MESSAGE("상대 꼬부기는 반딧불을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAIL_GLOW, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 꼬부기의 특수공격이 매우 크게 올라갔다!");
        MESSAGE("마자용은 덩굴채찍을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        MESSAGE("효과가 굉장했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K3-25 Adrenaline Orb with Contrary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
    }
}
