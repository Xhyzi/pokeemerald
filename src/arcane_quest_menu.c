#include "global.h"
#include "bg.h"
#include "decompress.h"
#include "event_object_movement.h"
#include "event_object_movement.h"
#include "graphics.h"
#include "item_icon.h"
#include "landmark.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/items.h"
#include "constants/quest.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "data/text/quest.h"
#include "arcane_quest_menu.h"
#include "arcane_quest.h"

// Init and control section
static void CB2_WaitUntilFadeInitQuestMenu();
static void CB2_InitQuestMenu();
static void CB2_QuestMenu();
static void VBlankCB_QuestMenu();
static void Quest_InitData();
static void Quest_InitBgs();
static void Quest_LoadBgGfx();
static void Quest_InitWindows();
// Control section
static void Task_HandleQuestMenuInput(u8 taskId);
static void Task_WaitFadeReturnToField(u8 taskId);
static bool8 Quest_HandleDownInput();
static bool8 Quest_HandleUpInput();
static void Quest_UpdateMenuAfterScroll();
static u16 Quest_GetActiveQuestCount();
static void Quest_BuildLoadedQuests();
static void Quest_HighlightSelectedCursor();
static void Quest_RemoveEmptyQuestGUISlots();
static u8 Quest_CalcDifficultyLevel(u16 questId);
// Sprite section
static void Quest_LoadSpriteGfx();
static void Quest_CreateSprites();
static void Quest_CreateSlider();
static void SpriteCB_Arrow(struct Sprite *sprite);
static void SpriteCB_Slider(struct Sprite *sprite);
static void Quest_UpdateRewardIconSprite(u16 itemId, u8 amount, u8 id, s16 x, s16 y);
static void Quest_UpdateNpcSprite(u16 questId);
static void Quest_UpdateQuestDifficulty(u16 questId);
// Text section
static void Quest_PrintConstantMenuTexts();
static void Quest_BuildAndPrintLoadedQuestNames();
static void Quest_PrintSelectedQuestInfo();
static void Quest_PrintText(u8 windowId, u8 x, u8 y, const u8* text, u8 font);
static void Quest_PrintDarkText(u8 windowId, u8 x, u8 y, const u8* text, u8 font);
static void Quest_PrintBufferedText(u8 windowId, u8 x, u8 y, u8 font, u8 fontColor);
static u8* Quest_BuildNumberText(u32 number, u8 digits);
static bool8 Quest_PrintRewardMoney(u16 questId);
static bool8 Quest_PrintRewardItemCount(u16 itemId, u16 amount, u8 slot);
static void Quest_UpdateRewardItem(u16 questId, u8 slot);

// Animations
static const union AnimCmd sAnimCmd_ArrowUp[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_ArrowDown[] =
{
    ANIMCMD_FRAME(0, 0, .vFlip = TRUE),
    ANIMCMD_END,
};

enum {ARROW_UP, ARROW_DOWN};
static const union AnimCmd *const sAnimTable_Arrow[] =
{
    [ARROW_UP] = sAnimCmd_ArrowUp,
    [ARROW_DOWN] = sAnimCmd_ArrowDown,
};

static const union AnimCmd sAnimCmd_DifficultyTrivial[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_DifficultyEasy[] =
{
    ANIMCMD_FRAME(1, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_DifficultyMedium[] =
{
    ANIMCMD_FRAME(2, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_DifficultyHard[] =
{
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_DifficultyDangerous[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END,
};

enum {DIFFICULTY_TRIVIAL, DIFFICULTY_EASY, DIFFICULTY_MEDIUM, DIFFICULTY_HARD, DIFFICULTY_DANGEROUS};
static const union AnimCmd *const sAnimTable_Difficulty[] =
{
    [DIFFICULTY_TRIVIAL] = sAnimCmd_DifficultyTrivial,
    [DIFFICULTY_EASY] = sAnimCmd_DifficultyEasy,
    [DIFFICULTY_MEDIUM] = sAnimCmd_DifficultyMedium,
    [DIFFICULTY_HARD] = sAnimCmd_DifficultyHard,
    [DIFFICULTY_DANGEROUS] = sAnimCmd_DifficultyDangerous,
};

// Graphics
#define TAG_SLIDER          0x2145
#define TAG_ARROW           0x2146
#define TAG_DIFFICULTY_ICON 0x2147
#define TAG_ITEM_ICON       5110
#define TAG_SCROLL_ARROW    5112

#define PAL_TAG_SLIDER          0x2145
#define PAL_TAG_ARROW           0x2146
#define PAL_TAG_DIFFICULTY_ICON 0x2147

static const struct CompressedSpriteSheet sSpriteSheet_Slider_8 = 
{
    .data = gSpriteGfx_Slider_8,
    .size = 0x20,
    .tag = TAG_SLIDER,
};

static const struct CompressedSpriteSheet sSpriteSheet_Slider_16 = 
{
    .data = gSpriteGfx_Slider_16,
    .size = 0x80,
    .tag = TAG_SLIDER,
};

static const struct CompressedSpriteSheet sSpriteSheet_Slider_32 = 
{
    .data = gSpriteGfx_Slider_32,
    .size = 0x100,
    .tag = TAG_SLIDER,
};

static const struct CompressedSpriteSheet sSpriteSheet_Slider_64 = 
{
    .data = gSpriteGfx_Slider_64,
    .size = 0x400,
    .tag = TAG_SLIDER,
};

static const struct CompressedSpriteSheet sSpriteSheet_Arrow = 
{
    .data = gSpriteGfx_Arrow,
    .size = 0x40,
    .tag = TAG_ARROW,
};

static const struct CompressedSpriteSheet sSpriteSheet_DifficultyIcon = 
{
    .data = gSpriteGfx_DifficultyIcon,
    .size = 0xA0,
    .tag = TAG_DIFFICULTY_ICON,
};

static const struct SpritePalette sSpritePalette_Slider = 
{
    .data = sSpritePal_Slider,
    .tag = PAL_TAG_SLIDER,
};

static const struct SpritePalette sSpritePalette_Arrow = 
{
    .data = gSpritePal_Arrow,
    .tag = PAL_TAG_ARROW,
};

static const struct SpritePalette sSpritePalette_DifficultyIcon = 
{
    .data = gSpritePal_DifficultyIcon,
    .tag = PAL_TAG_DIFFICULTY_ICON,
};

static const struct OamData sOamData_Slider_8 = 
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .bpp = ST_OAM_4BPP,
    .priority = 0,
};

static const struct OamData sOamData_Slider_16 = 
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .bpp = ST_OAM_4BPP,
    .priority = 0,
};

static const struct OamData sOamData_Slider_32 = 
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(16x32),
    .size = SPRITE_SIZE(16x32),
    .bpp = ST_OAM_4BPP,
    .priority = 0,
};

static const struct OamData sOamData_Slider_64 = 
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x64),
    .size = SPRITE_SIZE(32x64),
    .bpp = ST_OAM_4BPP,
    .priority = 0,
};

static const struct OamData sOamData_Arrow = 
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(16x8),
    .size = SPRITE_SIZE(16x8),
    .bpp = ST_OAM_4BPP,
    .priority = 0,
};

static const struct OamData sOamData_DifficultyIcon = 
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .bpp = ST_OAM_4BPP,
    .priority = 0,
};

