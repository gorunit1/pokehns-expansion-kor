const struct EasyChatWordInfo gEasyChatGroup_Speech[] = {
    [EC_INDEX(EC_WORD_LISTEN)] =
    {
        .text = COMPOUND_STRING("있잖아"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LIE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NOT_VERY)] =
    {
        .text = COMPOUND_STRING("딱히"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CASE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MEAN)] =
    {
        .text = COMPOUND_STRING("짓궂다"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNDERSTOOD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LIE)] =
    {
        .text = COMPOUND_STRING("거짓말"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EVEN_SO),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LAY)] =
    {
        .text = COMPOUND_STRING("으음"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RECOMMEND)] =
    {
        .text = COMPOUND_STRING("추천"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FEELING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NITWIT)] =
    {
        .text = COMPOUND_STRING("바보야"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNTIL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_QUITE)] =
    {
        .text = COMPOUND_STRING("상당히"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_JOKING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FROM)] =
    {
        .text = COMPOUND_STRING("에서"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NATURALLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FEELING)] =
    {
        .text = COMPOUND_STRING("기분"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AWFULLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BUT)] =
    {
        .text = COMPOUND_STRING("하지만"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_INSTEAD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HOWEVER)] =
    {
        .text = COMPOUND_STRING("바로"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CASE)] =
    {
        .text = COMPOUND_STRING("경우"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TOO_LATE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THE)] =
    {
        .text = COMPOUND_STRING("더"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AROUND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MISS)] =
    {
        .text = COMPOUND_STRING("자"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WILD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HOW)] =
    {
        .text = COMPOUND_STRING("어떻게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FINALLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HIT)] =
    {
        .text = COMPOUND_STRING("자 그럼"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NOT_VERY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ENOUGH)] =
    {
        .text = COMPOUND_STRING("충분"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THAT_S),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_A_LOT)] =
    {
        .text = COMPOUND_STRING("엄청"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_JUST),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_A_LITTLE)] =
    {
        .text = COMPOUND_STRING("조금은"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AS_IF),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ABSOLUTELY)] =
    {
        .text = COMPOUND_STRING("절대로"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_IF),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AND)] =
    {
        .text = COMPOUND_STRING("그래서"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AS_MUCH_AS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ONLY)] =
    {
        .text = COMPOUND_STRING("뿐"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_VERY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AROUND)] =
    {
        .text = COMPOUND_STRING("도"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MODE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PROBABLY)] =
    {
        .text = COMPOUND_STRING("아마"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MOOD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_IF)] =
    {
        .text = COMPOUND_STRING("만약"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_READY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_VERY)] =
    {
        .text = COMPOUND_STRING("매우"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HOWEVER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_A_TINY_BIT)] =
    {
        .text = COMPOUND_STRING("잠깐"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NITWIT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WILD)] =
    {
        .text = COMPOUND_STRING("도 참"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MUST_BE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THAT_S)] =
    {
        .text = COMPOUND_STRING("라고"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ONLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_JUST)] =
    {
        .text = COMPOUND_STRING("라기보단"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_QUITE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EVEN_SO)] =
    {
        .text = COMPOUND_STRING("그래도"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PROBABLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MUST_BE)] =
    {
        .text = COMPOUND_STRING("반드시"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NATURALLY)] =
    {
        .text = COMPOUND_STRING("당연히"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SOMETHING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FOR_NOW)] =
    {
        .text = COMPOUND_STRING("일단"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SOMEHOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNDERSTOOD)] =
    {
        .text = COMPOUND_STRING("과연"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_A_LOT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_JOKING)] =
    {
        .text = COMPOUND_STRING("농담이야"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ALTHOUGH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_READY)] =
    {
        .text = COMPOUND_STRING("뭐든지"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ALSO),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SOMETHING)] =
    {
        .text = COMPOUND_STRING("어떻게든"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FROM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SOMEHOW)] =
    {
        .text = COMPOUND_STRING("어쩐지"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ANY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALTHOUGH)] =
    {
        .text = COMPOUND_STRING("에는"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RATHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALSO)] =
    {
        .text = COMPOUND_STRING("에도"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PERFECT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PERFECT)] =
    {
        .text = COMPOUND_STRING("완벽하게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TOTALLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AS_MUCH_AS)] =
    {
        .text = COMPOUND_STRING("만큼"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LAY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_REALLY)] =
    {
        .text = COMPOUND_STRING("정말"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FOR_NOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRULY)] =
    {
        .text = COMPOUND_STRING("진짜로"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FANTASTIC),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SERIOUSLY)] =
    {
        .text = COMPOUND_STRING("진지하게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LISTEN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOTALLY)] =
    {
        .text = COMPOUND_STRING("완전히"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MISS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNTIL)] =
    {
        .text = COMPOUND_STRING("까지"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HIT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AS_IF)] =
    {
        .text = COMPOUND_STRING("마치"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_A_TINY_BIT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MOOD)] =
    {
        .text = COMPOUND_STRING("무드"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ABSOLUTELY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RATHER)] =
    {
        .text = COMPOUND_STRING("오히려"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_REALLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AWFULLY)] =
    {
        .text = COMPOUND_STRING("대단히"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_A_LITTLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MODE)] =
    {
        .text = COMPOUND_STRING("모드"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MORE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MORE)] =
    {
        .text = COMPOUND_STRING("좀 더"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SERIOUSLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOO_LATE)] =
    {
        .text = COMPOUND_STRING("더 이상"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRULY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FINALLY)] =
    {
        .text = COMPOUND_STRING("드디어"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MEAN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ANY)] =
    {
        .text = COMPOUND_STRING("역시"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RECOMMEND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_INSTEAD)] =
    {
        .text = COMPOUND_STRING("대신"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ENOUGH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FANTASTIC)] =
    {
        .text = COMPOUND_STRING("있다면"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BUT),
        .enabled = TRUE,
    },
};
