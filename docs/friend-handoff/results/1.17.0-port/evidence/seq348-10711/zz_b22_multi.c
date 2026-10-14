#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "battle_util.h"
#include "event_data.h"
#include "test/test.h"
#include "constants/battle.h"
#include "constants/difficulty.h"

// B22 scratch: AreMultiPartiesFullTeams game branch (forced in this copy) for the field call in multi_do and battle calls.
#define B22_HALF 15
#define B22_FULL_A 1
#define B22_FULL_B 2
#define B22_TRAINER_MULTI (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)

static u8 sB22Dummy[16];

static bool32 B22Call(u32 flags, u16 oppA, u16 oppB, bool32 inBattle, u16 resultBefore, u16 *resultAfter)
{
    struct BattleStruct *savedStruct = gBattleStruct;
    u32 savedFlags = gBattleTypeFlags;
    u16 savedA = TRAINER_BATTLE_PARAM.opponentA, savedB = TRAINER_BATTLE_PARAM.opponentB;
    u16 savedResult = gSpecialVar_Result;
    u16 savedDifficulty = VarGet(B_VAR_DIFFICULTY);
    bool32 ret;

    VarSet(B_VAR_DIFFICULTY, DIFFICULTY_NORMAL); // test builds default to DIFFICULTY_EASY (var 0); HnS has no difficulty var
    gBattleTypeFlags = flags;
    TRAINER_BATTLE_PARAM.opponentA = oppA;
    TRAINER_BATTLE_PARAM.opponentB = oppB;
    gBattleStruct = inBattle ? (struct BattleStruct *)sB22Dummy : NULL;
    gSpecialVar_Result = resultBefore;
    ret = AreMultiPartiesFullTeams();
    *resultAfter = gSpecialVar_Result;

    gBattleStruct = savedStruct;
    gBattleTypeFlags = savedFlags;
    TRAINER_BATTLE_PARAM.opponentA = savedA;
    TRAINER_BATTLE_PARAM.opponentB = savedB;
    gSpecialVar_Result = savedResult;
    VarSet(B_VAR_DIFFICULTY, savedDifficulty);
    return ret;
}

TEST("B22 01 multi_do after a wild battle keeps a half-team multi (stale VAR_RESULT 1)")
{
    u16 r;
    EXPECT_EQ(B22Call(0, B22_HALF, B22_HALF, FALSE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 02 multi_do after a wild double battle keeps a half-team multi")
{
    u16 r;
    EXPECT_EQ(B22Call(BATTLE_TYPE_DOUBLE, B22_HALF, B22_HALF, FALSE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 03 multi_do after a trainer battle keeps a half-team multi")
{
    u16 r;
    EXPECT_EQ(B22Call(BATTLE_TYPE_TRAINER, B22_HALF, B22_HALF, FALSE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 04 multi_do after a wild battle with one half trainer (opponentB) keeps half")
{
    u16 r;
    EXPECT_EQ(B22Call(0, B22_FULL_A, B22_HALF, FALSE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 05 half-team multi in battle is half")
{
    u16 r;
    EXPECT_EQ(B22Call(B22_TRAINER_MULTI, B22_HALF, B22_HALF, TRUE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 06 multi_do with full trainers after a wild battle is full and sets the result")
{
    u16 r;
    EXPECT_EQ(B22Call(0, B22_FULL_A, B22_FULL_B, FALSE, FALSE, &r), TRUE);
    EXPECT_EQ(r, TRUE);
}

TEST("B22 07 multi_wild in the field is full and sets the result")
{
    u16 r;
    EXPECT_EQ(B22Call(0, TRAINER_NONE, TRAINER_NONE, FALSE, FALSE, &r), TRUE);
    EXPECT_EQ(r, TRUE);
}

TEST("B22 08 multi_wild in the field after a trainer battle is full")
{
    u16 r;
    EXPECT_EQ(B22Call(BATTLE_TYPE_TRAINER, TRAINER_NONE, TRAINER_NONE, FALSE, FALSE, &r), TRUE);
    EXPECT_EQ(r, TRUE);
}

TEST("B22 09 multi_wild in battle is full and sets the result")
{
    u16 r;
    EXPECT_EQ(B22Call(BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER, TRAINER_NONE, TRAINER_NONE, TRUE, FALSE, &r), TRUE);
    EXPECT_EQ(r, TRUE);
}

TEST("B22 10 frontier multi in battle does not read gTrainers (b43032bf03)")
{
    u16 r;
    EXPECT_EQ(B22Call(B22_TRAINER_MULTI | BATTLE_TYPE_FRONTIER, B22_HALF, B22_HALF, TRUE, FALSE, &r), TRUE);
    EXPECT_EQ(r, TRUE);
}

TEST("B22 11 tower link multi in battle is half")
{
    u16 r;
    EXPECT_EQ(B22Call(B22_TRAINER_MULTI | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TOWER_LINK_MULTI, B22_FULL_A, B22_FULL_B, TRUE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 12 link opponent is half")
{
    u16 r;
    EXPECT_EQ(B22Call(B22_TRAINER_MULTI | BATTLE_TYPE_LINK, TRAINER_LINK_OPPONENT, TRAINER_NONE, TRUE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}

TEST("B22 13 multi_do after a frontier battle (stale flags, no battle struct) still reads Half")
{
    u16 r;
    EXPECT_EQ(B22Call(B22_TRAINER_MULTI | BATTLE_TYPE_FRONTIER, B22_HALF, B22_HALF, FALSE, TRUE, &r), FALSE);
    EXPECT_EQ(r, FALSE);
}
