#include "global.h"
#include "sprite.h"
#include "decompress.h"
#include "palette.h"
#include "graphics.h"
#include "arcane_quest_icon.h"
#include "arcane_quest_menu.h"
#include "event_object_movement.h"
#include "malloc.h"
#include "constants/items.h"
#include "constants/quest.h"
#include "arcane_quest.h"

static void SpriteCB_QuestIcon(struct Sprite* sprite);
static s16 Quest_GetNpcQuestIndex(u8 localId, u16 map, u8 *iconType);
static bool8 Quest_IsQuestNpcIconAlreadyLoaded(u8 questId, u8 localId);
static u8 Quest_GetEventObjectIndex(u8 localId, u16 map);
static u8 Quest_LoadQuestIconSprite(u8 questId, u8 localId, u8 questPos, u8 objectEventId, s16 cameraX, s16 cameraY, u8 iconType);

#define TAG_ICON_ARROW_PRIMARY      0x7777
#define TAG_ICON_CROSS_PRIMARY      0x7778
#define TAG_ICON_DOTS_PRIMARY       0x7779
#define TAG_ICON_QUESTION_PRIMARY   0x777A
#define TAG_ICON_NEW_PRIMARY        0x777B
#define TAG_ICON_ARROW_SECONDARY    0x777C
#define TAG_ICON_CROSS_SECONDARY    0x777D
#define TAG_ICON_DOTS_SECONDARY     0x777E
#define TAG_ICON_QUESTION_SECONDARY 0x777F
#define TAG_ICON_NEW_SECONDARY      0x7780
#define TAG_ICON_QUESTION_NOT_READY 0x7781

#define PAL_TAG_QUEST_ICON          0x7777   

