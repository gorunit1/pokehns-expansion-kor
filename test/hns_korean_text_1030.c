// 친구 요청 2026-10-30(2) 6절 한글 문구 회귀 테스트 — 조사(받침 있음·없음·ㄹ), 플레이어·상대 진영, 긴 이름 폭.
// 이름이 모두 "HnS text 1030:"로 시작한다.
#include "global.h"
#include "test/battle.h"
#include "test/test.h"
#include "battle_message.h"
#include "data.h"
#include "string_util.h"
#include "text.h"

#define MSG_WINDOW_WIDTH_PX BATTLE_MSG_MAX_WIDTH // 208 px = B_WIN_MSG 26칸(파티 메뉴 문장 창도 같은 폭 기준)

// 6-1 재확인 9: 텔레키네시스 — 2026-10-31 친구 결정으로 공식 본문 "[이름]을(를)\n허공에 띄웠다!"(USUM 8386).
// 이전(2026-10-30) 본문은 "[이름]은(는)\n높이 뛰어올랐다!"(BW 뛰어오르다 문장)였다.
SINGLE_BATTLE_TEST("HnS text 1030: Telekinesis message uses the official text and object particle")
{
    enum Species species;
    bool32 opponentTarget;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; opponentTarget = FALSE; }  // 마자용을(플레이어 쪽)
    PARAMETRIZE { species = SPECIES_PIKACHU;   opponentTarget = FALSE; }  // 피카츄를
    PARAMETRIZE { species = SPECIES_MARILL;    opponentTarget = FALSE; }  // 마릴을
    PARAMETRIZE { species = SPECIES_WOBBUFFET; opponentTarget = TRUE; }   // 상대 마자용을
    PARAMETRIZE { species = SPECIES_PIKACHU;   opponentTarget = TRUE; }   // 상대 피카츄를
    PARAMETRIZE { species = SPECIES_MARILL;    opponentTarget = TRUE; }   // 상대 마릴을

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TELEKINESIS) == EFFECT_TELEKINESIS);
        PLAYER(opponentTarget ? SPECIES_WYNAUT : species);
        OPPONENT(opponentTarget ? species : SPECIES_WYNAUT);
    } WHEN {
        if (opponentTarget)
            TURN { MOVE(player, MOVE_TELEKINESIS); }
        else
            TURN { MOVE(opponent, MOVE_TELEKINESIS); }
    } SCENE {
        if (!opponentTarget && species == SPECIES_WOBBUFFET)
            MESSAGE("마자용을 허공에 띄웠다!");
        else if (!opponentTarget && species == SPECIES_PIKACHU)
            MESSAGE("피카츄를 허공에 띄웠다!");
        else if (!opponentTarget)
            MESSAGE("마릴을 허공에 띄웠다!");
        else if (species == SPECIES_WOBBUFFET)
            MESSAGE("상대 마자용을 허공에 띄웠다!");
        else if (species == SPECIES_PIKACHU)
            MESSAGE("상대 피카츄를 허공에 띄웠다!");
        else
            MESSAGE("상대 마릴을 허공에 띄웠다!");
    }
}

// 6-2 재확인 10: 불새 충전 — (으)로부터, ㄹ 받침은 "로부터"
SINGLE_BATTLE_TEST("HnS text 1030: Sky Attack charge uses (eu)ro-buteo")
{
    enum Species species;
    bool32 onPlayerSide;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; onPlayerSide = TRUE; }   // 마자용으로부터
    PARAMETRIZE { species = SPECIES_PIKACHU;   onPlayerSide = TRUE; }   // 피카츄로부터
    PARAMETRIZE { species = SPECIES_MARILL;    onPlayerSide = TRUE; }   // 마릴로부터
    PARAMETRIZE { species = SPECIES_WOBBUFFET; onPlayerSide = FALSE; }  // 상대 마자용으로부터

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SKY_ATTACK) == EFFECT_TWO_TURNS_ATTACK);
        PLAYER(onPlayerSide ? species : SPECIES_WYNAUT);
        OPPONENT(onPlayerSide ? SPECIES_WYNAUT : species);
    } WHEN {
        if (onPlayerSide)
            TURN { MOVE(player, MOVE_SKY_ATTACK); }
        else
            TURN { MOVE(opponent, MOVE_SKY_ATTACK); }
    } SCENE {
        if (!onPlayerSide)
            MESSAGE("상대 마자용으로부터 눈부신 빛이 넘쳐흐른다!");
        else if (species == SPECIES_WOBBUFFET)
            MESSAGE("마자용으로부터 눈부신 빛이 넘쳐흐른다!");
        else if (species == SPECIES_PIKACHU)
            MESSAGE("피카츄로부터 눈부신 빛이 넘쳐흐른다!");
        else
            MESSAGE("마릴로부터 눈부신 빛이 넘쳐흐른다!");
    }
}

