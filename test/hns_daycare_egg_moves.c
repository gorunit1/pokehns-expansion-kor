// HnS 키우미집 알기술 전수 회귀 테스트 — 재확인 79 친구 결정(2026-10-24).
// Gen 8: 같은 종끼리만(흉내허브 무관). Gen 9+: 배우는 쪽이 흉내허브를 지닐 때만(종 무관, 허브는 그대로).
// 맡길 때마다 검사하고, 가르치는 쪽의 기술칸 순서대로 빈 칸만 채운다.
// 이름이 모두 "HnS egg move transfer:"로 시작한다(TESTS='HnS egg move transfer'). 언어와 무관하다.
// 학습 데이터는 새 게임 기본값(현대 기술 = 챌린지 tx_Mode_Modern_Moves 1) 기준이고, 조합은 ASSUME로 확인한다.
#include "global.h"
#include "config_changes.h"
#include "daycare.h"
#include "event_data.h"
#include "move.h"
#include "pokemon.h"
#include "test/overworld_script.h"
#include "test/test.h"

static bool32 LearnsByLevelUp(enum Species species, enum Move move)
{
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(species);
    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
    {
        if (learnset[i].move == move)
            return TRUE;
    }
    return FALSE;
}

static void ResetDaycareTest(u32 gen)
{
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;
    ZeroPlayerPartyMons();
    memset(&gSaveBlock1Ptr->daycare, 0, sizeof(gSaveBlock1Ptr->daycare));
    SetConfig(CONFIG_EGG_MOVE_TRANSFER, gen);
}

// Deposits party slot 0 (the party compacts after each deposit).
static void DepositFirstPartyMon(void)
{
    gSpecialVar_0x8004 = 0;
    StoreSelectedPokemonInDaycare();
}

static enum Move DaycareMove(u32 slot, u32 moveSlot)
{
    return GetBoxMonData(&gSaveBlock1Ptr->daycare.mons[slot].mon, MON_DATA_MOVE1 + moveSlot);
}

static enum Species DaycareSpecies(u32 slot)
{
    return GetBoxMonData(&gSaveBlock1Ptr->daycare.mons[slot].mon, MON_DATA_SPECIES);
}

#define ASSUME_BULBASAUR_EGG_MOVES()                                \
    gSaveBlock3Ptr->challengeSettings.tx_Mode_Modern_Moves = 1;     \
    ASSUME(SpeciesHasEggMove(SPECIES_BULBASAUR, MOVE_CURSE));        \
    ASSUME(SpeciesHasEggMove(SPECIES_BULBASAUR, MOVE_INGRAIN));      \
    ASSUME(SpeciesHasEggMove(SPECIES_BULBASAUR, MOVE_NATURE_POWER)); \
    ASSUME(SpeciesHasEggMove(SPECIES_BULBASAUR, MOVE_AMNESIA));      \
    ASSUME(!SpeciesHasEggMove(SPECIES_BULBASAUR, MOVE_TACKLE));      \
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_TACKLE));         \
    ASSUME(LearnsByLevelUp(SPECIES_SLOWPOKE, MOVE_CURSE));           \
    ASSUME(LearnsByLevelUp(SPECIES_SLOWPOKE, MOVE_AMNESIA));         \
    ASSUME(GET_BASE_SPECIES_ID(SPECIES_SLOWPOKE) != GET_BASE_SPECIES_ID(SPECIES_BULBASAUR))

