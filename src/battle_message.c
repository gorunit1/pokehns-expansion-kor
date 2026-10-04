#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_record.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "battle_special.h"
#include "battle_z_move.h"
#include "data.h"
#include "event_data.h"
#include "frontier_util.h"
#include "graphics.h"
#include "international_string_util.h"
#include "item.h"
#include "korean.h"
#include "link.h"
#include "load_save.h"
#include "menu.h"
#include "palette.h"
#include "recorded_battle.h"
#include "string_util.h"
#include "strings.h"
#include "test_runner.h"
#include "text.h"
#include "trainer_hill.h"
#include "trainer_slide.h"
#include "trainer_tower.h"
#include "window.h"
#include "line_break.h"
#include "constants/abilities.h"
#include "constants/battle_dome.h"
#include "constants/battle_string_ids.h"
#include "constants/frontier_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/trainer_hill.h"
#include "constants/weather.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    union {
        struct {
            DEPRECATED("Use color.background instead") u8 bgColor;
            DEPRECATED("Use color.foreground instead") u8 fgColor;
            DEPRECATED("Use color.shadow instead") u8 shadowColor;
            DEPRECATED("Use color.accent instead") u8 accentColor;
        };
        union TextColor color;
    };
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
};

#if TESTING
EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#else
static EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#endif
EWRAM_DATA struct BattleMsgData *gBattleMsgDataPtr = NULL;

// todo: make some of those names less vague: attacker/target vs pkmn, etc.

static const u8 sText_EmptyString4[] = _("");

