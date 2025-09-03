#include "global.h"
#include "arcane_suzume_door.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "palette.h"
#include "constants/rgb.h"

static const union AffineAnimCmd sAffineCmd_SuzumeWarpOut[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(-2, 0, 0, 4),   //8
    AFFINEANIMCMD_FRAME(-5, 0, 0, 4),   //20 -> 28
    AFFINEANIMCMD_FRAME(-10, 0, 0, 4),  //40 -> 68
    AFFINEANIMCMD_FRAME(-20, 0, 0, 4),  //80 -> 148
    AFFINEANIMCMD_FRAME(-40, -20, 0, 2),  //160 -> 308
    AFFINEANIMCMD_FRAME(0, -50, 0, 4),  // (308, 240) 
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineTable_SuzumeWarpOut[] =
{
    sAffineCmd_SuzumeWarpOut,
};

static const union AffineAnimCmd sAffineCmd_SuzumeWarpInto[] =
{
    AFFINEANIMCMD_FRAME(-56, 16, 0, 0),
    AFFINEANIMCMD_FRAME(0, 50, 0, 4),  // (308, 240) 
    AFFINEANIMCMD_FRAME(40, 20, 0, 2),  //160 -> 308
    AFFINEANIMCMD_FRAME(20, 0, 0, 4),  //80 -> 148
    AFFINEANIMCMD_FRAME(10, 0, 0, 4),  //40 -> 68
    AFFINEANIMCMD_FRAME(5, 0, 0, 4),   //20 -> 28
    AFFINEANIMCMD_FRAME(2, 0, 0, 4),   //8
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};


static const union AffineAnimCmd *const sAffineTable_SuzumeWarpInto[] =
{
    sAffineCmd_SuzumeWarpInto,
};

static void SpriteCB_SuzumeWarpOut(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
    {
        sprite->invisible = TRUE;
        // TODO: Do warp
    }
}

void Suzume_InitDoorWarp()
{
    u16 mask;
    u8 spriteId, graphicsId, pseudoId;
    spriteId = gPlayerAvatar.spriteId;
    gSprites[spriteId].invisible = TRUE;
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = TRUE;
    
    graphicsId = GetPlayerAvatarGraphicsIdByStateId(0);
    pseudoId = CreateObjectGraphicsSpriteSuzumeDoor(graphicsId, SpriteCB_SuzumeWarpOut, 120, 72, 0);
    StartSpriteAnim(&gSprites[pseudoId], 1);
    gSprites[pseudoId].affineAnims = sAffineTable_SuzumeWarpOut;
    mask = 1 << gSprites[pseudoId].oam.paletteNum;

    BeginNormalPaletteFade(mask << 16, 0, 0, 0x10, RGB_WHITE);
}