static const struct CompressedSpriteSheet sSpriteSheet_PrimaryArrowIcon = {
    .data = gSpriteGfx_QuestIconArrowPrimary,
    .size = 0x80,
    .tag = TAG_ICON_ARROW_PRIMARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_PrimaryCrossIcon = {
    .data = gSpriteGfx_QuestIconCrossPrimary,
    .size = 0x80,
    .tag = TAG_ICON_CROSS_PRIMARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_PrimaryDotsIcon = {
    .data = gSpriteGfx_QuestIconDotsPrimary,
    .size = 0x80,
    .tag = TAG_ICON_DOTS_PRIMARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_PrimaryQuestionIcon = {
    .data = gSpriteGfx_QuestIconQuestionPrimary,
    .size = 0x80,
    .tag = TAG_ICON_QUESTION_PRIMARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_PrimaryNewIcon = {
    .data = gSpriteGfx_QuestIconNewPrimary,
    .size = 0x200,
    .tag = TAG_ICON_NEW_PRIMARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_SecondaryArrowIcon = {
    .data = gSpriteGfx_QuestIconArrowSecondary,
    .size = 0x80,
    .tag = TAG_ICON_ARROW_SECONDARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_SecondaryCrossIcon = {
    .data = gSpriteGfx_QuestIconCrossSecondary,
    .size = 0x80,
    .tag = TAG_ICON_CROSS_SECONDARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_SecondaryDotsIcon = {
    .data = gSpriteGfx_QuestIconDotsSecondary,
    .size = 0x80,
    .tag = TAG_ICON_DOTS_SECONDARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_SecondaryQuestionIcon = {
    .data = gSpriteGfx_QuestIconQuestionSecondary,
    .size = 0x80,
    .tag = TAG_ICON_QUESTION_SECONDARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_SecondaryNewIcon = {
    .data = gSpriteGfx_QuestIconNewSecondary,
    .size = 0x200,
    .tag = TAG_ICON_NEW_SECONDARY,
};

static const struct CompressedSpriteSheet sSpriteSheet_QuestionNotReadyIcon = {
    .data = gSpriteGfx_QuestIconQuestionNotReady,
    .size = 0x80,
    .tag = TAG_ICON_QUESTION_NOT_READY,
};

static const struct SpritePalette sSpritePalette_QuestIcon = {
    .data = gSpritePal_QuestIcon,
    .tag = PAL_TAG_QUEST_ICON,
};

static const struct CompressedSpriteSheet *const sSpriteSheet_ArrowIcons[] = {
    [QUEST_PRIMARY] = &sSpriteSheet_PrimaryArrowIcon,
    [QUEST_SECONDARY] = &sSpriteSheet_SecondaryArrowIcon,
};

static const struct CompressedSpriteSheet *const sSpriteSheet_CrossIcons[] = {
    [QUEST_PRIMARY] = &sSpriteSheet_PrimaryCrossIcon,
    [QUEST_SECONDARY] = &sSpriteSheet_SecondaryCrossIcon,
};

static const struct CompressedSpriteSheet *const sSpriteSheet_DotsIcons[] = {
    [QUEST_PRIMARY] = &sSpriteSheet_PrimaryDotsIcon,
    [QUEST_SECONDARY] = &sSpriteSheet_SecondaryDotsIcon,
};

static const struct CompressedSpriteSheet *const sSpriteSheet_QuestionIcons[] = {
    [QUEST_PRIMARY] = &sSpriteSheet_PrimaryQuestionIcon,
    [QUEST_SECONDARY] = &sSpriteSheet_SecondaryQuestionIcon,
};

static const struct CompressedSpriteSheet *const sSpriteSheet_NewIcons[] = {
    [QUEST_PRIMARY] = &sSpriteSheet_PrimaryNewIcon,
    [QUEST_SECONDARY] = &sSpriteSheet_SecondaryNewIcon,
};

static const struct OamData sOamData_QuestIcon_16x16 = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .bpp = ST_OAM_4BPP,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 1,
};

static const struct OamData sOamData_QuestIcon_32x32 = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .bpp = ST_OAM_4BPP,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const struct SpriteTemplate sSpriteTemplate_QuestIcon = {
    .tileTag = TAG_ICON_ARROW_PRIMARY,
    .paletteTag = PAL_TAG_QUEST_ICON,
    .oam = &sOamData_QuestIcon_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_QuestIcon,
};

#define QUESTS_PER_MAP  10

static EWRAM_DATA struct MapQuest sMapQuests[QUESTS_PER_MAP] = {0}; 

#define sTimer      data[0]
#define sQuestPos   data[1]
#define sLocalId    data[2]
#define sLoaded     data[6]
// these 3 are used to adjust the sprite's position when it's associated object event moves
#define sMovementDirection data[3]
#define sMovementSpeed     data[4]
#define sMovementTimer     data[5]

/**
 * @brief Tries to load the quest sprite for the given quest ID and NPC number. Returns True if successful.
 * it ajusts the icon sprite position based on the associated object event and camera position.
 * This function should be called whenever an event object is loaded (to load the quest icon sprite if necessary)
 * 
 * @param map The map number.
 * @param localId The NPC number in the given map. 
 * @param objectEventId associated with the NPC.
 * @param cameraX camera X position.
 * @param cameraY camera Y position.
 * @return bool8 
 */
bool8 Quest_TryLoadQuestIconSprite(u16 map, u8 localId, u8 objectEventId, s16 cameraX, s16 cameraY)
{
    int i;
    u8 iconType = ICON_NORMAL;  // TODO: add iconType depending on where the npc was found
    s16 questId = Quest_GetNpcQuestIndex(localId, map, &iconType); // &iconType;
    
    // TODO: add "|| QuestInactive && iconType == marker
    // If no quest is found or if the quest has already been completed or quest is already loaded, return.
    if (questId == -1 || Quest_IsQuestCompleted(questId) || Quest_IsQuestNpcIconAlreadyLoaded(questId, localId))
        return FALSE;

    // Find an empty slot in the map quest array.
    for (i = 0; i < ARRAY_COUNT(sMapQuests); i++)
        if (!sMapQuests[i].inUse)
        {
            // TODO: Care! multiple npc can have the same questId
            sMapQuests[i].inUse = TRUE;
            sMapQuests[i].questId = questId;
            sMapQuests[i].localId = localId;
            sMapQuests[i].map = map;
            sMapQuests[i].indicatorId = Quest_LoadQuestIconSprite(questId, localId, i, objectEventId, cameraX, cameraY, iconType);
            return TRUE;
        }

    return FALSE;
}

/**
 * @brief Tries to remove the quest icon sprite associated with the given NPC number and map number if any
 * After removing the sprite, it clears the sMapQuest array entry.
 * This function should be called whenever an event object is unloaded (to remove the quest icon sprite if necessary)
 * 
 * @param localId 
 * @param map 
 * @return bool8 returns true if the sprite was removed, false otherwise
 */
bool8 Quest_TryRemoveQuestIconSprite(u8 localId, u16 map)
{
    int i;

    for (i = 0; i < ARRAY_COUNT(sMapQuests); i++)
        if (sMapQuests[i].inUse && sMapQuests[i].localId == localId 
            && sMapQuests[i].map == map)
        {
            DestroySprite(&gSprites[sMapQuests[i].indicatorId]);
            sMapQuests[i].inUse = FALSE;
            return TRUE;
        }
    return FALSE;
}

/**
 * @brief Tries to load the quest icon sprite for the given quest ID. Returns True if successful.
 * This function should be called whenever a quest status is changed (to load the required quest icon sprite if necessary)
 * 
 * @param questId 
 * @return bool8 
 */
bool8 Quest_TryLoadQuestIconSpritesByQuestId(u16 questId)
{
    int i, j;
    u8 map, mapGroup, localId;
    const struct QuestNpc *extraNpcs;
    const struct QuestSecondaryMarker *secondaryMarkers;

    // TODO: add support for multiple npcs and secondary icons
     for (i = 0; i < ARRAY_COUNT(gObjectEvents); i++)
    {
        extraNpcs = sQuestList[questId].extraNpcs;
        secondaryMarkers = sQuestList[questId].secondaryMarkers;

        map = sQuestList[questId].npc.map & 0xFF;
        mapGroup = sQuestList[questId].npc.map >> 8 & 0xFF;
        localId = sQuestList[questId].npc.localId;
        // checks if the event object of the given questId is currently active in the map
        // if so, generates the quest sprite associated to that object event
        if (gObjectEvents[i].localId == localId && gObjectEvents[i].mapNum == map 
            && gObjectEvents[i].mapGroup == mapGroup && gObjectEvents[i].active)
        {
            Quest_TryLoadQuestIconSprite(map, localId, i, gCamera.x, gCamera.y);
        }
        
        if (extraNpcs != NULL)
        {
            j = 0;
            while (extraNpcs[j].localId != NULL_NPC && extraNpcs[j].map != NULL_MAP)
            {
                localId = extraNpcs[j].localId;
                map = extraNpcs[j].map & 0xFF;
                mapGroup = extraNpcs[j].map >> 8 & 0xFF;
                if (gObjectEvents[i].localId == localId && gObjectEvents[i].mapNum == map 
                    && gObjectEvents[i].mapGroup == mapGroup && gObjectEvents[i].active)
                {
                    Quest_TryLoadQuestIconSprite(map, localId, i, gCamera.x, gCamera.y);
                }
                j++;
            }
        }
        
        if (secondaryMarkers != NULL)
        {
            j = 0;
            while (secondaryMarkers[j].localId != NULL_NPC && secondaryMarkers[j].map != NULL_MAP)
            {
                localId = secondaryMarkers[j].localId;
                map = secondaryMarkers[j].map & 0xFF;
                mapGroup = secondaryMarkers[j].map >> 8 & 0xFF;
                if (gObjectEvents[i].localId == localId && gObjectEvents[i].mapNum == map 
                    && gObjectEvents[i].mapGroup == mapGroup && gObjectEvents[i].active)
                {
                    Quest_TryLoadQuestIconSprite(map, localId, i, gCamera.x, gCamera.y);
                }
                j++;
            }
        }
    }
}

/**
 * @brief Tries to remove all the quest icon sprites associated with the given quest ID.
 * This function should be called whenever a quest status is changed (to remove the required quest icon sprite if necessary)
 * 
 * @param questId 
 * @return bool8 
 */
bool8 Quest_TryRemoveQuestIconSpritesByQuestId(u16 questId)
{
    bool8 removed = FALSE;
    int i;

    for (i = 0; i < ARRAY_COUNT(sMapQuests); i++)
        if (sMapQuests[i].inUse && sMapQuests[i].questId == questId)
        {
            DestroySprite(&gSprites[sMapQuests[i].indicatorId]);
            sMapQuests[i].inUse = FALSE;
            removed = TRUE;
        }
    
    return removed;
}

/**
 * @brief Tries to find the associated quest icon sprite for the given object event
 * and if so, copies the object event step data to the icon sprite.
 * This quest should be called whenever an event object step action is called (to update the quest icon sprite if necessary)
 * 
 * @param objectEvent 
 * @param sprite 
 * @return u8 
 */
u8 Quest_TryGetQuestIconAndCopyStepData(struct ObjectEvent *objectEvent, struct Sprite *sprite)
{
    int i;
    u8 localId = objectEvent->localId;
    u16 map = objectEvent->mapNum | objectEvent->mapGroup << 8;

    for (i = 0; i < ARRAY_COUNT(sMapQuests); i++)
    {
        if (sMapQuests[i].inUse && sMapQuests[i].localId == localId && sMapQuests[i].map == map)
        {
            u8 spriteId = sMapQuests[i].indicatorId;
            gSprites[spriteId].sMovementDirection = sprite->sMovementDirection;
            gSprites[spriteId].sMovementSpeed = sprite->sMovementSpeed;
            gSprites[spriteId].sMovementTimer = sprite->sMovementTimer;
            return spriteId;
        }
    }

    return MAX_SPRITES;
}

/**
 * @brief Returns the quest ID for the given map and NPC number. Returns -1 if no quest is found.
 * 
 * @param localId npc number within the map.
 * @param map map number.
 * @return s16 
 */
static s16 Quest_GetNpcQuestIndex(u8 localId, u16 map, u8 *iconType)
{
    int i, j;
    const struct QuestNpc *extraNpcs;
    const struct QuestSecondaryMarker *secondaryMarkers;

    for (i = 0; i < QUEST_COUNT; i++)
    {
        if (sQuestList[i].npc.map == map && sQuestList[i].npc.localId == localId)
            return i;
        extraNpcs = sQuestList[i].extraNpcs;
        
        // checks if the given npc is an extra npc for the quest
        j = 0;
        if (extraNpcs != NULL)
            while (extraNpcs[j].map != NULL_MAP && extraNpcs[j].localId != NULL_NPC)
            {
                if (extraNpcs[j].map == map && extraNpcs[j].localId == localId)
                    return i;
                j++;
            }
        secondaryMarkers = sQuestList[i].secondaryMarkers;
        
        // checks if the given npc is a secondary marker for the quest
        j = 0;
        if (secondaryMarkers != NULL)
            while (secondaryMarkers[j].map != NULL_MAP && secondaryMarkers[j].localId != NULL_NPC)
            {
                if (secondaryMarkers[j].map == map && secondaryMarkers[j].localId == localId
                    && Quest_IsQuestActive(j) && !Quest_AreQuestFlagsActive(j, secondaryMarkers[j].flags))
                {
                    *iconType = secondaryMarkers[j].iconType;
                    return j;
                }
                j++;
            }
    }
    return -1;
}

/**
 * @brief Checks if the quest is already loaded in the map structure.
 * 
 * @param questId 
 * @param localId 
 * @return bool8 
 */
// TODO: change, a quest can have multiple instances in sMapQuests
static bool8 Quest_IsQuestNpcIconAlreadyLoaded(u8 questId, u8 localId)
{
    int i;

    for (i = 0; i < ARRAY_COUNT(sMapQuests); i++)
        if (sMapQuests[i].inUse 
        && sMapQuests[i].questId == questId 
        && sMapQuests[i].localId == localId)
            return TRUE;
    return FALSE;
}

/**
 * @brief Loads a quest icon sprite for the given questId and associated object event.
 * The sprite position is fixed above the object event, taking into consideration the camera position.
 * 
 * @param questId 
 * @param localId 
 * @param questPos 
 * @param objectEventId 
 * @param cameraX 
 * @param cameraY 
 * @return u8 
 */
static u8 Quest_LoadQuestIconSprite(u8 questId, u8 localId, u8 questPos, u8 objectEventId, s16 cameraX, s16 cameraY, u8 iconType)
{
    u16 width;
    struct SpriteTemplate *template;
    struct Sprite *sprite;
    struct ObjectEvent *objectEvent;
    u8 spriteId;
    
    LoadSpritePalette(&sSpritePalette_QuestIcon);

    template = Alloc(sizeof(struct SpriteTemplate));
    CpuCopy16(&sSpriteTemplate_QuestIcon, template, sizeof(*template));    

    switch(iconType)
    {
        default:
        case ICON_NORMAL:
            if (Quest_IsQuestActive(questId))
            {
                if (sQuestList[questId].isQuestReadyCB != NULL && sQuestList[questId].isQuestReadyCB() == FALSE)
                {
                    template->tileTag = sSpriteSheet_QuestionNotReadyIcon.tag;
                    if (!IsSpriteTagAllocated(template->tileTag))
                        LoadCompressedSpriteSheet(&sSpriteSheet_QuestionNotReadyIcon);
                }
                else
                {
                    template->tileTag = sSpriteSheet_QuestionIcons[sQuestList[questId].type]->tag;
                    if (!IsSpriteTagAllocated(template->tileTag))
                        LoadCompressedSpriteSheet(sSpriteSheet_QuestionIcons[sQuestList[questId].type]);
                }
                width = 16;
            }
            else
            {
                template->tileTag = sSpriteSheet_NewIcons[sQuestList[questId].type]->tag;
                template->oam = &sOamData_QuestIcon_32x32;
                if (!IsSpriteTagAllocated(template->tileTag))
                    LoadCompressedSpriteSheet(sSpriteSheet_NewIcons[sQuestList[questId].type]);
                width = 32;
            }
            break;

        case ICON_ARROW:
            template->tileTag = sSpriteSheet_ArrowIcons[sQuestList[questId].type]->tag;
            if (!IsSpriteTagAllocated(template->tileTag))
                LoadCompressedSpriteSheet(sSpriteSheet_ArrowIcons[sQuestList[questId].type]);
            width = 16;
            break;
        
        case ICON_CROSS:
            template->tileTag = sSpriteSheet_CrossIcons[sQuestList[questId].type]->tag;
            if (!IsSpriteTagAllocated(template->tileTag))
                LoadCompressedSpriteSheet(sSpriteSheet_CrossIcons[sQuestList[questId].type]);
            width = 16;
            break;

        case ICON_DOTS:
            template->tileTag = sSpriteSheet_DotsIcons[sQuestList[questId].type]->tag;
            if (!IsSpriteTagAllocated(template->tileTag))
                LoadCompressedSpriteSheet(sSpriteSheet_DotsIcons[sQuestList[questId].type]);
            width = 16;
            break;
    }

    spriteId = CreateSprite(template, 0, 0, 100);

    gSprites[spriteId].sTimer = 0;
    gSprites[spriteId].sLocalId = localId;
    gSprites[spriteId].sQuestPos = questPos;

    // fix sprite position with camera
    sprite = &gSprites[spriteId];
    objectEvent = &gObjectEvents[objectEventId];
    GetMapCoordsFromSpritePos(objectEvent->currentCoords.x + cameraX, objectEvent->currentCoords.y + cameraY, &sprite->x, &sprite->y);
    
    sprite->centerToCornerVecX = - (width >> 1);
    sprite->centerToCornerVecY = - (width >> 1);
    sprite->x += 8;
    sprite->y += sprite->centerToCornerVecY - 2;
    gSprites[spriteId].coordOffsetEnabled = TRUE;
    return spriteId;
}

/**
 * @brief This callback controlls the quest icon sprite visibility and animation.
 * 
 * @param sprite 
 */
static void SpriteCB_QuestIcon(struct Sprite *sprite) 
{
    u8 eIndex = Quest_GetEventObjectIndex(sprite->sLocalId, sQuestList[sMapQuests[sprite->sQuestPos].questId].npc.map);

    if (eIndex != 255 && gObjectEvents[eIndex].active)
        sprite->invisible = FALSE;
    else
        sprite->invisible = TRUE;

    switch(sprite->sTimer % 64)
    {
        case 0:
        case 8:
        case 16:
        case 24:
            sprite->y -= 1;
            break;
        case 32:
        case 40:
        case 48:
        case 56:
            sprite->y += 1;
            break;
        default:
            break;
    }
    sprite->sTimer++;
}

/**
 * @brief Tries to find a given object event index within the object event array.
 * Returns -1 if not found.
 * 
 * @param localId 
 * @param map 
 * @return u8 
 */
static u8 Quest_GetEventObjectIndex(u8 localId, u16 map)
{
    int i;

    for (i = 0; i < ARRAY_COUNT(gObjectEvents); i++)
    {
        u16 emap = gObjectEvents[i].mapNum | (gObjectEvents[i].mapGroup << 8);
        if (emap == map && gObjectEvents[i].localId == localId)
            return i;
    }

    return -1;
}

/**
 * @brief This function has to be called from ResetSpriteData
 * in order to reset the quest icon sprites data structure when the sprites are cleared
 * 
 */
void Quest_ResetMapQuestsDataStructure()
{
    int i;

    for (i = 0; i < ARRAY_COUNT(sMapQuests); i++)
    {
        sMapQuests[i].questId = 0;
        sMapQuests[i].inUse = FALSE;
        sMapQuests[i].localId = 0;
        sMapQuests[i].indicatorId = 0;
    }
}

void Quest_RefreshQuestIcons(u16 questId)
{
    Quest_TryRemoveQuestIconSpritesByQuestId(questId);  // despawn quest icons
    Quest_TryLoadQuestIconSpritesByQuestId(questId);    // spawn new quest icons
}