const u8 gText_PkmnShroudedInMist[] = _("{B_ATK_PREFIX2}\n흰안개에 둘러싸였다!");
const u8 gText_PkmnGettingPumped[] = _("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n의욕이 넘치고 있다!");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1}의 {B_LAST_ABILITY} 때문에\n바꿀 수 없다!\p");
const u8 gText_StatSharply[] = _("크게 ");
const u8 gText_StatRose[] = _("올라갔다!");
const u8 gText_StatFell[] = _("떨어졌다!");
const u8 gText_DefendersStatRose[] = _("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE 0x0011}무사히 도망쳤다!\p");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA}의\n승부에서 이겼다!");
static const u8 sText_TwoLinkTrainersDefeated[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA} {B_LINK_OPPONENT2_NAME}{B_TXT_WAGWA}의\n승부에서 이겼다!");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA}의\n승부에서 졌다!");
static const u8 sText_PlayerLostToTwo[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA} {B_LINK_OPPONENT2_NAME}{B_TXT_WAGWA}의\n승부에서 졌다!");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA}의\n승부에서 비겼다!");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA} {B_LINK_OPPONENT2_NAME}{B_TXT_WAGWA}의\n승부에서 비겼다!");
static const u8 sText_WildFled[] = _("{PLAY_SE 0x0011}항복으로 대전이 중지되었습니다."); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_TwoWildFled[] = _("{PLAY_SE 0x0011}항복으로 대전이 중지되었습니다."); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_WAGWA}의\n승부에서 이겼다!\p");
static const u8 sText_OpponentMon1Appeared[] = _("{B_OPPONENT_MON1_NAME}{B_TXT_IGA} 튀어나왔다!\p");
static const u8 sText_WildPkmnAppeared[] = _("앗! 야생 {B_OPPONENT_MON1_NAME}{B_TXT_IGA} 튀어나왔다!\p");
static const u8 sText_WildPkmnAppearedLR[] = _("앗! 야생 {B_OPPONENT_MON1_NAME}{B_TXT_IGA} 튀어나왔다!\n도망갈까? {L_BUTTON}+{R_BUTTON}+{A_BUTTON}\p");
static const u8 sText_WildPkmnAppearedB[] = _("앗! 야생 {B_OPPONENT_MON1_NAME}{B_TXT_IGA} 튀어나왔다!\n도망갈까? {B_BUTTON}.\p");
static const u8 sText_LegendaryPkmnAppeared[] = _("{B_OPPONENT_MON1_NAME}{B_TXT_IGA} 나타났다!\p");
static const u8 sText_WildPkmnAppearedPause[] = _("앗! 야생 {B_OPPONENT_MON1_NAME}{B_TXT_IGA} 튀어나왔다!{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppeared[] = _("앗! 야생 {B_OPPONENT_MON1_NAME}{B_TXT_WAGWA}\n{B_OPPONENT_MON2_NAME}{B_TXT_IGA} 튀어나왔다!\p");
static const u8 sText_GhostAppearedCantId[] = _("The GHOST appeared!\pDarn!\nThe GHOST can't be ID'd!\p"); //frlg
static const u8 sText_TheGhostAppeared[] = _("The GHOST appeared!\p"); //frlg
static const u8 sText_Trainer1WantsToBattle[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_IGA}\n승부를 걸어왔다!\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_IGA}\n승부를 걸어왔다!");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA} {B_LINK_OPPONENT2_NAME}{B_TXT_IGA}\n승부를 걸어왔다!");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}{B_TXT_WAGWA} {B_OPPONENT_MON2_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_LinkTrainer2SentOutPkmn2[] = _("{B_LINK_OPPONENT2_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!!");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}{B_TXT_WAGWA} {B_OPPONENT_MON2_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_EUNNEUN}\n{B_LINK_OPPONENT_MON1_NAME}{B_TXT_EULREUL} 내보냈다!\p{B_LINK_OPPONENT2_NAME}{B_TXT_EUNNEUN}\n{B_LINK_OPPONENT_MON2_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_GoPkmn[] = _("가랏! {B_PLAYER_MON1_NAME}!");
static const u8 sText_GoTwoPkmn[] = _("가랏! {B_PLAYER_MON1_NAME}{B_TXT_WAGWA} {B_PLAYER_MON2_NAME}!");
static const u8 sText_GoPkmn2[] = _("가랏! {B_BUFF1}!");
static const u8 sText_DoItPkmn[] = _("다녀와! {B_BUFF1}!");
static const u8 sText_GoForItPkmn[] = _("힘내! {B_BUFF1}!");
static const u8 sText_JustALittleMorePkmn[] = _("앞으로 조금이야!\n힘내! {B_BUFF1}!"); //currently unused, will require code changes
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("상대가 약해져 있어!\n기회다! {B_BUFF1}!");
static const u8 sText_LinkPartnerSentOutPkmn1GoPkmn[] = _("{B_LINK_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_LINK_PLAYER_MON1_NAME}{B_TXT_EULREUL} 내보냈다!\l가랏! {B_LINK_PLAYER_MON2_NAME}!");
static const u8 sText_LinkPartnerSentOutPkmn2GoPkmn[] = _("{B_LINK_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_LINK_PLAYER_MON2_NAME}{B_TXT_EULREUL} 내보냈다!\l가랏! {B_LINK_PLAYER_MON1_NAME}!");
static const u8 sText_LinkPartnerSentOutPkmn1[] = _("{B_LINK_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_LinkPartnerSentOutPkmn2[] = _("{B_LINK_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_LinkPartnerWithdrewPkmn1[] = _("{B_LINK_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_LINK_PLAYER_MON1_NAME}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_LinkPartnerWithdrewPkmn2[] = _("{B_LINK_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_LINK_PLAYER_MON2_NAME}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_PkmnSwitchOut[] = _("{B_BUFF1}, switch out! Come back!"); //currently unused, I believe its used for when you switch on a pokemon in shift mode
static const u8 sText_PkmnThatsEnough[] = _("{B_BUFF1} 이제 됐어!\n돌아와!");
static const u8 sText_PkmnComeBack[] = _("{B_BUFF1}\n돌아와!");
static const u8 sText_PkmnOkComeBack[] = _("{B_BUFF1} 좋았어!\n돌아와!");
static const u8 sText_PkmnGoodComeBack[] = _("{B_BUFF1} 잘했어!\n돌아와!");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_Trainer2WithdrewPkmn[] = _("{B_TRAINER2_CLASS} {B_TRAINER2_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_SCR_TRAINER_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_WildPkmnPrefix[] = _("야생 ");
static const u8 sText_FoePkmnPrefix[] = _("상대 ");
static const u8 sText_WildPkmnPrefixLower[] = _("야생 ");
static const u8 sText_FoePkmnPrefixLower[] = _("상대 ");
static const u8 sText_EmptyString8[] = _("");
static const u8 sText_FoePkmnPrefix2[] = _("상대");
static const u8 sText_AllyPkmnPrefix[] = _("우리 편");
static const u8 sText_FoePkmnPrefix3[] = _("상대는");
static const u8 sText_AllyPkmnPrefix2[] = _("우리 편은");
static const u8 sText_FoePkmnPrefix4[] = _("상대를");
static const u8 sText_AllyPkmnPrefix3[] = _("우리 편을");
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF3}{B_TXT_EULREUL} 썼다!");
static const u8 sText_ExclamationMark[] = _("{B_TXT_EULREUL} 썼다!");
static const u8 sText_ExclamationMark2[] = _("{B_TXT_EULREUL} 썼다!");
static const u8 sText_ExclamationMark3[] = _("{B_TXT_EULREUL} 썼다!");
static const u8 sText_ExclamationMark4[] = _(" 공격!");
static const u8 sText_ExclamationMark5[] = _("!");
static const u8 sText_HP[] = _("HP");
static const u8 sText_Attack[] = _("공격");
static const u8 sText_Defense[] = _("방어");
static const u8 sText_Speed[] = _("스피드");
static const u8 sText_SpAttack[] = _("특수공격");
static const u8 sText_SpDefense[] = _("특수방어");
static const u8 sText_Accuracy[] = _("명중률");
static const u8 sText_Evasiveness[] = _("회피율");

const u8 *const gStatNamesTable[NUM_BATTLE_STATS] =
{
    [STAT_HP]      = sText_HP,
    [STAT_ATK]     = sText_Attack,
    [STAT_DEF]     = sText_Defense,
    [STAT_SPEED]   = sText_Speed,
    [STAT_SPATK]   = sText_SpAttack,
    [STAT_SPDEF]   = sText_SpDefense,
    [STAT_ACC]     = sText_Accuracy,
    [STAT_EVASION] = sText_Evasiveness,
};
const u8 *const gPokeblockWasTooXStringTable[FLAVOR_COUNT] =
{
    [FLAVOR_SPICY]  = COMPOUND_STRING("너무 맵다!"),
    [FLAVOR_DRY]    = COMPOUND_STRING("너무 떫다!"),
    [FLAVOR_SWEET]  = COMPOUND_STRING("너무 달다!"),
    [FLAVOR_BITTER] = COMPOUND_STRING("너무 쓰다!"),
    [FLAVOR_SOUR]   = COMPOUND_STRING("너무 시다!"),
};

static const u8 sText_Someones[] = _("누군가의");
static const u8 sText_Lanettes[] = _("유미의"); //no decapitalize until it is everywhere
static const u8 sText_Bills[] = _("이수재의");
static const u8 sText_EnigmaBerry[] = _("의문열매"); //no decapitalize until it is everywhere
static const u8 sText_BerrySuffix[] = _("열매"); //no decapitalize until it is everywhere
const u8 gText_EmptyString3[] = _(" ");

static const u8 sText_TwoInGameTrainersDefeated[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_WAGWA}\n{B_TRAINER2_CLASS} {B_TRAINER2_NAME}{B_TXT_WAGWA}의\l승부에서 이겼다!\p");

// New battle strings.
const u8 gText_drastically[] = _("매우 크게 ");
const u8 gText_severely[] = _("매우 크게 ");
static const u8 sText_TerrainReturnedToNormal[] = _("The terrain returned to normal!"); // Unused

const u8 *const gBattleStringsTable[STRINGID_COUNT] =
{
    [STRINGID_TRAINER1LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER1_LOSE_TEXT}"),
    [STRINGID_PKMNGAINEDEXP]                        = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}{B_BUFF2}\n{B_BUFF3} 경험치를 얻었다!\p"),
    [STRINGID_PKMNGREWTOLV]                         = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}\n레벨{B_BUFF2}{B_TXT_EU}로 올랐다!{WAIT_SE}\p"),
    [STRINGID_PKMNLEARNEDMOVE]                      = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}\n{B_BUFF2}{B_TXT_EULREUL} 배웠다!{WAIT_SE}\p"),
    [STRINGID_TRYTOLEARNMOVE1]                      = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN} 새로\n{B_BUFF2}{B_TXT_EULREUL} 배우고 싶다...!\p"),
    [STRINGID_TRYTOLEARNMOVE2]                      = COMPOUND_STRING("그러나 {B_BUFF1}{B_TXT_EUNNEUN} 기술을 4개\n알고 있으므로 더 이상 배울 수 없다!\p"),
    [STRINGID_TRYTOLEARNMOVE3]                      = COMPOUND_STRING("{B_BUFF2} 대신\n다른 기술을 잊게 하겠습니까?"),
    [STRINGID_PKMNFORGOTMOVE]                       = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN} {B_BUFF2}{B_TXT_EULREUL}\n깨끗이 잊었다!\p"),
    [STRINGID_STOPLEARNINGMOVE]                     = COMPOUND_STRING("{PAUSE 32}그럼... {B_BUFF2}{B_TXT_EULREUL}\n배우는 것을 포기하겠습니까?"),
    [STRINGID_DIDNOTLEARNMOVE]                      = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN} {B_BUFF2}{B_TXT_EULREUL}\n결국 배우지 않았다!\p"),
    [STRINGID_PKMNLEARNEDMOVE2]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 배웠다!"),
    [STRINGID_PKMNPROTECTEDITSELF]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 공격으로부터\n몸을 지켰다!"),
    [STRINGID_STATSWONTINCREASE2]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 능력은\n더 올라가지 않는다!"),
    [STRINGID_ITDOESNTAFFECT]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다..."),
    [STRINGID_SCR_ITDOESNTAFFECT]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다..."),
    [STRINGID_BATTLERFAINTED]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 쓰러졌다!\p"),
    [STRINGID_PLAYERGOTMONEY]                       = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN} 상금으로\n{B_BUFF1}원을 손에 넣었다!\p"),
    [STRINGID_PLAYERWHITEOUT]                       = COMPOUND_STRING("{B_PLAYER_NAME}에게는\n싸울 수 있는 포켓몬이 없다!\p"),
    [STRINGID_PLAYERWHITEOUT2_WILD]                 = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN} 당황해서\n{B_BUFF1}원을 잃어버렸다!\p"),
    [STRINGID_PLAYERWHITEOUT2_TRAINER]              = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN} 상금으로\n{B_BUFF1}원을 지불했다!\p"),
    [STRINGID_PLAYERWHITEOUT3]                      = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n눈앞이 캄캄해졌다!"),
    [STRINGID_PREVENTSESCAPE]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_SCR_ABILITY} 때문에 도망칠 수 없다!\p"),
    [STRINGID_HITXTIMES]                            = COMPOUND_STRING("{B_BUFF1}번 맞았다!"), //SV has dynamic plural here
    [STRINGID_PKMNFELLASLEEP]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n잠들어 버렸다!"),
    [STRINGID_PKMNMADESLEEP]                        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 {B_SCR_ABILITY} 때문에\n{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 잠들어 버렸다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYASLEEP]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 이미\n잠들어 있다"),
    [STRINGID_PKMNALREADYASLEEP2]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 이미\n잠들어 있다"),
    [STRINGID_PKMNWASPOISONED]                      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}의 몸에 독이 퍼졌다!"),
    [STRINGID_PKMNPOISONEDBY]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n맹독구슬 때문에 맹독에 중독됐다!"), //toxic orb
    [STRINGID_PKMNHURTBYPOISON]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n독에 의한 데미지를 입고 있다!"),
    [STRINGID_PKMNALREADYPOISONED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 이미\n몸에 독이 퍼진 상태다"),
    [STRINGID_PKMNBADLYPOISONED]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}의\n몸에 맹독이 퍼졌다!"),
    [STRINGID_PKMNENERGYDRAINED]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EU}로부터\n체력을 흡수했다!"),
    [STRINGID_PKMNWASBURNED]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n화상을 입었다!"),
    [STRINGID_PKMNBURNEDBY]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n화염구슬 때문에 화상을 입었다!"), //flame orb
    [STRINGID_PKMNHURTBYBURN]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n화상 데미지를 입고 있다!"),
    [STRINGID_PKMNWASFROZEN]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n얼어붙었다!"),
    [STRINGID_PKMNFROZENBY]                         = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 {B_SCR_ABILITY} 때문에\n{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 얼어붙었다!"), //not in gen 5+, ability popup - not used at all
    [STRINGID_PKMNISFROZEN]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n얼어버려서 움직이지 않는다!"),
    [STRINGID_PKMNWASDEFROSTED]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n얼음이 녹았다!"),
    [STRINGID_PKMNWASDEFROSTEDBY]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_CURRENT_MOVE} 때문에 얼음이 녹았다!"),
    [STRINGID_PKMNWASPARALYZED]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 마비되어\n기술이 나오기 어려워졌다!"),
    [STRINGID_PKMNWASPARALYZEDBY]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}의 {B_SCR_ABILITY} 때문에\l마비되어 기술이 나오기 어려워졌다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNISPARALYZED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n몸이 저려서 움직일 수 없다"),
    [STRINGID_PKMNISALREADYPARALYZED]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n이미 마비되어 있다"),
    [STRINGID_PKMNHEALEDPARALYSIS]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!"),
    [STRINGID_STATSWONTINCREASE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EUNNEUN} 더 올라가지 않는다!"),
    [STRINGID_STATSWONTDECREASE]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EUNNEUN} 더 떨어지지 않는다!"),
    [STRINGID_PKMNISCONFUSED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n혼란에 빠져 있다!"),
    [STRINGID_PKMNHEALEDCONFUSION]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n혼란이 풀렸다!"),
    [STRINGID_PKMNWASCONFUSED]                      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n혼란에 빠졌다!"),
    [STRINGID_PKMNALREADYCONFUSED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n이미 혼란에 빠져 있다"),
    [STRINGID_PKMNFELLINLOVE]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n헤롱헤롱해졌다!"),
    [STRINGID_PKMNINLOVE]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}에게 헤롱헤롱해 있다!"),
    [STRINGID_PKMNIMMOBILIZEDBYLOVE]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n헤롱헤롱해서 기술을 쓸 수 없었다!"),
    [STRINGID_PKMNCHANGEDTYPE]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}타입이 됐다!"),
    [STRINGID_PKMNFLINCHED]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n풀이 죽어 기술을 쓸 수 없다!"),
    [STRINGID_PKMNREGAINEDHEALTH]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n체력이 회복되었다!"),
    [STRINGID_PKMNHPFULL]                           = COMPOUND_STRING("그러나 {B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n체력이 가득찬 상태다!"),
    [STRINGID_PKMNRAISEDSPDEF]                      = COMPOUND_STRING("{B_ATK_PREFIX2} {B_CURRENT_MOVE}{B_TXT_EU}로\n특수공격에 강해졌다!"),
    [STRINGID_PKMNRAISEDDEF]                        = COMPOUND_STRING("{B_ATK_PREFIX2} {B_CURRENT_MOVE}{B_TXT_EU}로\n물리공격에 강해졌다!"),
    [STRINGID_PKMNRAISEDDEFSPDEF]                   = COMPOUND_STRING("{B_ATK_PREFIX2} {B_CURRENT_MOVE}{B_TXT_EU}로\n물리공격과 특수공격에 강해졌다!"),
    [STRINGID_PKMNAURORAVEIL]                       = COMPOUND_STRING("{B_ATK_PREFIX2} {B_CURRENT_MOVE}{B_TXT_EU}로\n물리공격과 특수공격에 강해졌다!"), // HnS: same text as STRINGID_PKMNRAISEDDEFSPDEF
    [STRINGID_PKMNCOVEREDBYVEIL]                    = COMPOUND_STRING("{B_ATK_PREFIX2}\n신비의 베일에 둘러싸였다!"),
    [STRINGID_PKMNUSEDSAFEGUARD]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n신비의 베일이 지켜주고 있다!"),
    [STRINGID_PKMNSAFEGUARDEXPIRED]                 = COMPOUND_STRING("{B_ATK_PREFIX3} 감싸던\n신비의 베일이 없어졌다!"),
    [STRINGID_PKMNWENTTOSLEEP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n잠을 자기 시작했다!"), //not in gen 5+
    [STRINGID_PKMNSLEPTHEALTHY]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n잠이 들어 건강해졌다!"),
    [STRINGID_PKMNWHIPPEDWHIRLWIND]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 주위에서\n공기가 소용돌이친다!"),
    [STRINGID_PKMNTOOKSUNLIGHT]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n빛을 흡수했다!"),
    [STRINGID_PKMNLOWEREDHEAD]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n목을 움츠렸다!"),
    [STRINGID_PKMNFLEWHIGH]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n하늘 높이 날아올랐다!"),
    [STRINGID_PKMNDUGHOLE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n구멍을 파서 땅속에 파고들었다!"),
    [STRINGID_PKMNSQUEEZEDBYBIND]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}에게 조이기를 당했다!"),
    [STRINGID_PKMNTRAPPEDINVORTEX]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소용돌이 속에 갇혔다!"),
    [STRINGID_PKMNWRAPPEDBY]                        = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}에게 휘감겼다!"),
    [STRINGID_PKMNCLAMPED]                          = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_NAME_WITH_PREFIX}의 껍질에 꼈다!"),
    [STRINGID_PKMNHURTBY]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_BUFF1}의\n데미지를 입고 있다"),
    [STRINGID_PKMNFREEDFROM]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EU}로부터 풀려났다!"),
    [STRINGID_PKMNCRASHED]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n의욕이 넘쳐서 땅에 부딪혔다!"),
    [STRINGID_PKMNSHROUDEDINMIST]                   = gText_PkmnShroudedInMist,
    [STRINGID_PKMNPROTECTEDBYMIST]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n흰안개가 지켜주고 있다"),
    [STRINGID_PKMNGETTINGPUMPED]                    = gText_PkmnGettingPumped,
    [STRINGID_PKMNHITWITHRECOIL]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n반동으로 데미지를 입었다!"),
    [STRINGID_PKMNPROTECTEDITSELF2]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n방어 태세에 들어갔다!"),
    [STRINGID_PKMNBUFFETEDBYSANDSTORM]              = COMPOUND_STRING("모래바람이 {B_ATK_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n덮쳤다!"),
    [STRINGID_PKMNPELTEDBYHAIL]                     = COMPOUND_STRING("싸라기눈이 {B_ATK_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n덮쳤다!"),
    [STRINGID_PKMNSEEDED]                           = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게\n씨앗을 심었다!"),
    [STRINGID_PKMNEVADEDATTACK]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격을 피했다!"),
    [STRINGID_PKMNSAPPEDBYLEECHSEED]                = COMPOUND_STRING("씨뿌리기가 {B_ATK_NAME_WITH_PREFIX}의\n체력을 빼앗는다!"),
    [STRINGID_PKMNFASTASLEEP]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n쿨쿨 잠들어 있다"),
    [STRINGID_PKMNWOKEUP]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 눈을 떴다!"),
    [STRINGID_PKMNWOKEUPINUPROAR]                   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소란스러워서 눈을 떴다!"),
    [STRINGID_PKMNCAUSEDUPROAR]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 소란피기 시작했다!"),
    [STRINGID_PKMNMAKINGUPROAR]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 소란피우고 있다!"),
    [STRINGID_PKMNCALMEDDOWN]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n얌전해졌다"),
    [STRINGID_PKMNSTOCKPILED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}개 비축했다!"),
    [STRINGID_PKMNCANTSLEEPINUPROAR2]               = COMPOUND_STRING("그러나 {B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소란피고 있어서 잠들지 않는다!"),
    [STRINGID_UPROARKEPTPKMNAWAKE]                  = COMPOUND_STRING("그러나 {B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소란스러워서 잠들 수 없다!"),
    [STRINGID_PKMNSTAYEDAWAKEUSING]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n잠들지 않는다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNCANNOTBEPOISONED]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n독에 중독되지 않는다!"),
    [STRINGID_PKMNCANNOTBURN]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n화상을 입지 않는다!"),
    [STRINGID_PKMNCANNOTPARALYZE]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n마비되지 않는다!"),
    [STRINGID_PKMNCANNOTFREEZE]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n얼지 않는다!"),
    [STRINGID_PKMNCANNOTSLEEP]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n잠들지 않는다!"),
    [STRINGID_PKMNSTORINGENERGY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 참고 있다"),
    [STRINGID_PKMNUNLEASHEDENERGY]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n참기가 풀렸다!"),
    [STRINGID_PKMNFATIGUECONFUSION]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n몹시 지쳐서 혼란에 빠졌다!"),
    [STRINGID_PLAYERPICKEDUPMONEY]                  = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}원을 주웠다!\p"),
    [STRINGID_PKMNUNAFFECTED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게는\n전혀 효과가 없다!"),
    [STRINGID_PKMNTRANSFORMEDINTO]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EU}로 변신했다!"),
    [STRINGID_PKMNMADESUBSTITUTE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n대타가 나타났다"),
    [STRINGID_PKMNHASSUBSTITUTE]                    = COMPOUND_STRING("그러나 {B_ATK_NAME_WITH_PREFIX}의\n대타는 이미 나와있다!"),
    [STRINGID_SUBSTITUTEDAMAGED]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EULREUL} 대신하여\n대타가 공격을 받았다!\p"),
    [STRINGID_PKMNSUBSTITUTEFADED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n대타는 사라져 버렸다...\p"),
    [STRINGID_PKMNMUSTRECHARGE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격의 반동으로 움직일 수 없다!"),
    [STRINGID_PKMNRAGEBUILDING]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n분노 볼티지가 올라가고 있다!"),
    [STRINGID_PKMNMOVEWASDISABLED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EULREUL} 봉인했다!"),
    [STRINGID_PKMNMOVEISDISABLED]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 사슬묶기 때문에\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!\p"),
    [STRINGID_PKMNMOVEDISABLEDNOMORE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n사슬묶기가 풀렸다!"),
    [STRINGID_PKMNGOTENCORE]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n앙코르를 받았다!"),
    [STRINGID_PKMNGOTENCOREDMOVE]                   = COMPOUND_STRING("앙코르의 효과로\n{B_CURRENT_MOVE}밖에 쓸 수 없다!\p"),
    [STRINGID_PKMNENCOREENDED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n앙코르 상태가 풀렸다!"),
    [STRINGID_PKMNTOOKAIM]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 목표를\n{B_DEF_NAME_WITH_PREFIX}{B_TXT_EU}로 결정했다!"),
    [STRINGID_PKMNSKETCHEDMOVE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 스케치했다!"),
    [STRINGID_PKMNTRYINGTOTAKEFOE]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 상대를\n길동무로 삼으려 하고 있다"),
    [STRINGID_PKMNTOOKFOE]                          = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 상대를\n길동무로 삼았다!"),
    [STRINGID_PKMNREDUCEDPP]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EULREUL} {B_BUFF2} 깎았다!"),
    [STRINGID_PKMNSTOLEITEM]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}{B_TXT_EU}로부터\n{B_LAST_ITEM}{B_TXT_EULREUL} 빼앗았다!"),
    [STRINGID_TARGETCANTESCAPENOW]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n이제 도망칠 수 없다!"),
    [STRINGID_PKMNFELLINTONIGHTMARE]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n악몽을 꾸기 시작했다!"),
    [STRINGID_PKMNLOCKEDINNIGHTMARE]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n악몽에 시달리고 있다!"),
    [STRINGID_PKMNLAIDCURSE]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n자신의 체력을 깎아서\l{B_DEF_NAME_WITH_PREFIX}에게 저주를 걸었다!"),
    [STRINGID_PKMNAFFLICTEDBYCURSE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n저주받고 있다!"),
    [STRINGID_SPIKESSCATTERED]                      = COMPOUND_STRING("{B_DEF_PREFIX1}의 발밑에\n압정이 뿌려졌다!"),
    [STRINGID_PKMNHURTBYSPIKES]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 압정뿌리기의\n데미지를 입었다!"),
    [STRINGID_PKMNIDENTIFIED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}의\n정체를 꿰뚫어 보았다!"),
    [STRINGID_PKMNPERISHCOUNTFELL]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 멸망의\n카운트가 {B_BUFF1}{B_TXT_IGA} 되었다!"),
    [STRINGID_PKMNBRACEDITSELF]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n버티기 태세에 들어갔다!"),
    [STRINGID_PKMNENDUREDHIT]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격을 버텼다!"),
    [STRINGID_MAGNITUDESTRENGTH]                    = COMPOUND_STRING("매그니튜드 {B_BUFF1}!!"),
    [STRINGID_PKMNCUTHPMAXEDATTACK]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n체력을 깎아서 풀 파워로 만들었다!"),
    [STRINGID_PKMNCOPIEDSTATCHANGES]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}의\n능력 변화를 복사했다!"),
    [STRINGID_PKMNGOTFREE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EU}로부터 풀려났다!"), //not in gen 5+, generic rapid spin?
    [STRINGID_PKMNSHEDLEECHSEED]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n씨뿌리기로부터 풀려났다!"), //not in gen 5+, generic rapid spin?
    [STRINGID_PKMNBLEWAWAYSPIKES]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n압정을 날려버렸다!"), //not in gen 5+, generic rapid spin?
    [STRINGID_PKMNFLEDFROMBATTLE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n배틀에서 이탈했다!"),
    [STRINGID_PKMNFORESAWATTACK]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n미래의 공격을 예지했다!"),
    [STRINGID_PKMNTOOKATTACK]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1} 공격을 받았다!"),
    [STRINGID_PKMNATTACK]                           = COMPOUND_STRING("{B_BUFF1}의 공격!"), //not in gen 5+, beat up text
    [STRINGID_PKMNCENTERATTENTION]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n주목의 대상이 되었다!"),
    [STRINGID_PKMNCHARGINGPOWER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n충전을 시작했다!"),
    [STRINGID_NATUREPOWERTURNEDINTO]                = COMPOUND_STRING("자연의힘은\n{B_CURRENT_MOVE}{B_TXT_IGA} 되었다!"),
    [STRINGID_PKMNSTATUSNORMAL]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n상태이상이 나았다!"),
    [STRINGID_PKMNPOISONCURED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 독은\n말끔하게 해독됐다!"),
    [STRINGID_PKMNBURNCURED]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n화상이 나았다!"),
    [STRINGID_PKMNPARALYSISCURED]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!"),
    [STRINGID_PKMNHASNOMOVESLEFT]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n쓸 수 있는 기술이 없다!\p"),
    [STRINGID_PKMNSUBJECTEDTOTORMENT]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n트집을 잡혔다!"),
    [STRINGID_PKMNCANTUSEMOVETORMENT]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n트집을 잡혔기 때문에\l계속해서 같은 기술을 쓸 수 없다!\p"),
    [STRINGID_PKMNTIGHTENINGFOCUS]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n집중력을 높이고 있다!"),
    [STRINGID_PKMNFELLFORTAUNT]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n도발에 넘어가 버렸다!"),
    [STRINGID_PKMNCANTUSEMOVETAUNT]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 도발당한 상태라서\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!\p"),
    [STRINGID_PKMNREADYTOHELP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}에게\n도우미가 되어주려 한다!"),
    [STRINGID_PKMNSWITCHEDITEMS]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 서로의\n도구를 교체했다!"),
    [STRINGID_PKMNCOPIEDFOE]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}의\n{B_DEF_ABILITY}{B_TXT_EULREUL} 복사했다!"),
    [STRINGID_PKMNWISHCAMETRUE]                     = COMPOUND_STRING("{B_BUFF1}의\n희망사항이 이루어졌다!"),
    [STRINGID_PKMNPLANTEDROOTS]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 뿌리를 뻗었다!"),
    [STRINGID_PKMNABSORBEDNUTRIENTS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 뿌리로부터\n양분을 흡수했다!"),
    [STRINGID_PKMNANCHOREDITSELF]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 뿌리를 뻗어서\n움직이지 않는다!"),
    [STRINGID_PKMNWASMADEDROWSY]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n졸음을 유도했다!"),
    [STRINGID_PKMNKNOCKEDOFF]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX}의\n{B_LAST_ITEM}{B_TXT_EULREUL} 탁 쳐서 떨구었다!"),
    [STRINGID_PKMNSWAPPEDABILITIES]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n서로의 특성을 교체했다!"),
    [STRINGID_PKMNSEALEDOPPONENTMOVE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n상대의 기술을 봉인했다!"),
    [STRINGID_PKMNCANTUSEMOVESEALED]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 봉인 때문에\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!\p"),
    [STRINGID_PKMNWANTSGRUDGE]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 상대에게\n원념을 걸려 하고 있다!"),
    [STRINGID_PKMNLOSTPPGRUDGE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 {B_BUFF1}{B_TXT_EUNNEUN}\n원념으로 PP가 0이 되었다!"),
    [STRINGID_PKMNSHROUDEDITSELF]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}에 둘러싸였다!"),
    [STRINGID_PKMNMOVEBOUNCED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 되받아쳤다!"),
    [STRINGID_PKMNWAITSFORTARGET]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n상대의 움직임을 살피고 있다!"),
    [STRINGID_PKMNSNATCHEDMOVE]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_SCR_NAME_WITH_PREFIX}의\n기술을 가로챘다!"),
    [STRINGID_PKMNMADEITRAIN]                       = COMPOUND_STRING("비가 내리기 시작했다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPROTECTEDBY]                      = COMPOUND_STRING("그러나 {B_DEF_NAME_WITH_PREFIX}에게는\n실패하고 말았다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSUSAGE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!"), //I don't see this in SV text
    [STRINGID_PKMNRESTOREDHPUSING]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n체력이 회복되었다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNCHANGEDTYPEWITH]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}타입이 됐다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSROMANCEWITH]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다..."), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSCONFUSIONWITH]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n혼란되지 않는다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNRAISEDFIREPOWERWITH]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n불꽃의 위력이 올라갔다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNANCHORSITSELFWITH]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_DEF_ABILITY} 때문에 들러붙어 있다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNCUTSATTACKWITH]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n공격이 떨어졌다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSSTATLOSSWITH]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_ABILITY}의 효과로 능력이 떨어지지 않는다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTSWITH]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n상처를 입었다!"),
    [STRINGID_PKMNTRACED]                           = COMPOUND_STRING("{B_BUFF1}의\n{B_BUFF2}{B_TXT_EULREUL} 트레이스했다!"),
    [STRINGID_STATSHARPLY]                          = gText_StatSharply,
    [STRINGID_STATHARSHLY]                          = COMPOUND_STRING("매우 크게 "),
    [STRINGID_ATTACKERSSTATROSE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"),
    [STRINGID_DEFENDERSSTATROSE]                    = gText_DefendersStatRose,
    [STRINGID_SCRIPTINGSTATROSE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"),
    [STRINGID_ATTACKERSSTATFELL]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}떨어졌다!"),
    [STRINGID_DEFENDERSSTATFELL]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}떨어졌다!"),
    [STRINGID_CRITICALHIT]                          = COMPOUND_STRING("급소에 맞았다!"),
    [STRINGID_ONEHITKO]                             = COMPOUND_STRING("일격필살!"),
    [STRINGID_123POOF]                              = COMPOUND_STRING("{PAUSE 32}1, {PAUSE 15}2, {PAUSE 15}... {PAUSE 15}... {PAUSE 15}... {PLAY_SE 0x0038}짠!\p"),
    [STRINGID_ANDELLIPSIS]                          = COMPOUND_STRING("그리고...!\p"),
    [STRINGID_NOTVERYEFFECTIVE]                     = COMPOUND_STRING("효과가 별로인 듯하다."),
    [STRINGID_SUPEREFFECTIVE]                       = COMPOUND_STRING("효과가 굉장했다!"),
    [STRINGID_GOTAWAYSAFELY]                        = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED]                         = COMPOUND_STRING("{PLAY_SE 0x0011}야생 {B_BUFF1}{B_TXT_EUNNEUN} 도망쳤다!"),
    [STRINGID_NORUNNINGFROMTRAINERS]                = COMPOUND_STRING("안돼! 승부 도중에\n상대에게 등을 보일 순 없어!\p"),
    [STRINGID_CANTESCAPE]                           = COMPOUND_STRING("도망칠 수 없다!\p"),
    [STRINGID_DONTLEAVEBIRCH]                       = COMPOUND_STRING("털보박사: 외, 외면하지 말아줘-!\p"), //no decapitalize until it is everywhere
    [STRINGID_BUTNOTHINGHAPPENED]                   = COMPOUND_STRING("그러나 아무 일도 일어나지 않았다"),
    [STRINGID_BUTITFAILED]                          = COMPOUND_STRING("그러나 실패하고 말았다!"),
    [STRINGID_ITHURTCONFUSION]                      = COMPOUND_STRING("영문도 모른 채\n자신을 공격했다!"),
    [STRINGID_STARTEDTORAIN]                        = COMPOUND_STRING("비가 내리기 시작했다!"),
    [STRINGID_DOWNPOURSTARTED]                      = COMPOUND_STRING("폭우로 변했다!"), // corresponds to DownpourText in pokegold and pokecrystal and is used by Rain Dance in GSC
    [STRINGID_RAINCONTINUES]                        = COMPOUND_STRING("비가 내리고 있다!"), //not in gen 5+
    [STRINGID_DOWNPOURCONTINUES]                    = COMPOUND_STRING("폭우가 계속 내리고 있다"), // unused
    [STRINGID_RAINSTOPPED]                          = COMPOUND_STRING("비가 그쳤다!"),
    [STRINGID_SANDSTORMBREWED]                      = COMPOUND_STRING("모래바람이 불기 시작했다!"),
    [STRINGID_SANDSTORMRAGES]                       = COMPOUND_STRING("모래바람이 세차게 분다!"),
    [STRINGID_SANDSTORMSUBSIDED]                    = COMPOUND_STRING("모래바람이 가라앉았다!"),
    [STRINGID_SUNLIGHTGOTBRIGHT]                    = COMPOUND_STRING("햇살이 강해졌다!"),
    [STRINGID_SUNLIGHTSTRONG]                       = COMPOUND_STRING("햇살이 강하다!"), //not in gen 5+
    [STRINGID_SUNLIGHTFADED]                        = COMPOUND_STRING("햇살이 약해졌다!"),
    [STRINGID_STARTEDHAIL]                          = COMPOUND_STRING("눈이 내리기 시작했다!"),
    [STRINGID_HAILCONTINUES]                        = COMPOUND_STRING("눈이 내리고 있다!"),
    [STRINGID_HAILSTOPPED]                          = COMPOUND_STRING("눈이 그쳤다!"),
    [STRINGID_STATCHANGESGONE]                      = COMPOUND_STRING("모든 상태가\n원래대로 되돌아왔다!"),
    [STRINGID_COINSSCATTERED]                       = COMPOUND_STRING("돈이 주위에 흩어졌다!"),
    [STRINGID_TOOWEAKFORSUBSTITUTE]                 = COMPOUND_STRING("그러나 대타를 내보내기에는\n체력이 부족했다!"),
    [STRINGID_SHAREDPAIN]                           = COMPOUND_STRING("서로의 체력을 나누어 가졌다!"),
    [STRINGID_BELLCHIMED]                           = COMPOUND_STRING("방울소리가 울려 퍼졌다!"),
    [STRINGID_FAINTINTHREE]                         = COMPOUND_STRING("멸망의노래를 들은 포켓몬은\n3턴 후에 쓰러져 버린다!"),
    [STRINGID_NOPPLEFT]                             = COMPOUND_STRING("남은 PP가 없다!\p"), //not in gen 5+
    [STRINGID_BUTNOPPLEFT]                          = COMPOUND_STRING("그러나 남은 PP가 없었다!"),
    [STRINGID_PLAYERUSEDITEM]                       = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 썼다!"),
    [STRINGID_WALLYUSEDITEM]                        = COMPOUND_STRING("민진은\n{B_LAST_ITEM}{B_TXT_EULREUL} 썼다!"), //no decapitalize until it is everywhere
    [STRINGID_TRAINERBLOCKEDBALL]                   = COMPOUND_STRING("트레이너가 볼을 튕겨내 버렸다!"),
    [STRINGID_DONTBEATHIEF]                         = COMPOUND_STRING("남의 것에 손대면 도둑!"),
    [STRINGID_ITDODGEDBALL]                         = COMPOUND_STRING("피했다!\n이 녀석은 잡힐 것 같지 않군!"),
    [STRINGID_PKMNBROKEFREE]                        = COMPOUND_STRING("안돼! 포켓몬이\n볼에서 나와버렸다!"),
    [STRINGID_ITAPPEAREDCAUGHT]                     = COMPOUND_STRING("아아!\n잡았다고 생각했는데!"),
    [STRINGID_AARGHALMOSTHADIT]                     = COMPOUND_STRING("아쉽다!\n조금만 더하면 잡을 수 있었는데!"),
    [STRINGID_SHOOTSOCLOSE]                         = COMPOUND_STRING("아깝다!\n조금만 더하면 됐는데!"),
#if IS_HNS
    [STRINGID_GOTCHAPKMNCAUGHTPLAYER]               = COMPOUND_STRING("신난다-!\n{B_DEF_NAME}{B_TXT_EULREUL} 붙잡았다!{WAIT_SE}{PLAY_BGM 659}\p"),
    [STRINGID_GOTCHAPKMNCAUGHTWALLY]                = COMPOUND_STRING("신난다-!\n{B_DEF_NAME}{B_TXT_EULREUL} 붙잡았다!{WAIT_SE}{PLAY_BGM 659}{PAUSE 127}"),
#else
    [STRINGID_GOTCHAPKMNCAUGHTPLAYER]               = COMPOUND_STRING("신난다-!\n{B_DEF_NAME}{B_TXT_EULREUL} 붙잡았다!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p"),
    [STRINGID_GOTCHAPKMNCAUGHTWALLY]                = COMPOUND_STRING("신난다-!\n{B_DEF_NAME}{B_TXT_EULREUL} 붙잡았다!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}"),
#endif
    [STRINGID_GIVENICKNAMECAPTURED]                 = COMPOUND_STRING("잡은 {B_OPPONENT_MON1_NAME}에게\n닉네임을 붙이겠습니까?"),
    [STRINGID_PKMNDATAADDEDTODEX]                   = COMPOUND_STRING("{B_OPPONENT_MON1_NAME}의 데이터가 새로\n포켓몬 도감에 등록됩니다!\p"),
    [STRINGID_ITISRAINING]                          = COMPOUND_STRING("비가 내리고 있다!"),
    [STRINGID_SANDSTORMISRAGING]                    = COMPOUND_STRING("모래바람이 세차게 분다!"),
    [STRINGID_CANTESCAPE2]                          = COMPOUND_STRING("도망칠 수 없다!\p"),
    [STRINGID_PKMNIGNORESASLEEP]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 잠든 채로\n명령을 무시했다!"),
    [STRINGID_PKMNIGNOREDORDERS]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 명령을 무시했다!"),
    [STRINGID_PKMNBEGANTONAP]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 낮잠을 자기 시작했다!"),
    [STRINGID_PKMNLOAFING]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 게으름을 피우고 있다!"),
    [STRINGID_PKMNWONTOBEY]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 말을 듣지 않는다!"),
    [STRINGID_PKMNTURNEDAWAY]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 외면했다!"),
    [STRINGID_PKMNPRETENDNOTNOTICE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 모른 체했다!"),
    [STRINGID_ENEMYABOUTTOSWITCHPKMN]               = COMPOUND_STRING("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF2}{B_TXT_EULREUL} 내보내려 하고 있다\p{B_PLAYER_NAME}도 포켓몬을\n교체하겠습니까?"),
    [STRINGID_CREPTCLOSER]                          = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}에게 다가갔다!"), //safari
    [STRINGID_CANTGETCLOSER]                        = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n더 이상 다가갈 수 없다!"), //safari
    [STRINGID_PKMNWATCHINGCAREFULLY]                = COMPOUND_STRING("{B_OPPONENT_MON1_NAME}{B_TXT_EUNNEUN}\n상황을 살피고 있다!"), //safari
    [STRINGID_PKMNCURIOUSABOUTX]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}에 흥미가 있는 듯하다!"), //safari
    [STRINGID_PKMNENTHRALLEDBYX]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}에 푹 빠진 모양이다!"), //safari
    [STRINGID_PKMNIGNOREDX]                         = COMPOUND_STRING("{B_OPPONENT_MON1_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}에 눈길도 주지 않는다!"), //safari
    [STRINGID_THREWPOKEBLOCKATPKMN]                 = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}에게 포켓몬스넥을 던졌다!"), //safari
    [STRINGID_OUTOFSAFARIBALLS]                     = COMPOUND_STRING("{PLAY_SE 0x0049}방송: 딩동!\n사파리볼을 다 썼으므로 종료합니다!\p"), //safari
    [STRINGID_PKMNSITEMCUREDPARALYSIS]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n마비가 풀렸다!"),
    [STRINGID_PKMNSITEMCUREDPOISON]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n독이 해독됐다!"),
    [STRINGID_PKMNSITEMHEALEDBURN]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n화상이 나았다!"),
    [STRINGID_PKMNSITEMDEFROSTEDIT]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n얼음 상태가 나았다!"),
    [STRINGID_PKMNSITEMWOKEIT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n눈을 떴다!"),
    [STRINGID_PKMNSITEMSNAPPEDOUT]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n혼란이 풀렸다!"),
    [STRINGID_PKMNSITEMCUREDPROBLEM]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n{B_BUFF1}상태가 나았다!"),
    [STRINGID_PKMNSITEMRESTOREDHEALTH]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n체력을 회복했다!"),
    [STRINGID_PKMNSITEMRESTOREDPP]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n{B_BUFF1}의 PP를 회복했다!"),
    [STRINGID_PKMNSITEMRESTOREDSTATUS]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n상태를 원래대로 되돌렸다!"),
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE]           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EU}로 인해 조금 회복했다."),
    [STRINGID_ITEMALLOWSONLYYMOVE]                  = COMPOUND_STRING("{B_LAST_ITEM}의 효과로\n{B_CURRENT_MOVE}밖에 쓸 수 없다!\p"),
    [STRINGID_PKMNHUNGONWITHX]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EU}로 버텼다!"),
    [STRINGID_EMPTYSTRING3]                         = gText_EmptyString3,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n체력이 회복되었다."), //not in gen 5+, ability popup
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM]             = COMPOUND_STRING("모래바람이 불기 시작했다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXPREVENTSYLOSS]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EUNNEUN} 떨어지지 않는다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXINFATUATEDY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n헤롱헤롱해졌다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXMADEYINEFFECTIVE]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다..."), //not in gen 5+, ability popup
    [STRINGID_ITSUCKEDLIQUIDOOZE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n해감액을 흡수했다!"),
    [STRINGID_PKMNTRANSFORMED]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n모습이 변화했다!"),
    [STRINGID_ELECTRICITYWEAKENED]                  = COMPOUND_STRING("전기의 위력이 약해졌다!"),
    [STRINGID_FIREWEAKENED]                         = COMPOUND_STRING("불꽃의 위력이 약해졌다!"),
    [STRINGID_PKMNHIDUNDERWATER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n물속에 몸을 숨겼다!"),
    [STRINGID_PKMNSPRANGUP]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n뛰어올랐다!"),
    [STRINGID_HMMOVESCANTBEFORGOTTEN]               = COMPOUND_STRING("그건 중요한 기술입니다\n잊게 할 수 없습니다!\p"),
    [STRINGID_XFOUNDONEY]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 주워 왔다!"),
    [STRINGID_PLAYERDEFEATEDTRAINER1]               = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA]                        = COMPOUND_STRING("기분 좋은 향기가 퍼졌다!"),
    [STRINGID_ITEMSCANTBEUSEDNOW]                   = COMPOUND_STRING("여기에서는 도구를 사용할 수 없습니다!{PAUSE 64}"), //not in gen 5+, i think
    [STRINGID_USINGITEMSTATOFPKMNROSE]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"), //todo: update this, will require code changes
    [STRINGID_USINGITEMSTATOFPKMNFELL]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}떨어졌다!"),
    [STRINGID_PKMNUSEDXTOGETPUMPED]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EULREUL} 써서\n의욕이 넘치기 시작했다!"),
    [STRINGID_PKMNSXMADEYUSELESS]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게는\n효과가 없는 것 같다..."), //not in gen 5+, ability popup
    [STRINGID_PKMNTRAPPEDBYSANDTOMB]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n모래지옥에 붙잡혔다!"),
    [STRINGID_EMPTYSTRING4]                         = COMPOUND_STRING(""),
    [STRINGID_ABOOSTED]                             = COMPOUND_STRING(" 많은 양의"),
    [STRINGID_PKMNSXINTENSIFIEDSUN]                 = COMPOUND_STRING("햇살이 강해졌다!"), //not in gen 5+, ability popup
    [STRINGID_YOUTHROWABALLNOWRIGHT]                = COMPOUND_STRING("여기서 볼을 던지는 거군요\n저... 해볼게요!"),
    [STRINGID_PKMNSXTOOKATTACK]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격을 끌어들였다!"), //In gen 5+ but without naming the ability
    [STRINGID_PKMNCHOSEXASDESTINY]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 미래에 맡겼다!"),
    [STRINGID_PKMNLOSTFOCUS]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 집중력이\n떨어져서 기술을 쓸 수 없었다!"),
    [STRINGID_USENEXTPKMN]                          = COMPOUND_STRING("다음 포켓몬을 쓰겠습니까?"),
    [STRINGID_PKMNFLEDUSINGITS]                     = COMPOUND_STRING("{PLAY_SE 0x0011}{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 가지고 있던\n{B_LAST_ITEM}{B_TXT_EULREUL} 써서 도망쳤다\p"),
    [STRINGID_PKMNFLEDUSING]                        = COMPOUND_STRING("{PLAY_SE 0x0011}무사히 도망쳤다\p"), //not in gen 5+, ability popup
    [STRINGID_PKMNWASDRAGGEDOUT]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n배틀에 끌려 나왔다!\p"),
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n상태이상이 나았다!"), //not used
    [STRINGID_TRAINER1USEDITEM]                     = COMPOUND_STRING("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 썼다!"),
    [STRINGID_BOXISFULL]                            = COMPOUND_STRING("박스가 가득 찼습니다!\n더 이상 잡을 수 없습니다!\p"),
    [STRINGID_PKMNAVOIDEDATTACK]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게는\n맞지 않았다!"),
    [STRINGID_PKMNSXMADEITINEFFECTIVE]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_ABILITY} 때문에 잠들지 않는다!"), //not in gen 5+, ability popup, sweet veil
    [STRINGID_PKMNSXPREVENTSFLINCHING]              = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_EFF_ABILITY} 때문에\n풀이 죽지 않는다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYHASBURN]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 이미\n화상을 입은 상태다"),
    [STRINGID_STATSWONTDECREASE2]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의 능력은\n더 떨어지지 않는다!"),
    [STRINGID_PKMNSXBLOCKSY]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_ABILITY} 때문에\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 받지 않는다!"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXWOREOFF]                        = COMPOUND_STRING("{B_ATK_PREFIX1} {B_BUFF1}의\n효과가 떨어졌다!"),
    [STRINGID_THEWALLSHATTERED]                     = COMPOUND_STRING("벽이 깨졌다!"), //retained for legacy compatibility; screen-breaking moves print each removed screen separately
    [STRINGID_REFLECTWOREOFF]                       = COMPOUND_STRING("{B_ATK_PREFIX1}의 리플렉터가\n없어졌다!"),
    [STRINGID_LIGHTSCREENWOREOFF]                   = COMPOUND_STRING("{B_ATK_PREFIX1}의 빛의장막이\n없어졌다!"),
    [STRINGID_AURORAVEILWOREOFF]                    = COMPOUND_STRING("{B_ATK_PREFIX1}의 오로라베일이\n없어졌다!"),
    [STRINGID_NOLONGERMIST]                         = COMPOUND_STRING("{B_ATK_PREFIX3} 감싸던\n흰안개가 없어졌다!"),
    [STRINGID_PKMNSXCUREDITSYPROBLEM]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_SCR_ABILITY} 때문에\n{B_BUFF1}{B_TXT_IGA} 나았다!"), //not in gen 5+, ability popup
    [STRINGID_ATTACKERCANTESCAPE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 도망칠 수 없다!"),
    [STRINGID_PKMNOBTAINEDX]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_BUFF1}{B_TXT_EULREUL}\n손에 넣었다!"),
    [STRINGID_PKMNOBTAINEDX2]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_BUFF2}{B_TXT_EULREUL}\n손에 넣었다!"),
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_BUFF1}{B_TXT_EULREUL}\n손에 넣었다!\p{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_BUFF2}{B_TXT_EULREUL}\n손에 넣었다!"),
    [STRINGID_BUTNOEFFECT]                          = COMPOUND_STRING("그러나 효과가 없었다!"),
    [STRINGID_TWOENEMIESDEFEATED]                   = sText_TwoInGameTrainersDefeated,
    [STRINGID_TRAINER2LOSETEXT]                     = COMPOUND_STRING("{B_TRAINER2_LOSE_TEXT}"),
    [STRINGID_PKMNINCAPABLEOFPOWER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n자신의 힘을 발휘할 수 없는 것 같다!"),
    [STRINGID_GLINTAPPEARSINEYE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 눈빛이 바뀌었다!"),
    [STRINGID_PKMNGETTINGINTOPOSITION]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 태세를 가다듬었다!"),
    [STRINGID_PKMNBEGANGROWLINGDEEPLY]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 낮게 으르렁거리기 시작했다!"),
    [STRINGID_PKMNEAGERFORMORE]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 아직도 의욕이 넘친다!"),
    [STRINGID_DEFEATEDOPPONENTBYREFEREE]            = COMPOUND_STRING("{B_PLAYER_MON1_NAME}{B_TXT_EUNNEUN} 판정으로\n상대 {B_OPPONENT_MON1_NAME}{B_TXT_EULREUL} 이겼다!"),
    [STRINGID_LOSTTOOPPONENTBYREFEREE]              = COMPOUND_STRING("{B_PLAYER_MON1_NAME}{B_TXT_EUNNEUN} 판정으로\n상대 {B_OPPONENT_MON1_NAME}에게 졌다!"),
    [STRINGID_TIEDOPPONENTBYREFEREE]                = COMPOUND_STRING("{B_PLAYER_MON1_NAME}{B_TXT_EUNNEUN} 판정으로\n상대 {B_OPPONENT_MON1_NAME}{B_TXT_WAGWA} 비겼다!"),
    [STRINGID_QUESTIONFORFEITMATCH]                 = COMPOUND_STRING("승부를 포기하고\n해산하겠습니까?"),
    [STRINGID_FORFEITEDMATCH]                       = COMPOUND_STRING("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n승부를 포기했다!"),
    [STRINGID_PKMNTRANSFERREDSOMEONESPC]            = gText_PkmnTransferredSomeonesPC,
    [STRINGID_PKMNTRANSFERREDLANETTESPC]            = gText_PkmnTransferredLanettesPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL]                = gText_PkmnTransferredSomeonesPCBoxFull,
    [STRINGID_PKMNBOXLANETTESPCFULL]                = gText_PkmnTransferredLanettesPCBoxFull,
    [STRINGID_TRAINER1WINTEXT]                      = COMPOUND_STRING("{B_TRAINER1_WIN_TEXT}"),
    [STRINGID_TRAINER2WINTEXT]                      = COMPOUND_STRING("{B_TRAINER2_WIN_TEXT}"),
    [STRINGID_ENDUREDSTURDY]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격을 버텼다!"),
    [STRINGID_POWERHERB]                            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n힘이 넘쳐흐른다!"),
    [STRINGID_HURTBYITEM]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM} 때문에!\n데미지를 입었다!"),
    [STRINGID_GRAVITYINTENSIFIED]                   = COMPOUND_STRING("중력이 강해졌다!"),
    [STRINGID_TARGETWOKEUP]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n눈을 떴다!"),
    [STRINGID_TAILWINDBLEW]                         = COMPOUND_STRING("{B_ATK_TEAM1}에게\n순풍이 불기 시작했다!"),
    [STRINGID_PKMNWENTBACK]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_ATK_TRAINER_NAME}의 곁으로 돌아간다!"),
    [STRINGID_PKMNCANTUSEITEMSANYMORE]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n도구를 쓸 수 없게 되었다!"),
    [STRINGID_PKMNFLUNG]                            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 내던졌다!"),
    [STRINGID_PKMNPREVENTEDFROMHEALING]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n회복 동작을 봉인당했다!"),
    [STRINGID_PKMNSWITCHEDATKANDDEF]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격과 방어를 바꿨다!"),
    [STRINGID_PKMNSABILITYSUPPRESSED]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n특성이 효과를 발휘하지 못하게 되었다!"),
    [STRINGID_SHIELDEDFROMCRITICALHITS]             = COMPOUND_STRING("주술의 힘으로\n{B_ATK_PREFIX2}의 급소가 숨겨졌다!"),
    [STRINGID_PKMNACQUIREDABILITY]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_DEF_ABILITY}{B_TXT_IGA} 되었다!"),
    [STRINGID_POISONSPIKESSCATTERED]                = COMPOUND_STRING("{B_DEF_PREFIX1}의 발밑에\n독압정이 뿌려졌다!"),
    [STRINGID_PKMNSWITCHEDSTATCHANGES]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 상대와 자신의\n 능력 변화를 바꿨다!"),
    [STRINGID_PKMNSURROUNDEDWITHVEILOFWATER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n물의 고리를 머금었다!"),
    [STRINGID_PKMNLEVITATEDONELECTROMAGNETISM]      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n전자력으로 떠올랐다!"),
    [STRINGID_PKMNTWISTEDDIMENSIONS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n시공을 뒤틀었다!"),
    [STRINGID_POINTEDSTONESFLOAT]                   = COMPOUND_STRING("{B_DEF_PREFIX1}의 주위에\n뾰족한 바위가 떠다니기 시작했다!"),
    [STRINGID_TRAPPEDBYSWIRLINGMAGMA]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n불꽃의 소용돌이에 갇혔다!"),
    [STRINGID_VANISHEDINSTANTLY]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 모습이\n일순간에 사라졌다!"),
    [STRINGID_PROTECTEDTEAM]                        = COMPOUND_STRING("{B_ATK_PREFIX2}{B_TXT_EULREUL}\n{B_CURRENT_MOVE}가 지켜 줬다!"),
    [STRINGID_SHAREDITSGUARD]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n서로의 가드를 셰어했다!"),
    [STRINGID_SHAREDITSPOWER]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n서로의 파워를 셰어했다!"),
    [STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON]         = COMPOUND_STRING("방어와 특수방어가 바뀌는\n공간을 만들어 냈다!"),
    [STRINGID_BECAMENIMBLE]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n몸이 가벼워졌다!"),
    [STRINGID_HURLEDINTOTHEAIR]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}는\n높이 뛰어올랐다!"),
    [STRINGID_HELDITEMSLOSEEFFECTS]                 = COMPOUND_STRING("지니게 한 도구의 효과가\n없어지는 공간을 만들어 냈다!"),
    [STRINGID_FELLSTRAIGHTDOWN]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n떨어뜨리기를 당해서 땅에 떨어졌다!"),
    [STRINGID_TARGETCHANGEDTYPE]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}타입이 됐다!"),
    [STRINGID_KINDOFFER]                            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n배려를 받아들이기로 했다!"),
    [STRINGID_RESETSTARGETSSTATLEVELS]              = COMPOUND_STRING("모든 상태가\n원래대로 되돌아왔다!"),
    [STRINGID_ALLYSWITCHPOSITION]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_WAGWA}\n{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN} 자리를 바꿨다!"),
    [STRINGID_REFLECTTARGETSTYPE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_DEF_NAME_WITH_PREFIX2}{B_TXT_WAGWA}\l같은 타입이 되었다!"),
    [STRINGID_EMBARGOENDS]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n도구를 쓸 수 있게 되었다!"),
    [STRINGID_ELECTROMAGNETISM]                     = COMPOUND_STRING("전자부유"),
    [STRINGID_BUFFERENDS]                           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}의 효과가 풀렸다!"),
    [STRINGID_TELEKINESISENDS]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n텔레키네시스에서 풀려났다!"),
    [STRINGID_TAILWINDENDS]                         = COMPOUND_STRING("{B_ATK_TEAM1}의\n순풍이 멈췄다!"),
    [STRINGID_LUCKYCHANTENDS]                       = COMPOUND_STRING("{B_ATK_TEAM1}의\n주술이 풀렸다!"),
    [STRINGID_TRICKROOMENDS]                        = COMPOUND_STRING("뒤틀린 시공이 원래대로 되돌아왔다!"),
    [STRINGID_WONDERROOMENDS]                       = COMPOUND_STRING("원더룸이 해제되어\n방어와 특수방어가 원래대로 되돌아왔다!"),
    [STRINGID_MAGICROOMENDS]                        = COMPOUND_STRING("매직룸이 해제되어\n도구의 효과가 원래대로 되돌아왔다!"),
    [STRINGID_MUDSPORTENDS]                         = COMPOUND_STRING("흙놀이의 효과가\n없어졌다!"),
    [STRINGID_WATERSPORTENDS]                       = COMPOUND_STRING("물놀이의 효과가\n없어졌다!"),
    [STRINGID_GRAVITYENDS]                          = COMPOUND_STRING("중력이 원래대로 되돌아왔다!"),
    [STRINGID_AQUARINGHEAL]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n물의 고리로 체력을 회복했다!"),
    [STRINGID_ELECTRICTERRAINENDS]                  = COMPOUND_STRING("발밑의 전기가 사라졌다!"),
    [STRINGID_MISTYTERRAINENDS]                     = COMPOUND_STRING("발밑의 안개가 사라졌다!"),
    [STRINGID_PSYCHICTERRAINENDS]                   = COMPOUND_STRING("발밑의 이상한 느낌이 사라졌다!"),
    [STRINGID_GRASSYTERRAINENDS]                    = COMPOUND_STRING("발밑의 풀이 사라졌다!"),
    [STRINGID_TARGETABILITYSTATRAISE]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"),
    [STRINGID_TARGETSSTATWASMAXEDOUT]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_BUFF1}이 최고치까지 올라갔다!"),
    [STRINGID_ATTACKERABILITYSTATRAISE]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"),
    [STRINGID_POISONHEALHPUP]                       = COMPOUND_STRING("The poisoning healed {B_ATK_NAME_WITH_PREFIX2} a little bit!"), //don't think this message is displayed anymore
    [STRINGID_BADDREAMSDMG]                         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n나이트메어에 시달리고 있다!"),
    [STRINGID_MOLDBREAKERENTERS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n틀깨기!"),
    [STRINGID_TERAVOLTENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n세차게 튀는 오라를 발산하고 있다!"),
    [STRINGID_TURBOBLAZEENTERS]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n활활 타오르는 오라를 발산하고 있다!"),
    [STRINGID_SLOWSTARTENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n컨디션이 좋아지지 않는다!"),
    [STRINGID_SLOWSTARTEND]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n컨디션을 회복했다!"),
    [STRINGID_SOLARPOWERHPDROP]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} takes its toll!"), //don't think this message is displayed anymore
    [STRINGID_AFTERMATHDMG]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n상처를 입었다!"),
    [STRINGID_ANTICIPATIONACTIVATES]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n몸을 떨었다!"),
    [STRINGID_FOREWARNACTIVATES]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX2}의\n{B_BUFF1}{B_TXT_EULREUL} 간파했다!"),
    [STRINGID_ICEBODYHPGAIN]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} healed it a little bit!"), //don't think this message is displayed anymore
    [STRINGID_SNOWWARNINGHAIL]                      = COMPOUND_STRING("눈이 내리기 시작했다!"),
    [STRINGID_FRISKACTIVATES]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX2}의\n{B_LAST_ITEM}{B_TXT_EULREUL} 통찰했다!"),
    [STRINGID_UNNERVEENTERS]                        = COMPOUND_STRING("{B_EFF_TEAM1}{B_TXT_EUNNEUN} 긴장해서\n나무열매를 먹을 수 없게 되었다!"),
    [STRINGID_HARVESTBERRY]                         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 수확했다!"),
    [STRINGID_PROTEANTYPECHANGE]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}타입이 됐다!"),
    [STRINGID_SYMBIOSISITEMPASS]                    = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN} {B_SCR_NAME_WITH_PREFIX}{B_TXT_EU}로부터\n{B_LAST_ITEM}{B_TXT_EULREUL} 받았다!"),
    [STRINGID_STEALTHROCKDMG]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}에게\n뾰족한 바위가 박혔다!"),
    [STRINGID_TOXICSPIKESABSORBED]                  = COMPOUND_STRING("{B_EFF_TEAM2} 발밑의\n독압정이 사라졌다!"),
    [STRINGID_TOXICSPIKESPOISONED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n몸에 독이 퍼졌다!"),
    [STRINGID_TOXICSPIKESBADLYPOISONED]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n몸에 맹독이 퍼졌다!"),
    [STRINGID_STICKYWEBSWITCHIN]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n끈적끈적네트에 걸렸다!"),
    [STRINGID_HEALINGWISHCAMETRUE]                  = COMPOUND_STRING("치유소원이\n{B_SCR_NAME_WITH_PREFIX2}에게 전해졌다!"),
    [STRINGID_HEALINGWISHHEALED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n체력이 회복되었다!"),
    [STRINGID_LUNARDANCECAMETRUE]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n신비한 달빛에 둘러싸였다!"),
    [STRINGID_CURSEDBODYDISABLED]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EULREUL} 봉인했다!"),
    [STRINGID_ATTACKERACQUIREDABILITY]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_ATK_ABILITY}{B_TXT_IGA} 되었다!"),
    [STRINGID_TARGETABILITYSTATLOWER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EUNNEUN} 더 떨어지지 않는다!"),
    [STRINGID_TARGETSTATWONTGOHIGHER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_EUNNEUN} 더 올라가지 않는다!"),
    [STRINGID_PKMNMOVEBOUNCEDABILITY]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 되받아쳤다!"),
    [STRINGID_IMPOSTERTRANSFORM]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_DEF_NAME_WITH_PREFIX2}{B_TXT_EU}로 변신했다!"),
    [STRINGID_ASSAULTVESTDOESNTALLOW]               = COMPOUND_STRING("{B_LAST_ITEM}의 효과로\n변화 기술을 쓸 수 없다!\p"),
    [STRINGID_GRAVITYPREVENTSUSAGE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n중력이 강해서\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!\p"),
    [STRINGID_HEALBLOCKPREVENTSUSAGE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 회복봉인 때문에\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!\p"),
    [STRINGID_NOTDONEYET]                           = COMPOUND_STRING("This move effect is not done yet!\p"),
    [STRINGID_STICKYWEBUSED]                        = COMPOUND_STRING("{B_DEF_TEAM2} 발밑에\n끈적끈적네트가 펼쳐졌다!"),
    [STRINGID_QUASHSUCCESS]                         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n순서를 미뤘다!"),
    [STRINGID_PKMNBLEWAWAYTOXICSPIKES]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Toxic Spikes!"), //not used
    [STRINGID_PKMNBLEWAWAYSTICKYWEB]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Sticky Web!"), //not used
    [STRINGID_PKMNBLEWAWAYSTEALTHROCK]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away Stealth Rock!"), //not used
    [STRINGID_IONDELUGEON]                          = COMPOUND_STRING("전기 입자가 쏟아졌다!"),
    [STRINGID_TOPSYTURVYSWITCHEDSTATS]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n능력 변화가 뒤집혔다!"),
    [STRINGID_TERRAINBECOMESMISTY]                  = COMPOUND_STRING("발밑이 안개로 자욱해졌다!"),
    [STRINGID_TERRAINBECOMESGRASSY]                 = COMPOUND_STRING("발밑에 풀이 무성해졌다!"),
    [STRINGID_TERRAINBECOMESELECTRIC]               = COMPOUND_STRING("발밑에 전기가 흐르기 시작했다!"),
    [STRINGID_TERRAINBECOMESPSYCHIC]                = COMPOUND_STRING("발밑에서 이상한 느낌이 든다!"),
    [STRINGID_TARGETELECTRIFIED]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의 기술이\n송전으로 전기타입이 되었다!"),
    [STRINGID_MEGAEVOREACTING]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의 {B_LAST_ITEM}와\n{B_ATK_TRAINER_NAME}의 메가링이 반응했다!"), //actually displays the type of mega ring in inventory, but we didnt implement them :(
    [STRINGID_MEGAEVOEVOLVED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n메가{B_BUFF1}{B_TXT_EU}로 메가진화했다!"),
    [STRINGID_DRASTICALLY]                          = gText_drastically,
    [STRINGID_SEVERELY]                             = gText_severely,
    [STRINGID_INFESTATION]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n{B_EFF_NAME_WITH_PREFIX}에게 엉겨 붙었다!"),
    [STRINGID_NOEFFECTONTARGET]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게는\n효과가 없는 것 같다..."),
    [STRINGID_BURSTINGFLAMESHIT]                    = COMPOUND_STRING("분출하는 불꽃이\n{B_EFF_NAME_WITH_PREFIX2}에게 명중했다!"),
    [STRINGID_BESTOWITEMGIVING]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EULREUL}\n{B_DEF_NAME_WITH_PREFIX2}에게 지니게 했다!"),
    [STRINGID_THIRDTYPEADDED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게\n{B_BUFF1}타입이 추가되었다!"),
    [STRINGID_FELLFORFEINT]                         = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n페인트에 걸렸다!"),
    [STRINGID_POKEMONCANNOTUSEMOVE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}{B_TXT_EULREUL} 쓸 수 없다!"),
    [STRINGID_COVEREDINPOWDER]                      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}에게\n분진을 뒤집어씌웠다!"),
    [STRINGID_POWDEREXPLODES]                       = COMPOUND_STRING("{B_CURRENT_MOVE}에 반응하여\n분진이 폭발했다!"),
    [STRINGID_BELCHCANTSELECT]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n나무열매를 먹지 않아서 기술을 쓸 수 없다!\p"),
    [STRINGID_SPECTRALTHIEFSTEAL]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n올라간 능력을 빼앗았다!"),
    [STRINGID_GRAVITYGROUNDING]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n중력의 영향으로\l공중에 있을 수 없게 되었다!"),
    [STRINGID_MISTYTERRAINPREVENTS]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n미스트필드가 지켜 주고 있다!"),
    [STRINGID_GRASSYTERRAINHEALS]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n체력이 회복되었다!"),
    [STRINGID_ELECTRICTERRAINPREVENTS]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n일렉트릭필드가 지켜 주고 있다!"),
    [STRINGID_PSYCHICTERRAINPREVENTS]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n사이코필드가 지켜 주고 있다!"),
    [STRINGID_SAFETYGOGGLESPROTECTED]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM} 때문에\n분진에 당하지 않는다!"),
    [STRINGID_FLOWERVEILPROTECTED]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n플라워베일이 지켜 주고 있다!"),
    [STRINGID_AROMAVEILPROTECTED]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EULREUL}\n아로마베일이 지켜 주고 있다!"),
    [STRINGID_CELEBRATEMESSAGE]                     = COMPOUND_STRING("축하합니다, {B_PLAYER_NAME}!"),
    [STRINGID_USEDINSTRUCTEDMOVE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX2}의 지시로\n{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 기술을 사용했다!"),
    [STRINGID_THROATCHOPENDS]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can use sound-based moves again!"), // no matching modern battle message in SV or Champions
    [STRINGID_PKMNCANTUSEMOVETHROATCHOP]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n지옥찌르기 효과로 기술을 쓸 수 없다!\p"),
    [STRINGID_LASERFOCUS]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n정신을 가다듬었다!"),
    [STRINGID_GEMACTIVATES]                         = COMPOUND_STRING("{B_LAST_ITEM}{B_TXT_EUNNEUN} {B_CURRENT_MOVE}의\n위력을 강하게 했다!"),
    [STRINGID_BERRYDMGREDUCES]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN} 입는 데미지를\n{B_LAST_ITEM}{B_TXT_IGA} 약하게 했다!"),
    [STRINGID_AIRBALLOONFLOAT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n풍선 때문에 떠 있다!"),
    [STRINGID_AIRBALLOONPOP]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n풍선이 터졌다!"),
    [STRINGID_INCINERATEBURN]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}의\n{B_LAST_ITEM}{B_TXT_EUNNEUN} 녹여 버렸다!"),
    [STRINGID_BUGBITE]                              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 빼앗아 먹었다!"),
    [STRINGID_ILLUSIONWOREOFF]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n일루전이 풀렸다!"),
    [STRINGID_ATTACKERCUREDTARGETSTATUS]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cured {B_DEF_NAME_WITH_PREFIX2}'s problem!"), // no matching modern battle message in SV or Champions, not used
    [STRINGID_PURIFYTARGETSTATUSNORMAL]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n상태이상이 나았다!"),
    [STRINGID_PURIFYTARGETPOISONCURED]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 독은\n말끔하게 해독됐다!"),
    [STRINGID_PURIFYTARGETBURNCURED]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n화상이 나았다!"),
    [STRINGID_PURIFYTARGETPARALYSISCURED]           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!"),
    [STRINGID_PURIFYTARGETSLEEPCURED]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 눈을 떴다!"),
    [STRINGID_ATTACKERLOSTFIRETYPE]                 = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}의 불꽃은 다 타 버렸다!"),
    [STRINGID_HEALERCURE]                           = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n치유되었다!"), // SV/Champions Healer output
    [STRINGID_SCRIPTINGABILITYSTATRAISE]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_SCR_ABILITY}{B_TXT_EU}로 {B_BUFF1}{B_TXT_IGA} {B_BUFF2}올라갔다!"),
    [STRINGID_RECEIVERABILITYTAKEOVER]              = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_SCR_ABILITY}{B_TXT_EULREUL} 이어받았다!"),
    [STRINGID_PKNMABSORBINGPOWER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n파워를 모으고 있다!"),
    [STRINGID_NOONEWILLBEABLETORUNAWAY]             = COMPOUND_STRING("다음 턴은 도망갈 수 없다!"),
    [STRINGID_DESTINYKNOTACTIVATES]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM} 때문에 헤롱헤롱해졌다!"),
    [STRINGID_CLOAKEDINAFREEZINGLIGHT]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n차가운 빛에 둘러싸였다!"),
    [STRINGID_CLEARAMULETWONTLOWERSTATS]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}의 효과로 능력이 떨어지지 않는다!"),
    [STRINGID_FERVENTWISHREACHED]                   = COMPOUND_STRING("{B_ATK_TRAINER_NAME}의 강한 기도가\n{B_ATK_NAME_WITH_PREFIX2}에게 닿았다!"),
    [STRINGID_AIRLOCKACTIVATES]                     = COMPOUND_STRING("날씨의 영향이 없어졌다."),
    [STRINGID_PRESSUREENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n프레셔를 발산하고 있다!"),
    [STRINGID_DARKAURAENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n다크오라를 발산하고 있다!"),
    [STRINGID_FAIRYAURAENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n페어리오라를 발산하고 있다!"),
    [STRINGID_AURABREAKENTERS]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n모든 오라를 제압한다!"),
    [STRINGID_COMATOSEENTERS]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n비몽사몽 상태!"),
    [STRINGID_SCREENCLEANERENTERS]                  = COMPOUND_STRING("All screens on the field were cleansed!"), // no matching modern battle message in SV or Champions, not used
    [STRINGID_FETCHEDPOKEBALL]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 주워 왔다!"),
    [STRINGID_ASANDSTORMKICKEDUP]                   = COMPOUND_STRING("모래바람이 불기 시작했다!"),
    [STRINGID_PKMNSWILLPERISHIN3TURNS]              = COMPOUND_STRING("Both Pokémon will perish in three turns!"),  //don't think this message is displayed anymore
    [STRINGID_AURAFLAREDTOLIFE]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n오라에 둘러싸였다!"),
    [STRINGID_ASONEENTERS]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n두 가지 특성을 겸비한다!"),
    [STRINGID_CURIOUSMEDICINEENTERS]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}의\n능력 변화가 원래대로 되돌아왔다!"),
    [STRINGID_CANACTFASTERTHANKSTO]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_BUFF1}{B_TXT_EU}로\n행동이 빨라졌다!"),
    [STRINGID_MICLEBERRYACTIVATES]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_LAST_ITEM}{B_TXT_EU}로\n다음에 쓸 기술이 명중하기 쉬워졌다!"),
    [STRINGID_PKMNSHOOKOFFTHETAUNT]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n도발의 효과가 풀렸다!"),
    [STRINGID_PKMNGOTOVERITSINFATUATION]            = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n헤롱헤롱 상태가 나았다!"),
    [STRINGID_ITEMCANNOTBEREMOVED]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n도구를 빼앗을 수 없다!"),
    [STRINGID_STICKYBARBTRANSFER]                   = COMPOUND_STRING("The {B_LAST_ITEM} attached itself to {B_ATK_NAME_WITH_PREFIX2}!"), //not used
    [STRINGID_PKMNBURNHEALED]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의\n화상이 나았다!"),
    [STRINGID_REDCARDACTIVATE]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 레드카드를\n{B_ATK_NAME_WITH_PREFIX2}에게 꺼내 들었다!"),
    [STRINGID_EJECTBUTTONACTIVATE]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM} 때문에 돌아간다!"),
    [STRINGID_ATKGOTOVERINFATUATION]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n헤롱헤롱 상태가 나았다!"),
    [STRINGID_TORMENTEDNOMORE]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n트집 효과가 사라졌다!"),
    [STRINGID_HEALBLOCKEDNOMORE]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n회복봉인 효과가 사라졌다!"),
    [STRINGID_ATTACKERBECAMEFULLYCHARGED]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n유대의 힘이 넘쳐흐른다!\p"),
    [STRINGID_ATTACKERBECAMEASHSPECIES]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n지우개굴닌자로 변했다!\p"),
    [STRINGID_EXTREMELYHARSHSUNLIGHT]               = COMPOUND_STRING("햇살이 아주 강해졌다!"),
    [STRINGID_EXTREMESUNLIGHTFADED]                 = COMPOUND_STRING("햇살이 원래대로 되돌아왔다!"),
    [STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT]     = COMPOUND_STRING("강한 햇살의 영향으로\n물타입의 공격이 증발했다!"),
    [STRINGID_EXTREMELYHARSHSUNLIGHTWASNOTLESSENED] = COMPOUND_STRING("강한 햇살의 기세는 멈추지 않는다!"),
    [STRINGID_HEAVYRAIN]                            = COMPOUND_STRING("강한 비가 내리기 시작했다!"),
    [STRINGID_HEAVYRAINLIFTED]                      = COMPOUND_STRING("강한 비가 그쳤다!"),
    [STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN]         = COMPOUND_STRING("강한 비의 영향으로\n불꽃타입의 공격이 사라졌다!"),
    [STRINGID_NORELIEFROMHEAVYRAIN]                 = COMPOUND_STRING("강한 비의 기세는 멈추지 않는다!"),
    [STRINGID_MYSTERIOUSAIRCURRENT]                 = COMPOUND_STRING("수수께끼의 난기류가\n비행포켓몬을 지킨다!"),
    [STRINGID_STRONGWINDSDISSIPATED]                = COMPOUND_STRING("수수께끼의 난기류가 가라앉았다!"),
    [STRINGID_MYSTERIOUSAIRCURRENTBLOWSON]          = COMPOUND_STRING("수수께끼의 난기류의 기세는 멈추지 않는다!"),
    [STRINGID_ATTACKWEAKENEDBSTRONGWINDS]           = COMPOUND_STRING("수수께끼의 난기류가 공격을 약하게 만들었다!"),
    [STRINGID_STUFFCHEEKSCANTSELECT]                = COMPOUND_STRING("나무열매를 지니고 있지 않아 기술을 쓸 수 없다!\p"),
    [STRINGID_PKMNREVERTEDTOPRIMAL]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 원시회귀!\n원시의 모습으로 돌아갔다!"),
    [STRINGID_BUTPOKEMONCANTUSETHEMOVE]             = COMPOUND_STRING("하지만 {B_ATK_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n사용할 수 없었다!"),
    [STRINGID_BUTHOOPACANTUSEIT]                    = COMPOUND_STRING("하지만 지금의 {B_ATK_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n사용할 수 없었다!"),
    [STRINGID_BROKETHROUGHPROTECTION]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX2}의\n방어를 깨뜨렸다!"),
    [STRINGID_ABILITYALLOWSONLYMOVE]                = COMPOUND_STRING("{B_ATK_ABILITY}의 효과로\n{B_CURRENT_MOVE}밖에 쓸 수 없다!\p"),
    [STRINGID_SWAPPEDABILITIES]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n서로의 특성을 교체했다!"),
    [STRINGID_PKMNHEALEDPOISON]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}의 독은 말끔하게 해독됐다!"),
    [STRINGID_BATTLERTYPECHANGEDTO]                 = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}타입이 됐다!"),
    [STRINGID_BOTHCANNOLONGERESCAPE]                = COMPOUND_STRING("서로의 포켓몬은\n도망칠 수 없게 되었다!"),
    [STRINGID_CANTESCAPEDUETOUSEDMOVE]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 배수의 진을 쳐서\n도망칠 수 없게 되었다!"),
    [STRINGID_PKMNBECAMEWEAKERTOFIRE]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n불꽃에 약해졌다!"),
    [STRINGID_ABOUTTOUSEPOLTERGEIST]                = COMPOUND_STRING("{B_LAST_ITEM}{B_TXT_IGA}\n{B_EFF_NAME_WITH_PREFIX}에게 덤벼들었다!"),
    [STRINGID_CANTESCAPEBECAUSEOFCURRENTMOVE]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n문어굳히기 때문에 도망칠 수 없게 되었다!"),
    [STRINGID_NEUTRALIZINGGASENTERS]                = COMPOUND_STRING("주위가 화학변화가스로 가득 찼다!"),
    [STRINGID_NEUTRALIZINGGASOVER]                  = COMPOUND_STRING("화학변화가스의 효과가 사라졌다!"),
    [STRINGID_TARGETTOOHEAVY]                       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n너무 무거워서 들 수 없다!"),
    [STRINGID_PKMNTOOKTARGETHIGH]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} {B_DEF_NAME_WITH_PREFIX2}{B_TXT_EULREUL}\n상공으로 데려갔다!"),
    [STRINGID_PKMNINSNAPTRAP]                       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n집게덫에 붙잡혔다!"),
    [STRINGID_METEORBEAMCHARGING]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}에게서\n우주의 힘이 넘쳐난다!"),
    [STRINGID_HEATUPBEAK]                           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n부리를 가열하기 시작했다!"),
    [STRINGID_COURTCHANGE]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n서로의 필드 효과를 교체했다!"),
    [STRINGID_ZPOWERSURROUNDS]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\nZ파워에 몸이 둘러싸였다!"),
    [STRINGID_ZMOVEUNLEASHED]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 뿜어내는\n전력의 Z기술!"),
    [STRINGID_ZMOVERESETSSTATS]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} Z파워로\n떨어진 능력을 원래대로 되돌렸다!"),
    [STRINGID_ZMOVEALLSTATSUP]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\nZ파워로 능력이 올라갔다!"),
    [STRINGID_ZMOVEZBOOSTCRIT]                      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} Z파워로\n급소에 맞기 쉬워졌다!"),
    [STRINGID_ZMOVERESTOREHP]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\nZ파워로 체력을 회복했다!"),
    [STRINGID_ZMOVESTATUP]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\nZ파워로 능력이 올라갔다!"),
    [STRINGID_ZMOVEHPTRAP]                          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n체력을 Z파워로 회복했다!"),
    [STRINGID_ATTACKEREXPELLEDTHEPOISON]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 걱정 끼치지 않으려고\n스스로 독을 치료했다!"),
    [STRINGID_ATTACKERSHOOKITSELFAWAKE]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 걱정 끼치지\n않으려고 어떻게든 잠에서 깨어났다!"),
    [STRINGID_ATTACKERBROKETHROUGHPARALYSIS]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 걱정 끼치지 않으려고\n정신력으로 마비를 치료했다!"),
    [STRINGID_ATTACKERHEALEDITSBURN]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 걱정 끼치지 않으려고\n근성으로 화상을 치료했다!"),
    [STRINGID_ATTACKERMELTEDTHEICE]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 걱정 끼치지 않으려고\n열심히 얼음을 녹였다!"),
    [STRINGID_TARGETTOUGHEDITOUT]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n슬프지 않게 하려고 버텼다!"),
    [STRINGID_ATTACKERLOSTELECTRICTYPE]             = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n전기를 다 써 버렸다!"),
    [STRINGID_ATTACKERSWITCHEDSTATWITHTARGET]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n서로의 {B_BUFF1}{B_TXT_EULREUL} 교체했다!"),
    [STRINGID_BEINGHITCHARGEDPKMNWITHPOWER]         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n{B_CURRENT_MOVE}에 맞아 충전되었다!"),
    [STRINGID_SUNLIGHTACTIVATEDABILITY]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n쾌청에 의해 고대활성을 발동했다!"),
    [STRINGID_STATWASHEIGHTENED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_BUFF1}{B_TXT_IGA} 강화되었다!"),
    [STRINGID_ELECTRICTERRAINACTIVATEDABILITY]      = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_EUNNEUN}\n일렉트릭필드에 의해 쿼크차지를 발동했다!"),
    [STRINGID_ABILITYWEAKENEDSURROUNDINGMONSSTAT]   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 {B_SCR_ABILITY}에 의해\n주위의 {B_BUFF1}{B_TXT_IGA} 약해졌다!\p"),
    [STRINGID_ATTACKERGAINEDSTRENGTHFROMTHEFALLEN]  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n쓰러진 동료에게서 힘을 받았다!"),
    [STRINGID_PKMNSABILITYPREVENTSABILITY]          = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}'s {B_SCR_ABILITY} prevents {B_DEF_NAME_WITH_PREFIX2}'s {B_DEF_ABILITY} from working!"), //not in gen 5+, ability popup, not used
    [STRINGID_PREPARESHELLTRAP]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n트랩셸을 설치했다!"),
    [STRINGID_SHELLTRAPDIDNTWORK]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n트랩셸은 불발로 끝났다!"),
    [STRINGID_SPIKESDISAPPEAREDFROMTEAM]            = COMPOUND_STRING("{B_ATK_TEAM2} 발밑의\n압정이 사라졌다!"),
    [STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("{B_ATK_TEAM2} 발밑의\n독압정이 사라졌다!"),
    [STRINGID_STICKYWEBDISAPPEAREDFROMTEAM]         = COMPOUND_STRING("{B_ATK_TEAM2} 발밑의\n끈적끈적네트가 사라졌다!"),
    [STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("{B_ATK_TEAM2} 주변의\n스텔스록이 사라졌다!"),
    [STRINGID_COULDNTFULLYPROTECT]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n공격을 막아 내지 못하고 데미지를 입었다!"),
    [STRINGID_STOCKPILEDEFFECTWOREOFF]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n비축해 두었던 효과가 사라졌다!"),
    [STRINGID_PKMNREVIVEDREADYTOFIGHT]              = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}\n정신을 차려 싸울 수 있게 되었다!"),
    [STRINGID_ITEMRESTOREDSPECIESHEALTH]            = COMPOUND_STRING("{B_BUFF1}의\n체력이 회복되었다."),
    [STRINGID_ITEMCUREDSPECIESSTATUS]               = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}\n건강해졌다!"),
    [STRINGID_ITEMRESTOREDSPECIESPP]                = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}\nPP가 회복되었다!"),
    [STRINGID_THUNDERCAGETRAPPED]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_EFF_NAME_WITH_PREFIX2}{B_TXT_EULREUL} 번개우리로 가뒀다!"),
    [STRINGID_PKMNHURTBYFROSTBITE]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n동상 데미지를 입고 있다!"),
    [STRINGID_PKMNGOTFROSTBITE]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n동상을 입었다!"),
    [STRINGID_PKMNSITEMHEALEDFROSTBITE]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 {B_LAST_ITEM}{B_TXT_EUNNEUN}\n동상을 치료했다!"),
    [STRINGID_ATTACKERHEALEDITSFROSTBITE]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 걱정 끼치지 않으려고\n열심히 얼음을 녹였다!"),
    [STRINGID_PKMNFROSTBITEHEALED]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n동상이 나았다!"),
    [STRINGID_PKMNFROSTBITEHEALEDBY]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n{B_CURRENT_MOVE} 때문에 동상이 나았다!"),
    [STRINGID_MIRRORHERBCOPIED]                     = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 흉내허브를 써서\n상대의 능력 변화를 흉내 냈다!"),
    [STRINGID_STARTEDSNOW]                          = COMPOUND_STRING("눈이 내리기 시작했다!"),
    [STRINGID_SNOWCONTINUES]                        = COMPOUND_STRING("눈이 내리고 있다."), // no matching modern battle message in SV or Champions
    [STRINGID_SNOWSTOPPED]                          = COMPOUND_STRING("눈이 그쳤다."),
    [STRINGID_SNOWWARNINGSNOW]                      = COMPOUND_STRING("눈이 내리기 시작했다!"),
    [STRINGID_PKMNITEMMELTED]                       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_DEF_NAME_WITH_PREFIX2}{B_TXT_IGA} 지닌 {B_LAST_ITEM}{B_TXT_EULREUL} 녹여 버렸다!"),
    [STRINGID_ULTRABURSTREACTING]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX2}{B_TXT_EU}로부터\n눈부신 빛이 넘쳐흐른다!"),
    [STRINGID_ULTRABURSTCOMPLETED]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 울트라버스트로 인해\n새로운 모습을 드러냈다!"),
    [STRINGID_TEAMGAINEDEXP]                        = COMPOUND_STRING("학습장치로 각자\n경험치를 얻었다!\p"),
    [STRINGID_CURRENTMOVECANTSELECT]                = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN}\n쓸 수 없다!\p"),
    [STRINGID_TARGETISBEINGSALTCURED]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n소금에 절여졌다!"),
    [STRINGID_TARGETISHURTBYSALTCURE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_BUFF1}의 데미지를 입고 있다."),
    [STRINGID_TARGETCOVEREDINSTICKYCANDYSYRUP]      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n물엿범벅이 되었다!"),
    [STRINGID_SHARPSTEELFLOATS]                     = COMPOUND_STRING("{B_DEF_TEAM2} 주변에\n뾰족한 강철이 떠다니기 시작했다!"),
    [STRINGID_SHARPSTEELDMG]                        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게\n뾰족한 강철이 박혔다!"),
    [STRINGID_PKMNBLEWAWAYSHARPSTEEL]               = COMPOUND_STRING("{B_ATK_TEAM2} 주변의\n강철이 사라졌다!"), // no matching modern battle message in SV or Champions
    [STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM]        = COMPOUND_STRING("{B_ATK_TEAM2} 주변의\n강철이 사라졌다!"),
    [STRINGID_TEAMTRAPPEDWITHVINES]                 = COMPOUND_STRING("{B_EFF_TEAM1} 포켓몬은\n채찍의 맹타에 휩싸였다!"),
    [STRINGID_PKMNHURTBYVINES]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n거다이편달이 퍼붓는 맹타에 아파하고 있다!"),
    [STRINGID_TEAMCAUGHTINVORTEX]                   = COMPOUND_STRING("{B_EFF_TEAM1} 포켓몬은\n거친 물살에 휩싸였다!"),
    [STRINGID_PKMNHURTBYVORTEX]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n거다이포격의 물살에 삼켜져서 괴로워하고 있다!"),
    [STRINGID_TEAMSURROUNDEDBYFIRE]                 = COMPOUND_STRING("{B_EFF_TEAM1} 포켓몬은\n불꽃에 휩싸였다!"),
    [STRINGID_PKMNBURNINGUP]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n거다이옥염의 불꽃에 휩싸여서 뜨거워하고 있다!"),
    [STRINGID_TEAMSURROUNDEDBYROCKS]                = COMPOUND_STRING("{B_EFF_TEAM1} 포켓몬은\n바위에 둘러싸였다!"),
    [STRINGID_PKMNHURTBYROCKSTHROWN]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n거다이석탄으로 날아든 바위에 맞아서 아파하고 있다!"),
    [STRINGID_MOVEBLOCKEDBYDYNAMAX]                 = COMPOUND_STRING("다이맥스의\n힘으로 튕겨 냈다!"),
    [STRINGID_ZEROTOHEROTRANSFORMATION]             = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n변신하고 돌아왔다!"),
    [STRINGID_THETWOMOVESBECOMEONE]                 = COMPOUND_STRING("2개의 기술이 하나가 되었다!\n콤비네이션 기술이다!{PAUSE 16}"),
    [STRINGID_ARAINBOWAPPEAREDONSIDE]               = COMPOUND_STRING("{B_ATK_TEAM2} 하늘에 무지개가 걸렸다!"),
    [STRINGID_THERAINBOWDISAPPEARED]                = COMPOUND_STRING("{B_ATK_TEAM2} 하늘에서 무지개가 사라졌다!"),
    [STRINGID_WAITINGFORPARTNERSMOVE]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_ATK_PARTNER_NAME}{B_TXT_EULREUL} 기다리고 있다...{PAUSE 16}"),
    [STRINGID_SEAOFFIREENVELOPEDSIDE]               = COMPOUND_STRING("{B_DEF_TEAM2} 주변이\n불바다에 둘러싸였다!"),
    [STRINGID_HURTBYTHESEAOFFIRE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n불바다의 데미지를 입었다!"),
    [STRINGID_THESEAOFFIREDISAPPEARED]              = COMPOUND_STRING("{B_ATK_TEAM2} 주변의\n불바다가 사라졌다!"),
    [STRINGID_SWAMPENVELOPEDSIDE]                   = COMPOUND_STRING("{B_DEF_TEAM2} 주변에\n습지초원이 펼쳐졌다!"),
    [STRINGID_THESWAMPDISAPPEARED]                  = COMPOUND_STRING("{B_ATK_TEAM2} 주변의\n습지초원이 사라졌다!"),
    [STRINGID_PKMNTELLCHILLINGRECEPTIONJOKE]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n썰렁한 개그를 선보였다!"),
    [STRINGID_HOSPITALITYRESTORATION]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX2}{B_TXT_IGA} 내온 차를\n{B_EFF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 모두 비웠다!"),
    [STRINGID_ELECTROSHOTCHARGING]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n전기를 흡수했다!"),
    [STRINGID_ITEMWASUSEDUP]                        = COMPOUND_STRING("{B_LAST_ITEM}{B_TXT_EUNNEUN} 역할을 다하고\n사라져 버렸다..."),
    [STRINGID_ATTACKERLOSTITSTYPE]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}의\n타입이 원래대로 되돌아왔다!"),
    [STRINGID_SHEDITSTAIL]                          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n꼬리를 잘라 대타로 삼았다!"),
    [STRINGID_CLOAKEDINAHARSHLIGHT]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}로부터\n눈부신 빛이 넘쳐흐른다!"),
    [STRINGID_SUPERSWEETAROMAWAFTS]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX2}에게\n향기가 배어서 가시지 않게 되었다!"),
    [STRINGID_DIMENSIONSWERETWISTED]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n시공을 뒤틀었다!"),
    [STRINGID_BIZARREARENACREATED]                  = COMPOUND_STRING("지니게 한 도구의 효과가\n없어지는 공간을 만들어 냈다!"),
    [STRINGID_BIZARREAREACREATED]                   = COMPOUND_STRING("방어와 특수방어가 바뀌는\n공간을 만들어 냈다!"),
    [STRINGID_TIDYINGUPCOMPLETE]                    = COMPOUND_STRING("정리정돈 끝!"),
    [STRINGID_PKMNTERASTALLIZEDINTO]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} terastallized into the {B_BUFF1} type!"), //테라스탈
    [STRINGID_BOOSTERENERGYACTIVATES]               = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}에 의해 {B_SCR_ABILITY}{B_TXT_EULREUL} 발동했다!"),
    [STRINGID_FOGCREPTUP]                           = COMPOUND_STRING("안개가 자욱이 끼기 시작했다!"),
    [STRINGID_FOGISDEEP]                            = COMPOUND_STRING("안개가 짙다."), // no matching modern battle message in SV or Champions
    [STRINGID_FOGLIFTED]                            = COMPOUND_STRING("안개가 걷혔다."),
    [STRINGID_PKMNMADESHELLGLEAM]                   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 등껍질을 빛나게 하여\n타입 상성을 왜곡시켰다!!"),
    [STRINGID_FICKLEBEAMDOUBLED]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n전력을 다하기 시작했다!"),
    [STRINGID_COMMANDERACTIVATES]                   = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 사령탑이 되어\n{B_DEF_NAME_WITH_PREFIX2}에게 삼켜졌다!"),
    [STRINGID_POKEFLUTECATCHY]                      = COMPOUND_STRING("{B_PLAYER_NAME} played the {B_LAST_ITEM}.\pNow, that's a catchy tune!"), //frlg
    [STRINGID_POKEFLUTE]                            = COMPOUND_STRING("{B_PLAYER_NAME} played the {B_LAST_ITEM}."), //frlg
    [STRINGID_MONHEARINGFLUTEAWOKE]                 = COMPOUND_STRING("The Pokémon hearing the flute awoke!"), //frlg
    [STRINGID_SUNLIGHTISHARSH]                      = COMPOUND_STRING("햇살이 강하다!"),
    [STRINGID_ITISHAILING]                          = COMPOUND_STRING("It's hailing!"), // replaced by snow in modern SV/Champions messages
    [STRINGID_ITISSNOWING]                          = COMPOUND_STRING("눈이 내리고 있다!"),
    [STRINGID_ISCOVEREDWITHGRASS]                   = COMPOUND_STRING("발밑에 풀이 무성하다!"),
    [STRINGID_MISTSWIRLSAROUND]                     = COMPOUND_STRING("발밑이 안개로 자욱하다!"),
    [STRINGID_ELECTRICCURRENTISRUNNING]             = COMPOUND_STRING("발밑에 전기가 흐르고 있다!"),
    [STRINGID_SEEMSWEIRD]                           = COMPOUND_STRING("발밑의 느낌이 이상하다!"),
    [STRINGID_WAGGLINGAFINGER]                      = COMPOUND_STRING("손가락을 흔들었더니\n{B_CURRENT_MOVE} 나왔다!"),
    [STRINGID_BLOCKEDBYSLEEPCLAUSE]                 = COMPOUND_STRING("Sleep Clause kept {B_DEF_NAME_WITH_PREFIX2} awake!"), // no official SV/Champions battle message
    [STRINGID_SUPEREFFECTIVETWOFOES]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_WAGWA} {B_DEF_PARTNER_NAME}에게\n효과가 굉장했다!"),
    [STRINGID_NOTVERYEFFECTIVETWOFOES]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_WAGWA} {B_DEF_PARTNER_NAME}에게\n효과가 별로였다."),
    [STRINGID_ITDOESNTAFFECTTWOFOES]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_WAGWA} {B_DEF_PARTNER_NAME}에게는\n효과가 없는 것 같다…"),
    [STRINGID_SENDCAUGHTMONPARTYORBOX]              = COMPOUND_STRING("{B_DEF_NAME}{B_TXT_EULREUL}\n지닌 포켓몬에 넣겠습니까?"),
    [STRINGID_PKMNSENTTOPCAFTERCATCH]               = gText_PkmnSentToPCAfterCatch,
    [STRINGID_PKMNDYNAMAXED]                        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} grew huge into its Dynamax form!"), //다이맥스
    [STRINGID_PKMNGIGANTAMAXED]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} grew huge into its Gigantamax form!"), //다이맥스
    [STRINGID_TIMETODYNAMAX]                        = COMPOUND_STRING("Time to Dynamax!"), //다이맥스
    [STRINGID_TIMETOGIGANTAMAX]                     = COMPOUND_STRING("Time to Gigantamax!"), //다이맥스
    [STRINGID_QUESTIONFORFEITBATTLE]                = COMPOUND_STRING("승부를 포기하고 항복합니다.\n패배로 처리됩니다만 괜찮겠습니까?"),
    [STRINGID_POWERCONSTRUCTPRESENCEOFMANY]         = COMPOUND_STRING("많은 기척이 느껴진다...!"),
    [STRINGID_POWERCONSTRUCTTRANSFORM]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n퍼펙트폼으로 바뀌었다!"),
    [STRINGID_ABILITYSHIELDPROTECTS]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}의 효과로 특성을 보호받고 있다!"),
    [STRINGID_MONTOOSCAREDTOMOVE]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is too scared to move!"), //frlg
    [STRINGID_GHOSTGETOUTGETOUT]                    = COMPOUND_STRING("GHOST: Get out…… Get out……"), //frlg
    [STRINGID_SILPHSCOPEUNVEILED]                   = COMPOUND_STRING("SILPH SCOPE unveiled the GHOST's\nidentity!"), //frlg
    [STRINGID_GHOSTWASMAROWAK]                      = COMPOUND_STRING("The GHOST was MAROWAK!\p\n"), //frlg
    [STRINGID_TRAINER1MON1COMEBACK]                 = COMPOUND_STRING("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}\n돌아와!"),
    [STRINGID_THREWROCK]                            = COMPOUND_STRING("{B_PLAYER_NAME} threw a ROCK\nat the {B_OPPONENT_MON1_NAME}!"), //frlg
    [STRINGID_THREWBAIT]                            = COMPOUND_STRING("{B_PLAYER_NAME} threw some BAIT\nat the {B_OPPONENT_MON1_NAME}!"), //frlg
    [STRINGID_PKMNANGRY]                            = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is angry!"), //frlg
    [STRINGID_PKMNEATING]                           = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is eating!"), //frlg
    [STRINGID_PKMNDISGUISEWASBUSTED]                = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n정체가 드러났다!"), //frlg
    [STRINGID_ZENMODETRIGGERED]                     = COMPOUND_STRING("{B_SCR_ABILITY} 발동!"),
    [STRINGID_ZENMODEENDED]                         = COMPOUND_STRING("{B_SCR_ABILITY} 해제!"),
    [STRINGID_SCRCUREDPARALYSIS]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n몸저림이 풀렸다!"),
    [STRINGID_SCRCUREDPOISON]                       = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의 독은\n말끔하게 해독됐다!"),
    [STRINGID_SCRCUREDBURN]                         = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n화상이 나았다!"),
    [STRINGID_SCRCUREDSLEEP]                        = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN} 눈을 떴다!"),
    [STRINGID_SCRCUREDCONFUSION]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n혼란이 풀렸다!"),
    [STRINGID_PARTYCUREDPARALYSIS]                  = COMPOUND_STRING("{B_BUFF1}의\n몸저림이 풀렸다!"),
    [STRINGID_PARTYCUREDPOISON]                     = COMPOUND_STRING("{B_BUFF1}의 독은\n말끔하게 해독됐다!"),
    [STRINGID_PARTYCUREDBURN]                       = COMPOUND_STRING("{B_BUFF1}의\n화상이 나았다!"),
    [STRINGID_PARTYCUREDSLEEP]                      = COMPOUND_STRING("{B_BUFF1}{B_TXT_EUNNEUN} 눈을 떴다!"),
    [STRINGID_PARTYCUREDFREEZE]                     = COMPOUND_STRING("{B_BUFF1}의\n얼음이 녹았다!"),
    [STRINGID_PARTYCUREDFROSTBITE]                  = COMPOUND_STRING("{B_BUFF1}의\n동상이 나았다!"),
    [STRINGID_PKMNATKNOTLOWERED]                    = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}의\n공격은 떨어지지 않는다!"),
    [STRINGID_WILDPKMNDROPPEDITEM]                  = COMPOUND_STRING("{B_SCR_NAME_WITH_PREFIX}{B_TXT_EUNNEUN}\n{B_LAST_ITEM}{B_TXT_EULREUL} 떨어뜨렸다!\p"),
    [STRINGID_DROPPEDITEMBAGFULL]                   = COMPOUND_STRING("가방이 가득 차서\n아이템을 주울 수 없습니다!\p"),
    [STRINGID_MOSTLYINEFFECTIVE]                    = COMPOUND_STRING("효과가 매우 별로인 듯하다."),
    [STRINGID_EXTREMELYEFFECTIVE]                   = COMPOUND_STRING("효과가 매우 굉장했다!!"),
    [STRINGID_NOTVERYEFFECTIVEONDEF]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게\n효과가 별로인 듯하다."),
    [STRINGID_SUPEREFFECTIVEONDEF]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게\n효과가 굉장했다!"),
    [STRINGID_MOSTLYINEFFECTIVEONDEF]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게\n효과가 매우 별로인 듯하다."),
    [STRINGID_EXTREMELYEFFECTIVEONDEF]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}에게\n효과가 매우 굉장했다!!"),
    [STRINGID_EXTREMELYEFFECTIVETWOFOES]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_WAGWA} {B_DEF_PARTNER_NAME}에게\n효과가 매우 굉장했다!!"),
    [STRINGID_MOSTLYINEFFECTIVETWOFOES]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}{B_TXT_WAGWA} {B_DEF_PARTNER_NAME}에게\n효과가 매우 별로였다."),
    [STRINGID_CRITICALHITONDEF]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX2}의\n급소에 맞았다!"),
    [STRINGID_S]                                    = COMPOUND_STRING("s"), //영문 전용 복수형
    [STRINGID_LOSTSOMEOFITSHP]                      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}의\n생명이 조금 깎였다!"),
};

