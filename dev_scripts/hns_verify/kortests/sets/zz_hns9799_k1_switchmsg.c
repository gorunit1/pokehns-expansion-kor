#include "global.h"
#include "test/battle.h"

// seq 171 #9799 scratch only (never committed): Korean versions of the 10 upstream tests in test/battle/battle_message.c
// (STRINGID_INTROMSG / INTROSENDOUT / RETURNMON / SWITCHINMON). Expected text = HnS AFTER seq171-9799.patch.
// "(same)": identical before the patch (chunk-171-174/base).
// "CHANGE EXPECTED (runner only)": the non-AI MULTI / TWO_VS_ONE / ONE_VS_TWO runner battles use flag sets no game battle
//   has (RECORDED_LINK together with INGAME_PARTNER or TWO_OPPONENTS); FAIL before the patch is expected.
// "CHANGE EXPECTED (AI fix)": IsSwitchinValid + BattlersShareParty; before the patch opponent B does not switch (FAIL).
// A space in MESSAGE matches one space or newline of the real text.

SINGLE_BATTLE_TEST("HNS9799 K1-01 Singles (same)")
{
    GIVEN {
        PLAYER(SPECIES_GASTLY);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GIRAFARIG);
    } WHEN {
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        MESSAGE("2가 승부를 걸어왔다!{PAUSE 49}");
        MESSAGE("2는 마자용을 내보냈다!");
        MESSAGE("가랏! 고오스!");
        MESSAGE("2는 마자용을 넣어버렸다!");
        MESSAGE("2는 키링키를 내보냈다!");
    }
}

AI_SINGLE_BATTLE_TEST("HNS9799 K1-02 AI Singles (same)")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_SMART_TRAINER);
        PLAYER(SPECIES_GASTLY);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE);  }
        OPPONENT(SPECIES_GIRAFARIG) { Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN { EXPECT_SWITCH(opponent, 1); }
    } SCENE {
        MESSAGE(AI_TRAINER_NAME "이 승부를 걸어왔다!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 내보냈다!");
        MESSAGE("가랏! 고오스!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키링키를 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9799 K1-03 Doubles (same)")
{
    GIVEN {
        PLAYER(SPECIES_GASTLY);
        PLAYER(SPECIES_HAUNTER);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_GIRAFARIG);
        OPPONENT(SPECIES_FARIGIRAF);
    } WHEN {
        TURN {
            SWITCH(opponentLeft, 3);
            SWITCH(opponentRight, 2);
        }
    } SCENE {
        MESSAGE("2가 승부를 걸어왔다!{PAUSE 49}");
        MESSAGE("2는 마자용과 마자를 내보냈다!");
        MESSAGE("가랏! 고오스와 고우스트!");
        MESSAGE("2는 마자용을 넣어버렸다!");
        MESSAGE("2는 키키링을 내보냈다!");
        MESSAGE("2는 마자를 넣어버렸다!");
        MESSAGE("2는 키링키를 내보냈다!");
    }
}

AI_DOUBLE_BATTLE_TEST("HNS9799 K1-04 AI Doubles (same)")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_SMART_TRAINER);
        PLAYER(SPECIES_GASTLY);
        PLAYER(SPECIES_HAUNTER);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE);  }
        OPPONENT(SPECIES_WYNAUT)    { Moves(MOVE_TACKLE);  }
        OPPONENT(SPECIES_GIRAFARIG) { Moves(MOVE_PSYCHIC); }
        OPPONENT(SPECIES_FARIGIRAF) { Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN {
            EXPECT_SWITCH(opponentLeft, 3);
            EXPECT_SWITCH(opponentRight, 2);
        }
    } SCENE {
        MESSAGE(AI_TRAINER_NAME "이 승부를 걸어왔다!");
        MESSAGE(AI_TRAINER_NAME "은 마자용과 마자를 내보냈다!");
        MESSAGE("가랏! 고오스와 고우스트!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키키링을 내보냈다!");
        MESSAGE(AI_TRAINER_NAME "은 마자를 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키링키를 내보냈다!");
    }
}

