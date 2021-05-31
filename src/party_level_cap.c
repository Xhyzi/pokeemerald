#include "global.h"
#include "main.h"
#include "party_level_cap.h"
#include "event_data.h"
#include "constants/vars.h"
#include "data/level_caps.h"

// Returns level cap
u8 GetCurrentPartyLevelCap()
{
    u16 tmp = VarGet(VAR_PARTY_LEVEL_CAP);

    if (tmp < (ARRAY_COUNT(gLevelCapTable))
        return gLevelCapTable[tmp];
    return gLevelCapTable[ARRAY_COUNT(gLevelCapTable) - 1];
}
