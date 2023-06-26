#include "global.h"
#include "arcane_ev_screen.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_message_box.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "script.h"
#include "script_menu.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "data/text/ev_screen.h"

struct MonStats {
    u8 hp;
    u8 atk;
    u8 def;
    u8 spatk;
    u8 spdef;
    u8 speed;
};

struct EvScreenMon {
    struct MonStats stats;
    struct MonStats baseEv;
    struct MonStats selectedEv;
    u16 baseTmpEv;
    u16 currTmpEv;
};

struct EvScreenData {
    struct EvScreenMon monData[PARTY_SIZE];
};

static const union AnimCmd sAnimCmd_SelectionArrowRight[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_SelectionArrowLeft[] = {
    ANIMCMD_FRAME(0, 0, .hFlip = TRUE),
    ANIMCMD_END,
};

enum {ARROW_RIGHT, ARROW_LEFT};
static const union AnimCmd *const sAnimTable_SelectionArrow[] = 
{
    [ARROW_RIGHT] = sAnimCmd_SelectionArrowRight,
    [ARROW_LEFT] = sAnimCmd_SelectionArrowLeft,
};

static const union AnimCmd sAnimCmd_MonSelectorLeft[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_MonSelectorRight[] = {
    ANIMCMD_FRAME(0, 0, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_MonSelector[] = 
{
    [ARROW_RIGHT] = sAnimCmd_MonSelectorRight,
    [ARROW_LEFT] = sAnimCmd_MonSelectorLeft,
};

enum {BG_0, BG_1, BG_2, BG_3};
static const struct BgTemplate sBgTemplates_SummaryScreen[4] = 
{
    [BG_0] = {
        .bg = BG_0,
        .charBaseIndex = 0,
        .mapBaseIndex = 27,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    [BG_1] = {
        .bg = BG_1,
        .charBaseIndex = 1,
        .mapBaseIndex = 18,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    [BG_2] = {
        .bg = BG_2,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    [BG_3] = {
        .bg = BG_3,
        .charBaseIndex = 3,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

#define STAT_HEADER_WIDTH 8
#define STAT_HEADER_HEIGHT 2
#define STAT_NUMBER_WIDTH 8
#define TOTAL_WIDTH 5
#define EV_PADDING_LEFT 10
#define AVAILABLE_WIDTH 10
#define MON_NAME_WIDTH 7
#define GUIDE_WIDTH 20
#define BLOCK_HP 60
#define BLOCK_ATK   BLOCK_HP + STAT_HEADER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_DEF   BLOCK_ATK + STAT_HEADER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPATK BLOCK_DEF + STAT_HEADER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPDEF BLOCK_SPATK + STAT_HEADER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPD   BLOCK_SPDEF + STAT_HEADER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_TOTAL BLOCK_SPD + STAT_HEADER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_EV    BLOCK_TOTAL + TOTAL_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_HP_STAT       BLOCK_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_ATK_STAT      BLOCK_HP_STAT + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_DEF_STAT      BLOCK_ATK_STAT + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPATK_STAT    BLOCK_DEF_STAT + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPDEF_STAT    BLOCK_SPATK_STAT + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPD_STAT      BLOCK_SPDEF_STAT + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_HP_EV         BLOCK_SPD_STAT + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_ATK_EV        BLOCK_HP_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_DEF_EV        BLOCK_ATK_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPATK_EV      BLOCK_DEF_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPDEF_EV      BLOCK_SPATK_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_SPD_EV        BLOCK_SPDEF_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_TMP_EV        BLOCK_SPD_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_INC_HP_EV     BLOCK_TMP_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_INC_ATK_EV    BLOCK_INC_HP_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_INC_DEF_EV    BLOCK_INC_ATK_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_INC_SPATK_EV  BLOCK_INC_DEF_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_INC_SPDEF_EV  BLOCK_INC_SPATK_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_INC_SPD_EV    BLOCK_INC_SPDEF_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_AVAILABLE     BLOCK_INC_SPD_EV + STAT_NUMBER_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_MON_NAME      BLOCK_AVAILABLE + AVAILABLE_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_GUIDE_A       BLOCK_MON_NAME + MON_NAME_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_GUIDE_B       BLOCK_GUIDE_A +  GUIDE_WIDTH * STAT_HEADER_HEIGHT
#define BLOCK_MESSAGE       BLOCK_GUIDE_B +  GUIDE_WIDTH * STAT_HEADER_HEIGHT

enum {W_MESSAGE, W_YES_NO, W_HP, W_ATK, W_DEF, W_SPATK, W_SPDEF, W_SPD, W_TOTAL, W_EV,
    W_HP_STAT, W_ATK_STAT, W_DEF_STAT, W_SPATK_STAT, W_SPDEF_STAT, W_SPD_STAT,
    W_HP_EV, W_ATK_EV, W_DEF_EV, W_SPATK_EV, W_SPDEF_EV, W_SPD_EV,
    W_TMP_EV,
    W_INC_HP_EV, W_INC_ATK_EV, W_INC_DEF_EV, W_INC_SPATK_EV, W_INC_SPDEF_EV, W_INC_SPD_EV,
    W_AVAILABLE,
    W_GUIDE_A, W_NEW_EV};

#define POS_BASE_X_STAT 9
#define POS_BASE_Y_STAT 7

static const struct WindowTemplate sWindowTemplate_EvScreen[] =
{
    [W_HP] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT,
        .tilemapTop = POS_BASE_Y_STAT,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_HP,
    },
    [W_ATK] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT,
        .tilemapTop = POS_BASE_Y_STAT + 2,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_ATK,
    },
    [W_DEF] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT,
        .tilemapTop = POS_BASE_Y_STAT + 4,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_DEF,
    },
    [W_SPATK] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT,
        .tilemapTop = POS_BASE_Y_STAT + 6,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPATK,
    },
    [W_SPDEF] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT,
        .tilemapTop = POS_BASE_Y_STAT + 8,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPDEF,
    },
    [W_SPD] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT,
        .tilemapTop = POS_BASE_Y_STAT + 10,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPD,
    },
    [W_TOTAL] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 6,
        .tilemapTop = POS_BASE_Y_STAT - 2,
        .width = TOTAL_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_TOTAL,
    },
    [W_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT - 2,
        .width = TOTAL_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_EV,
    },
    [W_HP_STAT] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_HP_STAT,
    },
    [W_ATK_STAT] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT + 2,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_ATK_STAT,
    },
    [W_DEF_STAT] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT + 4,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_DEF_STAT,
    },
    [W_SPATK_STAT] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT + 6,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPATK_STAT,
    },
    [W_SPDEF_STAT] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT + 8,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPDEF_STAT,
    },
    [W_SPD_STAT] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT + 10,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPD_STAT,
    },
    [W_HP_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_HP_EV,
    },
    [W_ATK_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT + 2,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_ATK_EV,
    },
    [W_DEF_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT + 4,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_DEF_EV,
    },
    [W_SPATK_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT + 6,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPATK_EV,
    },
    [W_SPDEF_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT + 8,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPDEF_EV,
    },
    [W_SPD_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + EV_PADDING_LEFT,
        .tilemapTop = POS_BASE_Y_STAT + 10,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_SPD_EV,
    },
    [W_TMP_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT - 5,
        .width = STAT_NUMBER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_TMP_EV,
    },
    [W_INC_HP_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_INC_HP_EV,
    },
    [W_INC_ATK_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT + 2,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_INC_ATK_EV,
    },
    [W_INC_DEF_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT + 4,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_INC_DEF_EV,
    },
    [W_INC_SPATK_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT + 6,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_INC_SPATK_EV,
    },
    [W_INC_SPDEF_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT + 8,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_INC_SPDEF_EV,
    },
    [W_INC_SPD_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT + 10,
        .width = STAT_HEADER_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_INC_SPD_EV,
    },
    [W_AVAILABLE] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 7,
        .tilemapTop = POS_BASE_Y_STAT - 5,
        .width = AVAILABLE_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_AVAILABLE,
    },
    [W_GUIDE_A] = {
        .bg = BG_1,
        .tilemapLeft = 9,
        .tilemapTop = 0,
        .width = GUIDE_WIDTH,
        .height = STAT_HEADER_HEIGHT,
        .paletteNum = 15,
        .baseBlock = BLOCK_GUIDE_A,
    },
    [W_MESSAGE] = {
        .bg = BG_0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 27,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [W_YES_NO] = {
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 9,
        .width = 5,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x125
    },
    [W_NEW_EV] = {
        .bg = BG_1,
        .tilemapLeft = POS_BASE_X_STAT + 16,
        .tilemapTop = POS_BASE_Y_STAT - 2,
        .width = 6,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = BLOCK_GUIDE_B,
    },
};