MULTI_BATTLE_TEST("HNS9799 K1-05 Multi CHANGE EXPECTED (runner only)")
{
    GIVEN {
        PLAYER(SPECIES_GASTLY);
        PARTNER(SPECIES_HAUNTER);
        PARTNER(SPECIES_GENGAR);
        OPPONENT_A(SPECIES_WOBBUFFET);
        OPPONENT_A(SPECIES_FARIGIRAF);
        OPPONENT_B(SPECIES_WYNAUT);
        OPPONENT_B(SPECIES_GIRAFARIG);
    } WHEN {
        TURN {
            SWITCH(playerRight, 1);
            SWITCH(opponentLeft, 1);
            SWITCH(opponentRight, 1);
        }
    } SCENE {
        MESSAGE("2와 4이 승부를 걸어왔다!");
        MESSAGE("2는 마자용을 내보냈다! 4은 마자를 내보냈다!");
        MESSAGE("3는 고우스트를 내보냈다! 가랏! 고오스!");
        MESSAGE("2는 마자용을 넣어버렸다!");
        MESSAGE("2는 키키링을 내보냈다!");
        MESSAGE("3는 고우스트를 넣어버렸다!");
        MESSAGE("3는 팬텀을 내보냈다!");
        MESSAGE("4는 마자를 넣어버렸다!");
        MESSAGE("4는 키링키를 내보냈다!"); // 0276c7310f: !! -> !
    }
}

AI_MULTI_BATTLE_TEST("HNS9799 K1-06 AI Multi CHANGE EXPECTED (AI fix)")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_SMART_TRAINER);
        PLAYER(SPECIES_GASTLY);
        PARTNER(SPECIES_HAUNTER);
        PARTNER(SPECIES_GENGAR);
        OPPONENT_A(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE);  }
        OPPONENT_A(SPECIES_FARIGIRAF) { Moves(MOVE_PSYCHIC); }
        OPPONENT_B(SPECIES_WYNAUT)    { Moves(MOVE_TACKLE);  }
        OPPONENT_B(SPECIES_GIRAFARIG) { Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN {
            SWITCH(playerRight, 1);
            EXPECT_SWITCH(opponentLeft, 1);
            EXPECT_SWITCH(opponentRight, 1);
        }
    } SCENE {
        MESSAGE(AI_TRAINER_NAME "과 " AI_TRAINER_2_NAME "이 승부를 걸어왔다!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 내보냈다! " AI_TRAINER_2_NAME "은 마자를 내보냈다!");
        MESSAGE(AI_PARTNER_NAME "은 고우스트를 내보냈다! 가랏! 고오스!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키키링을 내보냈다!");
        MESSAGE(AI_PARTNER_NAME "는 고우스트를 넣어버렸다!");
        MESSAGE(AI_PARTNER_NAME "는 팬텀을 내보냈다!");
        MESSAGE(AI_TRAINER_2_NAME "은 마자를 넣어버렸다!");
        MESSAGE(AI_TRAINER_2_NAME "은 키링키를 내보냈다!");
    }
}

TWO_VS_ONE_BATTLE_TEST("HNS9799 K1-07 2v1 CHANGE EXPECTED (runner only)")
{
    GIVEN {
        PLAYER(SPECIES_GASTLY);
        PARTNER(SPECIES_HAUNTER);
        PARTNER(SPECIES_GENGAR);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_GIRAFARIG);
        OPPONENT(SPECIES_FARIGIRAF);
    } WHEN {
        TURN {
            SWITCH(opponentLeft, 3);
            SWITCH(playerRight, 1);
            SWITCH(opponentRight, 2);
        }
    } SCENE {
        MESSAGE("2가 승부를 걸어왔다!");
        MESSAGE("2는 마자용과 마자를 내보냈다!");
        MESSAGE("3는 고우스트를 내보냈다! 가랏! 고오스!");
        MESSAGE("2는 마자용을 넣어버렸다!");
        MESSAGE("2는 키키링을 내보냈다!");
        MESSAGE("2는 마자를 넣어버렸다!");
        MESSAGE("2는 키링키를 내보냈다!");
        MESSAGE("3는 고우스트를 넣어버렸다!");
        MESSAGE("3는 팬텀을 내보냈다!");
    }
}

