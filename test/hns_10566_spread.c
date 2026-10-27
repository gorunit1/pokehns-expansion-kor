// HnS seq 486 #10566 회귀 테스트 — 기술을 쓰기 시작할 때 자리에 없던(쓰러진) 대상에게 HP 바·결과 문장이 나오지 않는다.
// upstream 새 테스트의 영문 문장 검사를 한글 문장으로 옮긴 것(영문 NOT 검사는 HnS에서 늘 통과하므로). 이름이 모두 "HnS 10566:"로 시작한다.
#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("HnS 10566: no result lines for a foe that fainted before the spread or status move")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        ASSUME(GetMoveTarget(MOVE_COTTON_SPORE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PLAYER(SPECIES_WHIMSICOTT) { Speed(3); Ability(ABILITY_PRANKSTER); }
        OPPONENT(SPECIES_AGGRON) { Defense(1); Speed(4); }
        OPPONENT(SPECIES_HERACROSS) { Speed(1); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_MEMENTO, target: playerRight); MOVE(playerLeft, MOVE_EARTHQUAKE); }
        TURN { MOVE(playerRight, MOVE_COTTON_SPORE); MOVE(playerLeft, MOVE_PSYCHIC_TERRAIN); }
        TURN { MOVE(playerRight, MOVE_COTTON_SPORE); }
    } SCENE {
        MESSAGE("마자용은 지진을 썼다!");
        NONE_OF {
            HP_BAR(opponentLeft);
            MESSAGE("상대 보스로라에게 효과가 매우 굉장했다!!");
        }
        MESSAGE("엘풍은 목화포자를 썼다!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponentLeft);
            MESSAGE("상대 보스로라의 스피드가 매우 크게 떨어졌다!");
        }
        MESSAGE("엘풍은 목화포자를 썼다!");
        NONE_OF {
            MESSAGE("상대 보스로라는 사이코필드가 지켜 주고 있다!");
            MESSAGE("상대 보스로라의 스피드가 매우 크게 떨어졌다!");
        }
    }
}

DOUBLE_BATTLE_TEST("HnS 10566: a missed spread move does not name a foe that fainted earlier in the turn")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); Item(ITEM_BRIGHTPOWDER); }
        PLAYER(SPECIES_WYNAUT) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); Item(ITEM_BRIGHTPOWDER); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft); MOVE(playerRight, MOVE_EARTHQUAKE, hit: FALSE); }
    } SCENE {
        MESSAGE("상대 마자용은 쓰러졌다!");
        MESSAGE("마자는 지진을 썼다!");
        MESSAGE("마자용에게는 맞지 않았다!");
        NOT MESSAGE("상대 마자용에게는 맞지 않았다!");
        MESSAGE("상대 마자에게는 맞지 않았다!");
    }
}

DOUBLE_BATTLE_TEST("HnS 10566: Unseen Fist through Protect does not name a foe that fainted earlier in the turn")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_BRUTAL_SWING) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_URSHIFU) { Attack(1); Speed(4); Ability(ABILITY_UNSEEN_FIST); }
        PLAYER(SPECIES_URSHIFU) { Attack(1); Speed(3); Ability(ABILITY_UNSEEN_FIST); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(1); }
    } WHEN {
        TURN {
            MOVE(opponentLeft, MOVE_PROTECT);
            MOVE(opponentRight, MOVE_PROTECT);
            MOVE(playerLeft, MOVE_BRUTAL_SWING);
            MOVE(playerRight, MOVE_BRUTAL_SWING);
        }
    } SCENE {
        MESSAGE("상대 마자용은 쓰러졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BRUTAL_SWING, playerRight);
        NONE_OF {
            MESSAGE("상대 마자용과 마자에게 효과가 굉장했다!");
            MESSAGE("상대 마자용은 공격을 막아 내지 못하고 데미지를 입었다!");
        }
        MESSAGE("상대 마자에게 효과가 굉장했다!");
    }
}
