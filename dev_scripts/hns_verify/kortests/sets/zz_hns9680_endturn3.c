#include "global.h"
#include "test/battle.h"
// seq 161 #9168: berries use B_ANIM_HELD_ITEM_BERRY after the port (scratch test, updated 2026-10-05)
#ifdef B_ANIM_HELD_ITEM_BERRY
#define HNS_BERRY_ANIM B_ANIM_HELD_ITEM_BERRY
#else
#define HNS_BERRY_ANIM B_ANIM_HELD_ITEM_EFFECT
#endif


// seq 132 #9680 area D: end-of-turn output regression set, part 3 (timers: volatiles, side and field statuses, Uproar).
// Scratch only, never committed. Expected output = HnS before #9680 (HEAD 18f412e9ff, code 587f4e7cdc).
// A space in MESSAGE matches one space or newline of the real text.

SINGLE_BATTLE_TEST("HNS9680 3-01 Taunt ends")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_SCRATCH, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TAUNT); MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자는 도발에 넘어가 버렸다!");
        NONE_OF { MESSAGE("상대 마자는 도발의 효과가 풀렸다!"); }
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("상대 마자는 도발의 효과가 풀렸다!");
        MESSAGE("상대 마자는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-02 Encore ends")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); Moves(MOVE_SCRATCH, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_ENCORE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("상대 마자는 앙코르를 받았다!");
        NONE_OF { MESSAGE("상대 마자의 앙코르 상태가 풀렸다!"); }
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자의 앙코르 상태가 풀렸다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-03 Disable ends")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); Moves(MOVE_SCRATCH, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); MOVE(player, MOVE_DISABLE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("상대 마자의 할퀴기를 봉인했다!");
        NONE_OF { MESSAGE("상대 마자의 사슬묶기가 풀렸다!"); }
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자의 사슬묶기가 풀렸다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-04 Torment (G-Max Meltdown) ends, then Dynamax ends")
{
    GIVEN {
        PLAYER(SPECIES_MELMETAL) { GigantamaxFactor(TRUE); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_SCRATCH, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_IRON_HEAD, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("상대 마자는 트집을 잡혔다!");
        NONE_OF { MESSAGE("상대 마자의 트집 효과가 사라졌다!"); }
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("멜메탈은 다이월을 썼다!");
        MESSAGE("멜메탈은 다이월을 썼다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("상대 마자의 트집 효과가 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("상대 마자는 할퀴기를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-05 Heal Block and Embargo end")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Moves(MOVE_SCRATCH, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_HEAL_BLOCK); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_EMBARGO); MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자는 회복 동작을 봉인당했다!");
        MESSAGE("상대 마자는 도구를 쓸 수 없게 되었다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자의 회복봉인 효과가 사라졌다!");
        MESSAGE("상대 마자는 할퀴기를 썼다!");
        MESSAGE("상대 마자는 도구를 쓸 수 있게 되었다!");
        MESSAGE("상대 마자는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-06 Embargo ends and a Sitrus Berry activates")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_EMBARGO); }
        TURN { MOVE(player, MOVE_SUPER_FANG); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 도구를 쓸 수 없게 되었다!");
        HP_BAR(opponent, damage: 150);
        NONE_OF { MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!"); }
        MESSAGE("상대 마자는 도구를 쓸 수 있게 되었다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!");
        HP_BAR(opponent, damage: -30);
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-07 Telekinesis ends, Magnet Rise ends (name token as in HnS)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_MAGNET_RISE); MOVE(opponent, MOVE_TELEKINESIS); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용은 전자력으로 떠올랐다!");
        MESSAGE("마자용는 높이 뛰어올랐다!");
        MESSAGE("마자용은 텔레키네시스에서 풀려났다!");
        // 2026-10-04 HnS fix (owner decision): HandleEndTurnMagnetRise sets gBattleScripting.battler, so the Magnet Rise user is named.
        MESSAGE("마자용은 전자부유의 효과가 풀렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-08 Reflect, Light Screen, Safeguard, Mist, Tailwind, Lucky Chant end (HnS texts)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_REFLECT); MOVE(opponent, MOVE_LIGHT_SCREEN); }
        TURN { MOVE(player, MOVE_SAFEGUARD); MOVE(opponent, MOVE_MIST); }
        TURN { MOVE(player, MOVE_TAILWIND); MOVE(opponent, MOVE_LUCKY_CHANT); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("우리 편은 리플렉터로 물리공격에 강해졌다!");
        MESSAGE("상대는 빛의장막으로 특수공격에 강해졌다!");
        MESSAGE("우리 편의 리플렉터가 없어졌다!");
        MESSAGE("상대의 빛의장막이 없어졌다!");
        MESSAGE("우리 편을 감싸던 신비의 베일이 없어졌다!");
        MESSAGE("우리 편의 순풍이 멈췄다!");
        MESSAGE("상대를 감싸던 흰안개가 없어졌다!");
        MESSAGE("상대의 주술이 풀렸다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-09 Aurora Veil ends on both sides (HnS text)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SNOWSCAPE); }
        TURN { MOVE(player, MOVE_AURORA_VEIL); MOVE(opponent, MOVE_AURORA_VEIL); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("우리 편은 오로라베일로 물리공격과 특수공격에 강해졌다!");
        MESSAGE("상대는 오로라베일로 물리공격과 특수공격에 강해졌다!");
        MESSAGE("눈이 그쳤다.");
        MESSAGE("우리 편의 오로라베일이 없어졌다!");
        MESSAGE("상대의 오로라베일이 없어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-10 Trick Room, Gravity, Water Sport, Mud Sport, Wonder Room, Magic Room end")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRICK_ROOM); MOVE(opponent, MOVE_GRAVITY); }
        TURN { MOVE(player, MOVE_WATER_SPORT); MOVE(opponent, MOVE_MUD_SPORT); }
        TURN { MOVE(player, MOVE_WONDER_ROOM); MOVE(opponent, MOVE_MAGIC_ROOM); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("방어와 특수방어가 바뀌는 공간을 만들어 냈다!");
        MESSAGE("뒤틀린 시공이 원래대로 되돌아왔다!");
        MESSAGE("중력이 원래대로 되돌아왔다!");
        MESSAGE("물놀이의 효과가 없어졌다!");
        MESSAGE("흙놀이의 효과가 없어졌다!");
        MESSAGE("원더룸이 해제되어 방어와 특수방어가 원래대로 되돌아왔다!");
        MESSAGE("매직룸이 해제되어 도구의 효과가 원래대로 되돌아왔다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-11 Magic Room ends and a Sitrus Berry activates")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_MAGIC_ROOM); }
        TURN { MOVE(player, MOVE_SUPER_FANG); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("지니게 한 도구의 효과가 없어지는 공간을 만들어 냈다!");
        HP_BAR(opponent, damage: 150);
        NONE_OF { MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!"); }
        MESSAGE("매직룸이 해제되어 도구의 효과가 원래대로 되돌아왔다!");
        ANIMATION(ANIM_TYPE_GENERAL, HNS_BERRY_ANIM, opponent);
        MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!");
        HP_BAR(opponent, damage: -30);
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-12 Electric, Misty, Psychic Terrain end")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_ELECTRIC_TERRAIN; }
    PARAMETRIZE { move = MOVE_MISTY_TERRAIN; }
    PARAMETRIZE { move = MOVE_PSYCHIC_TERRAIN; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, move); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        if (move == MOVE_ELECTRIC_TERRAIN)
            MESSAGE("발밑의 전기가 사라졌다!");
        else if (move == MOVE_MISTY_TERRAIN)
            MESSAGE("발밑의 안개가 사라졌다!");
        else
            MESSAGE("발밑의 이상한 느낌이 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG);
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-13 Uproar continues, then calms down")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_UPROAR); }
        TURN { SKIP_TURN(player); }
        TURN { SKIP_TURN(player); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 소란피기 시작했다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용은 소란피우고 있다!");
        MESSAGE("마자용은 소란피기를 썼다!");
        MESSAGE("마자용은 소란피우고 있다!");
        MESSAGE("마자용은 소란피기를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용은 얌전해졌다");
        MESSAGE("마자용은 축하를 썼다!");
        NONE_OF { MESSAGE("마자용은 소란피우고 있다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-14 Uproar (B_UPROAR Gen 4) wakes a sleeping foe at end of turn instead of the continue text")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); Status1(STATUS1_SLEEP_TURN(5)); }
    } WHEN {
        TURN { MOVE(player, MOVE_UPROAR); }
        TURN { SKIP_TURN(player); }
    } SCENE {
        MESSAGE("상대 마자는 쿨쿨 잠들어 있다");
        MESSAGE("마자용은 소란피기 시작했다!");
        NONE_OF { MESSAGE("마자용은 소란피우고 있다!"); }
        MESSAGE("상대 마자는 소란스러워서 눈을 떴다!");
        STATUS_ICON(opponent, none: TRUE);
        MESSAGE("마자용은 소란피기를 썼다!");
        MESSAGE("마자용은 소란피우고 있다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9680 3-15 Uproar end-of-turn wake-up skips a Soundproof sleeper (HnS rule from #9616)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Speed(2); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); Ability(ABILITY_SOUNDPROOF); Status1(STATUS1_SLEEP_TURN(5)); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_UPROAR, WITH_RNG(RNG_RANDOM_TARGET, 1)); }
        TURN { SKIP_TURN(playerLeft); }
    } SCENE {
        MESSAGE("상대 마자용은 쿨쿨 잠들어 있다");
        MESSAGE("마자용은 소란피기 시작했다!");
        MESSAGE("마자용은 소란피우고 있다!");
        // The Soundproof sleeper wakes only before its own action on the next turn (B_UPROAR_IGNORE_SOUNDPROOF path).
        MESSAGE("상대 마자용은 소란스러워서 눈을 떴다!");
        MESSAGE("상대 마자용은 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 3-16 UPSTREAM CHANGE EXPECTED (part-C 5.2): Magic Room end loop stops after White Herb, foe Sitrus waits for next turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_WHITE_HERB); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_MAGIC_ROOM); }
        TURN { MOVE(player, MOVE_SUPER_FANG); MOVE(opponent, MOVE_GROWL); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("마자용의 공격이 떨어졌다!");
        MESSAGE("매직룸이 해제되어 도구의 효과가 원래대로 되돌아왔다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("마자용은 하양허브로 상태를 원래대로 되돌렸다!");
        // HnS before #9680: Execute(WhiteHerbEnd2) ends the Magic Room loop; the Sitrus Berry activates
        // only before the next action. After #9680 (upstream Call) it activates inside the loop.
        MESSAGE("마자용은 축하를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        MESSAGE("상대 마자는 자뭉열매로 체력을 회복했다!");
        HP_BAR(opponent, damage: -30);
    }
}