static const struct SpriteTemplate sSpriteTemplate_Arrow = 
{
    .tileTag = TAG_ARROW,
    .paletteTag = PAL_TAG_ARROW,
    .oam = &sOamData_Arrow,
    .anims = sAnimTable_Arrow,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Arrow,
};

static const struct SpriteTemplate sSpriteTemplate_DifficultyIcon =
{
    .tileTag = TAG_DIFFICULTY_ICON,
    .paletteTag = PAL_TAG_DIFFICULTY_ICON,
    .oam = &sOamData_DifficultyIcon,
    .anims = sAnimTable_Difficulty,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

enum {BG_0, BG_1, BG_2, BG_3};
enum {W_HEADER, W_HEADER_CONTROL, W_QUEST_SLOT_1, W_QUEST_SLOT_2, W_QUEST_SLOT_3, W_QUEST_SLOT_4, 
    W_QUEST_SLOT_5, W_QUEST_SLOT_6, W_QUEST_SLOT_7, W_QUEST_SLOT_8,
    W_QUEST_LOCATION, W_QUEST_DESC,W_REWARD_MONEY, W_REWARD_1, W_REWARD_2,
    W_MONEY_HEADER, W_LEVEL_HEADER, W_ERR_MSG};

#define HEADER_WIDTH 8
#define HEADER_HEIGHT 2
#define QUEST_WIDTH 14
#define QUEST_HEIGHT 2
#define QUEST_NAME_WIDTH 12
#define QUEST_DESC_WIDTH 15
#define QUEST_DESC_HEIGHT 6
#define QUEST_REWARD_WIDTH 8
#define QUEST_REWARD_HEIGHT 2
#define QUEST_MONEY_HEADER_WIDTH 6
#define QUEST_MONEY_HEADER_HEIGHT 2
#define QUEST_HEADER_CONTRO_WIDTH 8
#define QUEST_LEVEL_WIDTH 6
#define QUEST_LEVEL_HEIGHT 2

// Windows memory directions
#define BLOCK_HEADER 1
#define BLOCK_HEADER_CONTROL    BLOCK_HEADER + HEADER_WIDTH * HEADER_HEIGHT
#define BLOCK_QUEST_SLOT_1      BLOCK_HEADER_CONTROL + QUEST_HEADER_CONTRO_WIDTH * HEADER_HEIGHT
#define BLOCK_QUEST_SLOT_2      BLOCK_QUEST_SLOT_1 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_SLOT_3      BLOCK_QUEST_SLOT_2 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_SLOT_4      BLOCK_QUEST_SLOT_3 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_SLOT_5      BLOCK_QUEST_SLOT_4 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_SLOT_6      BLOCK_QUEST_SLOT_5 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_SLOT_7      BLOCK_QUEST_SLOT_6 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_SLOT_8      BLOCK_QUEST_SLOT_7 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_LOCATION    BLOCK_QUEST_SLOT_8 + QUEST_WIDTH * QUEST_HEIGHT
#define BLOCK_QUEST_DESC        BLOCK_QUEST_LOCATION + QUEST_NAME_WIDTH * QUEST_HEIGHT
#define BLOCK_REWARD_1          BLOCK_QUEST_DESC + QUEST_DESC_WIDTH * QUEST_DESC_HEIGHT
#define BLOCK_REWARD_2          BLOCK_REWARD_1 + QUEST_REWARD_WIDTH * QUEST_REWARD_HEIGHT
#define BLOCK_REWARD_3          BLOCK_REWARD_2 + QUEST_REWARD_WIDTH * (QUEST_REWARD_HEIGHT + 1)
#define BLOCK_MONEY_HEADER      BLOCK_REWARD_3 + QUEST_REWARD_WIDTH * (QUEST_REWARD_HEIGHT + 1)
#define BLOCK_LEVEL_HEADER      BLOCK_MONEY_HEADER + QUEST_MONEY_HEADER_WIDTH * QUEST_MONEY_HEADER_HEIGHT
#define BLOCK_ERR_MSG           BLOCK_LEVEL_HEADER + QUEST_LEVEL_WIDTH * QUEST_LEVEL_HEIGHT

#define QUEST_TOP_POSITION 3
#define QUEST_TOP_OFFSET 2
#define QUEST_LEFT_POSITION 1

// Windows config
static const struct WindowTemplate sWindowTemplate_Fishing[] = 
{
    [W_HEADER] = {
        .bg = BG_0,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = HEADER_WIDTH,
        .height = HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_HEADER,
    },
    [W_HEADER_CONTROL] = {
        .bg = BG_0,
        .tilemapLeft = 20,
        .tilemapTop = 0,
        .width = QUEST_HEADER_CONTRO_WIDTH,
        .height = HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_HEADER_CONTROL,
    },
    [W_QUEST_SLOT_1] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_1,
    },
    [W_QUEST_SLOT_2] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_2,
    },
    [W_QUEST_SLOT_3] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET * 2,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_3,
    },
    [W_QUEST_SLOT_4] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET * 3,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_4,
    },
    [W_QUEST_SLOT_5] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET * 4,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_5,
    },
    [W_QUEST_SLOT_6] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET * 5,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_6,
    },
    [W_QUEST_SLOT_7] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET * 6,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_7,
    },
    [W_QUEST_SLOT_8] = {
        .bg = BG_0,
        .tilemapLeft = QUEST_LEFT_POSITION,
        .tilemapTop = QUEST_TOP_POSITION + QUEST_TOP_OFFSET * 7,
        .width = QUEST_WIDTH,
        .height = QUEST_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_SLOT_8,
    },
    [W_QUEST_LOCATION] = {
        .bg = BG_0,
        .tilemapLeft = 19,
        .tilemapTop = 2,
        .width = QUEST_NAME_WIDTH,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_LOCATION,
    },
    [W_QUEST_DESC] = {
        .bg = BG_0,
        .tilemapLeft = 15,
        .tilemapTop = 8,
        .width = QUEST_DESC_WIDTH,
        .height = QUEST_DESC_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_QUEST_DESC,
    },
    [W_REWARD_MONEY] = {
        .bg = BG_0,
        .tilemapLeft = 23,
        .tilemapTop = 15,
        .width = QUEST_REWARD_WIDTH,
        .height = QUEST_REWARD_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_REWARD_1,
    },
    [W_REWARD_1] = {
        .bg = BG_0,
        .tilemapLeft = 18,
        .tilemapTop = 17,
        .width = QUEST_REWARD_WIDTH,
        .height = QUEST_REWARD_HEIGHT + 1,
        .paletteNum = 15,
        .baseBlock = BLOCK_REWARD_2,
    },
    [W_REWARD_2] = {
        .bg = BG_0,
        .tilemapLeft = 25,
        .tilemapTop = 17,
        .width = QUEST_REWARD_WIDTH,
        .height = QUEST_REWARD_HEIGHT + 1,
        .paletteNum = 15,
        .baseBlock = BLOCK_REWARD_3,
    },
    [W_MONEY_HEADER] = {
        .bg = BG_0,
        .tilemapLeft = 16,
        .tilemapTop = 15,
        .width = QUEST_MONEY_HEADER_WIDTH,
        .height = QUEST_MONEY_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_MONEY_HEADER,
    },
    [W_LEVEL_HEADER] = {
        .bg = BG_0,
        .tilemapLeft = 19,
        .tilemapTop = 4,
        .width = QUEST_LEVEL_WIDTH,
        .height = QUEST_LEVEL_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_LEVEL_HEADER,
    },
    [W_ERR_MSG] = {
        .bg = BG_0,
        .tilemapLeft = 1,
        .tilemapTop = 9,
        .width = 16,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = BLOCK_ERR_MSG,
    }
};