// 1. Gen 8: same species without a Mirror Herb learns; another species does not, even with the herb.
TEST("HnS egg move transfer: Gen 8 shares only between the same species")
{
    enum Species teacher = SPECIES_NONE;
    enum Item learnerItem = ITEM_NONE;
    bool32 learns = FALSE;
    PARAMETRIZE { teacher = SPECIES_BULBASAUR; learnerItem = ITEM_NONE;        learns = TRUE; }
    PARAMETRIZE { teacher = SPECIES_BULBASAUR; learnerItem = ITEM_MIRROR_HERB; learns = TRUE; }
    PARAMETRIZE { teacher = SPECIES_SLOWPOKE;  learnerItem = ITEM_NONE;        learns = FALSE; }
    PARAMETRIZE { teacher = SPECIES_SLOWPOKE;  learnerItem = ITEM_MIRROR_HERB; learns = FALSE; }
    ASSUME_BULBASAUR_EGG_MOVES();

    ResetDaycareTest(GEN_8);
    gSpecialVar_0x8000 = learnerItem;
    gSpecialVar_0x8001 = teacher;
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=VAR_0x8000;
        givemon VAR_0x8001, 30, move1=MOVE_CURSE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE;
    );
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    EXPECT_EQ(DaycareMove(0, 1), learns ? MOVE_CURSE : MOVE_NONE);
}

// 2. Gen 9: only a receiver holding a Mirror Herb learns, for the same species and for another species.
TEST("HnS egg move transfer: Gen 9 needs the receiver's Mirror Herb")
{
    enum Species teacher = SPECIES_NONE;
    enum Item learnerItem = ITEM_NONE, teacherItem = ITEM_NONE;
    bool32 learns = FALSE;
    PARAMETRIZE { teacher = SPECIES_BULBASAUR; learnerItem = ITEM_NONE;        teacherItem = ITEM_NONE;        learns = FALSE; }
    PARAMETRIZE { teacher = SPECIES_BULBASAUR; learnerItem = ITEM_MIRROR_HERB; teacherItem = ITEM_NONE;        learns = TRUE; }
    PARAMETRIZE { teacher = SPECIES_BULBASAUR; learnerItem = ITEM_NONE;        teacherItem = ITEM_MIRROR_HERB; learns = FALSE; }
    PARAMETRIZE { teacher = SPECIES_SLOWPOKE;  learnerItem = ITEM_NONE;        teacherItem = ITEM_NONE;        learns = FALSE; }
    PARAMETRIZE { teacher = SPECIES_SLOWPOKE;  learnerItem = ITEM_MIRROR_HERB; teacherItem = ITEM_NONE;        learns = TRUE; }
    PARAMETRIZE { teacher = SPECIES_SLOWPOKE;  learnerItem = ITEM_NONE;        teacherItem = ITEM_MIRROR_HERB; learns = FALSE; }
    ASSUME_BULBASAUR_EGG_MOVES();

    ResetDaycareTest(GEN_9);
    gSpecialVar_0x8000 = learnerItem;
    gSpecialVar_0x8001 = teacher;
    gSpecialVar_0x8002 = teacherItem;
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=VAR_0x8000;
        givemon VAR_0x8001, 30, move1=MOVE_CURSE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=VAR_0x8002;
    );
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    EXPECT_EQ(DaycareMove(0, 1), learns ? MOVE_CURSE : MOVE_NONE);
    // The teacher (slot 1) never learns from the Bulbasaur, whose moves are not its Egg Moves or it has no herb.
    EXPECT_EQ(DaycareMove(1, 0), MOVE_CURSE);
    EXPECT_EQ(DaycareMove(1, 1), MOVE_NONE);
}

