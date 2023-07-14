#include "global.h"
#include "arcane_quest.h"
#include "arcane_quest_callbacks.h"
#include "arcane_quest_menu.h"
#include "event_data.h"
#include "malloc.h"
#include "menu.h"
#include "strings.h"
#include "string_util.h"
#include "window.h"
#include "constants/items.h"
#include "constants/quest.h"
#include "constants/vars.h"
#include "data/quest_data.h"
#include "data/text/quest.h"

#define QUEST_HEADER_WIDTH      30
#define QUEST_HEADER_HEIGHT     2
#define REWARD_BOX_WIDTH        9
#define REWARD_HEADER_HEIGHT    2
#define REWARD_BOX_HEIGHT       8
#define REWARD_AMOUNT_WIDTH     4
#define REWARD_AMOUNT_HEIGHT    2
#define REWARD_MONEY_WIDTH      9
#define REWARD_MONEY_HEIGHT     2

#define BASE_BLOCK 1
#define BLOCK_QUEST_HEADER      BASE_BLOCK
#define BLOCK_REWARD_HEADER     BLOCK_QUEST_HEADER + (QUEST_HEADER_WIDTH * QUEST_HEADER_HEIGHT)
#define BLOCK_REWARD_BOX        BLOCK_REWARD_HEADER + (REWARD_BOX_WIDTH * REWARD_HEADER_HEIGHT)
#define BLOCK_REWARD_AMOUNT_1   BLOCK_REWARD_BOX + (REWARD_BOX_WIDTH * REWARD_BOX_HEIGHT)
#define BLOCK_REWARD_AMOUNT_2   BLOCK_REWARD_AMOUNT_1 + (REWARD_AMOUNT_WIDTH * REWARD_AMOUNT_HEIGHT)
#define BLOCK_REWARD_MONEY      BLOCK_REWARD_AMOUNT_2 + (REWARD_AMOUNT_WIDTH * REWARD_AMOUNT_HEIGHT)
#define BLOCK_DIFFICULTY        BLOCK_REWARD_MONEY + (REWARD_MONEY_WIDTH * REWARD_MONEY_HEIGHT)

static EWRAM_DATA u8 sWindowHeaderId = 0;
static EWRAM_DATA u8 sWindowHeaderReward = 0;
static EWRAM_DATA u8 sWindowRewardsBackground = 0;
static EWRAM_DATA u8 sWindowReward1Amount = 0;
static EWRAM_DATA u8 sWindowReward2Amount = 0;
static EWRAM_DATA u8 sWindowRewardMoney = 0;
static EWRAM_DATA u8 sWindowDifficultyId = 0;

static void Quest_AddTextToWindow(u8 windowId, u8 font, u8 fillColor, const u8 *text, u8 x, u8 y);
static void Quest_DestroyWindow(u8 *windowId);
static void Quest_BuildRewardWindow(u16 questId);

static const struct WindowTemplate sWindowTemplate_QuestHeader = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = QUEST_HEADER_WIDTH,
    .height = QUEST_HEADER_HEIGHT,
    .paletteNum = 15,
    .baseBlock = BLOCK_QUEST_HEADER,
};

static const struct WindowTemplate sWindowTemplate_RewardsHeader = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 3,
    .width = REWARD_BOX_WIDTH,
    .height = REWARD_HEADER_HEIGHT,
    .paletteNum = 15,
    .baseBlock = BLOCK_REWARD_HEADER,
};

static const struct WindowTemplate sWindowTemplate_RewardsBackground = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 5,
    .width = REWARD_BOX_WIDTH,
    .height = REWARD_BOX_HEIGHT,
    .paletteNum = 15,
    .baseBlock = BLOCK_REWARD_BOX,
};

static const struct WindowTemplate sWindowTemplate_Reward1Amount = {
    .bg = 0,
    .tilemapLeft = 4,
    .tilemapTop = 6,
    .width = REWARD_AMOUNT_WIDTH,
    .height = REWARD_AMOUNT_HEIGHT,
    .paletteNum = 15,
    .baseBlock = BLOCK_REWARD_AMOUNT_1,
};

