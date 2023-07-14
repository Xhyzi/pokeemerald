#include "global.h"
#include "arcane_quest_callbacks.h"
#include "arcane_quest.h"
#include "event_data.h"

/**
 * @brief this callback are called to check if the quest can be completed
 * they have to be implemented for each single quest.
 * The result should be stored in gSpecialVar_Result
 * 
 * @return bool8 
 */
bool8 QuestCB_DummyTest()
{
    gSpecialVar_Result = FALSE;
    return FALSE;
}
