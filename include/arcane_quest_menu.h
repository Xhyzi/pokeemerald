#ifndef GUARD_ARCANE_QUEST_MENU_H
#define GUARD_ARCANE_QUEST_MENU_H

#define DIFFICULTY_ICON_COUNT 5

#define SLIDER_X        120
#define SLIDER_8_Y      24
#define SLIDER_8_END_Y  152
#define SLIDER_16_Y     28
#define SLIDER_16_END_Y 148
#define SLIDER_32_Y     36
#define SLIDER_32_END_Y 140
#define SLIDER_64_Y     52
#define SLIDER_64_END_Y 124

struct QuestMenuData
{
    u8 npcSpriteId;
    u8 arrowId[2];
    u8 selectedSlot;
    u16 loadedQuestsOffset;
    u16 *activeQuests;
    u16 activeQuestCount;
    u16 loadedQuests[8];
};

struct Slider
{
    u8 start_y;
    u8 end_y;
};

enum {SIZE_8, SIZE_16, SIZE_32, SIZE_64};

// slider positions depending on size
static const struct Slider sSliders[] = 
{
    [SIZE_8]    = {SLIDER_8_Y, SLIDER_8_END_Y},
    [SIZE_16]   = {SLIDER_16_Y, SLIDER_16_END_Y},
    [SIZE_32]   = {SLIDER_32_Y, SLIDER_32_END_Y},
    [SIZE_64]   = {SLIDER_64_Y, SLIDER_64_END_Y},
};

void Init_ArcaneQuestMenu(void);
void Quest_CreateRewardItem(u16 questId, u8 slot, u8 windowId, s16 x, s16 y, u8 txtOffsetX, u8 txtOffsetY, u8 fontColor, u8 fillValue);
void Quest_DestroyRewardItemSprite(u8 slot);
bool8 Quest_PrintRewardMoneyIntoLog(u16 questId, u8 windowId, s16 x, s16 y);
void Quest_UpdateQuestDifficulty(u16 questId);
void Quest_LoadDifficultyIconsToLogHeader();
void Quest_RemoveQuestDifficultyIconsAndFreeResources();

#endif // GUARD_ARCANE_QUEST_MENU_H
