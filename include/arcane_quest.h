#ifndef GUARD_ARCANE_QUEST_H
#define GUARD_ARCANE_QUEST_H

#define QUEST_NAME_LENGTH 16
#define QUEST_DESCRIPTION_LENGTH 25 * 3
#define QUEST_LOCATION_LENGTH 17

#define NULL_NPC 0xFF
#define NULL_MAP 0xFFFF

#define MASK_FLAG_1     0x1
#define MASK_FLAG_2     0x2
#define MASK_FLAG_3     0x4
#define MASK_FLAG_4     0x8
#define MASK_FLAG_5     0x10
#define MASK_FLAG_6     0x20
#define MASK_ALL_FLAGS  0x3F

#define QUEST_FLAG_COUNT 6

enum {QUEST_SECONDARY, QUEST_PRIMARY};

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
    u8 type;
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

bool8 Quest_SetQuestAsActive(u16 questId);
bool8 Quest_SetQuestAsCompleted(u16 questId);
bool8 Quest_ClearQuestActive(u16 questId);
bool8 Quest_ClearQuestCompleted(u16 questId);
bool8 Quest_IsQuestActive(u16 questId);
bool8 Quest_IsQuestCompleted(u16 questId);
bool8 Quest_SetQuestFlag(u16 questId, u8 flagId);
bool8 Quest_ClearQuestFlag(u16 questId, u8 flagId);
bool8 Quest_IsQuestFlagActive(u16 questId, u8 flagId);

extern const struct Quest sQuestList[];
extern const u8 gQuestFlagMask[QUEST_FLAG_COUNT];

#endif // GUARD_ARCANE_QUEST_H