// Bg templates
static const struct BgTemplate sBgTemplates_QuestMenu[4] = 
{
    [BG_0] = {
        .bg = BG_0,
        .charBaseIndex = 0,
        .mapBaseIndex = 12,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
    },
    [BG_1] = {
        .bg = BG_1,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
    },
    [BG_2] = {
        .bg = BG_2,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
    },
    [BG_3] = {
        .bg = BG_3,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 3,
    },
};

// Constants
#define SPRITE_NONE             0xFF
#define QUEST_NONE              0xFFFF
#define MONEY_DIGITS            7
#define DISPLAYED_QUEST_COUNT   8
// level difference between quest and player
#define LEVEL_DIFFICULTY_CAP    5
#define D_TRIVIAL_LEVEL_DIFF    -8
#define D_EASY_LEVEL_DIFF       -1
#define D_NORMAL_LEVEL_DIFF     3
#define D_HARD_LEVEL_DIFF       7
// used to edit slots tilemap
#define MAP_BYTES_PER_LINE      0x40
#define SLOTS_BASE_LINES        3
#define MAP_LINES_PER_SLOT      2
// max number of quest for each slider
#define SLIDER_64_MAX           15
#define SLIDER_32_MAX           35
#define SLIDER_16_MAX           60
// sprite positions
#define ARROW_POS_X                 68
#define ARROW_UP_POS_Y              28
#define ARROW_DOWN_POS_Y            164
#define DIFFICULTY_ICON_BASE_X      196
#define DIFFICULTY_ICON_PADDING_X   9
#define DIFFICULTY_ICON_Y           41
#define REWARD_ICON_POS_X           140
#define REWARD_ICON_OFFSET_X        56
#define REWARD_ICON_POS_Y           152
#define NPC_POS_X                   134
#define NPC_POS_Y                   26
// text positions
#define QUEST_OFFSET_X          0
#define QUEST_OFFSET_Y          0
#define MENU_HEADER_OFFSET_X    6
#define MONEY_HEADER_OFFSET_X   6
#define LEVEL_HEADER_OFFSET_X   3
#define LOCATION_OFFSET_X       3
#define DESCRIPTION_OFFSET_X    4
#define ERR_MSG_OFFSET_Y        4
#define REWARD_MONEY_OFFSET_X   4
#define REWARD_MONEY_OFFSET_Y   2
#define REWARD_OFFSET_Y         6
// font
#define SLOT_0          0
#define SLOT_1          1
#define FONT_SIZE_SMALL 0
#define FONT_SIZE_BIG   2

