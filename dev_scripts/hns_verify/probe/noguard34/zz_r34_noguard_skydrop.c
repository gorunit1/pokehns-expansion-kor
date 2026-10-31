#include "global.h"
#include "test/battle.h"

// 재확인 34 조사용(test/에 커밋하지 않음 — dev_scripts/hns_verify/probe/noguard34/README.md). 명중 100 비교는 MOVE_DYNAMIC_PUNCH를 MOVE_TACKLE로 바꿔 돌렸다.
// playerLeft(마자용, 가장 빠름)가 프리폴로 opponentLeft를 들어 올린 턴에 다른 포켓몬이 공격.
// case 0: 노가드 공격자(상대 오른쪽 괴력몬) -> 프리폴 사용자(playerLeft)에게 다이나믹펀치(명중 50, 강제 빗나감)
// case 1: 노가드 공격자(상대 오른쪽 괴력몬) -> 들린 포켓몬(아군 opponentLeft)에게 다이나믹펀치(명중 50, 강제 빗나감)
// case 2: 노가드 공격자(플레이어 오른쪽 괴력몬) -> 들린 포켓몬(opponentLeft)에게 다이나믹펀치(명중 50, 강제 빗나감)
// case 3: 노가드 방어자 = 들린 포켓몬(opponentLeft 괴력몬), 일반 공격자(playerRight 마자) 다이나믹펀치
// case 4: 대조 — 노가드 없음, 일반 공격자(playerRight)가 들린 포켓몬에게 다이나믹펀치(빗나가야 함)
DOUBLE_BATTLE_TEST("ZZ r34: No Guard and Sky Drop")
{
    u32 c;
    PARAMETRIZE { c = 0; }
    PARAMETRIZE { c = 1; }
    PARAMETRIZE { c = 2; }
    PARAMETRIZE { c = 3; }
    PARAMETRIZE { c = 4; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SKY_DROP) == EFFECT_SKY_DROP);
        ASSUME(GetMoveAccuracy(MOVE_DYNAMIC_PUNCH) == 50);
        PLAYER(SPECIES_WOBBUFFET) { Speed(10); }
        PLAYER(c == 2 ? SPECIES_MACHAMP : SPECIES_WYNAUT) { Speed(5); Ability(c == 2 ? ABILITY_NO_GUARD : ABILITY_SHADOW_TAG); }
        OPPONENT(c == 3 ? SPECIES_MACHAMP : SPECIES_WOBBUFFET) { Speed(1); Ability(c == 3 ? ABILITY_NO_GUARD : ABILITY_SHADOW_TAG); }
        OPPONENT(SPECIES_MACHAMP) { Speed(4); Ability((c == 0 || c == 1) ? ABILITY_NO_GUARD : ABILITY_GUTS); }
    } WHEN {
        if (c == 0)
            TURN { MOVE(playerLeft, MOVE_SKY_DROP, target: opponentLeft); MOVE(opponentRight, MOVE_DYNAMIC_PUNCH, target: playerLeft, hit: FALSE); }
        else if (c == 1)
            TURN { MOVE(playerLeft, MOVE_SKY_DROP, target: opponentLeft); MOVE(opponentRight, MOVE_DYNAMIC_PUNCH, target: opponentLeft, hit: FALSE); }
        else
            TURN { MOVE(playerLeft, MOVE_SKY_DROP, target: opponentLeft); MOVE(playerRight, MOVE_DYNAMIC_PUNCH, target: opponentLeft, hit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, playerLeft);
        if (c == 0)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DYNAMIC_PUNCH, opponentRight);
        else if (c == 1)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DYNAMIC_PUNCH, opponentRight);
        else if (c == 4)
            NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_DYNAMIC_PUNCH, playerRight);
        else
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DYNAMIC_PUNCH, playerRight);
    }
}