const u16 gTrainerUsedItemStringIds[] =
{
    STRINGID_PLAYERUSEDITEM, STRINGID_TRAINER1USEDITEM
};

const u16 gZEffectStringIds[] =
{
    [B_MSG_Z_RESET_STATS] = STRINGID_ZMOVERESETSSTATS,
    [B_MSG_Z_ALL_STATS_UP]= STRINGID_ZMOVEALLSTATSUP,
    [B_MSG_Z_BOOST_CRITS] = STRINGID_ZMOVEZBOOSTCRIT,
    [B_MSG_Z_FOLLOW_ME]   = STRINGID_PKMNCENTERATTENTION,
    [B_MSG_Z_RECOVER_HP]  = STRINGID_ZMOVERESTOREHP,
    [B_MSG_Z_STAT_UP]     = STRINGID_ZMOVESTATUP,
    [B_MSG_Z_HP_TRAP]     = STRINGID_ZMOVEHPTRAP,
};

const u16 gMentalHerbCureStringIds[] =
{
    [B_MSG_MENTALHERBCURE_INFATUATION] = STRINGID_ATKGOTOVERINFATUATION,
    [B_MSG_MENTALHERBCURE_TORMENT]     = STRINGID_TORMENTEDNOMORE,
    [B_MSG_MENTALHERBCURE_DISABLE]     = STRINGID_PKMNMOVEDISABLEDNOMORE,
    [B_MSG_MENTALHERBCURE_HEALBLOCK]   = STRINGID_HEALBLOCKEDNOMORE,
    [B_MSG_MENTALHERBCURE_ENCORE]      = STRINGID_PKMNENCOREENDED,
    [B_MSG_MENTALHERBCURE_TAUNT]       = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gStartingStatusStringIds[B_MSG_STARTING_STATUS_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_SET_TRICK_ROOM]       = STRINGID_DIMENSIONSWERETWISTED,
    [B_MSG_SET_MAGIC_ROOM]       = STRINGID_BIZARREARENACREATED,
    [B_MSG_SET_WONDER_ROOM]      = STRINGID_BIZARREAREACREATED,
    [B_MSG_SET_TAILWIND]         = STRINGID_TAILWINDBLEW,
    [B_MSG_SET_RAINBOW]          = STRINGID_ARAINBOWAPPEAREDONSIDE,
    [B_MSG_SET_SEA_OF_FIRE]      = STRINGID_SEAOFFIREENVELOPEDSIDE,
    [B_MSG_SET_SWAMP]            = STRINGID_SWAMPENVELOPEDSIDE,
    [B_MSG_SET_SPIKES]           = STRINGID_SPIKESSCATTERED,
    [B_MSG_SET_POISON_SPIKES]    = STRINGID_POISONSPIKESSCATTERED,
    [B_MSG_SET_STICKY_WEB]       = STRINGID_STICKYWEBUSED,
    [B_MSG_SET_STEALTH_ROCK]     = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SET_SHARP_STEEL]      = STRINGID_SHARPSTEELFLOATS,
};

