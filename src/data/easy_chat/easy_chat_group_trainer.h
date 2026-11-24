const struct EasyChatWordInfo gEasyChatGroup_Trainer[] = {
    [EC_INDEX(EC_WORD_I_CHOOSE_YOU)] =
    {
        .text = COMPOUND_STRING("너로 정했다!"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BAG),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GOTCHA)] =
    {
        .text = COMPOUND_STRING("겟"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GOTCHA),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRADE)] =
    {
        .text = COMPOUND_STRING("교환"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GOLD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SAPPHIRE)] =
    {
        .text = COMPOUND_STRING("사파이어"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRADE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EVOLVE)] =
    {
        .text = COMPOUND_STRING("진화해"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GREEN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ENCYCLOPEDIA)] =
    {
        .text = COMPOUND_STRING("도감"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_I_CHOOSE_YOU),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NATURE)] =
    {
        .text = COMPOUND_STRING("성격"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ENCYCLOPEDIA),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CENTER)] =
    {
        .text = COMPOUND_STRING("센터"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EGG)] =
    {
        .text = COMPOUND_STRING("알"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LEVEL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LINK)] =
    {
        .text = COMPOUND_STRING("통신"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RUBY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SP_ABILITY)] =
    {
        .text = COMPOUND_STRING("특성"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LEAF),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRAINER)] =
    {
        .text = COMPOUND_STRING("트레이너"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_VERSION),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_VERSION)] =
    {
        .text = COMPOUND_STRING("버전"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SAPPHIRE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_POKENAV)] =
    {
        .text = COMPOUND_STRING("포켓기어"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NATURE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_POKEMON)] =
    {
        .text = COMPOUND_STRING("포켓몬"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CENTER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GET)] =
    {
        .text = COMPOUND_STRING("포켓몬 겟"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SILVER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_POKEDEX)] =
    {
        .text = COMPOUND_STRING("포켓몬 도감"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EGG),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RUBY)] =
    {
        .text = COMPOUND_STRING("루비"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EMERALD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LEVEL)] =
    {
        .text = COMPOUND_STRING("레벨"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EVOLVE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RED)] =
    {
        .text = COMPOUND_STRING("레드"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LINK),
        .enabled = FALSE,
    },
    [EC_INDEX(EC_WORD_GREEN)] =
    {
        .text = COMPOUND_STRING("그린"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRAINER),
        .enabled = FALSE,
    },
    [EC_INDEX(EC_WORD_BAG)] =
    {
        .text = COMPOUND_STRING("가방"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SP_ABILITY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FLAME)] =
    {
        .text = COMPOUND_STRING("파이어"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FLAME),
        .enabled = FALSE,
    },
    [EC_INDEX(EC_WORD_GOLD)] =
    {
        .text = COMPOUND_STRING("골드"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_POKENAV),
        .enabled = FALSE,
    },
    [EC_INDEX(EC_WORD_LEAF)] =
    {
        .text = COMPOUND_STRING("리프"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_POKEMON),
        .enabled = FALSE,
    },
    [EC_INDEX(EC_WORD_SILVER)] =
    {
        .text = COMPOUND_STRING("실버"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GET),
        .enabled = FALSE,
    },
    [EC_INDEX(EC_WORD_EMERALD)] =
    {
        .text = COMPOUND_STRING("에메랄드"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_POKEDEX),
        .enabled = TRUE,
    },
};
