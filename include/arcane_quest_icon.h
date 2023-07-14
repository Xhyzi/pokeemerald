#ifndef GUARD_ARCANE_QUEST_ICON_H
#define GUARD_ARCANE_QUEST_ICON_H

struct MapQuest {
    u16 questId:15;
    u16 inUse:1;
    u8 indicatorId;
    u8 localId;
    u16 map;
};

bool8 Quest_TryLoadQuestIconSprite(u16 map, u8 localId, u8 objectEventId, s16 cameraX, s16 cameraY);
bool8 Quest_TryRemoveQuestIconSprite(u8 localId, u16 map);
bool8 Quest_TryLoadQuestIconSpritesByQuestId(u16 questId);
bool8 Quest_TryRemoveQuestIconSpritesByQuestId(u16 questId);
u8 Quest_TryGetQuestIconAndCopyStepData(struct ObjectEvent *objectEvent, struct Sprite *sprite);
void Quest_ResetMapQuestsDataStructure();
void Quest_RefreshQuestIcons(u16 questId);

#endif // GUARD_ARCANE_QUEST_ICON_H
