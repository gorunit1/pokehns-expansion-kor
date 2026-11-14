// 기술 떠올리기·필드 기술 가르침 공용 LearnMove()의 성공음(친구 요청 2026-11-14).
// 포팅 전 HnS 기술 떠올리기는 교체 학습에서도 PlayFanfare(MUS_LEVEL_UP)를 불렀다. upstream #8491 공통화 뒤
// 교체 분기(REPLACE_MOVE_1)만 성공음 없이 끝났다. 이름이 모두 "HnS learn move:"로 시작한다.
// 실제 UI 대신 호출을 세는 MoveLearnUI를 넣고, 기술 떠올리기가 쓰는 시작 상태(확인 질문)부터 상태 머신을 그대로 돌린다.
// 소리 자체(스피커 출력)는 확인하지 않는다 — 요청한 곡 번호·횟수·종료 대기만 확인한다.
#include "global.h"
#include "test/test.h"
#include "chooseboxmon.h"
#include "event_data.h"
#include "menu.h"
#include "move.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "sound.h"
#include "task.h"
#include "constants/songs.h"

static u32 sFanfareCount, sFanfareSong, sEndCount, sAnswerIndex, sPrintCount, sShowListCount, sPrintsAtFanfare;
static s8 sAnswers[4];

static void TestAsk(void) { }
static s32 TestWait(void) { return sAnswers[sAnswerIndex++]; }
static void TestPrint(const u8 *message) { sPrintCount++; }
static void TestFanfare(u32 songId)
{
    sFanfareCount++;
    sFanfareSong = songId;
    sPrintsAtFanfare = sPrintCount;
    PlayFanfare(songId); // 실제 팡파르 태스크를 만들어 종료 대기(IsFanfareTaskInactive)를 거치게 한다
}
static void TestShowMoveList(u8 taskId) { sShowListCount++; }
static void TestEnd(u8 taskId) { sEndCount++; }
static void TestTaskDummy(u8 taskId) { }

static const struct MoveLearnUI sTestUI =
{
    .askConfirmation = TestAsk,
    .waitConfirmation = TestWait,
    .printMessage = TestPrint,
    .playFanfare = TestFanfare,
    .showMoveList = TestShowMoveList,
    .endTask = TestEnd,
};

#define NEW_MOVE MOVE_THUNDERBOLT

// moveCount개의 기술을 아는 포켓몬을 0번 칸에 만들고, 기술 떠올리기 시작 상태의 태스크를 돌려준다
static u8 Setup(u32 moveCount, bool32 recoverPP)
{
    static const enum Move moves[MAX_MON_MOVES] = { MOVE_TACKLE, MOVE_GROWL, MOVE_SCRATCH, MOVE_LEER };
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    u8 taskId;

    ResetTasks();
    ZeroPlayerPartyMons();
    CreateMon(mon, SPECIES_WOBBUFFET, 50, 0, OTID_STRUCT_PLAYER_ID);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = i < moveCount ? moves[i] : MOVE_NONE;
        u32 pp = i < moveCount ? 1 : 0; // 남은 PP 1
        SetMonData(mon, MON_DATA_MOVE1 + i, &move);
        SetMonData(mon, MON_DATA_PP1 + i, &pp);
    }
    sFanfareCount = sFanfareSong = sEndCount = sAnswerIndex = sPrintCount = sShowListCount = sPrintsAtFanfare = 0;
    gSpecialVar_Result = 0xFF;
    taskId = CreateTask(TestTaskDummy, 1);
    gTasks[taskId].data[0] = GetLearnMoveStartAfterPromptState();
    gTasks[taskId].data[1] = 0;        // 파티 0번
    gTasks[taskId].data[2] = NEW_MOVE;
    gTasks[taskId].data[3] = recoverPP;
    return taskId;
}

// 팡파르 요청 또는 종료(endTask)까지 상태 머신을 돌린다. 돌린 횟수를 돌려준다.
static u32 StepUntilFanfareOrEnd(u8 taskId)
{
    u32 steps = 0;
    while (sFanfareCount == 0 && sEndCount == 0 && steps < 64)
    {
        gTasks[taskId].data[0] = LearnMove(&sTestUI, taskId);
        steps++;
    }
    return steps;
}

// 팡파르가 도는 동안에는 끝내지 않고, 팡파르 태스크가 끝난 뒤에 한 번 끝내는지
static void ExpectWaitsForFanfareThenEnds(u8 taskId)
{
    EXPECT(!IsFanfareTaskInactive());
    for (u32 i = 0; i < 5; i++)
        gTasks[taskId].data[0] = LearnMove(&sTestUI, taskId);
    EXPECT_EQ(sEndCount, 0);           // 재생 도중 종료하지 않음(#10818)
    EXPECT_EQ(sFanfareCount, 1);       // 기다리는 동안 다시 요청하지 않음
    RunTasks();                        // 헤드리스 테스트에서는 팡파르 태스크가 바로 끝남
    EXPECT(IsFanfareTaskInactive());
    gTasks[taskId].data[0] = LearnMove(&sTestUI, taskId);
    EXPECT_EQ(sEndCount, 1);
    EXPECT_EQ(sFanfareCount, 1);
}