AI_TWO_VS_ONE_BATTLE_TEST("HNS9799 K1-08 AI 2v1 (same)")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_SMART_TRAINER);
        PLAYER(SPECIES_GASTLY);
        PARTNER(SPECIES_HAUNTER);
        PARTNER(SPECIES_GENGAR);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE);  }
        OPPONENT(SPECIES_WYNAUT)    { Moves(MOVE_TACKLE);  }
        OPPONENT(SPECIES_GIRAFARIG) { Moves(MOVE_PSYCHIC); }
        OPPONENT(SPECIES_FARIGIRAF) { Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN {
            EXPECT_SWITCH(opponentLeft, 3);
            SWITCH(playerRight, 1);
            EXPECT_SWITCH(opponentRight, 2);
        }
    } SCENE {
        MESSAGE(AI_TRAINER_NAME "이 승부를 걸어왔다!");
        MESSAGE(AI_TRAINER_NAME "은 마자용과 마자를 내보냈다!");
        MESSAGE(AI_PARTNER_NAME "은 고우스트를 내보냈다! 가랏! 고오스!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키키링을 내보냈다!");
        MESSAGE(AI_TRAINER_NAME "은 마자를 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키링키를 내보냈다!");
        MESSAGE(AI_PARTNER_NAME "는 고우스트를 넣어버렸다!");
        MESSAGE(AI_PARTNER_NAME "는 팬텀을 내보냈다!");
    }
}

ONE_VS_TWO_BATTLE_TEST("HNS9799 K1-09 1v2 CHANGE EXPECTED (runner only)")
{
    GIVEN {
        PLAYER(SPECIES_GASTLY);
        PLAYER(SPECIES_HAUNTER);
        OPPONENT_A(SPECIES_WOBBUFFET);
        OPPONENT_A(SPECIES_FARIGIRAF);
        OPPONENT_B(SPECIES_WYNAUT);
        OPPONENT_B(SPECIES_GIRAFARIG);
    } WHEN {
        TURN {
            SWITCH(opponentLeft, 1);
            SWITCH(opponentRight, 1);
        }
    } SCENE {
        MESSAGE("2와 4이 승부를 걸어왔다!");
        MESSAGE("2는 마자용을 내보냈다! 4은 마자를 내보냈다!");
        MESSAGE("가랏! 고오스와 고우스트!");
        MESSAGE("2는 마자용을 넣어버렸다!");
        MESSAGE("2는 키키링을 내보냈다!");
        MESSAGE("4는 마자를 넣어버렸다!");
        MESSAGE("4는 키링키를 내보냈다!"); // 0276c7310f: !! -> !
    }
}

AI_ONE_VS_TWO_BATTLE_TEST("HNS9799 K1-10 AI 1v2 CHANGE EXPECTED (AI fix)")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_SMART_TRAINER);
        PLAYER(SPECIES_GASTLY);
        PLAYER(SPECIES_HAUNTER);
        OPPONENT_A(SPECIES_WOBBUFFET)   { Moves(MOVE_TACKLE);  }
        OPPONENT_A(SPECIES_FARIGIRAF)   { Moves(MOVE_PSYCHIC); }
        OPPONENT_B(SPECIES_WYNAUT)      { Moves(MOVE_TACKLE);  }
        OPPONENT_B(SPECIES_GIRAFARIG)   { Moves(MOVE_PSYCHIC); }
    } WHEN {
        TURN {
            EXPECT_SWITCH(opponentLeft, 1);
            EXPECT_SWITCH(opponentRight, 1);
        }
    } SCENE {
        MESSAGE(AI_TRAINER_NAME "과 " AI_TRAINER_2_NAME "이 승부를 걸어왔다!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 내보냈다! " AI_TRAINER_2_NAME "은 마자를 내보냈다!");
        MESSAGE("가랏! 고오스와 고우스트!");
        MESSAGE(AI_TRAINER_NAME "은 마자용을 넣어버렸다!");
        MESSAGE(AI_TRAINER_NAME "은 키키링을 내보냈다!");
        MESSAGE(AI_TRAINER_2_NAME "은 마자를 넣어버렸다!");
        MESSAGE(AI_TRAINER_2_NAME "은 키링키를 내보냈다!");
    }
}