// 3. Full move slots are kept; known moves and non-Egg Moves are skipped without using a slot.
TEST("HnS egg move transfer: full slots kept, known and non-Egg Moves skipped")
{
    u32 caseId = 0;
    PARAMETRIZE { caseId = 0; } // learner full
    PARAMETRIZE { caseId = 1; } // learner already knows Curse, one empty slot
    PARAMETRIZE { caseId = 2; } // teacher's first move is not an Egg Move
    ASSUME_BULBASAUR_EGG_MOVES();
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_GROWL));
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_VINE_WHIP));
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_GROWTH));

    ResetDaycareTest(GEN_9);
    if (caseId == 0)
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_VINE_WHIP, move4=MOVE_GROWTH, item=ITEM_MIRROR_HERB;
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_CURSE, move2=MOVE_INGRAIN, move3=MOVE_NATURE_POWER, move4=MOVE_AMNESIA;
        );
    }
    else if (caseId == 1)
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_CURSE, move3=MOVE_GROWL, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_CURSE, move2=MOVE_INGRAIN, move3=MOVE_NONE, move4=MOVE_NONE;
        );
    }
    else
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_GROWL, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_INGRAIN, move3=MOVE_NONE, move4=MOVE_NONE;
        );
    }
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    if (caseId == 0)
    {
        EXPECT_EQ(DaycareMove(0, 0), MOVE_TACKLE);
        EXPECT_EQ(DaycareMove(0, 1), MOVE_GROWL);
        EXPECT_EQ(DaycareMove(0, 2), MOVE_VINE_WHIP);
        EXPECT_EQ(DaycareMove(0, 3), MOVE_GROWTH);
    }
    else if (caseId == 1)
    {
        EXPECT_EQ(DaycareMove(0, 0), MOVE_TACKLE);
        EXPECT_EQ(DaycareMove(0, 1), MOVE_CURSE);
        EXPECT_EQ(DaycareMove(0, 2), MOVE_GROWL);
        EXPECT_EQ(DaycareMove(0, 3), MOVE_INGRAIN);
    }
    else
    {
        EXPECT_EQ(DaycareMove(0, 0), MOVE_GROWL);
        EXPECT_EQ(DaycareMove(0, 1), MOVE_INGRAIN);
        EXPECT_EQ(DaycareMove(0, 2), MOVE_NONE);
        EXPECT_EQ(DaycareMove(0, 3), MOVE_NONE);
    }
}

// 4. The teacher's move slot order decides which Egg Move fills a single empty slot.
TEST("HnS egg move transfer: one empty slot takes the teacher's first transferable move")
{
    enum Move first = MOVE_NONE, second = MOVE_NONE;
    PARAMETRIZE { first = MOVE_AMNESIA; second = MOVE_CURSE; }
    PARAMETRIZE { first = MOVE_CURSE;   second = MOVE_AMNESIA; }
    ASSUME_BULBASAUR_EGG_MOVES();
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_GROWL));
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_VINE_WHIP));

    ResetDaycareTest(GEN_9);
    gSpecialVar_0x8001 = first;
    gSpecialVar_0x8002 = second;
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_VINE_WHIP, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
        givemon SPECIES_SLOWPOKE, 30, move1=VAR_0x8001, move2=VAR_0x8002, move3=MOVE_NONE, move4=MOVE_NONE;
    );
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    EXPECT_EQ(DaycareMove(0, 3), first);
}

// 4b. Several empty slots are filled in the teacher's slot order (not the Egg Move list order).
TEST("HnS egg move transfer: several empty slots follow the teacher's slot order")
{
    ASSUME_BULBASAUR_EGG_MOVES();

    ResetDaycareTest(GEN_9);
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_NATURE_POWER, move2=MOVE_TACKLE, move3=MOVE_INGRAIN, move4=MOVE_CURSE;
    );
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    EXPECT_EQ(DaycareMove(0, 0), MOVE_TACKLE);
    EXPECT_EQ(DaycareMove(0, 1), MOVE_NATURE_POWER);
    EXPECT_EQ(DaycareMove(0, 2), MOVE_INGRAIN);
    EXPECT_EQ(DaycareMove(0, 3), MOVE_CURSE);
}

