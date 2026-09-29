#include "global.h"
#include "ow_synchronize.h"
#include "pokemon.h"
#include "random.h"
#include "save.h"
#include "constants/pokemon.h"

const static enum Ability sForceNatureAbilities[] = {ABILITY_SYNCHRONIZE, ABILITY_NONE};
const static enum Ability sForceOppositeGenderAbilities[] = {ABILITY_CUTE_CHARM, ABILITY_NONE};
const static enum Ability sIncreaseHatchingSpeedAbilities[] = {ABILITY_MAGMA_ARMOR, ABILITY_FLAME_BODY, ABILITY_STEAM_ENGINE, ABILITY_NONE};

static UNUSED bool32 HasHalfChance(enum Species species);
static UNUSED bool32 HasTwoThirdsChance(enum Species species);
static UNUSED bool32 IsFalse(enum Species species);
static UNUSED bool32 IsTrue(enum Species species);
static UNUSED bool32 IsTrueIfUndiscoveredEggGroup(enum Species species);

static const bool32 (*const sSynchronizeModes[])(enum Species) =
{
#if OW_SYNCHRONIZE_NATURE == GEN_3
    [WILDMON_ORIGIN] = HasHalfChance,
    [STATIC_WILDMON_ORIGIN] = IsFalse,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsFalse,
#elif OW_SYNCHRONIZE_NATURE <= GEN_5
    [WILDMON_ORIGIN] = HasHalfChance,
    [STATIC_WILDMON_ORIGIN] = HasHalfChance,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsFalse,
#elif OW_SYNCHRONIZE_NATURE == GEN_6
    [WILDMON_ORIGIN] = HasHalfChance,
    [STATIC_WILDMON_ORIGIN] = HasHalfChance,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsTrueIfUndiscoveredEggGroup,
#elif OW_SYNCHRONIZE_NATURE == GEN_7
    [WILDMON_ORIGIN] = HasHalfChance,
    [STATIC_WILDMON_ORIGIN] = HasHalfChance,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsTrue,
#elif OW_SYNCHRONIZE_NATURE >= GEN_8
    [WILDMON_ORIGIN] = IsTrue,
    [STATIC_WILDMON_ORIGIN] = IsTrue,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsFalse,
#else
    [WILDMON_ORIGIN] = IsFalse,
    [STATIC_WILDMON_ORIGIN] = IsFalse,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsFalse,
#endif
};

static const bool32 (*const sCuteCharmModes[])(enum Species) =
{
    [WILDMON_ORIGIN] = HasTwoThirdsChance,
    [STATIC_WILDMON_ORIGIN] = HasTwoThirdsChance,
    [ROAMER_ORIGIN] = IsFalse,
    [GIFTMON_ORIGIN] = IsFalse,
};

static UNUSED bool32 HasHalfChance(enum Species species)
{
    return Random() % 2;
}

static UNUSED bool32 HasTwoThirdsChance(enum Species species)
{
    return Random() % 3;
}

static UNUSED bool32 IsFalse(enum Species species)
{
    return FALSE;
}

static UNUSED bool32 IsTrue(enum Species species)
{
    return TRUE;
}

static UNUSED bool32 IsTrueIfUndiscoveredEggGroup(enum Species species)
{
    return (gSpeciesInfo[species].eggGroups[0] == EGG_GROUP_NO_EGGS_DISCOVERED);
}

static bool32 IsSynchronizeActive(void)
{
    return ((!GetMonData(&gPlayerParty[0], MON_DATA_SANITY_IS_EGG)
        && GetMonAbility(&gPlayerParty[0]) == ABILITY_SYNCHRONIZE));
}

bool32 DoesLeadingMonHaveAbilityEffect(const enum Ability *abilityArray)
{
    if (GetMonData(&gPlayerParty[0], MON_DATA_SANITY_IS_EGG))
        return FALSE;
    enum Ability leadingMonAbility = GetMonAbility(&gPlayerParty[0]);
    for (u32 i = 0; abilityArray[i] != ABILITY_NONE; i++)
    {
        if (leadingMonAbility == abilityArray[i])
            return TRUE;
    }
    return FALSE;
}

bool32 DoesPartyMemberHaveAbilityEffect(const enum Ability *abilityArray)
{
    for (u32 j = 0; j < gPlayerPartyCount; j++)
    {
        if (GetMonData(&gPlayerParty[j], MON_DATA_SANITY_IS_EGG))
            continue;
        enum Ability monAbility = GetMonAbility(&gPlayerParty[j]);
        for (u32 i = 0; abilityArray[i] != ABILITY_NONE; i++)
        {
            if (monAbility == abilityArray[i])
                return TRUE;
        }
    }
    return FALSE;
}

u32 GetSynchronizedNature(enum GeneratedMonOrigin origin, enum Species species)
{
    if (!IsSynchronizeActive())
        return NATURE_RANDOM;
    if (gSaveBlock3Ptr->challengeSettings.tx_Mode_Synchronize == 0)
    {
        if ((origin != WILDMON_ORIGIN && origin != STATIC_WILDMON_ORIGIN) || !HasHalfChance(species))
            return NATURE_RANDOM;
    }
    else if (!(sSynchronizeModes[origin](species)))
        return NATURE_RANDOM;
    return GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY) % NUM_NATURES;
}

u32 GetSynchronizedGender(enum GeneratedMonOrigin origin, enum Species species)
{
    if (!DoesLeadingMonHaveAbilityEffect(sForceOppositeGenderAbilities))
        return MON_GENDER_RANDOM;
    if (!(sCuteCharmModes[origin](species)))
        return MON_GENDER_RANDOM;
    // Species with a fixed gender ratio (genderless or single-gender) can never match the
    // requested gender, which would hang GetMonPersonality's personality reroll loop.
    switch (gSpeciesInfo[species].genderRatio)
    {
    case MON_MALE:
    case MON_FEMALE:
    case MON_GENDERLESS:
        return MON_GENDER_RANDOM;
    }
    // A genderless lead has no gender to invert. Normally impossible, but randomizer
    // settings can hand cute charm to a genderless species.
    u8 leadingMonGender = GetMonGender(&gPlayerParty[0]);
    if (leadingMonGender == MON_GENDERLESS)
        return MON_GENDER_RANDOM;
    if (leadingMonGender == MON_FEMALE)
        return MON_MALE;
    else
        return MON_FEMALE;
}

bool32 DoesPartyHaveIncubatorMon(void)
{
    return DoesPartyMemberHaveAbilityEffect(sIncreaseHatchingSpeedAbilities);
}