TEST("HnS learn move: learning into an empty slot requests the level-up fanfare once and waits for it")
{
    u8 taskId = Setup(3, TRUE);
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    sAnswers[0] = 0; // 배우겠습니까? 예

    StepUntilFanfareOrEnd(taskId);
    EXPECT_EQ(sFanfareCount, 1);
    EXPECT_EQ(sFanfareSong, MUS_LEVEL_UP);
    EXPECT_EQ(sPrintsAtFanfare, 1);    // 학습 문장 다음에 요청
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE4), NEW_MOVE);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1), MOVE_TACKLE);
    ExpectWaitsForFanfareThenEnds(taskId);
    DestroyTask(taskId);
}

TEST("HnS learn move: replacing a move requests the level-up fanfare once and waits for it")
{
    u32 recoverPP;
    PARAMETRIZE { recoverPP = TRUE; }   // 필드 기술 가르침·스크립트 기술 떠올리기
    PARAMETRIZE { recoverPP = FALSE; }  // 요약 화면 기술 떠올리기(P_SUMMARY_MOVE_RELEARNER_FULL_PP FALSE일 때)
    u8 taskId = Setup(MAX_MON_MOVES, recoverPP);
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    u32 slot = GetMoveSlotToReplace();   // 요약 화면에서 고른 칸(테스트에서는 요약 화면을 열지 않으므로 저장된 값)
    ASSUME(slot < MAX_MON_MOVES);
    sAnswers[0] = 0; // 배우겠습니까? 예
    sAnswers[1] = 0; // 다른 기술을 잊게 하겠습니까? 예

    // 요약 화면 호출까지
    for (u32 i = 0; i < 16 && sShowListCount == 0; i++)
        gTasks[taskId].data[0] = LearnMove(&sTestUI, taskId);
    EXPECT_EQ(sShowListCount, 1);
    EXPECT_EQ(sFanfareCount, 0);
    // 요약 화면에서 칸을 고르고 돌아온 상태
    gTasks[taskId].data[0] = GetLearnMoveResumeAfterSummaryScreenState();
    u32 printsBefore = sPrintCount;
    StepUntilFanfareOrEnd(taskId);

    EXPECT_EQ(sFanfareCount, 1);       // 수정 전: 0인 채로 종료(sEndCount 1)
    EXPECT_EQ(sEndCount, 0);
    EXPECT_EQ(sFanfareSong, MUS_LEVEL_UP);
    EXPECT_EQ(sPrintsAtFanfare, printsBefore + 2); // "…잊었다" + "…배웠다" 다음에 요청
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + slot), NEW_MOVE);
    EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + slot), recoverPP ? GetMovePP(NEW_MOVE) : 1); // PP 규칙 그대로
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        if (i != slot)
            EXPECT_NE(GetMonData(mon, MON_DATA_MOVE1 + i), NEW_MOVE);
    }
    ExpectWaitsForFanfareThenEnds(taskId);
    DestroyTask(taskId);
}

TEST("HnS learn move: refusing at the prompt or giving up the replacement requests no fanfare and changes nothing")
{
    u32 giveUpAtReplace;
    PARAMETRIZE { giveUpAtReplace = FALSE; } // 배우겠습니까? 아니오
    PARAMETRIZE { giveUpAtReplace = TRUE; }  // 잊게 하겠습니까? 아니오 → 포기하겠습니까? 예
    u8 taskId = Setup(MAX_MON_MOVES, TRUE);
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    if (giveUpAtReplace)
    {
        sAnswers[0] = 0;
        sAnswers[1] = 1;
        sAnswers[2] = 0;
    }
    else
    {
        sAnswers[0] = MENU_B_PRESSED;
    }

    StepUntilFanfareOrEnd(taskId);
    EXPECT_EQ(sFanfareCount, 0);
    EXPECT_EQ(sEndCount, 1);
    EXPECT_EQ(sShowListCount, 0);
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1), MOVE_TACKLE);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE2), MOVE_GROWL);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE3), MOVE_SCRATCH);
    EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE4), MOVE_LEER);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + i), 1);
    DestroyTask(taskId);
}

TEST("HnS learn move: a second learn after the first requests its own single fanfare")
{
    u8 taskId = Setup(2, TRUE);
    sAnswers[0] = 0;
    StepUntilFanfareOrEnd(taskId);
    ExpectWaitsForFanfareThenEnds(taskId);

    // 같은 메뉴에서 다시 배우기(태스크·카운터를 새로)
    sFanfareCount = sEndCount = sAnswerIndex = sPrintCount = 0;
    gTasks[taskId].data[0] = GetLearnMoveStartAfterPromptState();
    gTasks[taskId].data[2] = MOVE_ICE_BEAM;
    sAnswers[0] = 0;
    StepUntilFanfareOrEnd(taskId);
    EXPECT_EQ(sFanfareCount, 1);
    EXPECT_EQ(sEndCount, 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE4), MOVE_ICE_BEAM);
    ExpectWaitsForFanfareThenEnds(taskId);
    DestroyTask(taskId);
}
