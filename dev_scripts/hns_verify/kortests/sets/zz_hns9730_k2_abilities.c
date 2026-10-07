#include "global.h"
#include "test/battle.h"

// seq 181 #9730 area E Korean stat-change output set K2: abilities (scratch only, never committed).

SINGLE_BATTLE_TEST("HNS9730 K2-01 Intimidate lowers the foe's Attack (pre: single space, PKMNCUTSATTACKWITH)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-02 foe Intimidate lowers the player's Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-03 Intimidate in doubles hits both foes")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-04 Intimidate vs Clear Body")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CLEAR_BODY); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        MESSAGE("상대 마자는 클리어바디의 효과로 능력이 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-05 Intimidate vs Hyper Cutter")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_HYPER_CUTTER); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_HYPER_CUTTER);
        MESSAGE("상대 마자의 공격은 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-06 Intimidate vs Inner Focus")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_INNER_FOCUS); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_INNER_FOCUS);
        MESSAGE("상대 마자의 공격은 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-07 Intimidate vs Oblivious")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_OBLIVIOUS); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_OBLIVIOUS);
        MESSAGE("상대 마자의 공격은 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-08 Intimidate vs Guard Dog")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_GUARD_DOG); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_GUARD_DOG);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-09 Intimidate vs Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-10 Intimidate vs Rattled")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_RATTLED); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_RATTLED);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-11 Intimidate vs Defiant")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DEFIANT); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_DEFIANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-12 Intimidate vs Competitive")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_COMPETITIVE); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_COMPETITIVE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-13 Intimidate vs Contrary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-14 Intimidate vs Attack already at -6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
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
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("마자용 이제 됐어! 돌아와!");
        MESSAGE("가랏! 마자!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("상대 마자의 공격은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-15 Intimidate vs Clear Amulet")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_CLEAR_AMULET); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("상대 마자는 클리어참의 효과로 능력이 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-16 Intimidate vs Adrenaline Orb")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_ADRENALINE_ORB); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 주눅구슬로 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-17 Intimidate vs Mist")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MIST); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 흰안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponent);
        MESSAGE("상대는 흰안개에 둘러싸였다!");
        MESSAGE("마자용 이제 됐어! 돌아와!");
        MESSAGE("가랏! 마자!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("상대 마자를 흰안개가 지켜주고 있다");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-18 Intimidate vs White Herb")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_WHITE_HERB); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 하양허브로 상태를 원래대로 되돌렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-19 Intimidate vs Defiant and White Herb")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DEFIANT); Item(ITEM_WHITE_HERB); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_DEFIANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-20 Growl vs Clear Body")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CLEAR_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        MESSAGE("상대 마자는 클리어바디의 효과로 능력이 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-21 Growl vs Hyper Cutter")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_HYPER_CUTTER); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_HYPER_CUTTER);
        MESSAGE("상대 마자의 공격은 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-22 Leer vs Big Pecks, Sand Attack vs Keen Eye")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_BIG_PECKS); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_KEEN_EYE); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEER); }
        TURN { SWITCH(opponent, 1); }
        TURN { MOVE(player, MOVE_SAND_ATTACK); }
    } SCENE {
        MESSAGE("마자용은 째려보기를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_BIG_PECKS);
        MESSAGE("상대 마자의 방어는 떨어지지 않는다!");
        MESSAGE("2는 마자를 넣어버렸다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("마자용은 모래뿌리기를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_KEEN_EYE);
        MESSAGE("상대 마자용의 명중률은 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-23 Clear Body does not block the user's own Close Combat")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CLEAR_BODY); }
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