static const struct WindowTemplate sWindowTemplate_Reward2Amount = 
{
    .bg = 0,
    .tilemapLeft = 4,
    .tilemapTop = 9,
    .width = REWARD_AMOUNT_WIDTH,
    .height = REWARD_AMOUNT_HEIGHT,
    .paletteNum = 15,
    .baseBlock = BLOCK_REWARD_AMOUNT_2,
};

static const struct WindowTemplate sWindowTemplate_RewardMoney = 
{
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 11,
    .width = REWARD_MONEY_WIDTH,
    .height = REWARD_MONEY_HEIGHT,
    .paletteNum = 15,
    .baseBlock = BLOCK_REWARD_MONEY,
};

static const struct WindowTemplate sWindowTemplate_Difficulty = 
{
    .bg = 0,
    .tilemapLeft = 16,
    .tilemapTop = 0,
    .width = 9,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = BLOCK_DIFFICULTY,
};

static const u8 sFontColor_Std[] = {0, 1, 2};

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

    return FALSE;
    return gSaveBlock1Ptr->questFlags[questId].active == TRUE;
}

bool8 Quest_IsQuestCompleted(u16 questId)
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    return TRUE;
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

bool8 Quest_AreQuestFlagsActive(u16 questId, u8 flags) 
{
    if (questId >= QUEST_COUNT)
        return FALSE;

    return (gSaveBlock1Ptr->questFlags[questId].flags & flags) == flags;
}

/**
 * @brief Called from STD_GIVE_QUEST_REWARD to load the data used withing the script
 * 
 */
void Quest_BufferQuestRewards()
{
    u16 questId;

    questId = VarGet(VAR_0x8000);
    
    if (questId >= QUEST_COUNT)
        questId = 0;
    
    VarSet(VAR_0x8001, sQuestList[questId].rewardItems[0].id);
    VarSet(VAR_0x8002, sQuestList[questId].rewardItems[0].amount);
    VarSet(VAR_0x8003, sQuestList[questId].rewardItems[1].id);
    VarSet(VAR_0x8004, sQuestList[questId].rewardItems[1].amount);
    VarSet(VAR_0x8005, sQuestList[questId].rewardMoney);
}

/**
 * @brief Shows the quest popup window with the quest information
 * 
 * @param questId 
 */
void Quest_ShowQuestPopup(u16 questId)
{
    if (sWindowHeaderId != 0 || sWindowHeaderReward != 0 || sWindowRewardsBackground != 0)
        return;

    // Add windows
    sWindowHeaderId = AddWindow(&sWindowTemplate_QuestHeader);
    sWindowHeaderReward = AddWindow(&sWindowTemplate_RewardsHeader);
    Quest_BuildRewardWindow(questId);
    sWindowDifficultyId = AddWindow(&sWindowTemplate_Difficulty);

    // print texts
    Quest_AddTextToWindow(sWindowHeaderId, 2, 0xFF, sQuestList[questId].name, 7, 0);
    Quest_AddTextToWindow(sWindowHeaderReward, 2, 0xEE, gText_RewardHeader, 7, 0);
    Quest_AddTextToWindow(sWindowRewardsBackground, 0, 0x11, gText_ExpandedPlaceholder_Empty, 0, 0);
    Quest_AddTextToWindow(sWindowDifficultyId, 0, 0xFF, gText_Difficulty, 4, 0);
    
    if (sWindowRewardMoney != 0)
        Quest_PrintRewardMoneyIntoLog(questId, sWindowRewardMoney, 20, 0);
    if (sWindowReward1Amount != 0)
        Quest_CreateRewardItem(questId, 0, sWindowReward1Amount, 24, 56, 0, 0, 1, 0x11);
    if (sWindowReward2Amount != 0)
        Quest_CreateRewardItem(questId, 1, sWindowReward2Amount, 24, 56 + 24, 0, 0, 1, 0x11);
    
    Quest_LoadDifficultyIconsToLogHeader();
    Quest_UpdateQuestDifficulty(questId);
}

