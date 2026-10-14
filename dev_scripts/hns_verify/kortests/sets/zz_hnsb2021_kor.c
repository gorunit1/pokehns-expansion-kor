#include "global.h"
#include "test/battle.h"

// HNSB2021: Korean regression set for the battle fixes of full-sync bundles 20-21 (2026-10-14, friend reply 2026-10-14c 4·6·7).
// Upstream tests of these fixes use English MESSAGE lines and fail in HnS (known limit), so the same situations are kept here
// with HnS's Korean strings. A space in a MESSAGE pattern also matches a line break (⏎) of the printed text.
// A: Acupressure targets (seq 327 #10636 7f874300 CancelerSetTargets, seq 332 #10645 aa31b7a4 SetFinalChosenTarget; HnS keeps
//    the upstream GetBattleMoveTarget "case TARGET_USER_OR_ALLY" hunk out - recheck 66: a called Acupressure targets the user).
//    Test runner paths: player and non-AI opponent targets come from the recorded battle (MOVE(..., target:)), not from the
//    player controller's HandleInputChooseMove; A1c is an AI doubles test and goes through SetFinalChosenTarget.
// B: spread moves called by Metronome / Sleep Talk (seq 337 #10661 9b10cb2a): TARGET_BOTH (Swift) skips the user's partner,
//    TARGET_FOES_AND_ALLY (Earthquake, Surf) still hits it.
// C: Shell Trap after a spread move and the shared turn-order function (seq 345 #10682 5ad78ec0, ChangeOrderTargetAfterAttacker
//    now takes the battler): Shell Trap order, After You, Pursuit.
// D: Symbiosis item is not used again as a resistance Berry by the same spread move (seq 334 #10653 d1c3e4e5).
// E: Copycat copies a move that fainted its target (seq 333 #10654 d45e2fe2).
// Bystanders that are not anchors keep the runner's default Celebrate; Celebrate is never used as an anchor.

static s32 HnsB2021_StageSum(struct BattlePokemon *mon)
{
    s32 sum = 0;
    for (u32 stat = STAT_ATK; stat < NUM_BATTLE_STATS; stat++)
        sum += mon->statStages[stat] - DEFAULT_STAT_STAGE;
    return sum;
}