const u16 gTerrainStringIds[B_MSG_TERRAIN_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY] = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC] = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY] = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_TERRAIN_END_MISTY] = STRINGID_MISTYTERRAINENDS,
    [B_MSG_TERRAIN_END_ELECTRIC] = STRINGID_ELECTRICTERRAINENDS,
    [B_MSG_TERRAIN_END_PSYCHIC] = STRINGID_PSYCHICTERRAINENDS,
    [B_MSG_TERRAIN_END_GRASSY] = STRINGID_GRASSYTERRAINENDS,
};

const u16 gTerrainPreventsStringIds[] =
{
    [B_MSG_TERRAINPREVENTS_MISTY]    = STRINGID_MISTYTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_ELECTRIC] = STRINGID_ELECTRICTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_PSYCHIC]  = STRINGID_PSYCHICTERRAINPREVENTS
};

const u16 gHealingWishStringIds[] =
{
    STRINGID_HEALINGWISHCAMETRUE,
    STRINGID_LUNARDANCECAMETRUE
};

const u16 gDmgHazardsStringIds[] =
{
    [B_MSG_PKMNHURTBYSPIKES]   = STRINGID_PKMNHURTBYSPIKES,
    [B_MSG_STEALTHROCKDMG]     = STRINGID_STEALTHROCKDMG,
    [B_MSG_SHARPSTEELDMG]      = STRINGID_SHARPSTEELDMG,
    [B_MSG_POINTEDSTONESFLOAT] = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SPIKESSCATTERED]    = STRINGID_SPIKESSCATTERED,
    [B_MSG_SHARPSTEELFLOATS]   = STRINGID_SHARPSTEELFLOATS,
};