enum {FONT_LIGHT, FONT_DARK};
static const u8 sFontColor_Std[] = {0, 1, 2};
static const u8 sFontColor_Dark[] = {0, 2, 3};

static const u8 *const sFontColor[] = {
    [FONT_LIGHT]    = sFontColor_Std,
    [FONT_DARK]     = sFontColor_Dark
};

// stores ptr to menu data structure
static EWRAM_DATA struct QuestMenuData *sData = {0};

/************************************** * 
 * ****** INIT QUEST MENU ************* *
 * ************************************ */
// To be used from a callnative cmd
void Init_ArcaneQuestMenu(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    PlaySE(SE_RG_HELP_OPEN);
    SetMainCallback2(CB2_WaitUntilFadeInitQuestMenu);
}

// Wait until fade is done to init quest menu
static void CB2_WaitUntilFadeInitQuestMenu()
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_InitQuestMenu);
}

// Init quest menu
static void CB2_InitQuestMenu()
{
    switch(gMain.state)
    {
        // Reset everything and inits data structure
        case 0:
            InitHeap(gHeap, HEAP_SIZE);
            SetVBlankCallback(NULL);
            SetHBlankCallback(NULL);
            ClearScheduledBgCopiesToVram();
            ResetTasks();
            FreeAllSpritePalettes();
            ResetSpriteData();
            ResetPaletteFade();
            DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
            DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
            DmaFill16(3, 0, (void *)(PLTT + 2), PLTT_SIZE - 2);
            Quest_InitData();

            gMain.state++;
            break;

        // Init backgrounds and windows
        case 1:
            Quest_InitBgs();
            Quest_LoadBgGfx();
            Quest_InitWindows();

            gMain.state++;
            break;

        // Init sprites, print text and resume to input handler with a fade
        default:
        case 2:
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, RGB_BLACK);
            
            // Print texts
            Quest_PrintConstantMenuTexts();
            Quest_BuildAndPrintLoadedQuestNames();
            Quest_PrintSelectedQuestInfo();

            // Create sprites
            Quest_LoadSpriteGfx();
            Quest_CreateSprites();
            Quest_CreateSlider();

            // Highlight cursor
            Quest_HighlightSelectedCursor();         

            // Set callbacks and resume to input handler
            SetMainCallback2(CB2_QuestMenu);
            SetVBlankCallback(VBlankCB_QuestMenu);
            CreateTask(Task_HandleQuestMenuInput, 0); 
            break;
    }
}

// CB2 for quest menu
static void CB2_QuestMenu()
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

// VBlank for quest menu
static void VBlankCB_QuestMenu()
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

// Allocates menu data structure and it's values
static void Quest_InitData()
{
    int i, j;

    sData = AllocZeroed(sizeof(struct QuestMenuData));
    sData->npcSpriteId = SPRITE_NONE;
    sData->rewardId[0] = SPRITE_NONE;
    sData->rewardId[1] = SPRITE_NONE;
    sData->selectedSlot = 0;
    sData->loadedQuestsOffset = 0;
    sData->activeQuestCount = Quest_GetActiveQuestCount();
    sData->activeQuests = AllocZeroed(sizeof(u16) * sData->activeQuestCount);

    // fill active quests array
    for (i = 0, j = 0; i < QUEST_COUNT; i++)
        if (Quest_IsQuestActive(i))
        {
            sData->activeQuests[j] = i;
            j++;
        }
}

// Inits background register values
static void Quest_InitBgs()
{
    ResetAllBgsCoordinates();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates_QuestMenu, ARRAY_COUNT(sBgTemplates_QuestMenu));
    // TODO: SetGpuReg for transparency
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

// Loads background gfx
static void Quest_LoadBgGfx()
{
    LZ77UnCompVram(gBgTilemap_QuestMenuSelector_1, (u16 *)BG_SCREEN_ADDR(sBgTemplates_QuestMenu[BG_1].mapBaseIndex));
    LZ77UnCompVram(gBgTiles_QuestMenuGUI, (void *)BG_CHAR_ADDR(sBgTemplates_QuestMenu[BG_2].charBaseIndex));
    LZ77UnCompVram(gBgTilemap_QuestMenuGUI, (u16 *)BG_SCREEN_ADDR(sBgTemplates_QuestMenu[BG_2].mapBaseIndex));
    LZ77UnCompVram(gBgTiles_QuestMenuBackground, (void *)BG_CHAR_ADDR(sBgTemplates_QuestMenu[BG_3].charBaseIndex));
    LZ77UnCompVram(gBgTilemap_QuestMenuBackground, (u16 *)BG_SCREEN_ADDR(sBgTemplates_QuestMenu[BG_3].mapBaseIndex));
    LoadPalette(gBgPalette_QuestMenuGUI, 0, 0x20);
    LoadPalette(gBgPalette_QuestMenuBackground, 0x10, 0x20);
}

// Init text window configurations
static void Quest_InitWindows()
{
    InitWindows(sWindowTemplate_Fishing);
    DeactivateAllTextPrinters();
    LoadPalette(GetOverworldTextboxPalettePtr(), 0xF0, 0x20);
}

