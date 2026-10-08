#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"

// HnS: since #9865 AI_WeatherHasEffect reads the AI's known or guessed abilities instead of the real HasWeatherEffect().
// Cloud Nine and Air Lock always show their pop-up on entry and get recorded, so a foe that showed none does not have
// them. AI_KnowsWeatherNegatingAbility keeps a guessed Cloud Nine from blocking the weather, as before #9865.
// PASSES_RANDOMLY(1, 1, RNG_AI_ABILITY) runs every guess of the player's unrevealed ability.

ASSUMPTIONS
{
    ASSUME(GetSpeciesAbility(SPECIES_PSYDUCK, 1) == ABILITY_CLOUD_NINE);
    ASSUME(GetSpeciesAbility(SPECIES_GOLDUCK, 1) == ABILITY_CLOUD_NINE);
    ASSUME(GetSpeciesAbility(SPECIES_LICKITUNG, 2) == ABILITY_CLOUD_NINE);
    ASSUME(GetSpeciesAbility(SPECIES_LICKILICKY, 2) == ABILITY_CLOUD_NINE);
    ASSUME(GetSpeciesAbility(SPECIES_SWABLU, 2) == ABILITY_CLOUD_NINE);
    ASSUME(GetSpeciesAbility(SPECIES_ALTARIA, 2) == ABILITY_CLOUD_NINE);
    ASSUME(GetSpeciesAbility(SPECIES_DRAMPA, 2) == ABILITY_CLOUD_NINE);
    ASSUME(GetMoveEffect(MOVE_WEATHER_BALL) == EFFECT_WEATHER_BALL);
}

AI_SINGLE_BATTLE_TEST("AI weather (HnS): rain still boosts Weather Ball against a foe that showed no Cloud Nine")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_PSYDUCK;    ability = ABILITY_DAMP; }
    PARAMETRIZE { species = SPECIES_GOLDUCK;    ability = ABILITY_SWIFT_SWIM; }
    PARAMETRIZE { species = SPECIES_LICKITUNG;  ability = ABILITY_OWN_TEMPO; }
    PARAMETRIZE { species = SPECIES_LICKILICKY; ability = ABILITY_OBLIVIOUS; }
    PARAMETRIZE { species = SPECIES_SWABLU;     ability = ABILITY_NATURAL_CURE; }
    PARAMETRIZE { species = SPECIES_ALTARIA;    ability = ABILITY_NATURAL_CURE; }
    PARAMETRIZE { species = SPECIES_DRAMPA;     ability = ABILITY_SAP_SIPPER; }
    PASSES_RANDOMLY(1, 1, RNG_AI_ABILITY);
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(species) { Ability(ability); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_POLITOED) { Ability(ABILITY_DRIZZLE); Moves(MOVE_WEATHER_BALL, MOVE_BODY_SLAM); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_WEATHER_BALL); }
    }
}

AI_SINGLE_BATTLE_TEST("AI weather (HnS): a Cloud Nine or Air Lock that showed its pop-up still blocks the weather")
{
    u32 species;
    enum Ability ability;

    PARAMETRIZE { species = SPECIES_GOLDUCK;   ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_LICKITUNG; ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_ALTARIA;   ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA;  ability = ABILITY_AIR_LOCK; }
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(species) { Ability(ability); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_POLITOED) { Ability(ABILITY_DRIZZLE); Moves(MOVE_WEATHER_BALL, MOVE_BODY_SLAM); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_BODY_SLAM); }
    } SCENE {
        ABILITY_POPUP(player, ability);
    }
}

AI_SINGLE_BATTLE_TEST("AI weather (HnS): Cloud Nine counts from the turn after its pop-up when it switches in")
{
    enum Ability ability;

    PARAMETRIZE { ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { ability = ABILITY_DAMP; }
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_GOLDUCK) { Ability(ability); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_POLITOED) { Ability(ABILITY_DRIZZLE); Moves(MOVE_WEATHER_BALL, MOVE_BODY_SLAM); }
    } WHEN {
        TURN { SWITCH(player, 1); EXPECT_MOVE(opponent, MOVE_WEATHER_BALL); }
        if (ability == ABILITY_CLOUD_NINE)
            TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_BODY_SLAM); }
        else
            TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_WEATHER_BALL); }
    } SCENE {
        if (ability == ABILITY_CLOUD_NINE)
            ABILITY_POPUP(player, ABILITY_CLOUD_NINE);
        else
            NOT ABILITY_POPUP(player, ABILITY_CLOUD_NINE);
    }
}

AI_SINGLE_BATTLE_TEST("AI weather (HnS): AI sets up rain against a foe that showed no Cloud Nine")
{
    enum Ability ability;

    PARAMETRIZE { ability = ABILITY_OWN_TEMPO; }
    PARAMETRIZE { ability = ABILITY_OBLIVIOUS; }
    PASSES_RANDOMLY(1, 1, RNG_AI_ABILITY);
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_RAIN_DANCE) == EFFECT_WEATHER);
        ASSUME(GetMoveWeatherType(MOVE_RAIN_DANCE) == BATTLE_WEATHER_RAIN);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY);
        PLAYER(SPECIES_LICKITUNG) { Ability(ability); Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_KABUTOPS) { Ability(ABILITY_SWIFT_SWIM); Moves(MOVE_RAIN_DANCE, MOVE_POUND); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); EXPECT_MOVE(opponent, MOVE_RAIN_DANCE); }
    }
}

AI_SINGLE_BATTLE_TEST("AI weather (HnS): smart mon choices count a Drizzle switch-in against a foe that showed no Cloud Nine")
{
    enum Ability ability;

    PARAMETRIZE { ability = ABILITY_OWN_TEMPO; }
    PARAMETRIZE { ability = ABILITY_CLOUD_NINE; }
    if (ability != ABILITY_CLOUD_NINE)
        PASSES_RANDOMLY(1, 1, RNG_AI_ABILITY);
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_SMART_MON_CHOICES);
        PLAYER(SPECIES_LICKITUNG) { Speed(2); HP(120); Ability(ability); Moves(MOVE_SCRATCH); } // only rain-boosted Bubble Beam KOs
        OPPONENT(SPECIES_ZIGZAGOON) { Speed(1); Level(1); Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_POLITOED) { Speed(5); Ability(ABILITY_DRIZZLE); Moves(MOVE_BUBBLE_BEAM); }
        OPPONENT(SPECIES_CONKELDURR) { Speed(1); Ability(ABILITY_GUTS); Moves(MOVE_SUPERPOWER); }
    } WHEN {
        if (ability == ABILITY_CLOUD_NINE)
            TURN { MOVE(player, MOVE_SCRATCH); EXPECT_MOVE(opponent, MOVE_SCRATCH); EXPECT_SEND_OUT(opponent, 2); }
        else
            TURN { MOVE(player, MOVE_SCRATCH); EXPECT_MOVE(opponent, MOVE_SCRATCH); EXPECT_SEND_OUT(opponent, 1); }
    }
}