#define TAG_SELECTION_ARROW 11400
#define TAG_MON_SELECTOR 11401

#define PAL_TAG_SELECTION_ARROW 11400
#define PAL_TAG_MON_SELECTOR 11401

static const struct CompressedSpriteSheet sSpriteSheet_SelectionArrow =
{
    .data = gSpriteGfx_EvSelectionArrow,
    .size = 0x40,
    .tag = TAG_SELECTION_ARROW,
};

static const struct CompressedSpriteSheet sSpriteSheet_MonSelector = 
{
    .data = gSpriteGfx_EvMonSelector,
    .size = 0x200,
    .tag = TAG_MON_SELECTOR,
};

static const struct SpritePalette sSpritePalette_SelectionArrow = 
{
    .data = gSpritePal_EvSelectionArrow,
    .tag = PAL_TAG_SELECTION_ARROW,
};

static const struct SpritePalette sSpritePalette_MonSelector = 
{
    .data = gSpritePal_EvMonSelector,
    .tag = PAL_TAG_MON_SELECTOR,
};

static const struct OamData sOamData_SelectionArrow =
{
    .objMode = ST_OAM_OBJ_NORMAL,
    .size = SPRITE_SIZE(8x16),
    .shape = SPRITE_SHAPE(8x16),
    .priority = 1,
};

static void SpriteCB_StatSelectionSprite(struct Sprite* sprite);
static void SpriteCB_MonSelector(struct Sprite* sprite);
static void SpriteCb_MonIcon(struct Sprite* sprite);

static const struct SpriteTemplate sSpriteTemplate_SelectionArrow =
{
    .tileTag = TAG_SELECTION_ARROW,
    .paletteTag = PAL_TAG_SELECTION_ARROW,
    .oam = &sOamData_SelectionArrow,
    .anims = sAnimTable_SelectionArrow,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_StatSelectionSprite,
};

static const struct SpriteTemplate sSpriteTemplate_MonSelector =
{
    .tileTag = TAG_MON_SELECTOR,
    .paletteTag = PAL_TAG_MON_SELECTOR,
    .oam = &gObjectEventBaseOam_32x32,
    .anims = sAnimTable_MonSelector,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_MonSelector,
};

#define MON_ICON_START_X    24
#define MON_ICON_PADDING_X  16
#define MON_ICON_START_Y    16
#define MON_ICON_PADDING_Y  24

#define STAT_ARROW_BASE_Y 33
#define STAT_ARROW_PADDING_Y 16
#define STAT_ARROW_BASE_X 189
#define STAT_ARROW_PADDING_X 41

#define MON_SELECTOR_BASE_X 24
#define MON_SELECTOR_PADDING_X 16
#define MON_SELECTOR_BASE_Y 20
#define MON_SELECTOR_PADDING_Y 24

enum {D_NONE, DIR_RIGHT, DIR_LEFT};
enum {STATE_CHOOSE_MON, STATE_CHOOSE_STAT};
enum {COLOR_REGULAR, COLOR_BLUE, COLOR_GREEN, COLOR_RED};
enum Stat {EV_STAT_HP, EV_STAT_ATK, EV_STAT_DEF, EV_STAT_SPATK, EV_STAT_SPDEF, EV_STAT_SPEED};

static const u8 sFontColor_Std[3] = {0, 2, 3};
static const u8 sFontColor_White[3] = {0, 1, 2};
static const u8 sFontColor_Blue[3] = {0, 8, 9};
static const u8 sFontColor_Green[3] = {0, 6, 7};
static const u8 sFontColor_Red[3] = {0, 4, 5};

static EWRAM_DATA u8 sPartyMonIds[6] = {0}; 
static EWRAM_DATA u8 sSelectedPartyMonId = 0;
static EWRAM_DATA u8 sSelectedMonSpriteId = 0;
static EWRAM_DATA u8 sSelectedStat = 0;
static EWRAM_DATA u8 sSelectionArrowIds[2] = {0};
static EWRAM_DATA struct EvScreenData *data = NULL;
static EWRAM_DATA u8 sMenuStoredState = 0;

static void CB2_InitEvScreenWaitFade();
static void CB2_InitEvScreen();
static void EvScreen_InitData();
static void EvScreen_InitBgs();
static void EvScreen_InitPartyMons();
static void EvScreen_LoadBgData();
static void EvScreen_InitWindows();
static void CB2_EvScreen();
static void VBlankCB_EvScreen();

static void EvScreen_CreateSelectedMonSprite();
static void EvScreen_StartMonAnim();
static void EvScreen_HighlightSelectedMon();
static void EvScreen_CreateMonSelectorSprite();
static void EvScreen_CreateStatSelectionSprites();
static void EvScreen_MonCursorDown();
static void EvScreen_MonCursorUp();
static void EvScreenDestroySelectionSprites();

static void EvScreen_PrintConstantTexts();
static void EvScreen_PrintSelectedMonStats();
static void EvScreen_PrintSelectedMonEVs();
static void EvScreen_PrintMonSelectionGuide();
static void EvScreen_PrintBackToMonSelectionGuide();
static void EvScreen_UpdateStatText(u8 stat);
static void EvScreen_PrintIntoWindow(u8 windowId, const u8 *str, u8 font, s8 x, s8 y);
static u8* EvScreen_BuildNumberText(u16 number);
static void EvScreen_PrintNumberWindow(u8 windowId, u16 number, u8 font, s8 x, s8 y);
static void EvScreen_PrintNumberWindowColor(u8 windowId, u16 number, u8 font, s8 x, s8 y, u8 color);
static void EvScreen_PrintMonStatByColor(u8 stat);
static void EvScreen_PrintMonEvByColor(u8 stat);

