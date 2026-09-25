// multichoice lists
static const struct MenuAction MultichoiceList_BrineyOnDewford[] =
{
    {COMPOUND_STRING("등화도시")},
    {COMPOUND_STRING("잿빛도")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_EnterInfo[] =
{
    {COMPOUND_STRING("참가한")},
    {gText_Info2},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ContestInfo[] =
{
    {COMPOUND_STRING("포켓몬 콘테스트란?")},
    {COMPOUND_STRING("콘테스트의 종류")},
    {COMPOUND_STRING("랭크에 대해서")},
    {gText_Cancel2},
};

static const struct MenuAction MultichoiceList_ContestType[] =
{
    {gText_CoolnessContest},
    {gText_BeautyContest},
    {gText_CutenessContest},
    {gText_SmartnessContest},
    {gText_ToughnessContest},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BasePCWithRegistry[] =
{
    {gText_Decoration2},
    {gText_PackUp},
    {gText_Registry},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BasePCNoRegistry[] =
{
    {gText_Decoration2},
    {gText_PackUp},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_RegisterMenu[] =
{
    {gMenuText_Register},
    {gText_Registry},
    {gText_Information},
    {gText_Cancel2},
};

static const struct MenuAction MultichoiceList_Bike[] =
{
    {COMPOUND_STRING("마하")},
    {COMPOUND_STRING("더트")},
};

static const struct MenuAction MultichoiceList_StatusInfo[] =
{
    {COMPOUND_STRING("독")},
    {COMPOUND_STRING("마비")},
    {COMPOUND_STRING("잠듦")},
    {COMPOUND_STRING("화상")},
    {COMPOUND_STRING("얼음")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BrineyOffDewford[] =
{
    {COMPOUND_STRING("무로마을")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ViewedPaintings[] =
{
    {COMPOUND_STRING("봤어요")},
    {COMPOUND_STRING("볼 거예요")},
};

static const struct MenuAction MultichoiceList_YesNoInfo2[] =
{
    {gText_Yes},
    {gText_No},
    {gText_Info2},
};

static const struct MenuAction MultichoiceList_ChallengeInfo[] =
{
    {COMPOUND_STRING("도전한다")},
    {COMPOUND_STRING("설명을 듣는다")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_LevelMode[] =
{
    {gText_Lv50},
    {gText_OpenLevel},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_Mechadoll1_Q1[] =
{
    {COMPOUND_STRING("뚜벅쵸")},
    {COMPOUND_STRING("포챠나")},
    {COMPOUND_STRING("테일로")},
};

static const struct MenuAction MultichoiceList_Mechadoll1_Q2[] =
{
    {COMPOUND_STRING("루리리")},
    {COMPOUND_STRING("로파파")},
    {COMPOUND_STRING("갈모매")},
};

static const struct MenuAction MultichoiceList_Mechadoll1_Q3[] =
{
    {COMPOUND_STRING("독케일")},
    {COMPOUND_STRING("주뱃")},
    {COMPOUND_STRING("토중몬")},
};

static const struct MenuAction MultichoiceList_Mechadoll2_Q1[] =
{
    {COMPOUND_STRING("랄토스")},
    {COMPOUND_STRING("지그제구리")},
    {COMPOUND_STRING("게을로")},
};

static const struct MenuAction MultichoiceList_Mechadoll2_Q2[] =
{
    {COMPOUND_STRING("포챠나")},
    {COMPOUND_STRING("버섯꼬")},
    {COMPOUND_STRING("지그제구리")},
};

static const struct MenuAction MultichoiceList_Mechadoll2_Q3[] =
{
    {COMPOUND_STRING("포챠나")},
    {COMPOUND_STRING("주뱃")},
    {COMPOUND_STRING("샤프니아")},
};

static const struct MenuAction MultichoiceList_Mechadoll3_Q1[] =
{
    {COMPOUND_STRING("화상치료제")},
    {COMPOUND_STRING("항구메일")},
    {COMPOUND_STRING("같은 가격")},
};

static const struct MenuAction MultichoiceList_Mechadoll3_Q2[] =
{
    {COMPOUND_STRING("60원")},
    {COMPOUND_STRING("¥55원")},
    {COMPOUND_STRING("남지 않는다")},
};

static const struct MenuAction MultichoiceList_Mechadoll3_Q3[] =
{
    {COMPOUND_STRING("비싸다")},
    {COMPOUND_STRING("싸다")},
    {COMPOUND_STRING("같은 가격")},
};

static const struct MenuAction MultichoiceList_Mechadoll4_Q1[] =
{
    {COMPOUND_STRING("남성")},
    {COMPOUND_STRING("여성")},
    {COMPOUND_STRING("둘 다 아니다")},
};

static const struct MenuAction MultichoiceList_Mechadoll4_Q2[] =
{
    {COMPOUND_STRING("할아버지")},
    {COMPOUND_STRING("할머니")},
    {COMPOUND_STRING("같은 가격")},
};

static const struct MenuAction MultichoiceList_Mechadoll4_Q3[] =
{
    {COMPOUND_STRING("없다")},
    {COMPOUND_STRING("한 명")},
    {COMPOUND_STRING("두 명")},
};

static const struct MenuAction MultichoiceList_Mechadoll5_Q1[] =
{
    {COMPOUND_STRING("두 마리")},
    {COMPOUND_STRING("세 마리")},
    {COMPOUND_STRING("네 마리")},
};

static const struct MenuAction MultichoiceList_Mechadoll5_Q2[] =
{
    {COMPOUND_STRING("6채")},
    {COMPOUND_STRING("7채")},
    {COMPOUND_STRING("8채")},
};

static const struct MenuAction MultichoiceList_Mechadoll5_Q3[] =
{
    {COMPOUND_STRING("여섯 명")},
    {COMPOUND_STRING("일곱 명")},
    {COMPOUND_STRING("여덟 명")},
};

static const struct MenuAction MultichoiceList_VendingMachine[] =
{
    {COMPOUND_STRING("맛있는물{CLEAR_TO 0x48}200원")},
    {COMPOUND_STRING("미네랄사이다{CLEAR_TO 0x48}300원")},
    {COMPOUND_STRING("후르츠밀크{CLEAR_TO 0x48}350원")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_MachBikeInfo[] =
{
    {COMPOUND_STRING("타는 방법")},
    {COMPOUND_STRING("도는 방법")},
    {COMPOUND_STRING("모래 비탈길")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_AcroBikeInfo[] =
{
    {COMPOUND_STRING("윌리")},
    {COMPOUND_STRING("다니엘")},
    {COMPOUND_STRING("점프")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_Satisfaction[] =
{
    {COMPOUND_STRING("만족")},
    {COMPOUND_STRING("불만")},
};

static const struct MenuAction MultichoiceList_SternDeepSea[] =
{
    {COMPOUND_STRING("심해의이빨")},
    {COMPOUND_STRING("심해의비늘")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_UnusedAshVendor[] =
{
    {COMPOUND_STRING("파랑비드로")},
    {COMPOUND_STRING("노랑비드로")},
    {COMPOUND_STRING("빨강비드로")},
    {COMPOUND_STRING("하양비드로")},
    {COMPOUND_STRING("검정비드로")},
    {COMPOUND_STRING("고운 의자")},
    {COMPOUND_STRING("고운 책")},
    {gText_Cancel2},
};

static const struct MenuAction MultichoiceList_GameCornerDolls[] =
{
    {COMPOUND_STRING("나무지기인형{CLEAR_TO 0x3a}1000개")},
    {COMPOUND_STRING("아차모인형      1000개")},
    {COMPOUND_STRING("물짱이인형      1000개")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_GameCornerDolls2[] =
{
    {COMPOUND_STRING("레지락인형      9000개")},
    {COMPOUND_STRING("레지아이스인형{CLEAR_TO 0x3a}9000개")},
    {COMPOUND_STRING("레지스틸인형{CLEAR_TO 0x3a}9000개")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_PrizeMons[] = 
{
    {COMPOUND_STRING("케이시{CLEAR_TO 0x40}120개")},
    {COMPOUND_STRING("삐삐{CLEAR_TO 0x40}500개")},
    {COMPOUND_STRING("먹고자{CLEAR_TO 0x3a}2800개")},
    {COMPOUND_STRING("미뇽{CLEAR_TO 0x3a}5500개")},
    {COMPOUND_STRING("폴리곤{CLEAR_TO 0x3a}6500개")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_GameCornerTMs[] =
{
    {COMPOUND_STRING("그림자분신{CLEAR_TO 0x3a}1500개")},
    {COMPOUND_STRING("사이코키네시스{CLEAR_TO 0x3a}3500개")},
    {COMPOUND_STRING("냉동빔{CLEAR_TO 0x3a}4000개")},
    {COMPOUND_STRING("10만볼트{CLEAR_TO 0x3a}4000개")},
    {COMPOUND_STRING("화염방사{CLEAR_TO 0x3a}4000개")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_GameCornerCoins[] =
{
    {COMPOUND_STRING("    50개   {CLEAR_TO 0x03}1000원")},
    {COMPOUND_STRING("  500개 10000원")},
    {COMPOUND_STRING("1000개 10000원")},
    {COMPOUND_STRING("2500개 25000원")},
    {COMPOUND_STRING("5000개 50000원")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_HowsFishing[] =
{
    {COMPOUND_STRING("최고예요")},
    {COMPOUND_STRING("그냥 그래요")},
};

static const struct MenuAction MultichoiceList_SSTidalSlateportWithBF[] =
{
    {gText_LilycoveCity},
    {gText_BattleFrontier},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_SSTidalBattleFrontier[] =
{
    {gText_SlateportCity},
    {gText_LilycoveCity},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_RightLeft[] =
{
    {COMPOUND_STRING("오른쪽")},
    {COMPOUND_STRING("왼쪽")},
};

static const struct MenuAction MultichoiceList_SSTidalSlateportNoBF[] =
{
    {gText_LilycoveCity},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_Floors[] =
{
    {gText_5F},
    {gText_4F},
    {gText_3F},
    {gText_2F},
    {gText_1F},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsR[] =
{
    {gText_RedShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsY[] =
{
    {gText_YellowShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRY[] =
{
    {gText_RedShard},
    {gText_YellowShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsB[] =
{
    {gText_BlueShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRB[] =
{
    {gText_RedShard},
    {gText_BlueShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsYB[] =
{
    {gText_YellowShard},
    {gText_BlueShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRYB[] =
{
    {gText_RedShard},
    {gText_YellowShard},
    {gText_BlueShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsG[] =
{
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRG[] =
{
    {gText_RedShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsYG[] =
{
    {gText_YellowShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRYG[] =
{
    {gText_RedShard},
    {gText_YellowShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsBG[] =
{
    {gText_BlueShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRBG[] =
{
    {gText_RedShard},
    {gText_BlueShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsYBG[] =
{
    {gText_YellowShard},
    {gText_BlueShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ShardsRYBG[] =
{
    {gText_RedShard},
    {gText_YellowShard},
    {gText_BlueShard},
    {gText_GreenShard},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_TourneyWithRecord[] =
{
    {gText_Opponent},
    {gText_Tourney_Tree},
    {gText_ReadyToStart},
    {gText_Record2},
    {gText_Rest},
    {gText_Retire},
};

static const struct MenuAction MultichoiceList_TourneyNoRecord[] =
{
    {gText_Opponent},
    {gText_Tourney_Tree},
    {gText_ReadyToStart},
    {gText_Rest},
    {gText_Retire},
};

static const struct MenuAction MultichoiceList_Tent[] =
{
    {COMPOUND_STRING("빨강텐트")},
    {COMPOUND_STRING("파랑텐")},
};

static const struct MenuAction MultichoiceList_LinkServicesNoBerry[] =
{
    {gText_TradeCenter},
    {gText_Colosseum},
    {gText_RecordCorner},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_YesNoInfo[] =
{
    {gText_Yes},
    {gText_No},
    {gText_Info2},
};

static const struct MenuAction MultichoiceList_BattleMode[] =
{
    {COMPOUND_STRING("싱글배틀")},
    {COMPOUND_STRING("더블배틀")},
    {COMPOUND_STRING("멀티배")},
    {gText_Info2},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_LinkServicesNoRecord[] =
{
    {gText_TradeCenter},
    {gText_Colosseum},
    {gText_BerryCrush3},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_LinkServicesAll[] =
{
    {gText_TradeCenter},
    {gText_Colosseum},
    {gText_RecordCorner},
    {gText_BerryCrush3},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_LinkServicesNoRecordBerry[] =
{
    {gText_TradeCenter},
    {gText_Colosseum},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_WirelessMinigame[] =
{
    {COMPOUND_STRING("미니 포켓몬 점프")},
    {COMPOUND_STRING("두트리오 나무열매먹기")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_LinkLeader[] =
{
    {COMPOUND_STRING("그룹에 들어간다")},
    {COMPOUND_STRING("리더가 된다")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ContestRank[] =
{
    {COMPOUND_STRING("노말랭크")},
    {COMPOUND_STRING("슈퍼랭크")},
    {COMPOUND_STRING("하이퍼랭크")},
    {COMPOUND_STRING("마스터랭크")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_FrontierItemChoose[] =
{
    {COMPOUND_STRING("배틀백")},
    {COMPOUND_STRING("지닌물건")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_LinkContestInfo[] =
{
    {COMPOUND_STRING("통신 콘테스")},
    {COMPOUND_STRING("에메랄드 모드에 대해서")},
    {COMPOUND_STRING("글로벌 모드에 대해서")},
    {gText_Cancel2},
};

static const struct MenuAction MultichoiceList_LinkContestMode[] =
{
    {COMPOUND_STRING("에메랄드 모드")},
    {COMPOUND_STRING("글로벌 모드")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_ForcedStartMenu[] =
{
    {gText_MenuOptionPokedex},
    {gText_MenuOptionPokemon},
    {gText_MenuOptionBag},
    {gText_MenuOptionPokenav},
    {gText_Blank}, // blank because it's filled by the player's name
    {gText_MenuOptionSave},
    {gText_MenuOptionOption},
    {gText_MenuOptionExit},
};

static const struct MenuAction MultichoiceList_FrontierGamblerBet[] =
{
    {COMPOUND_STRING("  5BP")},
    {COMPOUND_STRING("10BP")},
    {COMPOUND_STRING("15BP")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_UnusedSSTidal1[] =
{
    {gText_SouthernIsland},
    {gText_BirthIsland},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_UnusedSSTidal2[] =
{
    {gText_SouthernIsland},
    {gText_FarawayIsland},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_UnusedSSTidal3[] =
{
    {gText_BirthIsland},
    {gText_FarawayIsland},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_UnusedSSTidal4[] =
{
    {gText_SouthernIsland},
    {gText_BirthIsland},
    {gText_FarawayIsland},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_Fossil[] =
{
    {COMPOUND_STRING("발톱화석")},
    {COMPOUND_STRING("뿌리화석")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_FossilHns[] =
{
    {COMPOUND_STRING("발톱화석")},
    {COMPOUND_STRING("뿌리화석")},
    {COMPOUND_STRING("조개화석")},
    {COMPOUND_STRING("껍질화석")},
    {COMPOUND_STRING("비밀의호박")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_YesNo[] =
{
    {gText_Yes},
    {COMPOUND_STRING("아니")},
};

static const struct MenuAction MultichoiceList_FrontierRules[] =
{
    {COMPOUND_STRING("2개의 코스")},
    {COMPOUND_STRING("레벨 50")},
    {COMPOUND_STRING("오픈 레")},
    {COMPOUND_STRING("포켓몬의 종류와 수")},
    {COMPOUND_STRING("지닌물건")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_FrontierPassInfo[] =
{
    {COMPOUND_STRING("심볼")},
    {COMPOUND_STRING("대전 기록")},
    {COMPOUND_STRING("배틀포인트")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattleArenaRules[] =
{
    {gText_BattleRules},
    {gText_JudgeMind},
    {gText_JudgeSkill},
    {gText_JudgeBody},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattleTowerRules[] =
{
    {COMPOUND_STRING("타워에 대해서")},
    {COMPOUND_STRING("데려가는 포켓몬")},
    {COMPOUND_STRING("배틀살롱")},
    {COMPOUND_STRING("통신멀티")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattleDomeRules[] =
{
    {COMPOUND_STRING("조합")},
    {COMPOUND_STRING("토너먼트 표")},
    {COMPOUND_STRING("더블녹아웃")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattleFactoryRules[] =
{
    {gText_BasicRules},
    {gText_SwapPartners},
    {gText_SwapNumber},
    {gText_SwapNotes},
    {COMPOUND_STRING("오픈 레")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattlePalaceRules[] =
{
    {gText_BattleBasics},
    {gText_PokemonNature},
    {gText_PokemonMoves},
    {gText_Underpowered},
    {gText_WhenInDanger},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattlePyramidRules[] =
{
    {COMPOUND_STRING("피라미드의 포켓몬")},
    {COMPOUND_STRING("피라미드의 트레이너")},
    {COMPOUND_STRING("피라미드의 미로")},
    {COMPOUND_STRING("배틀백")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattlePikeRules[] =
{
    {COMPOUND_STRING("포켓내비와 가방")},
    {COMPOUND_STRING("지닌물건")},
    {COMPOUND_STRING("포켓몬의 순")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_GoOnRecordRestRetire[] =
{
    {gText_GoOn},
    {gText_Record2},
    {gText_Rest},
    {gText_Retire},
};

static const struct MenuAction MultichoiceList_GoOnRestRetire[] =
{
    {gText_GoOn},
    {gText_Rest},
    {gText_Retire},
};

static const struct MenuAction MultichoiceList_GoOnRecordRetire[] =
{
    {gText_GoOn},
    {gText_Record2},
    {gText_Retire},
};

static const struct MenuAction MultichoiceList_GoOnRetire[] =
{
    {gText_GoOn},
    {gText_Retire},
};

static const struct MenuAction MultichoiceList_TVLati[] =
{
    {COMPOUND_STRING("빨강")},
    {COMPOUND_STRING("파랑")},
};

static const struct MenuAction MultichoiceList_BattleTowerFeelings[] =
{
    {COMPOUND_STRING("지금부터 승부다!")},
    {COMPOUND_STRING("승부에서 이겼다!")},
    {COMPOUND_STRING("승부에서 졌다!")},
    {COMPOUND_STRING("가르쳐 주지 않는다")},
};

static const struct MenuAction MultichoiceList_WheresRayquaza[] =
{
    {COMPOUND_STRING("각성의사당")},
    {COMPOUND_STRING("송화산")},
    {COMPOUND_STRING("하늘기둥")},
    {COMPOUND_STRING("기억하지 않는다")},
};

static const struct MenuAction MultichoiceList_SlateportTentRules[] =
{
    {gText_BasicRules},
    {gText_SwapPartners},
    {gText_SwapNumber},
    {gText_SwapNotes},
    {gText_BattlePokemon},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_FallarborTentRules[] =
{
    {gText_BattleTrainers},
    {gText_BattleRules},
    {gText_JudgeMind},
    {gText_JudgeSkill},
    {gText_JudgeBody},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_TagMatchType[] =
{
    {gText_NormalTagMatch},
    {gText_VarietyTagMatch},
    {gText_UniqueTagMatch},
    {gText_ExpertTagMatch},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BerryPlot[] =
{
    {COMPOUND_STRING("비료를 준다")},
    {COMPOUND_STRING("나무열매를 심는다")},
    {gText_Exit},
};

static const struct MenuAction sMultichoiceList_BikeShop[] = {
    { COMPOUND_STRING("자전거{CLEAR_TO 0x49}1000000원") },
    { COMPOUND_STRING("NO THANKS") }
};

static const struct MenuAction sMultichoiceList_Eeveelutions[] = {
    { COMPOUND_STRING("EEVEE") },
    { COMPOUND_STRING("FLAREON") },
    { COMPOUND_STRING("JOLTEON") },
    { COMPOUND_STRING("VAPOREON") },
    { COMPOUND_STRING("Quit looking.") }
};

static const u8 gText_SeviiIslands[] = _("SEVII ISLANDS");
static const u8 gText_OneIsland[] = _("ONE ISLAND");
static const u8 gText_TwoIsland[] = _("TWO ISLAND");
static const u8 gText_ThreeIsland[] = _("THREE ISLAND");
static const u8 gText_FourIsland[] = _("FOUR ISLAND");
static const u8 gText_FiveIsland[] = _("FIVE ISLAND");
static const u8 gText_SixIsland[] = _("SIX ISLAND");
static const u8 gText_SevenIsland[] = _("SEVEN ISLAND");

static const struct MenuAction sMultichoiceList_Island23[] = {
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_Island13[] = {
    { gText_OneIsland },
    { gText_ThreeIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_Island12[] = {
    { gText_OneIsland },
    { gText_TwoIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeviiNavel[] = {
    { gText_SeviiIslands },
    { gText_NavelRock },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeviiBirth[] = {
    { gText_SeviiIslands },
    { gText_BirthIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeviiNavelBirth[] = {
    { gText_SeviiIslands },
    { gText_NavelRock },
    { gText_BirthIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_Seagallop123[] = {
    { gText_OneIsland },
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeagallopV23[] = {
    { gText_Vermilion },
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeagallopV13[] = {
    { gText_Vermilion },
    { gText_OneIsland },
    { gText_ThreeIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeagallopV12[] = {
    { gText_Vermilion },
    { gText_OneIsland },
    { gText_TwoIsland },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_SeagallopVermilion[] = {
    { gText_Vermilion },
    { gText_Exit }
};

const u8 sText_NoThanks[] = _("NO THANKS");

static const struct MenuAction sMultichoiceList_GameCornerPokemonPrizes[] = {
#if defined(FIRERED)
    { COMPOUND_STRING("ABRA{CLEAR_TO 0x55} 180 COINS") },
    { COMPOUND_STRING("CLEFAIRY{CLEAR_TO 0x55} 500 COINS") },
    { COMPOUND_STRING("DRATINI{CLEAR_TO 0x4B} 2,800 COINS") },
    { COMPOUND_STRING("SCYTHER{CLEAR_TO 0x4B} 5,500 COINS") },
    { COMPOUND_STRING("PORYGON{CLEAR_TO 0x4B} 9,999 COINS") },
#else
    { COMPOUND_STRING("ABRA{CLEAR_TO 0x55} 120") },
    { COMPOUND_STRING("CLEFAIRY{CLEAR_TO 0x55} 750") },
    { COMPOUND_STRING("PINSIR{CLEAR_TO 0x4B} 2,500") },
    { COMPOUND_STRING("DRATINI{CLEAR_TO 0x4B} 4,600") },
    { COMPOUND_STRING("PORYGON{CLEAR_TO 0x4B} 6,500") },
#endif
    { sText_NoThanks }
};

static const struct MenuAction sMultichoiceList_GameCornerTMPrizes[] = {
    { COMPOUND_STRING("TM13{CLEAR_TO 0x48}4,000 COINS") },
    { COMPOUND_STRING("TM23{CLEAR_TO 0x48}3,500 COINS") },
    { COMPOUND_STRING("TM24{CLEAR_TO 0x48}4,000 COINS") },
    { COMPOUND_STRING("TM30{CLEAR_TO 0x48}4,500 COINS") },
    { COMPOUND_STRING("TM35{CLEAR_TO 0x48}4,000 COINS") },
    { sText_NoThanks }
};

static const struct MenuAction sMultichoiceList_GameCornerBattleItemPrizes[] = {
    { COMPOUND_STRING("SMOKE BALL{CLEAR_TO 0x5A}800 COINS") },
    { COMPOUND_STRING("MIRACLE SEED{CLEAR_TO 0x50}1,000 COINS") },
    { COMPOUND_STRING("CHARCOAL{CLEAR_TO 0x50}1,000 COINS") },
    { COMPOUND_STRING("MYSTIC WATER{CLEAR_TO 0x50}1,000 COINS") },
    { COMPOUND_STRING("YELLOW FLUTE{CLEAR_TO 0x50}1,600 COINS") },
    { sText_NoThanks }
};

static const struct MenuAction sMultichoiceList_DeptStoreElevator[] = {
    { COMPOUND_STRING("5F") },
    { COMPOUND_STRING("4F") },
    { COMPOUND_STRING("3F") },
    { COMPOUND_STRING("2F") },
    { COMPOUND_STRING("1F") },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_GameCornerCoinPurchaseCounter[] = {
    { COMPOUND_STRING(" 50 COINS{CLEAR_TO 0x45}¥1,000") },
    { COMPOUND_STRING("500 COINS{CLEAR_TO 0x40}¥10,000") },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_LinkedDirectUnion[] = {
    { COMPOUND_STRING("LINKED GAME PLAY") },
    { COMPOUND_STRING("DIRECT CORNER") },
    { COMPOUND_STRING("UNION ROOM") },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_CeladonVendingMachine[] = {
    { COMPOUND_STRING("FRESH WATER{CLEAR_TO 0x57}¥200") },
    { COMPOUND_STRING("SODA POP{CLEAR_TO 0x57}¥300") },
    { COMPOUND_STRING("LEMONADE{CLEAR_TO 0x57}¥350") },
    { gText_Exit }
};

const u8 sText_FreshWater[] = _("FRESH WATER");
const u8 sText_SodaPop[] = _("SODA POP");
const u8 sText_Lemonade[] = _("LEMONADE");

static const struct MenuAction sMultichoiceList_ThirstyGirlFreshWater[] = {
    { sText_FreshWater },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_ThirstyGirlSodaPop[] = {
    { sText_SodaPop },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_ThirstyGirlFreshWaterSodaPop[] = {
    { sText_FreshWater },
    { sText_SodaPop },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_ThirstyGirlLemonade[] = {
    { sText_Lemonade },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_ThirstyGirlFreshWaterLemonade[] = {
    { sText_FreshWater },
    { sText_Lemonade },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_ThirstyGirlSodaPopLemonade[] = {
    { sText_SodaPop },
    { sText_Lemonade },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_ThirstyGirlFreshWaterSodaPopLemonade[] = {
    { sText_FreshWater },
    { sText_SodaPop },
    { sText_Lemonade },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_RocketHideoutElevator[] = {
    { gText_B1F },
    { gText_B2F },
    { gText_B4F },
    { gText_Exit }
};

static const u8 sText_HelixFossil[] = _("HELIX FOSSIL");
static const u8 sText_DomeFossil[] = _("DOME FOSSIL");
static const u8 sText_OldAmber[] = _("OLD AMBER");

static const struct MenuAction sMultichoiceList_Helix[] = {
    { sText_HelixFossil },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_Dome[] = {
    { sText_DomeFossil },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_Amber[] = {
    { sText_OldAmber },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_HelixAmber[] = {
    { sText_HelixFossil },
    { sText_OldAmber },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_DomeAmber[] = {
    { sText_DomeFossil },
    { sText_OldAmber },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_Mushrooms[] = {
    { COMPOUND_STRING("2 TINYMUSHROOMS") },
    { COMPOUND_STRING("1 BIG MUSHROOM") }
};

static const struct MenuAction sMultichoiceList_RooftopB1F[] = {
    { gText_Rooftop },
    { gText_B1F },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_TrainerTowerMode[] = {
    { gText_Single },
    { gText_Double },
    { gText_Knockout },
    { gText_Mixed },
    { gText_Exit }
};

static const struct MenuAction sMultichoiceList_TrainerCardIconTint[] = {
    { gText_Normal },
    { gText_DexSearchColorBlack },
    { gText_DexSearchColorPink },
    { COMPOUND_STRING("SEPIA") }
};

static const u8 sText_Eggs[] = _("EGGS");
static const u8 sText_Victories[] = _("VICTORIES");

static const struct MenuAction sMultichoiceList_HOF_Quit[] = {
    { gText_HallOfFame },
    { gText_ShopQuit }
};

static const struct MenuAction sMultichoiceList_Eggs_Quit[] = {
    { sText_Eggs },
    { gText_ShopQuit }
};

static const struct MenuAction sMultichoiceList_Victories_Quit[] = {
    { sText_Victories },
    { gText_ShopQuit }
};

static const struct MenuAction sMultichoiceList_HOF_Eggs_Quit[] = {
    { gText_HallOfFame },
    { sText_Eggs },
    { gText_ShopQuit }
};

static const struct MenuAction sMultichoiceList_HOF_Victories_Quit[] = {
    { gText_HallOfFame },
    { sText_Victories },
    { gText_ShopQuit }
};

static const struct MenuAction sMultichoiceList_Eggs_Victories_Quit[] = {
    { sText_Eggs },
    { sText_Victories },
    { gText_ShopQuit }
};

static const struct MenuAction sMultichoiceList_HOF_Eggs_Victories_Quit[] = {
    { gText_HallOfFame },
    { sText_Eggs },
    { sText_Victories },
    { gText_ShopQuit }
};


static const struct MenuAction MultichoiceList_DaysOfWeek[] =
{
    {gText_Sunday},
    {gText_Monday},
    {gText_Tuesday},
    {gText_Wednesday},
    {gText_Thursday},
    {gText_Friday},
    {gText_Saturday},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_KurtsBalls[] =
{
    {gText_LoveBall},
    {gText_Lure},
    {gText_FriendBall},
    {gText_Heavy},
    {gText_Moon},
    {gText_Fast},
    {gText_LevelBall},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_OlivineHarbor[] =
{
    {gText_Vermilion},   
    {gText_SouthernIsland},
    {gText_BirthIsland},
    {gText_FarawayIsland},
    {gText_BattleFrontier},
    {gText_Exit},
};
static const struct MenuAction MultichoiceList_VermilionHarbor[] =
{
    {gText_Olivine},   
    {gText_SouthernIsland},
    {gText_BirthIsland},
    {gText_FarawayIsland},
    {gText_BattleFrontier},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_7Floors[] = 
{
    {gText_Floor6},
    {gText_Floor5},
    {gText_Floor4},
    {gText_Floor3},
    {gText_Floor2},
    {gText_Floor1},
    {gText_Floor0},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_5Floors[] = 
{
    {gText_Floor5},
    {gText_Floor4},
    {gText_Floor3},
    {gText_Floor2},
    {gText_Floor1},
    {gText_Exit},   
};

static const struct MenuAction MultichoiceList_GoldSilver[] = 
{
    {gText_Gold},
    {gText_Silver},
};



static const struct MenuAction MultichoiceList_ElderQuiz1[] = 
{
    {gText_Pal},
    {gText_Underling},
    {gText_Friend},
};
static const struct MenuAction MultichoiceList_ElderQuiz2[] = 
{
    {gText_Strategy},
    {gText_Training},
    {gText_Cheating},
};
static const struct MenuAction MultichoiceList_ElderQuiz3[] = 
{
    {gText_WeakPerson},
    {gText_ToughPerson},
    {gText_Anybody},
};
static const struct MenuAction MultichoiceList_ElderQuiz4[] = 
{
    {gText_Love2},
    {gText_Violence},
    {gText_Knowledge},
};
static const struct MenuAction MultichoiceList_ElderQuiz5[] = 
{
    {gText_Tough3},
    {gText_Weak},
    {gText_Both},
};
static const struct MenuAction MultichoiceList_HoennStarters[] = 
{
    {gText_GreenStone},
    {gText_RedStone},
    {gText_BlueStone},
};

static const struct MenuAction MultichoiceList_MomMenu[] =
{
    {gText_MomMenuCheckSavings},
    {gText_MomMenuDeposit},
    {gText_MomMenuWithdraw},
    {gText_MomMenuToggleSaving},
    {gText_MomMenuExit},
};

static const struct MenuAction MultichoiceList_LinkServicesHns[] =
{
    {gText_Trade},
    {gText_Battle},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_BattleModeHns[] =
{
    {COMPOUND_STRING("싱글배틀")},
    {COMPOUND_STRING("더블배틀")},
    {gText_Exit},
};

static const struct MenuAction MultichoiceList_Exit[] =
{
    {gText_Exit},
};

struct MultichoiceListStruct
{
    const struct MenuAction *list;
    u8 count;
};

static const struct MultichoiceListStruct sMultichoiceLists[] =
{
    [MULTI_BRINEY_ON_DEWFORD]          = MULTICHOICE(MultichoiceList_BrineyOnDewford),
    [MULTI_PC]                         = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_ENTERINFO]                  = MULTICHOICE(MultichoiceList_EnterInfo),
    [MULTI_CONTEST_INFO]               = MULTICHOICE(MultichoiceList_ContestInfo),
    [MULTI_CONTEST_TYPE]               = MULTICHOICE(MultichoiceList_ContestType),
    [MULTI_BASE_PC_NO_REGISTRY]        = MULTICHOICE(MultichoiceList_BasePCNoRegistry),
    [MULTI_BASE_PC_WITH_REGISTRY]      = MULTICHOICE(MultichoiceList_BasePCWithRegistry),
    [MULTI_REGISTER_MENU]              = MULTICHOICE(MultichoiceList_RegisterMenu),
    [MULTI_SSTIDAL_LILYCOVE]           = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_UNUSED_9]                   = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_UNUSED_10]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_FRONTIER_PASS_INFO]         = MULTICHOICE(MultichoiceList_FrontierPassInfo),
    [MULTI_BIKE]                       = MULTICHOICE(MultichoiceList_Bike),
    [MULTI_STATUS_INFO]                = MULTICHOICE(MultichoiceList_StatusInfo),
    [MULTI_BRINEY_OFF_DEWFORD]         = MULTICHOICE(MultichoiceList_BrineyOffDewford),
    [MULTI_UNUSED_15]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_VIEWED_PAINTINGS]           = MULTICHOICE(MultichoiceList_ViewedPaintings),
    [MULTI_YESNOINFO]                  = MULTICHOICE(MultichoiceList_YesNoInfo),
    [MULTI_BATTLE_MODE]                = MULTICHOICE(MultichoiceList_BattleMode),
    [MULTI_UNUSED_19]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_YESNOINFO_2]                = MULTICHOICE(MultichoiceList_YesNoInfo2),
    [MULTI_UNUSED_21]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_UNUSED_22]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_CHALLENGEINFO]              = MULTICHOICE(MultichoiceList_ChallengeInfo),
    [MULTI_LEVEL_MODE]                 = MULTICHOICE(MultichoiceList_LevelMode),
    [MULTI_MECHADOLL1_Q1]              = MULTICHOICE(MultichoiceList_Mechadoll1_Q1),
    [MULTI_MECHADOLL1_Q2]              = MULTICHOICE(MultichoiceList_Mechadoll1_Q2),
    [MULTI_MECHADOLL1_Q3]              = MULTICHOICE(MultichoiceList_Mechadoll1_Q3),
    [MULTI_MECHADOLL2_Q1]              = MULTICHOICE(MultichoiceList_Mechadoll2_Q1),
    [MULTI_MECHADOLL2_Q2]              = MULTICHOICE(MultichoiceList_Mechadoll2_Q2),
    [MULTI_MECHADOLL2_Q3]              = MULTICHOICE(MultichoiceList_Mechadoll2_Q3),
    [MULTI_MECHADOLL3_Q1]              = MULTICHOICE(MultichoiceList_Mechadoll3_Q1),
    [MULTI_MECHADOLL3_Q2]              = MULTICHOICE(MultichoiceList_Mechadoll3_Q2),
    [MULTI_MECHADOLL3_Q3]              = MULTICHOICE(MultichoiceList_Mechadoll3_Q3),
    [MULTI_MECHADOLL4_Q1]              = MULTICHOICE(MultichoiceList_Mechadoll4_Q1),
    [MULTI_MECHADOLL4_Q2]              = MULTICHOICE(MultichoiceList_Mechadoll4_Q2),
    [MULTI_MECHADOLL4_Q3]              = MULTICHOICE(MultichoiceList_Mechadoll4_Q3),
    [MULTI_MECHADOLL5_Q1]              = MULTICHOICE(MultichoiceList_Mechadoll5_Q1),
    [MULTI_MECHADOLL5_Q2]              = MULTICHOICE(MultichoiceList_Mechadoll5_Q2),
    [MULTI_MECHADOLL5_Q3]              = MULTICHOICE(MultichoiceList_Mechadoll5_Q3),
    [MULTI_UNUSED_40]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_UNUSED_41]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_VENDING_MACHINE]            = MULTICHOICE(MultichoiceList_VendingMachine),
    [MULTI_MACH_BIKE_INFO]             = MULTICHOICE(MultichoiceList_MachBikeInfo),
    [MULTI_ACRO_BIKE_INFO]             = MULTICHOICE(MultichoiceList_AcroBikeInfo),
    [MULTI_SATISFACTION]               = MULTICHOICE(MultichoiceList_Satisfaction),
    [MULTI_STERN_DEEPSEA]              = MULTICHOICE(MultichoiceList_SternDeepSea),
    [MULTI_UNUSED_ASH_VENDOR]          = MULTICHOICE(MultichoiceList_UnusedAshVendor),
    [MULTI_GAME_CORNER_DOLLS]          = MULTICHOICE(MultichoiceList_GameCornerDolls),
    [MULTI_GAME_CORNER_COINS]          = MULTICHOICE(MultichoiceList_GameCornerCoins),
    [MULTI_HOWS_FISHING]               = MULTICHOICE(MultichoiceList_HowsFishing),
    [MULTI_UNUSED_51]                  = MULTICHOICE(MultichoiceList_Exit),
    [MULTI_SSTIDAL_SLATEPORT_WITH_BF]  = MULTICHOICE(MultichoiceList_SSTidalSlateportWithBF),
    [MULTI_SSTIDAL_BATTLE_FRONTIER]    = MULTICHOICE(MultichoiceList_SSTidalBattleFrontier),
    [MULTI_RIGHTLEFT]                  = MULTICHOICE(MultichoiceList_RightLeft),
    [MULTI_GAME_CORNER_TMS]            = MULTICHOICE(MultichoiceList_GameCornerTMs),
    [MULTI_SSTIDAL_SLATEPORT_NO_BF]    = MULTICHOICE(MultichoiceList_SSTidalSlateportNoBF),
    [MULTI_FLOORS]                     = MULTICHOICE(MultichoiceList_Floors),
    [MULTI_SHARDS_R]                   = MULTICHOICE(MultichoiceList_ShardsR),
    [MULTI_SHARDS_Y]                   = MULTICHOICE(MultichoiceList_ShardsY),
    [MULTI_SHARDS_RY]                  = MULTICHOICE(MultichoiceList_ShardsRY),
    [MULTI_SHARDS_B]                   = MULTICHOICE(MultichoiceList_ShardsB),
    [MULTI_SHARDS_RB]                  = MULTICHOICE(MultichoiceList_ShardsRB),
    [MULTI_SHARDS_YB]                  = MULTICHOICE(MultichoiceList_ShardsYB),
    [MULTI_SHARDS_RYB]                 = MULTICHOICE(MultichoiceList_ShardsRYB),
    [MULTI_SHARDS_G]                   = MULTICHOICE(MultichoiceList_ShardsG),
    [MULTI_SHARDS_RG]                  = MULTICHOICE(MultichoiceList_ShardsRG),
    [MULTI_SHARDS_YG]                  = MULTICHOICE(MultichoiceList_ShardsYG),
    [MULTI_SHARDS_RYG]                 = MULTICHOICE(MultichoiceList_ShardsRYG),
    [MULTI_SHARDS_BG]                  = MULTICHOICE(MultichoiceList_ShardsBG),
    [MULTI_SHARDS_RBG]                 = MULTICHOICE(MultichoiceList_ShardsRBG),
    [MULTI_SHARDS_YBG]                 = MULTICHOICE(MultichoiceList_ShardsYBG),
    [MULTI_SHARDS_RYBG]                = MULTICHOICE(MultichoiceList_ShardsRYBG),
    [MULTI_TOURNEY_WITH_RECORD]        = MULTICHOICE(MultichoiceList_TourneyWithRecord),
    [MULTI_CABLE_CLUB_NO_RECORD_MIX]   = MULTICHOICE(MultichoiceList_LinkServicesNoRecordBerry),
    [MULTI_WIRELESS_NO_RECORD_BERRY]   = MULTICHOICE(MultichoiceList_LinkServicesNoRecordBerry),
    [MULTI_CABLE_CLUB_WITH_RECORD_MIX] = MULTICHOICE(MultichoiceList_LinkServicesNoBerry),
    [MULTI_WIRELESS_NO_BERRY]          = MULTICHOICE(MultichoiceList_LinkServicesNoBerry),
    [MULTI_WIRELESS_NO_RECORD]         = MULTICHOICE(MultichoiceList_LinkServicesNoRecord),
    [MULTI_WIRELESS_ALL_SERVICES]      = MULTICHOICE(MultichoiceList_LinkServicesAll),
    [MULTI_WIRELESS_MINIGAME]          = MULTICHOICE(MultichoiceList_WirelessMinigame),
    [MULTI_LINK_LEADER]                = MULTICHOICE(MultichoiceList_LinkLeader),
    [MULTI_CONTEST_RANK]               = MULTICHOICE(MultichoiceList_ContestRank),
    [MULTI_FRONTIER_ITEM_CHOOSE]       = MULTICHOICE(MultichoiceList_FrontierItemChoose),
    [MULTI_LINK_CONTEST_INFO]          = MULTICHOICE(MultichoiceList_LinkContestInfo),
    [MULTI_LINK_CONTEST_MODE]          = MULTICHOICE(MultichoiceList_LinkContestMode),
    [MULTI_FORCED_START_MENU]          = MULTICHOICE(MultichoiceList_ForcedStartMenu),
    [MULTI_FRONTIER_GAMBLER_BET]       = MULTICHOICE(MultichoiceList_FrontierGamblerBet),
    [MULTI_TENT]                       = MULTICHOICE(MultichoiceList_Tent),
    [MULTI_UNUSED_SSTIDAL_1]           = MULTICHOICE(MultichoiceList_UnusedSSTidal1),
    [MULTI_UNUSED_SSTIDAL_2]           = MULTICHOICE(MultichoiceList_UnusedSSTidal2),
    [MULTI_UNUSED_SSTIDAL_3]           = MULTICHOICE(MultichoiceList_UnusedSSTidal3),
    [MULTI_UNUSED_SSTIDAL_4]           = MULTICHOICE(MultichoiceList_UnusedSSTidal4),
    [MULTI_FOSSIL]                     = MULTICHOICE(MultichoiceList_Fossil),
    [MULTI_YESNO]                      = MULTICHOICE(MultichoiceList_YesNo),
    [MULTI_FRONTIER_RULES]             = MULTICHOICE(MultichoiceList_FrontierRules),
    [MULTI_BATTLE_ARENA_RULES]         = MULTICHOICE(MultichoiceList_BattleArenaRules),
    [MULTI_BATTLE_TOWER_RULES]         = MULTICHOICE(MultichoiceList_BattleTowerRules),
    [MULTI_BATTLE_DOME_RULES]          = MULTICHOICE(MultichoiceList_BattleDomeRules),
    [MULTI_BATTLE_FACTORY_RULES]       = MULTICHOICE(MultichoiceList_BattleFactoryRules),
    [MULTI_BATTLE_PALACE_RULES]        = MULTICHOICE(MultichoiceList_BattlePalaceRules),
    [MULTI_BATTLE_PYRAMID_RULES]       = MULTICHOICE(MultichoiceList_BattlePyramidRules),
    [MULTI_BATTLE_PIKE_RULES]          = MULTICHOICE(MultichoiceList_BattlePikeRules),
    [MULTI_GO_ON_RECORD_REST_RETIRE]   = MULTICHOICE(MultichoiceList_GoOnRecordRestRetire),
    [MULTI_GO_ON_REST_RETIRE]          = MULTICHOICE(MultichoiceList_GoOnRestRetire),
    [MULTI_GO_ON_RECORD_RETIRE]        = MULTICHOICE(MultichoiceList_GoOnRecordRetire),
    [MULTI_GO_ON_RETIRE]               = MULTICHOICE(MultichoiceList_GoOnRetire),
    [MULTI_TOURNEY_NO_RECORD]          = MULTICHOICE(MultichoiceList_TourneyNoRecord),
    [MULTI_TV_LATI]                    = MULTICHOICE(MultichoiceList_TVLati),
    [MULTI_BATTLE_TOWER_FEELINGS]      = MULTICHOICE(MultichoiceList_BattleTowerFeelings),
    [MULTI_WHERES_RAYQUAZA]            = MULTICHOICE(MultichoiceList_WheresRayquaza),
    [MULTI_SLATEPORT_TENT_RULES]       = MULTICHOICE(MultichoiceList_SlateportTentRules),
    [MULTI_FALLARBOR_TENT_RULES]       = MULTICHOICE(MultichoiceList_FallarborTentRules),
    [MULTI_TAG_MATCH_TYPE]             = MULTICHOICE(MultichoiceList_TagMatchType),
    [MULTI_BERRY_PLOT]                 = MULTICHOICE(MultichoiceList_BerryPlot),
    [MULTI_BIKE_SHOP]                  = MULTICHOICE(sMultichoiceList_BikeShop),
    [MULTI_EEVEELUTIONS]               = MULTICHOICE(sMultichoiceList_Eeveelutions),
    [MULTI_ISLAND_23]                  = MULTICHOICE(sMultichoiceList_Island23),
    [MULTI_ISLAND_13]                  = MULTICHOICE(sMultichoiceList_Island13),
    [MULTI_ISLAND_12]                  = MULTICHOICE(sMultichoiceList_Island12),
    [MULTI_SEVII_NAVEL]                = MULTICHOICE(sMultichoiceList_SeviiNavel),
    [MULTI_SEVII_BIRTH]                = MULTICHOICE(sMultichoiceList_SeviiBirth),
    [MULTI_SEVII_NAVEL_BIRTH]          = MULTICHOICE(sMultichoiceList_SeviiNavelBirth),
    [MULTI_SEAGALLOP_123]              = MULTICHOICE(sMultichoiceList_Seagallop123),
    [MULTI_SEAGALLOP_V23]              = MULTICHOICE(sMultichoiceList_SeagallopV23),
    [MULTI_SEAGALLOP_V13]              = MULTICHOICE(sMultichoiceList_SeagallopV13),
    [MULTI_SEAGALLOP_V12]              = MULTICHOICE(sMultichoiceList_SeagallopV12),
    [MULTI_SEAGALLOP_VERMILION]        = MULTICHOICE(sMultichoiceList_SeagallopVermilion),
    [MULTI_GAME_CORNER_POKEMON_PRIZES] = MULTICHOICE(sMultichoiceList_GameCornerPokemonPrizes),
    [MULTI_GAME_CORNER_TMPRIZES]           = MULTICHOICE(sMultichoiceList_GameCornerTMPrizes),
    [MULTI_GAME_CORNER_BATTLE_ITEM_PRIZES] = MULTICHOICE(sMultichoiceList_GameCornerBattleItemPrizes),
    [MULTI_DEPT_STORE_ELEVATOR]            = MULTICHOICE(sMultichoiceList_DeptStoreElevator),
    [MULTI_GAME_CORNER_COIN_PURCHASE_COUNTER] = MULTICHOICE(sMultichoiceList_GameCornerCoinPurchaseCounter),
    [MULTI_LINKED_DIRECT_UNION]         = MULTICHOICE(sMultichoiceList_LinkedDirectUnion),
    [MULTI_CELADON_VENDING_MACHINE]           = MULTICHOICE(sMultichoiceList_CeladonVendingMachine),
    [MULTI_THIRSTY_GIRL_FRESH_WATER]                   = MULTICHOICE(sMultichoiceList_ThirstyGirlFreshWater),
    [MULTI_THIRSTY_GIRL_SODA_POP]                      = MULTICHOICE(sMultichoiceList_ThirstyGirlSodaPop),
    [MULTI_THIRSTY_GIRL_FRESH_WATER_SODA_POP]          = MULTICHOICE(sMultichoiceList_ThirstyGirlFreshWaterSodaPop),
    [MULTI_THIRSTY_GIRL_LEMONADE]                      = MULTICHOICE(sMultichoiceList_ThirstyGirlLemonade),
    [MULTI_THIRSTY_GIRL_FRESH_WATER_LEMONADE]          = MULTICHOICE(sMultichoiceList_ThirstyGirlFreshWaterLemonade),
    [MULTI_THIRSTY_GIRL_SODA_POP_LEMONADE]             = MULTICHOICE(sMultichoiceList_ThirstyGirlSodaPopLemonade),
    [MULTI_THIRSTY_GIRL_FRESH_WATER_SODA_POP_LEMONADE] = MULTICHOICE(sMultichoiceList_ThirstyGirlFreshWaterSodaPopLemonade),
    [MULTI_ROCKET_HIDEOUT_ELEVATOR]                    = MULTICHOICE(sMultichoiceList_RocketHideoutElevator),
    [MULTI_HELIX]                                      = MULTICHOICE(sMultichoiceList_Helix),
    [MULTI_DOME]                                       = MULTICHOICE(sMultichoiceList_Dome),
    [MULTI_AMBER]                                      = MULTICHOICE(sMultichoiceList_Amber),
    [MULTI_HELIX_AMBER]                                = MULTICHOICE(sMultichoiceList_HelixAmber),
    [MULTI_DOME_AMBER]                                 = MULTICHOICE(sMultichoiceList_DomeAmber),
    [MULTI_MUSHROOMS]                                  = MULTICHOICE(sMultichoiceList_Mushrooms),
    [MULTI_ROOFTOP_B1F]                                = MULTICHOICE(sMultichoiceList_RooftopB1F),
    [MULTI_TRAINER_TOWER_MODE]                         = MULTICHOICE(sMultichoiceList_TrainerTowerMode),
    [MULTI_TRAINER_CARD_ICON_TINT]                     = MULTICHOICE(sMultichoiceList_TrainerCardIconTint),
    [MULTI_HOF_QUIT]                                   = MULTICHOICE(sMultichoiceList_HOF_Quit),
    [MULTI_EGGS_QUIT]                                  = MULTICHOICE(sMultichoiceList_Eggs_Quit),
    [MULTI_VICTORIES_QUIT]                             = MULTICHOICE(sMultichoiceList_Victories_Quit),
    [MULTI_HOF_EGGS_QUIT]                              = MULTICHOICE(sMultichoiceList_HOF_Eggs_Quit),
    [MULTI_HOF_VICTORIES_QUIT]                         = MULTICHOICE(sMultichoiceList_HOF_Victories_Quit),
    [MULTI_EGGS_VICTORIES_QUIT]                        = MULTICHOICE(sMultichoiceList_Eggs_Victories_Quit),
    [MULTI_HOF_EGGS_VICTORIES_QUIT]                    = MULTICHOICE(sMultichoiceList_HOF_Eggs_Victories_Quit),
    [MULTI_DAYS_OF_WEEK]                 = MULTICHOICE(MultichoiceList_DaysOfWeek),
    [MULTI_KURT_BALLS]                 = MULTICHOICE(MultichoiceList_KurtsBalls),
    [MULTI_PRIZE_MONS]                  = MULTICHOICE(MultichoiceList_PrizeMons),
    [MULTI_7FLOORS]                  = MULTICHOICE(MultichoiceList_7Floors),
    [MULTI_GOLDSILVER]                 = MULTICHOICE(MultichoiceList_GoldSilver),
    [MULTI_OLIVINE_HARBOR]              = MULTICHOICE(MultichoiceList_OlivineHarbor),
    [MULTI_VERMILION_HARBOR]              = MULTICHOICE(MultichoiceList_VermilionHarbor),
    [MULTI_ELDERQUIIZ1]                 = MULTICHOICE(MultichoiceList_ElderQuiz1),
    [MULTI_ELDERQUIIZ2]                 = MULTICHOICE(MultichoiceList_ElderQuiz2),
    [MULTI_ELDERQUIIZ3]                 = MULTICHOICE(MultichoiceList_ElderQuiz3),
    [MULTI_ELDERQUIIZ4]                 = MULTICHOICE(MultichoiceList_ElderQuiz4),
    [MULTI_ELDERQUIIZ5]                 = MULTICHOICE(MultichoiceList_ElderQuiz5),
    [MULTI_HOENN_STARTERS]              = MULTICHOICE(MultichoiceList_HoennStarters),
    [MULTI_5FLOORS]                    = MULTICHOICE(MultichoiceList_5Floors),
    [MULTI_MOM_MENU]                   = MULTICHOICE(MultichoiceList_MomMenu),
    [MULTI_LINK_SERVICES_HNS]          = MULTICHOICE(MultichoiceList_LinkServicesHns),
    [MULTI_BATTLE_MODE_HNS]            = MULTICHOICE(MultichoiceList_BattleModeHns),
    [MULTI_FOSSIL_HNS]                = MULTICHOICE(MultichoiceList_FossilHns),
    [MULTI_GAME_CORNER_DOLLS2]         = MULTICHOICE(MultichoiceList_GameCornerDolls2),
};

const u8 *const gStdStrings[] =
{
    [STDSTRING_COOL] = gText_Cool,
    [STDSTRING_BEAUTY] = gText_Beauty,
    [STDSTRING_CUTE] = gText_Cute,
    [STDSTRING_SMART] = gText_Smart,
    [STDSTRING_TOUGH] = gText_Tough,
    [STDSTRING_NORMAL] = gText_Normal,
    [STDSTRING_SUPER] = gText_Super,
    [STDSTRING_HYPER] = gText_Hyper,
    [STDSTRING_MASTER] = gText_Master,
    [STDSTRING_COOL2] = gText_Cool2,
    [STDSTRING_BEAUTY2] = gText_Beauty2,
    [STDSTRING_CUTE2] = gText_Cute2,
    [STDSTRING_SMART2] = gText_Smart2,
    [STDSTRING_TOUGH2] = gText_Tough2,
    [STDSTRING_ITEMS] = gText_Items,
    [STDSTRING_KEYITEMS] = gText_Key_Items,
    [STDSTRING_POKEBALLS] = gText_Poke_Balls,
    [STDSTRING_TMHMS] = gText_TMs_Hms,
    [STDSTRING_BERRIES] = gText_Berries2,
    [STDSTRING_SINGLE] = gText_Single2,
    [STDSTRING_DOUBLE] = gText_Double2,
    [STDSTRING_MULTI] = gText_Multi,
    [STDSTRING_MULTI_LINK] = gText_MultiLink,
    [STDSTRING_BATTLE_TOWER] = gText_BattleTower2,
    [STDSTRING_BATTLE_DOME] = gText_BattleDome,
    [STDSTRING_BATTLE_FACTORY] = gText_BattleFactory,
    [STDSTRING_BATTLE_PALACE] = gText_BattlePalace,
    [STDSTRING_BATTLE_ARENA] = gText_BattleArena,
    [STDSTRING_BATTLE_PIKE] = gText_BattlePike,
    [STDSTRING_BATTLE_PYRAMID] = gText_BattlePyramid,
    [STDSTRING_BOULDER_BADGE]    = gText_Boulderbadge,
    [STDSTRING_CASCADE_BADGE]    = gText_Cascadebadge,
    [STDSTRING_THUNDER_BADGE]    = gText_Thunderbadge,
    [STDSTRING_RAINBOW_BADGE]    = gText_Rainbowbadge,
    [STDSTRING_SOUL_BADGE]       = gText_Soulbadge,
    [STDSTRING_MARSH_BADGE]      = gText_Marshbadge,
    [STDSTRING_VOLCANO_BADGE]    = gText_Volcanobadge,
    [STDSTRING_EARTH_BADGE]      = gText_Earthbadge,
    [STDSTRING_COINS]            = COMPOUND_STRING("COINS"),
    [STDSTRING_MEDICINE]         = gText_Medicine,
#if I_COMBINE_BAG_POCKETS == FALSE
    [STDSTRING_BATTLE_ITEMS]     = gText_BattleItems,
    [STDSTRING_TREASURES]        = gText_Treasures,
#endif
};

static const u8 sLinkServicesMultichoiceIds[] =
{
    MULTI_CABLE_CLUB_NO_RECORD_MIX,
    MULTI_WIRELESS_NO_RECORD_BERRY,
    MULTI_CABLE_CLUB_WITH_RECORD_MIX,
    MULTI_WIRELESS_NO_BERRY,
    MULTI_WIRELESS_NO_RECORD,
    MULTI_WIRELESS_ALL_SERVICES
};

static const u8 *const sPCNameStrings[] =
{
    gText_SomeonesPC,
    gText_LanettesPC,
    gText_PlayersPC,
    gText_Challenges,
    gText_LogOff,
};

static const u8 *const sLilycoveSSTidalDestinations[SSTIDAL_SELECTION_COUNT] =
{
    [SSTIDAL_SELECTION_SLATEPORT]       = gText_SlateportCity,
    [SSTIDAL_SELECTION_BATTLE_FRONTIER] = gText_BattleFrontier,
    [SSTIDAL_SELECTION_SOUTHERN_ISLAND] = gText_SouthernIsland,
    [SSTIDAL_SELECTION_NAVEL_ROCK]      = gText_NavelRock,
    [SSTIDAL_SELECTION_BIRTH_ISLAND]    = gText_BirthIsland,
    [SSTIDAL_SELECTION_FARAWAY_ISLAND]  = gText_FarawayIsland,
    [SSTIDAL_SELECTION_EXIT]            = gText_Exit,
};

static const u8 *const sCableClubOptions_WithRecordMix[] =
{
    CableClub_Text_TradeUsingLinkCable,
    CableClub_Text_BattleUsingLinkCable,
    CableClub_Text_RecordCornerUsingLinkCable,
    CableClub_Text_CancelSelectedItem,
};
static const u8 *const sWirelessOptionsNoBerryCrush[] =
{
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CanMixRecords,
    CableClub_Text_CancelSelectedItem,
};
static const u8 *const sWirelessOptions_NoRecordMix[] =
{
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CanMakeBerryPowder,
    CableClub_Text_CancelSelectedItem,
};
static const u8 *const sWirelessOptions_AllServices[] =
{
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CanMixRecords,
    CableClub_Text_CanMakeBerryPowder,
    CableClub_Text_CancelSelectedItem,
};
static const u8 *const sCableClubOptions_NoRecordMix[] =
{
    CableClub_Text_TradeUsingLinkCable,
    CableClub_Text_BattleUsingLinkCable,
    CableClub_Text_CancelSelectedItem,
};
static const u8 *const sWirelessOptions_NoRecordMixBerryCrush[] =
{
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CancelSelectedItem,
};


static const u8 *const sSeagallopDestStrings[] = {
    [SEAGALLOP_VERMILION_CITY] = gText_Vermilion,
    [SEAGALLOP_ONE_ISLAND]     = gText_OneIsland,
    [SEAGALLOP_TWO_ISLAND]     = gText_TwoIsland,
    [SEAGALLOP_THREE_ISLAND]   = gText_ThreeIsland,
    [SEAGALLOP_FOUR_ISLAND]    = gText_FourIsland,
    [SEAGALLOP_FIVE_ISLAND]    = gText_FiveIsland,
    [SEAGALLOP_SIX_ISLAND]     = gText_SixIsland,
    [SEAGALLOP_SEVEN_ISLAND]   = gText_SevenIsland,
};
