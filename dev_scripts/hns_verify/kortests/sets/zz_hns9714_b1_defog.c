#include "global.h"
#include "test/battle.h"

// seq 139 #9714 area B: Korean output regression set, part 1 (Defog).
// Scratch only, never committed. Expected output = HnS before #9714 (code f3a58f9939).
// A space in MESSAGE matches one space or newline of the real text.
// Friend requests A (Tailwind start text) and B (Grassy Terrain heal text) are being changed elsewhere:
// those two texts are never expected here, and no test lets Grassy Terrain heal anybody.

SINGLE_BATTLE_TEST("HNS9714 1-01 Defog by the player removes each foe-side screen: Reflect, Light Screen, Mist, Aurora Veil, Safeguard")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_REFLECT; }
    PARAMETRIZE { move = MOVE_LIGHT_SCREEN; }
    PARAMETRIZE { move = MOVE_MIST; }
    PARAMETRIZE { move = MOVE_AURORA_VEIL; }
    PARAMETRIZE { move = MOVE_SAFEGUARD; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SNOWSCAPE); }
        TURN { MOVE(opponent, move); }
        TURN { MOVE(player, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("마자용은 안개제거를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, player);
        if (move != MOVE_MIST)
            MESSAGE("상대 마자의 회피율이 떨어졌다!");
        if (move == MOVE_REFLECT)
            MESSAGE("상대의 리플렉터가 없어졌다!");
        else if (move == MOVE_LIGHT_SCREEN)
            MESSAGE("상대의 빛의장막이 없어졌다!");
        else if (move == MOVE_MIST)
            MESSAGE("상대를 감싸던 흰안개가 없어졌다!");
        else if (move == MOVE_AURORA_VEIL)
            MESSAGE("상대의 오로라베일이 없어졌다!");
        else
            MESSAGE("상대를 감싸던 신비의 베일이 없어졌다!");
        MESSAGE("상대 마자는 축하를 썼다!");
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-02 Defog by the foe removes each player-side screen: Reflect, Light Screen, Mist, Aurora Veil, Safeguard")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_REFLECT; }
    PARAMETRIZE { move = MOVE_LIGHT_SCREEN; }
    PARAMETRIZE { move = MOVE_MIST; }
    PARAMETRIZE { move = MOVE_AURORA_VEIL; }
    PARAMETRIZE { move = MOVE_SAFEGUARD; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SNOWSCAPE); }
        TURN { MOVE(player, move); }
        TURN { MOVE(opponent, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("상대 마자는 안개제거를 썼다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, opponent);
        if (move != MOVE_MIST)
            MESSAGE("마자용의 회피율이 떨어졌다!");
        if (move == MOVE_REFLECT)
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        else if (move == MOVE_LIGHT_SCREEN)
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
        else if (move == MOVE_MIST)
            MESSAGE("우리 편을 감싸던 흰안개가 없어졌다!");
        else if (move == MOVE_AURORA_VEIL)
            MESSAGE("우리 편의 오로라베일이 없어졌다!");
        else
            MESSAGE("우리 편을 감싸던 신비의 베일이 없어졌다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9714 1-03 Defog (left or right player) removes all five foe-side screens in order, user's own screens stay")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SNOWSCAPE); MOVE(opponentRight, MOVE_SAFEGUARD); MOVE(playerLeft, MOVE_REFLECT); }
        TURN { MOVE(opponentLeft, MOVE_AURORA_VEIL); MOVE(opponentRight, MOVE_REFLECT); }
        TURN { MOVE(opponentLeft, MOVE_LIGHT_SCREEN); MOVE(opponentRight, MOVE_MIST);
               MOVE(user == 0 ? playerLeft : playerRight, MOVE_DEFOG, target: opponentLeft); }
    } SCENE {
        if (user == 0)
            MESSAGE("마자용은 안개제거를 썼다!");
        else
            MESSAGE("마자는 안개제거를 썼다!");
        MESSAGE("상대의 리플렉터가 없어졌다!");
        MESSAGE("상대의 빛의장막이 없어졌다!");
        MESSAGE("상대를 감싸던 흰안개가 없어졌다!");
        MESSAGE("상대의 오로라베일이 없어졌다!");
        MESSAGE("상대를 감싸던 신비의 베일이 없어졌다!");
        NONE_OF { MESSAGE("우리 편의 리플렉터가 없어졌다!"); }
    }
}

DOUBLE_BATTLE_TEST("HNS9714 1-04 Defog (left or right foe) removes all five player-side screens in order, user's own screens stay")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SNOWSCAPE); MOVE(playerRight, MOVE_SAFEGUARD); MOVE(opponentLeft, MOVE_REFLECT); }
        TURN { MOVE(playerLeft, MOVE_AURORA_VEIL); MOVE(playerRight, MOVE_REFLECT); }
        TURN { MOVE(playerLeft, MOVE_LIGHT_SCREEN); MOVE(playerRight, MOVE_MIST);
               MOVE(user == 0 ? opponentLeft : opponentRight, MOVE_DEFOG, target: playerRight); }
    } SCENE {
        if (user == 0)
            MESSAGE("상대 마자용은 안개제거를 썼다!");
        else
            MESSAGE("상대 마자는 안개제거를 썼다!");
        MESSAGE("우리 편의 리플렉터가 없어졌다!");
        MESSAGE("우리 편의 빛의장막이 없어졌다!");
        MESSAGE("우리 편을 감싸던 흰안개가 없어졌다!");
        MESSAGE("우리 편의 오로라베일이 없어졌다!");
        MESSAGE("우리 편을 감싸던 신비의 베일이 없어졌다!");
        NONE_OF { MESSAGE("상대의 리플렉터가 없어졌다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-05 Defog by the player removes each hazard from both sides: own side first")
{
    u32 hazard;
    PARAMETRIZE { hazard = 0; }
    PARAMETRIZE { hazard = 1; }
    PARAMETRIZE { hazard = 2; }
    PARAMETRIZE { hazard = 3; }
    PARAMETRIZE { hazard = 4; }
    if (hazard == 4) {
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_PLAYER);
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_OPPONENT);
    }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (hazard == 0)
            TURN { MOVE(player, MOVE_SPIKES); MOVE(opponent, MOVE_SPIKES); }
        else if (hazard == 1)
            TURN { MOVE(player, MOVE_TOXIC_SPIKES); MOVE(opponent, MOVE_TOXIC_SPIKES); }
        else if (hazard == 2)
            TURN { MOVE(player, MOVE_STEALTH_ROCK); MOVE(opponent, MOVE_STEALTH_ROCK); }
        else if (hazard == 3)
            TURN { MOVE(player, MOVE_STICKY_WEB); MOVE(opponent, MOVE_STICKY_WEB); }
        else
            TURN {}
        TURN { MOVE(player, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("마자용은 안개제거를 썼다!");
        MESSAGE("상대 마자의 회피율이 떨어졌다!");
        if (hazard == 0) {
            MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
        } else if (hazard == 1) {
            MESSAGE("우리 편 발밑의 독압정이 사라졌다!");
            MESSAGE("상대 발밑의 독압정이 사라졌다!");
        } else if (hazard == 2) {
            MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
            MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        } else if (hazard == 3) {
            MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!");
            MESSAGE("상대 발밑의 끈적끈적네트가 사라졌다!");
        } else {
            MESSAGE("우리 편 주변의 강철이 사라졌다!");
            MESSAGE("상대 주변의 강철이 사라졌다!");
        }
        MESSAGE("상대 마자는 축하를 썼다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-06 Defog by the foe removes each hazard from both sides: player side first")
{
    u32 hazard;
    PARAMETRIZE { hazard = 0; }
    PARAMETRIZE { hazard = 1; }
    PARAMETRIZE { hazard = 2; }
    PARAMETRIZE { hazard = 3; }
    PARAMETRIZE { hazard = 4; }
    if (hazard == 4) {
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_PLAYER);
        SetStartingStatus(STARTING_STATUS_SHARP_STEEL_OPPONENT);
    }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (hazard == 0)
            TURN { MOVE(player, MOVE_SPIKES); MOVE(opponent, MOVE_SPIKES); }
        else if (hazard == 1)
            TURN { MOVE(player, MOVE_TOXIC_SPIKES); MOVE(opponent, MOVE_TOXIC_SPIKES); }
        else if (hazard == 2)
            TURN { MOVE(player, MOVE_STEALTH_ROCK); MOVE(opponent, MOVE_STEALTH_ROCK); }
        else if (hazard == 3)
            TURN { MOVE(player, MOVE_STICKY_WEB); MOVE(opponent, MOVE_STICKY_WEB); }
        else
            TURN {}
        TURN { MOVE(opponent, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("상대 마자는 안개제거를 썼다!");
        MESSAGE("마자용의 회피율이 떨어졌다!");
        if (hazard == 0) {
            MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
        } else if (hazard == 1) {
            MESSAGE("우리 편 발밑의 독압정이 사라졌다!");
            MESSAGE("상대 발밑의 독압정이 사라졌다!");
        } else if (hazard == 2) {
            MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
            MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        } else if (hazard == 3) {
            MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!");
            MESSAGE("상대 발밑의 끈적끈적네트가 사라졌다!");
        } else {
            MESSAGE("우리 편 주변의 강철이 사라졌다!");
            MESSAGE("상대 주변의 강철이 사라졌다!");
        }
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-07 Defog removes all five hazards from both sides in table order (player or foe user)")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    SetStartingStatus(STARTING_STATUS_SPIKES_PLAYER_L1);
    SetStartingStatus(STARTING_STATUS_SPIKES_OPPONENT_L1);
    SetStartingStatus(STARTING_STATUS_TOXIC_SPIKES_PLAYER_L1);
    SetStartingStatus(STARTING_STATUS_TOXIC_SPIKES_OPPONENT_L1);
    SetStartingStatus(STARTING_STATUS_STICKY_WEB_PLAYER);
    SetStartingStatus(STARTING_STATUS_STICKY_WEB_OPPONENT);
    SetStartingStatus(STARTING_STATUS_STEALTH_ROCK_PLAYER);
    SetStartingStatus(STARTING_STATUS_STEALTH_ROCK_OPPONENT);
    SetStartingStatus(STARTING_STATUS_SHARP_STEEL_PLAYER);
    SetStartingStatus(STARTING_STATUS_SHARP_STEEL_OPPONENT);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (user == 0)
            TURN { MOVE(player, MOVE_DEFOG); }
        else
            TURN { MOVE(opponent, MOVE_DEFOG); }
    } SCENE {
        if (user == 0)
            MESSAGE("마자용은 안개제거를 썼다!");
        else
            MESSAGE("상대 마자는 안개제거를 썼다!");
        MESSAGE("우리 편 발밑의 압정이 사라졌다!");
        MESSAGE("우리 편 발밑의 끈적끈적네트가 사라졌다!");
        MESSAGE("우리 편 발밑의 독압정이 사라졌다!");
        MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
        MESSAGE("우리 편 주변의 강철이 사라졌다!");
        MESSAGE("상대 발밑의 압정이 사라졌다!");
        MESSAGE("상대 발밑의 끈적끈적네트가 사라졌다!");
        MESSAGE("상대 발밑의 독압정이 사라졌다!");
        MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        MESSAGE("상대 주변의 강철이 사라졌다!");
    } THEN {
        ResetStartingStatuses();
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-08 Defog removes each terrain: text and background animation (player or foe user)")
{
    enum Move move;
    u32 user;
    PARAMETRIZE { move = MOVE_ELECTRIC_TERRAIN; user = 0; }
    PARAMETRIZE { move = MOVE_MISTY_TERRAIN;    user = 0; }
    PARAMETRIZE { move = MOVE_PSYCHIC_TERRAIN;  user = 0; }
    PARAMETRIZE { move = MOVE_GRASSY_TERRAIN;   user = 0; }
    PARAMETRIZE { move = MOVE_ELECTRIC_TERRAIN; user = 1; }
    PARAMETRIZE { move = MOVE_MISTY_TERRAIN;    user = 1; }
    PARAMETRIZE { move = MOVE_PSYCHIC_TERRAIN;  user = 1; }
    PARAMETRIZE { move = MOVE_GRASSY_TERRAIN;   user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (user == 0) {
            TURN { MOVE(opponent, move); }
            TURN { MOVE(player, MOVE_DEFOG); }
        } else {
            TURN { MOVE(player, move); }
            TURN { MOVE(opponent, MOVE_DEFOG); }
        }
    } SCENE {
        if (user == 0) {
            MESSAGE("마자용은 안개제거를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, player);
            MESSAGE("상대 마자의 회피율이 떨어졌다!");
        } else {
            MESSAGE("상대 마자는 안개제거를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, opponent);
            MESSAGE("마자용의 회피율이 떨어졌다!");
        }
        if (move == MOVE_ELECTRIC_TERRAIN)
            MESSAGE("발밑의 전기가 사라졌다!");
        else if (move == MOVE_MISTY_TERRAIN)
            MESSAGE("발밑의 안개가 사라졌다!");
        else if (move == MOVE_PSYCHIC_TERRAIN)
            MESSAGE("발밑의 이상한 느낌이 사라졌다!");
        else
            MESSAGE("발밑의 풀이 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, player);
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-09 Defog order with mixed effects: hazards and screens per side, terrain between the sides")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_STEALTH_ROCK); MOVE(opponent, MOVE_SPIKES); }
        if (user == 0) {
            TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); MOVE(opponent, MOVE_REFLECT); }
            TURN { MOVE(player, MOVE_DEFOG); }
        } else {
            TURN { MOVE(player, MOVE_REFLECT); MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
            TURN { MOVE(opponent, MOVE_DEFOG); }
        }
    } SCENE {
        if (user == 0) {
            MESSAGE("마자용은 안개제거를 썼다!");
            MESSAGE("상대 마자의 회피율이 떨어졌다!");
            MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            MESSAGE("발밑의 전기가 사라졌다!");
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        } else {
            MESSAGE("상대 마자는 안개제거를 썼다!");
            MESSAGE("마자용의 회피율이 떨어졌다!");
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            MESSAGE("발밑의 전기가 사라졌다!");
            MESSAGE("상대 주변의 스텔스록이 사라졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-10 Defog against a Substitute (Gen5+): no evasion drop, screens and hazards still go")
{
    u32 user;
    PARAMETRIZE { user = 0; }
    PARAMETRIZE { user = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(user == 0 ? 1 : 2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(user == 0 ? 2 : 1); }
    } WHEN {
        if (user == 0) {
            TURN { MOVE(opponent, MOVE_LIGHT_SCREEN); MOVE(player, MOVE_SPIKES); }
            TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_DEFOG); }
        } else {
            TURN { MOVE(player, MOVE_LIGHT_SCREEN); MOVE(opponent, MOVE_SPIKES); }
            TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, MOVE_DEFOG); }
        }
        TURN {}
    } SCENE {
        if (user == 0) {
            MESSAGE("상대 마자의 대타가 나타났다");
            MESSAGE("마자용은 안개제거를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, player);
            NONE_OF { MESSAGE("상대 마자의 회피율이 떨어졌다!"); }
            MESSAGE("상대의 빛의장막이 없어졌다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
            MESSAGE("상대 마자는 축하를 썼다!");
            MESSAGE("마자용은 축하를 썼다!");
        } else {
            MESSAGE("마자용의 대타가 나타났다");
            MESSAGE("상대 마자는 안개제거를 썼다!");
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, opponent);
            NONE_OF { MESSAGE("마자용의 회피율이 떨어졌다!"); }
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
            MESSAGE("우리 편 발밑의 압정이 사라졌다!");
            MESSAGE("마자용은 축하를 썼다!");
            MESSAGE("상대 마자는 축하를 썼다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-11 UPSTREAM CHANGE EXPECTED: foe Defog against a Substitute with only terrain to clear (attacker overwritten before #9714)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, MOVE_REFLECT); }
        TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); MOVE(opponent, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("마자용의 대타가 나타났다");
        MESSAGE("상대 마자는 안개제거를 썼다!");
        // Before #9714 the check pass (trydefog FALSE) finds only the terrain and returns with gBattlerAttacker
        // still set to side 0, so the rest of the move runs with the player as the attacker:
        // the Defog animation starts from the player, the evasion drop goes through the player's substitute,
        // and the clear pass (user = side 0) also removes the foe's OWN Reflect.
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, player);
        MESSAGE("마자용의 회피율이 떨어졌다!");
        MESSAGE("발밑의 전기가 사라졌다!");
        // (no B_ANIM_RESTORE_BG: the animation battler is side 0 = the player, who is behind a substitute)
        MESSAGE("상대의 리플렉터가 없어졌다!");
    } THEN {
        // Before #9714 the move end also runs with the player as the attacker: the player's last move becomes
        // Defog too (it used Electric Terrain), and the foe's own Reflect is gone.
        EXPECT_EQ(gLastMoves[B_POSITION_PLAYER_LEFT], MOVE_DEFOG);
        EXPECT_EQ(gLastMoves[B_POSITION_OPPONENT_LEFT], MOVE_DEFOG);
        EXPECT_EQ(gSideStatuses[B_SIDE_OPPONENT] & SIDE_STATUS_REFLECT, 0);
        EXPECT_EQ(player->statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 1);
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-12 Defog removes Electric Terrain: Booster Energy activates Quark Drive afterwards")
{
    GIVEN {
        PLAYER(SPECIES_IRON_MOTH) { Attack(100); Defense(100); Speed(100); SpAttack(110); SpDefense(100); Ability(ABILITY_QUARK_DRIVE); Item(ITEM_BOOSTER_ENERGY); }
        OPPONENT(SPECIES_TAPU_KOKO) { Speed(1); Ability(ABILITY_ELECTRIC_SURGE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_DEFOG); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_ELECTRIC_SURGE);
        ABILITY_POPUP(player, ABILITY_QUARK_DRIVE);
        MESSAGE("상대 카푸꼬꼬꼭은 안개제거를 썼다!");
        MESSAGE("발밑의 전기가 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        ABILITY_POPUP(player, ABILITY_QUARK_DRIVE);
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-13 Defog reflected by Magic Bounce or Magic Coat: the reflector's Defog clears the original user's side")
{
    u32 mode;
    PARAMETRIZE { mode = 0; } // Magic Bounce
    PARAMETRIZE { mode = 1; } // Magic Coat
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_ESPEON) { Speed(2); Ability(mode == 0 ? ABILITY_MAGIC_BOUNCE : ABILITY_SYNCHRONIZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_REFLECT); MOVE(opponent, MOVE_LIGHT_SCREEN); }
        TURN { MOVE(opponent, MOVE_STEALTH_ROCK); }
        TURN { MOVE(opponent, MOVE_SPIKES); }
        if (mode == 0)
            TURN { MOVE(player, MOVE_DEFOG); }
        else
            TURN { MOVE(opponent, MOVE_MAGIC_COAT); MOVE(player, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("마자용은 안개제거를 썼다!");
        if (mode == 0)
            ABILITY_POPUP(opponent, ABILITY_MAGIC_BOUNCE);
        MESSAGE("마자용의 회피율이 떨어졌다!");
        MESSAGE("우리 편의 리플렉터가 없어졌다!");
        MESSAGE("우리 편 발밑의 압정이 사라졌다!");
        MESSAGE("우리 편 주변의 스텔스록이 사라졌다!");
        NONE_OF { MESSAGE("상대의 빛의장막이 없어졌다!"); }
    } THEN {
        EXPECT_EQ(gSideStatuses[B_SIDE_OPPONENT] & SIDE_STATUS_LIGHTSCREEN, SIDE_STATUS_LIGHTSCREEN);
        EXPECT_EQ(gSideStatuses[B_SIDE_PLAYER] & SIDE_STATUS_REFLECT, 0);
    }
}

SINGLE_BATTLE_TEST("HNS9714 1-14 Defog check pass against minimum evasion: nothing (fails), only the user's hazards (works), only the user's own Reflect (fails)")
{
    u32 mode;
    PARAMETRIZE { mode = 0; }
    PARAMETRIZE { mode = 1; }
    PARAMETRIZE { mode = 2; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Ability(ABILITY_SIMPLE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_DEFOG); }
        TURN { MOVE(opponent, MOVE_DEFOG); }
        TURN { MOVE(opponent, MOVE_DEFOG); }
        if (mode == 1)
            TURN { MOVE(player, MOVE_SPIKES); }
        else if (mode == 2)
            TURN { MOVE(opponent, MOVE_REFLECT); }
        else
            TURN {}
        TURN { MOVE(opponent, MOVE_DEFOG); }
    } SCENE {
        MESSAGE("상대 마자는 안개제거를 썼다!");
        MESSAGE("상대 마자는 안개제거를 썼다!");
        MESSAGE("상대 마자는 안개제거를 썼다!");
        MESSAGE("상대 마자는 안개제거를 썼다!");
        if (mode == 1) {
            MESSAGE("마자용의 회피율은 더 떨어지지 않는다!");
            MESSAGE("상대 발밑의 압정이 사라졌다!");
        } else {
            MESSAGE("그러나 실패하고 말았다!");
            NONE_OF { MESSAGE("상대의 리플렉터가 없어졌다!"); }
        }
    } THEN {
        EXPECT_EQ(player->statStages[STAT_EVASION], MIN_STAT_STAGE);
    }
}