static void Task_HandleInputChooseMon(u8 taskId);
static void SubTask_HandleStartButton(u8 taskId);
static void Task_HandleInputSelectStat(u8 taskId);
static void Task_HandleYesNoBox(u8 taskId);
static void Task_HandleYesNoBoxAnswer(u8 taskId);
static void Task_WaitFadeReturnToField(u8 taskId);
static void EvScreen_ConsolidateEvChangesAndRecalcStats();

static u16 EvScreen_CalcMonStat(u8 stat, u8 monSlot);
static bool8 EvScreen_TryAddEv(u8 stat, u8 monSlot);
static bool8 EvScreen_TryRemoveEv(u8 stat, u8 monSlot);
static void EvScreen_LoadSelectedMonData();
static u16 EvScreen_CalcTotalEvs(u8 monSlot);
static u16 EvScreen_GetMonStatSelectedEvs(u8 stat, u8 monSlot);
static u16 EvScreen_CalcTotalEvForStat(u8 stat, u8 monSlot);

// called from callnative
void Init_EvScreen()
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    SetMainCallback2(CB2_InitEvScreenWaitFade);
    gMain.state = 0;
}

// executes fade and inits EvScreen
static void CB2_InitEvScreenWaitFade()
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_InitEvScreen);
}

static void CB2_InitEvScreen()
{
    switch(gMain.state)
    {   
        // reset everything
        case 0:
            data = Alloc(sizeof(struct EvScreenData));
            EvScreen_InitData();
            SetVBlankHBlankCallbacksToNull();
            ClearScheduledBgCopiesToVram();
            FreeAllSpritePalettes();
            ResetPaletteFade();
            ResetSpriteData();
            ResetTasks();

            // clear vram
            DmaClearLarge16(3, (void*)VRAM, VRAM_SIZE, 0x1000);
            //DmaClearLarge16(3, (void*)OAM, OAM_SIZE, 0x1000);
            DmaClear16(3, (void*)&gPlttBufferFaded, PLTT_SIZE);
            DmaClear16(3, (void*)&gPlttBufferUnfaded, PLTT_SIZE);

            gMain.state++;
            break;

        // init bg, load data, init windows
        case 1:
            EvScreen_InitBgs();
            EvScreen_LoadBgData();
            EvScreen_InitWindows();

            gMain.state++;
            break;

        default:
        case 2:
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, RGB_BLACK);
            EvScreen_PrintConstantTexts();
            EvScreen_PrintSelectedMonStats();
            EvScreen_PrintSelectedMonEVs();
            EvScreen_CreateSelectedMonSprite();
            EvScreen_PrintMonSelectionGuide();
            EvScreen_StartMonAnim();
            EvScreen_HighlightSelectedMon();
            EvScreen_CreateMonSelectorSprite();

            SetMainCallback2(CB2_EvScreen);
            SetVBlankCallback(VBlankCB_EvScreen);

            // create control task
            CreateTask(Task_HandleInputChooseMon, 0);
            break;
    }
}

static void EvScreen_InitData()
{
    int i;
    struct Pokemon *mon;

    sSelectedPartyMonId = 0;
    sSelectedStat = 0;
    sMenuStoredState = STATE_CHOOSE_MON;

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        mon = &gPlayerParty[i];
        // stats
        data->monData[i].stats.hp = GetMonData(mon, MON_DATA_MAX_HP);
        data->monData[i].stats.atk = GetMonData(mon, MON_DATA_ATK);
        data->monData[i].stats.def = GetMonData(mon, MON_DATA_DEF);
        data->monData[i].stats.spatk = GetMonData(mon, MON_DATA_SPATK);
        data->monData[i].stats.spdef = GetMonData(mon, MON_DATA_SPDEF);
        data->monData[i].stats.speed = GetMonData(mon, MON_DATA_SPEED);

        // evs
        data->monData[i].baseEv.hp = GetMonData(mon, MON_DATA_HP_EV);
        data->monData[i].baseEv.atk = GetMonData(mon, MON_DATA_ATK_EV);
        data->monData[i].baseEv.def = GetMonData(mon, MON_DATA_DEF_EV);
        data->monData[i].baseEv.spatk = GetMonData(mon, MON_DATA_SPATK_EV);
        data->monData[i].baseEv.spdef = GetMonData(mon, MON_DATA_SPDEF_EV);
        data->monData[i].baseEv.speed = GetMonData(mon, MON_DATA_SPEED_EV);

        // selected evs
        data->monData[i].selectedEv.hp = 0;
        data->monData[i].selectedEv.atk = 0;
        data->monData[i].selectedEv.def = 0;
        data->monData[i].selectedEv.spatk = 0;
        data->monData[i].selectedEv.spdef = 0;
        data->monData[i].selectedEv.speed = 0;

        // count of tmp evs
        data->monData[i].baseTmpEv = GetMonData(mon, MON_DATA_TMP_EV);
        data->monData[i].currTmpEv = data->monData[i].baseTmpEv;
    }
}

static void EvScreen_InitBgs()
{
    ResetAllBgsCoordinatesAndBgCntRegs();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates_SummaryScreen, ARRAY_COUNT(sBgTemplates_SummaryScreen));
    ShowBg(0);
    ShowBg(1);
    HideBg(2);
    ShowBg(3);
    SetGpuReg(REG_OFFSET_DISPCNT, 0x1B40);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 9));
    EvScreen_InitPartyMons();
}

static void EvScreen_InitPartyMons()
{
    int i;
    LoadMonIconPalettes();

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        sPartyMonIds[i] = CreateMonIcon(GetMonData(&gPlayerParty[i], MON_DATA_SPECIES), 
                    SpriteCb_MonIcon, 
                    i % 2 == 0 ? MON_ICON_START_X : MON_ICON_START_X + MON_ICON_PADDING_X, 
                    MON_ICON_START_Y + MON_ICON_PADDING_Y * i, 
                    1,
                    GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY));
    }

}

static void EvScreen_LoadBgData()
{
    LZ77UnCompVram(gBgTiles_EvScreen, (void*) BG_CHAR_ADDR(sBgTemplates_SummaryScreen[BG_3].charBaseIndex));
    LZ77UnCompVram(gBgTilemap_EvScreen, (u16*) BG_SCREEN_ADDR(sBgTemplates_SummaryScreen[BG_3].mapBaseIndex));
    LoadPalette(gBgPalette_EvScreen, 0, 0x20);
}

static void EvScreen_InitWindows()
{
    InitWindows(sWindowTemplate_EvScreen);
    DeactivateAllTextPrinters();
    LoadPalette(GetOverworldTextboxPalettePtr(), 0xF0, 0x20);
}