/************************************** * 
 * ********* MENU CONTROL ************* *
 * ************************************ */
// Handles input for the menu
static void Task_HandleQuestMenuInput(u8 taskId)
{   
    if (JOY_REPEAT(DPAD_DOWN)) {
        if (Quest_HandleDownInput())
            PlaySE(SE_RG_BAG_CURSOR);
    }
    if (JOY_REPEAT(DPAD_UP)) {
        if (Quest_HandleUpInput())
            PlaySE(SE_RG_BAG_CURSOR);
    }
    if (JOY_NEW(B_BUTTON)) {
        PlaySE(SE_RG_HELP_CLOSE);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_WaitFadeReturnToField;
    }
}

// Waits until fade is done to return to field
static void Task_WaitFadeReturnToField(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
    }
}

// Handles dpad down input
static bool8 Quest_HandleDownInput()
{
    u16 nextQuest = sData->selectedSlot + sData->loadedQuestsOffset + 1;
    
    // no more quests to show
    if (nextQuest >= sData->activeQuestCount)
        return FALSE;
    
    if (sData->selectedSlot < 7)
        sData->selectedSlot++;
    else
        sData->loadedQuestsOffset++;

    Quest_UpdateMenuAfterScroll();
    
    return TRUE;
}

// Handles dpad up input
static bool8 Quest_HandleUpInput()
{
    u16 currentQuest = sData->selectedSlot + sData->loadedQuestsOffset;

    if (currentQuest != 0)
    {
        if (sData->selectedSlot > 0)
            sData->selectedSlot--;
        else
            sData->loadedQuestsOffset--;

        Quest_UpdateMenuAfterScroll();
        
        return TRUE;
    }
    
    return FALSE;
}
// Updates the menu after a scroll
static void Quest_UpdateMenuAfterScroll()
{
    Quest_BuildAndPrintLoadedQuestNames();
    Quest_HighlightSelectedCursor();
    Quest_PrintSelectedQuestInfo();
}

// Counts the number of active quests
static u16 Quest_GetActiveQuestCount()
{
    int i;
    u16 count = 0;

    for (i = 0; i < QUEST_COUNT; i++)
    {
        if (Quest_IsQuestActive(i))
            count++;
    }

    return count;
}

// Builds the list of the currently loaded quests (8 max) from the active quests depending on cursor position and offset
static void Quest_BuildLoadedQuests() 
{
    int i;
    u16 offset = sData->loadedQuestsOffset;

    for (i = offset; i < offset + DISPLAYED_QUEST_COUNT; i++) {
        if (i >= sData->activeQuestCount)
            sData->loadedQuests[i - offset] = QUEST_NONE;
        else
            sData->loadedQuests[i - offset] = sData->activeQuests[i];
    }
}

// Highlights the selected cursor
static void Quest_HighlightSelectedCursor()
{
    const u32 *src;

    // selects a tilemap
    switch(sData->selectedSlot)
    {
        default:
            src = gBgTilemap_QuestMenuSelector_None;
            break;
        case 0:
            src = gBgTilemap_QuestMenuSelector_1;
            break;
        case 1:
            src = gBgTilemap_QuestMenuSelector_2;
            break;
        case 2:
            src = gBgTilemap_QuestMenuSelector_3;
            break;
        case 3:
            src = gBgTilemap_QuestMenuSelector_4;
            break;
        case 4:
            src = gBgTilemap_QuestMenuSelector_5;
            break;
        case 5:
            src = gBgTilemap_QuestMenuSelector_6;
            break;
        case 6:
            src = gBgTilemap_QuestMenuSelector_7;
            break;
        case 7:
            src = gBgTilemap_QuestMenuSelector_8;
            break;
    }
    // loads the tilemap
    LZ77UnCompVram(src, (u16 *)BG_SCREEN_ADDR(sBgTemplates_QuestMenu[BG_1].mapBaseIndex));
    
    // if loaded quests are less than 8, remove the empty slots fromt the tilemap
    if (sData->activeQuestCount < DISPLAYED_QUEST_COUNT)
        Quest_RemoveEmptyQuestGUISlots();
}

// Removes the empty quest gui slots from tilemap
static void Quest_RemoveEmptyQuestGUISlots()
{
    void *dest;
    u32 memOffset;

    u16 loadedQuests = sData->activeQuestCount;
    memOffset = MAP_BYTES_PER_LINE * (SLOTS_BASE_LINES + MAP_LINES_PER_SLOT * loadedQuests);
    dest = (u16 *)(BG_SCREEN_ADDR(sBgTemplates_QuestMenu[BG_1].mapBaseIndex) + memOffset);
    DmaFill32(3, 0, (void *)dest, 0x800 - memOffset);
}

// Calculates difficulty level for a quest
// It uses the average level of the player's party and the advised level of the quest
// For the average, it doesnt take into account the pokemon that are more than LEVEL_DIFFICULTY_CAP levels below the highest level pokemon
static u8 Quest_CalcDifficultyLevel(u16 questId)
{
    u32 acumulator;
    u8 advisedLevel, maxLevel, tmp, count, avg;
    s16 diff;
    int i;

    advisedLevel = sQuestList[questId].advisedLevel;
    
    // get the highest level pokemon
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        tmp = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        if (tmp > maxLevel)
            maxLevel = tmp;
    }

    // get the average level of the pokemon that are not more than LEVEL_DIFFICULTY_CAP levels below the highest level pokemon
    acumulator = 0;
    count = 0;
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        tmp = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        if (tmp + LEVEL_DIFFICULTY_CAP >= maxLevel)
        {
            acumulator += tmp;
            count++;
        }
    }
    avg = acumulator / count;    

    // calcs diff between the average level and the advised level
    diff = advisedLevel - avg;

    if (diff < D_TRIVIAL_LEVEL_DIFF)
        return DIFFICULTY_TRIVIAL;
    else if (diff < D_EASY_LEVEL_DIFF)
        return DIFFICULTY_EASY;
    else if (diff < D_NORMAL_LEVEL_DIFF)
        return DIFFICULTY_MEDIUM;
    else if (diff < D_HARD_LEVEL_DIFF)
        return DIFFICULTY_HARD;
    else
        return DIFFICULTY_DANGEROUS;
}