// 5. Either Day Care slot can be the receiver, whatever the deposit order.
TEST("HnS egg move transfer: either slot can receive, in either deposit order")
{
    bool32 learnerFirst = FALSE;
    PARAMETRIZE { learnerFirst = TRUE; }
    PARAMETRIZE { learnerFirst = FALSE; }
    ASSUME_BULBASAUR_EGG_MOVES();

    ResetDaycareTest(GEN_9);
    if (learnerFirst)
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
            givemon SPECIES_SLOWPOKE, 30, move1=MOVE_CURSE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE;
        );
    }
    else
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_SLOWPOKE, 30, move1=MOVE_CURSE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE;
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
        );
    }
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    u32 learnerSlot = learnerFirst ? 0 : 1;
    EXPECT_EQ(DaycareSpecies(learnerSlot), SPECIES_BULBASAUR);
    EXPECT_EQ(DaycareMove(learnerSlot, 1), MOVE_CURSE);
    EXPECT_EQ(DaycareMove(1 - learnerSlot, 1), MOVE_NONE);
}

// 5b. Taking a Pokémon out and depositing it again checks again (either slot taken out),
// and a single Pokémon alone in the Day Care learns nothing.
TEST("HnS egg move transfer: withdraw and deposit again, and a single Pokemon")
{
    u32 withdrawSlot = 0;
    PARAMETRIZE { withdrawSlot = 0; }
    PARAMETRIZE { withdrawSlot = 1; }
    ASSUME_BULBASAUR_EGG_MOVES();
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_GROWL));
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_VINE_WHIP));
    ASSUME(LearnsByLevelUp(SPECIES_BULBASAUR, MOVE_GROWTH));

    ResetDaycareTest(GEN_9);
    // The learner is full at first, so nothing is learned on the first pass.
    if (withdrawSlot == 0)
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_VINE_WHIP, move4=MOVE_GROWTH, item=ITEM_MIRROR_HERB;
            givemon SPECIES_SLOWPOKE, 30, move1=MOVE_CURSE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE;
        );
    }
    else
    {
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_SLOWPOKE, 30, move1=MOVE_CURSE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE;
            givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_GROWL, move3=MOVE_VINE_WHIP, move4=MOVE_GROWTH, item=ITEM_MIRROR_HERB;
        );
    }
    DepositFirstPartyMon();
    DepositFirstPartyMon();
    EXPECT_EQ(DaycareSpecies(withdrawSlot), SPECIES_BULBASAUR);
    EXPECT_EQ(DaycareMove(withdrawSlot, 3), MOVE_GROWTH);

    // Take the learner out (the other one moves to slot 0 if needed), forget a move, and deposit it again.
    gSpecialVar_0x8004 = withdrawSlot;
    TakePokemonFromDaycare();
    EXPECT_EQ(DaycareSpecies(0), SPECIES_SLOWPOKE);
    EXPECT_EQ(DaycareSpecies(1), SPECIES_NONE);
    enum Move none = MOVE_NONE;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE4, &none);
    DepositFirstPartyMon();

    EXPECT_EQ(DaycareSpecies(1), SPECIES_BULBASAUR);
    EXPECT_EQ(DaycareMove(1, 3), MOVE_CURSE);

    // A single Pokémon alone: nothing is learned and the empty slot is not touched.
    ResetDaycareTest(GEN_9);
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
    );
    DepositFirstPartyMon();
    EXPECT_EQ(DaycareMove(0, 0), MOVE_TACKLE);
    EXPECT_EQ(DaycareMove(0, 1), MOVE_NONE);
    EXPECT_EQ(DaycareSpecies(1), SPECIES_NONE);
}