// 6-5 재확인 11: 멸망의바디 공식 문장(SV 8930·PLZA 6876)
SINGLE_BATTLE_TEST("HnS text 1030: Perish Body prints the official Korean line")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_BITE));
        PLAYER(SPECIES_CURSOLA) { Ability(ABILITY_PERISH_BODY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_BITE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BITE, opponent);
        ABILITY_POPUP(player, ABILITY_PERISH_BODY);
        MESSAGE("두 포켓몬 모두 3턴 후에 쓰러져 버린다!");
    }
}

// 6-7 재확인 47: 불사르기 — 2026-10-31 친구 결정으로 공식 본문 "[이름]의\n[도구]이(가) 불타 없어졌다!"(SV 7709~7712).
// 이전(2026-10-30) 본문은 "…[도구]을(를) 녹여 버렸다!"였다. 부식가스(STRINGID_PKMNITEMMELTED)와는 다른 문자열.
SINGLE_BATTLE_TEST("HnS text 1030: Incinerate uses the official text and subject particle after the item")
{
    enum Item item;
    bool32 targetOpponent;
    PARAMETRIZE { item = ITEM_ORAN_BERRY; targetOpponent = TRUE; }   // 상대 마자용의 오랭열매가
    PARAMETRIZE { item = ITEM_GHOST_GEM;  targetOpponent = TRUE; }   // 상대 마자용의 고스트주얼이(ㄹ 받침도 "이")
    PARAMETRIZE { item = ITEM_ORAN_BERRY; targetOpponent = FALSE; }  // 마자의 오랭열매가(플레이어 쪽)
    PARAMETRIZE { item = ITEM_GHOST_GEM;  targetOpponent = FALSE; }  // 마자의 고스트주얼이

    GIVEN {
        ASSUME(GetItemPocket(ITEM_ORAN_BERRY) == POCKET_BERRIES);
        PLAYER(SPECIES_WYNAUT) { Item(targetOpponent ? ITEM_NONE : item); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(targetOpponent ? item : ITEM_NONE); }
    } WHEN {
        if (targetOpponent)
            TURN { MOVE(player, MOVE_INCINERATE); }
        else
            TURN { MOVE(opponent, MOVE_INCINERATE); }
    } SCENE {
        if (!targetOpponent && item == ITEM_ORAN_BERRY)
            MESSAGE("마자의 오랭열매가 불타 없어졌다!");
        else if (!targetOpponent)
            MESSAGE("마자의 고스트주얼이 불타 없어졌다!");
        else if (item == ITEM_ORAN_BERRY)
            MESSAGE("상대 마자용의 오랭열매가 불타 없어졌다!");
        else
            MESSAGE("상대 마자용의 고스트주얼이 불타 없어졌다!");
    }
}

// 6-4 재확인 10g: 지금 쓰는 한글 클래스명의 공백이 다음 한글 글자와 합쳐지지 않는지.
// 테스트 빌드는 HnS 트레이너 표를 쓰지 않아(TESTING) 클래스명을 {B_BUFF1}로 넣는다 — 모든 배틀 자리표시자가 지나는
// 같은 복사 루프(공백 → CHAR_NBSP 변환)를 탄다.
static void HnsText_ExpectClassIntro(enum TrainerClassID trainerClass, const u8 *expected)
{
    u8 buffer[128];
    StringCopy(gBattleTextBuff1, gTrainerClasses[trainerClass].name);
    BattleStringExpandPlaceholders(COMPOUND_STRING("{B_BUFF1} 켄{B_TXT_IGA}\n승부를 걸어왔다!"), buffer, sizeof(buffer));
    EXPECT_EQ(StringCompare(buffer, expected), 0);
}

