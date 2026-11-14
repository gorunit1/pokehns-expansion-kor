// 공박사 연구소 "알에 변화 있니?" 대사 조건(친구 요청 2026-11-14, HGSS 맞춤): 조수에게 받은 토게피 알이
// 아직 부화하지 않았을 때만. HnS에는 부화 플래그가 없어 파티·PC에 토게피 알이 남아 있는지로 판단한다.
// 스크립트 분기(data/maps/NewBarkTown_Lab_hns/scripts.inc)는 이 함수의 결과를 그대로 쓴다. 이름이 "HnS Elm egg:"로 시작한다.
#include "global.h"
#include "test/test.h"
#include "event_data.h"
#include "field_specials.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "daycare.h"

static void MakeEgg(struct Pokemon *mon, enum Species species)
{
    bool8 isEgg = TRUE;
    CreateEgg(mon, species, TRUE);
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);
}

TEST("HnS Elm egg: an unhatched Togepi Egg in the party or a BOX counts, a hatched Togepi or another Egg does not")
{
    struct Pokemon egg;
    bool8 notEgg = FALSE;

    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_CYNDAQUIL, 10, 0, OTID_STRUCT_PLAYER_ID);
    EXPECT(!HnsPlayerHasTogepiEgg());                       // 알 수령 전

    MakeEgg(&gParties[B_TRAINER_PLAYER][1], SPECIES_TOGEPI);
    EXPECT(HnsPlayerHasTogepiEgg());                        // 받은 뒤, 부화 전(파티)
    gSpecialVar_Result = 0xFF;
    HnsCheckPlayerHasTogepiEgg();
    EXPECT_EQ(gSpecialVar_Result, TRUE);

    SetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_IS_EGG, &notEgg);
    EXPECT(!HnsPlayerHasTogepiEgg());                       // 부화 후
    HnsCheckPlayerHasTogepiEgg();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    ZeroMonData(&gParties[B_TRAINER_PLAYER][1]);
    MakeEgg(&gParties[B_TRAINER_PLAYER][1], SPECIES_PICHU);
    EXPECT(!HnsPlayerHasTogepiEgg());                       // 다른 종의 알

    MakeEgg(&egg, SPECIES_TOGEPI);
    *GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1) = egg.box;
    EXPECT(HnsPlayerHasTogepiEgg());                        // PC에 맡긴 알(파티 한도 1이면 PC로 감)
    ZeroBoxMonData(GetBoxedMonPtr(TOTAL_BOXES_COUNT - 1, IN_BOX_COUNT - 1));
    EXPECT(!HnsPlayerHasTogepiEgg());
    ZeroPlayerPartyMons();
}
