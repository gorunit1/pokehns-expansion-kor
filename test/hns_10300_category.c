// HnS seq 478 #10300 회귀 테스트 — 기술 분류(물리·특수·변화)를 dynamicMoveCategory 하나로 합치고 DAMAGE_CATEGORY_NONE(0)을
// 앞에 넣은 뒤의 분류 아이콘 표(#10365 seq 480 앞당김), HnS 물리·특수 분리 챌린지(optionStyle), 프레젠트 회복 스크립트, 동적 타입 초기화.
// 이름이 모두 "HnS 10300:"으로 시작한다.
#include "global.h"
#include "battle_util.h"
#include "pokemon_summary_screen.h"
#include "test/battle.h"

// The category icon animation for each category (physical, special, status) is the frame at tile 0, 4, 8.
TEST("HnS 10300: category icon animations follow the category values")
{
    u32 physical = gSpriteTemplate_CategoryIcons.anims[DAMAGE_CATEGORY_PHYSICAL][0].frame.imageValue;
    u32 special = gSpriteTemplate_CategoryIcons.anims[DAMAGE_CATEGORY_SPECIAL][0].frame.imageValue;
    u32 status = gSpriteTemplate_CategoryIcons.anims[DAMAGE_CATEGORY_STATUS][0].frame.imageValue;
    EXPECT_EQ(physical, 0);
    EXPECT_EQ(special, 4);
    EXPECT_EQ(status, 8);
}

// The HnS physical/special challenge option (optionStyle 1) splits by type out of battle (summary screen),
// and a status move stays a status move.
TEST("HnS 10300: the type-based split option keeps status moves as status out of battle")
{
    u32 savedStyle = gSaveBlock3Ptr->challengeSettings.optionStyle;
    enum DamageCategory punchSplit, punchType, growlType, psychicType;

    gSaveBlock3Ptr->challengeSettings.optionStyle = 0;
    punchSplit = GetBattleMoveCategory(MOVE_FIRE_PUNCH);
    gSaveBlock3Ptr->challengeSettings.optionStyle = 1;
    punchType = GetBattleMoveCategory(MOVE_FIRE_PUNCH);
    growlType = GetBattleMoveCategory(MOVE_GROWL);
    psychicType = GetBattleMoveCategory(MOVE_PSYCHIC);
    gSaveBlock3Ptr->challengeSettings.optionStyle = savedStyle;

    EXPECT_EQ(punchSplit, DAMAGE_CATEGORY_PHYSICAL);
    EXPECT_EQ(punchType, DAMAGE_CATEGORY_SPECIAL);
    EXPECT_EQ(growlType, DAMAGE_CATEGORY_STATUS);
    EXPECT_EQ(psychicType, DAMAGE_CATEGORY_SPECIAL);
}

// Present's heal roll after a damaging roll: it heals (the last roll's power does not carry over), with the sparkle and the line.
SINGLE_BATTLE_TEST("HnS 10300: Present heals on a heal roll after a damaging roll")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PRESENT) == EFFECT_PRESENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(200); MaxHP(400); }
    } WHEN {
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 0)); }
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 254)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PRESENT, player);
        HP_BAR(opponent);
        MESSAGE("마자용은 프레젠트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PRESENT, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SIMPLE_HEAL, opponent);
        HP_BAR(opponent);
        MESSAGE("상대 마자의 체력이 회복되었다!");
    } THEN {
        EXPECT_GT(opponent->hp, 100); // the heal (+100) is more than the 40-power hit took
    }
}

// Present's heal roll on a target at full HP: the "HP is full" line, no animation.
SINGLE_BATTLE_TEST("HnS 10300: Present's heal roll on a full-HP target says its HP is full")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 254)); }
    } SCENE {
        MESSAGE("마자용은 프레젠트를 썼다!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_PRESENT, player);
        MESSAGE("그러나 상대 마자는 체력이 가득찬 상태다!");
    }
}

// Sleep Talk is Normal, so Pixilate makes it Fairy; the Ember it calls is still Fire (not very effective on a Dragon).
SINGLE_BATTLE_TEST("HnS 10300: a move called by Sleep Talk does not keep Sleep Talk's Pixilate type")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_SLEEP_TALK) == TYPE_NORMAL);
        ASSUME(GetMoveType(MOVE_EMBER) == TYPE_FIRE);
        PLAYER(SPECIES_SYLVEON) { Ability(ABILITY_PIXILATE); Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_SLEEP_TALK, MOVE_EMBER); }
        OPPONENT(SPECIES_BAGON);
    } WHEN {
        TURN { MOVE(player, MOVE_SLEEP_TALK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EMBER, player);
        HP_BAR(opponent);
        MESSAGE("효과가 별로인 듯하다.");
    }
}

// Present's heal roll restores a quarter of the target's max HP.
SINGLE_BATTLE_TEST("HnS 10300: Present's heal roll restores a quarter of max HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT) { HP(100); MaxHP(400); }
    } WHEN {
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 254)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PRESENT, player);
        HP_BAR(opponent);
        MESSAGE("상대 마자의 체력이 회복되었다!");
    } THEN {
        EXPECT_EQ(opponent->hp, 200);
    }
}

// A Present heal roll as the last move of a turn leaves no status category behind: next turn, Prankster does not raise the
// priority of a damaging move (the category is cleared after every action, as the dynamic type is).
SINGLE_BATTLE_TEST("HnS 10300: a Present heal roll does not make the next turn's moves count as status moves")
{
    GIVEN {
        ASSUME(GetMoveCategory(MOVE_TACKLE) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_MURKROW) { Ability(ABILITY_PRANKSTER); Speed(10); HP(100); MaxHP(400); }
    } WHEN {
        TURN { MOVE(player, MOVE_PRESENT, WITH_RNG(RNG_PRESENT, 254)); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        MESSAGE("상대 니로우의 체력이 회복되었다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
    }
}
