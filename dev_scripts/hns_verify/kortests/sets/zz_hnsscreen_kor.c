#include "global.h"
#include "test/battle.h"

// HNSSCREEN: Korean regression set for removed screens (2026-10-13).
// T: upstream #10829 pulled forward (6d08adc6) - timers of broken screens are cleared.
// P: HnS fix - the screen messages ({B_ATK_PREFIX1}의 리플렉터가\n없어졌다! etc.) name the side that lost the screen.
// C: HnS fix - Screen Cleaner prints each side with its own prefix and clears the removed screens' timers.

SINGLE_BATTLE_TEST("HNSSCREEN T01 broken foe screen is announced once with the foe prefix and not again later")
{
    enum Move screen, move;
    PARAMETRIZE { screen = MOVE_REFLECT; move = MOVE_BRICK_BREAK; }
    PARAMETRIZE { screen = MOVE_LIGHT_SCREEN; move = MOVE_BRICK_BREAK; }
    PARAMETRIZE { screen = MOVE_AURORA_VEIL; move = MOVE_BRICK_BREAK; }
    PARAMETRIZE { screen = MOVE_REFLECT; move = MOVE_PSYCHIC_FANGS; }
    PARAMETRIZE { screen = MOVE_LIGHT_SCREEN; move = MOVE_PSYCHIC_FANGS; }
    PARAMETRIZE { screen = MOVE_AURORA_VEIL; move = MOVE_PSYCHIC_FANGS; }
    PARAMETRIZE { screen = MOVE_REFLECT; move = MOVE_RAGING_BULL; }
    PARAMETRIZE { screen = MOVE_LIGHT_SCREEN; move = MOVE_RAGING_BULL; }
    PARAMETRIZE { screen = MOVE_AURORA_VEIL; move = MOVE_RAGING_BULL; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SNOWSCAPE); MOVE(opponent, screen); }
        TURN { MOVE(player, move); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SNOWSCAPE, player);
        ANIMATION(ANIM_TYPE_MOVE, screen, opponent);
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        if (screen == MOVE_REFLECT)
            MESSAGE("상대의 리플렉터가 없어졌다!");
        else if (screen == MOVE_LIGHT_SCREEN)
            MESSAGE("상대의 빛의장막이 없어졌다!");
        else
            MESSAGE("상대의 오로라베일이 없어졌다!");
        HP_BAR(opponent);
        NONE_OF {
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("상대의 빛의장막이 없어졌다!");
            MESSAGE("상대의 오로라베일이 없어졌다!");
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
            MESSAGE("우리 편의 오로라베일이 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN T02 breaking Light Screen after a broken Reflect names only Light Screen")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_BRICK_BREAK; }
    PARAMETRIZE { move = MOVE_PSYCHIC_FANGS; }
    PARAMETRIZE { move = MOVE_RAGING_BULL; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); MOVE(player, move); }
        TURN { MOVE(opponent, MOVE_LIGHT_SCREEN); MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFLECT, opponent);
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LIGHT_SCREEN, opponent);
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        NONE_OF {
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        }
        MESSAGE("상대의 빛의장막이 없어졌다!");
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN T03 screens that run out are still announced with the right prefix")
{
    enum Move screen;
    bool32 foe;
    PARAMETRIZE { screen = MOVE_REFLECT; foe = TRUE; }
    PARAMETRIZE { screen = MOVE_LIGHT_SCREEN; foe = TRUE; }
    PARAMETRIZE { screen = MOVE_AURORA_VEIL; foe = TRUE; }
    PARAMETRIZE { screen = MOVE_REFLECT; foe = FALSE; }
    PARAMETRIZE { screen = MOVE_LIGHT_SCREEN; foe = FALSE; }
    PARAMETRIZE { screen = MOVE_AURORA_VEIL; foe = FALSE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        if (foe)
            TURN { MOVE(player, MOVE_SNOWSCAPE); MOVE(opponent, screen); }
        else
            TURN { MOVE(opponent, MOVE_SNOWSCAPE); MOVE(player, screen); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        if (foe)
        {
            ANIMATION(ANIM_TYPE_MOVE, screen, opponent);
            if (screen == MOVE_REFLECT)
                MESSAGE("상대의 리플렉터가 없어졌다!");
            else if (screen == MOVE_LIGHT_SCREEN)
                MESSAGE("상대의 빛의장막이 없어졌다!");
            else
                MESSAGE("상대의 오로라베일이 없어졌다!");
        }
        else
        {
            ANIMATION(ANIM_TYPE_MOVE, screen, player);
            if (screen == MOVE_REFLECT)
                MESSAGE("우리 편의 리플렉터가 없어졌다!");
            else if (screen == MOVE_LIGHT_SCREEN)
                MESSAGE("우리 편의 빛의장막이 없어졌다!");
            else
                MESSAGE("우리 편의 오로라베일이 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN T04 screen-breaking moves deal the same damage with and without a screen", s16 damage)
{
    enum Move screen, move;
    PARAMETRIZE { screen = MOVE_CELEBRATE; move = MOVE_BRICK_BREAK; }
    PARAMETRIZE { screen = MOVE_REFLECT; move = MOVE_BRICK_BREAK; }
    PARAMETRIZE { screen = MOVE_CELEBRATE; move = MOVE_PSYCHIC_FANGS; }
    PARAMETRIZE { screen = MOVE_REFLECT; move = MOVE_PSYCHIC_FANGS; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, screen); MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
        EXPECT_EQ(results[2].damage, results[3].damage);
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN P01 a foe breaking the player's screen uses the ally prefix")
{
    enum Move screen;
    PARAMETRIZE { screen = MOVE_REFLECT; }
    PARAMETRIZE { screen = MOVE_LIGHT_SCREEN; }
    PARAMETRIZE { screen = MOVE_AURORA_VEIL; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SNOWSCAPE); MOVE(player, screen); }
        TURN { MOVE(opponent, MOVE_BRICK_BREAK); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, screen, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BRICK_BREAK, opponent);
        if (screen == MOVE_REFLECT)
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        else if (screen == MOVE_LIGHT_SCREEN)
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
        else
            MESSAGE("우리 편의 오로라베일이 없어졌다!");
        HP_BAR(player);
        NONE_OF {
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
            MESSAGE("우리 편의 오로라베일이 없어졌다!");
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("상대의 빛의장막이 없어졌다!");
            MESSAGE("상대의 오로라베일이 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN P02 the attacker is restored after the screen message (Life Orb recoil hits the user)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); }
        TURN { MOVE(player, MOVE_BRICK_BREAK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFLECT, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BRICK_BREAK, player);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        HP_BAR(opponent);
        HP_BAR(player);
        NOT HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN P03 Defog still announces the foe's screen with the foe prefix")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); }
        TURN { MOVE(player, MOVE_DEFOG); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFLECT, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DEFOG, player);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        NOT MESSAGE("우리 편의 리플렉터가 없어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C01 Screen Cleaner: only the user's side")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFLECT); }
        TURN { SWITCH(player, 1); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFLECT, player);
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("우리 편의 리플렉터가 없어졌다!");
        NONE_OF {
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("상대의 리플렉터가 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C02 Screen Cleaner: only the foe's side")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); }
        TURN { SWITCH(player, 1); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFLECT, opponent);
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        NONE_OF {
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C03 Screen Cleaner: different screens on both sides")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFLECT); MOVE(opponent, MOVE_LIGHT_SCREEN); }
        TURN { SWITCH(player, 1); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFLECT, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LIGHT_SCREEN, opponent);
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("우리 편의 리플렉터가 없어졌다!");
        MESSAGE("상대의 빛의장막이 없어졌다!");
        NONE_OF {
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("상대의 빛의장막이 없어졌다!");
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
            MESSAGE("상대의 리플렉터가 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C04 Screen Cleaner: the same screen on both sides")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFLECT); MOVE(opponent, MOVE_REFLECT); }
        TURN { SWITCH(player, 1); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("우리 편의 리플렉터가 없어졌다!");
        MESSAGE("상대의 리플렉터가 없어졌다!");
        NONE_OF {
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
            MESSAGE("상대의 리플렉터가 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C05 Screen Cleaner: all three screens on the foe's side")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SNOWSCAPE); MOVE(opponent, MOVE_REFLECT); }
        TURN { MOVE(opponent, MOVE_LIGHT_SCREEN); }
        TURN { MOVE(opponent, MOVE_AURORA_VEIL); }
        TURN { SWITCH(player, 1); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AURORA_VEIL, opponent);
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        MESSAGE("상대의 빛의장막이 없어졌다!");
        MESSAGE("상대의 오로라베일이 없어졌다!");
        NONE_OF {
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("상대의 빛의장막이 없어졌다!");
            MESSAGE("상대의 오로라베일이 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C06 Screen Cleaner with no screens: no popup, no message")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 리플렉터가 없어졌다!");
        }
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C07 a screen set after Screen Cleaner still runs out normally")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_REFLECT); }
        TURN { SWITCH(player, 1); MOVE(opponent, MOVE_LIGHT_SCREEN); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SCREEN_CLEANER);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LIGHT_SCREEN, opponent);
        NOT MESSAGE("상대의 리플렉터가 없어졌다!");
        MESSAGE("상대의 빛의장막이 없어졌다!");
    }
}

SINGLE_BATTLE_TEST("HNSSCREEN C08 Screen Cleaner used by the foe: the foe's side first, then the player's side")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MR_RIME) { Ability(ABILITY_SCREEN_CLEANER); }
    } WHEN {
        TURN { MOVE(player, MOVE_LIGHT_SCREEN); MOVE(opponent, MOVE_REFLECT); }
        TURN { SWITCH(opponent, 1); MOVE(player, MOVE_TACKLE); }
        TURN {}
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_SCREEN_CLEANER);
        MESSAGE("상대의 리플렉터가 없어졌다!");
        MESSAGE("우리 편의 빛의장막이 없어졌다!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
        NONE_OF {
            MESSAGE("상대의 리플렉터가 없어졌다!");
            MESSAGE("우리 편의 빛의장막이 없어졌다!");
        }
    }
}