/************************************** * 
 * ************* SPRITES ************** *
 * ************************************ */

#define sArrowType  data[1]
#define sSliderSize data[2]

// Loads the quest menu sprite graphic resources
static void Quest_LoadSpriteGfx()
{
    LoadCompressedSpriteSheet(&sSpriteSheet_Arrow);
    LoadCompressedSpriteSheet(&sSpriteSheet_DifficultyIcon);
    LoadSpritePalette(&sSpritePalette_Arrow);
    LoadSpritePalette(&sSpritePalette_DifficultyIcon);
}

// Creates arrow and difficulty sprites
static void Quest_CreateSprites()
{
    int i;

    sData->arrowId[ARROW_UP] = CreateSprite(&sSpriteTemplate_Arrow, -8, -8, 0);
    gSprites[sData->arrowId[ARROW_UP]].invisible = TRUE;
    gSprites[sData->arrowId[ARROW_UP]].sArrowType = ARROW_UP;
    StartSpriteAnim(&gSprites[sData->arrowId[ARROW_UP]], ARROW_UP);
    sData->arrowId[ARROW_DOWN] = CreateSprite(&sSpriteTemplate_Arrow, -8, -8, 0);
    gSprites[sData->arrowId[ARROW_DOWN]].invisible = TRUE;
    gSprites[sData->arrowId[ARROW_DOWN]].sArrowType = ARROW_DOWN;
    StartSpriteAnim(&gSprites[sData->arrowId[ARROW_DOWN]], ARROW_DOWN);

    for (i = 0; i < DIFFICULTY_ICON_COUNT; i++)
        sData->difficultyId[i] = CreateSprite(&sSpriteTemplate_DifficultyIcon, DIFFICULTY_ICON_BASE_X + i * DIFFICULTY_ICON_PADDING_X, DIFFICULTY_ICON_Y, 0);
}

// Creates the slider sprite with a size depending on the amount of quests
static void Quest_CreateSlider()
{
    u8 id, size;
    u16 count;
    struct SpriteTemplate template;

    LoadSpritePalette(&sSpritePalette_Slider);
    count = sData->activeQuestCount;

    // loads template info
    template.tileTag = TAG_SLIDER;
    template.paletteTag = TAG_SLIDER;
    template.anims = gDummySpriteAnimTable;
    template.affineAnims = gDummySpriteAffineAnimTable;
    template.images = NULL;

    if (count > DISPLAYED_QUEST_COUNT)  
    {
        // Decide slider size
        template.callback = SpriteCB_Slider;
        if (count < SLIDER_64_MAX) {
            template.oam = &sOamData_Slider_64;
            LoadCompressedSpriteSheet(&sSpriteSheet_Slider_64);
            size = SIZE_64;
        }
        else if (count < SLIDER_32_MAX) {
            template.oam = &sOamData_Slider_32;
            LoadCompressedSpriteSheet(&sSpriteSheet_Slider_32);
            size = SIZE_32;
        }
        else if (count < SLIDER_16_MAX) {
            template.oam = &sOamData_Slider_16;
            LoadCompressedSpriteSheet(&sSpriteSheet_Slider_16);
            size = SIZE_16;
        }
        else {
            template.oam = &sOamData_Slider_8;
            LoadCompressedSpriteSheet(&sSpriteSheet_Slider_8);
            size = SIZE_8;
        }

        id = CreateSprite(&template, 0, 0, 0);
        gSprites[id].sSliderSize = size;
    }
    else // no slider needed (full list fits on screen)
    {
        template.callback = SpriteCallbackDummy;
        template.oam = &sOamData_Slider_64;
        LoadCompressedSpriteSheet(&sSpriteSheet_Slider_64);
        CreateSprite(&template, SLIDER_X, SLIDER_64_Y, 0);
        CreateSprite(&template, SLIDER_X, SLIDER_64_Y + 64, 0);
        CreateSprite(&template, SLIDER_X, SLIDER_64_END_Y, 0);
    }   
}

// Controls slider position
static void SpriteCB_Slider(struct Sprite *sprite)
{
    u8 pixelDiff, offsetSize, sliderOffset, sliderSize;

    sliderSize = sprite->sSliderSize;
    pixelDiff = sSliders[sliderSize].end_y - sSliders[sliderSize].start_y;
    offsetSize = sData->activeQuestCount - DISPLAYED_QUEST_COUNT;
    sliderOffset = (pixelDiff * sData->loadedQuestsOffset) / offsetSize;

    // sets positions
    sprite->x2 = SLIDER_X;
    sprite->y2 = sSliders[sliderSize].start_y + sliderOffset;
}