// 6. The herb is not used up; the teacher's moves and item and the learner's other data do not change.
TEST("HnS egg move transfer: Mirror Herb kept, other data unchanged")
{
    ASSUME_BULBASAUR_EGG_MOVES();

    ResetDaycareTest(GEN_9);
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_BULBASAUR, 20, move1=MOVE_TACKLE, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
        givemon SPECIES_SLOWPOKE, 30, move1=MOVE_AMNESIA, move2=MOVE_CURSE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_ORAN_BERRY;
    );
    struct BoxPokemon learnerBefore = gParties[B_TRAINER_PLAYER][0].box;
    struct BoxPokemon teacherBefore = gParties[B_TRAINER_PLAYER][1].box;
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    struct BoxPokemon *learner = &gSaveBlock1Ptr->daycare.mons[0].mon;
    struct BoxPokemon *teacher = &gSaveBlock1Ptr->daycare.mons[1].mon;
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_HELD_ITEM), ITEM_MIRROR_HERB);
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_MOVE1), MOVE_TACKLE);
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_MOVE2), MOVE_AMNESIA);
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_MOVE3), MOVE_CURSE);
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_MOVE4), MOVE_NONE);
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_PP2), GetMovePP(MOVE_AMNESIA));
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_PERSONALITY), GetBoxMonData(&learnerBefore, MON_DATA_PERSONALITY));
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_OT_ID), GetBoxMonData(&learnerBefore, MON_DATA_OT_ID));
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_EXP), GetBoxMonData(&learnerBefore, MON_DATA_EXP));
    EXPECT_EQ(GetBoxMonData(learner, MON_DATA_PP1), GetBoxMonData(&learnerBefore, MON_DATA_PP1));
    // The teacher is stored unchanged (Slowpoke has no Mirror Herb, so it learns nothing).
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        EXPECT_EQ(GetBoxMonData(teacher, MON_DATA_MOVE1 + i), GetBoxMonData(&teacherBefore, MON_DATA_MOVE1 + i));
        EXPECT_EQ(GetBoxMonData(teacher, MON_DATA_PP1 + i), GetBoxMonData(&teacherBefore, MON_DATA_PP1 + i));
    }
    EXPECT_EQ(GetBoxMonData(teacher, MON_DATA_HELD_ITEM), ITEM_ORAN_BERRY);
    EXPECT_EQ(GetBoxMonData(teacher, MON_DATA_PERSONALITY), GetBoxMonData(&teacherBefore, MON_DATA_PERSONALITY));
}

// HnS incense baby lines keep their separate lists: below Gen 9 an evolved Marill only uses
// Marill's own Egg Moves; from Gen 9 Azurill's (incense baby) list counts too.
TEST("HnS egg move transfer: incense baby lists kept")
{
    u32 gen = 0;
    enum Move move = MOVE_NONE;
    bool32 learns = FALSE;
    PARAMETRIZE { gen = GEN_8; move = MOVE_BELLY_DRUM; learns = TRUE; }
    PARAMETRIZE { gen = GEN_8; move = MOVE_COPYCAT;    learns = FALSE; }
    PARAMETRIZE { gen = GEN_9; move = MOVE_BELLY_DRUM; learns = TRUE; }
    PARAMETRIZE { gen = GEN_9; move = MOVE_COPYCAT;    learns = TRUE; }

    ResetDaycareTest(gen);
    ASSUME(GetEggSpecies(SPECIES_MARILL) == SPECIES_AZURILL);
    ASSUME(SpeciesHasEggMove(SPECIES_MARILL, MOVE_BELLY_DRUM));
    ASSUME(!SpeciesHasEggMove(SPECIES_MARILL, MOVE_COPYCAT));
    ASSUME(SpeciesHasEggMove(SPECIES_AZURILL, MOVE_COPYCAT));
    ASSUME(LearnsByLevelUp(SPECIES_MARILL, MOVE_WATER_GUN));

    gSpecialVar_0x8001 = move;
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_MARILL, 20, move1=MOVE_WATER_GUN, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE, item=ITEM_MIRROR_HERB;
        givemon SPECIES_MARILL, 20, move1=VAR_0x8001, move2=MOVE_NONE, move3=MOVE_NONE, move4=MOVE_NONE;
    );
    DepositFirstPartyMon();
    DepositFirstPartyMon();

    EXPECT_EQ(DaycareMove(0, 1), learns ? move : MOVE_NONE);
}
