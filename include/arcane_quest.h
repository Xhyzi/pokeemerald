#ifndef GUARD_ARCANE_QUEST_H
#define GUARD_ARCANE_QUEST_H

#define QUEST_NAME_LENGTH 22
#define QUEST_DESCRIPTION_LENGTH 25 * 3
#define QUEST_LOCATION_LENGTH 17

#define NULL_NPC 0xFF
#define NULL_MAP 0xFFFF

#define QFLAG_0         0x1
#define QFLAG_1         0x2
#define QFLAG_2         0x4
#define QFLAG_3         0x8
#define QFLAG_4         0x10
#define QFLAG_5         0x20
#define MASK_ALL_FLAGS  0x3F

#define QUEST_FLAG_COUNT 6

enum {QUEST_SECONDARY, QUEST_PRIMARY};
enum {ICON_NORMAL, ICON_ARROW, ICON_DOTS, ICON_CROSS};

typedef bool8 (*IsQuestReadyCB)(void);

struct RewardItem {
    u16 id;
    u16 amount;
};

// TODO: añadir flags en las que se activa el icono
struct QuestNpc {
    u8 localId;
    u16 map;
};

struct QuestSecondaryMarker {
    u8 localId;
    u16 map;
    u8 flags:6;
    u8 iconType:2;
};

/**
 * @brief Quest structure, represents a quest.
 *  - type: is the type of quest, can be QUEST_PRIMARY or QUEST_SECONDARY.
 *  - name: name of the quest.
 *  - description: description of the quest, 24 characters per line, 3 lines.
 *  - location: location of the quest, 16 characters.
 *  - advisedLevel: level advised to complete the quest.
 *  - difficulty: difficulty of the quest, can go from 1 to 5.
 *  - npc: npc that gives the quest, it's a pair (localid, map).
 *  - extraNpcs: extra npcs that gives or recieve the quest, it's a pair (localid, map).
 *          This entry is default to NULL, a table of extra npcs can be linked.
 *          The last entry of this table should have the localId set to NULL_NPC and the map to NULL_MAP. (0xFF, 0xFFFF)
 *  - secondaryMarkers: secondary markers of the quest, (localId, map, flags, iconType).
 *          This entry is default to NULL, a table of secondary markers can be linked.
 *          The last entry of this table should have the localId set to NULL_NPC and the map to NULL_MAP. (0xFF, 0xFFFF)
 *          The flags are the flags that when active, will hide the marker.
 *          The iconType is the type of icon that will be shown, can be ICON_ARROW, ICON_DOTS or ICON_CROSS.
 *  - rewardMoney: money reward of the quest, can be set to 0.
 *  - rewardItems: items reward of the quest, can be set to ITEM_NONE or amount = 0.        
 *  - isQuestReadyCB: callback that allows to check if the quest can be completed.
 *          By default is set to NULL, if you want to link a callback it has to be designed ad hoc for each quest at
 *          arcane_quest_callbacks.c    
 */
// TODO: make QuestNpc have types (GIVER, RECIEVER, BOTH)
struct Quest {
    u8 type;
    const u8 name[QUEST_NAME_LENGTH + 1];
    const u8 description[QUEST_DESCRIPTION_LENGTH + 1];
    const u8 location[QUEST_LOCATION_LENGTH + 1];
    u8 advisedLevel;
    u8 difficulty;
    const struct QuestNpc npc;
    const struct QuestNpc *extraNpcs;
    const struct QuestSecondaryMarker *secondaryMarkers;
    u32 rewardMoney;
    struct RewardItem rewardItems[2];
    IsQuestReadyCB isQuestReadyCB;  // callback that allows to check if the quest can be completed
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
bool8 Quest_AreQuestFlagsActive(u16 questId, u8 flags);

extern const struct Quest sQuestList[];

#endif // GUARD_ARCANE_QUEST_H