const u16 gSwitchInAbilityStringIds[] =
{
    [B_MSG_SWITCHIN_MOLDBREAKER] = STRINGID_MOLDBREAKERENTERS,
    [B_MSG_SWITCHIN_TERAVOLT] = STRINGID_TERAVOLTENTERS,
    [B_MSG_SWITCHIN_TURBOBLAZE] = STRINGID_TURBOBLAZEENTERS,
    [B_MSG_SWITCHIN_SLOWSTART] = STRINGID_SLOWSTARTENTERS,
    [B_MSG_SWITCHIN_UNNERVE] = STRINGID_UNNERVEENTERS,
    [B_MSG_SWITCHIN_ANTICIPATION] = STRINGID_ANTICIPATIONACTIVATES,
    [B_MSG_SWITCHIN_FOREWARN] = STRINGID_FOREWARNACTIVATES,
    [B_MSG_SWITCHIN_PRESSURE] = STRINGID_PRESSUREENTERS,
    [B_MSG_SWITCHIN_DARKAURA] = STRINGID_DARKAURAENTERS,
    [B_MSG_SWITCHIN_FAIRYAURA] = STRINGID_FAIRYAURAENTERS,
    [B_MSG_SWITCHIN_AURABREAK] = STRINGID_AURABREAKENTERS,
    [B_MSG_SWITCHIN_COMATOSE] = STRINGID_COMATOSEENTERS,
    [B_MSG_SWITCHIN_SCREENCLEANER] = STRINGID_SCREENCLEANERENTERS,
    [B_MSG_SWITCHIN_ASONE] = STRINGID_ASONEENTERS,
    [B_MSG_SWITCHIN_CURIOUS_MEDICINE] = STRINGID_CURIOUSMEDICINEENTERS,
    [B_MSG_SWITCHIN_PASTEL_VEIL] = STRINGID_PKMNHEALEDPOISON,
    [B_MSG_SWITCHIN_NEUTRALIZING_GAS] = STRINGID_NEUTRALIZINGGASENTERS,
};

