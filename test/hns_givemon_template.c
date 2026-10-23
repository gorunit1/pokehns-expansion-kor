// HnS 적응 회귀 테스트 — seq 403 #9970(givemon/createmon이 PokemonTemplate·CreateMonFromTemplate로, 재확인 87).
// 이름이 모두 "HnS givemon:"으로 시작한다(TESTS='HnS givemon'). 언어와 무관하다.
#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "test/overworld_script.h"
#include "test/test.h"

// 신작 스타팅 스크립트(NewBarkTown_Lab_hns)는 abilityNum에 NUM_ABILITY_PERSONALITY를 직접 넘긴다.
// upstream 그대로면 이 값이 "지정한 특성"으로 처리돼 늘 0번 특성(assert 대체값)이 된다.
TEST("HnS givemon: NUM_ABILITY_PERSONALITY keeps the personality ability")
{
    ASSUME(GetSpeciesAbility(SPECIES_GROWLITHE, 1) != ABILITY_NONE);

    for (u32 i = 0; i < 16; i++)
    {
        ZeroPlayerPartyMons();
        RUN_OVERWORLD_SCRIPT(
            givemon SPECIES_GROWLITHE, 5, abilityNum=NUM_ABILITY_PERSONALITY;
        );
        u32 personality = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PERSONALITY);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_ABILITY_NUM), personality & 1);
    }
}

// 챌린지 "파티 개체값 최대"는 스크립트로 받는 포켓몬의 개체값도 덮는다(이식 전 ScriptGiveMonParameterized와 같음).
TEST("HnS givemon: max party IVs challenge overrides the template IVs")
{
    u8 oldSetting = gSaveBlock3Ptr->challengeSettings.tx_Challenges_MaxPartyIVs;
    gSaveBlock3Ptr->challengeSettings.tx_Challenges_MaxPartyIVs = 1;

    ZeroPlayerPartyMons();
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_GROWLITHE, 5, hpIv=0, atkIv=1, defIv=2, speedIv=3, spAtkIv=4, spDefIv=5;
    );

    gSaveBlock3Ptr->challengeSettings.tx_Challenges_MaxPartyIVs = oldSetting;

    for (u32 i = 0; i < NUM_STATS; i++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP_IV + i), MAX_PER_STAT_IVS);
}
