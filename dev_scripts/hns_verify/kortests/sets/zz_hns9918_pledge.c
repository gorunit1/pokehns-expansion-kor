#include "global.h"
#include "test/battle.h"

// seq 188 #9918 (Customizeable Pledge Moves) Korean output regression set. Prefix "HNS9918".
// SCENE = Korean output of the pre-port HnS (base 1f773c7038, code ccbf3123a9), recorded with tools/trace.patch.
// Pins the pledge combo messages (waiting / combined), Rainbow / Sea of Fire / Swamp start, end-turn and end messages
// with their side names (team tokens), the side effects (Rainbow doubled chance, Swamp quarter Speed, Sea of Fire 1/8
// damage), the starting-status path (B_MSG_SET_*), and Future Sight / Doom Desire (setfutureattack moves into
// CancelerInterruptibleMoves in #9918).
// "#10214 CHANGE EXPECTED": the sea of fire end message takes {B_ATK_TEAM2}, but the HnS end-turn block does not set
// gBattlerAttacker (it stays the fastest battler), so the side name can be the wrong one. Upstream #10214 (seq 385)
// sets it; these tests will change then.

DOUBLE_BATTLE_TEST("HNS9918 K01 Water + Fire Pledge by the player: rainbow on the player's side, then it disappears")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_WATER_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_FIRE_PLEDGE, target: opponentRight); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 물의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 불꽃의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_PLEDGE, playerRight);
        HP_BAR(opponentRight);
        MESSAGE("우리 편 하늘에 무지개가 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAINBOW, playerRight);
        MESSAGE("우리 편 하늘에서 무지개가 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K02 Fire + Water Pledge by the opponent: rainbow on the opposing side, then it disappears")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_FIRE_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_WATER_PLEDGE, target: playerRight); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자용은 불꽃의맹세를 썼다!");
        MESSAGE("상대 마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("상대 마자는 물의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_PLEDGE, opponentRight);
        HP_BAR(playerRight);
        MESSAGE("상대 하늘에 무지개가 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAINBOW, opponentRight);
        MESSAGE("상대 하늘에서 무지개가 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K03 Fire + Grass Pledge by the player: sea of fire on the opposing side and its damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); }
    } SCENE {
        MESSAGE("마자용은 불꽃의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 풀의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
        HP_BAR(opponentLeft);
        MESSAGE("상대 주변이 불바다에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponentLeft);
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K04 Grass + Fire Pledge by the opponent: sea of fire on the player's side and its damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_GRASS_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_FIRE_PLEDGE, target: playerLeft); }
    } SCENE {
        MESSAGE("상대 마자용은 풀의맹세를 썼다!");
        MESSAGE("상대 마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("상대 마자는 불꽃의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, opponentRight);
        HP_BAR(playerLeft);
        MESSAGE("우리 편 주변이 불바다에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, playerLeft);
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        MESSAGE("마자는 불바다의 데미지를 입었다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K05 Grass + Water Pledge by the player: swamp on the opposing side, then it disappears")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GRASS_PLEDGE, target: opponentRight); MOVE(playerRight, MOVE_WATER_PLEDGE, target: opponentRight); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 풀의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 물의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, playerRight);
        HP_BAR(opponentRight);
        MESSAGE("상대 주변에 습지초원이 펼쳐졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SWAMP, opponentRight);
        MESSAGE("상대 주변의 습지초원이 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K06 Water + Grass Pledge by the opponent: swamp on the player's side, then it disappears")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_WATER_PLEDGE, target: playerRight); MOVE(opponentRight, MOVE_GRASS_PLEDGE, target: playerRight); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자용은 물의맹세를 썼다!");
        MESSAGE("상대 마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("상대 마자는 풀의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, opponentRight);
        HP_BAR(playerRight);
        MESSAGE("우리 편 주변에 습지초원이 펼쳐졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SWAMP, playerRight);
        MESSAGE("우리 편 주변의 습지초원이 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K07 Rainbow doubles a 50% secondary effect (Poison Fang always badly poisons)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_WATER_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_FIRE_PLEDGE, target: opponentLeft); }
        TURN { MOVE(playerLeft, MOVE_POISON_FANG, target: opponentLeft, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); MOVE(playerRight, MOVE_POISON_FANG, target: opponentRight, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } SCENE {
        MESSAGE("마자용은 물의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 불꽃의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_PLEDGE, playerRight);
        MESSAGE("우리 편 하늘에 무지개가 걸렸다!");
        MESSAGE("마자용은 맹독엄니를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POISON_FANG, playerLeft);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponentLeft);
        MESSAGE("상대 마자용의 몸에 맹독이 퍼졌다!");
        MESSAGE("마자는 맹독엄니를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POISON_FANG, playerRight);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponentRight);
        MESSAGE("상대 마자의 몸에 맹독이 퍼졌다!");
        MESSAGE("상대 마자용은 독에 의한 데미지를 입고 있다!");
        MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K08 Swamp quarters the Speed of the opposing side (turn order flips)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(90); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GRASS_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_WATER_PLEDGE, target: opponentLeft); MOVE(opponentLeft, MOVE_HARDEN); MOVE(opponentRight, MOVE_HARDEN); }
        TURN { MOVE(playerLeft, MOVE_SPLASH); MOVE(playerRight, MOVE_SPLASH); MOVE(opponentLeft, MOVE_HARDEN); MOVE(opponentRight, MOVE_HARDEN); }
    } SCENE {
        MESSAGE("상대 마자용은 단단해지기를 썼다!");
        MESSAGE("상대 마자용의 방어가 올라갔다!");
        MESSAGE("상대 마자는 단단해지기를 썼다!");
        MESSAGE("상대 마자의 방어가 올라갔다!");
        MESSAGE("마자용은 풀의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 물의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, playerRight);
        MESSAGE("상대 주변에 습지초원이 펼쳐졌다!");
        MESSAGE("마자용은 튀어오르기를 썼다!");
        MESSAGE("그러나 아무 일도 일어나지 않았다");
        MESSAGE("마자는 튀어오르기를 썼다!");
        MESSAGE("그러나 아무 일도 일어나지 않았다");
        MESSAGE("상대 마자용은 단단해지기를 썼다!");
        MESSAGE("상대 마자용의 방어가 올라갔다!");
        MESSAGE("상대 마자는 단단해지기를 썼다!");
        MESSAGE("상대 마자의 방어가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K09 Sea of fire deals 1/8 of max HP at the end of the turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); HP(160); MaxHP(160); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_GRASS_PLEDGE, target: opponentRight); MOVE(playerRight, MOVE_FIRE_PLEDGE, target: opponentRight); }
    } SCENE {
        MESSAGE("마자용은 풀의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 불꽃의맹세를 썼다!");
        MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
        HP_BAR(opponentRight);
        MESSAGE("상대 주변이 불바다에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponentLeft);
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        HP_BAR(opponentLeft, damage: 20);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponentRight);
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        HP_BAR(opponentRight);
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K10 Pledge user waits, then its partner is asleep: no combined move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); Status1(STATUS1_SLEEP_TURN(3)); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_WATER_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_FIRE_PLEDGE, target: opponentRight); }
    } SCENE {
        MESSAGE("마자용은 물의맹세를 썼다!");
        MESSAGE("마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("마자는 쿨쿨 잠들어 있다");
        NONE_OF {
            MESSAGE("2개의 기술이 하나가 되었다! 콤비네이션 기술이다!{PAUSE 16}");
            MESSAGE("마자는 불꽃의맹세를 썼다!");
        }
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K11 Sea of fire on the opposing side and swamp on the player's side end in the same turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft);
               MOVE(opponentLeft, MOVE_WATER_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_GRASS_PLEDGE, target: playerLeft); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 불꽃의맹세를 썼다!");
        MESSAGE("마자는 풀의맹세를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_PLEDGE, playerRight);
        MESSAGE("상대 주변이 불바다에 둘러싸였다!");
        MESSAGE("상대 마자용은 물의맹세를 썼다!");
        MESSAGE("상대 마자용은 마자를 기다리고 있다...{PAUSE 16}");
        MESSAGE("상대 마자는 풀의맹세를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASS_PLEDGE, opponentRight);
        MESSAGE("우리 편 주변에 습지초원이 펼쳐졌다!");
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("우리 편 주변의 습지초원이 사라졌다!");
        MESSAGE("상대 주변의 불바다가 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K12 Sea of fire on the opposing side ends; the player is faster, the foe hit last (#10214 CHANGE EXPECTED)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); }
        TURN {}
        TURN {}
        TURN { MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft); }
    } SCENE {
        MESSAGE("상대 주변이 불바다에 둘러싸였다!");
        MESSAGE("상대 마자용은 몸통박치기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("우리 편 주변의 불바다가 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K13 Swamp on the player's side ends after the opponent hit on the last turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_WATER_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_GRASS_PLEDGE, target: playerLeft); }
        TURN {}
        TURN {}
        TURN { MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft); }
    } SCENE {
        MESSAGE("우리 편 주변에 습지초원이 펼쳐졌다!");
        MESSAGE("상대 마자용은 몸통박치기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponentLeft);
        MESSAGE("우리 편 주변의 습지초원이 사라졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9918 K14 Starting status: swamp on the opposing side (4 turns)")
{
    SetStartingStatus(STARTING_STATUS_SWAMP_OPPONENT_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 주변에 습지초원이 펼쳐졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SWAMP, opponent);
        MESSAGE("상대 주변의 습지초원이 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9918 K15 Starting status: swamp on the player's side (4 turns)")
{
    SetStartingStatus(STARTING_STATUS_SWAMP_PLAYER_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("우리 편 주변에 습지초원이 펼쳐졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SWAMP, player);
        MESSAGE("우리 편 주변의 습지초원이 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9918 K16 Starting status: sea of fire on the opposing side and its damage")
{
    SetStartingStatus(STARTING_STATUS_SEA_OF_FIRE_OPPONENT_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
    } SCENE {
        MESSAGE("상대 주변이 불바다에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, opponent);
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9918 K17 Starting status: sea of fire on the player's side (4 turns)")
{
    SetStartingStatus(STARTING_STATUS_SEA_OF_FIRE_PLAYER_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("우리 편 주변이 불바다에 둘러싸였다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SEA_OF_FIRE, player);
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        MESSAGE("우리 편 주변의 불바다가 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9918 K18 Starting status: rainbow on the player's side (4 turns)")
{
    SetStartingStatus(STARTING_STATUS_RAINBOW_PLAYER_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("우리 편 하늘에 무지개가 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAINBOW, player);
        MESSAGE("우리 편 하늘에서 무지개가 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9918 K19 Starting status: rainbow on the opposing side (4 turns)")
{
    SetStartingStatus(STARTING_STATUS_RAINBOW_OPPONENT_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 하늘에 무지개가 걸렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAINBOW, opponent);
        MESSAGE("상대 하늘에서 무지개가 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9918 K20 Future Sight is foreseen and hits two turns later")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); }
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FUTURE_SIGHT, player);
        MESSAGE("마자용은 미래의 공격을 예지했다!");
        MESSAGE("상대 마자는 미래예지 공격을 받았다!");
        HP_BAR(opponent);
        MESSAGE("효과가 별로인 듯하다.");
    }
}

SINGLE_BATTLE_TEST("HNS9918 K21 Doom Desire is chosen as destiny and hits two turns later")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DOOM_DESIRE); }
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 파멸의소원을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOOM_DESIRE, opponent);
        MESSAGE("상대 마자는 파멸의소원을 미래에 맡겼다!");
        MESSAGE("마자용은 파멸의소원 공격을 받았다!");
        HP_BAR(player);
    }
}

DOUBLE_BATTLE_TEST("HNS9918 K22 Sea of fire on the player's side ends; the foe is faster, the player hit last (#10214 CHANGE EXPECTED)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_GRASS_PLEDGE, target: playerLeft); MOVE(opponentRight, MOVE_FIRE_PLEDGE, target: playerLeft); }
        TURN {}
        TURN {}
        TURN { MOVE(playerLeft, MOVE_TACKLE, target: opponentLeft); }
    } SCENE {
        MESSAGE("우리 편 주변이 불바다에 둘러싸였다!");
        MESSAGE("마자용은 몸통박치기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, playerLeft);
        MESSAGE("마자용은 불바다의 데미지를 입었다!");
        MESSAGE("마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 주변의 불바다가 사라졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9918 K23 Starting status: sea of fire on the opposing side ends (#10214 CHANGE EXPECTED)")
{
    SetStartingStatus(STARTING_STATUS_SEA_OF_FIRE_OPPONENT_TEMPORARY);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 주변이 불바다에 둘러싸였다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
        MESSAGE("우리 편 주변의 불바다가 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}