const u16 gMissStringIds[] =
{
    [B_MSG_MISSED]      = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_PROTECTED]   = STRINGID_PKMNPROTECTEDITSELF,
    [B_MSG_AVOIDED_ATK] = STRINGID_PKMNAVOIDEDATTACK,
};

const u16 gNoEscapeStringIds[] =
{
    [B_MSG_CANT_ESCAPE]          = STRINGID_CANTESCAPE,
    [B_MSG_DONT_LEAVE_BIRCH]     = STRINGID_DONTLEAVEBIRCH,
    [B_MSG_PREVENTS_ESCAPE]      = STRINGID_PREVENTSESCAPE,
    [B_MSG_CANT_ESCAPE_2]        = STRINGID_CANTESCAPE2,
    [B_MSG_ATTACKER_CANT_ESCAPE] = STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] =
{
    [B_MSG_STARTED_RAIN]      = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_DOWNPOUR]  = STRINGID_DOWNPOURSTARTED, // Unused
    [B_MSG_WEATHER_FAILED]    = STRINGID_BUTITFAILED,
    [B_MSG_STARTED_SANDSTORM] = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_SUNLIGHT]  = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL]      = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW]      = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_FOG]       = STRINGID_FOGCREPTUP, // Unused, can use for custom moves that set fog
};

const u16 gAbilityWeatherChangeStringId[] =
{
    [B_MSG_STARTED_DRIZZLE]        = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_SAND_STREAM]    = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_DROUGHT]        = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL_WARNING]   = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW_WARNING]   = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_DESOLATE_LAND]  = STRINGID_EXTREMELYHARSHSUNLIGHT,
    [B_MSG_STARTED_PRIMORDIAL_SEA] = STRINGID_HEAVYRAIN,
    [B_MSG_STARTED_STRONG_WINDS]   = STRINGID_MYSTERIOUSAIRCURRENT,
};

const u16 gWeatherEndsStringIds[B_MSG_WEATHER_END_COUNT] =
{
    [B_MSG_WEATHER_END_RAIN]                       = STRINGID_RAINSTOPPED,
    [B_MSG_WEATHER_END_SUN]                        = STRINGID_SUNLIGHTFADED,
    [B_MSG_WEATHER_END_SANDSTORM]                  = STRINGID_SANDSTORMSUBSIDED,
    [B_MSG_WEATHER_END_HAIL]                       = STRINGID_HAILSTOPPED,
    [B_MSG_WEATHER_END_SNOW]                       = STRINGID_SNOWSTOPPED,
    [B_MSG_WEATHER_END_FOG]                        = STRINGID_FOGLIFTED,
    [B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT]   = STRINGID_EXTREMESUNLIGHTFADED,
    [B_MSG_WEATHER_END_HEAVY_RAIN]                 = STRINGID_HEAVYRAINLIFTED,
    [B_MSG_WEATHER_END_STRONG_WINDS]               = STRINGID_STRONGWINDSDISSIPATED,
};

const u16 gWeatherTurnStringIds[] =
{
    [B_MSG_WEATHER_TURN_RAIN]         = STRINGID_RAINCONTINUES,
    [B_MSG_WEATHER_TURN_DOWNPOUR]     = STRINGID_DOWNPOURCONTINUES,
    [B_MSG_WEATHER_TURN_SUN]          = STRINGID_SUNLIGHTSTRONG,
    [B_MSG_WEATHER_TURN_SANDSTORM]    = STRINGID_SANDSTORMRAGES,
    [B_MSG_WEATHER_TURN_HAIL]         = STRINGID_HAILCONTINUES,
    [B_MSG_WEATHER_TURN_SNOW]         = STRINGID_SNOWCONTINUES,
    [B_MSG_WEATHER_TURN_FOG]          = STRINGID_FOGISDEEP,
    [B_MSG_WEATHER_TURN_STRONG_WINDS] = STRINGID_MYSTERIOUSAIRCURRENTBLOWSON,
};

const u16 gSandStormHailDmgStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_PKMNBUFFETEDBYSANDSTORM,
    [B_MSG_HAIL]      = STRINGID_PKMNPELTEDBYHAIL
};

const u16 gProtectLikeUsedStringIds[] =
{
    [B_MSG_PROTECTED_ITSELF] = STRINGID_PKMNPROTECTEDITSELF2,
    [B_MSG_BRACED_ITSELF]    = STRINGID_PKMNBRACEDITSELF,
    [B_MSG_PROTECTED_TEAM]   = STRINGID_PROTECTEDTEAM,
};

const u16 gBrokeProtectionStringIds[] =
{
    [B_MSG_FEINT]           = STRINGID_FELLFORFEINT,
    [B_MSG_HYPERSPACE_FURY] = STRINGID_BROKETHROUGHPROTECTION,
};

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    [B_MSG_SIDE_STATUS_FAILED]     = STRINGID_BUTITFAILED,
    [B_MSG_SET_REFLECT_SINGLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_REFLECT_DOUBLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_LIGHTSCREEN_SINGLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_LIGHTSCREEN_DOUBLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_SAFEGUARD]          = STRINGID_PKMNCOVEREDBYVEIL,
    [B_MSG_SET_AURORA_VEIL]        = STRINGID_PKMNRAISEDDEFSPDEF, // HnS: keep user-requested mapping (upstream: STRINGID_PKMNAURORAVEIL, same Korean text)
};

const u16 gLeechSeedStringIds[] =
{
    [B_MSG_LEECH_SEED_SET]   = STRINGID_PKMNSEEDED,
    [B_MSG_LEECH_SEED_MISS]  = STRINGID_PKMNEVADEDATTACK,
    [B_MSG_LEECH_SEED_FAIL]  = STRINGID_ITDOESNTAFFECT,
    [B_MSG_LEECH_SEED_DRAIN] = STRINGID_PKMNSAPPEDBYLEECHSEED,
    [B_MSG_LEECH_SEED_OOZE]  = STRINGID_ITSUCKEDLIQUIDOOZE,
};

const u16 gRestUsedStringIds[] =
{
    [B_MSG_REST]          = STRINGID_PKMNSLEPTHEALTHY,
    [B_MSG_REST_STATUSED] = STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] =
{
    [B_MSG_UPROAR_CONTINUES] = STRINGID_PKMNMAKINGUPROAR,
    [B_MSG_UPROAR_ENDS]      = STRINGID_PKMNCALMEDDOWN
};

const u16 gWokeUpStringIds[] =
{
    [B_MSG_WOKE_UP]        = STRINGID_PKMNWOKEUP,
    [B_MSG_WOKE_UP_UPROAR] = STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gUproarAwakeStringIds[] =
{
    [B_MSG_CANT_SLEEP_UPROAR]  = STRINGID_PKMNCANTSLEEPINUPROAR2,
    [B_MSG_UPROAR_KEPT_AWAKE]  = STRINGID_UPROARKEPTPKMNAWAKE,
};

const u16 gStatUpStringIds[] =
{
    [B_MSG_ATTACKER_STAT_CHANGED] = STRINGID_ATTACKERSSTATROSE,
    [B_MSG_DEFENDER_STAT_CHANGED] = STRINGID_DEFENDERSSTATROSE,
    [B_MSG_STAT_WONT_CHANGE]      = STRINGID_STATSWONTINCREASE,
    [B_MSG_STAT_CHANGE_EMPTY]     = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]     = STRINGID_USINGITEMSTATOFPKMNROSE,
    [B_MSG_USED_DIRE_HIT]         = STRINGID_PKMNUSEDXTOGETPUMPED,
};

const u16 gStatDownStringIds[] =
{
    [B_MSG_ATTACKER_STAT_CHANGED] = STRINGID_ATTACKERSSTATFELL,
    [B_MSG_DEFENDER_STAT_CHANGED] = STRINGID_DEFENDERSSTATFELL,
    [B_MSG_STAT_WONT_CHANGE]      = STRINGID_STATSWONTDECREASE,
    [B_MSG_STAT_CHANGE_EMPTY]     = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]     = STRINGID_USINGITEMSTATOFPKMNFELL,
};

// Index copied from move's index in sTrappingMoves
const u16 gWrappedStringIds[NUM_TRAPPING_MOVES] =
{
    [B_MSG_WRAPPED_BIND]        = STRINGID_PKMNSQUEEZEDBYBIND,     // MOVE_BIND
    [B_MSG_WRAPPED_WRAP]        = STRINGID_PKMNWRAPPEDBY,          // MOVE_WRAP
    [B_MSG_WRAPPED_FIRE_SPIN]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_FIRE_SPIN
    [B_MSG_WRAPPED_CLAMP]       = STRINGID_PKMNCLAMPED,            // MOVE_CLAMP
    [B_MSG_WRAPPED_WHIRLPOOL]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_WHIRLPOOL
    [B_MSG_WRAPPED_SAND_TOMB]   = STRINGID_PKMNTRAPPEDBYSANDTOMB,  // MOVE_SAND_TOMB
    [B_MSG_WRAPPED_MAGMA_STORM] = STRINGID_TRAPPEDBYSWIRLINGMAGMA, // MOVE_MAGMA_STORM
    [B_MSG_WRAPPED_INFESTATION] = STRINGID_INFESTATION,            // MOVE_INFESTATION
    [B_MSG_WRAPPED_SNAP_TRAP]   = STRINGID_PKMNINSNAPTRAP,         // MOVE_SNAP_TRAP
    [B_MSG_WRAPPED_THUNDER_CAGE]= STRINGID_THUNDERCAGETRAPPED,     // MOVE_THUNDER_CAGE
};

const u16 gMistUsedStringIds[] =
{
    [B_MSG_SET_MIST]    = STRINGID_PKMNSHROUDEDINMIST,
    [B_MSG_MIST_FAILED] = STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] =
{
    [B_MSG_GETTING_PUMPED]      = STRINGID_PKMNGETTINGPUMPED,
    [B_MSG_FOCUS_ENERGY_FAILED] = STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] =
{
    [B_MSG_TRANSFORMED]      = STRINGID_PKMNTRANSFORMEDINTO,
    [B_MSG_TRANSFORM_FAILED] = STRINGID_BUTITFAILED
};

const u16 gSubstituteUsedStringIds[] =
{
    [B_MSG_SET_SUBSTITUTE]    = STRINGID_PKMNMADESUBSTITUTE,
    [B_MSG_SUBSTITUTE_FAILED] = STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPOISONED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASPOISONED
};

const u16 gGotParalyzedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPARALYZED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASPARALYZED
};

const u16 gFellAsleepStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLASLEEP,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNFELLASLEEP,
};

const u16 gGotBurnedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASBURNED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASBURNED
};

const u16 gGotFrostbiteStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNGOTFROSTBITE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNGOTFROSTBITE,
};

const u16 gFrostbiteHealedStringIds[] =
{
    [B_MSG_FROSTBITE_HEALED]         = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_FROSTBITE_HEALED_BY_MOVE] = STRINGID_PKMNFROSTBITEHEALEDBY
};

const u16 gGotFrozenStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASFROZEN,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] =
{
    [B_MSG_DEFROSTED]         = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_DEFROSTED_BY_MOVE] = STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gPurifyStatusCureStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PURIFYTARGETPARALYSISCURED,
    [B_MSG_CURED_POISON]     = STRINGID_PURIFYTARGETPOISONCURED,
    [B_MSG_CURED_BURN]       = STRINGID_PURIFYTARGETBURNCURED,
    [B_MSG_CURED_FREEZE]     = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_CURED_FROSTBITE]  = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_CURED_SLEEP]      = STRINGID_PURIFYTARGETSLEEPCURED,
    [B_MSG_CURED_PROBLEM]    = STRINGID_PURIFYTARGETSTATUSNORMAL,
    [B_MSG_NORMALIZED_STATUS] = STRINGID_PURIFYTARGETSTATUSNORMAL,
    [B_MSG_CURED_CONFUSION]  = STRINGID_PKMNHEALEDCONFUSION,
};

const u16 gKOFailedStringIds[] =
{
    [B_MSG_KO_MISS]       = STRINGID_PKMNEVADEDATTACK,
    [B_MSG_KO_UNAFFECTED] = STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLINLOVE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNSXINFATUATEDY
};

const u16 gAbsorbDrainStringIds[] =
{
    [B_MSG_ABSORB]      = STRINGID_PKMNENERGYDRAINED,
    [B_MSG_ABSORB_OOZE] = STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] =
{
    [B_MSG_WEAKEN_ELECTRIC] = STRINGID_ELECTRICITYWEAKENED,
    [B_MSG_WEAKEN_FIRE]     = STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] =
{
    [B_MSG_BELL]                     = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_ATTACKER] = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_PARTNER]  = STRINGID_BELLCHIMED,
    [B_MSG_BELL_BOTH_SOUNDPROOF]     = STRINGID_BELLCHIMED,
    [B_MSG_SOOTHING_AROMA]           = STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] =
{
    [B_MSG_FUTURE_SIGHT] = STRINGID_PKMNFORESAWATTACK,
    [B_MSG_DOOM_DESIRE]  = STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] =
{
    [BALL_NO_SHAKES]     = STRINGID_PKMNBROKEFREE,
    [BALL_1_SHAKE]       = STRINGID_ITAPPEAREDCAUGHT,
    [BALL_2_SHAKES]      = STRINGID_AARGHALMOSTHADIT,
    [BALL_3_SHAKES_FAIL] = STRINGID_SHOOTSOCLOSE
};

// Overworld weathers that don't have an associated battle weather default to "It is raining."
const u16 gWeatherStartsStringIds[] =
{
    [WEATHER_NONE]               = STRINGID_ITISRAINING,
    [WEATHER_SUNNY_CLOUDS]       = STRINGID_ITISRAINING,
    [WEATHER_SUNNY]              = STRINGID_ITISRAINING,
    [WEATHER_RAIN]               = STRINGID_ITISRAINING,
    [WEATHER_SNOW]               = (B_OVERWORLD_SNOW >= GEN_9 ? STRINGID_ITISSNOWING : STRINGID_ITISHAILING),
    [WEATHER_RAIN_THUNDERSTORM]  = STRINGID_ITISRAINING,
    [WEATHER_FOG_HORIZONTAL]     = STRINGID_FOGISDEEP,
    [WEATHER_VOLCANIC_ASH]       = STRINGID_ITISRAINING,
    [WEATHER_SANDSTORM]          = STRINGID_SANDSTORMISRAGING,
    [WEATHER_FOG_DIAGONAL]       = STRINGID_FOGISDEEP,
    [WEATHER_UNDERWATER]         = STRINGID_ITISRAINING,
    [WEATHER_SHADE]              = STRINGID_ITISRAINING,
    [WEATHER_DROUGHT]            = STRINGID_SUNLIGHTISHARSH,
    [WEATHER_DOWNPOUR]           = STRINGID_ITISRAINING,
    [WEATHER_UNDERWATER_BUBBLES] = STRINGID_ITISRAINING,
    [WEATHER_ABNORMAL]           = STRINGID_ITISRAINING
};

const u16 gTerrainStartsStringIds[] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_MISTSWIRLSAROUND,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_ELECTRICCURRENTISRUNNING,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_SEEMSWEIRD,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_ISCOVEREDWITHGRASS,
};

const u16 gPrimalWeatherBlocksStringIds[] =
{
    [B_MSG_PRIMAL_WEATHER_FIZZLED_BY_RAIN]      = STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN,
    [B_MSG_PRIMAL_WEATHER_EVAPORATED_IN_SUN]    = STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT,
};

const u16 gInobedientStringIds[] =
{
    [B_MSG_LOAFING]            = STRINGID_PKMNLOAFING,
    [B_MSG_WONT_OBEY]          = STRINGID_PKMNWONTOBEY,
    [B_MSG_TURNED_AWAY]        = STRINGID_PKMNTURNEDAWAY,
    [B_MSG_PRETEND_NOT_NOTICE] = STRINGID_PKMNPRETENDNOTNOTICE,
    [B_MSG_INCAPABLE_OF_POWER] = STRINGID_PKMNINCAPABLEOFPOWER
};

const u16 gSafariReactionStringIds[NUM_SAFARI_REACTIONS] =
{
    [B_MSG_MON_WATCHING] = STRINGID_PKMNWATCHINGCAREFULLY,
    [B_MSG_MON_ANGRY]    = STRINGID_PKMNANGRY,
    [B_MSG_MON_EATING]   = STRINGID_PKMNEATING
};

const u16 gSafariGetNearStringIds[] =
{
    [B_MSG_CREPT_CLOSER]    = STRINGID_CREPTCLOSER,
    [B_MSG_CANT_GET_CLOSER] = STRINGID_CANTGETCLOSER
};

const u16 gSafariPokeblockResultStringIds[] =
{
    [B_MSG_MON_CURIOUS]    = STRINGID_PKMNCURIOUSABOUTX,
    [B_MSG_MON_ENTHRALLED] = STRINGID_PKMNENTHRALLEDBYX,
    [B_MSG_MON_IGNORED]    = STRINGID_PKMNIGNOREDX
};

const u16 CureStatusBerryEffectStringID[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PKMNSITEMCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PKMNSITEMCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PKMNSITEMHEALEDBURN,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNSITEMDEFROSTEDIT,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNSITEMHEALEDFROSTBITE,
    [B_MSG_CURED_SLEEP] = STRINGID_PKMNSITEMWOKEIT,
    [B_MSG_CURED_CONFUSION] = STRINGID_PKMNSITEMSNAPPEDOUT,
    // HnS: keep PROBLEM/NORMALIZED (removed upstream) so these indexes stay inside the table.
    [B_MSG_CURED_PROBLEM]     = STRINGID_PKMNSITEMCUREDPROBLEM,
    [B_MSG_NORMALIZED_STATUS] = STRINGID_PKMNSITEMNORMALIZEDSTATUS,
};

const u16 gItemSwapStringIds[] =
{
    [B_MSG_ITEM_SWAP_TAKEN] = STRINGID_PKMNOBTAINEDX,
    [B_MSG_ITEM_SWAP_GIVEN] = STRINGID_PKMNOBTAINEDX2,
    [B_MSG_ITEM_SWAP_BOTH]  = STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] =
{
    [B_MSG_FLASH_FIRE_BOOST]    = STRINGID_PKMNRAISEDFIREPOWERWITH,
    [B_MSG_FLASH_FIRE_NO_BOOST] = STRINGID_PKMNSXMADEYINEFFECTIVE
};

const u16 gCaughtMonStringIds[] =
{
    [B_MSG_SENT_SOMEONES_PC]   = STRINGID_PKMNTRANSFERREDSOMEONESPC,
    [B_MSG_SENT_LANETTES_PC]   = STRINGID_PKMNTRANSFERREDLANETTESPC,
    [B_MSG_SOMEONES_BOX_FULL]  = STRINGID_PKMNBOXSOMEONESPCFULL,
    [B_MSG_LANETTES_BOX_FULL]  = STRINGID_PKMNBOXLANETTESPCFULL,
    [B_MSG_SWAPPED_INTO_PARTY] = STRINGID_PKMNSENTTOPCAFTERCATCH,
};

const u16 gRoomsStringIds[] =
{
    STRINGID_PKMNTWISTEDDIMENSIONS, STRINGID_TRICKROOMENDS,
    STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON, STRINGID_WONDERROOMENDS,
    STRINGID_HELDITEMSLOSEEFFECTS, STRINGID_MAGICROOMENDS,
    STRINGID_EMPTYSTRING3
};

const u16 gStatusConditionsStringIds[] =
{
    STRINGID_PKMNWASPOISONED, STRINGID_PKMNBADLYPOISONED, STRINGID_PKMNWASBURNED, STRINGID_PKMNWASPARALYZED, STRINGID_PKMNFELLASLEEP, STRINGID_PKMNGOTFROSTBITE
};

