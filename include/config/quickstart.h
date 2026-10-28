#ifndef GUARD_CONFIG_QUICKSTART_H
#define GUARD_CONFIG_QUICKSTART_H

#define GENDER_MALE              0
#define GENDER_FEMALE            1
#define GENDER_RANDOM            2

// Quickstart Settings
#define ENABLE_QUICKSTART            FALSE // If TRUE press SELECT to start a new game from the titlescreen (Disabled on Release Builds)
                                           // HnS: FALSE - `make hns` is not a release build, and the quickstart skips the HnS intro
                                           // (English default names) and shares SELECT with the title screen button combos.
#define QUICKSTART_HUD               TRUE  // Displays a small hud element on the titlescreen when Quickstart is enabled
#define QUICKSTART_GENDER            GENDER_RANDOM

#define QUICKSTART_HUD_X             (DISPLAY_WIDTH - 32) // Quickstart HUD X Position
#define QUICKSTART_HUD_Y             (16)                 // Quickstart HUD Y Position

#endif // GUARD_CONFIG_QUICKSTART_H