static void CB2_EvScreen()
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
    ScriptContext_RunScript();
}

static void VBlankCB_EvScreen()
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void EvScreen_CreateSelectedMonSprite()
{
    sSelectedMonSpriteId = CreateMonIcon(GetMonData(&gPlayerParty[sSelectedPartyMonId], MON_DATA_SPECIES), 
                    SpriteCb_MonIcon, 
                    96, 
                    34, 
                    1,
                    GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY));
}

static void EvScreen_StartMonAnim()
{
    int i;

    for (i = 0; i < gPlayerPartyCount; i++)
        gSprites[sPartyMonIds[i]].callback = SpriteCb_MonIcon;

}

static void EvScreen_HighlightSelectedMon()
{
    int i;

    for (i = 0; i < gPlayerPartyCount; i++)
        if (i == sSelectedPartyMonId)
        {
            gSprites[sPartyMonIds[i]].oam.objMode = ST_OAM_OBJ_NORMAL;
            gSprites[sPartyMonIds[i]].callback = SpriteCb_MonIcon;
        }
        else
        {
            gSprites[sPartyMonIds[i]].oam.objMode = ST_OAM_OBJ_BLEND;
            gSprites[sPartyMonIds[i]].callback = SpriteCallbackDummy;
        }
}

static void EvScreen_CreateMonSelectorSprite() 
{
    LoadCompressedSpriteSheet(&sSpriteSheet_MonSelector);
    LoadSpritePalette(&sSpritePalette_MonSelector);
    CreateSprite(&sSpriteTemplate_MonSelector, 0, 0, 0);
}

static void EvScreen_CreateStatSelectionSprites()
{
    LoadCompressedSpriteSheet(&sSpriteSheet_SelectionArrow);
    LoadSpritePalette(&sSpritePalette_SelectionArrow);
    sSelectionArrowIds[0] = CreateSprite(&sSpriteTemplate_SelectionArrow, STAT_ARROW_BASE_X, STAT_ARROW_BASE_Y, 0);
    gSprites[sSelectionArrowIds[0]].data[0] = ARROW_LEFT;
    sSelectionArrowIds[1] = CreateSprite(&sSpriteTemplate_SelectionArrow, STAT_ARROW_BASE_X + STAT_ARROW_PADDING_X, STAT_ARROW_BASE_Y, 0);
    gSprites[sSelectionArrowIds[1]].data[0] = ARROW_RIGHT;
    StartSpriteAnim(&gSprites[sSelectionArrowIds[0]], ARROW_LEFT);
}

static void EvScreen_MonCursorDown() 
{
    sSelectedPartyMonId++;
    EvScreen_HighlightSelectedMon();
    EvScreen_LoadSelectedMonData();
}

static void EvScreen_MonCursorUp()
{
    sSelectedPartyMonId--;
    EvScreen_HighlightSelectedMon();
    EvScreen_LoadSelectedMonData();
}

static void EvScreenDestroySelectionSprites()
{
    DestroySprite(&gSprites[sSelectionArrowIds[0]]);
    DestroySpriteAndFreeResources(&gSprites[sSelectionArrowIds[1]]);
}

static void EvScreen_PrintConstantTexts()
{
    EvScreen_PrintIntoWindow(W_HP, sText_HP, 1, 2, 2);
    EvScreen_PrintIntoWindow(W_ATK, sText_Atk, 1, 2, 2);
    EvScreen_PrintIntoWindow(W_DEF, sText_Def, 1, 2, 2);
    EvScreen_PrintIntoWindow(W_SPATK, sText_SpAtk, 1, 2, 2);
    EvScreen_PrintIntoWindow(W_SPDEF, sText_SpDef, 1, 2, 2);
    EvScreen_PrintIntoWindow(W_SPD, sText_Spd, 1, 2, 2);
    EvScreen_PrintIntoWindow(W_TOTAL, sText_Total, 1, 1, 2);
    EvScreen_PrintIntoWindow(W_EV, sText_EV, 1, 6, 2);
    EvScreen_PrintIntoWindow(W_AVAILABLE, sText_AvailableEV, 1, 4, 3);
    EvScreen_PrintIntoWindow(W_NEW_EV, sText_NewEV, 1, 0, 2);
}

static void EvScreen_PrintSelectedMonStats()
{
    EvScreen_PrintMonStatByColor(EV_STAT_HP);
    EvScreen_PrintMonStatByColor(EV_STAT_ATK);
    EvScreen_PrintMonStatByColor(EV_STAT_DEF);
    EvScreen_PrintMonStatByColor(EV_STAT_SPATK);
    EvScreen_PrintMonStatByColor(EV_STAT_SPDEF);
    EvScreen_PrintMonStatByColor(EV_STAT_SPEED);
}

static void EvScreen_PrintSelectedMonEVs()
{
    u16 tmpEv; 

    u16 hp, atk, def, spatk, spdef, spd, total;

    struct Pokemon *mon = &gPlayerParty[sSelectedPartyMonId];

    tmpEv = data->monData[sSelectedPartyMonId].currTmpEv;
    if (tmpEv > 0)
        EvScreen_PrintNumberWindowColor(W_TMP_EV, tmpEv, 1, 0, 3, COLOR_REGULAR);
    else
        EvScreen_PrintNumberWindowColor(W_TMP_EV, tmpEv, 1, 0, 3, COLOR_RED);

    EvScreen_PrintMonEvByColor(EV_STAT_HP);
    EvScreen_PrintMonEvByColor(EV_STAT_ATK);
    EvScreen_PrintMonEvByColor(EV_STAT_DEF);
    EvScreen_PrintMonEvByColor(EV_STAT_SPATK);
    EvScreen_PrintMonEvByColor(EV_STAT_SPDEF);
    EvScreen_PrintMonEvByColor(EV_STAT_SPEED);

    hp = data->monData[sSelectedPartyMonId].selectedEv.hp;
    atk = data->monData[sSelectedPartyMonId].selectedEv.atk;
    def = data->monData[sSelectedPartyMonId].selectedEv.def;
    spatk = data->monData[sSelectedPartyMonId].selectedEv.spatk;
    spdef = data->monData[sSelectedPartyMonId].selectedEv.spdef;
    spd = data->monData[sSelectedPartyMonId].selectedEv.speed;

    EvScreen_PrintNumberWindow(W_INC_HP_EV, hp, 1, 2, 2);
    EvScreen_PrintNumberWindow(W_INC_ATK_EV, atk, 1, 2, 2);
    EvScreen_PrintNumberWindow(W_INC_DEF_EV, def, 1, 2, 2);
    EvScreen_PrintNumberWindow(W_INC_SPATK_EV, spatk, 1, 2, 2);
    EvScreen_PrintNumberWindow(W_INC_SPDEF_EV, spdef, 1, 2, 2);
    EvScreen_PrintNumberWindow(W_INC_SPD_EV, spd, 1, 2, 2);
}