const u16 gStatusProtectsStringIds[] =
{
    [B_MSG_STATUS_PROTECTS_POISON]    = STRINGID_PKMNCANNOTBEPOISONED,
    [B_MSG_STATUS_PROTECTS_BURN]      = STRINGID_PKMNCANNOTBURN,
    [B_MSG_STATUS_PROTECTS_PARALYSIS] = STRINGID_PKMNCANNOTPARALYZE,
    [B_MSG_STATUS_PROTECTS_FREEZE]    = STRINGID_PKMNCANNOTFREEZE,
    [B_MSG_STATUS_PROTECTS_SLEEP]     = STRINGID_PKMNCANNOTSLEEP,
};

const u16 gDamageNonTypesStartStringIds[] =
{
    [B_MSG_TRAPPED_WITH_VINES]  = STRINGID_TEAMTRAPPEDWITHVINES,
    [B_MSG_CAUGHT_IN_VORTEX]    = STRINGID_TEAMCAUGHTINVORTEX,
    [B_MSG_SURROUNDED_BY_FIRE]  = STRINGID_TEAMSURROUNDEDBYFIRE,
    [B_MSG_SURROUNDED_BY_ROCKS] = STRINGID_TEAMSURROUNDEDBYROCKS,
};

const u16 gDamageNonTypesDmgStringIds[] =
{
    [B_MSG_HURT_BY_VINES]        = STRINGID_PKMNHURTBYVINES,
    [B_MSG_HURT_BY_VORTEX]       = STRINGID_PKMNHURTBYVORTEX,
    [B_MSG_BURNING_UP]           = STRINGID_PKMNBURNINGUP,
    [B_MSG_HURT_BY_ROCKS_THROWN] = STRINGID_PKMNHURTBYROCKSTHROWN,
};

const u16 gRemoveHazardsStringIds[] =
{
    [HAZARDS_SPIKES] = STRINGID_SPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STICKY_WEB] = STRINGID_STICKYWEBDISAPPEAREDFROMTEAM,
    [HAZARDS_TOXIC_SPIKES] = STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STEALTH_ROCK] = STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM,
    [HAZARDS_STEELSURGE] = STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM,
};

const u16 gZenModeStringIds[] =
{
    [B_MSG_ZEN_MODE_TRIGGERED] = STRINGID_ZENMODETRIGGERED,
    [B_MSG_ZEN_MODE_ENDED] = STRINGID_ZENMODEENDED
};

const u16 gCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_SCRCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_SCRCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_SCRCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_SCRCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
    // HnS: GetCuredStatusMessage() can still return PROBLEM; keep a {B_SCR} fallback inside the table.
    [B_MSG_CURED_PROBLEM] = STRINGID_PURIFYTARGETSTATUSNORMAL,
    [B_MSG_NORMALIZED_STATUS] = STRINGID_PURIFYTARGETSTATUSNORMAL,
};

const u16 gPartyCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PARTYCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PARTYCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PARTYCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_PARTYCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PARTYCUREDFREEZE,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PARTYCUREDFROSTBITE,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gHurtByStringIds[] =
{
    [B_MSG_HURT] = STRINGID_AFTERMATHDMG,
    [B_MSG_HURT_BY_ITEM] = STRINGID_PKMNHURTSWITH,
};

const u8 gText_PkmnIsEvolving[] = _("...오잉!?\n{STR_VAR_1}의 모습이...!");
const u8 gText_CongratsPkmnEvolved[] = _("축하합니다! {STR_VAR_1}{B_TXT_EUNNEUN}\n{STR_VAR_2}{B_TXT_EU}로 진화했습니다!{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("얼라리...?\n{STR_VAR_1}의 변화가 멈췄다!\p");
const u8 gText_EllipsisQuestionMark[] = _("...?\p");
const u8 gText_WhatWillPkmnDo[] = _("{B_BUFF1}{B_TXT_EUNNEUN}\n무엇을 할까?");
const u8 gText_WhatWillPkmnDo2[] = _("{B_PLAYER_NAME}{B_TXT_EUNNEUN}\n무엇을 할까?");
const u8 gText_WhatWillWallyDo[] = _("민진은\n무엇을 할까?");
const u8 gText_LinkStandby[] = _("{PAUSE 16}통신 대기 중...");
const u8 gText_BattleMenu[] = _("{SIZE 7}싸운다{CLEAR_TO 56}가방\n포켓몬{CLEAR_TO 56}도망간다");
const u8 gText_SafariZoneMenu[] = _("{SIZE 7}볼{CLEAR_TO 56}포켓몬스넥\n다가간다{CLEAR_TO 56}도망간다");
const u8 gText_SafariZoneMenuFrlg[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}BALL{CLEAR_TO 56}BAIT\nROCK{CLEAR_TO 56}RUN");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("타입/");
const u8 gText_MoveInterfacePpType[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW DYNAMIC_COLOR4 DYNAMIC_COLOR5 DYNAMIC_COLOR6}PP\n타입/");
const u8 gText_MoveInterfaceDynamicColors[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW DYNAMIC_COLOR4 DYNAMIC_COLOR5 DYNAMIC_COLOR6}");
const u8 gText_WhichMoveToForget4[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW DYNAMIC_COLOR4 DYNAMIC_COLOR5 DYNAMIC_COLOR6}어느 기술을\n잊게 하겠습니까?");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW DYNAMIC_COLOR4 DYNAMIC_COLOR5 DYNAMIC_COLOR6}예\n아니오");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW DYNAMIC_COLOR4 DYNAMIC_COLOR5 DYNAMIC_COLOR6}바꿀 기술을\n선택해 주세요");
const u8 gText_BattleSwitchWhich2[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW DYNAMIC_COLOR4 DYNAMIC_COLOR5 DYNAMIC_COLOR6}");
const u8 gText_BattleSwitchWhich3[] = _("{UP_ARROW}");
const u8 gText_BattleSwitchWhich4[] = _("{ESCAPE 4}");
const u8 gText_BattleSwitchWhich5[] = _("-");
const u8 gText_SafariBalls[] = _("{HIGHLIGHT DARK_GRAY}사파리볼");
const u8 gText_SafariBallLeft[] = _("{HIGHLIGHT DARK_GRAY}{ENG}개 남음$" "{HIGHLIGHT DARK_GRAY}");
const u8 gText_Sleep[] = _("잠듦");
const u8 gText_Poison[] = _("독");
const u8 gText_Burn[] = _("화상");
const u8 gText_Paralysis[] = _("마비");
const u8 gText_Ice[] = _("얼음");
const u8 gText_Confusion[] = _("혼란");
const u8 gText_Love[] = _("헤롱헤롱");
const u8 gText_SpaceAndSpace[] = _("{B_TXT_WAGWA} ");
const u8 gText_CommaSpace[] = _(", ");
const u8 gText_Space2[] = _(" ");
const u8 gText_LineBreak[] = _("\l");
const u8 gText_NewLine[] = _("\n");
const u8 gText_Are[] = _("{B_TXT_EUNNEUN} ");
const u8 gText_Are2[] = _("{B_TXT_EUNNEUN}\l");
const u8 gText_BadEgg[] = _("불량알");
const u8 gText_BattleWallyName[] = _("민진");
const u8 gText_Win[] = _("{HIGHLIGHT TRANSPARENT}승");
const u8 gText_Loss[] = _("{HIGHLIGHT TRANSPARENT}패");
const u8 gText_Draw[] = _("{HIGHLIGHT TRANSPARENT}무승부");
static const u8 sText_SpaceIs[] = _("{B_TXT_EUNNEUN}");
static const u8 sText_ApostropheS[] = _("의");
const u8 gText_BattleTourney[] = _("배틀토너먼트");

const u8 *const gRoundsStringTable[DOME_ROUNDS_COUNT] =
{
    [DOME_ROUND1]    = COMPOUND_STRING("1차"),
    [DOME_ROUND2]    = COMPOUND_STRING("2차"),
    [DOME_SEMIFINAL] = COMPOUND_STRING("준결승"),
    [DOME_FINAL]     = COMPOUND_STRING("결승"),
};

const u8 gText_TheGreatNewHope[] = _("기대받는 대형 신인!\p");
const u8 gText_WillChampionshipDreamComeTrue[] = _("꿈꾸던 우승은 이루어질 것인가!?\p");
const u8 gText_AFormerChampion[] = _("전 챔피언!\p");
const u8 gText_ThePreviousChampion[] = _("전회 챔피언!\p");
const u8 gText_TheUnbeatenChampion[] = _("무적의 챔피언!\p");
const u8 gText_PlayerMon1Name[] = _("{B_PLAYER_MON1_NAME}");
const u8 gText_Vs[] = _("vs");
const u8 gText_OpponentMon1Name[] = _("{B_OPPONENT_MON1_NAME}");
const u8 gText_Mind[] = _("마음");
const u8 gText_Skill[] = _("기술");
const u8 gText_Body[] = _("몸");
const u8 gText_Judgment[] = _("{B_BUFF1}{CLEAR 13}판정{CLEAR 13}{B_BUFF2}");
static const u8 sText_TwoTrainersSentPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON1_NAME}{B_TXT_EULREUL} 내보냈다!\p{B_TRAINER2_CLASS} {B_TRAINER2_NAME}{B_TXT_EUNNEUN}\n{B_OPPONENT_MON2_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_Trainer2SentOutPkmn[] = _("{B_TRAINER2_CLASS} {B_TRAINER2_NAME}{B_TXT_EUNNEUN}\n{B_BUFF1}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_TwoTrainersWantToBattle[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_WAGWA}\n{B_TRAINER2_CLASS} {B_TRAINER2_NAME}{B_TXT_IGA}\l승부를 걸어왔다!\p");
static const u8 sText_InGamePartnerSentOutZGoN[] = _("{B_PARTNER_CLASS} {B_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_PLAYER_MON2_NAME}{B_TXT_EULREUL} 내보냈다!\l가랏! {B_PLAYER_MON1_NAME}!");
static const u8 sText_InGamePartnerSentOutNGoZ[] = _("{B_PARTNER_CLASS} {B_PARTNER_NAME}{B_TXT_EUNNEUN}\n{B_PLAYER_MON1_NAME}{B_TXT_EULREUL} 내보냈다!\l가랏! {B_PLAYER_MON2_NAME}!");
static const u8 sText_InGamePartnerSentOutPkmn1[] = _("{B_PARTNER_NAME_WITH_CLASS}{B_TXT_EUNNEUN}\n{B_PLAYER_MON1_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_InGamePartnerSentOutPkmn2[] = _("{B_PARTNER_NAME_WITH_CLASS}{B_TXT_EUNNEUN}\n{B_PLAYER_MON2_NAME}{B_TXT_EULREUL} 내보냈다!");
static const u8 sText_InGamePartnerWithdrewPkmn1[] = _("{B_PARTNER_NAME_WITH_CLASS}{B_TXT_EUNNEUN}\n{B_PLAYER_MON1_NAME}{B_TXT_EULREUL} 넣어버렸다!");
static const u8 sText_InGamePartnerWithdrewPkmn2[] = _("{B_PARTNER_NAME_WITH_CLASS}{B_TXT_EUNNEUN}\n{B_PLAYER_MON2_NAME}{B_TXT_EULREUL} 넣어버렸다!");

const u16 gBattlePalaceFlavorTextTable[] =
{
    [B_MSG_GLINT_IN_EYE]   = STRINGID_GLINTAPPEARSINEYE,
    [B_MSG_GETTING_IN_POS] = STRINGID_PKMNGETTINGINTOPOSITION,
    [B_MSG_GROWL_DEEPLY]   = STRINGID_PKMNBEGANGROWLINGDEEPLY,
    [B_MSG_EAGER_FOR_MORE] = STRINGID_PKMNEAGERFORMORE,
};

const u8 *const gRefereeStringsTable[] =
{
    [B_MSG_REF_NOTHING_IS_DECIDED] = COMPOUND_STRING("심판: 앞으로 3턴 뒤에 결착이\n나지 않으면 판정을 하게 됩니다!"),
    [B_MSG_REF_THATS_IT]           = COMPOUND_STRING("심판: 거기까지---!\n이 대결은 판정을 하겠습니다!"),
    [B_MSG_REF_JUDGE_MIND]         = COMPOUND_STRING("심판: 첫 번째 판정! “마음”!\n공격할 마음을 보여주었는가!\p"),
    [B_MSG_REF_JUDGE_SKILL]        = COMPOUND_STRING("심판: 두 번째 판정! “기술”!\n제대로 기술을 썼는가!\p"),
    [B_MSG_REF_JUDGE_BODY]         = COMPOUND_STRING("심판: 세 번째 판정! “몸”!\n넘치는 체력을 가졌는가!\p"),
    [B_MSG_REF_PLAYER_WON]         = COMPOUND_STRING("심판: 판정 {B_BUFF1} 대 {B_BUFF2}\n승자! {B_PLAYER_NAME}의 {B_PLAYER_MON1_NAME}!\p"),
    [B_MSG_REF_OPPONENT_WON]       = COMPOUND_STRING("심판: 판정 {B_BUFF1} 대 {B_BUFF2}\n승자! {B_TRAINER1_NAME}의 {B_OPPONENT_MON1_NAME}!\p"),
    [B_MSG_REF_DRAW]               = COMPOUND_STRING("심판: 판정 3 대 3!\n무승부---!\p"),
    [B_MSG_REF_COMMENCE_BATTLE]    = COMPOUND_STRING("심판: {B_PLAYER_MON1_NAME} VS {B_OPPONENT_MON1_NAME}\n승부! 시작---!!"),
};

static const u8 sText_Trainer1Fled[] = _("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_EUNNEUN}\n도망쳤다!");
static const u8 sText_PlayerLostAgainstTrainer1[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_WAGWA}의\n승부에서 졌다!");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}{B_TXT_WAGWA}의\n승부에서 비겼다!");
const u8 gText_RecordBattleToPass[] = _("지금의 배틀을 프런티어패스에\n기록하겠습니까?");
const u8 gText_BattleRecordedOnPass[] = _("{B_PLAYER_NAME}의 배틀이\n프런티어패스에 기록되었다!");
static const u8 sText_LinkTrainerWantsToBattlePause[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_IGA}\n승부를 걸어왔다!{PAUSE 49}");
static const u8 sText_TwoLinkTrainersWantToBattlePause[] = _("{B_LINK_OPPONENT1_NAME}{B_TXT_WAGWA} {B_LINK_OPPONENT2_NAME}{B_TXT_IGA}\n승부를 걸어왔다!{PAUSE 49}");
static const u8 sText_Your1[] = _("우리 편");
static const u8 sText_Opposing1[] = _("상대");
static const u8 sText_Your2[] = _("우리 편");
static const u8 sText_Opposing2[] = _("상대");
static const u8 sText_EmptyStatus[] = _("$$$$$$$");

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_KantoTutorial[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
    [B_WIN_OAK_OLD_MAN] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 1,
        .speed = 1,
        .fgColor = 2,
        .bgColor = 1,
        .shadowColor = 3,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_Arena[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [ARENA_WIN_PLAYER_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_VS] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_OPPONENT_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_MIND] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_SKILL] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_BODY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TITLE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TEXT] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 2,
        .color.background = 1,
        .color.accent = 1,
        .color.shadow = 3,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
};

static const struct BattleWindowText *const sBattleTextOnWindowsInfo[] =
{
    [B_WIN_TYPE_NORMAL] = sTextOnWindowsInfo_Normal,
    [B_WIN_TYPE_ARENA]  = sTextOnWindowsInfo_Arena,
    [B_WIN_TYPE_KANTO_TUTORIAL] = sTextOnWindowsInfo_KantoTutorial,
};

static const u8 sRecordedBattleTextSpeeds[] = {8, 4, 1, 0};

