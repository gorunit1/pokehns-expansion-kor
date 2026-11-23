// Add entries here
// These entries are example entries which you can replace, but they exist to get you started.
// Remember to modify include/constants/help_window.h to include identifiers so they can be used in event scripts.
const struct HelpWindow gHelpWindowInfo[] =
{
    [HELP_DEMO_WINDOW] =
    {
        .header = COMPOUND_STRING("정보: 도움말 창"),
        .desc = COMPOUND_STRING("이것은 도움말 창입니다.\n"
                                "플레이어가 읽지 않을 만큼\n"
                                "많은 글을 화면에 넣을 수 있습니다!\n\n멋지지 않나요!"
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_NORMAL,
        .headerColor = {0, 4, 5},
    },
    [HELP_GAMESTART_WINDOW] =
    {
        .header = COMPOUND_STRING("정보: 다양한 설정"),
        .desc = COMPOUND_STRING("포켓몬센터에서는 불이익 없이\n"
                                "언제든 시간을 돌릴 수 있습니다.\n"
                                "가방의 중요한 물건과 설정 메뉴도\n"
                                "확인해 보세요. 플레이 방식을\n"
                                "더 다양하게 바꿀 수 있습니다.\n"
                                "즐겁게 플레이해 주세요!"
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_NORMAL,
        .headerColor = {0, 8, 3},
    },
    [HELP_TRADE_WINDOW] =
    {
        .header = COMPOUND_STRING("경고: 통신 호환성"),
        .desc = COMPOUND_STRING("Attempting to link incorrectly may result\n"
                                "in permanent damage to your save file.\n"
                                "Only link with another player if:\n"
                                "You are both playing Heart & Soul.\n"
                                "You are both on the same version.\n"
                                "You both have the same challenge settings.\n"
                                "You are NOT using any randomizer settings."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 4, 5},
    },
    [HELP_TELEPORTER_WINDOW] =
    {
        .header = COMPOUND_STRING("추가 콘텐츠: 텔레포터"),
        .desc = COMPOUND_STRING("텔레포터를 사용하면 포켓몬을\n"
                                "가라르의 모습으로 영구적으로\n"
                                "바꿀 수 있습니다.\n"
                                "가라르의 모습은 스토리 진행이나\n"
                                "전국도감 완성에 필요하지 않습니다."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_NORMAL,
        .headerColor = {0, 8, 3},
    },
    [HELP_SINJOH_WINDOW] =
    {
        .header = COMPOUND_STRING("추가 콘텐츠: 신도유적"),
        .desc = COMPOUND_STRING("이 사람을 통해 보너스 콘텐츠인\n"
                                "신도유적에 갈 수 있습니다.\n"
                                "스토리 진행이나 전국도감 완성에는\n"
                                "필요하지 않습니다.\n"
                                "\n"
                                "원할 때만 즐겨 주세요."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_NORMAL,
        .headerColor = {0, 8, 3},
    },
    [HELP_ALOLA_WINDOW] =
    {
        .header = COMPOUND_STRING("추가 콘텐츠: 알로라 섬"),
        .desc = COMPOUND_STRING("이 사람을 통해 보너스 콘텐츠인\n"
                                "알로라 섬에 갈 수 있습니다.\n"
                                "스토리 진행이나 전국도감 완성에는\n"
                                "필요하지 않습니다.\n"
                                "\n"
                                "원할 때만 즐겨 주세요."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_NORMAL,
        .headerColor = {0, 8, 3},
    },
    [HELP_DAYCARE1_WINDOW] =
    {
        .header = COMPOUND_STRING("아기 포켓몬의 특성"),
        .desc = COMPOUND_STRING("피츄의 특성은 정전기.\n"
                                "삐의 특성은 헤롱헤롱바디.\n"
                                "푸푸린의 특성은 헤롱헤롱바디.\n"
                                "배루키의 특성은 근성.\n"
                                "뽀뽀라의 특성은 둔감.\n"
                                "에레키드의 특성은 정전기.\n"
                                "마그비의 특성은 불꽃몸."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 8, 3},
    },
    [HELP_DAYCARE2_WINDOW] =
    {
        .header = COMPOUND_STRING("아기 포켓몬의 타입"),
        .desc = COMPOUND_STRING("피츄는 전기타입.\n"
                                "삐는 노말타입.\n"
                                "푸푸린은 노말타입.\n"
                                "배루키는 격투타입.\n"
                                "뽀뽀라는 얼음타입.\n"
                                "에레키드는 전기타입.\n"
                                "마그비는 불꽃타입."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 8, 3},
    },
    [HELP_DAYCARE3_WINDOW] =
    {
        .header = COMPOUND_STRING("아기 포켓몬의 울음소리"),
        .desc = COMPOUND_STRING("피츄는 아-앙 하고 웁니다.\n"
                                "삐는 꺄- 하고 웁니다.\n"
                                "푸푸린은 랄라라 하고 웁니다.\n"
                                "배루키는 이야 하고 웁니다.\n"
                                "뽀뽀라는 에헤헤 하고 웁니다.\n"
                                "에레키드는 어이 하고 웁니다.\n"
                                "마그비는 쳇 하고 웁니다."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 8, 3},
    },
    [HELP_DAYCARE4_WINDOW] =
    {
        .header = COMPOUND_STRING("아기 포켓몬의 행동"),
        .desc = COMPOUND_STRING("피츄는 하루 종일 논다.\n"
                                "삐는 달을 바라본다.\n"
                                "푸푸린은 상대를 재운다.\n"
                                "배루키는 끊임없이 단련한다.\n"
                                "뽀뽀라는 뽐내기를 좋아한다.\n"
                                "에레키드는 전기를 모은다.\n"
                                "마그비는 자주 화를 낸다."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 8, 3},
    },
    [HELP_POKEBLOCK_WINDOW] =
    {
        .header = COMPOUND_STRING("먹이통: 일반 포켓몬스낵"),
        .desc = COMPOUND_STRING("POKéBLOCK colors attract perfect IVs:\n"
                                "RED: HP,ATK,SPE   BLU: HP,SPA,SPE\n"
                                "PNK: ATK,SPA,SPE  GRN: HP,DEF,SPD\n"
                                "YEL: HP,ATK,DEF   PUR: ATK,DEF,SPD\n"
                                "IND: HP,SPA,SPD   BRN: DEF,SPE,SPD\n"
                                "LBL: SPA,SPE,SPD  OLV: ATK,DEF,SPE\n"
                                "GRY: HP,ATK,SPA\n"
                                "Every POKéBLOCK attracts HIDDEN ABILITY."

                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 8, 3},
    },
    [HELP_GOLD_POKEBLOCK_WINDOW] =
    {
        .header = COMPOUND_STRING("먹이통: 금색 포켓몬스낵"),
        .desc = COMPOUND_STRING("금색 포켓몬스낵은 최고 개체값이\n"
                                "5개인 포켓몬을 끌어들입니다.\n"
                                "맛에 따라 최고가 아닌 능력치가 정해집니다.\n"
                                "매운맛:특공 제외  떫은맛:공격 제외\n"
                                "단맛:특방 제외    쓴맛:스피드 제외\n"
                                "신맛:HP 제외\n"
                                "모두 숨겨진 특성을 끌어들입니다."
                            ),
        .headerFont = FONT_NORMAL,
        .descFont = FONT_SMALL,
        .headerColor = {0, 8, 3},
    },
    // Add more entries
};