static void EvScreen_PrintMonSelectionGuide()
{
    EvScreen_PrintIntoWindow(W_GUIDE_A, sText_GuideSelectMon, 0, 2, 0);
}

static void EvScreen_PrintBackToMonSelectionGuide()
{
    EvScreen_PrintIntoWindow(W_GUIDE_A, sText_BackToMon, 0, 2, 0);
}

static void EvScreen_UpdateStatText(u8 stat)
{
    u16 tmp;

    switch(stat)
    {
        case EV_STAT_HP:
            EvScreen_PrintMonStatByColor(EV_STAT_HP);
            EvScreen_PrintMonEvByColor(EV_STAT_HP);
            tmp = data->monData[sSelectedPartyMonId].selectedEv.hp;
            EvScreen_PrintNumberWindow(W_INC_HP_EV, tmp, 1, 2, 2);
            break;

        case EV_STAT_ATK:
            EvScreen_PrintMonStatByColor(EV_STAT_ATK);
            EvScreen_PrintMonEvByColor(EV_STAT_ATK);
            tmp = data->monData[sSelectedPartyMonId].selectedEv.atk;
            EvScreen_PrintNumberWindow(W_INC_ATK_EV, tmp, 1, 2, 2);
            break;
        
        case EV_STAT_DEF:
            EvScreen_PrintMonStatByColor(EV_STAT_DEF);
            EvScreen_PrintMonEvByColor(EV_STAT_DEF);
            tmp = data->monData[sSelectedPartyMonId].selectedEv.def;
            EvScreen_PrintNumberWindow(W_INC_DEF_EV, tmp, 1, 2, 2);
            break;

        case EV_STAT_SPATK:
            EvScreen_PrintMonStatByColor(EV_STAT_SPATK);
            EvScreen_PrintMonEvByColor(EV_STAT_SPATK);
            tmp = data->monData[sSelectedPartyMonId].selectedEv.spatk;
            EvScreen_PrintNumberWindow(W_INC_SPATK_EV, tmp, 1, 2, 2);
            break;
        
        case EV_STAT_SPDEF:
            EvScreen_PrintMonStatByColor(EV_STAT_SPDEF);
            EvScreen_PrintMonEvByColor(EV_STAT_SPDEF);
            tmp = data->monData[sSelectedPartyMonId].selectedEv.spdef;
            EvScreen_PrintNumberWindow(W_INC_SPDEF_EV, tmp, 1, 2, 2);
            break;

        case EV_STAT_SPEED:
            EvScreen_PrintMonStatByColor(EV_STAT_SPEED);
            EvScreen_PrintMonEvByColor(EV_STAT_SPEED);
            tmp = data->monData[sSelectedPartyMonId].selectedEv.speed;
            EvScreen_PrintNumberWindow(W_INC_SPD_EV, tmp, 1, 2, 2);
            break;
    }

    tmp = data->monData[sSelectedPartyMonId].currTmpEv;
    if (tmp > 0)
        EvScreen_PrintNumberWindowColor(W_TMP_EV, tmp, 1, 0, 3, COLOR_REGULAR);
    else
        EvScreen_PrintNumberWindowColor(W_TMP_EV, tmp, 1, 0, 3, COLOR_RED);
}

static void EvScreen_PrintIntoWindow(u8 windowId, const u8 *str, u8 font, s8 x, s8 y)
{
    StringExpandPlaceholders(gStringVar4, str);
    FillWindowPixelBuffer(windowId, 0);
    PutWindowTilemap(windowId);
    AddTextPrinterParameterized3(windowId, font, x, y, sFontColor_White, -1, gStringVar4);
    CopyWindowToVram(windowId, 3);
}

static u8* EvScreen_BuildNumberText(u16 number)
{
    return ConvertIntToDecimalStringN(gStringVar4, number, STR_CONV_MODE_RIGHT_ALIGN, 3);
}

static void EvScreen_PrintNumberWindow(u8 windowId, u16 number, u8 font, s8 x, s8 y)
{
    EvScreen_BuildNumberText(number);
    FillWindowPixelBuffer(windowId, 0);
    PutWindowTilemap(windowId);
    AddTextPrinterParameterized3(windowId, font, x, y, sFontColor_Std, -1, gStringVar4);
    CopyWindowToVram(windowId, 3);
}

static void EvScreen_PrintNumberWindowColor(u8 windowId, u16 number, u8 font, s8 x, s8 y, u8 color)
{
    const u8* colorTable;
    EvScreen_BuildNumberText(number);
    FillWindowPixelBuffer(windowId, 0);
    PutWindowTilemap(windowId);
    
    switch(color)
    {
        case COLOR_REGULAR:
            colorTable = sFontColor_Std;
            break;
        case COLOR_BLUE:
            colorTable = sFontColor_Blue;
            break;
        case COLOR_GREEN:
            colorTable = sFontColor_Green;
            break;
        case COLOR_RED:
            colorTable = sFontColor_Red;
            break;
    }
    AddTextPrinterParameterized3(windowId, font, x, y, colorTable, -1, gStringVar4);
    CopyWindowToVram(windowId, 3);
}

static void EvScreen_PrintMonStatByColor(u8 stat)
{
    u16 base, actual;
    u8 windowId;

    switch(stat)
    {
        case EV_STAT_HP:
            actual = EvScreen_CalcMonStat(EV_STAT_HP, sSelectedPartyMonId);
            base = data->monData[sSelectedPartyMonId].stats.hp;
            windowId = W_HP_STAT;
            break;
        case EV_STAT_ATK:
            actual = EvScreen_CalcMonStat(EV_STAT_ATK, sSelectedPartyMonId);
            base = data->monData[sSelectedPartyMonId].stats.atk;
            windowId = W_ATK_STAT;
            break;
        case EV_STAT_DEF:
            actual = EvScreen_CalcMonStat(EV_STAT_DEF, sSelectedPartyMonId);
            base = data->monData[sSelectedPartyMonId].stats.def;
            windowId = W_DEF_STAT;
            break;
        case EV_STAT_SPATK:
            actual = EvScreen_CalcMonStat(EV_STAT_SPATK, sSelectedPartyMonId);
            base = data->monData[sSelectedPartyMonId].stats.spatk;
            windowId = W_SPATK_STAT;
            break;
        case EV_STAT_SPDEF:
            actual = EvScreen_CalcMonStat(EV_STAT_SPDEF, sSelectedPartyMonId);
            base = data->monData[sSelectedPartyMonId].stats.spdef;
            windowId = W_SPDEF_STAT;
            break;
        case EV_STAT_SPEED:
            actual = EvScreen_CalcMonStat(EV_STAT_SPEED, sSelectedPartyMonId);
            base = data->monData[sSelectedPartyMonId].stats.speed;
            windowId = W_SPD_STAT;
            break;
        default:
            return;
    }

    if (base != actual)
        EvScreen_PrintNumberWindowColor(windowId, actual, 1, 0, 2, COLOR_BLUE);
    else 
        EvScreen_PrintNumberWindowColor(windowId, actual, 1, 0, 2, COLOR_REGULAR);
}