TEST("HnS text 1030: Korean trainer class names with a space stay intact")
{
    HnsText_ExpectClassIntro(TRAINER_CLASS_COOLTRAINER_HNS, COMPOUND_STRING("엘리트 트레이너 켄이\n승부를 걸어왔다!"));
    HnsText_ExpectClassIntro(TRAINER_CLASS_BIRD_KEEPER_HNS, COMPOUND_STRING("새 조련사 켄이\n승부를 걸어왔다!"));
    HnsText_ExpectClassIntro(TRAINER_CLASS_PKMN_BREEDER, COMPOUND_STRING("포켓몬 브리더 켄이\n승부를 걸어왔다!"));
}

// 참고: 영문 클래스명(HnS 트레이너·프런티어 데이터에서 쓰지 않음)은 공백이 아직 CHAR_NBSP(0x39, 한글 첫 바이트)로 바뀐다.
TEST("HnS text 1030: unused English class name with a space still gets CHAR_NBSP")
{
    u8 buffer[64];
    StringCopy(gBattleTextBuff1, gTrainerClasses[TRAINER_CLASS_TEAM_AQUA].name);
    BattleStringExpandPlaceholders(COMPOUND_STRING("{B_BUFF1}"), buffer, sizeof(buffer));
    EXPECT_EQ(buffer[4], CHAR_NBSP); // "TEAM AQUA"
}

// 6-6 재확인 42: 볼 바꾸기 성공 문장(파티 메뉴 문장, 일반 메뉴용 {K_EU}). 볼 이름은 모두 "볼"(ㄹ)로 끝나 "로".
TEST("HnS text 1030: ball swap message particles and width")
{
    static const u8 sMsg[] = _("{STR_VAR_1}의 볼을\n{STR_VAR_2}{K_EU}로 바꿨다!{PAUSE_UNTIL_PRESS}");
    u8 out[128];
    StringCopy(gStringVar1, COMPOUND_STRING("마자용"));
    StringCopy(gStringVar2, COMPOUND_STRING("GS볼"));
    StringExpandPlaceholders(out, sMsg);
    EXPECT_EQ(StringCompare(out, COMPOUND_STRING("마자용의 볼을\nGS볼로 바꿨다!{PAUSE_UNTIL_PRESS}")), 0);
    // 받침 있는 이름으로 끝나는 경우의 (으)로 처리도 확인(가상의 볼 이름)
    StringCopy(gStringVar2, COMPOUND_STRING("마자용"));
    StringExpandPlaceholders(out, sMsg);
    EXPECT_EQ(StringCompare(out, COMPOUND_STRING("마자용의 볼을\n마자용으로 바꿨다!{PAUSE_UNTIL_PRESS}")), 0);
    // 폭: 6글자 별명 + 가장 긴 볼 이름(6글자)
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("가나다라마바의 볼을"), 0), MSG_WINDOW_WIDTH_PX);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("가나다라마바볼로 바꿨다!"), 0), MSG_WINDOW_WIDTH_PX);
}

// 바뀐 배틀 문장의 가장 긴 줄(6글자 이름 + "상대 ", 10글자 도구 이름)이 문장 창 폭 안
TEST("HnS text 1030: longest lines of the changed battle messages fit the message window")
{
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("상대 가나다라마바을"), 0), MSG_WINDOW_WIDTH_PX);           // 6-1 1줄
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("허공에 띄웠다!"), 0), MSG_WINDOW_WIDTH_PX);                // 6-1 2줄
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("상대 가나다라마바으로부터"), 0), MSG_WINDOW_WIDTH_PX);     // 6-2 1줄
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("눈부신 빛이 넘쳐흐른다!"), 0), MSG_WINDOW_WIDTH_PX);       // 6-2 2줄
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("두 포켓몬 모두 3턴 후에 쓰러져 버린다!"), 0), MSG_WINDOW_WIDTH_PX); // 6-5 한 줄
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("상대 가나다라마바의"), 0), MSG_WINDOW_WIDTH_PX);           // 6-7 1줄
    EXPECT_LE(GetStringWidth(FONT_NORMAL, COMPOUND_STRING("가나다라마바사아자차이 불타 없어졌다!"), 0), MSG_WINDOW_WIDTH_PX); // 6-7 2줄
}
