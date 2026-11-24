const struct EasyChatWordInfo gEasyChatGroup_People[] = {
    [EC_INDEX(EC_WORD_OPPONENT)] =
    {
        .text = COMPOUND_STRING("상대"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FAMILY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_I)] =
    {
        .text = COMPOUND_STRING("나"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THEY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_YOU)] =
    {
        .text = COMPOUND_STRING("당신"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THEY_WERE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_YOURS)] =
    {
        .text = COMPOUND_STRING("당신이"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THEY_RE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SON)] =
    {
        .text = COMPOUND_STRING("당신에게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THEY_VE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_YOUR)] =
    {
        .text = COMPOUND_STRING("당신의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TO_THEM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_YOU_RE)] =
    {
        .text = COMPOUND_STRING("당신은"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THEIR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_YOU_VE)] =
    {
        .text = COMPOUND_STRING("당신을"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CHILDREN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MOTHER)] =
    {
        .text = COMPOUND_STRING("엄마"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HIM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GRANDFATHER)] =
    {
        .text = COMPOUND_STRING("할아버지"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNCLE)] =
    {
        .text = COMPOUND_STRING("작은아버지"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SHE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FATHER)] =
    {
        .text = COMPOUND_STRING("아빠"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WOMAN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BOY)] =
    {
        .text = COMPOUND_STRING("남자애"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_I),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ADULT)] =
    {
        .text = COMPOUND_STRING("어른"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BOY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BROTHER)] =
    {
        .text = COMPOUND_STRING("형"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SISTER)] =
    {
        .text = COMPOUND_STRING("누나"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HE_S),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GRANDMOTHER)] =
    {
        .text = COMPOUND_STRING("할머니"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AREN_T),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AUNT)] =
    {
        .text = COMPOUND_STRING("이모"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HIS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PARENT)] =
    {
        .text = COMPOUND_STRING("어버이"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PLACE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MAN)] =
    {
        .text = COMPOUND_STRING("아저씨"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DAUGHTER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ME)] =
    {
        .text = COMPOUND_STRING("이 몸"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WHO_WAS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GIRL)] =
    {
        .text = COMPOUND_STRING("여자애"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WHO),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BABE)] =
    {
        .text = COMPOUND_STRING("아기"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_IT_S),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FAMILY)] =
    {
        .text = COMPOUND_STRING("가족"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TO_WHOM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HER)] =
    {
        .text = COMPOUND_STRING("그녀"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WHOSE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HIM)] =
    {
        .text = COMPOUND_STRING("그"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SOMEONE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HE)] =
    {
        .text = COMPOUND_STRING("너"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SISTER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PLACE)] =
    {
        .text = COMPOUND_STRING("네가"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DAUGHTER)] =
    {
        .text = COMPOUND_STRING("네게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_YOU),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HIS)] =
    {
        .text = COMPOUND_STRING("너의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SON),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HE_S)] =
    {
        .text = COMPOUND_STRING("너는"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_YOU_RE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AREN_T)] =
    {
        .text = COMPOUND_STRING("너를"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_YOU_VE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SIBLINGS)] =
    {
        .text = COMPOUND_STRING("형제"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_YOUR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_KID)] =
    {
        .text = COMPOUND_STRING("아이"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_YOURS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CHILDREN)] =
    {
        .text = COMPOUND_STRING("군"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ALLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MR)] =
    {
        .text = COMPOUND_STRING("님"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RIVAL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MRS)] =
    {
        .text = COMPOUND_STRING("씨"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MYSELF)] =
    {
        .text = COMPOUND_STRING("자신"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BEEN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_I_WAS)] =
    {
        .text = COMPOUND_STRING("자신이"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WE_RE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TO_ME)] =
    {
        .text = COMPOUND_STRING("자신에게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TO_US),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MY)] =
    {
        .text = COMPOUND_STRING("자신의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_OUR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_I_AM)] =
    {
        .text = COMPOUND_STRING("자신은"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PERSON),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_I_VE)] =
    {
        .text = COMPOUND_STRING("자신을"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_OPPONENT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHO)] =
    {
        .text = COMPOUND_STRING("누구"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MRS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SOMEONE)] =
    {
        .text = COMPOUND_STRING("누군가"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BABE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHO_WAS)] =
    {
        .text = COMPOUND_STRING("누가"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WHO_IS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TO_WHOM)] =
    {
        .text = COMPOUND_STRING("누구에게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FATHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHOSE)] =
    {
        .text = COMPOUND_STRING("누구의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_KID),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WHO_IS)] =
    {
        .text = COMPOUND_STRING("아무도"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MAN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_IT_S)] =
    {
        .text = COMPOUND_STRING("누구를"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ADULT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LADY)] =
    {
        .text = COMPOUND_STRING("언니"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PARENT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FRIEND)] =
    {
        .text = COMPOUND_STRING("친구"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LADY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALLY)] =
    {
        .text = COMPOUND_STRING("동료"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MOTHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PERSON)] =
    {
        .text = COMPOUND_STRING("사람"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GIRL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DUDE)] =
    {
        .text = COMPOUND_STRING("오빠"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DUDE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THEY)] =
    {
        .text = COMPOUND_STRING("걔"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WE_VE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THEY_WERE)] =
    {
        .text = COMPOUND_STRING("걔가"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AUNT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TO_THEM)] =
    {
        .text = COMPOUND_STRING("걔에게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THEIR)] =
    {
        .text = COMPOUND_STRING("걔의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MYSELF),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THEY_RE)] =
    {
        .text = COMPOUND_STRING("걔는"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TO_ME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THEY_VE)] =
    {
        .text = COMPOUND_STRING("걔를"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_I_AM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WE)] =
    {
        .text = COMPOUND_STRING("모두"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_I_VE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BEEN)] =
    {
        .text = COMPOUND_STRING("모두가"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TO_US)] =
    {
        .text = COMPOUND_STRING("모두에게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_I_WAS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_OUR)] =
    {
        .text = COMPOUND_STRING("모두의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNCLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WE_RE)] =
    {
        .text = COMPOUND_STRING("모두는"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SHE_IS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RIVAL)] =
    {
        .text = COMPOUND_STRING("라이벌"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SOME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WE_VE)] =
    {
        .text = COMPOUND_STRING("우린"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HERS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WOMAN)] =
    {
        .text = COMPOUND_STRING("그 사람"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SHE_WAS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SHE)] =
    {
        .text = COMPOUND_STRING("그분"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TO_HER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SHE_WAS)] =
    {
        .text = COMPOUND_STRING("제가"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FRIEND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TO_HER)] =
    {
        .text = COMPOUND_STRING("제게"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GRANDMOTHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HERS)] =
    {
        .text = COMPOUND_STRING("저의"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GRANDFATHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SHE_IS)] =
    {
        .text = COMPOUND_STRING("저는"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BROTHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SOME)] =
    {
        .text = COMPOUND_STRING("저를"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SIBLINGS),
        .enabled = TRUE,
    },
};
