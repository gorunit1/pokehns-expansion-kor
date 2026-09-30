#include "global.h"
#include "item.h"
#include "constants/flags.h"
#include <stdio.h>
static u16 sVar8004;
static bool8 sFlagSet;
static u16 sChosen[4];
#define gSpecialVar_0x8004 sVar8004
static bool8 FlagGetStub(u16 id) { (void)id; return sFlagSet; }
#define FlagGet FlagGetStub
static void SetPlayerBerryData(u8 playerId, u16 itemId) { sChosen[playerId] = itemId; }
#include "extracted.inc"
struct E { int id; const char *name; };
static const struct E sItems[] = {
#include "names.inc"
};
static const char *NameOf(int item)
{
    for (unsigned i = 0; i < sizeof(sItems)/sizeof(sItems[0]); i++)
        if (sItems[i].id == item) return sItems[i].name + 5;
    return "?";
}
int main(void)
{
    for (int flag = 0; flag <= 1; flag++)
    for (int opp = 1; opp <= 3; opp++)
    for (int item = FIRST_BERRY_INDEX; item <= LAST_BERRY_INDEX; item++)
    {
        int nfl = (item == ITEM_ENIGMA_BERRY_E_READER) ? FLAVOR_COUNT : 1;
        for (int fl = 0; fl < nfl; fl++)
        {
            struct BlenderBerry bb;
            memset(&bb, 0, sizeof(bb));
            for (int k = 0; k < FLAVOR_COUNT; k++) bb.flavors[k] = 10;
            bb.flavors[fl] = 0; /* E-Reader: make flavor fl the minimum */
            sVar8004 = opp; sFlagSet = flag;
            memset(sChosen, 0, sizeof(sChosen));
            SetOpponentsBerryData(item, opp + 1, &bb);
            printf("flagHidden=%d\topp=%d\tplayer=%s", flag, opp, NameOf(item));
            if (nfl > 1) printf("(minFlavor=%d)", fl);
            for (int p = 1; p <= opp; p++) printf("\t%s", NameOf(sChosen[p]));
            printf("\n");
        }
    }
    return 0;
}
