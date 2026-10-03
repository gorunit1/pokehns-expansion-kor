#include "global.h"
#include "constants/trainers.h"

static enum TrainerPicID GetEmeraldTrainerPic(enum Gender gender)
{
    return gender == MALE ? TRAINER_PIC_BRENDAN : TRAINER_PIC_MAY;
}
static enum TrainerPicID GetRSTrainerPic(enum Gender gender)
{
    return gender == MALE ? TRAINER_PIC_RS_BRENDAN : TRAINER_PIC_RS_MAY;
}

static enum TrainerPicID GetKantoTrainerPic(enum Gender gender)
{
    return gender == MALE ? TRAINER_PIC_RED : TRAINER_PIC_LEAF;
}

// HnS: Gold/Kris are used for the player and for every link player, whatever their game version.
static enum TrainerPicID GetHnsTrainerPic(enum Gender gender)
{
    return gender == MALE ? TRAINER_PIC_GOLD_HNS : TRAINER_PIC_KRIS_HNS;
}

enum TrainerPicID GetPlayerTrainerPic(enum Gender gender, enum GameVersion version)
{
    if (IS_HNS)
        return GetHnsTrainerPic(gender);

    switch (version)
    {
        case VERSION_SAPPHIRE:
        case VERSION_RUBY:
            return GetRSTrainerPic(gender);
        case VERSION_LEAF_GREEN:
        case VERSION_FIRE_RED:
            return GetKantoTrainerPic(gender);
        case VERSION_EMERALD:
        default:
            return GetEmeraldTrainerPic(gender);
    }
}
