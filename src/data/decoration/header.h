const struct Decoration gDecorations[] =
{
    [DECOR_NONE] =
    {
        .id = DECOR_NONE,
        .name = _("작은 책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DESK,
        .price = 0,
        .description = COMPOUND_STRING("1인용 사이즈의 작은 책상\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_SMALL_DESK,
        .icon = {gItemIcon_QuestionMark, gItemIconPalette_QuestionMark},
    },

    [DECOR_SMALL_DESK] =
    {
        .id = DECOR_SMALL_DESK,
        .name = _("작은 책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DESK,
        .price = 3000,
        .description = COMPOUND_STRING("1인용 사이즈의 작은 책상\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_SMALL_DESK,
        .icon = {NULL, NULL},
    },

    [DECOR_POKEMON_DESK] =
    {
        .id = DECOR_POKEMON_DESK,
        .name = _("포켓몬책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DESK,
        .price = 3000,
        .description = COMPOUND_STRING("몬스터볼 형태의 책상\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_POKEMON_DESK,
        .icon = {NULL, NULL},
    },

    [DECOR_HEAVY_DESK] =
    {
        .id = DECOR_HEAVY_DESK,
        .name = _("묵직한 책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x2,
        .category = DECORCAT_DESK,
        .price = 6000,
        .description = COMPOUND_STRING("철로 만들어진 큰 책상\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_HEAVY_DESK,
        .icon = {gDecorIcon_HeavyDesk, gDecorIconPalette_HeavyDesk},
    },

    [DECOR_RAGGED_DESK] =
    {
        .id = DECOR_RAGGED_DESK,
        .name = _("울퉁불퉁책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x2,
        .category = DECORCAT_DESK,
        .price = 6000,
        .description = COMPOUND_STRING("나무로 만들어진 큰 책상\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_RAGGED_DESK,
        .icon = {gDecorIcon_RaggedDesk, gDecorIconPalette_RaggedDesk},
    },

    [DECOR_COMFORT_DESK] =
    {
        .id = DECOR_COMFORT_DESK,
        .name = _("푹신푹신책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x2,
        .category = DECORCAT_DESK,
        .price = 6000,
        .description = COMPOUND_STRING("나뭇잎으로 만들어진\n큰 책상 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_COMFORT_DESK,
        .icon = {gDecorIcon_ComfortDesk, gDecorIconPalette_ComfortDesk},
    },

    [DECOR_PRETTY_DESK] =
    {
        .id = DECOR_PRETTY_DESK,
        .name = _("고운 책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = COMPOUND_STRING("유리로 만들어진\n거대한 책상 위에\n많은 물건을 놓을 수 있다."),
        .tiles = DecorGfx_PRETTY_DESK,
        .icon = {gDecorIcon_PrettyDesk, gDecorIconPalette_PrettyDesk},
    },

    [DECOR_BRICK_DESK] =
    {
        .id = DECOR_BRICK_DESK,
        .name = _("벽돌책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = COMPOUND_STRING("벽돌로 만들어진\n거대한 책상 위에\n많은 물건을 놓을 수 있다."),
        .tiles = DecorGfx_BRICK_DESK,
        .icon = {gDecorIcon_BrickDesk, gDecorIconPalette_BrickDesk},
    },

    [DECOR_CAMP_DESK] =
    {
        .id = DECOR_CAMP_DESK,
        .name = _("캠프책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = COMPOUND_STRING("통나무로 만들어진\n거대한 책상 위에\n많은 물건을 놓을 수 있다."),
        .tiles = DecorGfx_CAMP_DESK,
        .icon = {gDecorIcon_CampDesk, gDecorIconPalette_CampDesk},
    },

    [DECOR_HARD_DESK] =
    {
        .id = DECOR_HARD_DESK,
        .name = _("딱딱한 책상"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = COMPOUND_STRING("돌로 만들어진\n거대한 책상 위에\n많은 물건을 놓을 수 있다."),
        .tiles = DecorGfx_HARD_DESK,
        .icon = {gDecorIcon_HardDesk, gDecorIconPalette_HardDesk},
    },

    [DECOR_SMALL_CHAIR] =
    {
        .id = DECOR_SMALL_CHAIR,
        .name = _("작은 의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("혼자 앉는 작은 의자"),
        .tiles = DecorGfx_SMALL_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_POKEMON_CHAIR] =
    {
        .id = DECOR_POKEMON_CHAIR,
        .name = _("포켓몬의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("몬스터볼 형태의\n작은 의자"),
        .tiles = DecorGfx_POKEMON_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_HEAVY_CHAIR] =
    {
        .id = DECOR_HEAVY_CHAIR,
        .name = _("묵직한 의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("철로 만들어진 작은 의자"),
        .tiles = DecorGfx_HEAVY_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_PRETTY_CHAIR] =
    {
        .id = DECOR_PRETTY_CHAIR,
        .name = _("고운 의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("유리로 만들어진 작은 의자"),
        .tiles = DecorGfx_PRETTY_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_COMFORT_CHAIR] =
    {
        .id = DECOR_COMFORT_CHAIR,
        .name = _("푹신푹신의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("나뭇잎으로 만들어진\n작은 의자"),
        .tiles = DecorGfx_COMFORT_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_RAGGED_CHAIR] =
    {
        .id = DECOR_RAGGED_CHAIR,
        .name = _("울퉁불퉁의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("나무로 만들어진 작은 의자"),
        .tiles = DecorGfx_RAGGED_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_BRICK_CHAIR] =
    {
        .id = DECOR_BRICK_CHAIR,
        .name = _("벽돌의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("벽돌로 만들어진 작은 의자"),
        .tiles = DecorGfx_BRICK_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_CAMP_CHAIR] =
    {
        .id = DECOR_CAMP_CHAIR,
        .name = _("캠프의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("통나무로 만들어진\n작은 의자"),
        .tiles = DecorGfx_CAMP_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_HARD_CHAIR] =
    {
        .id = DECOR_HARD_CHAIR,
        .name = _("딱딱한 의자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = COMPOUND_STRING("돌로 만들어진 작은 의자"),
        .tiles = DecorGfx_HARD_CHAIR,
        .icon = {NULL, NULL},
    },

    [DECOR_RED_PLANT] =
    {
        .id = DECOR_RED_PLANT,
        .name = _("새빨간 화분"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 3000,
        .description = COMPOUND_STRING("새빨간 꽃이 피어 있는 화분"),
        .tiles = DecorGfx_RED_PLANT,
        .icon = {gDecorIcon_RedPlant, gDecorIconPalette_RedPlant},
    },

    [DECOR_TROPICAL_PLANT] =
    {
        .id = DECOR_TROPICAL_PLANT,
        .name = _("남국의 화분"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 3000,
        .description = COMPOUND_STRING("남국의 식물이\n심어져 있는 화분"),
        .tiles = DecorGfx_TROPICAL_PLANT,
        .icon = {gDecorIcon_TropicalPlant, gDecorIconPalette_TropicalPlant},
    },

    [DECOR_PRETTY_FLOWERS] =
    {
        .id = DECOR_PRETTY_FLOWERS,
        .name = _("귀여운 꽃"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 3000,
        .description = COMPOUND_STRING("귀여운 꽃이 피어 있는 화분"),
        .tiles = DecorGfx_PRETTY_FLOWERS,
        .icon = {gDecorIcon_PrettyFlowers, gDecorIconPalette_PrettyFlowers},
    },

    [DECOR_COLORFUL_PLANT] =
    {
        .id = DECOR_COLORFUL_PLANT,
        .name = _("컬러풀 화분"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_PLANT,
        .price = 5000,
        .description = COMPOUND_STRING("여러 가지 꽃이 피어 있는\n큰 화분"),
        .tiles = DecorGfx_COLORFUL_PLANT,
        .icon = {gDecorIcon_ColorfulPlant, gDecorIconPalette_ColorfulPlant},
    },

    [DECOR_BIG_PLANT] =
    {
        .id = DECOR_BIG_PLANT,
        .name = _("큰 화분"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_PLANT,
        .price = 5000,
        .description = COMPOUND_STRING("우산 같은 형태를 한\n큰 화분"),
        .tiles = DecorGfx_BIG_PLANT,
        .icon = {gDecorIcon_BigPlant, gDecorIconPalette_BigPlant},
    },

    [DECOR_GORGEOUS_PLANT] =
    {
        .id = DECOR_GORGEOUS_PLANT,
        .name = _("훌륭한 화분"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_PLANT,
        .price = 5000,
        .description = COMPOUND_STRING("아주 잘 손질된\n큰 화분"),
        .tiles = DecorGfx_GORGEOUS_PLANT,
        .icon = {gDecorIcon_GorgeousPlant, gDecorIconPalette_GorgeousPlant},
    },

    [DECOR_RED_BRICK] =
    {
        .id = DECOR_RED_BRICK,
        .name = _("빨강벽돌"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("빨간색 벽돌\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_RED_BRICK,
        .icon = {gDecorIcon_RedBrick, gDecorIconPalette_RedBrick},
    },

    [DECOR_YELLOW_BRICK] =
    {
        .id = DECOR_YELLOW_BRICK,
        .name = _("노랑벽돌"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("노란색 벽돌\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_YELLOW_BRICK,
        .icon = {gDecorIcon_YellowBrick, gDecorIconPalette_YellowBrick},
    },

    [DECOR_BLUE_BRICK] =
    {
        .id = DECOR_BLUE_BRICK,
        .name = _("파랑벽돌"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("파란색 벽돌\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_BLUE_BRICK,
        .icon = {gDecorIcon_BlueBrick, gDecorIconPalette_BlueBrick},
    },

    [DECOR_RED_BALLOON] =
    {
        .id = DECOR_RED_BALLOON,
        .name = _("빨강풍선"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("물이 들어 있는 빨간 풍선\n밟으면 터진다."),
        .tiles = DecorGfx_RED_BALLOON,
        .icon = {NULL, NULL},
    },

    [DECOR_BLUE_BALLOON] =
    {
        .id = DECOR_BLUE_BALLOON,
        .name = _("파랑풍선"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("물이 들어 있는 파란 풍선\n밟으면 터진다."),
        .tiles = DecorGfx_BLUE_BALLOON,
        .icon = {NULL, NULL},
    },

    [DECOR_YELLOW_BALLOON] =
    {
        .id = DECOR_YELLOW_BALLOON,
        .name = _("노랑풍선"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("물이 들어 있는 노란 풍선\n밟으면 터진다."),
        .tiles = DecorGfx_YELLOW_BALLOON,
        .icon = {NULL, NULL},
    },

    [DECOR_RED_TENT] =
    {
        .id = DECOR_RED_TENT,
        .name = _("빨강텐트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_ORNAMENT,
        .price = 10000,
        .description = COMPOUND_STRING("크고 빨간 텐트\n안에 들어갈 수 있다."),
        .tiles = DecorGfx_RED_TENT,
        .icon = {gDecorIcon_RedTent, gDecorIconPalette_RedTent},
    },

    [DECOR_BLUE_TENT] =
    {
        .id = DECOR_BLUE_TENT,
        .name = _("파랑텐트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_ORNAMENT,
        .price = 10000,
        .description = COMPOUND_STRING("크고 파란 텐트\n안에 들어갈 수 있다."),
        .tiles = DecorGfx_BLUE_TENT,
        .icon = {gDecorIcon_BlueTent, gDecorIconPalette_BlueTent},
    },

    [DECOR_SOLID_BOARD] =
    {
        .id = DECOR_SOLID_BOARD,
        .name = _("단단한 판자"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = COMPOUND_STRING("구멍 위에 놓으면\n다리처럼 건널 수 있다."),
        .tiles = DecorGfx_SOLID_BOARD,
        .icon = {gDecorIcon_SolidBoard, gDecorIconPalette_SolidBoard},
    },

    [DECOR_SLIDE] =
    {
        .id = DECOR_SLIDE,
        .name = _("미끄럼대"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_2x4,
        .category = DECORCAT_ORNAMENT,
        .price = 8000,
        .description = COMPOUND_STRING("미끄럼대 위에서 미끄러지며\n내려올 수 있다."),
        .tiles = DecorGfx_SLIDE,
        .icon = {gDecorIcon_Slide, gDecorIconPalette_Slide},
    },

    [DECOR_FENCE_LENGTH] =
    {
        .id = DECOR_FENCE_LENGTH,
        .name = _("세로울타리"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("작은 울타리\n지나갈 수 없다."),
        .tiles = DecorGfx_FENCE_LENGTH,
        .icon = {NULL, NULL},
    },

    [DECOR_FENCE_WIDTH] =
    {
        .id = DECOR_FENCE_WIDTH,
        .name = _("가로울타리"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = COMPOUND_STRING("작은 울타리\n지나갈 수 없다."),
        .tiles = DecorGfx_FENCE_WIDTH,
        .icon = {NULL, NULL},
    },

    [DECOR_TIRE] =
    {
        .id = DECOR_TIRE,
        .name = _("타이어"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_ORNAMENT,
        .price = 800,
        .description = COMPOUND_STRING("낡고 큰 타이어\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_TIRE,
        .icon = {gDecorIcon_Tire, gDecorIconPalette_Tire},
    },

    [DECOR_STAND] =
    {
        .id = DECOR_STAND,
        .name = _("큰 받침대"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_4x2,
        .category = DECORCAT_ORNAMENT,
        .price = 7000,
        .description = COMPOUND_STRING("계단 위를 걸어\n미끄럼대에 올라간다."),
        .tiles = DecorGfx_STAND,
        .icon = {gDecorIcon_Stand, gDecorIconPalette_Stand},
    },

    [DECOR_MUD_BALL] =
    {
        .id = DECOR_MUD_BALL,
        .name = _("진흙경단"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 200,
        .description = COMPOUND_STRING("둥근 진흙경단\n밟으면 터진다."),
        .tiles = DecorGfx_MUD_BALL,
        .icon = {NULL, NULL},
    },

    [DECOR_BREAKABLE_DOOR] =
    {
        .id = DECOR_BREAKABLE_DOOR,
        .name = _("부서지는 도어"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = COMPOUND_STRING("빠져나갈 수 있는\n이상한 도어"),
        .tiles = DecorGfx_BREAKABLE_DOOR,
        .icon = {gDecorIcon_BreakableDoor, gDecorIconPalette_BreakableDoor},
    },

    [DECOR_SAND_ORNAMENT] =
    {
        .id = DECOR_SAND_ORNAMENT,
        .name = _("모래장식품"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = COMPOUND_STRING("만지면 무너지는\n모래 장식품"),
        .tiles = DecorGfx_SAND_ORNAMENT,
        .icon = {gDecorIcon_SandOrnament, gDecorIconPalette_SandOrnament},
    },

    [DECOR_SILVER_SHIELD] =
    {
        .id = DECOR_SILVER_SHIELD,
        .name = _("은상패"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 0,
        .description = COMPOUND_STRING("배틀타워에서\n50명 연승한 기념의 상패"),
        .tiles = DecorGfx_SILVER_SHIELD,
        .icon = {NULL, NULL},
    },

    [DECOR_GOLD_SHIELD] =
    {
        .id = DECOR_GOLD_SHIELD,
        .name = _("금상패"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 0,
        .description = COMPOUND_STRING("배틀타워에서\n100명 연승한 기념의 상패"),
        .tiles = DecorGfx_GOLD_SHIELD,
        .icon = {NULL, NULL},
    },

    [DECOR_GLASS_ORNAMENT] =
    {
        .id = DECOR_GLASS_ORNAMENT,
        .name = _("글라스오브제"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 0,
        .description = COMPOUND_STRING("미술관에 있는 유명한\n유리 조각의 레플리카"),
        .tiles = DecorGfx_GLASS_ORNAMENT,
        .icon = {gDecorIcon_GlassOrnament, gDecorIconPalette_GlassOrnament},
    },

    [DECOR_TV] =
    {
        .id = DECOR_TV,
        .name = _("TV"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = COMPOUND_STRING("진짜와 꼭 닮게 만들어진\n장난감 TV"),
        .tiles = DecorGfx_TV,
        .icon = {NULL, NULL},
    },

    [DECOR_ROUND_TV] =
    {
        .id = DECOR_ROUND_TV,
        .name = _("둥근 TV"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 4000,
        .description = COMPOUND_STRING("도토링을 모델로 하여\n만들어진 장난감 TV"),
        .tiles = DecorGfx_ROUND_TV,
        .icon = {NULL, NULL},
    },

    [DECOR_CUTE_TV] =
    {
        .id = DECOR_CUTE_TV,
        .name = _("귀여운 TV"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 4000,
        .description = COMPOUND_STRING("에나비를 모델로 하여\n만들어진 장난감 TV"),
        .tiles = DecorGfx_CUTE_TV,
        .icon = {NULL, NULL},
    },

    [DECOR_GLITTER_MAT] =
    {
        .id = DECOR_GLITTER_MAT,
        .name = _("반짝반짝매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 2000,
        .description = COMPOUND_STRING("밟으면 반짝이는\n이상한 매트"),
        .tiles = DecorGfx_GLITTER_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_JUMP_MAT] =
    {
        .id = DECOR_JUMP_MAT,
        .name = _("점프매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 2000,
        .description = COMPOUND_STRING("밟으면 점프하는\n짓궂은 매트"),
        .tiles = DecorGfx_JUMP_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_SPIN_MAT] =
    {
        .id = DECOR_SPIN_MAT,
        .name = _("뱅그르매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 2000,
        .description = COMPOUND_STRING("밟으면 그 자리에서\n뱅그르 돌며 내쫓기는\n짓궂은 매트"),
        .tiles = DecorGfx_SPIN_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_C_LOW_NOTE_MAT] =
    {
        .id = DECOR_C_LOW_NOTE_MAT,
        .name = _("음표매트CL"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 도에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_C_LOW_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_D_NOTE_MAT] =
    {
        .id = DECOR_D_NOTE_MAT,
        .name = _("음표매트D"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 레에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_D_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_E_NOTE_MAT] =
    {
        .id = DECOR_E_NOTE_MAT,
        .name = _("음표매트E"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 미에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_E_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_F_NOTE_MAT] =
    {
        .id = DECOR_F_NOTE_MAT,
        .name = _("음표매트F"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 파에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_F_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_G_NOTE_MAT] =
    {
        .id = DECOR_G_NOTE_MAT,
        .name = _("음표매트G"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 솔에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_G_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_A_NOTE_MAT] =
    {
        .id = DECOR_A_NOTE_MAT,
        .name = _("음표매트A"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 라에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_A_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_B_NOTE_MAT] =
    {
        .id = DECOR_B_NOTE_MAT,
        .name = _("음표매트B"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 시에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_B_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_C_HIGH_NOTE_MAT] =
    {
        .id = DECOR_C_HIGH_NOTE_MAT,
        .name = _("음표매트CH"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = COMPOUND_STRING("밟으면 높은 도에 해당하는\n소리가 나는 매트 방향을\n바꾸면 음정이 변한다."),
        .tiles = DecorGfx_C_HIGH_NOTE_MAT,
        .icon = {NULL, NULL},
    },

    [DECOR_SURF_MAT] =
    {
        .id = DECOR_SURF_MAT,
        .name = _("파도타기매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("파도타기를 표현한\n둥근 매트 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_SURF_MAT,
        .icon = {gDecorIcon_SurfMat, gDecorIconPalette_SurfMat},
    },

    [DECOR_THUNDER_MAT] =
    {
        .id = DECOR_THUNDER_MAT,
        .name = _("번개매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("번개를 표현한 둥근 매트\n위에 물건을 놓을 수 있다."),
        .tiles = DecorGfx_THUNDER_MAT,
        .icon = {gDecorIcon_ThunderMat, gDecorIconPalette_ThunderMat},
    },

    [DECOR_FIRE_BLAST_MAT] =
    {
        .id = DECOR_FIRE_BLAST_MAT,
        .name = _("불대문자매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("불대문자를 표현한\n둥근 매트 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_FIRE_BLAST_MAT,
        .icon = {gDecorIcon_FireBlastMat, gDecorIconPalette_FireBlastMat},
    },

    [DECOR_POWDER_SNOW_MAT] =
    {
        .id = DECOR_POWDER_SNOW_MAT,
        .name = _("눈싸라기매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("눈싸라기를 표현한\n둥근 매트 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_POWDER_SNOW_MAT,
        .icon = {gDecorIcon_PowderSnowMat, gDecorIconPalette_PowderSnowMat},
    },

    [DECOR_ATTRACT_MAT] =
    {
        .id = DECOR_ATTRACT_MAT,
        .name = _("헤롱헤롱매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("헤롱헤롱을 표현한\n둥근 매트 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_ATTRACT_MAT,
        .icon = {gDecorIcon_AttractMat, gDecorIconPalette_AttractMat},
    },

    [DECOR_FISSURE_MAT] =
    {
        .id = DECOR_FISSURE_MAT,
        .name = _("땅가르기매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("땅가르기를 표현한\n둥근 매트 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_FISSURE_MAT,
        .icon = {gDecorIcon_FissureMat, gDecorIconPalette_FissureMat},
    },

    [DECOR_SPIKES_MAT] =
    {
        .id = DECOR_SPIKES_MAT,
        .name = _("압정뿌리기매트"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = COMPOUND_STRING("압정뿌리기를 표현한\n둥근 매트 위에\n물건을 놓을 수 있다."),
        .tiles = DecorGfx_SPIKES_MAT,
        .icon = {gDecorIcon_SpikesMat, gDecorIconPalette_SpikesMat},
    },

    [DECOR_BALL_POSTER] =
    {
        .id = DECOR_BALL_POSTER,
        .name = _("볼포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = COMPOUND_STRING("몬스터볼이 프린트되어 있는\n작은 포스터"),
        .tiles = DecorGfx_BALL_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_GREEN_POSTER] =
    {
        .id = DECOR_GREEN_POSTER,
        .name = _("초록포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = COMPOUND_STRING("나무지기가 프린트되어 있는\n작은 포스터"),
        .tiles = DecorGfx_GREEN_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_RED_POSTER] =
    {
        .id = DECOR_RED_POSTER,
        .name = _("빨강포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = COMPOUND_STRING("아차모가 프린트되어 있는\n작은 포스터"),
        .tiles = DecorGfx_RED_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_BLUE_POSTER] =
    {
        .id = DECOR_BLUE_POSTER,
        .name = _("옥색포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = COMPOUND_STRING("물짱이가 프린트되어 있는\n작은 포스터"),
        .tiles = DecorGfx_BLUE_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_CUTE_POSTER] =
    {
        .id = DECOR_CUTE_POSTER,
        .name = _("귀여운 포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = COMPOUND_STRING("루리리가 프린트되어 있는\n작은 포스터"),
        .tiles = DecorGfx_CUTE_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_PIKA_POSTER] =
    {
        .id = DECOR_PIKA_POSTER,
        .name = _("번쩍번쩍포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = COMPOUND_STRING("피카츄와 피츄가\n프린트되어 있는 큰 포스터"),
        .tiles = DecorGfx_PIKA_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_LONG_POSTER] =
    {
        .id = DECOR_LONG_POSTER,
        .name = _("긴 포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = COMPOUND_STRING("세비퍼가 프린트되어 있는\n큰 포스터"),
        .tiles = DecorGfx_LONG_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_SEA_POSTER] =
    {
        .id = DECOR_SEA_POSTER,
        .name = _("바다포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = COMPOUND_STRING("시라칸이 프린트되어 있는\n큰 포스터"),
        .tiles = DecorGfx_SEA_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_SKY_POSTER] =
    {
        .id = DECOR_SKY_POSTER,
        .name = _("하늘포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = COMPOUND_STRING("갈모매가 프린트되어 있는\n큰 포스터"),
        .tiles = DecorGfx_SKY_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_KISS_POSTER] =
    {
        .id = DECOR_KISS_POSTER,
        .name = _("키스포스터"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = COMPOUND_STRING("뽀뽀라가 프린트되어 있는\n큰 포스터"),
        .tiles = DecorGfx_KISS_POSTER,
        .icon = {NULL, NULL},
    },

    [DECOR_PICHU_DOLL] =
    {
        .id = DECOR_PICHU_DOLL,
        .name = _("피츄인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("피츄의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_PICHU_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_PIKACHU_DOLL] =
    {
        .id = DECOR_PIKACHU_DOLL,
        .name = _("피카츄인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("피카츄의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_PIKACHU_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_MARILL_DOLL] =
    {
        .id = DECOR_MARILL_DOLL,
        .name = _("마릴인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("마릴의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_MARILL_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_TOGEPI_DOLL] =
    {
        .id = DECOR_TOGEPI_DOLL,
        .name = _("토게피인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("토게피의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_TOGEPI_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_CYNDAQUIL_DOLL] =
    {
        .id = DECOR_CYNDAQUIL_DOLL,
        .name = _("브케인인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("브케인의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_CYNDAQUIL_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_CHIKORITA_DOLL] =
    {
        .id = DECOR_CHIKORITA_DOLL,
        .name = _("치코리타인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("치코리타의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_CHIKORITA_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_TOTODILE_DOLL] =
    {
        .id = DECOR_TOTODILE_DOLL,
        .name = _("리아코인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("리아코의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_TOTODILE_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_JIGGLYPUFF_DOLL] =
    {
        .id = DECOR_JIGGLYPUFF_DOLL,
        .name = _("푸린인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("푸린의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_JIGGLYPUFF_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_MEOWTH_DOLL] =
    {
        .id = DECOR_MEOWTH_DOLL,
        .name = _("나옹인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("나옹의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_MEOWTH_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_CLEFAIRY_DOLL] =
    {
        .id = DECOR_CLEFAIRY_DOLL,
        .name = _("삐삐인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("삐삐의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_CLEFAIRY_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_DITTO_DOLL] =
    {
        .id = DECOR_DITTO_DOLL,
        .name = _("메타몽인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("메타몽의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_DITTO_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_SMOOCHUM_DOLL] =
    {
        .id = DECOR_SMOOCHUM_DOLL,
        .name = _("뽀뽀라인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("뽀뽀라의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_SMOOCHUM_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_TREECKO_DOLL] =
    {
        .id = DECOR_TREECKO_DOLL,
        .name = _("나무지기인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("나무지기의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_TREECKO_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_TORCHIC_DOLL] =
    {
        .id = DECOR_TORCHIC_DOLL,
        .name = _("아차모인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("아차모의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_TORCHIC_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_MUDKIP_DOLL] =
    {
        .id = DECOR_MUDKIP_DOLL,
        .name = _("물짱이인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("물짱이의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_MUDKIP_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_DUSKULL_DOLL] =
    {
        .id = DECOR_DUSKULL_DOLL,
        .name = _("해골몽인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("해골몽의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_DUSKULL_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_WYNAUT_DOLL] =
    {
        .id = DECOR_WYNAUT_DOLL,
        .name = _("마자인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("마자의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_WYNAUT_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_BALTOY_DOLL] =
    {
        .id = DECOR_BALTOY_DOLL,
        .name = _("오뚝군인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("오뚝군의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_BALTOY_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_KECLEON_DOLL] =
    {
        .id = DECOR_KECLEON_DOLL,
        .name = _("켈리몬인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("켈리몬의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_KECLEON_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_AZURILL_DOLL] =
    {
        .id = DECOR_AZURILL_DOLL,
        .name = _("루리리인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("루리리의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_AZURILL_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_SKITTY_DOLL] =
    {
        .id = DECOR_SKITTY_DOLL,
        .name = _("에나비인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("에나비의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_SKITTY_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_SWABLU_DOLL] =
    {
        .id = DECOR_SWABLU_DOLL,
        .name = _("파비코인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("파비코의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_SWABLU_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_GULPIN_DOLL] =
    {
        .id = DECOR_GULPIN_DOLL,
        .name = _("꼴깍몬인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("꼴깍몬의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_GULPIN_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_LOTAD_DOLL] =
    {
        .id = DECOR_LOTAD_DOLL,
        .name = _("연꽃몬인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("연꽃몬의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_LOTAD_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_SEEDOT_DOLL] =
    {
        .id = DECOR_SEEDOT_DOLL,
        .name = _("도토링인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = COMPOUND_STRING("도토링의 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_SEEDOT_DOLL,
        .icon = {NULL, NULL},
    },

    [DECOR_PIKA_CUSHION] =
    {
        .id = DECOR_PIKA_CUSHION,
        .name = _("번쩍쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("피카츄 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_PIKA_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_ROUND_CUSHION] =
    {
        .id = DECOR_ROUND_CUSHION,
        .name = _("둥근 쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("마릴리 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_ROUND_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_KISS_CUSHION] =
    {
        .id = DECOR_KISS_CUSHION,
        .name = _("키스쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("뽀뽀라 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_KISS_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_ZIGZAG_CUSHION] =
    {
        .id = DECOR_ZIGZAG_CUSHION,
        .name = _("지그재그쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("지그제구리 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_ZIGZAG_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_SPIN_CUSHION] =
    {
        .id = DECOR_SPIN_CUSHION,
        .name = _("빙그르쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("얼루기 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_SPIN_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_DIAMOND_CUSHION] =
    {
        .id = DECOR_DIAMOND_CUSHION,
        .name = _("다이아쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("깜까미 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_DIAMOND_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_BALL_CUSHION] =
    {
        .id = DECOR_BALL_CUSHION,
        .name = _("볼쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("몬스터볼 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_BALL_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_GRASS_CUSHION] =
    {
        .id = DECOR_GRASS_CUSHION,
        .name = _("풀쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("풀마크 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_GRASS_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_FIRE_CUSHION] =
    {
        .id = DECOR_FIRE_CUSHION,
        .name = _("불꽃쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("불꽃마크 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_FIRE_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_WATER_CUSHION] =
    {
        .id = DECOR_WATER_CUSHION,
        .name = _("물쿠션"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = COMPOUND_STRING("물마크 쿠션\n매트 위에도 놓을 수 있다."),
        .tiles = DecorGfx_WATER_CUSHION,
        .icon = {NULL, NULL},
    },

    [DECOR_SNORLAX_DOLL] =
    {
        .id = DECOR_SNORLAX_DOLL,
        .name = _("잠만보인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("잠만보의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_SNORLAX_DOLL,
        .icon = {gDecorIcon_SnorlaxDoll, gDecorIconPalette_SnorlaxDoll},
    },

    [DECOR_RHYDON_DOLL] =
    {
        .id = DECOR_RHYDON_DOLL,
        .name = _("코뿔리인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("코뿌리의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_RHYDON_DOLL,
        .icon = {gDecorIcon_RhydonDoll, gDecorIconPalette_RhydonDoll},
    },

    [DECOR_LAPRAS_DOLL] =
    {
        .id = DECOR_LAPRAS_DOLL,
        .name = _("라프라스인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("라프라스의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_LAPRAS_DOLL,
        .icon = {gDecorIcon_LaprasDoll, gDecorIconPalette_LaprasDoll},
    },

    [DECOR_VENUSAUR_DOLL] =
    {
        .id = DECOR_VENUSAUR_DOLL,
        .name = _("이상해꽃인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("이상해꽃의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_VENUSAUR_DOLL,
        .icon = {gDecorIcon_VenusaurDoll, gDecorIconPalette_VenusaurDoll},
    },

    [DECOR_CHARIZARD_DOLL] =
    {
        .id = DECOR_CHARIZARD_DOLL,
        .name = _("리자몽인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("리자몽의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_CHARIZARD_DOLL,
        .icon = {gDecorIcon_CharizardDoll, gDecorIconPalette_CharizardDoll},
    },

    [DECOR_BLASTOISE_DOLL] =
    {
        .id = DECOR_BLASTOISE_DOLL,
        .name = _("거북왕인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("거북왕의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_BLASTOISE_DOLL,
        .icon = {gDecorIcon_BlastoiseDoll, gDecorIconPalette_BlastoiseDoll},
    },

    [DECOR_WAILMER_DOLL] =
    {
        .id = DECOR_WAILMER_DOLL,
        .name = _("고래왕자인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("고래왕자의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_WAILMER_DOLL,
        .icon = {gDecorIcon_WailmerDoll, gDecorIconPalette_WailmerDoll},
    },

    [DECOR_REGIROCK_DOLL] =
    {
        .id = DECOR_REGIROCK_DOLL,
        .name = _("레지락인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("레지락의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_REGIROCK_DOLL,
        .icon = {gDecorIcon_RegirockDoll, gDecorIconPalette_RegirockDoll},
    },

    [DECOR_REGICE_DOLL] =
    {
        .id = DECOR_REGICE_DOLL,
        .name = _("레지아이스인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("레지아이스의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_REGICE_DOLL,
        .icon = {gDecorIcon_RegiceDoll, gDecorIconPalette_RegiceDoll},
    },

    [DECOR_REGISTEEL_DOLL] =
    {
        .id = DECOR_REGISTEEL_DOLL,
        .name = _("레지스틸인형"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = COMPOUND_STRING("레지스틸의 큰 봉제인형\n매트나 책상 위에도\n놓을 수 있다."),
        .tiles = DecorGfx_REGISTEEL_DOLL,
        .icon = {gDecorIcon_RegisteelDoll, gDecorIconPalette_RegisteelDoll},
    }
};
