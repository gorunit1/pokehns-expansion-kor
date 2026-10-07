#include "global.h"
#include "test/battle.h"

// Friend request C (P2): Mental Herb item pop-up order regression set.
// Scratch only, never committed. Run on a scratch copy with chunk-132/D-tests/trace.patch applied:
// item pop-ups (BattleScript_ItemPopUp_*) are not visible to SCENE before upstream #10321 (seq 413),
// so the pop-up position is checked from the trace (TR:I lines) by tmp-P2/check_order.py.
// SCENE pins the existing Korean text and animations, which must be identical before and after the patch.
// Every battle gets one idle TURN {} after the activation: before #10321 CreateItemPopUp() also runs in
// headless tests, and a pop-up still on screen when the battle ends fails with "Task_FreeAbilityPopUpGfx not freed".
// A space in MESSAGE matches one space or newline of the real text.

SINGLE_BATTLE_TEST("HNSPOPUP 01 Mental Herb target: Taunt")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TAUNT); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 도발을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAUNT, opponent);
        MESSAGE("마자용은 도발에 넘어가 버렸다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 도발의 효과가 풀렸다!");
    } THEN {
        EXPECT(player->volatiles.tauntTimer == 0);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 02 Mental Herb target: Encore")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_ENCORE); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 앙코르를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ENCORE, opponent);
        MESSAGE("마자용은 앙코르를 받았다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용의 앙코르 상태가 풀렸다!");
    } THEN {
        EXPECT(player->volatiles.encoreTimer == 0);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 03 Mental Herb target: Attract")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); Gender(MON_MALE); }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ATTRACT); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자용은 헤롱헤롱을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ATTRACT, opponent);
        MESSAGE("마자용은 헤롱헤롱해졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 헤롱헤롱 상태가 나았다!");
    } THEN {
        EXPECT(player->volatiles.infatuation == 0);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 04 Mental Herb target: Torment")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TORMENT); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 트집을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TORMENT, opponent);
        MESSAGE("마자용은 트집을 잡혔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용의 트집 효과가 사라졌다!");
    } THEN {
        EXPECT(player->volatiles.torment == FALSE);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 05 Mental Herb target: Disable")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_DISABLE); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 사슬묶기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DISABLE, opponent);
        MESSAGE("마자용의 축하를 봉인했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용의 사슬묶기가 풀렸다!");
    } THEN {
        EXPECT(player->volatiles.disableTimer == 0);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 06 Mental Herb target: Heal Block and Psychic Noise")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_HEAL_BLOCK; }
    PARAMETRIZE { move = MOVE_PSYCHIC_NOISE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, move); }
        TURN {}
    } SCENE {
        if (move == MOVE_HEAL_BLOCK)
            MESSAGE("상대 마자는 회복봉인을 썼다!");
        else
            MESSAGE("상대 마자는 사이코노이즈를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        MESSAGE("마자용은 회복 동작을 봉인당했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용의 회복봉인 효과가 사라졌다!");
        // Pre-existing, not part of the pop-up patch: Psychic Noise leaves healBlockTimer set after the cure,
        // so the end-of-turn line repeats at turn 2 (fixed by upstream #10307, seq 254).
    } THEN {
        EXPECT(player->volatiles.healBlock == FALSE);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 07 Mental Herb attacker: Cute Charm infatuation")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); Gender(MON_MALE); }
        OPPONENT(SPECIES_CLEFAIRY) { Gender(MON_FEMALE); Ability(ABILITY_CUTE_CHARM); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 할퀴기를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ABILITY_POPUP(opponent, ABILITY_CUTE_CHARM);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
        MESSAGE("마자용은 헤롱헤롱해졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 헤롱헤롱 상태가 나았다!");
    } THEN {
        EXPECT(player->volatiles.infatuation == 0);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 08 Mental Herb attacker: Cursed Body disable")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); }
        OPPONENT(SPECIES_FRILLISH) { Ability(ABILITY_CURSED_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_AQUA_JET); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 아쿠아제트를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AQUA_JET, player);
        ABILITY_POPUP(opponent, ABILITY_CURSED_BODY);
        MESSAGE("마자용의 아쿠아제트를 봉인했다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용의 사슬묶기가 풀렸다!");
    } THEN {
        EXPECT(player->volatiles.disableTimer == 0);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 09 Mental Herb attacker after foe's Destiny Knot (two item pop-ups)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); Gender(MON_MALE); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_DESTINY_KNOT); Gender(MON_FEMALE); }
    } WHEN {
        TURN { MOVE(player, MOVE_ATTRACT); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 헤롱헤롱을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ATTRACT, player);
        MESSAGE("상대 마자용은 헤롱헤롱해졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("마자용은 빨간실 때문에 헤롱헤롱해졌다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 헤롱헤롱 상태가 나았다!");
    } THEN {
        EXPECT(player->volatiles.infatuation == 0);
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 10 Mental Herb Fling: Taunt and Torment on the target (Fling path, two messages)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_MENTAL_HERB); Speed(100); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_RAZOR_CLAW); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_TAUNT); MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_TORMENT); MOVE(opponent, MOVE_POUND); }
        TURN { MOVE(player, MOVE_FLING); MOVE(opponent, MOVE_SCRATCH); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 내던지기를 썼다!");
        MESSAGE("마자용은 멘탈허브를 내던졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLING, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자의 트집 효과가 사라졌다!");
        MESSAGE("상대 마자는 도발의 효과가 풀렸다!");
    } THEN {
        EXPECT(opponent->volatiles.tauntTimer == 0);
        EXPECT(opponent->volatiles.torment == FALSE);
        EXPECT_EQ(opponent->item, ITEM_RAZOR_CLAW);
    }
}

DOUBLE_BATTLE_TEST("HNSPOPUP 11 Mental Herb target after Trick: six statuses, one pop-up, messages in order")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_MALE); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); }
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_MENTAL_HERB); };
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_CELEBRATE);
               MOVE(opponentLeft, MOVE_HEAL_BLOCK, target: playerLeft);
               MOVE(playerRight, MOVE_DISABLE, target: playerLeft);
               MOVE(opponentRight, MOVE_TORMENT, target: playerLeft); }
        TURN { MOVE(playerLeft, MOVE_SPLASH);
               MOVE(opponentLeft, MOVE_CELEBRATE);
               MOVE(playerRight, MOVE_CELEBRATE);
               MOVE(opponentRight, MOVE_TAUNT, target: playerLeft); }
        TURN { MOVE(playerLeft, MOVE_POUND, target: opponentRight);
               MOVE(opponentLeft, MOVE_ATTRACT, target: playerLeft);
               MOVE(playerRight, MOVE_ENCORE, target: playerLeft);
               MOVE(opponentRight, MOVE_TRICK, target: playerLeft); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 트릭을 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK, opponentRight);
        MESSAGE("상대 마자는 서로의 도구를 교체했다!");
        MESSAGE("마자용은 멘탈허브를 손에 넣었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, playerLeft);
        MESSAGE("마자용은 헤롱헤롱 상태가 나았다!");
        MESSAGE("마자용의 트집 효과가 사라졌다!");
        MESSAGE("마자용의 사슬묶기가 풀렸다!");
        MESSAGE("마자용의 회복봉인 효과가 사라졌다!");
        MESSAGE("마자용의 앙코르 상태가 풀렸다!");
        MESSAGE("마자용은 도발의 효과가 풀렸다!");
    }
}

SINGLE_BATTLE_TEST("HNSPOPUP 12 control: White Herb keeps pop-up, animation, message order")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_WHITE_HERB); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_GROWL); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 울음소리를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
    }
}