static void EvScreen_PrintMonEvByColor(u8 stat)
{
    u16 base, actual;
    u8 windowId;

    switch(stat)
    {
        case EV_STAT_HP:
            base = data->monData[sSelectedPartyMonId].baseEv.hp;
            actual = base + data->monData[sSelectedPartyMonId].selectedEv.hp;
            windowId = W_HP_EV;
            break;
        case EV_STAT_ATK:
            base = data->monData[sSelectedPartyMonId].baseEv.atk;
            actual = base + data->monData[sSelectedPartyMonId].selectedEv.atk;
            windowId = W_ATK_EV;
            break;
        case EV_STAT_DEF:
            base = data->monData[sSelectedPartyMonId].baseEv.def;
            actual = base + data->monData[sSelectedPartyMonId].selectedEv.def;
            windowId = W_DEF_EV;
            break;
        case EV_STAT_SPATK:
            base = data->monData[sSelectedPartyMonId].baseEv.spatk;
            actual = base + data->monData[sSelectedPartyMonId].selectedEv.spatk;
            windowId = W_SPATK_EV;
            break;
        case EV_STAT_SPDEF:
            base = data->monData[sSelectedPartyMonId].baseEv.spdef;
            actual = base + data->monData[sSelectedPartyMonId].selectedEv.spdef;
            windowId = W_SPDEF_EV;
            break;
        case EV_STAT_SPEED:
            base = data->monData[sSelectedPartyMonId].baseEv.speed;
            actual = base + data->monData[sSelectedPartyMonId].selectedEv.speed;
            windowId = W_SPD_EV;
            break;
        default:
            return;
    }

    if (actual >= 252)
        EvScreen_PrintNumberWindowColor(windowId, actual, 1, 5, 2, COLOR_GREEN);
    else if (base != actual)
        EvScreen_PrintNumberWindowColor(windowId, actual, 1, 5, 2, COLOR_BLUE);
    else 
        EvScreen_PrintNumberWindowColor(windowId, actual, 1, 5, 2, COLOR_REGULAR);
}

#define tState data[0]
static void Task_HandleInputChooseMon(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (JOY_NEW(A_BUTTON)) {
        sSelectedStat = 0;
        sMenuStoredState = STATE_CHOOSE_STAT;
        EvScreen_CreateStatSelectionSprites();
        EvScreen_PrintBackToMonSelectionGuide();
        PlaySE(SE_RG_BAG_POCKET);
        task->func = Task_HandleInputSelectStat;
    }
    else if (JOY_REPEAT(DPAD_DOWN) && sSelectedPartyMonId < gPlayerPartyCount - 1) {
        EvScreen_MonCursorDown();
        PlaySE(SE_RG_BAG_CURSOR);
    }
    else if (JOY_REPEAT(DPAD_UP) && sSelectedPartyMonId > 0) {
        EvScreen_MonCursorUp();
        PlaySE(SE_RG_BAG_CURSOR);
    }
    
    SubTask_HandleStartButton(taskId);
}

static void SubTask_HandleStartButton(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (JOY_NEW(START_BUTTON))  {
        StringExpandPlaceholders(gStringVar4, sText_HaveYouFinished);
        AddTextPrinterForMessage(TRUE);
        PlaySE(SE_PC_LOGIN);
        task->func = Task_HandleYesNoBox;
        task->tState = 0;
    }
}
#undef tState

#define tRepeatedTimesRight         data[0]
#define tRepeatedTimesLeft          data[1]
#define tTimeSinceLastRepeatRight   data[2]
#define tTimeSinceLastRepeatLeft    data[3]
static void Task_HandleInputSelectStat(u8 taskId)
{
    int i;
    bool8 b;
    struct Task *task = &gTasks[taskId];

    if (JOY_REPEAT(DPAD_RIGHT)) {
        if (task->tRepeatedTimesRight < 10) {
            task->tRepeatedTimesRight++;
            if (EvScreen_TryAddEv(sSelectedStat, sSelectedPartyMonId)) {
                PlaySE(SE_SELECT);
                EvScreen_UpdateStatText(sSelectedStat);
            }
            else
                PlaySE(SE_FAILURE);
        }
        else {
            for (i = 0; i < 10; i++)
                b = EvScreen_TryAddEv(sSelectedStat, sSelectedPartyMonId);
            EvScreen_UpdateStatText(sSelectedStat);
            if (b)
                PlaySE(SE_SELECT);
            else
                PlaySE(SE_FAILURE);
        }
        task->tTimeSinceLastRepeatRight = 0;
    }
    else if (JOY_REPEAT(DPAD_LEFT)) {
        if (task->tRepeatedTimesLeft < 10) {
            task->tRepeatedTimesLeft++;
            if (EvScreen_TryRemoveEv(sSelectedStat, sSelectedPartyMonId)) {
                PlaySE(SE_SELECT);
                EvScreen_UpdateStatText(sSelectedStat);
            }
            else
                PlaySE(SE_FAILURE);
        }
        else {
            for (i = 0; i < 10; i++)
                b = EvScreen_TryRemoveEv(sSelectedStat, sSelectedPartyMonId);
            EvScreen_UpdateStatText(sSelectedStat);
            if (b)
                PlaySE(SE_SELECT);
            else
                PlaySE(SE_FAILURE);
        }
    }
    else if (JOY_NEW(KEY_OR_INTR)){
        task->tRepeatedTimesRight = 0;
    }
    
    if (JOY_REPEAT(DPAD_DOWN) && sSelectedStat < 5) {
        sSelectedStat++;
        PlaySE(SE_RG_BAG_CURSOR);
    }
    else if (JOY_REPEAT(DPAD_UP) && sSelectedStat > 0) {
        sSelectedStat--;
        PlaySE(SE_RG_BAG_CURSOR);
    }
    
    if (JOY_NEW(B_BUTTON)) {
        sMenuStoredState = STATE_CHOOSE_MON;
        EvScreenDestroySelectionSprites();
        EvScreen_PrintMonSelectionGuide();
        task->func = Task_HandleInputChooseMon;
        PlaySE(SE_RG_HELP_CLOSE);
    }

    SubTask_HandleStartButton(taskId);


    if (task->tTimeSinceLastRepeatRight > 10)
        task->tRepeatedTimesRight = 0;
    if (task->tTimeSinceLastRepeatLeft > 10)
        task->tRepeatedTimesLeft = 0;

    task->tTimeSinceLastRepeatLeft++;
    task->tTimeSinceLastRepeatRight++;
}
#undef tRepeatedTimesRight
#undef tRepeatedTimesLeft
#undef tTimeSinceLastRepeatRight
#undef tTimeSinceLastRepeatLeft

#define tState data[0]
static void Task_HandleYesNoBox(u8 taskId) 
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
        case 0:
           LoadMessageBoxAndBorderGfxAtWindow(W_MESSAGE);
           task->tState++;
           break;
        case 1:
           DrawDialogueFrame(W_MESSAGE, TRUE);
           task->tState++;
           break;
        case 2:
            if (RunTextPrintersAndIsPrinter0Active() != TRUE)
            {
                DisplayYesNoMenuDefaultYesAtWindow(W_YES_NO);
                CreateTask(Task_HandleYesNoInputFromMenu,0);
                task->func = Task_HandleYesNoBoxAnswer;
            }
    }
}

