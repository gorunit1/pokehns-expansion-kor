const struct ContestEffect gContestEffects[] =
{
    [CONTEST_EFFECT_HIGHLY_APPEALING] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("많이 어필할 수 있다."),
        #else
        .description = COMPOUND_STRING("많이 어필할 수 있다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_APPEAL,
        .appeal = 40,
        .jam = 0,
        .function = ContestEffect_HighlyAppealing,
    },
    [CONTEST_EFFECT_USER_MORE_EASILY_STARTLED] =
    {
        .description = COMPOUND_STRING("아주 많이 어필할 수 있지만\n이다음에 놀라기 쉬워진다."),
        .effectType = CONTEST_EFFECT_TYPE_APPEAL,
        .appeal = 60,
        .jam = 0,
        .function = ContestEffect_UserMoreEasilyStartled,
    },
    [CONTEST_EFFECT_GREAT_APPEAL_BUT_NO_MORE_MOVES] =
    {
        .description = COMPOUND_STRING("엄청난 어필이 되지만 이후부터\n마지막까지 아무것도 할 수 없다."),
        .effectType = CONTEST_EFFECT_TYPE_APPEAL,
        .appeal = 80,
        .jam = 0,
        .function = ContestEffect_GreatAppealButNoMoreMoves,
    },
    [CONTEST_EFFECT_REPETITION_NOT_BORING] =
    {
        .description = COMPOUND_STRING("계속해서 써도 관객이\n싫증 내지 않게 어필할 수 있다."),
        .effectType = CONTEST_EFFECT_TYPE_APPEAL,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_RepetitionNotBoring,
    },
    [CONTEST_EFFECT_AVOID_STARTLE_ONCE] =
    {
        .description = COMPOUND_STRING("다른 포켓몬이 놀래도\n한 번 정도는 참을 수 있다."),
        .effectType = CONTEST_EFFECT_TYPE_AVOID_STARTLE,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_AvoidStartleOnce,
    },
    [CONTEST_EFFECT_AVOID_STARTLE] =
    {
        .description = COMPOUND_STRING("다른 포켓몬이 놀래도\n참을 수 있다."),
        .effectType = CONTEST_EFFECT_TYPE_AVOID_STARTLE,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_AvoidStartle,
    },
    [CONTEST_EFFECT_AVOID_STARTLE_SLIGHTLY] =
    {
        .description = COMPOUND_STRING("다른 포켓몬이 놀래도\n조금은 참을 수 있다."),
        .effectType = CONTEST_EFFECT_TYPE_AVOID_STARTLE,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_AvoidStartleSlightly,
    },
    [CONTEST_EFFECT_USER_LESS_EASILY_STARTLED] =
    {
        .description = COMPOUND_STRING("이다음에 다른 포켓몬에게\n잘 놀라지 않게 된다."),
        .effectType = CONTEST_EFFECT_TYPE_AVOID_STARTLE,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_UserLessEasilyStartled,
    },
    [CONTEST_EFFECT_STARTLE_FRONT_MON] =
    {
        .description = COMPOUND_STRING("자신의 앞에서 어필한\n포켓몬을 조금 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MON,
        .appeal = 30,
        .jam = 20,
        .function = ContestEffect_StartleFrontMon,
    },
    [CONTEST_EFFECT_SLIGHTLY_STARTLE_PREV_MONS] =
    {
        .description = COMPOUND_STRING("어필이 끝난 포켓몬 모두를\n조금 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 30,
        .jam = 10,
        .function = ContestEffect_StartlePrevMons,
    },
    [CONTEST_EFFECT_STARTLE_PREV_MON] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("자신의 앞에서 어필한\n포켓몬을 놀라게 만든다."),
        #else
        .description = COMPOUND_STRING("자신의 앞에서 어필한\n포켓몬을 놀라게 만든다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MON,
        .appeal = 20,
        .jam = 30,
        .function = ContestEffect_StartleFrontMon,
    },
    [CONTEST_EFFECT_STARTLE_PREV_MONS] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("어필이 끝난\n포켓몬 모두를 놀라게 만든다."),
        #else
        .description = COMPOUND_STRING("어필이 끝난\n포켓몬 모두를 놀라게 만든다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 20,
        .function = ContestEffect_StartlePrevMons,
    },
    [CONTEST_EFFECT_BADLY_STARTLE_FRONT_MON] =
    {
        .description = COMPOUND_STRING("자신의 앞에서 어필한\n포켓몬을 상당히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MON,
        .appeal = 10,
        .jam = 40,
        .function = ContestEffect_StartleFrontMon,
    },
    [CONTEST_EFFECT_BADLY_STARTLE_PREV_MONS] =
    {
        .description = COMPOUND_STRING("어필이 끝난 포켓몬 모두를\n상당히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 10,
        .jam = 30,
        .function = ContestEffect_StartlePrevMons,
    },
    [CONTEST_EFFECT_STARTLE_PREV_MON_2] =
    {
        .description = COMPOUND_STRING("자신의 앞에서 어필한\n포켓몬을 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MON,
        .appeal = 30,
        .jam = 20,
        .function = ContestEffect_StartlePrevMon2,
    },
    [CONTEST_EFFECT_STARTLE_PREV_MONS_2] =
    {
        .description = COMPOUND_STRING("어필이 끝난\n포켓몬 모두를 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 30,
        .jam = 10,
        .function = ContestEffect_StartlePrevMons2,
    },
    [CONTEST_EFFECT_SHIFT_JUDGE_ATTENTION] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("관객의 다른 포켓몬에 대한\n기대를 없앨 수 있다."),
        #else
        .description = COMPOUND_STRING("관객의 다른 포켓몬에 대한\n기대를 없앨 수 있다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_WORSEN,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_ShiftJudgeAttention,
    },
    [CONTEST_EFFECT_STARTLE_MON_WITH_JUDGES_ATTENTION] =
    {
        .description = COMPOUND_STRING("관객에게 기대받는\n포켓몬을 특히 놀라게 한다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonWithJudgesAttention,
    },
    [CONTEST_EFFECT_JAMS_OTHERS_BUT_MISS_ONE_TURN] =
    {
        .description = COMPOUND_STRING("모두를 방해하고\n다음 어필은 참가하지 않는다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 40,
        .jam = 40,
        .function = ContestEffect_JamsOthersButMissOneTurn,
    },
    [CONTEST_EFFECT_STARTLE_MONS_SAME_TYPE_APPEAL] =
    {
        .description = COMPOUND_STRING("같은 타입의 어필을 한\n포켓몬을 특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonsSameTypeAppeal,
    },
    [CONTEST_EFFECT_STARTLE_MONS_COOL_APPEAL] =
    {
        .description = COMPOUND_STRING("근사함 어필을 한\n포켓몬을 특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonsCoolAppeal,
    },
    [CONTEST_EFFECT_STARTLE_MONS_BEAUTY_APPEAL] =
    {
        .description = COMPOUND_STRING("아름다움 어필을 한\n포켓몬을 특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonsBeautyAppeal,
    },
    [CONTEST_EFFECT_STARTLE_MONS_CUTE_APPEAL] =
    {
        .description = COMPOUND_STRING("귀여움 어필을 한\n포켓몬을 특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonsCuteAppeal,
    },
    [CONTEST_EFFECT_STARTLE_MONS_SMART_APPEAL] =
    {
        .description = COMPOUND_STRING("슬기로움 어필을 한\n포켓몬을 특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonsSmartAppeal,
    },
    [CONTEST_EFFECT_STARTLE_MONS_TOUGH_APPEAL] =
    {
        .description = COMPOUND_STRING("강인함 어필을 한\n포켓몬을 특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_StartleMonsToughAppeal,
    },
    [CONTEST_EFFECT_MAKE_FOLLOWING_MON_NERVOUS] =
    {
        .description = COMPOUND_STRING("이다음에 어필할\n포켓몬 1마리를 긴장시킨다."),
        .effectType = CONTEST_EFFECT_TYPE_WORSEN,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_MakeFollowingMonNervous,
    },
    [CONTEST_EFFECT_MAKE_FOLLOWING_MONS_NERVOUS] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("이다음 어필을 하는\n포켓몬 모두를 긴장시킨다."),
        #else
        .description = COMPOUND_STRING("이다음 어필을 하는\n포켓몬 모두를 긴장시킨다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_WORSEN,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_MakeFollowingMonsNervous,
    },
    [CONTEST_EFFECT_WORSEN_CONDITION_OF_PREV_MONS] =
    {
        .description = COMPOUND_STRING("어필이 끝난\n포켓몬의 컨디션을 떨어뜨린다."),
        .effectType = CONTEST_EFFECT_TYPE_WORSEN,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_WorsenConditionOfPrevMons,
    },
    [CONTEST_EFFECT_BADLY_STARTLES_MONS_IN_GOOD_CONDITION] =
    {
        .description = COMPOUND_STRING("컨디션이 좋은 포켓몬을\n특히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 30,
        .jam = 10,
        .function = ContestEffect_BadlyStartlesMonsInGoodCondition,
    },
    [CONTEST_EFFECT_BETTER_IF_FIRST] =
    {
        .description = COMPOUND_STRING("제일 처음으로 어필하면\n어필이 굉장히 잘 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_BetterIfFirst,
    },
    [CONTEST_EFFECT_BETTER_IF_LAST] =
    {
        .description = COMPOUND_STRING("제일 마지막에 어필하면\n어필이 굉장히 잘 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_BetterIfLast,
    },
    [CONTEST_EFFECT_APPEAL_AS_GOOD_AS_PREV_ONES] =
    {
        .description = COMPOUND_STRING("그 턴에서 어필한 다른 포켓몬과\n같은 정도의 어필을 보인다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_AppealAsGoodAsPrevOnes,
    },
    [CONTEST_EFFECT_APPEAL_AS_GOOD_AS_PREV_ONE] =
    {
        .description = COMPOUND_STRING("하나 앞의 포켓몬 어필과\n같은 정도로 잘 할 수 있다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_AppealAsGoodAsPrevOne,
    },
    [CONTEST_EFFECT_BETTER_WHEN_LATER] =
    {
        .description = COMPOUND_STRING("나중에 어필을 할수록\n굉장한 어필을 보여줄 수 있다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_BetterWhenLater,
    },
    [CONTEST_EFFECT_QUALITY_DEPENDS_ON_TIMING] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("쓸 때마다 어필의 완성도가\n여러 가지로 바뀐다."),
        #else
        .description = COMPOUND_STRING("쓸 때마다 어필의 완성도가\n여러 가지로 바뀐다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_QualityDependsOnTiming,
    },
    [CONTEST_EFFECT_BETTER_IF_SAME_TYPE] =
    {
        .description = COMPOUND_STRING("하나 앞의 포켓몬 어필과\n타입이 같다면 마음에 들게 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_BetterIfSameType,
    },
    [CONTEST_EFFECT_BETTER_IF_DIFF_TYPE] =
    {
        .description = COMPOUND_STRING("하나 앞의 포켓몬 어필과\n타입이 다르면 마음에 들게 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 20,
        .jam = 0,
        .function = ContestEffect_BetterIfDiffType,
    },
    [CONTEST_EFFECT_AFFECTED_BY_PREV_APPEAL] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("하나 앞의 포켓몬 어필이\n잘 된 정도에 따라 영향을 받는다."),
        #else
        .description = COMPOUND_STRING("하나 앞의 포켓몬 어필이\n잘 된 정도에 따라 영향을 받는다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_AffectedByPrevAppeal,
    },
    [CONTEST_EFFECT_IMPROVE_CONDITION_PREVENT_NERVOUSNESS] =
    {
        .description = COMPOUND_STRING("어필 컨디션이 올라간다\n긴장도 잘 하지 않게 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_ImproveConditionPreventNervousness,
    },
    [CONTEST_EFFECT_BETTER_WITH_GOOD_CONDITION] =
    {
        .description = COMPOUND_STRING("컨디션이 좋을 때 쓰면\n어필이 아주 잘 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_BetterWithGoodCondition,
    },
    [CONTEST_EFFECT_NEXT_APPEAL_EARLIER] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("이다음의 어필을\n첫 번째로 할 수 있다."),
        #else
        .description = COMPOUND_STRING("이다음의 어필을\n첫 번째로 할 수 있다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_TURN_ORDER,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_NextAppealEarlier,
    },
    [CONTEST_EFFECT_NEXT_APPEAL_LATER] =
    {
        #if C_UPDATED_MOVE_EFFECTS >= GEN_6
        .description = COMPOUND_STRING("이다음의 어필을\n마지막에 쓸 수 있다."),
        #else
        .description = COMPOUND_STRING("이다음의 어필을\n마지막에 쓸 수 있다."),
        #endif
        .effectType = CONTEST_EFFECT_TYPE_TURN_ORDER,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_NextAppealLater,
    },
    [CONTEST_EFFECT_MAKE_SCRAMBLING_TURN_ORDER_EASIER] =
    {
        .description = COMPOUND_STRING("이다음의 어필 순서가\n엉망이 되기 쉬워진다."),
        .effectType = CONTEST_EFFECT_TYPE_TURN_ORDER,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_MakeScramblingTurnOrderEasier,
    },
    [CONTEST_EFFECT_SCRAMBLE_NEXT_TURN_ORDER] =
    {
        .description = COMPOUND_STRING("이다음의 어필 순서를\n엉망으로 한다."),
        .effectType = CONTEST_EFFECT_TYPE_TURN_ORDER,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_ScrambleNextTurnOrder,
    },
    [CONTEST_EFFECT_EXCITE_AUDIENCE_IN_ANY_CONTEST] =
    {
        .description = COMPOUND_STRING("어느 콘테스트에서 보여도\n반드시 열광하는 어필."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = C_UPDATED_MOVE_EFFECTS >= GEN_6 ? 20 : 10,
        .jam = 0,
        .function = ContestEffect_ExciteAudienceInAnyContest,
    },
    [CONTEST_EFFECT_BADLY_STARTLE_MONS_WITH_GOOD_APPEALS] =
    {
        .description = COMPOUND_STRING("어필이 잘 된 포켓몬 모두를\n상당히 놀라게 만든다."),
        .effectType = CONTEST_EFFECT_TYPE_STARTLE_MONS,
        .appeal = 20,
        .jam = 10,
        .function = ContestEffect_BadlyStartleMonsWithGoodAppeals,
    },
    [CONTEST_EFFECT_BETTER_WHEN_AUDIENCE_EXCITED] =
    {
        .description = COMPOUND_STRING("관객이 열광하고 있을수록\n어필이 마음에 들게 된다."),
        .effectType = CONTEST_EFFECT_TYPE_SPECIAL_APPEAL,
        .appeal = 10,
        .jam = 0,
        .function = ContestEffect_BetterWhenAudienceExcited,
    },
    [CONTEST_EFFECT_DONT_EXCITE_AUDIENCE] =
    {
        .description = COMPOUND_STRING("이 어필 다음에 관객이\n잠시 열광하지 않게 된다."),
        .effectType = CONTEST_EFFECT_TYPE_WORSEN,
        .appeal = 30,
        .jam = 0,
        .function = ContestEffect_DontExciteAudience,
    },
};
