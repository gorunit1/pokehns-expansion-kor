#include "global.h"
#include "test/battle.h"

// seq 139 #9714 area B: Korean output regression set, part 2 (Tidy Up, Rapid Spin / Mortal Spin hazard texts,
// Substitute fading). Scratch only, never committed. Expected output = HnS before #9714 (code f3a58f9939).
// A space in MESSAGE matches one space or newline of the real text.

SINGLE_BATTLE_TEST("HNS9714 2-01 Tidy Up removes each hazard from both sides (player or foe user), then raises the user's stats")
{
    u32 hazard, user;
    PARAMETRIZE { hazard = 0; user = 0; }
    PARAMETRIZE { hazard = 1; user = 0; }
    PARAMETRIZE { hazard = 2; user = 0; }
    PARAMETRIZE { hazard = 3; user = 0; }
    PARAMETRIZE { hazard = 4; user = 0; }
    PARAMETRIZE { hazard = 0; user = 1; }
    PARAMETRIZE { hazard = 4; user = 1; }
    if (hazard == 4) {
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_PLAYER);
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_OPPONENT);
    }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (hazard == 0)
            TURN { MOVE(player, MOVE_SPIKES); MOVE(opponent, MOVE_SPIKES); }
        else if (hazard == 1)
            TURN { MOVE(player, MOVE_TOXIC_SPIKES); MOVE(opponent, MOVE_TOXIC_SPIKES); }
        else if (hazard == 2)
            TURN { MOVE(player, MOVE_STEALTH_ROCK); MOVE(opponent, MOVE_STEALTH_ROCK); }
        else if (hazard == 3)
            TURN { MOVE(player, MOVE_STICKY_WEB); MOVE(opponent, MOVE_STICKY_WEB); }
        else
            TURN {}
        if (user == 0)
            TURN { MOVE(player, MOVE_TIDY_UP); }
        else
            TURN { MOVE(opponent, MOVE_TIDY_UP); }
    } SCENE {
        if (user == 0)
            MESSAGE("마자용은 정리정돈을 썼다!");
        else
            MESSAGE("상대 마자는 정리정돈을 썼다!");
        if (hazard == 0) {
            MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
        } else if (hazard == 1) {
            MESSAGE("우리 편 발밑의 독압정이 사라졌다!");
            MESSAGE("상대 발밑의 독압정이 사라졌다!");
        } else if (hazard == 2) {
            MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
            MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        } else if (hazard == 3) {
            MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!");
            MESSAGE("상대 발밑의 끈적끈적네트가 사라졌다!");
        } else {
            MESSAGE("우리 편 주변의 강철이 사라졌다!");
            MESSAGE("상대 주변의 강철이 사라졌다!");
        }
        MESSAGE("정리정돈 끝!");
        if (user == 0) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
            MESSAGE("마자용의 공격이 올라갔다!");
            MESSAGE("마자용의 스피드가 올라갔다!");
        } else {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            MESSAGE("상대 마자의 공격이 올라갔다!");
            MESSAGE("상대 마자의 스피드가 올라갔다!");
        }
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9714 2-02 Tidy Up removes Substitutes: foe's, user's own, both (fade animation and text per battler)")
{
    u32 mode;
    PARAMETRIZE { mode = 0; } // foe's substitute
    PARAMETRIZE { mode = 1; } // user's own substitute
    PARAMETRIZE { mode = 2; } // both
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        if (mode == 0)
            TURN { MOVE(opponent, MOVE_SUBSTITUTE); }
        else if (mode == 1)
            TURN { MOVE(player, MOVE_SUBSTITUTE); }
        else
            TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SUBSTITUTE); }
        TURN { MOVE(player, MOVE_TIDY_UP); }
    } SCENE {
        MESSAGE("마자용은 정리정돈을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TIDY_UP, player);
        if (mode == 1 || mode == 2) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, player);
            MESSAGE("마자용의 대타는 사라져 버렸다...");
        }
        if (mode == 0 || mode == 2) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponent);
            MESSAGE("상대 마자의 대타는 사라져 버렸다...");
        }
        MESSAGE("정리정돈 끝!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("마자용의 공격이 올라갔다!");
        MESSAGE("마자용의 스피드가 올라갔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9714 2-03 Tidy Up with nothing to clear: no tidy text, stats rise")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TIDY_UP); }
    } SCENE {
        MESSAGE("상대 마자는 정리정돈을 썼다!");
        NONE_OF { MESSAGE("정리정돈 끝!"); }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("상대 마자의 공격이 올라갔다!");
        MESSAGE("상대 마자의 스피드가 올라갔다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9714 2-04 Tidy Up in doubles (right-side user): hazards both sides, four Substitutes in battler order, user's stats")
{
    u32 user;
    PARAMETRIZE { user = 0; } // playerRight
    PARAMETRIZE { user = 1; } // opponentRight
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SUBSTITUTE); MOVE(playerRight, MOVE_SUBSTITUTE); MOVE(opponentLeft, MOVE_SUBSTITUTE); MOVE(opponentRight, MOVE_SUBSTITUTE); }
        TURN { MOVE(playerLeft, MOVE_STEALTH_ROCK); MOVE(opponentLeft, MOVE_TOXIC_SPIKES); }
        if (user == 0)
            TURN { MOVE(playerRight, MOVE_TIDY_UP); }
        else
            TURN { MOVE(opponentRight, MOVE_TIDY_UP); }
    } SCENE {
        if (user == 0)
            MESSAGE("마자는 정리정돈을 썼다!");
        else
            MESSAGE("상대 마자는 정리정돈을 썼다!");
        MESSAGE("우리 편 발밑의 독압정이 사라졌다!");
        MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, playerLeft);
        MESSAGE("마자용의 대타는 사라져 버렸다...");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponentLeft);
        MESSAGE("상대 마자용의 대타는 사라져 버렸다...");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, playerRight);
        MESSAGE("마자의 대타는 사라져 버렸다...");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponentRight);
        MESSAGE("상대 마자의 대타는 사라져 버렸다...");
        MESSAGE("정리정돈 끝!");
        if (user == 0) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
            MESSAGE("마자의 공격이 올라갔다!");
            MESSAGE("마자의 스피드가 올라갔다!");
        } else {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
            MESSAGE("상대 마자의 공격이 올라갔다!");
            MESSAGE("상대 마자의 스피드가 올라갔다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNS9714 2-05 Rapid Spin removes each hazard from the user's side (player or foe user)")
{
    u32 hazard, user;
    PARAMETRIZE { hazard = 0; user = 0; }
    PARAMETRIZE { hazard = 1; user = 0; }
    PARAMETRIZE { hazard = 2; user = 0; }
    PARAMETRIZE { hazard = 3; user = 0; }
    PARAMETRIZE { hazard = 4; user = 0; }
    PARAMETRIZE { hazard = 0; user = 1; }
    PARAMETRIZE { hazard = 4; user = 1; }
    if (hazard == 4) {
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_PLAYER);
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_OPPONENT);
    }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (hazard == 0)
            TURN { MOVE(player, MOVE_SPIKES); MOVE(opponent, MOVE_SPIKES); }
        else if (hazard == 1)
            TURN { MOVE(player, MOVE_TOXIC_SPIKES); MOVE(opponent, MOVE_TOXIC_SPIKES); }
        else if (hazard == 2)
            TURN { MOVE(player, MOVE_STEALTH_ROCK); MOVE(opponent, MOVE_STEALTH_ROCK); }
        else if (hazard == 3)
            TURN { MOVE(player, MOVE_STICKY_WEB); MOVE(opponent, MOVE_STICKY_WEB); }
        else
            TURN {}
        if (user == 0)
            TURN { MOVE(player, MOVE_RAPID_SPIN); }
        else
            TURN { MOVE(opponent, MOVE_RAPID_SPIN); }
    } SCENE {
        if (user == 0)
            MESSAGE("마자용은 고속스핀을 썼다!");
        else
            MESSAGE("상대 마자는 고속스핀을 썼다!");
        if (user == 0) {
            if (hazard == 0)
                MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            else if (hazard == 1)
                MESSAGE("우리 편 발밑의 독압정이 사라졌다!");
            else if (hazard == 2)
                MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
            else if (hazard == 3)
                MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!");
            else
                MESSAGE("우리 편 주변의 강철이 사라졌다!");
            NONE_OF { MESSAGE("상대 발밑의 압정이 사라졌다!"); MESSAGE("상대 주변의 강철이 사라졌다!"); }
        } else {
            if (hazard == 0)
                MESSAGE("상대 발밑의 압정이 사라졌다!");
            else
                MESSAGE("상대 주변의 강철이 사라졌다!");
            NONE_OF { MESSAGE("우리 편 발밑의 압정이 사라졌다!"); MESSAGE("우리 편 주변의 강철이 사라졌다!"); }
        }
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9714 2-06 Rapid Spin frees from Wrap and Leech Seed, then removes two hazards (order and names)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPIKES); }
        TURN { MOVE(opponent, MOVE_STEALTH_ROCK); }
        TURN { MOVE(opponent, MOVE_LEECH_SEED); }
        TURN { MOVE(opponent, MOVE_WRAP); MOVE(player, MOVE_RAPID_SPIN); }
    } SCENE {
        MESSAGE("마자용은 고속스핀을 썼다!");
        MESSAGE("우리 편 발밑의 압정이 사라졌다!");
        MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9714 2-07 Mortal Spin (right-side user) removes the user's side hazards")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SPIKES); MOVE(opponentLeft, MOVE_STICKY_WEB); }
        if (user == 0)
            TURN { MOVE(playerRight, MOVE_MORTAL_SPIN); }
        else
            TURN { MOVE(opponentRight, MOVE_MORTAL_SPIN); }
    } SCENE {
        if (user == 0) {
            MESSAGE("마자는 킬러스핀을 썼다!");
            MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!");
            NONE_OF { MESSAGE("상대 발밑의 압정이 사라졌다!"); }
        } else {
            MESSAGE("상대 마자는 킬러스핀을 썼다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
            NONE_OF { MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!"); }
        }
    }
}

SINGLE_BATTLE_TEST("HNS9714 2-08 Substitute broken by damage: foe's and player's (fade animation and text)")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); MaxHP(100); HP(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); MaxHP(100); HP(100); }
    } WHEN {
        if (user == 0) {
            TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SEISMIC_TOSS); }
        } else {
            TURN { MOVE(player, MOVE_SUBSTITUTE); }
            TURN { MOVE(opponent, MOVE_SEISMIC_TOSS); }
        }
    } SCENE {
        if (user == 0) {
            MESSAGE("마자용은 지구던지기를 썼다!");
            MESSAGE("상대 마자를 대신하여 대타가 공격을 받았다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponent);
            MESSAGE("상대 마자의 대타는 사라져 버렸다...");
        } else {
            MESSAGE("상대 마자는 지구던지기를 썼다!");
            MESSAGE("마자용을 대신하여 대타가 공격을 받았다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, player);
            MESSAGE("마자용의 대타는 사라져 버렸다...");
        }
    }
}

DOUBLE_BATTLE_TEST("HNS9714 2-09 Spread move breaks two Substitutes in doubles (names in hit order)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); MaxHP(40); HP(40); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); MaxHP(40); HP(40); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SUBSTITUTE); MOVE(opponentRight, MOVE_SUBSTITUTE); MOVE(playerLeft, MOVE_SWIFT); }
    } SCENE {
        MESSAGE("마자용은 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponentLeft);
        MESSAGE("상대 마자용의 대타는 사라져 버렸다...");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponentRight);
        MESSAGE("상대 마자의 대타는 사라져 버렸다...");
    }
}

SINGLE_BATTLE_TEST("HNS9714 2-10 Multi-hit move breaks a Substitute on the second hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); MaxHP(40); HP(40); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_DOUBLE_KICK); }
    } SCENE {
        MESSAGE("마자용은 두번차기를 썼다!");
        MESSAGE("상대 마자를 대신하여 대타가 공격을 받았다!");
        SUB_HIT(opponent, subBreak: FALSE);
        MESSAGE("상대 마자를 대신하여 대타가 공격을 받았다!");
        SUB_HIT(opponent, subBreak: TRUE);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, opponent);
        MESSAGE("상대 마자의 대타는 사라져 버렸다...");
        MESSAGE("2번 맞았다!");
    }
}
