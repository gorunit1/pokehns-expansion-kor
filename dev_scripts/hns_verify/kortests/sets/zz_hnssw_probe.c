#include "global.h"
#include "test/battle.h"

// review-switch scratch probe only (never committed). Minimal SCENE so the decoded trace runs to the end.

// S1: two Future Sights land in the same end turn. The faster foe Emergency Exit mon is hit first and leaves,
// then the second Future Sight script ends with `clearspecialstatuses` (memset gSpecialStatuses -> queuedSwitch).
SINGLE_BATTLE_TEST("HNSSW S1 foe EE from Future Sight, then the second Future Sight of the same end turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { SEND_OUT(opponent, 1); }
        TURN { }
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[1], 1);
    }
}

// S1p: player side of S1.
SINGLE_BATTLE_TEST("HNSSW S1p player EE from Future Sight, then the second Future Sight of the same end turn")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { SEND_OUT(player, 1); }
        TURN { }
    } SCENE {
        MESSAGE("갑주무사는 미래예지를 썼다!");
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[0], 1);
    }
}

// S1ai: S1 with the in-game trainer AI as the opponent.
AI_SINGLE_BATTLE_TEST("HNSSW S1ai foe EE from Future Sight (trainer AI), then the second Future Sight")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_FUTURE_SIGHT, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); Moves(MOVE_FUTURE_SIGHT); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); EXPECT_MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[1], 1);
    }
}

// S2: trainer AI, faster player EE (poison) leaves, then the last foe faints from poison; the mon in its ball
// gets the experience and levels up (P1 with the in-game controller).
AI_SINGLE_BATTLE_TEST("HNSSW S2 player EE (poison) then the last foe faints: experience and level-up for the mon in its ball")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); Level(5); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Level(50); HP(1); Status1(STATUS1_POISON); Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("갑주무사는 축하를 썼다!");
    } THEN {
        EXPECT_GT(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 5);
    }
}

// S3: trainer AI, mirrored P6: the foe hits the player's Eject Button holder (it leaves), then faints from Life Orb.
AI_SINGLE_BATTLE_TEST("HNSSW S3 player Eject Button leaves, the last foe faints from Life Orb: KO animation and experience")
{
    GIVEN {
        PLAYER(SPECIES_WYNAUT) { Level(30); Item(ITEM_EJECT_BUTTON); Speed(10); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); HP(5); Item(ITEM_LIFE_ORB); Speed(50); Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("상대 마자용은 할퀴기를 썼다!");
    }
}

// S4: trainer AI, foe EE (poison, faster) leaves and the player's last mon faints from poison (whiteout).
AI_SINGLE_BATTLE_TEST("HNSSW S4 foe EE (poison) then the player's last mon faints (trainer AI)")
{
    GIVEN {
        PLAYER(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자는 축하를 썼다!");
    }
}

// S5: trainer AI doubles, two foe EE in one Sandstorm with a single bench mon.
AI_DOUBLE_BATTLE_TEST("HNSSW S5 two foe EE in one Sandstorm, one bench mon (trainer AI)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_SANDSTORM, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(40); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SANDSTORM); MOVE(playerRight, MOVE_CELEBRATE); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자용은 모래바람을 썼다!");
    }
}

// S6: multi with the in-game partner (Lance/Silver style): partner mon and opponent B mon EE in the same Sandstorm.
AI_MULTI_BATTLE_TEST("HNSSW S6 multi: partner EE and opponent B EE in one Sandstorm")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_SANDSTORM, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(40); Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SANDSTORM); EXPECT_SEND_OUT(playerRight, 1); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자용은 모래바람을 썼다!");
    }
}

// S7: multi, the player's own mon EE at end of turn (party screen of the player in a multi).
AI_MULTI_BATTLE_TEST("HNSSW S7 multi: player EE in Sandstorm")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(50); Moves(MOVE_SANDSTORM, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_CELEBRATE); }
        PARTNER(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SANDSTORM); SEND_OUT(playerLeft, 1); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("갑주무사는 모래바람을 썼다!");
    }
}

// S8: two opponents (1v2), opponent B EE at end of turn.
AI_ONE_VS_TWO_BATTLE_TEST("HNSSW S8 two opponents: opponent B EE in Sandstorm")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_SANDSTORM, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_WOBBUFFET) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_A(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Speed(40); Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_WYNAUT) { Speed(10); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SANDSTORM); MOVE(playerRight, MOVE_CELEBRATE); }
        TURN { MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("마자용은 모래바람을 썼다!");
    }
}