// Controls arrow visibility
static void SpriteCB_Arrow(struct Sprite *sprite)
{
    if (sprite->sArrowType == ARROW_UP)
    {
        sprite->x2 = ARROW_POS_X;
        sprite->y2 = ARROW_UP_POS_Y;

        if (sData->loadedQuestsOffset == 0)
            sprite->invisible = TRUE;
        else
            sprite->invisible = FALSE;
    }
    else
    {
        sprite->x2 = ARROW_POS_X;
        sprite->y2 = ARROW_DOWN_POS_Y;

        if (sData->loadedQuestsOffset == sData->activeQuestCount - DISPLAYED_QUEST_COUNT
            || sData->activeQuestCount < DISPLAYED_QUEST_COUNT) 
            sprite->invisible = TRUE;
        else
            sprite->invisible = FALSE;
    }
}

// Creates the reward icon sprite for the given quest slot
static void Quest_UpdateRewardIconSprite(u16 itemId, u8 amount, u8 id, s16 x, s16 y)
{
    struct SpriteSheet sheet;
    struct CompressedSpritePalette pal;
    struct SpriteTemplate *template;
    u8 spriteId;

    if (id > 1)
        id = 1;
    
    spriteId = sData->rewardId[id];

    // if a previous sprite exists, destroy it and free its resources
    if (spriteId != SPRITE_NONE)
    {
        FreeSpriteTilesByTag(id + TAG_ITEM_ICON);
        FreeSpritePaletteByTag(id + TAG_ITEM_ICON);
        DestroySprite(&gSprites[spriteId]);
        sData->rewardId[id] = SPRITE_NONE;
    }

    // if no item, dont create the sprite
    if (itemId == ITEM_NONE || amount == 0)
        return;

    // allocate buffer, if no space, dont create the sprite
    if (!AllocItemIconTemporaryBuffers())
        return; //MAX_SPRITES
    LZDecompressWram((void *)gItemIconTable[itemId][0], gItemIconDecompressionBuffer);
    CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);

    // fill sheet, palette and template info
    sheet.data = gItemIcon4x4Buffer;
    sheet.size = 0x200;
    sheet.tag = id + TAG_ITEM_ICON;
    LoadSpriteSheet(&sheet);

    pal.data = (void *)gItemIconTable[itemId][1];
    pal.tag = id + TAG_ITEM_ICON;
    LoadCompressedSpritePalette(&pal);

    // copy info to template and add tags
    template = Alloc(sizeof(*template));
    CpuCopy16(&gItemIconSpriteTemplate, template, sizeof(*template));
    template->tileTag = id + TAG_ITEM_ICON;
    template->paletteTag = id + TAG_ITEM_ICON;

    // create sprite
    spriteId = CreateSprite(template, x, y, 0);
    sData->rewardId[id] = spriteId;

    // free resources
    Free(template);
    FreeItemIconTemporaryBuffers();
}

// Updates the npc sprite for the given quest
static void Quest_UpdateNpcSprite(u16 questId)
{
    struct OamData oam;
    u16 map;
    u8 localId, gfxId;

    // if a previous sprite exists, destroy it and free its resources
    if (sData->npcSpriteId != SPRITE_NONE)
        DestroySpriteAndFreeResources(&gSprites[sData->npcSpriteId]);

    localId = sQuestList[questId].npc.localId - 1; // localId starts at 1 in porymap, while in code it starts at 0 (thus the -1)
    map = sQuestList[questId].npc.map;
    gfxId = GetObjectEventGfxIdByLocalIdAndMap(localId, map & 0xFF, map >> 8 & 0xFF);
    sData->npcSpriteId = CreateObjectGraphicsSprite(gfxId, SpriteCallbackDummy, NPC_POS_X, NPC_POS_Y, 0);
    
    // adjust sprite position depending on size
    oam = gSprites[sData->npcSpriteId].oam;
    if (oam.shape == SPRITE_SHAPE(16x16) &&
        oam.size == SPRITE_SIZE(16x16))
        gSprites[sData->npcSpriteId].y2 += 8;
}

// Update the difficulty sprites for the given quest
// Icons changes color depending on quest and party level
// number of icons changes depending on quest difficulty
static void Quest_UpdateQuestDifficulty(u16 questId)
{
    u8 level, difficulty;
    int i;

    difficulty = sQuestList[questId].difficulty; 
    level = Quest_CalcDifficultyLevel(questId);

    for (i = 0; i < difficulty; i++)
    {
        gSprites[sData->difficultyId[i]].invisible = FALSE;
        StartSpriteAnim(&gSprites[sData->difficultyId[i]], level);
    }

    for (i = difficulty; i < DIFFICULTY_ICON_COUNT; i++)
    {
        gSprites[sData->difficultyId[i]].invisible = TRUE;
    }
}

// Set all difficulty icons to invisible
static void Quest_ClearDifficultyIcons()
{
    int i;

    for (i = 0; i < DIFFICULTY_ICON_COUNT; i++)
        gSprites[sData->difficultyId[i]].invisible = TRUE; 
}

#undef sArrowType
#undef sSliderSize

/************************************** * 
 * ******** TEXT PRINTERS ************* *
 * ************************************ */
// Prints constants texts that are not changed within the menu
static void Quest_PrintConstantMenuTexts()
{
    Quest_PrintText(W_HEADER, MENU_HEADER_OFFSET_X, 0, gText_QuestMenuHeader, FONT_SIZE_BIG);
    Quest_PrintText(W_HEADER_CONTROL, 0, 0, gText_QuestControlHeader, FONT_SIZE_SMALL);
    Quest_PrintDarkText(W_MONEY_HEADER, MONEY_HEADER_OFFSET_X, 2, gText_MoneyHeader, FONT_SIZE_SMALL);
    // if no quests, dont print level header
    if (sData->activeQuestCount > 0)
        Quest_PrintText(W_LEVEL_HEADER, LEVEL_HEADER_OFFSET_X, 0, gText_Level, FONT_SIZE_SMALL);
}