SINGLE_BATTLE_TEST("HNS9730 K2-24 Growl vs Mirror Armor reflects")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-25 Tickle vs Mirror Armor with the user at -6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CHARM); }
        TURN { MOVE(opponent, MOVE_CHARM); }
        TURN { MOVE(opponent, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_TICKLE); }
    } SCENE {
        MESSAGE("상대 마자는 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("상대 마자는 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("상대 마자는 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
        MESSAGE("마자용은 간지르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TICKLE, player);
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        MESSAGE("마자용의 공격은 더 떨어지지 않는다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-26 Rock Tomb secondary vs Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_TOMB); }
    } SCENE {
        MESSAGE("마자용은 암석봉인을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_TOMB, player);
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-27 Growl vs Defiant")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DEFIANT); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_DEFIANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-28 Tickle vs Competitive")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_COMPETITIVE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TICKLE); }
    } SCENE {
        MESSAGE("마자용은 간지르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TICKLE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_COMPETITIVE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_COMPETITIVE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-29 Growl vs Defiant at +6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DEFIANT); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); MOVE(player, MOVE_SCARY_FACE); }
    } SCENE {
        MESSAGE("상대 마자는 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용은 겁나는얼굴을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCARY_FACE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 매우 크게 떨어졌다!");
        MESSAGE("상대 마자의 공격은 더 올라가지 않는다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-30 Defiant does not activate on a partner's drop")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_DEFIANT); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_GROWL, target: playerLeft); }
    } SCENE {
        MESSAGE("마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, playerRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-31 Speed Boost end of turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-32 Moxie on KO")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_MOXIE); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 마자는 쓰러졌다!");
        ABILITY_POPUP(player, ABILITY_MOXIE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-33 Weak Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_WEAK_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_WEAK_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-34 Stamina")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_STAMINA); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_STAMINA);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-35 Justified")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_JUSTIFIED); }
    } WHEN {
        TURN { MOVE(player, MOVE_BITE); }
    } SCENE {
        MESSAGE("마자용은 물기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITE, player);
        MESSAGE("효과가 굉장했다!");
        ABILITY_POPUP(opponent, ABILITY_JUSTIFIED);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자는 풀이 죽어 기술을 쓸 수 없다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-36 Steam Engine")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_STEAM_ENGINE); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        MESSAGE("마자용은 물대포를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        ABILITY_POPUP(opponent, ABILITY_STEAM_ENGINE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 매우 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-37 Anger Point on a critical hit (pre: no 의 in TARGETSSTATWASMAXEDOUT)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_ANGER_POINT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: TRUE); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("급소에 맞았다!");
        ABILITY_POPUP(opponent, ABILITY_ANGER_POINT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자공격이 최고치까지 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-38 Anger Shell below half")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_ANGER_SHELL); MaxHP(100); HP(60); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); }
    } SCENE {
        MESSAGE("마자용은 분노의앞니를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, player);
        ABILITY_POPUP(opponent, ABILITY_ANGER_SHELL);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-39 Berserk below half")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_BERSERK); MaxHP(100); HP(60); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); }
    } SCENE {
        MESSAGE("마자용은 분노의앞니를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, player);
        ABILITY_POPUP(opponent, ABILITY_BERSERK);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-40 Download")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_DOWNLOAD); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DOWNLOAD);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-41 Intrepid Sword and Dauntless Shield")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTREPID_SWORD); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DAUNTLESS_SHIELD); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INTREPID_SWORD);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        ABILITY_POPUP(opponent, ABILITY_DAUNTLESS_SHIELD);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-42 Beast Boost on KO")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_BEAST_BOOST); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("상대 마자는 쓰러졌다!");
        ABILITY_POPUP(player, ABILITY_BEAST_BOOST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("2는 마자를 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-43 Sap Sipper absorbs a Grass move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SAP_SIPPER); }
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        MESSAGE("마자용은 덩굴채찍을 썼다!");
        ABILITY_POPUP(opponent, ABILITY_SAP_SIPPER);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-44 Motor Drive absorbs an Electric move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MOTOR_DRIVE); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); }
    } SCENE {
        MESSAGE("마자용은 전기쇼크를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MOTOR_DRIVE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-45 Lightning Rod at +6 Sp. Atk")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_LIGHTNING_ROD); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TAIL_GLOW); }
        TURN { MOVE(opponent, MOVE_TAIL_GLOW); }
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); }
    } SCENE {
        MESSAGE("상대 마자는 반딧불을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAIL_GLOW, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 매우 크게 올라갔다!");
        MESSAGE("상대 마자는 반딧불을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAIL_GLOW, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 매우 크게 올라갔다!");
        MESSAGE("마자용은 전기쇼크를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_LIGHTNING_ROD);
        MESSAGE("상대 마자에게는 효과가 없는 것 같다...");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-46 Cotton Down")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_COTTON_DOWN); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_COTTON_DOWN);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-47 Gooey")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_GOOEY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_GOOEY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-48 Moody end of turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_MOODY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_MOODY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수방어가 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-49 Supersweet Syrup")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SUPERSWEET_SYRUP); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SUPERSWEET_SYRUP);
        MESSAGE("마자용에게 향기가 배어서 가시지 않게 되었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 회피율이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-50 Electromorphosis")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_ELECTROMORPHOSIS); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_ELECTROMORPHOSIS);
        MESSAGE("상대 마자는 할퀴기에 맞아 충전되었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-51 Wind Power")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_WIND_POWER); }
    } WHEN {
        TURN { MOVE(player, MOVE_GUST); }
    } SCENE {
        MESSAGE("마자용은 바람일으키기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GUST, player);
        ABILITY_POPUP(opponent, ABILITY_WIND_POWER);
        MESSAGE("상대 마자는 바람일으키기에 맞아 충전되었다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-52 Wind Rider absorbs a wind move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_WIND_RIDER); }
    } WHEN {
        TURN { MOVE(player, MOVE_GUST); }
    } SCENE {
        MESSAGE("마자용은 바람일으키기를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_WIND_RIDER);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-53 Flower Veil protects a Grass ally from Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_ODDISH) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        MESSAGE("상대 뚜벅쵸를 플라워베일이 지켜 주고 있다!");
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        MESSAGE("상대 이상해씨를 플라워베일이 지켜 주고 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-54 Flower Veil vs Intimidate")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_ODDISH) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        MESSAGE("상대 뚜벅쵸를 플라워베일이 지켜 주고 있다!");
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        MESSAGE("상대 이상해씨를 플라워베일이 지켜 주고 있다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-55 Flower Veil vs Rock Tomb secondary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_ODDISH) { Ability(ABILITY_FLOWER_VEIL); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_TOMB); }
    } SCENE {
        MESSAGE("마자용은 암석봉인을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_TOMB, player);
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-56 Opportunist copies a foe's Swords Dance")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_OPPORTUNIST); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        ABILITY_POPUP(opponentLeft, ABILITY_OPPORTUNIST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-57 Costar copies the ally's boosts")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_COSTAR); }
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SWORDS_DANCE); MOVE(playerRight, MOVE_CELEBRATE); }
        TURN { SWITCH(playerRight, 2); }
    } SCENE {
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        MESSAGE("마자 이제 됐어! 돌아와!");
        MESSAGE("가랏! 마자!");
        ABILITY_POPUP(playerRight, ABILITY_COSTAR);
        MESSAGE("마자는 마자의 능력 변화를 복사했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-58 Infiltrator ignores Mist")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INFILTRATOR); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MIST); MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("상대 마자는 흰안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponent);
        MESSAGE("상대는 흰안개에 둘러싸였다!");
        MESSAGE("마자용은 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-59 Steadfast on flinch")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_STEADFAST); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FAKE_OUT); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자용은 속이기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FAKE_OUT, player);
        MESSAGE("상대 마자는 풀이 죽어 기술을 쓸 수 없다!");
        ABILITY_POPUP(opponent, ABILITY_STEADFAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-60 Water Compaction and Thermal Exchange")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_THERMAL_EXCHANGE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_WATER_COMPACTION); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN); MOVE(opponent, MOVE_EMBER); }
    } SCENE {
        MESSAGE("마자용은 물대포를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        ABILITY_POPUP(opponent, ABILITY_WATER_COMPACTION);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 크게 올라갔다!");
        MESSAGE("상대 마자는 불꽃세례를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EMBER, opponent);
        ABILITY_POPUP(player, ABILITY_THERMAL_EXCHANGE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-61 Contrary Intimidate with Defiant-like raise blocked at +6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        TURN { MOVE(opponent, MOVE_GROWL); }
        TURN { MOVE(opponent, MOVE_GROWL); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 매우 크게 떨어졌다!");
        MESSAGE("상대 마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("상대 마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("마자용 이제 됐어! 돌아와!");
        MESSAGE("가랏! 마자!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-62 Mind's Eye blocks accuracy drop")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MINDS_EYE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SAND_ATTACK); }
    } SCENE {
        MESSAGE("마자용은 모래뿌리기를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MINDS_EYE);
        MESSAGE("상대 마자의 명중률은 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-63 Full Metal Body vs Rock Tomb secondary")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_FULL_METAL_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_TOMB); }
    } SCENE {
        MESSAGE("마자용은 암석봉인을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_TOMB, player);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-64 reflected Growl from Mirror Armor triggers the user's Defiant")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_DEFIANT); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-65 Intimidate vs Guard Dog at +6 Attack (pre: names the Intimidate user)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_GUARD_DOG); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용 좋았어! 돌아와!");
        MESSAGE("다녀와! 마자!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ABILITY_POPUP(opponent, ABILITY_GUARD_DOG);
        MESSAGE("마자의 공격은 더 올라가지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-66 Intimidate vs Rattled at +6 Speed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_RATTLED); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { SWITCH(player, 1); }
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

SINGLE_BATTLE_TEST("HNS9730 K2-67 Defiant at +6 Attack after Intimidate")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_INTIMIDATE); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_DEFIANT); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용 좋았어! 돌아와!");
        MESSAGE("다녀와! 마자!");
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_DEFIANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K2-68 Speed Boost at +6 prints nothing")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SPEED_BOOST); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자는 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        ABILITY_POPUP(opponent, ABILITY_SPEED_BOOST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
        MESSAGE("상대 마자는 고속이동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AGILITY, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
        ABILITY_POPUP(opponent, ABILITY_SPEED_BOOST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 스피드가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K2-69 Cotton Down hits everyone else, one with Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_COTTON_DOWN); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_COTTON_DOWN);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 스피드가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 스피드가 떨어졌다!");
        ABILITY_POPUP(opponentRight, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
    }
}
