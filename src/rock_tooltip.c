#include "global.h"
#include "decompress.h"
#include "palette.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "rock_tooltip.h"
#include "constants/item.h"
#include "constants/songs.h"

static const u32 sSpriteGfx_TooltipKimberlita[] = INCBIN_U32("graphics/rock_tooltip/tooltip_kimberlita.4bpp.lz");
static const u32 sSpriteGfx_TooltipDiorita[] = INCBIN_U32("graphics/rock_tooltip/tooltip_diorita.4bpp.lz");
static const u32 sSpriteGfx_TooltipBasalto[] = INCBIN_U32("graphics/rock_tooltip/tooltip_basalto.4bpp.lz");
static const u32 sSpriteGfx_TooltipRoca[] = INCBIN_U32("graphics/rock_tooltip/tooltip_roca.4bpp.lz");
static const u32 sSpriteGfx_TooltipObsidiana[] = INCBIN_U32("graphics/rock_tooltip/tooltip_obsidiana.4bpp.lz");
static const u32 sSpriteGfx_TooltipTopaco[] = INCBIN_U32("graphics/rock_tooltip/tooltip_topaco.4bpp.lz");
static const u16 sSpritepal_Tooltip[] = INCBIN_U16("graphics/rock_tooltip/tooltip_kimberlita.gbapal");

#define TAG_TOOLTIP_KIMBERLITA  57300
#define TAG_TOOLTIP_DIORITA     57301
#define TAG_TOOLTIP_BASALTO     57302
#define TAG_TOOLTIP_ROCA        57303
#define TAG_TOOLTIP_OBSIDIANA   57304
#define TAG_TOOLTIP_TOPACO      57305

#define PAL_TAG_TOOLTIP    57300

static void SpriteCallback_RockTooltipSlideIn(struct Sprite* sprite);
static void SpriteCallback_RockTooltipWait(struct Sprite* sprite);
static void SpriteCallback_SlideOut(struct Sprite* sprite);
static void CreateRockTooltipSprite(u8 rockId, u8 slot);
static u8 GetFreeTooltipSlot();

EWRAM_DATA u8 sTooltipMask = 0;

static const struct CompressedSpriteSheet sSpriteSheet_TooltipKimberlita = 
{
    .data = sSpriteGfx_TooltipKimberlita,
    .size = 0x400,
    .tag = TAG_TOOLTIP_KIMBERLITA,
};

static const struct CompressedSpriteSheet sSpriteSheet_TooltipDiorita = 
{
    .data = sSpriteGfx_TooltipDiorita,
    .size = 0x400,
    .tag = TAG_TOOLTIP_DIORITA,
};

static const struct CompressedSpriteSheet sSpriteSheet_TooltipBasalto = 
{
    .data = sSpriteGfx_TooltipBasalto,
    .size = 0x400,
    .tag = TAG_TOOLTIP_BASALTO,
};

static const struct CompressedSpriteSheet sSpriteSheet_TooltipRoca = 
{
    .data = sSpriteGfx_TooltipRoca,
    .size = 0x400,
    .tag = TAG_TOOLTIP_ROCA,
};

static const struct CompressedSpriteSheet sSpriteSheet_TooltipObsidiana = 
{
    .data = sSpriteGfx_TooltipObsidiana,
    .size = 0x400,
    .tag = TAG_TOOLTIP_OBSIDIANA,
};

static const struct CompressedSpriteSheet sSpriteSheet_TooltipTopaco = 
{
    .data = sSpriteGfx_TooltipTopaco,
    .size = 0x400,
    .tag = TAG_TOOLTIP_TOPACO,
};

static const struct SpritePalette sSpritePalette_Tooltip = 
{
    .data = sSpritepal_Tooltip,
    .tag = PAL_TAG_TOOLTIP,
};

static const struct OamData sOamData_Tooltip = 
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
    .priority = 0,
};

static const struct SpriteTemplate sSpriteTemplate_TooltipKimberlita = 
{
    .tileTag = TAG_TOOLTIP_KIMBERLITA,
    .paletteTag = PAL_TAG_TOOLTIP,
    .oam = &sOamData_Tooltip,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallback_RockTooltipSlideIn,
};

static const struct SpriteTemplate sSpriteTemplate_TooltipDiorita = 
{
    .tileTag = TAG_TOOLTIP_DIORITA,
    .paletteTag = PAL_TAG_TOOLTIP,
    .oam = &sOamData_Tooltip,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallback_RockTooltipSlideIn,
};

static const struct SpriteTemplate sSpriteTemplate_TooltipBasalto = 
{
    .tileTag = TAG_TOOLTIP_BASALTO,
    .paletteTag = PAL_TAG_TOOLTIP,
    .oam = &sOamData_Tooltip,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallback_RockTooltipSlideIn,
};