// Prints the quest names
static void Quest_BuildAndPrintLoadedQuestNames() 
{
    int i;

    Quest_BuildLoadedQuests();
    for (i = 0; i < DISPLAYED_QUEST_COUNT; i++)
        if (sData->loadedQuests[i] != QUEST_NONE)
            Quest_PrintText(W_QUEST_SLOT_1 + i, QUEST_OFFSET_X, QUEST_OFFSET_Y, sQuestList[sData->loadedQuests[i]].name, FONT_SIZE_SMALL);
}

// Prints the selected quest info
static void Quest_PrintSelectedQuestInfo()
{
    u16 pos = sData->selectedSlot + sData->loadedQuestsOffset;
    u16 questId = sData->activeQuests[pos];

    if (sData->activeQuestCount > 0)
    {
        // Print quest location, description, reward money, items, npc sprite and difficulty
        Quest_PrintText(W_QUEST_LOCATION, LOCATION_OFFSET_X, 0, sQuestList[questId].location, FONT_SIZE_SMALL);
        Quest_PrintDarkText(W_QUEST_DESC, DESCRIPTION_OFFSET_X, 0, sQuestList[questId].description, FONT_SIZE_SMALL);
        Quest_PrintRewardMoney(questId);
        Quest_UpdateRewardItem(questId, 0);
        Quest_UpdateRewardItem(questId, 1);
        Quest_UpdateNpcSprite(questId);
        Quest_UpdateQuestDifficulty(questId);
    }
    else
    {
        // if no active quest, print err message and clear all icons
        Quest_ClearDifficultyIcons();
        Quest_PrintText(W_ERR_MSG, 0, ERR_MSG_OFFSET_Y, gText_EmptyQuestLog, FONT_SIZE_BIG);
    }
}

// Prints a string into the given windowId with clear font
static void Quest_PrintText(u8 windowId, u8 x, u8 y, const u8* text, u8 font)
{
    StringExpandPlaceholders(gStringVar4, text);
    Quest_PrintBufferedText(windowId, x, y, font, FONT_LIGHT);
}

// Prints a string into the given windowId with dark font
static void Quest_PrintDarkText(u8 windowId, u8 x, u8 y, const u8* text, u8 font)
{
    StringExpandPlaceholders(gStringVar4, text);
    Quest_PrintBufferedText(windowId, x, y, font, FONT_DARK);
}

// TODO: Move sFont_ to array
// Prints pre-buffered text into the given windowId with the given font color
static void Quest_PrintBufferedText(u8 windowId, u8 x, u8 y, u8 font, u8 fontColor)
{
    FillWindowPixelBuffer(windowId, 0);
    PutWindowTilemap(windowId);
    AddTextPrinterParameterized3(windowId, font, x, y, sFontColor[fontColor], -1, gStringVar4);
    CopyWindowToVram(windowId, 3);
}

// converts a number to a decimal string with the given number of digits with right alignment
static u8* Quest_BuildNumberText(u32 number, u8 digits)
{
    return ConvertIntToDecimalStringN(gStringVar4, number, STR_CONV_MODE_RIGHT_ALIGN, digits);
}

// Print given quest reward money (if any)
static bool8 Quest_PrintRewardMoney(u16 questId)
{
    int i;
    u8 *ptr;
    u32 money = sQuestList[questId].rewardMoney;

    if (money > 0)
    {
        ptr = Quest_BuildNumberText(money, MONEY_DIGITS);
        *ptr-- = CHAR_SPACE;
        *ptr++ = CHAR_CURRENCY;
        *ptr++ = EOS;
        Quest_PrintBufferedText(W_REWARD_MONEY, REWARD_MONEY_OFFSET_X, REWARD_MONEY_OFFSET_Y, 0, FONT_DARK);
        return TRUE;
    }
    else    // no money reward
    {
        Quest_PrintDarkText(W_REWARD_MONEY, REWARD_MONEY_OFFSET_X, REWARD_MONEY_OFFSET_Y, gText_NoMoney, FONT_SIZE_SMALL);
        return FALSE;
    }
}

// Print given quest reward item for the given slot (if any)
static bool8 Quest_PrintRewardItemCount(u16 itemId, u16 amount, u8 slot)
{
    u8 *ptr;

    if (itemId != ITEM_NONE && amount > 0)
    {
        // Adds "x" sign before number depending on amount of digits
        if (amount < 10)
        {
            ptr = Quest_BuildNumberText(itemId, 3);
            ptr--; ptr--; *ptr-- = CHAR_MULT_SIGN;
        }
        else
        {
            ptr = Quest_BuildNumberText(itemId, 4);
            ptr--; ptr--; ptr--; *ptr-- = CHAR_MULT_SIGN;
        }       
        
        Quest_PrintBufferedText(W_REWARD_1 + slot, 0, REWARD_OFFSET_Y, FONT_SIZE_BIG, FONT_LIGHT);
        return TRUE;
    }
    else
    {
        Quest_PrintText(W_REWARD_1 + slot, 0, REWARD_OFFSET_Y, gText_EmptyString2, FONT_SIZE_BIG);
        return FALSE;
    }
}

// Update reward item count and sprite
static void Quest_UpdateRewardItem(u16 questId, u8 slot)
{
    u16 itemId;
    u8 amount;

    if (slot > 1)
        slot = 1;
    itemId = sQuestList[questId].rewardItems[slot].id;
    amount = sQuestList[questId].rewardItems[slot].amount;

    Quest_UpdateRewardIconSprite(itemId, amount, slot, REWARD_ICON_POS_X + REWARD_ICON_OFFSET_X * slot, REWARD_ICON_POS_Y);
    Quest_PrintRewardItemCount(itemId, amount, slot);
}
