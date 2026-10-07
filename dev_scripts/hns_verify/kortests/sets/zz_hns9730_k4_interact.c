#include "global.h"
#include "test/battle.h"

// seq 181 #9730 area E Korean stat-change output set K4: move interactions, side effects and HnS-specific
// message paths around stat changes (scratch only, never committed).

SINGLE_BATTLE_TEST("HNS9730 K4-01 Parting Shot lowers and switches")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_PARTING_SHOT); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
        MESSAGE("상대 마자의 특수공격이 떨어졌다!");
        MESSAGE("마자용은 1의 곁으로 돌아간다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-02 Parting Shot with both stats at -6 (no switch)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_PARTING_SHOT); }
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
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        MESSAGE("상대 마자의 능력은 더 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-03 Parting Shot vs Contrary with both stats at +6")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CONTRARY); }
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_CHARM); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_EERIE_IMPULSE); }
        TURN { MOVE(player, MOVE_PARTING_SHOT); }
    } SCENE {
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 애교부리기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 괴전파를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EERIE_IMPULSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
        MESSAGE("마자용은 괴전파를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EERIE_IMPULSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
        MESSAGE("마자용은 괴전파를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EERIE_IMPULSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        MESSAGE("상대 마자의 능력은 더 올라가지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-04 Parting Shot vs Clear Body (no switch)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CLEAR_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_PARTING_SHOT); }
    } SCENE {
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, player);
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        MESSAGE("상대 마자는 클리어바디의 효과로 능력이 떨어지지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-05 Parting Shot vs Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_PARTING_SHOT); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, player);
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 떨어졌다!");
        MESSAGE("마자용은 1의 곁으로 돌아간다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-06 Magic Coat bounces Growl")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MAGIC_COAT); MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("상대 마자는 매직코트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, opponent);
        MESSAGE("상대 마자는 매직코트에 둘러싸였다!");
        MESSAGE("마자용은 울음소리를 썼다!");
        MESSAGE("상대 마자는 울음소리를 되받아쳤다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-07 Magic Bounce bounces Charm")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MAGIC_BOUNCE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CHARM); }
    } SCENE {
        MESSAGE("마자용은 애교부리기를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MAGIC_BOUNCE);
        MESSAGE("마자용의 애교부리기를 되받아쳤다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CHARM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-08 Snatch steals Swords Dance")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SNATCH); MOVE(player, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("상대 마자는 가로채기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, opponent);
        MESSAGE("상대 마자는 상대의 움직임을 살피고 있다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SNATCH_MOVE, opponent);
        MESSAGE("상대 마자는 마자용의 기술을 가로챘다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-09 Snatch steals Dragon Dance")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_SNATCH); MOVE(opponent, MOVE_DRAGON_DANCE); }
    } SCENE {
        MESSAGE("마자용은 가로채기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, player);
        MESSAGE("마자용은 상대의 움직임을 살피고 있다!");
        MESSAGE("상대 마자는 용의춤을 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SNATCH_MOVE, player);
        MESSAGE("마자용은 상대 마자의 기술을 가로챘다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-10 Magic Bounce bounces Parting Shot (seq 129 interim: the user is switched)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MAGIC_BOUNCE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_PARTING_SHOT); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MAGIC_BOUNCE);
        MESSAGE("마자용의 막말내뱉기를 되받아쳤다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("마자용의 특수공격이 떨어졌다!");
        MESSAGE("마자용은 1의 곁으로 돌아간다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-11 Sticky Web on switch-in")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_STICKY_WEB); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 끈적끈적네트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STICKY_WEB, player);
        MESSAGE("상대 발밑에 끈적끈적네트가 펼쳐졌다!");
        MESSAGE("2는 마자를 넣어버렸다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("상대 마자용은 끈적끈적네트에 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-12 Sticky Web vs Mirror Armor on switch-in")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_STICKY_WEB); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 끈적끈적네트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STICKY_WEB, player);
        MESSAGE("상대 발밑에 끈적끈적네트가 펼쳐졌다!");
        MESSAGE("2는 마자를 넣어버렸다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("상대 마자용은 끈적끈적네트에 걸렸다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-13 Sticky Web vs Contrary and Defiant")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_DEFIANT); }
    } WHEN {
        TURN { MOVE(player, MOVE_STICKY_WEB); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        MESSAGE("마자용은 끈적끈적네트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STICKY_WEB, player);
        MESSAGE("상대 발밑에 끈적끈적네트가 펼쳐졌다!");
        MESSAGE("2는 마자를 넣어버렸다!");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("상대 마자용은 끈적끈적네트에 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자용의 스피드가 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_DEFIANT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자용의 공격이 크게 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-14 Rototiller: Grass types only, others not affected")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_ODDISH);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROTOTILLER); }
    } SCENE {
        MESSAGE("마자용은 일구기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROTOTILLER, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("뚜벅쵸의 공격이 올라갔다!");
        MESSAGE("뚜벅쵸의 특수공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 이상해씨의 공격이 올라갔다!");
        MESSAGE("상대 이상해씨의 특수공격이 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-15 Rototiller with no Grass type on the field")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROTOTILLER); }
    } SCENE {
        MESSAGE("마자용은 일구기를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-16 Flower Shield")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_ODDISH);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FLOWER_SHIELD); }
    } SCENE {
        MESSAGE("마자용은 플라워가드를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLOWER_SHIELD, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("뚜벅쵸의 방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 이상해씨의 방어가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-17 Coaching")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_COACHING); }
    } SCENE {
        MESSAGE("마자용은 코칭을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COACHING, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 공격이 올라갔다!");
        MESSAGE("마자의 방어가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-18 Gear Up and Magnetic Flux with Plus/Minus")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_PLUS); }
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_MINUS); }
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GEAR_UP); }
        TURN { MOVE(playerLeft, MOVE_MAGNETIC_FLUX); }
    } SCENE {
        MESSAGE("마자용은 어시스트기어를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GEAR_UP, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 특수공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 공격이 올라갔다!");
        MESSAGE("마자의 특수공격이 올라갔다!");
        MESSAGE("마자용은 자기장조작을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGNETIC_FLUX, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 방어가 올라갔다!");
        MESSAGE("마자의 특수방어가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-19 Howl raises user and ally")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HOWL); }
    } SCENE {
        MESSAGE("마자용은 멀리짖기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HOWL, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 공격이 올라갔다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HOWL, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 공격이 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-20 Aromatic Mist and Decorate on the ally")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_AROMATIC_MIST, target: playerRight); }
        TURN { MOVE(playerLeft, MOVE_DECORATE, target: playerRight); }
    } SCENE {
        MESSAGE("마자용은 아로마미스트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AROMATIC_MIST, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 특수방어가 올라갔다!");
        MESSAGE("마자용은 데코레이션을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DECORATE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 공격이 크게 올라갔다!");
        MESSAGE("마자의 특수공격이 크게 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-21 Icy Wind lowers both foes")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ICY_WIND); }
    } SCENE {
        MESSAGE("마자용은 얼어붙은바람을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICY_WIND, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        MESSAGE("상대 마자의 스피드가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 스피드가 떨어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-22 Growl on both foes, one Clear Body")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_CLEAR_BODY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_CLEAR_BODY);
        MESSAGE("상대 마자는 클리어바디의 효과로 능력이 떨어지지 않는다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-23 Captivate vs Oblivious")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_MALE); }
        PLAYER(SPECIES_WYNAUT) { Gender(MON_MALE); }
        OPPONENT(SPECIES_WYNAUT) { Gender(MON_FEMALE); Ability(ABILITY_OBLIVIOUS); }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CAPTIVATE); }
    } SCENE {
        MESSAGE("마자용은 유혹을 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_OBLIVIOUS);
        MESSAGE("상대 마자에게는 효과가 없는 것 같다...");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CAPTIVATE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 특수공격이 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-24 Insomnia blocks Hypnosis")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_INSOMNIA); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPNOSIS); }
    } SCENE {
        MESSAGE("마자용은 최면술을 썼다!");
        ABILITY_POPUP(opponent, ABILITY_INSOMNIA);
        MESSAGE("상대 마자는 잠들지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-25 Insomnia blocks Rest")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_INSOMNIA); HP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_REST); }
    } SCENE {
        MESSAGE("마자용은 잠자기를 썼다!");
        ABILITY_POPUP(player, ABILITY_INSOMNIA);
        MESSAGE("마자용은 잠들지 않는다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-26 Sweet Veil ally blocks Hypnosis")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SWEET_VEIL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPNOSIS, target: opponentRight); }
    } SCENE {
        MESSAGE("마자용은 최면술을 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_SWEET_VEIL);
        MESSAGE("상대 마자는 스위트베일 때문에 잠들지 않는다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-27 Sweet Veil ally that came in blocks Yawn sleep at end of turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_SWEET_VEIL); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_YAWN, target: opponentLeft); }
        TURN { SWITCH(opponentRight, 2); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 하품을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, playerLeft);
        MESSAGE("상대 마자의 졸음을 유도했다!");
        MESSAGE("2는 마자용을 넣어버렸다!");
        MESSAGE("2는 마자를 내보냈다!");
        ABILITY_POPUP(opponentRight, ABILITY_SWEET_VEIL);
        MESSAGE("상대 마자는 스위트베일 때문에 잠들지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-28 Leaf Guard blocks Rest in sun")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_LEAF_GUARD); HP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); MOVE(player, MOVE_REST); }
    } SCENE {
        MESSAGE("상대 마자는 쾌청을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        MESSAGE("햇살이 강해졌다!");
        MESSAGE("마자용은 잠자기를 썼다!");
        ABILITY_POPUP(player, ABILITY_LEAF_GUARD);
        MESSAGE("마자용은 잠들지 않는다!");
        MESSAGE("햇살이 강하다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-29 Sweet Veil blocks Rest")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SWEET_VEIL); HP(100); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_REST); }
    } SCENE {
        MESSAGE("마자용은 잠자기를 썼다!");
        ABILITY_POPUP(player, ABILITY_SWEET_VEIL);
        MESSAGE("마자용은 스위트베일 때문에 잠들지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-30 Swagger vs Own Tempo")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_OWN_TEMPO); }
    } WHEN {
        TURN { MOVE(player, MOVE_SWAGGER); }
    } SCENE {
        MESSAGE("마자용은 뽐내기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWAGGER, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        ABILITY_POPUP(opponent, ABILITY_OWN_TEMPO);
        MESSAGE("상대 마자는 혼란되지 않는다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-31 Swagger on a confused foe at +6 Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); MOVE(player, MOVE_CONFUSE_RAY); }
        TURN { MOVE(player, MOVE_SWAGGER); }
    } SCENE {
        MESSAGE("상대 마자는 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("마자용은 이상한빛을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, player);
        MESSAGE("상대 마자는 혼란에 빠졌다!");
        MESSAGE("상대 마자는 혼란에 빠져 있다!");
        MESSAGE("영문도 모른 채 자신을 공격했다!");
        MESSAGE("마자용은 뽐내기를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-32 Rage builds Attack")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_RAGE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("마자용은 분노를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAGE, player);
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 분노 볼티지가 올라가고 있다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-33 Spectral Thief steals boosts")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SWORDS_DANCE); MOVE(player, MOVE_SPECTRAL_THIEF); }
    } SCENE {
        MESSAGE("상대 마자는 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWORDS_DANCE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 크게 올라갔다!");
        MESSAGE("마자용은 섀도스틸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPECTRAL_THIEF, player);
        MESSAGE("마자용은 올라간 능력을 빼앗았다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 크게 올라갔다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPECTRAL_THIEF, player);
        MESSAGE("효과가 굉장했다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-34 Defiant activates on Sticky Web set by the foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Ability(ABILITY_COMPETITIVE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_STICKY_WEB); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 끈적끈적네트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STICKY_WEB, opponent);
        MESSAGE("우리 편 발밑에 끈적끈적네트가 펼쳐졌다!");
        MESSAGE("마자용 이제 됐어! 돌아와!");
        MESSAGE("가랏! 마자!");
        MESSAGE("마자는 끈적끈적네트에 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자의 스피드가 떨어졌다!");
        ABILITY_POPUP(player, ABILITY_COMPETITIVE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자의 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-35 Bulk Up, Calm Mind, Coil, Work Up, Hone Claws")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_BULK_UP); MOVE(opponent, MOVE_CALM_MIND); }
        TURN { MOVE(player, MOVE_COIL); MOVE(opponent, MOVE_WORK_UP); }
        TURN { MOVE(player, MOVE_HONE_CLAWS); }
    } SCENE {
        MESSAGE("마자용은 벌크업을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULK_UP, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("상대 마자는 명상을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CALM_MIND, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 올라갔다!");
        MESSAGE("상대 마자의 특수방어가 올라갔다!");
        MESSAGE("마자용은 똬리틀기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COIL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 명중률이 올라갔다!");
        MESSAGE("상대 마자는 분발을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WORK_UP, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자의 특수공격이 올라갔다!");
        MESSAGE("마자용은 손톱갈기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HONE_CLAWS, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 명중률이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-36 Victory Dance, Cosmic Power, Tail Glow")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_VICTORY_DANCE); MOVE(opponent, MOVE_COSMIC_POWER); }
        TURN { MOVE(player, MOVE_TAIL_GLOW); }
    } SCENE {
        MESSAGE("마자용은 승리의춤을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VICTORY_DANCE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("상대 마자는 코스믹파워를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COSMIC_POWER, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 방어가 올라갔다!");
        MESSAGE("상대 마자의 특수방어가 올라갔다!");
        MESSAGE("마자용은 반딧불을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAIL_GLOW, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수공격이 매우 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-37 Contrary Shell Smash at mixed limits")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
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
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-38 Clangorous Soul with Throat Spray")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_THROAT_SPRAY); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_CLANGOROUS_SOUL); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 소울비트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLANGOROUS_SOUL, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("마자용의 특수공격이 올라갔다!");
        MESSAGE("마자용의 특수방어가 올라갔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용은 목스프레이로 특수공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-39 Snarl vs Competitive")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_COMPETITIVE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SNARL); }
    } SCENE {
        MESSAGE("마자용은 바크아웃을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNARL, player);
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_COMPETITIVE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-40 Trailblaze and Lunge")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRAILBLAZE); }
        TURN { MOVE(player, MOVE_LUNGE); }
    } SCENE {
        MESSAGE("마자용은 개척하기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRAILBLAZE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 올라갔다!");
        MESSAGE("마자용은 덤벼들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LUNGE, player);
        MESSAGE("효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-41 Magic Coat bounces Parting Shot (seq 129 interim: the user is switched)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MAGIC_COAT); MOVE(player, MOVE_PARTING_SHOT); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("상대 마자는 매직코트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_COAT, opponent);
        MESSAGE("상대 마자는 매직코트에 둘러싸였다!");
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        MESSAGE("상대 마자는 막말내뱉기를 되받아쳤다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("마자용의 특수공격이 떨어졌다!");
        MESSAGE("마자용은 1의 곁으로 돌아간다!");
        MESSAGE("가랏! 마자!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-42 Magic Bounce bounces Growl back onto Contrary user")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_CONTRARY); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MAGIC_BOUNCE); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MAGIC_BOUNCE);
        MESSAGE("마자용의 울음소리를 되받아쳤다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-43 Snatch steals Swords Dance at +6 for the snatcher")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_BELLY_DRUM); }
        TURN { MOVE(opponent, MOVE_SNATCH); MOVE(player, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("상대 마자는 배북을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BELLY_DRUM, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자는 체력을 깎아서 풀 파워로 만들었다!");
        MESSAGE("상대 마자는 가로채기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNATCH, opponent);
        MESSAGE("상대 마자는 상대의 움직임을 살피고 있다!");
        MESSAGE("마자용은 칼춤을 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SNATCH_MOVE, opponent);
        MESSAGE("상대 마자는 마자용의 기술을 가로챘다!");
        MESSAGE("상대 마자의 공격은 더 올라가지 않는다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-44 Growl into a Mirror Armor foe and a plain foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GROWL); }
    } SCENE {
        MESSAGE("마자용은 울음소리를 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("마자용의 공격이 떨어졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 공격이 떨어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-45 Icy Wind into Flower Veil Grass foes")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_ODDISH) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ICY_WIND); }
    } SCENE {
        MESSAGE("마자용은 얼어붙은바람을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICY_WIND, playerLeft);
        MESSAGE("상대 뚜벅쵸에게 효과가 굉장했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        MESSAGE("상대 마자용의 스피드가 떨어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-46 Flower Veil blocks Hypnosis on a Grass ally (status path)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_ODDISH);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPNOSIS, target: opponentRight); }
    } SCENE {
        MESSAGE("마자용은 최면술을 썼다!");
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        MESSAGE("상대 뚜벅쵸를 플라워베일이 지켜 주고 있다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-47 Octolock vs Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_OCTOLOCK); }
    } SCENE {
        MESSAGE("마자용은 문어굳히기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_OCTOLOCK, player);
        MESSAGE("상대 마자는 문어굳히기 때문에 도망칠 수 없게 되었다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 떨어졌다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 특수방어가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-48 Octolock and Syrup Bomb vs Mist")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_MIST); }
        TURN { MOVE(player, MOVE_OCTOLOCK); }
        TURN { MOVE(player, MOVE_SYRUP_BOMB); }
    } SCENE {
        MESSAGE("상대 마자는 흰안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponent);
        MESSAGE("상대는 흰안개에 둘러싸였다!");
        MESSAGE("마자용은 문어굳히기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_OCTOLOCK, player);
        MESSAGE("상대 마자는 문어굳히기 때문에 도망칠 수 없게 되었다!");
        MESSAGE("상대 마자를 흰안개가 지켜주고 있다");
        MESSAGE("마자용은 시럽봄을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SYRUP_BOMB, player);
        MESSAGE("상대 마자는 물엿범벅이 되었다!");
        MESSAGE("상대 마자를 흰안개가 지켜주고 있다");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent);
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-49 Syrup Bomb vs Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_SYRUP_BOMB); }
    } SCENE {
        MESSAGE("마자용은 시럽봄을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SYRUP_BOMB, player);
        MESSAGE("상대 마자는 물엿범벅이 되었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent);
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-50 Curse after the user's own Mist")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_MIST); }
        TURN { MOVE(player, MOVE_CURSE); }
    } SCENE {
        MESSAGE("마자용은 흰안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, player);
        MESSAGE("우리 편은 흰안개에 둘러싸였다!");
        MESSAGE("마자용은 저주를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CURSE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 스피드가 떨어졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 방어가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-51 Coaching onto an ally behind a Substitute")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_SUBSTITUTE); MOVE(playerLeft, MOVE_COACHING, target: playerRight); }
    } SCENE {
        MESSAGE("마자는 대타출동을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, playerRight);
        MESSAGE("마자의 대타가 나타났다");
        MESSAGE("마자용은 코칭을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_COACHING, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        MESSAGE("마자의 공격이 올라갔다!");
        MESSAGE("마자의 방어가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-52 Screech vs Mirror Armor")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_MIRROR_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCREECH); }
    } SCENE {
        MESSAGE("마자용은 싫은소리를 썼다!");
        ABILITY_POPUP(opponent, ABILITY_MIRROR_ARMOR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 방어가 매우 크게 떨어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-53 Parting Shot vs Mist (no switch)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MIST); }
        TURN { MOVE(player, MOVE_PARTING_SHOT); }
    } SCENE {
        MESSAGE("상대 마자는 흰안개를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MIST, opponent);
        MESSAGE("상대는 흰안개에 둘러싸였다!");
        MESSAGE("마자용은 막말내뱉기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PARTING_SHOT, player);
        MESSAGE("상대 마자를 흰안개가 지켜주고 있다");
    }
}

SINGLE_BATTLE_TEST("HNS9730 K4-54 Rototiller in singles with no Grass type")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ROTOTILLER); }
    } SCENE {
        MESSAGE("마자용은 일구기를 썼다!");
        MESSAGE("그러나 실패하고 말았다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-55 Flower Veil vs Leaf Storm by the Grass ally (self drop)")
{
    GIVEN {
        PLAYER(SPECIES_ODDISH);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_LEAF_STORM, target: opponentLeft); }
    } SCENE {
        MESSAGE("뚜벅쵸는 리프스톰을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEAF_STORM, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        MESSAGE("뚜벅쵸의 특수공격이 매우 크게 떨어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-56 Flower Veil vs Octolock on the Grass ally")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_ODDISH);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_OCTOLOCK, target: opponentRight); }
    } SCENE {
        MESSAGE("마자용은 문어굳히기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_OCTOLOCK, playerLeft);
        MESSAGE("상대 뚜벅쵸는 문어굳히기 때문에 도망칠 수 없게 되었다!");
        ABILITY_POPUP(opponentLeft, ABILITY_FLOWER_VEIL);
        MESSAGE("상대 뚜벅쵸를 플라워베일이 지켜 주고 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9730 K4-57 Flower Veil vs Gooey on a Grass attacker (not the move target)")
{
    GIVEN {
        PLAYER(SPECIES_ODDISH);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_FLOWER_VEIL); }
        OPPONENT(SPECIES_WYNAUT) { Ability(ABILITY_GOOEY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); }
    } SCENE {
        MESSAGE("뚜벅쵸는 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
    }
}