static const struct SpriteTemplate sSpriteTemplate_TooltipRoca = 
{
    .tileTag = TAG_TOOLTIP_ROCA,
    .paletteTag = PAL_TAG_TOOLTIP,
    .oam = &sOamData_Tooltip,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallback_RockTooltipSlideIn,
};

static const struct SpriteTemplate sSpriteTemplate_TooltipObsidiana = 
{
    .tileTag = TAG_TOOLTIP_OBSIDIANA,
    .paletteTag = PAL_TAG_TOOLTIP,
    .oam = &sOamData_Tooltip,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallback_RockTooltipSlideIn,
};

static const struct SpriteTemplate sSpriteTemplate_TooltipTopaco = 
{
    .tileTag = TAG_TOOLTIP_TOPACO,
    .paletteTag = PAL_TAG_TOOLTIP,
    .oam = &sOamData_Tooltip,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallback_RockTooltipSlideIn,
};

#define sTimer          data[0]
#define sTooltipSlot    data[1]
static void SpriteCallback_RockTooltipSlideIn(struct Sprite* sprite)
{
    if (sprite->pos2.x < -TOOLTIP_X_MOVEMENT)
        sprite->callback = SpriteCallback_RockTooltipWait;
    else
        sprite->pos2.x -= TOOLTIP_SPEED;
}

static void SpriteCallback_RockTooltipWait(struct Sprite* sprite)
{
    if (sprite->sTimer > TOOLTIP_WAIT_TIME)
        sprite->callback = SpriteCallback_SlideOut;

    sprite->sTimer++;
}

static void SpriteCallback_SlideOut(struct Sprite* sprite)
{
    u8 mask;
    sprite->pos2.x += TOOLTIP_SPEED;
    if (sprite->pos2.x == 0)
    {
        mask = 1 << sprite->sTooltipSlot;
        mask = ~mask; 
        sTooltipMask = sTooltipMask & mask;
        DestroySpriteAndFreeResources(sprite);
    }
}
#undef sTimer

void CreateRockTooltip(u8 rockId)
{
    LoadSpritePalette(&sSpritePalette_Tooltip);
    CreateRockTooltipSprite(rockId, GetFreeTooltipSlot());
    PlaySE(TOOLTIP_SOUND_EFFECT);
}

static void CreateRockTooltipSprite(u8 rockId, u8 slot)
{
    u8 id;

    if (slot == 0xFF)
        return;

    switch (rockId)
    {
    default:
    case ROCK_KIMBERLITA:
        LoadCompressedSpriteSheet(&sSpriteSheet_TooltipKimberlita);
        id = CreateSprite(&sSpriteTemplate_TooltipKimberlita, TOOLTIP_POS_X, TOOLTIP_POS_Y - TOOLTIP_OFFSET_Y * slot, 0);
        break;
    
    case ROCK_DIORITA:
        LoadCompressedSpriteSheet(&sSpriteSheet_TooltipDiorita);
        id = CreateSprite(&sSpriteTemplate_TooltipDiorita, TOOLTIP_POS_X, TOOLTIP_POS_Y - TOOLTIP_OFFSET_Y * slot, 0);
        break;
    
    case ROCK_BASALTO:
        LoadCompressedSpriteSheet(&sSpriteSheet_TooltipBasalto);
        id = CreateSprite(&sSpriteTemplate_TooltipBasalto, TOOLTIP_POS_X, TOOLTIP_POS_Y - TOOLTIP_OFFSET_Y * slot, 0);
        break;

    case ROCK_ROCA:
        LoadCompressedSpriteSheet(&sSpriteSheet_TooltipRoca);
        id = CreateSprite(&sSpriteTemplate_TooltipRoca, TOOLTIP_POS_X, TOOLTIP_POS_Y - TOOLTIP_OFFSET_Y * slot, 0);
        break;

    case ROCK_OBSIDIANA:
        LoadCompressedSpriteSheet(&sSpriteSheet_TooltipObsidiana);
        id = CreateSprite(&sSpriteTemplate_TooltipObsidiana, TOOLTIP_POS_X, TOOLTIP_POS_Y - TOOLTIP_OFFSET_Y * slot, 0);
        break;

    case ROCK_TOPACO:
        LoadCompressedSpriteSheet(&sSpriteSheet_TooltipTopaco);
        id = CreateSprite(&sSpriteTemplate_TooltipTopaco, TOOLTIP_POS_X, TOOLTIP_POS_Y - TOOLTIP_OFFSET_Y * slot, 0);
        break;
    }

    gSprites[id].sTooltipSlot = slot;
    sTooltipMask |= (1 << slot);
}

static u8 GetFreeTooltipSlot()
{
    u32 i;
    u8 mask;

    for (i = 0, mask = 1; i < MAX_NUMBER_OF_TOOLTIPS; i++)
    {
        if (mask & sTooltipMask)
            mask = mask << 1;
        else
            return i;
    }
    return 0xFF;
}

#undef sTooltipSlot
