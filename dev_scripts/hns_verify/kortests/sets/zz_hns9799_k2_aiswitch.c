#include "global.h"
#include "test/battle.h"

// seq 171 #9799 scratch only (never committed): IsSwitchinValid "override switch" branch.
// Both AI battlers pick a specific switch-in (FindMonThatAbsorbsOpponentsMove -> SetSwitchinAndSwitch(battler, monIndex))
// in the same frame, before either action is chosen, so only IsSwitchinValid can stop the second one.
// K2-01: one trainer (shared party): only opponentLeft may switch into Jolteon; opponentRight must attack.
//        pre (HnS) and HnS-adapted patch compare AI_monToSwitchIntoId[battler]; upstream compares mostSuitableMonId.
// K2-02: two trainers (own parties, 1v2): both may switch into their own slot 1 (pre FAILs: no BattlersShareParty).

AI_DOUBLE_BATTLE_TEST("HNS9799 K2-01 one trainer: both foes absorb-switch into the same mon, only the first does")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_SMART_SWITCHING | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_ZIGZAGOON) { Moves(MOVE_SHOCK_WAVE, MOVE_EARTHQUAKE); }
        PLAYER(SPECIES_ZIGZAGOON) { Moves(MOVE_SHOCK_WAVE, MOVE_EARTHQUAKE); }
        OPPONENT(SPECIES_ZIGZAGOON) { Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_ZIGZAGOON) { Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_GLISCOR) { Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_GLISCOR) { Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SHOCK_WAVE, target: opponentLeft);
            MOVE(playerRight, MOVE_SHOCK_WAVE, target: opponentRight);
            EXPECT_MOVE(opponentLeft, MOVE_SCRATCH);
            EXPECT_MOVE(opponentRight, MOVE_SCRATCH);
        }
        TURN {
            MOVE(playerLeft, MOVE_SHOCK_WAVE, target: opponentLeft);
            MOVE(playerRight, MOVE_SHOCK_WAVE, target: opponentRight);
            EXPECT_SWITCH(opponentLeft, 2);
            EXPECT_MOVE(opponentRight, MOVE_SCRATCH);
        }
    }
}

// 2026-10-07: player MaxHP 999 so neither foe wins the 1v1 (#9847 smart AI stays in when it wins the 1v1); keeps testing the #9799 BattlersShareParty fix.
AI_ONE_VS_TWO_BATTLE_TEST("HNS9799 K2-02 two trainers: both foes absorb-switch into their own slot 1")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_SMART_SWITCHING | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_ZIGZAGOON) { Moves(MOVE_SHOCK_WAVE); MaxHP(999); HP(999); }
        PLAYER(SPECIES_ZIGZAGOON) { Moves(MOVE_SHOCK_WAVE); MaxHP(999); HP(999); }
        OPPONENT_A(SPECIES_ZIGZAGOON) { Moves(MOVE_SCRATCH); }
        OPPONENT_A(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); Moves(MOVE_SCRATCH); }
        OPPONENT_B(SPECIES_ZIGZAGOON) { Moves(MOVE_SCRATCH); }
        OPPONENT_B(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SHOCK_WAVE, target: opponentLeft);
            MOVE(playerRight, MOVE_SHOCK_WAVE, target: opponentRight);
            EXPECT_MOVE(opponentLeft, MOVE_SCRATCH);
            EXPECT_MOVE(opponentRight, MOVE_SCRATCH);
        }
        TURN {
            MOVE(playerLeft, MOVE_SHOCK_WAVE, target: opponentLeft);
            MOVE(playerRight, MOVE_SHOCK_WAVE, target: opponentRight);
            EXPECT_SWITCH(opponentLeft, 1);
            EXPECT_SWITCH(opponentRight, 1);
        }
    }
}