// S9: doubles, player EE (burn) and the partner faints from poison in the same end of turn, one bench mon.
DOUBLE_BATTLE_TEST("HNSSW S9 player EE and the fainted partner share the only bench mon")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); Speed(40); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(playerLeft, 2); SEND_OUT(playerRight, 0); }
        TURN { }
    } SCENE {
        MESSAGE("마자는 독에 의한 데미지를 입고 있다!");
    }
}

// S10: wild, faster player EE (poison) flees before the wild mon faints from poison (P4 full output).
WILD_BATTLE_TEST("HNSSW S10 wild: faster player EE (poison) and the wild mon would faint from poison")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(10); }
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("갑주무사는 축하를 썼다!");
    }
}

// S11: Leech Seed + Liquid Ooze: the seeded last foe faints, the player's EE receiver drops below half.
SINGLE_BATTLE_TEST("HNSSW S11 Liquid Ooze drain: seeded last foe faints, EE receiver drops below half")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); MaxHP(400); HP(40); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); SEND_OUT(player, 1); }
    } SCENE {
        MESSAGE("갑주무사는 씨뿌리기를 썼다!");
    }
}

// S1t: S1 without the replacement: the foe mon left in its ball and the player attack each other on the next turn.
SINGLE_BATTLE_TEST("HNSSW S1t foe EE wiped by the second Future Sight: next turn both use Tackle")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_TACKLE); }
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
    }
}

// S1pt: player side of S1t.
SINGLE_BATTLE_TEST("HNSSW S1pt player EE wiped by the second Future Sight: next turn both use Tackle")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        MESSAGE("갑주무사는 미래예지를 썼다!");
    }
}

// S12: doubles, a single Future Sight on the EE mon and Doom Desire on another battler land in the same end turn.
DOUBLE_BATTLE_TEST("HNSSW S12 doubles: Future Sight on the foe EE mon, Doom Desire on the player's mon, same end turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FUTURE_SIGHT, target: opponentLeft); MOVE(opponentRight, MOVE_DOOM_DESIRE, target: playerRight); }
        TURN { }
        TURN { SEND_OUT(opponentLeft, 2); }
        TURN { }
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
    }
}

// S1k: the foe mon left in its ball after S1 is knocked out on the next turn.
SINGLE_BATTLE_TEST("HNSSW S1k foe EE wiped by the second Future Sight: the mon in its ball is knocked out next turn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); Attack(999); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
        TURN { MOVE(player, MOVE_TACKLE); SEND_OUT(opponent, 1); }
        TURN { }
    } SCENE {
        MESSAGE("마자용은 미래예지를 썼다!");
    }
}

// S1pk: player side of S1k.
SINGLE_BATTLE_TEST("HNSSW S1pk player EE wiped by the second Future Sight: the mon in its ball is knocked out next turn")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Speed(50); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); Attack(999); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
        TURN { MOVE(opponent, MOVE_TACKLE); SEND_OUT(player, 1); }
        TURN { }
    } SCENE {
        MESSAGE("갑주무사는 미래예지를 썼다!");
    }
}

// S13: doubles, both sides lose a mon to poison in the same end turn while the player's faster EE mon waits in its ball.
DOUBLE_BATTLE_TEST("HNSSW S13 doubles: player EE (poison) waits, the player's partner and a foe faint from poison")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(60); }
        PLAYER(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(50); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(playerLeft, 2); SEND_OUT(playerRight, 0); SEND_OUT(opponentLeft, 2); }
        TURN { }
    } SCENE {
        MESSAGE("갑주무사는 독에 의한 데미지를 입고 있다!");
    }
}

// S14: doubles, the player's faster EE mon waits in its ball and both foes (no bench) faint from poison.
DOUBLE_BATTLE_TEST("HNSSW S14 doubles: player EE (poison) waits, both last foes faint from poison")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(50); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Status1(STATUS1_POISON); Speed(30); }
    } WHEN {
        TURN { SEND_OUT(playerLeft, 2); }
    } SCENE {
        MESSAGE("갑주무사는 독에 의한 데미지를 입고 있다!");
    }
}

// S15: singles, the player's faster EE mon waits in its ball and a foe that still has a bench mon faints from poison.
SINGLE_BATTLE_TEST("HNSSW S15 singles: player EE (poison) waits, a non-last foe faints from poison")
{
    GIVEN {
        PLAYER(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(140); Status1(STATUS1_POISON); Speed(60); }
        PLAYER(SPECIES_WYNAUT) { Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { HP(1); Status1(STATUS1_POISON); Speed(40); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
        TURN { }
    } SCENE {
        MESSAGE("갑주무사는 독에 의한 데미지를 입고 있다!");
    }
}
