#include "global.h"
#include "test/battle.h"

#define TMP70_MAKE_EGG(i) { bool32 t = TRUE; u8 f = 120; SetMonData(&gBattleTestRunnerState->data.recordedBattle.parties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG, &t); SetMonData(&gBattleTestRunnerState->data.recordedBattle.parties[B_TRAINER_PLAYER][i], MON_DATA_FRIENDSHIP, &f); }

WILD_BATTLE_TEST("TMP70 A egg-only party: the egg is sent out and can win, gaining EXP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(5); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_CATERPIE) { Level(2); HP(1); }
        TMP70_MAKE_EGG(0);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
    } THEN {
        Test_MgbaPrintf("TMP70A outcome=%d isEgg=%d level=%d exp=%d friendship(egg cycles)=%d hp=%d", gBattleOutcome,
            GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL),
            GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_EXP), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP),
            GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP));
    }
}

WILD_BATTLE_TEST("TMP70 B egg-only party: the egg can faint in battle")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(5); HP(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_CATERPIE) { Level(50); Moves(MOVE_TACKLE); }
        TMP70_MAKE_EGG(0);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
    } THEN {
        Test_MgbaPrintf("TMP70B outcome=%d isEgg=%d hp=%d friendship(egg cycles)=%d", gBattleOutcome,
            GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP),
            GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP));
    }
}