static void Task_HandleYesNoBoxAnswer(u8 taskId)
{

    if (!FuncIsActiveTask(Task_HandleYesNoInputFromMenu)) {
        ClearDialogWindowAndFrame(W_MESSAGE, TRUE);
        if (gSpecialVar_Result == 0) {  // NO
            switch(sMenuStoredState) {
                case STATE_CHOOSE_MON:
                    gTasks[taskId].func = Task_HandleInputChooseMon;
                    break;
                case STATE_CHOOSE_STAT:
                    gTasks[taskId].func = Task_HandleInputSelectStat;
                    break;
            }
        }
        else { // YES -> end
            EvScreen_ConsolidateEvChangesAndRecalcStats();
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_WaitFadeReturnToField;
        }
    }
}
#undef tState

static void Task_WaitFadeReturnToField(u8 taskId) {
    if (!gPaletteFade.active) {
        SetMainCallback2(CB2_ReturnToField);
    }
}

static void EvScreen_ConsolidateEvChangesAndRecalcStats()
{
    int i;
    u16 tmp;

    for (i = 0; i < gPlayerPartyCount; i++) {
        tmp = data->monData[i].baseEv.hp + data->monData[i].selectedEv.hp; 
        SetMonData(&gPlayerParty[i], MON_DATA_HP_EV, &tmp);
        tmp = data->monData[i].baseEv.atk + data->monData[i].selectedEv.atk;
        SetMonData(&gPlayerParty[i], MON_DATA_ATK_EV, &tmp);
        tmp = data->monData[i].baseEv.def + data->monData[i].selectedEv.def;
        SetMonData(&gPlayerParty[i], MON_DATA_DEF_EV, &tmp);
        tmp = data->monData[i].baseEv.spatk + data->monData[i].selectedEv.spatk;
        SetMonData(&gPlayerParty[i], MON_DATA_SPATK_EV, &tmp);
        tmp = data->monData[i].baseEv.spdef + data->monData[i].selectedEv.spdef;
        SetMonData(&gPlayerParty[i], MON_DATA_SPDEF_EV, &tmp);
        tmp = data->monData[i].baseEv.speed + data->monData[i].selectedEv.speed;
        SetMonData(&gPlayerParty[i], MON_DATA_SPEED_EV, &tmp);
        tmp = data->monData[i].currTmpEv;
        SetMonData(&gPlayerParty[i], MON_DATA_TMP_EV, &tmp);
        CalculateMonStats(&gPlayerParty[i]);
    }
    HealPlayerParty();
}


static u16 EvScreen_CalcMonStat(u8 stat, u8 monSlot)
{
    u16 tmp, base, iv, ev, level;
    u8 nature;
    struct Pokemon *mon = &gPlayerParty[monSlot];

    switch(stat)
    {
        case EV_STAT_HP:
            base = gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].baseHP;
            level = GetMonData(mon, MON_DATA_LEVEL);
            iv = GetMonData(mon, MON_DATA_HP_IV);
            ev = data->monData[monSlot].baseEv.hp + data->monData[monSlot].selectedEv.hp;
            tmp = (((2 * base + iv + ev / 4) * level) / 100) + level + 10;
            return tmp;
        case EV_STAT_ATK:
            base = gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].baseAttack;
            level = GetMonData(mon, MON_DATA_LEVEL);
            iv = GetMonData(mon, MON_DATA_ATK_IV);
            ev = data->monData[monSlot].baseEv.atk + data->monData[monSlot].selectedEv.atk;
            tmp = (((2 * base + iv + ev / 4) * level) / 100) + 5;
            nature = GetNature(mon);
            tmp = ModifyStatByNature(nature, tmp, STAT_ATK); 
            return tmp;
        case EV_STAT_DEF:
            base = gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].baseDefense;
            level = GetMonData(mon, MON_DATA_LEVEL);
            iv = GetMonData(mon, MON_DATA_DEF_IV);
            ev = data->monData[monSlot].baseEv.def + data->monData[monSlot].selectedEv.def;
            tmp = (((2 * base + iv + ev / 4) * level) / 100) + 5;
            nature = GetNature(mon);
            tmp = ModifyStatByNature(nature, tmp, STAT_DEF);
            return tmp;
        case EV_STAT_SPATK:
            base = gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].baseSpAttack;
            level = GetMonData(mon, MON_DATA_LEVEL);
            iv = GetMonData(mon, MON_DATA_SPATK_IV);
            ev = data->monData[monSlot].baseEv.spatk + data->monData[monSlot].selectedEv.spatk;
            tmp = (((2 * base + iv + ev / 4) * level) / 100) + 5;
            nature = GetNature(mon);
            tmp = ModifyStatByNature(nature, tmp, STAT_SPATK);
            return tmp;
        case EV_STAT_SPDEF:
            base = gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].baseSpDefense;
            level = GetMonData(mon, MON_DATA_LEVEL);
            iv = GetMonData(mon, MON_DATA_SPDEF_IV);
            ev = data->monData[monSlot].baseEv.spdef + data->monData[monSlot].selectedEv.spdef;
            tmp = (((2 * base + iv + ev / 4) * level) / 100) + 5;
            nature = GetNature(mon);
            tmp = ModifyStatByNature(nature, tmp, STAT_SPDEF);
            return tmp;
        case EV_STAT_SPEED:
            base = gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].baseSpeed;
            level = GetMonData(mon, MON_DATA_LEVEL);
            iv = GetMonData(mon, MON_DATA_SPEED_IV);
            ev = data->monData[monSlot].baseEv.speed + data->monData[monSlot].selectedEv.speed;
            tmp = (((2 * base + iv + ev / 4) * level) / 100) + 5;
            nature = GetNature(mon);
            tmp = ModifyStatByNature(nature, tmp, STAT_SPEED);
            return tmp;
    }
}

static bool8 EvScreen_TryAddEv(u8 stat, u8 monSlot)
{
    if (EvScreen_CalcTotalEvs(monSlot) >= 512)
        return FALSE;
    if (EvScreen_CalcTotalEvForStat(stat, monSlot) >= 252)
        return FALSE;
    if (data->monData[monSlot].currTmpEv < 1)
        return FALSE;
    data->monData[monSlot].currTmpEv--;
    
    switch(stat)
    {
        case EV_STAT_HP:
            data->monData[monSlot].selectedEv.hp++;
            return TRUE;
        case EV_STAT_ATK:
            data->monData[monSlot].selectedEv.atk++;
            return TRUE;
        case EV_STAT_DEF:
            data->monData[monSlot].selectedEv.def++;
            return TRUE;
        case EV_STAT_SPATK:
            data->monData[monSlot].selectedEv.spatk++;
            return TRUE;
        case EV_STAT_SPDEF:
            data->monData[monSlot].selectedEv.spdef++;
            return TRUE;
        case EV_STAT_SPEED:
            data->monData[monSlot].selectedEv.speed++;
            return TRUE;
        default:
            return FALSE;
    }
}