void BufferStringBattle(enum StringID stringID, enum BattlerId battler)
{
    s32 i;
    const u8 *stringPtr = NULL;

    gBattleMsgDataPtr = (struct BattleMsgData *)(&gBattleResources->bufferA[battler][4]);
    gLastUsedItem = gBattleMsgDataPtr->lastItem;
    gLastUsedAbility = gBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = gBattleMsgDataPtr->scrActive;
    gBattleStruct->scriptPartyIdx = gBattleMsgDataPtr->bakScriptPartyIdx;
    gBattleStruct->hpScale = gBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = gBattleMsgDataPtr->itemEffectBattler;
    gBattleStruct->stringMoveType = gBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = gBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = gBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = gBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = gBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringID)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                {
                    stringPtr = sText_TwoTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                    {
                        if (TESTING && gBattleTypeFlags & BATTLE_TYPE_MULTI)
                        {
                            if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
                                stringPtr = sText_Trainer1WantsToBattle;
                            else
                                stringPtr = sText_TwoTrainersWantToBattle;
                        }
                        else if (TESTING && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                        {
                            stringPtr = sText_TwoTrainersWantToBattle;
                        }
                        else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
                        {
                            stringPtr = sText_LinkTrainerWantsToBattlePause;
                        }
                        else
                        {
                            stringPtr = sText_TwoLinkTrainersWantToBattlePause;
                        }
                    }
                    else
                    {
                        stringPtr = sText_TwoLinkTrainersWantToBattle;
                    }
                }
                else
                {
                    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                        stringPtr = sText_LinkTrainerWantsToBattlePause;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
                if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1WantsToBattle;
                else if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
                    stringPtr = sText_TwoTrainersWantToBattle;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = sText_TwoTrainersWantToBattle;
                else
                    stringPtr = sText_Trainer1WantsToBattle;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST && IsGhostBattleWithoutScope())
                stringPtr = sText_GhostAppearedCantId;
            else if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
                stringPtr = sText_TheGhostAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_LegendaryPkmnAppeared;
            else if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))))
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_CATCH_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else if (!gSaveblock3.challengeSettings.lrToRun && gSaveblock3.challengeSettings.runType == 1)
                stringPtr = sText_WildPkmnAppearedLR;
            else if (!gSaveblock3.challengeSettings.lrToRun && gSaveblock3.challengeSettings.runType == 3)
                stringPtr = sText_WildPkmnAppearedB;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (BattlerIsPlayer(battler) || BattlerIsPlayer(BATTLE_PARTNER(battler))
         || BattlerIsWally(battler) || BattlerIsWally(BATTLE_PARTNER(battler)))
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(BATTLE_PARTNER(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                {
                    if (BattlerIsPlayer(battler) && (battler & BIT_FLANK) == B_FLANK_LEFT) // Player is battler 0
                        stringPtr = sText_InGamePartnerSentOutZGoN;
                    else // Player is battler 2
                        stringPtr = sText_InGamePartnerSentOutNGoZ;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                {
                    stringPtr = sText_GoTwoPkmn;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    if (BattlerIsPlayer(battler))
                        stringPtr = sText_LinkPartnerSentOutPkmn2GoPkmn;
                    else
                        stringPtr = sText_LinkPartnerSentOutPkmn1GoPkmn;
                }
                else
                {
                    stringPtr = sText_GoTwoPkmn;
                }
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(BATTLE_PARTNER(battler))))
            {
                if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = sText_TwoTrainersSentPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                    stringPtr = sText_TwoTrainersSentPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_TwoLinkTrainersSentOutPkmn;
                else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler))) // Link Opponent 1 and test opponent
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
            }
            else
            {
                if (!(BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler))))
                    stringPtr = sText_Trainer1SentOutPkmn;
                else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn;
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if ((GetBattlerPosition(battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(battler) || BattlerIsWally(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler))
            {
                if (BattlerIsLink(battler)) // Link Partner
                {
                    stringPtr = sText_LinkPartnerWithdrewPkmn1;
                }
                else // In-game Partner
                {
                    stringPtr = sText_InGamePartnerWithdrewPkmn1;
                }
            }
            else if (BattlerIsLink(battler) || TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
            || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent 1 and test opponent
            {
                stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler))
            {
                if (BattlerIsLink(battler)) // Link Partner
                {
                    stringPtr = sText_LinkPartnerWithdrewPkmn2;
                }
                else // In-game Partner
                {
                    stringPtr = sText_InGamePartnerWithdrewPkmn2;
                }
            }
            else if (BattlerIsLink(battler) || TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS || gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if ((GetBattlerPosition(gBattleScripting.battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler))
            {
                if (BattlerIsLink(gBattleScripting.battler)) // Link Partner
                {
                    stringPtr = sText_LinkPartnerSentOutPkmn1;
                }
                else // In-game Partner
                {
                    stringPtr = sText_InGamePartnerSentOutPkmn1;
                }
            }
            else if (BattlerIsLink(gBattleScripting.battler) || TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
            || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent 1 and test opponent
            {
                // Must use the {B_BUFF1} variant, like the battler 2/3 case below does.
                // {B_OPPONENT_MON1_NAME} is resolved from this console's own
                // gBattlerPartyIndexes at print time, but over a link the authoritative
                // index does not arrive until switchinanim, which runs after this
                // printstring -- so it named the opponent's previous mon. gBattleTextBuff1
                // was just filled with the right nickname by Cmd_switchindataupdate and
                // travels with the message.
                stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler))
            {
                if (BattlerIsLink(gBattleScripting.battler)) // Link Partner
                {
                    stringPtr = sText_LinkPartnerSentOutPkmn2;
                }
                else // In-game Partner
                {
                    stringPtr = sText_InGamePartnerSentOutPkmn2;
                }
            }
            else if (BattlerIsLink(gBattleScripting.battler) || TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS || gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainer2SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2SentOutPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn2;
            }
        }
        /*if (IsOnPlayerSide(gBattleScripting.battler))
        {
            if ((gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) && (BattlerIsPartner(gBattleScripting.battler)))
                stringPtr = sText_InGamePartnerSentOutPkmn2;
            else if (*(&gBattleStruct->hpScale) == 0 || IsDoubleBattle())
                stringPtr = sText_GoPkmn2;
            else if (*(&gBattleStruct->hpScale) == 1)
                stringPtr = sText_DoItPkmn;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_GoForItPkmn;
            else
                stringPtr = sText_YourFoesWeakGetEmPkmn;
        }
        else
        {
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                {
                    if (gBattleScripting.battler == 1)
                        stringPtr = sText_Trainer1SentOutPkmn2;
                    else
                        stringPtr = sText_Trainer2SentOutPkmn;
                }
                else
                {
                    if (TESTING && gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    {
                        if (gBattleScripting.battler == 1)
                        {
                            stringPtr = sText_Trainer1SentOutPkmn;
                        }
                        else
                        {
                            if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                                stringPtr = sText_Trainer2SentOutPkmn;
                            else
                                stringPtr = sText_Trainer1SentOutPkmn2;
                        }
                    }
                    else if (TESTING && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    {
                        if (gBattleScripting.battler == 1)
                            stringPtr = sText_Trainer1SentOutPkmn;
                        else
                            stringPtr = sText_Trainer2SentOutPkmn;
                    }
                    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    {
                        stringPtr = sText_LinkTrainerMultiSentOutPkmn;
                    }
                    else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                    {
                        stringPtr = sText_Trainer1SentOutPkmn2;
                    }
                    else
                    {
                        stringPtr = sText_LinkTrainerSentOutPkmn2;
                    }
                }
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                {
                    if (gBattleScripting.battler == 1)
                        stringPtr = sText_Trainer1SentOutPkmn2;
                    else
                        stringPtr = sText_Trainer2SentOutPkmn;
                }
                else
                {
                    stringPtr = sText_Trainer1SentOutPkmn2;
                }
            }
        }*/
        break;
    case STRINGID_USEDMOVE: // Pokémon used a move msg
        if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
         && !IsZMove(gBattleMsgDataPtr->currentMove)
         && !IsMaxMove(gBattleMsgDataPtr->currentMove))
            StringCopy(gBattleTextBuff3, gTypesInfo[*(&gBattleStruct->stringMoveType)].generic);
        else
            StringCopy(gBattleTextBuff3, GetMoveName(gBattleMsgDataPtr->currentMove));
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(BATTLE_PARTNER(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(BATTLE_PARTNER(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                        stringPtr = sText_TwoInGameTrainersDefeated;
                    else
                        stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    case STRINGID_TRAINERSLIDE:
        stringPtr = gBattleStruct->trainerSlideMsg;
        break;
    default: // load a string from the table
        if (stringID >= STRINGID_COUNT)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            stringPtr = gBattleStringsTable[stringID];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8 *src)
{
#ifndef NDEBUG
    u32 j, strWidth;
    u32 dstID = BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
    for (j = 1;; j++)
    {
        strWidth = GetStringLineWidth(0, gDisplayedStringBattle, 0, j, sizeof(gDisplayedStringBattle));
        if (strWidth == 0)
            break;
    }
    return dstID;
#else
    return BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
#endif
}

static const u8 *TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[8];
    u32 chars1, chars2;
    u8 *statusPtr;

    memcpy(status, sText_EmptyStatus, min(ARRAY_COUNT(status), ARRAY_COUNT(sText_EmptyStatus)));

    statusPtr = status;
    for (i = 0; i < ARRAY_COUNT(status); i++)
    {
        if (*src == EOS) break; // one line required to match -g
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32 *)(&status[0]);
    chars2 = *(u32 *)(&status[4]);

    for (i = 0; i < ARRAY_COUNT(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32 *)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32 *)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

static void GetBattlerNick(enum BattlerId battler, u8 *dst)
{
    struct Pokemon *illusionMon = GetIllusionMonPtr(battler);
    struct Pokemon *mon = GetBattlerMon(battler);

    if (illusionMon != NULL)
        mon = illusionMon;
    GetMonData(mon, MON_DATA_NICKNAME, dst);
    StringGet_Nickname(dst);
}

#define HANDLE_NICKNAME_STRING_CASE(battler)                            \
    if (!IsOnPlayerSide(battler))                                       \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
    }                                                                   \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;

#define HANDLE_NICKNAME_STRING_LOWERCASE(battler)                       \
    if (!IsOnPlayerSide(battler))                       \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefixLower;                           \
        else                                                            \
            toCpy = sText_WildPkmnPrefixLower;                          \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
    }                                                                   \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;

static const u8 *BattleStringGetOpponentNameByTrainerId(u16 trainerId, u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
    {
        u32 i;
        for (i = 0; i < ARRAY_COUNT(gBattleResources->secretBase->trainerName); i++)
            text[i] = gBattleResources->secretBase->trainerName[i];
        text[i] = EOS;
        ConvertInternationalString(text, gBattleResources->secretBase->language);
        toCpy = text;
    }
    else if (trainerId == TRAINER_UNION_ROOM)
    {
        toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_LINK_OPPONENT)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler)].name;
        else
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler) & BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
    {
        CopyFrontierBrainTrainerName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
    {
        GetFrontierTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
    {
        GetTrainerTowerOpponentName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
    {
        GetTrainerHillTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
    {
        GetEreaderTrainerName(text);
        toCpy = text;
    }
    else
    {
        enum TrainerClassID trainerClass = GetTrainerClassFromId(TRAINER_BATTLE_PARAM.opponentA);

        if (trainerClass == TRAINER_CLASS_RIVAL_EARLY_FRLG || trainerClass == TRAINER_CLASS_RIVAL_LATE_FRLG || trainerClass == TRAINER_CLASS_CHAMPION_FRLG)
            toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
        else
        {
            toCpy = GetTrainerNameFromId(trainerId);
            if (toCpy[0] == B_BUFF_PLACEHOLDER_BEGIN && toCpy[1] == B_TXT_RIVAL_NAME)
                toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
        }
    }

    assertf(DoesStringProperlyTerminate(toCpy, TRAINER_NAME_LENGTH + 1),"Opponent needs a valid name")
    {
        return sText_EmptyString4;
    }

    return toCpy;
}

static const u8 *BattleStringGetOpponentName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_OPPONENT_LEFT:
        toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    case B_POSITION_OPPONENT_RIGHT:
        if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI) && !BATTLE_TWO_VS_ONE_OPPONENT)
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, battler);
        else
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetPlayerName(u8 *text, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_PLAYER_LEFT:
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            toCpy = gLinkPlayers[0].name;
        else
            toCpy = gSaveBlock2Ptr->playerName;
        break;
    case B_POSITION_PLAYER_RIGHT:
        if (((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)))
            || gTestRunnerEnabled)
        {
            toCpy = gLinkPlayers[0].name;
        }
        else if ((gBattleTypeFlags & BATTLE_TYPE_LINK) && gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_MULTI))
        {
            toCpy = gLinkPlayers[2].name;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        {
            GetFrontierTrainerName(text, gPartnerTrainerId);
            toCpy = text;
        }
        else
        {
            toCpy = gSaveBlock2Ptr->playerName;
        }
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetTrainerName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    if (IsOnPlayerSide(battler))
        return BattleStringGetPlayerName(text, battler);
    else
        return BattleStringGetOpponentName(text, multiplayerId, battler);
}

static const u8 *BattleStringGetOpponentClassByTrainerId(u16 trainerId)
{
    const u8 *toCpy;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
        toCpy = gTrainerClasses[GetSecretBaseTrainerClass()].name;
    else if (trainerId == TRAINER_UNION_ROOM)
        toCpy = gTrainerClasses[GetUnionRoomTrainerClass()].name;
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
        toCpy = gTrainerClasses[GetFrontierBrainTrainerClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
        toCpy = gTrainerClasses[GetFrontierOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
        toCpy = gTrainerClasses[GetTrainerTowerOpponentClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
        toCpy = gTrainerClasses[GetTrainerHillOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
        toCpy = gTrainerClasses[GetEreaderTrainerClassId()].name;
    else if (trainerId == TRAINER_LINK_OPPONENT)
        toCpy = gTrainerClasses[TRAINER_NONE].name;
    else
        toCpy = gTrainerClasses[GetTrainerClassFromId(trainerId)].name;

    return toCpy;
}

// Ensure the defined length for an item name can contain the full defined length of a berry name.
// This ensures that custom Enigma Berry names will fit in the text buffer at the top of BattleStringExpandPlaceholders.
STATIC_ASSERT(BERRY_NAME_LENGTH + ARRAY_COUNT(sText_BerrySuffix) <= ITEM_NAME_LENGTH, BerryNameTooLong);

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst, u32 dstSize)
{
    u32 dstID = 0; // if they used dstID, why not use srcID as well?
    const u8 *toCpy = NULL;
    u8 text[max(max(max(32, TRAINER_NAME_LENGTH + 1), POKEMON_NAME_LENGTH + 1), ITEM_NAME_LENGTH)];
    u8 *textStart = &text[0];
    u8 multiplayerId;
    u8 fontId = FONT_NORMAL;

    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
        multiplayerId = gRecordedBattleMultiplayerId;
    else
        multiplayerId = GetMultiplayerId();

    // Clear destination first
    while (dstID < dstSize)
    {
        dst[dstID] = EOS;
        dstID++;
    }

    dstID = 0;
    while (*src != EOS)
    {
        toCpy = NULL;

        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            u32 classLength = 0;
            u32 nameLength = 0;
            const u8 *classString;
            const u8 *nameString;
            switch (*src)
            {
            case B_TXT_EUNNEUN:
            case B_TXT_IGA:
            case B_TXT_EULREUL:
            case B_TXT_EU:
            case B_TXT_I:
            case B_TXT_WAGWA:
            case B_TXT_AYA:
            {
                u8 jong = dstID >= 2 ? GetJongCode((dst[dstID - 2] << 8) | dst[dstID - 1]) : 0;

                switch (*src)
                {
                case B_TXT_EUNNEUN: toCpy = jong ? gText_ExpandedPlaceholder_Eun : gText_ExpandedPlaceholder_Neun; break;
                case B_TXT_IGA: toCpy = jong ? gText_ExpandedPlaceholder_I : gText_ExpandedPlaceholder_Ga; break;
                case B_TXT_EULREUL: toCpy = jong ? gText_ExpandedPlaceholder_Eul : gText_ExpandedPlaceholder_Reul; break;
                case B_TXT_EU: toCpy = (jong && jong != 8) ? gText_ExpandedPlaceholder_Eu : gText_ExpandedPlaceholder_Empty; break;
                case B_TXT_I: toCpy = jong ? gText_ExpandedPlaceholder_I : gText_ExpandedPlaceholder_Empty; break;
                case B_TXT_WAGWA: toCpy = jong ? gText_ExpandedPlaceholder_Gwa : gText_ExpandedPlaceholder_Wa; break;
                case B_TXT_AYA: toCpy = jong ? gText_ExpandedPlaceholder_A : gText_ExpandedPlaceholder_Ya; break;
                }
                break;
            }
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                {
                    toCpy = gBattleTextBuff2;
                }
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                {
                    toCpy = gBattleTextBuff3;
                }
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // Unused, to change into sth else.
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                GetBattlerNick(BATTLE_PARTNER(gBattlerAttacker), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget)
                break;
            case B_TXT_DEF_NAME: // target name
                GetBattlerNick(gBattlerTarget, text);
                toCpy = text;
                break;
            case B_TXT_DEF_PARTNER_NAME: // partner target name
                GetBattlerNick(BATTLE_PARTNER(gBattlerTarget), text);
                toCpy = text;
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler)
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->currentMove);
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (gBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->originallyUsedMove);
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY_E_READER)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleScripting.multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleScripting.multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilitiesInfo[gLastUsedAbility].name;
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerAttacker]].name;
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerTarget]].name;
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattleScripting.battler]].name;
                break;
            case B_TXT_EFF_ABILITY: // effect battler ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gEffectBattler]].name;
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                break;
            case B_TXT_TRAINER1_NAME_WITH_CLASS: // trainer1 name with trainer class
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(LEFT_FOE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(RIGHT_FOE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_26: // ?
                if (!IsOnPlayerSide(gBattleScripting.battler))
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        toCpy = sText_FoePkmnPrefix;
                    else
                        toCpy = sText_WildPkmnPrefix;
                    while (*toCpy != EOS)
                    {
                        dst[dstID] = *toCpy;
                        dstID++;
                        toCpy++;
                    }
                }
                GetMonData(&GetBattlerParty(gBattleScripting.battler)[gBattleStruct->scriptPartyIdx], MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_PC_LANETTE))
                    toCpy = (IS_FRLG || IS_HNS) ? sText_Bills : sText_Lanettes;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_TRAINER2_CLASS:
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                break;
            case B_TXT_TRAINER2_NAME:
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                break;
            case B_TXT_TRAINER2_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerBLoseText();
                }
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                break;
            case B_TXT_PARTNER_CLASS:
                toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                break;
            case B_TXT_PARTNER_NAME:
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                break;
            case B_TXT_RIVAL_NAME:
                toCpy = gSaveBlock2Ptr->rivalName;
                break;
            case B_TXT_PARTNER_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetPlayerName(textStart, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME:
                toCpy = BattleStringGetTrainerName(text, multiplayerId, gBattlerAttacker);
                break;
            case B_TXT_ATK_TRAINER_CLASS:
                switch (GetBattlerPosition(gBattlerAttacker))
                {
                case B_POSITION_PLAYER_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                        toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                    break;
                case B_POSITION_OPPONENT_LEFT:
                    toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                case B_POSITION_OPPONENT_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                    else
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                default:
                    break;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME_WITH_CLASS:
                toCpy = textStart;
                if (GetBattlerPosition(gBattlerAttacker) == B_POSITION_PLAYER_LEFT)
                {
                    textStart = StringCopy(textStart, BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker));
                }
                else
                {
                    classString = NULL;
                    switch (GetBattlerPosition(gBattlerAttacker))
                    {
                    case B_POSITION_PLAYER_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                            classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                        break;
                    case B_POSITION_OPPONENT_LEFT:
                        classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    case B_POSITION_OPPONENT_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                        else
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    default:
                        break;
                    }
                    classLength = 0;
                    nameLength = 0;
                    while (classString[classLength] != EOS)
                    {
                        textStart[classLength] = classString[classLength];
                        classLength++;
                    }
                    textStart[classLength] = CHAR_SPACE;
                    textStart += 1 + classLength;
                    nameString = BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker);
                    if (nameString != textStart)
                    {
                        while (nameString[nameLength] != EOS)
                        {
                            textStart[nameLength] = nameString[nameLength];
                            nameLength++;
                        }
                        textStart[nameLength] = EOS;
                    }
                }
                break;
            case B_TXT_ATK_TEAM1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_ATK_TEAM2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_DEF_TEAM1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_DEF_TEAM2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_EFF_TEAM1:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_EFF_TEAM2:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerTarget)
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattleScripting.battler)
                break;
            }

            if (toCpy != NULL)
            {
                while (*toCpy != EOS)
                {
                    // CHAR_NBSP (0x39) is also a Korean glyph lead byte.
                    // Keep a placeholder's trailing space as a normal space so
                    // it cannot consume the first byte of the following Korean glyph.
                    if (*toCpy == CHAR_SPACE && toCpy[1] != EOS && !IsKoreanGlyph(toCpy[1]))
                        dst[dstID] = CHAR_NBSP;
                    else
                        dst[dstID] = *toCpy;
                    dstID++;
                    toCpy++;
                }
            }

            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER2_LOSE_TEXT
                || *src == B_TXT_TRAINER1_WIN_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstID] = EXT_CTRL_CODE_BEGIN;
                dstID++;
                dst[dstID] = EXT_CTRL_CODE_PAUSE_UNTIL_PRESS;
                dstID++;
            }
        }
        else
        {
            dst[dstID] = *src;
            dstID++;
        }
        src++;
    }

    dst[dstID] = *src;
    dstID++;

    BreakStringAutomatic(dst, BATTLE_MSG_MAX_WIDTH, BATTLE_MSG_MAX_LINES, fontId, SHOW_SCROLL_PROMPT);

    return dstID;
}

static void IllusionNickHack(enum BattlerId battler, u32 partyId, u8 *dst)
{
    u32 id = PARTY_SIZE;
    struct Pokemon *party = GetBattlerParty(battler);
    struct Pokemon *mon = &party[partyId], *partnerMon;

    if (GetMonAbility(mon) == ABILITY_ILLUSION)
    {
        if (IsBattlerAlive(BATTLE_PARTNER(battler)))
            partnerMon = GetBattlerMon(BATTLE_PARTNER(battler));
        else
            partnerMon = mon;

        id = GetIllusionMonPartyId(party, mon, partnerMon, battler);
    }

    if (id != PARTY_SIZE)
        GetMonData(&party[id], MON_DATA_NICKNAME, dst);
    else
        GetMonData(mon, MON_DATA_NICKNAME, dst);
}

void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcID = 1;
    u32 value = 0;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    u16 hword;

    *dst = EOS;
    while (src[srcID] != B_BUFF_EOS)
    {
        switch (src[srcID])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcID + 1]);
            StringAppend(dst, gBattleStringsTable[hword]);
            srcID += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcID + 1])
            {
            case 1:
                value = src[srcID + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcID + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcID + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcID + 2]);
            srcID += src[srcID + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, GetMoveName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypesInfo[src[srcID + 1]].name);
            srcID += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
        case B_BUFF_MON_NICK_WITH_PREFIX_LOWER: // poke nick with lowercase prefix
            if (!IsOnPlayerSide(src[srcID + 1]))
            {
                if (src[srcID] == B_BUFF_MON_NICK_WITH_PREFIX_LOWER)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefixLower);
                    else
                        StringAppend(dst, sText_WildPkmnPrefixLower);
                }
                else
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefix);
                    else
                        StringAppend(dst, sText_WildPkmnPrefix);
                }
            }
            GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, nickname);
            StringGet_Nickname(nickname);
            StringAppend(dst, nickname);
            srcID += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_SPECIES: // species name
            StringCopy(dst, GetSpeciesName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (src[srcID + 2] == gBattlerPartyIndexes[src[srcID + 1]])
            {
                GetBattlerNick(src[srcID + 1], dst);
            }
            else if (gBattleScripting.illusionNickHack) // for STRINGID_ENEMYABOUTTOSWITCHPKMN
            {
                gBattleScripting.illusionNickHack = 0;
                IllusionNickHack(src[srcID + 1], src[srcID + 2], dst);
                StringGet_Nickname(dst);
            }
            else
            {
                GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, dst);
                StringGet_Nickname(dst);
            }
            srcID += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilitiesInfo[T1_READ_16(&src[srcID + 1])].name);
            srcID += 3;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcID + 1]);
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (hword == ITEM_ENIGMA_BERRY_E_READER)
                {
                    if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcID += 3;
            break;
        }
    }
}

void BattlePutTextOnWindow(const u8 *text, u8 windowId)
{
    const struct BattleWindowText *textInfo = sBattleTextOnWindowsInfo[gBattleScripting.windowsType];
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;

    if (windowId & B_WIN_COPYTOVRAM)
    {
        windowId &= ~B_WIN_COPYTOVRAM;
        copyToVram = FALSE;
    }
    else
    {
        FillWindowPixelBuffer(windowId, textInfo[windowId].fillValue);
        copyToVram = TRUE;
    }

    printerTemplate.currentChar = text;
    printerTemplate.type = WINDOW_TEXT_PRINTER;
    printerTemplate.windowId = windowId;
    printerTemplate.fontId = textInfo[windowId].fontId;
    printerTemplate.x = textInfo[windowId].x;
    printerTemplate.y = textInfo[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = textInfo[windowId].letterSpacing;
    printerTemplate.lineSpacing = textInfo[windowId].lineSpacing;
    printerTemplate.color = textInfo[windowId].color;

    if (B_WIN_MOVE_NAME_1 <= windowId && windowId <= B_WIN_MOVE_NAME_4)
    {
        // We cannot check the actual width of the window because
        // B_WIN_MOVE_NAME_1 and B_WIN_MOVE_NAME_3 are 16 wide for
        // Z-move details.
        if (gBattleStruct->zmove.viewing && windowId == B_WIN_MOVE_NAME_1)
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 16 * TILE_WIDTH);
        else
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 8 * TILE_WIDTH);
    }

    if (printerTemplate.x == 0xFF)
    {
        u32 width = GetBattleWindowTemplatePixelWidth(gBattleScripting.windowsType, windowId);
        s32 alignX = GetStringCenterAlignXOffsetWithLetterSpacing(printerTemplate.fontId, printerTemplate.currentChar, width, printerTemplate.letterSpacing);
        printerTemplate.x = printerTemplate.currentX = alignX;
    }

    if (windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)) || gTestRunnerEnabled || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != B_WIN_OAK_OLD_MAN))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == B_WIN_MSG || windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
    {
        if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            speed = 1;
        else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            speed = sRecordedBattleTextSpeeds[GetTextSpeedInRecordedBattle()];
        else
            speed = GetPlayerTextSpeedDelay();

        gTextFlags.canABSpeedUpPrint = 1;
    }
    else
    {
        speed = textInfo[windowId].speed;
        gTextFlags.canABSpeedUpPrint = 0;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);

    if (copyToVram)
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
}

void SetPpNumbersPaletteInMoveSelection(enum BattlerId battler)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct *)(&gBattleResources->bufferA[battler][4]);
    const u16 *palPtr = gPPTextPalette;
    u8 var;

    if (!gBattleStruct->zmove.viewing)
        var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[battler]],
                         chooseMoveStruct->maxPp[gMoveSelectionCursor[battler]]);
    else
        var = 3;

    gPlttBufferUnfaded[BG_PLTT_ID(5) + 12] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[BG_PLTT_ID(5) + 11] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 12], &gPlttBufferFaded[BG_PLTT_ID(5) + 12], PLTT_SIZEOF(1));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 11], &gPlttBufferFaded[BG_PLTT_ID(5) + 11], PLTT_SIZEOF(1));
}

u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
    {
        return 3;
    }
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        if (currentPp <= maxPp / 4)
            return 1;
        if (currentPp > maxPp / 2)
            return 3;
    }

    return 0;
}