/**
 * @brief Hides the quest popup window
 * 
 */
void Quest_HideQuestPopup(void)
{
    Quest_DestroyWindow(&sWindowHeaderId);
    Quest_DestroyWindow(&sWindowHeaderReward);
    Quest_DestroyWindow(&sWindowRewardsBackground);
    sWindowHeaderId = 0;
    sWindowHeaderReward = 0;
    sWindowRewardsBackground = 0;

    if (sWindowReward1Amount != 0)
    {
        Quest_DestroyRewardItemSprite(0);
        Quest_DestroyWindow(&sWindowReward1Amount);
        sWindowReward1Amount = 0;
    }
    if (sWindowReward2Amount != 0)
    {
        Quest_DestroyRewardItemSprite(1);
        Quest_DestroyWindow(&sWindowReward2Amount);
        sWindowReward2Amount = 0;
    }
    if (sWindowRewardMoney != 0)
    {
        Quest_DestroyWindow(&sWindowRewardMoney);
        sWindowRewardMoney = 0;
    }
    Quest_DestroyWindow(&sWindowDifficultyId);
    Quest_RemoveQuestDifficultyIconsAndFreeResources();
}

// Builds the reward window
static void Quest_BuildRewardWindow(u16 questId)
{
    u8 rewardCount, rewardBoxHeight, moneyTop;
    int i;
    struct WindowTemplate *window;
    
    rewardBoxHeight = 0;
    rewardCount = 0;

    for (i = 0; i < 2; i++)
        if (sQuestList[questId].rewardItems[i].amount != 0 && sQuestList[questId].rewardItems[i].id != ITEM_NONE)
        {
            rewardBoxHeight += 3;
            rewardCount++;
        }

    if (sQuestList[questId].rewardMoney != 0)
        rewardBoxHeight += 2;
    
    window = Alloc(sizeof(struct WindowTemplate));
    CpuCopy16(&sWindowTemplate_RewardsBackground, window, sizeof(struct WindowTemplate));
    window->height = rewardBoxHeight;
    sWindowRewardsBackground = AddWindow(window);
    Free(window);
    
    switch(rewardCount)
    {
        case 0:
            moneyTop = sWindowTemplate_Reward1Amount.tilemapTop - 1;
            break;
        case 1:
            sWindowReward1Amount = AddWindow(&sWindowTemplate_Reward1Amount);
            moneyTop = sWindowTemplate_Reward2Amount.tilemapTop - 1;
            break;
        case 2:
            sWindowReward1Amount = AddWindow(&sWindowTemplate_Reward1Amount);
            sWindowReward2Amount = AddWindow(&sWindowTemplate_Reward2Amount);
            moneyTop = sWindowTemplate_Reward2Amount.tilemapTop + 2;
            break;
    }

    if (sQuestList[questId].rewardMoney != 0)
    {
        window = Alloc(sizeof(struct WindowTemplate));
        CpuCopy16(&sWindowTemplate_RewardMoney, window, sizeof(struct WindowTemplate));
        window->tilemapTop = moneyTop;
        sWindowRewardMoney = AddWindow(window);
    }
}

static void Quest_AddTextToWindow(u8 windowId, u8 font, u8 fillColor, const u8 *text, u8 x, u8 y)
{
    StringExpandPlaceholders(gStringVar4, text);
    FillWindowPixelBuffer(windowId, fillColor);
    PutWindowTilemap(windowId);
    AddTextPrinterParameterized3(windowId, font, x, y, sFontColor_Std, -1, gStringVar4);
    CopyWindowToVram(windowId, 3);
}

static void Quest_DestroyWindow(u8 *windowId)
{
    ClearWindowTilemap(*windowId);
    CopyWindowToVram(*windowId, 3);
    RemoveWindow(*windowId);
    *windowId = 0;
}

