#ifndef GUARD_ARCANE_QUEST_MENU_H
#define GUARD_ARCANE_QUEST_MENU_H

enum {REWARD_MONEY, REWARD_ITEM, REWARD_POKEMON};
enum {QUEST_STATUS_INACTIVE, QUEST_STATUS_ACTIVE, QUEST_STATUS_COMPLETED};

#define QUEST_NAME_LENGTH 16
#define QUEST_DESCRIPTION_LENGTH 25 * 3
#define QUEST_LOCATION_LENGTH 17

#define NULL_NPC 0xFF
#define NULL_MAP 0xFFFF

#define DIFFICULTY_ICON_COUNT 5

struct QuestMenuData
{
    u8 npcSpriteId;
    u8 rewardId[2];
    u8 arrowId[2];
    u8 difficultyId[DIFFICULTY_ICON_COUNT];
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

struct RewardItem {
    u16 id;
    u16 amount;
};

struct QuestNpc {
    u8 localId;
    u16 map;
};

// represents the quest constant data
struct Quest {
    const u8 name[QUEST_NAME_LENGTH + 1];
    const u8 description[QUEST_DESCRIPTION_LENGTH + 1];
    const u8 location[QUEST_LOCATION_LENGTH + 1];
    u8 advisedLevel;
    u8 difficulty;
    const struct QuestNpc npc;
    const struct QuestNpc *extraNpcs;
    u32 rewardMoney;
    struct RewardItem rewardItems[2];
};

enum {SIZE_8, SIZE_16, SIZE_32, SIZE_64};

#define SLIDER_X        120
#define SLIDER_8_Y      24
#define SLIDER_8_END_Y  152
#define SLIDER_16_Y     28
#define SLIDER_16_END_Y 148
#define SLIDER_32_Y     36
#define SLIDER_32_END_Y 140
#define SLIDER_64_Y     52
#define SLIDER_64_END_Y 124

// slider positions depending on size
static const struct Slider sSliders[] = 
{
    [SIZE_8]    = {SLIDER_8_Y, SLIDER_8_END_Y},
    [SIZE_16]   = {SLIDER_16_Y, SLIDER_16_END_Y},
    [SIZE_32]   = {SLIDER_32_Y, SLIDER_32_END_Y},
    [SIZE_64]   = {SLIDER_64_Y, SLIDER_64_END_Y},
};

void Init_ArcaneQuestMenu(void);

#endif // GUARD_ARCANE_QUEST_MENU_H
