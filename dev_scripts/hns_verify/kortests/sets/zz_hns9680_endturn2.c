#include "global.h"
#include "test/battle.h"

// seq 132 #9680 area D: end-of-turn output regression set, part 2 (weather, residual damage/heal, delayed effects).
// Scratch only, never committed. Expected output = HnS before #9680 (HEAD 18f412e9ff, code 587f4e7cdc).
// A space in MESSAGE matches one space or newline of the real text.

SINGLE_BATTLE_TEST("HNS9680 2-01 Rain continues each turn and stops after five turns")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_RAIN_DANCE); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("비가 내리기 시작했다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("비가 내리고 있다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES);
        MESSAGE("비가 내리고 있다!");
        MESSAGE("비가 내리고 있다!");
        MESSAGE("비가 내리고 있다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("비가 그쳤다!");
        NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES); }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-02 Sandstorm damage by speed order, then subsides")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("모래바람이 불기 시작했다!");
        MESSAGE("모래바람이 세차게 분다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SANDSTORM_CONTINUES);
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        HP_BAR(player, damage: 30);
        MESSAGE("모래바람이 상대 마자를 덮쳤다!");
        HP_BAR(opponent, damage: 18);
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 세차게 분다!");
        MESSAGE("모래바람이 마자용을 덮쳤다!");
        MESSAGE("모래바람이 상대 마자를 덮쳤다!");
        MESSAGE("모래바람이 가라앉았다!");
        NONE_OF { MESSAGE("모래바람이 마자용을 덮쳤다!"); }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-03 Hail damage and end, sun end, snow end texts")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SUNNY_DAY; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, move); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (move == MOVE_HAIL) {
            MESSAGE("눈이 내리기 시작했다!");
            MESSAGE("눈이 내리고 있다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HAIL_CONTINUES);
            MESSAGE("싸라기눈이 상대 마자를 덮쳤다!");
            HP_BAR(opponent, damage: 18);
            MESSAGE("싸라기눈이 마자용을 덮쳤다!");
            HP_BAR(player, damage: 30);
            MESSAGE("눈이 내리고 있다!");
            MESSAGE("눈이 내리고 있다!");
            MESSAGE("눈이 내리고 있다!");
            MESSAGE("싸라기눈이 마자용을 덮쳤다!");
            MESSAGE("눈이 그쳤다!");
        } else if (move == MOVE_SUNNY_DAY) {
            MESSAGE("햇살이 강해졌다!");
            MESSAGE("햇살이 강하다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUN_CONTINUES);
            MESSAGE("햇살이 강하다!");
            MESSAGE("햇살이 강하다!");
            MESSAGE("햇살이 강하다!");
            MESSAGE("햇살이 약해졌다!");
        } else {
            MESSAGE("눈이 내리기 시작했다!");
            MESSAGE("눈이 내리고 있다.");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SNOW_CONTINUES);
            NONE_OF { MESSAGE("싸라기눈이 마자용을 덮쳤다!"); }
            MESSAGE("눈이 내리고 있다.");
            MESSAGE("눈이 내리고 있다.");
            MESSAGE("눈이 내리고 있다.");
            MESSAGE("눈이 그쳤다.");
        }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-04 Poison, Toxic, Burn, Frostbite damage: text, status animation, HP bar")
{
    u32 status;
    PARAMETRIZE { status = STATUS1_POISON; }
    PARAMETRIZE { status = STATUS1_TOXIC_POISON; }
    PARAMETRIZE { status = STATUS1_BURN; }
    PARAMETRIZE { status = STATUS1_FROSTBITE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Status1(status); }
        OPPONENT(SPECIES_WYNAUT) { Status1(status); }
    } WHEN {
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 축하를 썼다!");
        if (status == STATUS1_POISON || status == STATUS1_TOXIC_POISON) {
            MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, player);
            HP_BAR(player, damage: status == STATUS1_POISON ? 61 : 30);
            MESSAGE("상대 마자는 독에 의한 데미지를 입고 있다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
            HP_BAR(opponent, damage: status == STATUS1_POISON ? 37 : 18);
            MESSAGE("마자용은 독에 의한 데미지를 입고 있다!");
            HP_BAR(player, damage: status == STATUS1_POISON ? 61 : 60);
        } else if (status == STATUS1_BURN) {
            MESSAGE("마자용은 화상 데미지를 입고 있다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, player);
            HP_BAR(player, damage: 30);
            MESSAGE("상대 마자는 화상 데미지를 입고 있다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
            HP_BAR(opponent, damage: 18);
        } else {
            MESSAGE("마자용은 동상 데미지를 입고 있다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_FRB, player);
            HP_BAR(player, damage: 30);
            MESSAGE("상대 마자는 동상 데미지를 입고 있다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_FRB, opponent);
            HP_BAR(opponent, damage: 18);
        }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-05 Leech Seed: drain, Liquid Ooze, receiver under Heal Block")
{
    u32 mode;
    PARAMETRIZE { mode = 0; }
    PARAMETRIZE { mode = 1; }
    PARAMETRIZE { mode = 2; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); if (mode == 1) Ability(ABILITY_LIQUID_OOZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); MOVE(opponent, mode == 2 ? MOVE_HEAL_BLOCK : MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자에게 씨앗을 심었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_LEECH_SEED_DRAIN, opponent);
        if (mode == 0) {
            HP_BAR(opponent, damage: 37);
            HP_BAR(player, damage: -37);
            MESSAGE("씨뿌리기가 상대 마자의 체력을 빼앗는다!");
        } else if (mode == 1) {
            HP_BAR(opponent, damage: 37);
            ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
            HP_BAR(player, damage: 37);
            MESSAGE("마자용은 해감액을 흡수했다!");
        } else {
            HP_BAR(opponent, hp: 300);
            NONE_OF {
                HP_BAR(player);
                MESSAGE("씨뿌리기가 상대 마자의 체력을 빼앗는다!");
            }
        }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-06 Wrap: turn damage five times, then freed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_WRAP); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 마자용에게 휘감겼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TURN_TRAP, opponent);
        MESSAGE("상대 마자는 김밥말이의 데미지를 입고 있다");
        HP_BAR(opponent, damage: 37);
        MESSAGE("상대 마자는 김밥말이의 데미지를 입고 있다");
        MESSAGE("상대 마자는 김밥말이의 데미지를 입고 있다");
        MESSAGE("상대 마자는 김밥말이의 데미지를 입고 있다");
        MESSAGE("상대 마자는 김밥말이의 데미지를 입고 있다");
        HP_BAR(opponent, damage: 37);
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("상대 마자는 김밥말이로부터 풀려났다!");
        NONE_OF { HP_BAR(opponent); }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-07 Nightmare and Curse damage")
{
    GIVEN {
        PLAYER(SPECIES_GENGAR) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Status1(STATUS1_SLEEP_TURN(3)); Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_CURSE); }
        TURN { MOVE(player, MOVE_NIGHTMARE); }
    } SCENE {
        MESSAGE("팬텀은 자신의 체력을 깎아서 상대 마자에게 저주를 걸었다!");
        MESSAGE("상대 마자는 저주받고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CURSED, opponent);
        HP_BAR(opponent, damage: 75);
        MESSAGE("상대 마자는 악몽을 꾸기 시작했다!");
        MESSAGE("상대 마자는 악몽에 시달리고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_NIGHTMARE, opponent);
        HP_BAR(opponent, damage: 75);
        MESSAGE("상대 마자는 저주받고 있다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CURSED, opponent);
        HP_BAR(opponent, damage: 75);
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-08 Salt Cure, Octolock and Syrup Bomb end-of-turn effects")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_SALT_CURE; }
    PARAMETRIZE { move = MOVE_OCTOLOCK; }
    PARAMETRIZE { move = MOVE_SYRUP_BOMB; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, move); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (move == MOVE_SALT_CURE) {
            MESSAGE("상대 마자는 소금에 절여졌다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SALT_CURE_DAMAGE, opponent);
            HP_BAR(opponent, damage: 37);
            MESSAGE("상대 마자는 소금절이의 데미지를 입고 있다.");
            MESSAGE("상대 마자는 소금절이의 데미지를 입고 있다.");
            MESSAGE("상대 마자는 소금절이의 데미지를 입고 있다.");
            MESSAGE("상대 마자는 소금절이의 데미지를 입고 있다.");
        } else if (move == MOVE_OCTOLOCK) {
            MESSAGE("상대 마자는 문어굳히기 때문에 도망칠 수 없게 되었다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            MESSAGE("상대 마자의 방어가 떨어졌다!");
            MESSAGE("상대 마자의 특수방어가 떨어졌다!");
            MESSAGE("상대 마자는 축하를 썼다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            MESSAGE("상대 마자의 방어가 떨어졌다!");
            MESSAGE("상대 마자의 특수방어가 떨어졌다!");
        } else {
            MESSAGE("상대 마자는 물엿범벅이 되었다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
            MESSAGE("상대 마자의 스피드가 떨어졌다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent);
            MESSAGE("상대 마자의 스피드가 떨어졌다!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent);
            MESSAGE("상대 마자의 스피드가 떨어졌다!");
            MESSAGE("마자용은 축하를 썼다!");
            MESSAGE("상대 마자는 축하를 썼다!");
            NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SYRUP_BOMB_SPEED_DROP, opponent); }
        }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-09 Aqua Ring and Ingrain heal")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); }
        OPPONENT(SPECIES_WYNAUT) { HP(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_AQUA_RING); MOVE(opponent, MOVE_INGRAIN); }
    } SCENE {
        MESSAGE("상대 마자는 뿌리를 뻗었다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_AQUA_RING_HEAL, player);
        MESSAGE("마자용은 물의 고리로 체력을 회복했다!");
        HP_BAR(player, damage: -30);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_INGRAIN_HEAL, opponent);
        MESSAGE("상대 마자는 뿌리로부터 양분을 흡수했다!");
        HP_BAR(opponent, damage: -18);
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-10 Grassy Terrain heals each turn, then ends")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); }
        OPPONENT(SPECIES_WYNAUT) { HP(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("발밑에 풀이 무성해졌다!");
        MESSAGE("마자용의 체력이 회복되었다!");
        HP_BAR(player, damage: -30);
        MESSAGE("상대 마자의 체력이 회복되었다!");
        HP_BAR(opponent, damage: -18);
        MESSAGE("마자용의 체력이 회복되었다!");
        MESSAGE("마자용의 체력이 회복되었다!");
        MESSAGE("마자용의 체력이 회복되었다!");
        MESSAGE("마자용의 체력이 회복되었다!");
        MESSAGE("상대 마자의 체력이 회복되었다!");
        MESSAGE("발밑의 풀이 사라졌다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RESTORE_BG);
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-11 Wish comes true, Future Sight hits two turns later")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(100); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_WISH); MOVE(opponent, MOVE_FUTURE_SIGHT); }
        TURN {}
        TURN {}
    } SCENE {
        MESSAGE("상대 마자는 미래의 공격을 예지했다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_WISH_HEAL, player);
        MESSAGE("마자용의 희망사항이 이루어졌다!");
        HP_BAR(player, damage: -245);
        MESSAGE("마자용의 체력이 회복되었다!");
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용은 미래예지 공격을 받았다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FUTURE_SIGHT_HIT);
        HP_BAR(player);
        MESSAGE("효과가 별로인 듯하다.");
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-12 Wish at full HP and under Heal Block")
{
    u32 mode;
    PARAMETRIZE { mode = 0; }
    PARAMETRIZE { mode = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_WISH); MOVE(opponent, mode == 1 ? MOVE_HEAL_BLOCK : MOVE_CELEBRATE); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 축하를 썼다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        MESSAGE("마자용의 희망사항이 이루어졌다!");
        if (mode == 0)
            MESSAGE("그러나 마자용은 체력이 가득찬 상태다!");
        else
            MESSAGE("마자용은 회복봉인 때문에 -을 쓸 수 없다!");
        NONE_OF { HP_BAR(player); }
    }
}

DOUBLE_BATTLE_TEST("HNS9680 2-13 Pledge field effects: sea of fire damage and end, rainbow end, swamp end")
{
    u32 mode;
    PARAMETRIZE { mode = 0; }
    PARAMETRIZE { mode = 1; }
    PARAMETRIZE { mode = 2; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        if (mode == 0)
            TURN { MOVE(playerLeft, MOVE_FIRE_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_GRASS_PLEDGE, target: opponentLeft); }
        else if (mode == 1)
            TURN { MOVE(playerLeft, MOVE_WATER_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_FIRE_PLEDGE, target: opponentLeft); }
        else
            TURN { MOVE(playerLeft, MOVE_GRASS_PLEDGE, target: opponentLeft); MOVE(playerRight, MOVE_WATER_PLEDGE, target: opponentLeft); }
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (mode == 0) {
            MESSAGE("상대 주변이 불바다에 둘러싸였다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponentLeft);
            MESSAGE("상대 마자용은 불바다의 데미지를 입었다!");
            HP_BAR(opponentLeft, damage: 61);
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponentRight);
            MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
            HP_BAR(opponentRight, damage: 37);
            MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
            MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
            MESSAGE("상대 마자는 불바다의 데미지를 입었다!");
            MESSAGE("우리 편 주변의 불바다가 사라졌다!");
        } else if (mode == 1) {
            MESSAGE("우리 편 하늘에 무지개가 걸렸다!");
            MESSAGE("마자는 축하를 썼다!");
            MESSAGE("마자는 축하를 썼다!");
            MESSAGE("마자는 축하를 썼다!");
            MESSAGE("우리 편 하늘에서 무지개가 사라졌다!");
        } else {
            MESSAGE("상대 주변에 습지초원이 펼쳐졌다!");
            MESSAGE("마자는 축하를 썼다!");
            MESSAGE("마자는 축하를 썼다!");
            MESSAGE("마자는 축하를 썼다!");
            MESSAGE("상대 주변의 습지초원이 사라졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-14 Perish Song count, both faint, replacements")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_PERISH_SONG); }
        TURN {}
        TURN {}
        TURN { SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
    } SCENE {
        MESSAGE("멸망의노래를 들은 포켓몬은 3턴 후에 쓰러져 버린다!");
        MESSAGE("마자용의 멸망의 카운트가 3이 되었다!");
        MESSAGE("상대 마자의 멸망의 카운트가 3이 되었다!");
        MESSAGE("마자용의 멸망의 카운트가 2이 되었다!");
        MESSAGE("상대 마자의 멸망의 카운트가 2이 되었다!");
        MESSAGE("마자용의 멸망의 카운트가 1이 되었다!");
        MESSAGE("상대 마자의 멸망의 카운트가 1이 되었다!");
        MESSAGE("마자용의 멸망의 카운트가 0이 되었다!");
        HP_BAR(player, hp: 0);
        MESSAGE("마자용은 쓰러졌다!");
        MESSAGE("상대 마자의 멸망의 카운트가 0이 되었다!");
        HP_BAR(opponent, hp: 0);
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("가랏! 마자!");
        MESSAGE("2는 마자용을 내보냈다!");
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-15 Yawn: falls asleep, Electric/Misty Terrain prevent, Sweet Veil (HnS text)")
{
    u32 mode;
    PARAMETRIZE { mode = 0; }
    PARAMETRIZE { mode = 1; }
    PARAMETRIZE { mode = 2; }
    PARAMETRIZE { mode = 3; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); if (mode == 3) Ability(ABILITY_SWEET_VEIL); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_YAWN); }
        TURN { MOVE(player, mode == 1 ? MOVE_ELECTRIC_TERRAIN : mode == 2 ? MOVE_MISTY_TERRAIN : MOVE_CELEBRATE); MOVE(opponent, mode == 3 ? MOVE_SKILL_SWAP : MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("상대 마자의 졸음을 유도했다!");
        if (mode == 0) {
            MESSAGE("상대 마자는 축하를 썼다!");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_SLP, opponent);
            MESSAGE("상대 마자는 잠들어 버렸다!");
            STATUS_ICON(opponent, sleep: TRUE);
        } else if (mode == 1) {
            MESSAGE("발밑에 전기가 흐르기 시작했다!");
            MESSAGE("상대 마자는 일렉트릭필드가 지켜 주고 있다!");
            NONE_OF { MESSAGE("상대 마자는 잠들어 버렸다!"); }
        } else if (mode == 2) {
            MESSAGE("발밑이 안개로 자욱해졌다!");
            MESSAGE("상대 마자를 미스트필드가 지켜 주고 있다!");
            NONE_OF { MESSAGE("상대 마자는 잠들어 버렸다!"); }
        } else {
            MESSAGE("상대 마자는 서로의 특성을 교체했다!");
            ABILITY_POPUP(opponent, ABILITY_SWEET_VEIL);
            MESSAGE("상대 마자는 스위트베일 때문에 잠들지 않는다!");
            NONE_OF { MESSAGE("상대 마자는 잠들어 버렸다!"); }
        }
    }
}

DOUBLE_BATTLE_TEST("HNS9680 2-16 G-Max Wildfire residual damage at end of turn")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { GigantamaxFactor(TRUE); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EMBER, target: opponentLeft, gimmick: GIMMICK_DYNAMAX); }
        TURN {}
    } SCENE {
        MESSAGE("상대 포켓몬은 불꽃에 휩싸였다!");
        MESSAGE("상대 마자용은 거다이옥염의 불꽃에 휩싸여서 뜨거워하고 있다!");
        HP_BAR(opponentLeft, damage: 81);
        MESSAGE("상대 마자는 거다이옥염의 불꽃에 휩싸여서 뜨거워하고 있다!");
        HP_BAR(opponentRight, damage: 50);
        MESSAGE("리자몽은 방어 태세에 들어갔다!");
        MESSAGE("상대 마자용은 거다이옥염의 불꽃에 휩싸여서 뜨거워하고 있다!");
        HP_BAR(opponentLeft, damage: 81);
        MESSAGE("상대 마자는 거다이옥염의 불꽃에 휩싸여서 뜨거워하고 있다!");
        HP_BAR(opponentRight, damage: 50);
    }
}

SINGLE_BATTLE_TEST("HNS9680 2-17 Future Sight knocks out the foe at end of turn, replacement")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); }
        TURN {}
        TURN { SEND_OUT(opponent, 1); }
        TURN {}
    } SCENE {
        MESSAGE("마자용은 미래의 공격을 예지했다!");
        MESSAGE("상대 마자는 미래예지 공격을 받았다!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FUTURE_SIGHT_HIT);
        HP_BAR(opponent, hp: 0);
        MESSAGE("효과가 별로인 듯하다.");
        MESSAGE("상대 마자는 쓰러졌다!");
        MESSAGE("2는 마자용을 내보냈다!");
    }
}

DOUBLE_BATTLE_TEST("HNS9680 2-18 Yawn on both foes with Sleep Clause: one falls asleep, the other is kept awake")
{
    GIVEN {
        FLAG_SET(B_FLAG_SLEEP_CLAUSE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_YAWN, target: opponentLeft); MOVE(playerRight, MOVE_YAWN, target: opponentRight); }
        TURN {}
    } SCENE {
        MESSAGE("상대 마자용의 졸음을 유도했다!");
        MESSAGE("상대 마자의 졸음을 유도했다!");
        MESSAGE("상대 마자는 축하를 썼다!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_SLP, opponentLeft);
        MESSAGE("상대 마자용은 잠들어 버렸다!");
        STATUS_ICON(opponentLeft, sleep: TRUE);
        MESSAGE("Sleep Clause kept 상대 마자 awake!");
        NONE_OF { STATUS_ICON(opponentRight, sleep: TRUE); }
    }
}
