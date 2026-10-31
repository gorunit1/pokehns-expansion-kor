#include "global.h"
#include "test/battle.h"
#include "nuzlocke.h"
#include "event_data.h"
#include "pokemon_storage_system.h"

// TMP (not committed): last non-egg mon faints from recoil while KOing the wild mon (battle not lost)
WILD_BATTLE_TEST("TMP70B recoil KO of both last mons under Nuzlocke")
{
    GIVEN {
        gSaveBlock3Ptr->challengeSettings.tx_Challenges_Nuzlocke = TRUE;
        FlagSet(FLAG_SYS_POKEMON_GET);
        FlagSet(FLAG_START_NUZLOCKE);
        FlagClear(FLAG_END_NUZLOCKE);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Moves(MOVE_DOUBLE_EDGE); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_CATERPIE) { HP(1); }
        { bool32 t = TRUE; SetMonData(&gBattleTestRunnerState->data.recordedBattle.parties[B_TRAINER_PLAYER][1], MON_DATA_IS_EGG, &t); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_EDGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_EDGE, player);
    } THEN {
        Test_MgbaPrintf("TMP70B outcome=%d nuz=%d slot0 species=%d egg=%d hp=%d slot1 species=%d egg=%d alive=%d",
            gBattleOutcome, IsNuzlockeActive(),
            GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP),
            GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_IS_EGG),
            CountPartyAliveNonEggMonsExcept(PARTY_SIZE));
    }
}
