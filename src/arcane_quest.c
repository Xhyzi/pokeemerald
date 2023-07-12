#include "global.h"
#include "arcane_quest.h"
#include "constants/items.h"
#include "constants/quest.h"
#include "data/quest_data.h"

const u8 gQuestFlagMask[] = {
    MASK_FLAG_1,
    MASK_FLAG_2,
    MASK_FLAG_3,
    MASK_FLAG_4,
    MASK_FLAG_5,
    MASK_FLAG_6,
};

bool8 Quest_SetQuestAsActive(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    gSaveBlock1Ptr->questFlags[questId].active = TRUE;
    return TRUE;
}

bool8 Quest_SetQuestAsCompleted(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    gSaveBlock1Ptr->questFlags[questId].completed = TRUE;
    return TRUE;
}

bool8 Quest_ClearQuestActive(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    gSaveBlock1Ptr->questFlags[questId].active = FALSE;
    return TRUE;
}

bool8 Quest_ClearQuestCompleted(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    gSaveBlock1Ptr->questFlags[questId].completed = FALSE;
    return TRUE;
}

bool8 Quest_IsQuestActive(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    return gSaveBlock1Ptr->questFlags[questId].active == TRUE;
}

bool8 Quest_IsQuestCompleted(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    return gSaveBlock1Ptr->questFlags[questId].completed == TRUE;
}

bool8 Quest_SetQuestFlag(u16 questId, u8 flagId) 
{
    if (flagId > QUEST_FLAG_COUNT && questId >= QUEST_COUNT)
        return FALSE;

    return gSaveBlock1Ptr->questFlags[questId].flags |= (1 << flagId);
}

bool8 Quest_ClearQuestFlag(u16 questId, u8 flagId) 
{
    if (flagId > QUEST_FLAG_COUNT && questId >= QUEST_COUNT)
        return FALSE;

    return gSaveBlock1Ptr->questFlags[questId].flags &= ~(1 << flagId);
}

bool8 Quest_IsQuestFlagActive(u16 questId, u8 flagId) 
{
    if (flagId > QUEST_FLAG_COUNT && questId >= QUEST_COUNT)
        return FALSE;

    return gSaveBlock1Ptr->questFlags[questId].flags & (1 << flagId);
}