static bool8 EvScreen_TryRemoveEv(u8 stat, u8 monSlot)
{
    if (EvScreen_GetMonStatSelectedEvs(stat, monSlot) < 1)
        return FALSE;
    data->monData[monSlot].currTmpEv++;

    switch(stat)
    {
        case EV_STAT_HP:
            data->monData[monSlot].selectedEv.hp--;
            return TRUE;
        
        case EV_STAT_ATK:
            data->monData[monSlot].selectedEv.atk--;
            return TRUE;
        
        case EV_STAT_DEF:
            data->monData[monSlot].selectedEv.def--;
            return TRUE;

        case EV_STAT_SPATK:
            data->monData[monSlot].selectedEv.spatk--;
            return TRUE;
        
        case EV_STAT_SPDEF:
            data->monData[monSlot].selectedEv.spdef--;
            return TRUE;
        
        case EV_STAT_SPEED:
            data->monData[monSlot].selectedEv.speed--;
            return TRUE;
        default:
            return FALSE;
    }
}

static void EvScreen_LoadSelectedMonData()
{
    // destroys previous sprite and creates a new one
    DestroySprite(&gSprites[sSelectedMonSpriteId]);
    EvScreen_CreateSelectedMonSprite();
    // load current mon stats and Evs
    EvScreen_PrintSelectedMonStats();
    EvScreen_PrintSelectedMonEVs();
}

static u16 EvScreen_CalcTotalEvs(u8 monSlot) {
    u16 total = 0;
    total += data->monData[monSlot].baseEv.hp + data->monData[monSlot].selectedEv.hp;
    total += data->monData[monSlot].baseEv.atk + data->monData[monSlot].selectedEv.atk;
    total += data->monData[monSlot].baseEv.def + data->monData[monSlot].selectedEv.def;
    total += data->monData[monSlot].baseEv.spatk + data->monData[monSlot].selectedEv.spatk;
    total += data->monData[monSlot].baseEv.spdef + data->monData[monSlot].selectedEv.spdef;
    total += data->monData[monSlot].baseEv.speed + data->monData[monSlot].selectedEv.speed;
    return total;
}

static u16 EvScreen_GetMonStatSelectedEvs(u8 stat, u8 monSlot)
{
    switch(stat)
    {
        case EV_STAT_HP:
            return data->monData[monSlot].selectedEv.hp;
        case EV_STAT_ATK:
            return data->monData[monSlot].selectedEv.atk;
        case EV_STAT_DEF:
            return data->monData[monSlot].selectedEv.def;
        case EV_STAT_SPATK:
            return data->monData[monSlot].selectedEv.spatk;
        case EV_STAT_SPDEF:
            return data->monData[monSlot].selectedEv.spdef;
        case EV_STAT_SPEED:
            return data->monData[monSlot].selectedEv.speed;
    }
}

static u16 EvScreen_CalcTotalEvForStat(u8 stat, u8 monSlot)
{
    switch(stat)
    {
        case EV_STAT_HP:
            return data->monData[monSlot].baseEv.hp + data->monData[monSlot].selectedEv.hp;
        case EV_STAT_ATK:
            return data->monData[monSlot].baseEv.atk + data->monData[monSlot].selectedEv.atk;
        case EV_STAT_DEF:
            return data->monData[monSlot].baseEv.def + data->monData[monSlot].selectedEv.def;
        case EV_STAT_SPATK:
            return data->monData[monSlot].baseEv.spatk + data->monData[monSlot].selectedEv.spatk;
        case EV_STAT_SPDEF:
            return data->monData[monSlot].baseEv.spdef + data->monData[monSlot].selectedEv.spdef;
        case EV_STAT_SPEED:
            return data->monData[monSlot].baseEv.speed + data->monData[monSlot].selectedEv.speed;
        default:
            return 0;
    }
}

#define sArrowType data[0]
#define sCurrentDirection data[1]
#define sTimer data[2]
#define sAcumulatedRight data[3]
#define sAcumulatedLeft data[4]
#define sLastPressedCountdown data[5]
#define sLastSelectedStat data[6]
static void SpriteCB_StatSelectionSprite(struct Sprite* sprite)
{   
    // moves sprite arrow to coordinates
    sprite->y2 = STAT_ARROW_BASE_Y + STAT_ARROW_PADDING_Y * sSelectedStat;

    // if the selected stat has changed, resets the sprite
    if (sprite->sLastSelectedStat != sSelectedStat)
    {
        sprite->invisible = FALSE;
        sprite->sLastSelectedStat = sSelectedStat;
        sprite->sCurrentDirection = D_NONE;
        sprite->sLastPressedCountdown = 0;
        sprite->sTimer = 0;
    }

    // if the arrow is moving to the right or left
    if (JOY_REPEAT(DPAD_RIGHT)) {
        sprite->sCurrentDirection = DIR_RIGHT;
        sprite->sLastPressedCountdown = 30;
    }
    if (JOY_REPEAT(DPAD_LEFT)) {
        sprite->sCurrentDirection = DIR_LEFT;
        sprite->sLastPressedCountdown = 30;
    }

    if (sprite->sLastPressedCountdown > 0)
    {
        sprite->invisible = FALSE;
        sprite->sLastPressedCountdown--;
    }
    else if (sprite->sTimer % 45 < 35)
    {
        sprite->invisible = FALSE;
    }
    else
    {
        sprite->invisible = TRUE;
    }

    if ((sprite->sCurrentDirection == DIR_LEFT && sprite->sArrowType != ARROW_LEFT)
        || (sprite->sCurrentDirection == DIR_RIGHT && sprite->sArrowType != ARROW_RIGHT))
    {
        sprite->invisible = TRUE;
    }
    
    
    sprite->sTimer++;
}
#undef sArrowType
#undef sCurrentDirection
#undef sTimer
#undef sAcumulatedRight
#undef sAcumulatedLeft
#undef sLastPressedCountdown
#undef sLastSelectedStat

static void SpriteCB_MonSelector(struct Sprite* sprite)
{
    sprite->y2 = MON_SELECTOR_BASE_Y + MON_SELECTOR_PADDING_Y * sSelectedPartyMonId;

    if (sSelectedPartyMonId % 2 == 0) 
    {
        sprite->x2 = MON_SELECTOR_BASE_X;
        StartSpriteAnim(sprite, ARROW_LEFT);
    }
    else
    {
        sprite->x2 = MON_SELECTOR_BASE_X + MON_SELECTOR_PADDING_X;
        StartSpriteAnim(sprite, ARROW_RIGHT);
    }
}

#define sMonIconStill data[3]
static void SpriteCb_MonIcon(struct Sprite *sprite)
{
    if (!sprite->sMonIconStill)
        UpdateMonIconFrame(sprite);
}
#undef sMonIconStill