DOUBLE_BATTLE_TEST("HNSB2021 A1a partner fainted last turn: Acupressure aimed at the fainted partner raises the user (#10636 CancelerSetTargets)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerRight); }
        TURN { MOVE(opponentLeft, MOVE_HARDEN); MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerRight); }
    } SCENE {
        MESSAGE("상대 마자용은 할퀴기를 썼다!");
        MESSAGE("마자는 쓰러졌다!");
        MESSAGE("상대 마자용은 단단해지기를 썼다!");
        MESSAGE("마자용은 경혈찌르기를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        ONE_OF {
            MESSAGE("마자용의 공격이 크게 올라갔다!");
            MESSAGE("마자용의 방어가 크게 올라갔다!");
            MESSAGE("마자용의 스피드가 크게 올라갔다!");
            MESSAGE("마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("마자용의 명중률이 크게 올라갔다!");
            MESSAGE("마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 2);
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A1b partner fainted last turn: Acupressure aimed at the user raises the user (target the fixed player controller sends)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerRight); }
        TURN { MOVE(opponentLeft, MOVE_HARDEN); MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerLeft); }
    } SCENE {
        MESSAGE("마자는 쓰러졌다!");
        MESSAGE("상대 마자용은 단단해지기를 썼다!");
        MESSAGE("마자용은 경혈찌르기를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        ONE_OF {
            MESSAGE("마자용의 공격이 크게 올라갔다!");
            MESSAGE("마자용의 방어가 크게 올라갔다!");
            MESSAGE("마자용의 스피드가 크게 올라갔다!");
            MESSAGE("마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("마자용의 명중률이 크게 올라갔다!");
            MESSAGE("마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 2);
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 0);
    }
}

AI_DOUBLE_BATTLE_TEST("HNSB2021 A1c AI: partner fainted, the AI's Acupressure raises the AI user (#10645 SetFinalChosenTarget)")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_ACUPRESSURE); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Speed(1); Moves(MOVE_ACUPRESSURE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentRight); MOVE(playerRight, MOVE_HARDEN); EXPECT_MOVE(opponentLeft, MOVE_ACUPRESSURE); }
        TURN { MOVE(playerLeft, MOVE_GROWL); MOVE(playerRight, MOVE_SWORDS_DANCE); EXPECT_MOVE(opponentLeft, MOVE_ACUPRESSURE); }
    } SCENE {
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("마자는 단단해지기를 썼다!");
        MESSAGE("상대 마자용은 경혈찌르기를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentLeft, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ONE_OF {
            MESSAGE("상대 마자용의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자용의 회피율이 크게 올라갔다!");
        }
        MESSAGE("마자는 칼춤을 썼다!");
        MESSAGE("상대 마자용은 경혈찌르기를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentLeft, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ONE_OF {
            MESSAGE("상대 마자용의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 4 - 1); // two Acupressure (+2 each), one Growl (-1)
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A2a partner faints earlier in the same turn: the user is raised, not But it failed (#10636)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerRight); MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerRight); }
    } SCENE {
        MESSAGE("상대 마자용은 할퀴기를 썼다!");
        HP_BAR(playerRight);
        MESSAGE("마자는 쓰러졌다!");
        MESSAGE("마자용은 경혈찌르기를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerLeft);
        ONE_OF {
            MESSAGE("마자용의 공격이 크게 올라갔다!");
            MESSAGE("마자용의 방어가 크게 올라갔다!");
            MESSAGE("마자용의 스피드가 크게 올라갔다!");
            MESSAGE("마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("마자용의 명중률이 크게 올라갔다!");
            MESSAGE("마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 2);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A2b foe partner faints earlier in the same turn: the foe user is raised, not But it failed (#10636)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Speed(2); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentRight); MOVE(opponentLeft, MOVE_ACUPRESSURE, target: opponentRight); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        HP_BAR(opponentRight);
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("상대 마자용은 경혈찌르기를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentLeft, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ONE_OF {
            MESSAGE("상대 마자용의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 2);
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A3 living partner targeted: the partner is raised, the user is not")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ACUPRESSURE, target: playerRight); MOVE(playerRight, MOVE_HARDEN); }
    } SCENE {
        MESSAGE("마자용은 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft, playerRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        ONE_OF {
            MESSAGE("마자의 공격이 크게 올라갔다!");
            MESSAGE("마자의 방어가 크게 올라갔다!");
            MESSAGE("마자의 스피드가 크게 올라갔다!");
            MESSAGE("마자의 특수공격이 크게 올라갔다!");
            MESSAGE("마자의 특수방어가 크게 올라갔다!");
            MESSAGE("마자의 명중률이 크게 올라갔다!");
            MESSAGE("마자의 회피율이 크게 올라갔다!");
        }
        MESSAGE("마자는 단단해지기를 썼다!");
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 2 + 1); // Acupressure +2, Harden +1
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A4a foe-left calls Acupressure by Metronome, its partner alive: the foe-left user is raised, no player mon (recheck 66)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_ACUPRESSURE)); }
    } SCENE {
        MESSAGE("상대 마자용은 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, opponentLeft);
        MESSAGE("손가락을 흔들었더니 경혈찌르기 나왔다!");
        MESSAGE("상대 마자용은 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentLeft, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ONE_OF {
            MESSAGE("상대 마자용의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 2);
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 0);
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A4b foe-right calls Acupressure by Metronome, its partner alive: the foe-right user is raised, no player mon (recheck 66)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_ACUPRESSURE)); }
    } SCENE {
        MESSAGE("상대 마자는 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, opponentRight);
        MESSAGE("손가락을 흔들었더니 경혈찌르기 나왔다!");
        MESSAGE("상대 마자는 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentRight, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        ONE_OF {
            MESSAGE("상대 마자의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 2);
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 0);
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A4c foe-left calls Acupressure by Sleep Talk, its partner alive: the foe-left user is raised, no player mon (recheck 66)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_ACUPRESSURE); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SLEEP_TALK); }
    } SCENE {
        MESSAGE("상대 마자용은 잠꼬대를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLEEP_TALK, opponentLeft);
        MESSAGE("상대 마자용은 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentLeft, opponentLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
        ONE_OF {
            MESSAGE("상대 마자용의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자용의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자용의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자용의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 2);
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 0);
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A4d foe-right calls Acupressure by Sleep Talk, its partner alive: the foe-right user is raised, no player mon (recheck 66)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_ACUPRESSURE); }
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_SLEEP_TALK); }
    } SCENE {
        MESSAGE("상대 마자는 잠꼬대를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLEEP_TALK, opponentRight);
        MESSAGE("상대 마자는 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, opponentRight, opponentRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentRight);
        ONE_OF {
            MESSAGE("상대 마자의 공격이 크게 올라갔다!");
            MESSAGE("상대 마자의 방어가 크게 올라갔다!");
            MESSAGE("상대 마자의 스피드가 크게 올라갔다!");
            MESSAGE("상대 마자의 특수공격이 크게 올라갔다!");
            MESSAGE("상대 마자의 특수방어가 크게 올라갔다!");
            MESSAGE("상대 마자의 명중률이 크게 올라갔다!");
            MESSAGE("상대 마자의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 2);
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 0);
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 A4e player-right calls Acupressure by Metronome: the user is raised, not the player-left partner (recheck 66)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_ACUPRESSURE)); }
    } SCENE {
        MESSAGE("마자는 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, playerRight);
        MESSAGE("손가락을 흔들었더니 경혈찌르기 나왔다!");
        MESSAGE("마자는 경혈찌르기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerRight, playerRight);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, playerRight);
        ONE_OF {
            MESSAGE("마자의 공격이 크게 올라갔다!");
            MESSAGE("마자의 방어가 크게 올라갔다!");
            MESSAGE("마자의 스피드가 크게 올라갔다!");
            MESSAGE("마자의 특수공격이 크게 올라갔다!");
            MESSAGE("마자의 특수방어가 크게 올라갔다!");
            MESSAGE("마자의 명중률이 크게 올라갔다!");
            MESSAGE("마자의 회피율이 크게 올라갔다!");
        }
    } THEN {
        EXPECT_EQ(HnsB2021_StageSum(playerRight), 2);
        EXPECT_EQ(HnsB2021_StageSum(playerLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(opponentLeft), 0);
        EXPECT_EQ(HnsB2021_StageSum(opponentRight), 0);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B1 player-left Metronome calls Swift: both foes hit, the partner is not (#10661)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_SWIFT)); }
    } SCENE {
        MESSAGE("마자용은 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, playerLeft);
        MESSAGE("손가락을 흔들었더니 스피드스타 나왔다!");
        MESSAGE("마자용은 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, playerLeft);
        HP_BAR(opponentLeft);
        NOT HP_BAR(playerRight);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
        EXPECT_EQ(playerRight->hp, playerRight->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B2 player-right Metronome calls Swift: both foes hit, the partner is not (#10661)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_SWIFT)); }
    } SCENE {
        MESSAGE("마자는 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, playerRight);
        MESSAGE("손가락을 흔들었더니 스피드스타 나왔다!");
        MESSAGE("마자는 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, playerRight);
        NOT HP_BAR(playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerRight->hp, playerRight->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
        EXPECT_EQ(playerLeft->hp, playerLeft->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B3 foe-left Metronome calls Swift: both player mons hit, its partner is not (#10661)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_SWIFT)); }
    } SCENE {
        MESSAGE("상대 마자용은 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, opponentLeft);
        MESSAGE("손가락을 흔들었더니 스피드스타 나왔다!");
        MESSAGE("상대 마자용은 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, opponentLeft);
        HP_BAR(playerLeft);
        HP_BAR(playerRight);
        NOT HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(playerRight->hp, playerRight->maxHP);
        EXPECT_EQ(opponentRight->hp, opponentRight->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B4 foe-right Metronome calls Swift: both player mons hit, its partner is not (#10661)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_SWIFT)); }
    } SCENE {
        MESSAGE("상대 마자는 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, opponentRight);
        MESSAGE("손가락을 흔들었더니 스피드스타 나왔다!");
        MESSAGE("상대 마자는 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, opponentRight);
        HP_BAR(playerLeft);
        NOT HP_BAR(opponentLeft);
        HP_BAR(playerRight);
    } THEN {
        EXPECT_EQ(opponentRight->hp, opponentRight->maxHP);
        EXPECT_LT(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(playerRight->hp, playerRight->maxHP);
        EXPECT_EQ(opponentLeft->hp, opponentLeft->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B5 player-left Sleep Talk calls Swift: both foes hit, the partner is not (#10661)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_SWIFT); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SLEEP_TALK); }
    } SCENE {
        MESSAGE("마자용은 잠꼬대를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLEEP_TALK, playerLeft);
        MESSAGE("마자용은 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, playerLeft);
        HP_BAR(opponentLeft);
        NOT HP_BAR(playerRight);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
        EXPECT_EQ(playerRight->hp, playerRight->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B6 foe-right Sleep Talk calls Swift: both player mons hit, its partner is not (#10661)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_SWIFT); }
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_SLEEP_TALK); }
    } SCENE {
        MESSAGE("상대 마자는 잠꼬대를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLEEP_TALK, opponentRight);
        MESSAGE("상대 마자는 스피드스타를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, opponentRight);
        HP_BAR(playerLeft);
        NOT HP_BAR(opponentLeft);
        HP_BAR(playerRight);
    } THEN {
        EXPECT_EQ(opponentRight->hp, opponentRight->maxHP);
        EXPECT_LT(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(playerRight->hp, playerRight->maxHP);
        EXPECT_EQ(opponentLeft->hp, opponentLeft->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B7 player-left Metronome calls Earthquake: both foes and the partner are hit")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_EARTHQUAKE)); }
    } SCENE {
        MESSAGE("마자용은 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, playerLeft);
        MESSAGE("손가락을 흔들었더니 지진 나왔다!");
        MESSAGE("마자용은 지진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(playerRight);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
        EXPECT_LT(playerRight->hp, playerRight->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B8 foe-right Metronome calls Earthquake: both player mons and its partner are hit")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_EARTHQUAKE)); }
    } SCENE {
        MESSAGE("상대 마자는 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, opponentRight);
        MESSAGE("손가락을 흔들었더니 지진 나왔다!");
        MESSAGE("상대 마자는 지진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponentRight);
        HP_BAR(playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(playerRight);
    } THEN {
        EXPECT_EQ(opponentRight->hp, opponentRight->maxHP);
        EXPECT_LT(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(playerRight->hp, playerRight->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B9 player-left Metronome calls Surf: both foes and the partner are hit")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SURF) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_METRONOME, WITH_RNG(RNG_METRONOME, MOVE_SURF)); }
    } SCENE {
        MESSAGE("마자용은 손가락흔들기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_METRONOME, playerLeft);
        MESSAGE("손가락을 흔들었더니 파도타기 나왔다!");
        MESSAGE("마자용은 파도타기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SURF, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(playerRight);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerLeft->hp, playerLeft->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
        EXPECT_LT(playerRight->hp, playerRight->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 B10 player-right Sleep Talk calls Earthquake: both foes and the partner are hit")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_EARTHQUAKE); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_SLEEP_TALK); }
    } SCENE {
        MESSAGE("마자는 잠꼬대를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLEEP_TALK, playerRight);
        MESSAGE("마자는 지진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerRight);
        HP_BAR(playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    } THEN {
        EXPECT_EQ(playerRight->hp, playerRight->maxHP);
        EXPECT_LT(opponentLeft->hp, opponentLeft->maxHP);
        EXPECT_LT(opponentRight->hp, opponentRight->maxHP);
        EXPECT_LT(playerLeft->hp, playerLeft->maxHP);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 C1 Shell Trap user hit by a physical spread move moves right after that attack (#10682)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        ASSUME(GetMoveCategory(MOVE_EARTHQUAKE) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(3); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_EARTHQUAKE); MOVE(playerRight, MOVE_SHELL_TRAP); MOVE(playerLeft, MOVE_HARDEN); MOVE(opponentRight, MOVE_SWORDS_DANCE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SHELL_TRAP_SETUP, playerRight);
        MESSAGE("마자는 트랩셸을 설치했다!");
        MESSAGE("상대 마자용은 지진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponentLeft);
        HP_BAR(playerRight);
        NONE_OF {
            MESSAGE("마자용은 단단해지기를 썼다!");
            MESSAGE("상대 마자는 칼춤을 썼다!");
        }
        MESSAGE("마자는 트랩셸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, playerRight);
        MESSAGE("마자용은 단단해지기를 썼다!");
        MESSAGE("상대 마자는 칼춤을 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 C2 two Shell Trap users hit by one spread move activate in speed order (faster right user first, #10682)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SHELL_TRAP); MOVE(playerRight, MOVE_SHELL_TRAP); MOVE(opponentLeft, MOVE_EARTHQUAKE); MOVE(opponentRight, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("상대 마자용은 지진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponentLeft);
        NONE_OF {
            MESSAGE("마자용은 트랩셸을 썼다!");
            MESSAGE("상대 마자는 칼춤을 썼다!");
        }
        MESSAGE("마자는 트랩셸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, playerRight);
        NOT MESSAGE("상대 마자는 칼춤을 썼다!");
        MESSAGE("마자용은 트랩셸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, playerLeft);
        MESSAGE("상대 마자는 칼춤을 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 C3 two Shell Trap users hit by one spread move activate in speed order (faster left user first, #10682)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SHELL_TRAP); MOVE(playerRight, MOVE_SHELL_TRAP); MOVE(opponentLeft, MOVE_EARTHQUAKE); MOVE(opponentRight, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("상대 마자용은 지진을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponentLeft);
        NONE_OF {
            MESSAGE("마자는 트랩셸을 썼다!");
            MESSAGE("상대 마자는 칼춤을 썼다!");
        }
        MESSAGE("마자용은 트랩셸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, playerLeft);
        NOT MESSAGE("상대 마자는 칼춤을 썼다!");
        MESSAGE("마자는 트랩셸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, playerRight);
        MESSAGE("상대 마자는 칼춤을 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 C4 Shell Trap user hit by a single-target physical move moves right after it")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(3); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerRight); MOVE(playerRight, MOVE_SHELL_TRAP); MOVE(playerLeft, MOVE_HARDEN); MOVE(opponentRight, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자는 트랩셸을 설치했다!");
        MESSAGE("상대 마자용은 할퀴기를 썼다!");
        HP_BAR(playerRight);
        NOT MESSAGE("마자용은 단단해지기를 썼다!");
        MESSAGE("마자는 트랩셸을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_TRAP, playerRight);
        MESSAGE("마자용은 단단해지기를 썼다!");
        MESSAGE("상대 마자는 칼춤을 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 C5 After You: the slowest target moves right after the user (same turn-order function as #10682)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_AFTER_YOU, target: playerRight); MOVE(playerRight, MOVE_HARDEN); MOVE(opponentLeft, MOVE_GROWL); MOVE(opponentRight, MOVE_SWORDS_DANCE); }
    } SCENE {
        MESSAGE("마자용은 당신먼저를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AFTER_YOU, playerLeft, playerRight);
        MESSAGE("마자는 배려를 받아들이기로 했다!");
        NOT MESSAGE("상대 마자용은 울음소리를 썼다!");
        MESSAGE("마자는 단단해지기를 썼다!");
        MESSAGE("상대 마자용은 울음소리를 썼다!");
        MESSAGE("상대 마자는 칼춤을 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 C6 two foes Pursuit a switching player mon from fastest to slowest (same turn-order function as #10682)")
{
    u32 speedLeft, speedRight;
    PARAMETRIZE { speedLeft = 5; speedRight = 3; }
    PARAMETRIZE { speedLeft = 3; speedRight = 5; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Speed(4); }
        PLAYER(SPECIES_GRIMER) { Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(speedLeft); }
        OPPONENT(SPECIES_WYNAUT) { Speed(speedRight); }
    } WHEN {
        TURN { SWITCH(playerLeft, 2); MOVE(playerRight, MOVE_HARDEN); MOVE(opponentLeft, MOVE_PURSUIT, target: playerLeft); MOVE(opponentRight, MOVE_PURSUIT, target: playerLeft); }
    } SCENE {
        if (speedLeft > speedRight) {
            MESSAGE("상대 마자용은 따라가때리기를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_PURSUIT, opponentLeft);
            HP_BAR(playerLeft);
            MESSAGE("상대 마자는 따라가때리기를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_PURSUIT, opponentRight);
            HP_BAR(playerLeft);
        } else {
            MESSAGE("상대 마자는 따라가때리기를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_PURSUIT, opponentRight);
            HP_BAR(playerLeft);
            MESSAGE("상대 마자용은 따라가때리기를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_PURSUIT, opponentLeft);
            HP_BAR(playerLeft);
        }
        MESSAGE("마자는 단단해지기를 썼다!");
        NONE_OF {
            MESSAGE("상대 마자용은 따라가때리기를 썼다!");
            MESSAGE("상대 마자는 따라가때리기를 썼다!");
        }
    } THEN {
        EXPECT_EQ(playerLeft->species, SPECIES_GRIMER);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 D1 Symbiosis item received during a spread move is not used again as a resistance Berry (#10653)")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        ASSUME(GetItemHoldEffect(ITEM_SHUCA_BERRY) == HOLD_EFFECT_RESIST_BERRY);
        ASSUME(GetItemHoldEffectParam(ITEM_SHUCA_BERRY) == TYPE_GROUND);
        PLAYER(SPECIES_ORANGURU) { Ability(ABILITY_SYMBIOSIS); Item(ITEM_POTION); }
        PLAYER(SPECIES_PIKACHU) { Item(ITEM_SHUCA_BERRY); }
        OPPONENT(SPECIES_PIKACHU) { Item(ITEM_SHUCA_BERRY); }
        OPPONENT(SPECIES_ORANGURU) { Ability(ABILITY_SYMBIOSIS); Item(ITEM_POTION); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE); }
    } SCENE {
        MESSAGE("하랑우탄은 지진을 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponentLeft);
        MESSAGE("상대 피카츄는 입는 데미지를 슈캐열매가 약하게 했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, playerRight);
        MESSAGE("피카츄는 입는 데미지를 슈캐열매가 약하게 했다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(playerRight);
        HP_BAR(opponentRight);
        ABILITY_POPUP(opponentRight, ABILITY_SYMBIOSIS);
        MESSAGE("상대 피카츄는 상대 하랑우탄으로부터 상처약을 받았다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponentLeft);
            MESSAGE("상대 피카츄는 입는 데미지를 상처약이 약하게 했다!");
            MESSAGE("상대 피카츄는 입는 데미지를 슈캐열매가 약하게 했다!");
        }
        ABILITY_POPUP(playerLeft, ABILITY_SYMBIOSIS);
        MESSAGE("피카츄는 하랑우탄으로부터 상처약을 받았다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, playerRight);
            MESSAGE("피카츄는 입는 데미지를 상처약이 약하게 했다!");
            MESSAGE("피카츄는 입는 데미지를 슈캐열매가 약하게 했다!");
        }
    } THEN {
        EXPECT_EQ(playerLeft->item, ITEM_NONE);
        EXPECT_EQ(playerRight->item, ITEM_POTION);
        EXPECT_EQ(opponentLeft->item, ITEM_POTION);
        EXPECT_EQ(opponentRight->item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 E1 Copycat copies the player's move that fainted its foe target (#10654)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); MOVE(playerRight, MOVE_COPYCAT, target: opponentRight); MOVE(opponentRight, MOVE_HARDEN); }
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        HP_BAR(opponentLeft);
        MESSAGE("상대 마자용은 쓰러졌다!");
        MESSAGE("마자는 흉내쟁이를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        MESSAGE("마자는 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerRight, opponentRight);
        HP_BAR(opponentRight);
        MESSAGE("상대 마자는 단단해지기를 썼다!");
    }
}

DOUBLE_BATTLE_TEST("HNSB2021 E2 Copycat copies the foe's move that fainted the user's partner (#10654)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerRight); MOVE(playerLeft, MOVE_COPYCAT, target: opponentLeft); MOVE(opponentRight, MOVE_HARDEN); }
    } SCENE {
        MESSAGE("상대 마자용은 할퀴기를 썼다!");
        HP_BAR(playerRight);
        MESSAGE("마자는 쓰러졌다!");
        MESSAGE("마자용은 흉내쟁이를 썼다!");
        NOT MESSAGE("그러나 실패하고 말았다!");
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        ONE_OF { // the called move picks its own foe target (TARGET_DEPENDS)
            HP_BAR(opponentLeft);
            HP_BAR(opponentRight);
        }
        MESSAGE("상대 마자는 단단해지기를 썼다!");
    }
}
