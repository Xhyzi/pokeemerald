#include "global.h"
#include "graphics.h"
#include "mail.h"
#include "palette.h"
#include "pokemon_icon.h"
#include "sprite.h"

#define POKE_ICON_BASE_PAL_TAG 56000

#define INVALID_ICON_SPECIES SPECIES_OLD_UNOWN_J // Oddly specific, used when an icon should be a ?. Any of the 'old unown' would work

struct MonIconSpriteTemplate
{
    const struct OamData *oam;
    const u8 *image;
    const union AnimCmd *const *anims;
    const union AffineAnimCmd *const *affineAnims;
    void (*callback)(struct Sprite *);
    u16 paletteTag;
};

// static functions
static u8 CreateMonIconSprite(struct MonIconSpriteTemplate *, s16, s16, u8);

// .rodata

const u8 *const gMonIconTable[] =
{
    [SPECIES_NONE] = gMonIcon_Castform,
    [SPECIES_BULBASAUR] = gMonIcon_Castform,
    [SPECIES_IVYSAUR] = gMonIcon_Castform,
    [SPECIES_VENUSAUR] = gMonIcon_Castform,
    [SPECIES_CHARMANDER] = gMonIcon_Castform,
    [SPECIES_CHARMELEON] = gMonIcon_Castform,
    [SPECIES_CHARIZARD] = gMonIcon_Castform,
    [SPECIES_SQUIRTLE] = gMonIcon_Castform,
    [SPECIES_WARTORTLE] = gMonIcon_Castform,
    [SPECIES_BLASTOISE] = gMonIcon_Castform,
    [SPECIES_CATERPIE] = gMonIcon_Castform,
    [SPECIES_METAPOD] = gMonIcon_Castform,
    [SPECIES_BUTTERFREE] = gMonIcon_Castform,
    [SPECIES_WEEDLE] = gMonIcon_Castform,
    [SPECIES_KAKUNA] = gMonIcon_Castform,
    [SPECIES_BEEDRILL] = gMonIcon_Castform,
    [SPECIES_PIDGEY] = gMonIcon_Castform,
    [SPECIES_PIDGEOTTO] = gMonIcon_Castform,
    [SPECIES_PIDGEOT] = gMonIcon_Castform,
    [SPECIES_RATTATA] = gMonIcon_Castform,
    [SPECIES_RATICATE] = gMonIcon_Castform,
    [SPECIES_SPEAROW] = gMonIcon_Castform,
    [SPECIES_FEAROW] = gMonIcon_Castform,
    [SPECIES_EKANS] = gMonIcon_Castform,
    [SPECIES_ARBOK] = gMonIcon_Castform,
    [SPECIES_PIKACHU] = gMonIcon_Castform,
    [SPECIES_RAICHU] = gMonIcon_Castform,
    [SPECIES_SANDSHREW] = gMonIcon_Castform,
    [SPECIES_SANDSLASH] = gMonIcon_Castform,
    [SPECIES_NIDORAN_F] = gMonIcon_Castform,
    [SPECIES_NIDORINA] = gMonIcon_Castform,
    [SPECIES_NIDOQUEEN] = gMonIcon_Castform,
    [SPECIES_NIDORAN_M] = gMonIcon_Castform,
    [SPECIES_NIDORINO] = gMonIcon_Castform,
    [SPECIES_NIDOKING] = gMonIcon_Castform,
    [SPECIES_CLEFAIRY] = gMonIcon_Castform,
    [SPECIES_CLEFABLE] = gMonIcon_Castform,
    [SPECIES_VULPIX] = gMonIcon_Castform,
    [SPECIES_NINETALES] = gMonIcon_Castform,
    [SPECIES_JIGGLYPUFF] = gMonIcon_Castform,
    [SPECIES_WIGGLYTUFF] = gMonIcon_Castform,
    [SPECIES_ZUBAT] = gMonIcon_Castform,
    [SPECIES_GOLBAT] = gMonIcon_Castform,
    [SPECIES_ODDISH] = gMonIcon_Castform,
    [SPECIES_GLOOM] = gMonIcon_Castform,
    [SPECIES_VILEPLUME] = gMonIcon_Castform,
    [SPECIES_PARAS] = gMonIcon_Castform,
    [SPECIES_PARASECT] = gMonIcon_Castform,
    [SPECIES_VENONAT] = gMonIcon_Castform,
    [SPECIES_VENOMOTH] = gMonIcon_Castform,
    [SPECIES_DIGLETT] = gMonIcon_Castform,
    [SPECIES_DUGTRIO] = gMonIcon_Castform,
    [SPECIES_MEOWTH] = gMonIcon_Castform,
    [SPECIES_PERSIAN] = gMonIcon_Castform,
    [SPECIES_PSYDUCK] = gMonIcon_Castform,
    [SPECIES_GOLDUCK] = gMonIcon_Castform,
    [SPECIES_MANKEY] = gMonIcon_Castform,
    [SPECIES_PRIMEAPE] = gMonIcon_Castform,
    [SPECIES_GROWLITHE] = gMonIcon_Castform,
    [SPECIES_ARCANINE] = gMonIcon_Castform,
    [SPECIES_POLIWAG] = gMonIcon_Castform,
    [SPECIES_POLIWHIRL] = gMonIcon_Castform,
    [SPECIES_POLIWRATH] = gMonIcon_Castform,
    [SPECIES_ABRA] = gMonIcon_Castform,
    [SPECIES_KADABRA] = gMonIcon_Castform,
    [SPECIES_ALAKAZAM] = gMonIcon_Castform,
    [SPECIES_MACHOP] = gMonIcon_Castform,
    [SPECIES_MACHOKE] = gMonIcon_Castform,
    [SPECIES_MACHAMP] = gMonIcon_Castform,
    [SPECIES_BELLSPROUT] = gMonIcon_Castform,
    [SPECIES_WEEPINBELL] = gMonIcon_Castform,
    [SPECIES_VICTREEBEL] = gMonIcon_Castform,
    [SPECIES_TENTACOOL] = gMonIcon_Castform,
    [SPECIES_TENTACRUEL] = gMonIcon_Castform,
    [SPECIES_GEODUDE] = gMonIcon_Castform,
    [SPECIES_GRAVELER] = gMonIcon_Castform,
    [SPECIES_GOLEM] = gMonIcon_Castform,
    [SPECIES_PONYTA] = gMonIcon_Castform,
    [SPECIES_RAPIDASH] = gMonIcon_Castform,
    [SPECIES_SLOWPOKE] = gMonIcon_Castform,
    [SPECIES_SLOWBRO] = gMonIcon_Castform,
    [SPECIES_MAGNEMITE] = gMonIcon_Castform,
    [SPECIES_MAGNETON] = gMonIcon_Castform,
    [SPECIES_FARFETCHD] = gMonIcon_Castform,
    [SPECIES_DODUO] = gMonIcon_Castform,
    [SPECIES_DODRIO] = gMonIcon_Castform,
    [SPECIES_SEEL] = gMonIcon_Castform,
    [SPECIES_DEWGONG] = gMonIcon_Castform,
    [SPECIES_GRIMER] = gMonIcon_Castform,
    [SPECIES_MUK] = gMonIcon_Castform,
    [SPECIES_SHELLDER] = gMonIcon_Castform,
    [SPECIES_CLOYSTER] = gMonIcon_Castform,
    [SPECIES_GASTLY] = gMonIcon_Castform,
    [SPECIES_HAUNTER] = gMonIcon_Castform,
    [SPECIES_GENGAR] = gMonIcon_Castform,
    [SPECIES_ONIX] = gMonIcon_Castform,
    [SPECIES_DROWZEE] = gMonIcon_Castform,
    [SPECIES_HYPNO] = gMonIcon_Castform,
    [SPECIES_KRABBY] = gMonIcon_Castform,
    [SPECIES_KINGLER] = gMonIcon_Castform,
    [SPECIES_VOLTORB] = gMonIcon_Castform,
    [SPECIES_ELECTRODE] = gMonIcon_Castform,
    [SPECIES_EXEGGCUTE] = gMonIcon_Castform,
    [SPECIES_EXEGGUTOR] = gMonIcon_Castform,
    [SPECIES_CUBONE] = gMonIcon_Castform,
    [SPECIES_MAROWAK] = gMonIcon_Castform,
    [SPECIES_HITMONLEE] = gMonIcon_Castform,
    [SPECIES_HITMONCHAN] = gMonIcon_Castform,
    [SPECIES_LICKITUNG] = gMonIcon_Castform,
    [SPECIES_KOFFING] = gMonIcon_Castform,
    [SPECIES_WEEZING] = gMonIcon_Castform,
    [SPECIES_RHYHORN] = gMonIcon_Castform,
    [SPECIES_RHYDON] = gMonIcon_Castform,
    [SPECIES_CHANSEY] = gMonIcon_Castform,
    [SPECIES_TANGELA] = gMonIcon_Castform,
    [SPECIES_KANGASKHAN] = gMonIcon_Castform,
    [SPECIES_HORSEA] = gMonIcon_Castform,
    [SPECIES_SEADRA] = gMonIcon_Castform,
    [SPECIES_GOLDEEN] = gMonIcon_Castform,
    [SPECIES_SEAKING] = gMonIcon_Castform,
    [SPECIES_STARYU] = gMonIcon_Castform,
    [SPECIES_STARMIE] = gMonIcon_Castform,
    [SPECIES_MR_MIME] = gMonIcon_Castform,
    [SPECIES_SCYTHER] = gMonIcon_Castform,
    [SPECIES_JYNX] = gMonIcon_Castform,
    [SPECIES_ELECTABUZZ] = gMonIcon_Castform,
    [SPECIES_MAGMAR] = gMonIcon_Castform,
    [SPECIES_PINSIR] = gMonIcon_Castform,
    [SPECIES_TAUROS] = gMonIcon_Castform,
    [SPECIES_MAGIKARP] = gMonIcon_Castform,
    [SPECIES_GYARADOS] = gMonIcon_Castform,
    [SPECIES_LAPRAS] = gMonIcon_Castform,
    [SPECIES_DITTO] = gMonIcon_Castform,
    [SPECIES_EEVEE] = gMonIcon_Castform,
    [SPECIES_VAPOREON] = gMonIcon_Castform,
    [SPECIES_JOLTEON] = gMonIcon_Castform,
    [SPECIES_FLAREON] = gMonIcon_Castform,
    [SPECIES_PORYGON] = gMonIcon_Castform,
    [SPECIES_OMANYTE] = gMonIcon_Castform,
    [SPECIES_OMASTAR] = gMonIcon_Castform,
    [SPECIES_KABUTO] = gMonIcon_Castform,
    [SPECIES_KABUTOPS] = gMonIcon_Castform,
    [SPECIES_AERODACTYL] = gMonIcon_Castform,
    [SPECIES_SNORLAX] = gMonIcon_Castform,
    [SPECIES_ARTICUNO] = gMonIcon_Castform,
    [SPECIES_ZAPDOS] = gMonIcon_Castform,
    [SPECIES_MOLTRES] = gMonIcon_Castform,
    [SPECIES_DRATINI] = gMonIcon_Castform,
    [SPECIES_DRAGONAIR] = gMonIcon_Castform,
    [SPECIES_DRAGONITE] = gMonIcon_Castform,
    [SPECIES_MEWTWO] = gMonIcon_Castform,
    [SPECIES_MEW] = gMonIcon_Castform,
    [SPECIES_CHIKORITA] = gMonIcon_Castform,
    [SPECIES_BAYLEEF] = gMonIcon_Castform,
    [SPECIES_MEGANIUM] = gMonIcon_Castform,
    [SPECIES_CYNDAQUIL] = gMonIcon_Castform,
    [SPECIES_QUILAVA] = gMonIcon_Castform,
    [SPECIES_TYPHLOSION] = gMonIcon_Castform,
    [SPECIES_TOTODILE] = gMonIcon_Castform,
    [SPECIES_CROCONAW] = gMonIcon_Castform,
    [SPECIES_FERALIGATR] = gMonIcon_Castform,
    [SPECIES_SENTRET] = gMonIcon_Castform,
    [SPECIES_FURRET] = gMonIcon_Castform,
    [SPECIES_HOOTHOOT] = gMonIcon_Castform,
    [SPECIES_NOCTOWL] = gMonIcon_Castform,
    [SPECIES_LEDYBA] = gMonIcon_Castform,
    [SPECIES_LEDIAN] = gMonIcon_Castform,
    [SPECIES_SPINARAK] = gMonIcon_Castform,
    [SPECIES_ARIADOS] = gMonIcon_Castform,
    [SPECIES_CROBAT] = gMonIcon_Castform,
    [SPECIES_CHINCHOU] = gMonIcon_Castform,
    [SPECIES_LANTURN] = gMonIcon_Castform,
    [SPECIES_PICHU] = gMonIcon_Castform,
    [SPECIES_CLEFFA] = gMonIcon_Castform,
    [SPECIES_IGGLYBUFF] = gMonIcon_Castform,
    [SPECIES_TOGEPI] = gMonIcon_Castform,
    [SPECIES_TOGETIC] = gMonIcon_Castform,
    [SPECIES_NATU] = gMonIcon_Castform,
    [SPECIES_XATU] = gMonIcon_Castform,
    [SPECIES_MAREEP] = gMonIcon_Castform,
    [SPECIES_FLAAFFY] = gMonIcon_Castform,
    [SPECIES_AMPHAROS] = gMonIcon_Castform,
    [SPECIES_BELLOSSOM] = gMonIcon_Castform,
    [SPECIES_MARILL] = gMonIcon_Castform,
    [SPECIES_AZUMARILL] = gMonIcon_Castform,
    [SPECIES_SUDOWOODO] = gMonIcon_Castform,
    [SPECIES_POLITOED] = gMonIcon_Castform,
    [SPECIES_HOPPIP] = gMonIcon_Castform,
    [SPECIES_SKIPLOOM] = gMonIcon_Castform,
    [SPECIES_JUMPLUFF] = gMonIcon_Castform,
    [SPECIES_AIPOM] = gMonIcon_Castform,
    [SPECIES_SUNKERN] = gMonIcon_Castform,
    [SPECIES_SUNFLORA] = gMonIcon_Castform,
    [SPECIES_YANMA] = gMonIcon_Castform,
    [SPECIES_WOOPER] = gMonIcon_Castform,
    [SPECIES_QUAGSIRE] = gMonIcon_Castform,
    [SPECIES_ESPEON] = gMonIcon_Castform,
    [SPECIES_UMBREON] = gMonIcon_Castform,
    [SPECIES_MURKROW] = gMonIcon_Castform,
    [SPECIES_SLOWKING] = gMonIcon_Castform,
    [SPECIES_MISDREAVUS] = gMonIcon_Castform,
    [SPECIES_UNOWN] = gMonIcon_Castform,
    [SPECIES_WOBBUFFET] = gMonIcon_Castform,
    [SPECIES_GIRAFARIG] = gMonIcon_Castform,
    [SPECIES_PINECO] = gMonIcon_Castform,
    [SPECIES_FORRETRESS] = gMonIcon_Castform,
    [SPECIES_DUNSPARCE] = gMonIcon_Castform,
    [SPECIES_GLIGAR] = gMonIcon_Castform,
    [SPECIES_STEELIX] = gMonIcon_Castform,
    [SPECIES_SNUBBULL] = gMonIcon_Castform,
    [SPECIES_GRANBULL] = gMonIcon_Castform,
    [SPECIES_QWILFISH] = gMonIcon_Castform,
    [SPECIES_SCIZOR] = gMonIcon_Castform,
    [SPECIES_SHUCKLE] = gMonIcon_Castform,
    [SPECIES_HERACROSS] = gMonIcon_Castform,
    [SPECIES_SNEASEL] = gMonIcon_Castform,
    [SPECIES_TEDDIURSA] = gMonIcon_Castform,
    [SPECIES_URSARING] = gMonIcon_Castform,
    [SPECIES_SLUGMA] = gMonIcon_Castform,
    [SPECIES_MAGCARGO] = gMonIcon_Castform,
    [SPECIES_SWINUB] = gMonIcon_Castform,
    [SPECIES_PILOSWINE] = gMonIcon_Castform,
    [SPECIES_CORSOLA] = gMonIcon_Castform,
    [SPECIES_REMORAID] = gMonIcon_Castform,
    [SPECIES_OCTILLERY] = gMonIcon_Castform,
    [SPECIES_DELIBIRD] = gMonIcon_Castform,
    [SPECIES_MANTINE] = gMonIcon_Castform,
    [SPECIES_SKARMORY] = gMonIcon_Castform,
    [SPECIES_HOUNDOUR] = gMonIcon_Castform,
    [SPECIES_HOUNDOOM] = gMonIcon_Castform,
    [SPECIES_KINGDRA] = gMonIcon_Castform,
    [SPECIES_PHANPY] = gMonIcon_Castform,
    [SPECIES_DONPHAN] = gMonIcon_Castform,
    [SPECIES_PORYGON2] = gMonIcon_Castform,
    [SPECIES_STANTLER] = gMonIcon_Castform,
    [SPECIES_SMEARGLE] = gMonIcon_Castform,
    [SPECIES_TYROGUE] = gMonIcon_Castform,
    [SPECIES_HITMONTOP] = gMonIcon_Castform,
    [SPECIES_SMOOCHUM] = gMonIcon_Castform,
    [SPECIES_ELEKID] = gMonIcon_Castform,
    [SPECIES_MAGBY] = gMonIcon_Castform,
    [SPECIES_MILTANK] = gMonIcon_Castform,
    [SPECIES_BLISSEY] = gMonIcon_Castform,
    [SPECIES_RAIKOU] = gMonIcon_Castform,
    [SPECIES_ENTEI] = gMonIcon_Castform,
    [SPECIES_SUICUNE] = gMonIcon_Castform,
    [SPECIES_LARVITAR] = gMonIcon_Castform,
    [SPECIES_PUPITAR] = gMonIcon_Castform,
    [SPECIES_TYRANITAR] = gMonIcon_Castform,
    [SPECIES_LUGIA] = gMonIcon_Castform,
    [SPECIES_HO_OH] = gMonIcon_Castform,
    [SPECIES_CELEBI] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_B] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_C] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_D] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_E] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_F] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_G] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_H] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_I] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_J] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_K] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_L] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_M] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_N] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_O] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_P] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_Q] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_R] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_S] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_T] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_U] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_V] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_W] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_X] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_Y] = gMonIcon_Castform,
    [SPECIES_OLD_UNOWN_Z] = gMonIcon_Castform,
    [SPECIES_TREECKO] = gMonIcon_Castform,
    [SPECIES_GROVYLE] = gMonIcon_Castform,
    [SPECIES_SCEPTILE] = gMonIcon_Castform,
    [SPECIES_TORCHIC] = gMonIcon_Castform,
    [SPECIES_COMBUSKEN] = gMonIcon_Castform,
    [SPECIES_BLAZIKEN] = gMonIcon_Castform,
    [SPECIES_MUDKIP] = gMonIcon_Castform,
    [SPECIES_MARSHTOMP] = gMonIcon_Castform,
    [SPECIES_SWAMPERT] = gMonIcon_Castform,
    [SPECIES_POOCHYENA] = gMonIcon_Castform,
    [SPECIES_MIGHTYENA] = gMonIcon_Castform,
    [SPECIES_ZIGZAGOON] = gMonIcon_Castform,
    [SPECIES_LINOONE] = gMonIcon_Castform,
    [SPECIES_WURMPLE] = gMonIcon_Castform,
    [SPECIES_SILCOON] = gMonIcon_Castform,
    [SPECIES_BEAUTIFLY] = gMonIcon_Castform,
    [SPECIES_CASCOON] = gMonIcon_Castform,
    [SPECIES_DUSTOX] = gMonIcon_Castform,
    [SPECIES_LOTAD] = gMonIcon_Castform,
    [SPECIES_LOMBRE] = gMonIcon_Castform,
    [SPECIES_LUDICOLO] = gMonIcon_Castform,
    [SPECIES_SEEDOT] = gMonIcon_Castform,
    [SPECIES_NUZLEAF] = gMonIcon_Castform,
    [SPECIES_SHIFTRY] = gMonIcon_Castform,
    [SPECIES_NINCADA] = gMonIcon_Castform,
    [SPECIES_NINJASK] = gMonIcon_Castform,
    [SPECIES_SHEDINJA] = gMonIcon_Castform,
    [SPECIES_TAILLOW] = gMonIcon_Castform,
    [SPECIES_SWELLOW] = gMonIcon_Castform,
    [SPECIES_SHROOMISH] = gMonIcon_Castform,
    [SPECIES_BRELOOM] = gMonIcon_Castform,
    [SPECIES_SPINDA] = gMonIcon_Castform,
    [SPECIES_WINGULL] = gMonIcon_Castform,
    [SPECIES_PELIPPER] = gMonIcon_Castform,
    [SPECIES_SURSKIT] = gMonIcon_Castform,
    [SPECIES_MASQUERAIN] = gMonIcon_Castform,
    [SPECIES_WAILMER] = gMonIcon_Castform,
    [SPECIES_WAILORD] = gMonIcon_Castform,
    [SPECIES_SKITTY] = gMonIcon_Castform,
    [SPECIES_DELCATTY] = gMonIcon_Castform,
    [SPECIES_KECLEON] = gMonIcon_Castform,
    [SPECIES_BALTOY] = gMonIcon_Castform,
    [SPECIES_CLAYDOL] = gMonIcon_Castform,
    [SPECIES_NOSEPASS] = gMonIcon_Castform,
    [SPECIES_TORKOAL] = gMonIcon_Castform,
    [SPECIES_SABLEYE] = gMonIcon_Castform,
    [SPECIES_BARBOACH] = gMonIcon_Castform,
    [SPECIES_WHISCASH] = gMonIcon_Castform,
    [SPECIES_LUVDISC] = gMonIcon_Castform,
    [SPECIES_CORPHISH] = gMonIcon_Castform,
    [SPECIES_CRAWDAUNT] = gMonIcon_Castform,
    [SPECIES_FEEBAS] = gMonIcon_Castform,
    [SPECIES_MILOTIC] = gMonIcon_Castform,
    [SPECIES_CARVANHA] = gMonIcon_Castform,
    [SPECIES_SHARPEDO] = gMonIcon_Castform,
    [SPECIES_TRAPINCH] = gMonIcon_Castform,
    [SPECIES_VIBRAVA] = gMonIcon_Castform,
    [SPECIES_FLYGON] = gMonIcon_Castform,
    [SPECIES_MAKUHITA] = gMonIcon_Castform,
    [SPECIES_HARIYAMA] = gMonIcon_Castform,
    [SPECIES_ELECTRIKE] = gMonIcon_Castform,
    [SPECIES_MANECTRIC] = gMonIcon_Castform,
    [SPECIES_NUMEL] = gMonIcon_Castform,
    [SPECIES_CAMERUPT] = gMonIcon_Castform,
    [SPECIES_SPHEAL] = gMonIcon_Castform,
    [SPECIES_SEALEO] = gMonIcon_Castform,
    [SPECIES_WALREIN] = gMonIcon_Castform,
    [SPECIES_CACNEA] = gMonIcon_Castform,
    [SPECIES_CACTURNE] = gMonIcon_Castform,
    [SPECIES_SNORUNT] = gMonIcon_Castform,
    [SPECIES_GLALIE] = gMonIcon_Castform,
    [SPECIES_LUNATONE] = gMonIcon_Castform,
    [SPECIES_SOLROCK] = gMonIcon_Castform,
    [SPECIES_AZURILL] = gMonIcon_Castform,
    [SPECIES_SPOINK] = gMonIcon_Castform,
    [SPECIES_GRUMPIG] = gMonIcon_Castform,
    [SPECIES_PLUSLE] = gMonIcon_Castform,
    [SPECIES_MINUN] = gMonIcon_Castform,
    [SPECIES_MAWILE] = gMonIcon_Castform,
    [SPECIES_MEDITITE] = gMonIcon_Castform,
    [SPECIES_MEDICHAM] = gMonIcon_Castform,
    [SPECIES_SWABLU] = gMonIcon_Castform,
    [SPECIES_ALTARIA] = gMonIcon_Castform,
    [SPECIES_WYNAUT] = gMonIcon_Castform,
    [SPECIES_DUSKULL] = gMonIcon_Castform,
    [SPECIES_DUSCLOPS] = gMonIcon_Castform,
    [SPECIES_ROSELIA] = gMonIcon_Castform,
    [SPECIES_SLAKOTH] = gMonIcon_Castform,
    [SPECIES_VIGOROTH] = gMonIcon_Castform,
    [SPECIES_SLAKING] = gMonIcon_Castform,
    [SPECIES_GULPIN] = gMonIcon_Castform,
    [SPECIES_SWALOT] = gMonIcon_Castform,
    [SPECIES_TROPIUS] = gMonIcon_Castform,
    [SPECIES_WHISMUR] = gMonIcon_Castform,
    [SPECIES_LOUDRED] = gMonIcon_Castform,
    [SPECIES_EXPLOUD] = gMonIcon_Castform,
    [SPECIES_CLAMPERL] = gMonIcon_Castform,
    [SPECIES_HUNTAIL] = gMonIcon_Castform,
    [SPECIES_GOREBYSS] = gMonIcon_Castform,
    [SPECIES_ABSOL] = gMonIcon_Castform,
    [SPECIES_SHUPPET] = gMonIcon_Castform,
    [SPECIES_BANETTE] = gMonIcon_Castform,
    [SPECIES_SEVIPER] = gMonIcon_Castform,
    [SPECIES_ZANGOOSE] = gMonIcon_Castform,
    [SPECIES_RELICANTH] = gMonIcon_Castform,
    [SPECIES_ARON] = gMonIcon_Castform,
    [SPECIES_LAIRON] = gMonIcon_Castform,
    [SPECIES_AGGRON] = gMonIcon_Castform,
    [SPECIES_CASTFORM] = gMonIcon_Castform,
    [SPECIES_VOLBEAT] = gMonIcon_Castform,
    [SPECIES_ILLUMISE] = gMonIcon_Castform,
    [SPECIES_LILEEP] = gMonIcon_Castform,
    [SPECIES_CRADILY] = gMonIcon_Castform,
    [SPECIES_ANORITH] = gMonIcon_Castform,
    [SPECIES_ARMALDO] = gMonIcon_Castform,
    [SPECIES_RALTS] = gMonIcon_Castform,
    [SPECIES_KIRLIA] = gMonIcon_Castform,
    [SPECIES_GARDEVOIR] = gMonIcon_Castform,
    [SPECIES_BAGON] = gMonIcon_Castform,
    [SPECIES_SHELGON] = gMonIcon_Castform,
    [SPECIES_SALAMENCE] = gMonIcon_Castform,
    [SPECIES_BELDUM] = gMonIcon_Castform,
    [SPECIES_METANG] = gMonIcon_Castform,
    [SPECIES_METAGROSS] = gMonIcon_Castform,
    [SPECIES_REGIROCK] = gMonIcon_Castform,
    [SPECIES_REGICE] = gMonIcon_Castform,
    [SPECIES_REGISTEEL] = gMonIcon_Castform,
    [SPECIES_KYOGRE] = gMonIcon_Castform,
    [SPECIES_GROUDON] = gMonIcon_Castform,
    [SPECIES_RAYQUAZA] = gMonIcon_Castform,
    [SPECIES_LATIAS] = gMonIcon_Castform,
    [SPECIES_LATIOS] = gMonIcon_Castform,
    [SPECIES_JIRACHI] = gMonIcon_Castform,
    [SPECIES_DEOXYS] = gMonIcon_Castform,
    [SPECIES_CHIMECHO] = gMonIcon_Castform,
    [SPECIES_EGG] = gMonIcon_Castform,
    [SPECIES_UNOWN_B] = gMonIcon_Castform,
    [SPECIES_UNOWN_C] = gMonIcon_Castform,
    [SPECIES_UNOWN_D] = gMonIcon_Castform,
    [SPECIES_UNOWN_E] = gMonIcon_Castform,
    [SPECIES_UNOWN_F] = gMonIcon_Castform,
    [SPECIES_UNOWN_G] = gMonIcon_Castform,
    [SPECIES_UNOWN_H] = gMonIcon_Castform,
    [SPECIES_UNOWN_I] = gMonIcon_Castform,
    [SPECIES_UNOWN_J] = gMonIcon_Castform,
    [SPECIES_UNOWN_K] = gMonIcon_Castform,
    [SPECIES_UNOWN_L] = gMonIcon_Castform,
    [SPECIES_UNOWN_M] = gMonIcon_Castform,
    [SPECIES_UNOWN_N] = gMonIcon_Castform,
    [SPECIES_UNOWN_O] = gMonIcon_Castform,
    [SPECIES_UNOWN_P] = gMonIcon_Castform,
    [SPECIES_UNOWN_Q] = gMonIcon_Castform,
    [SPECIES_UNOWN_R] = gMonIcon_Castform,
    [SPECIES_UNOWN_S] = gMonIcon_Castform,
    [SPECIES_UNOWN_T] = gMonIcon_Castform,
    [SPECIES_UNOWN_U] = gMonIcon_Castform,
    [SPECIES_UNOWN_V] = gMonIcon_Castform,
    [SPECIES_UNOWN_W] = gMonIcon_Castform,
    [SPECIES_UNOWN_X] = gMonIcon_Castform,
    [SPECIES_UNOWN_Y] = gMonIcon_Castform,
    [SPECIES_UNOWN_Z] = gMonIcon_Castform,
    [SPECIES_UNOWN_EMARK] = gMonIcon_Castform,
    [SPECIES_UNOWN_QMARK] = gMonIcon_Castform,
};

const u8 gMonIconPaletteIndices[] =
{
    [SPECIES_NONE] = 0,
    [SPECIES_BULBASAUR] = 1,
    [SPECIES_IVYSAUR] = 1,
    [SPECIES_VENUSAUR] = 1,
    [SPECIES_CHARMANDER] = 0,
    [SPECIES_CHARMELEON] = 0,
    [SPECIES_CHARIZARD] = 0,
    [SPECIES_SQUIRTLE] = 0,
    [SPECIES_WARTORTLE] = 2,
    [SPECIES_BLASTOISE] = 2,
    [SPECIES_CATERPIE] = 1,
    [SPECIES_METAPOD] = 1,
    [SPECIES_BUTTERFREE] = 0,
    [SPECIES_WEEDLE] = 1,
    [SPECIES_KAKUNA] = 2,
    [SPECIES_BEEDRILL] = 2,
    [SPECIES_PIDGEY] = 0,
    [SPECIES_PIDGEOTTO] = 0,
    [SPECIES_PIDGEOT] = 0,
    [SPECIES_RATTATA] = 2,
    [SPECIES_RATICATE] = 1,
    [SPECIES_SPEAROW] = 0,
    [SPECIES_FEAROW] = 0,
    [SPECIES_EKANS] = 2,
    [SPECIES_ARBOK] = 2,
    [SPECIES_PIKACHU] = 2,
    [SPECIES_RAICHU] = 0,
    [SPECIES_SANDSHREW] = 2,
    [SPECIES_SANDSLASH] = 2,
    [SPECIES_NIDORAN_F] = 2,
    [SPECIES_NIDORINA] = 2,
    [SPECIES_NIDOQUEEN] = 2,
    [SPECIES_NIDORAN_M] = 2,
    [SPECIES_NIDORINO] = 2,
    [SPECIES_NIDOKING] = 2,
    [SPECIES_CLEFAIRY] = 0,
    [SPECIES_CLEFABLE] = 0,
    [SPECIES_VULPIX] = 2,
    [SPECIES_NINETALES] = 1,
    [SPECIES_JIGGLYPUFF] = 0,
    [SPECIES_WIGGLYTUFF] = 0,
    [SPECIES_ZUBAT] = 2,
    [SPECIES_GOLBAT] = 2,
    [SPECIES_ODDISH] = 1,
    [SPECIES_GLOOM] = 0,
    [SPECIES_VILEPLUME] = 0,
    [SPECIES_PARAS] = 0,
    [SPECIES_PARASECT] = 0,
    [SPECIES_VENONAT] = 0,
    [SPECIES_VENOMOTH] = 2,
    [SPECIES_DIGLETT] = 2,
    [SPECIES_DUGTRIO] = 2,
    [SPECIES_MEOWTH] = 1,
    [SPECIES_PERSIAN] = 1,
    [SPECIES_PSYDUCK] = 1,
    [SPECIES_GOLDUCK] = 2,
    [SPECIES_MANKEY] = 1,
    [SPECIES_PRIMEAPE] = 2,
    [SPECIES_GROWLITHE] = 0,
    [SPECIES_ARCANINE] = 0,
    [SPECIES_POLIWAG] = 0,
    [SPECIES_POLIWHIRL] = 0,
    [SPECIES_POLIWRATH] = 0,
    [SPECIES_ABRA] = 2,
    [SPECIES_KADABRA] = 2,
    [SPECIES_ALAKAZAM] = 2,
    [SPECIES_MACHOP] = 0,
    [SPECIES_MACHOKE] = 2,
    [SPECIES_MACHAMP] = 0,
    [SPECIES_BELLSPROUT] = 1,
    [SPECIES_WEEPINBELL] = 1,
    [SPECIES_VICTREEBEL] = 1,
    [SPECIES_TENTACOOL] = 2,
    [SPECIES_TENTACRUEL] = 2,
    [SPECIES_GEODUDE] = 1,
    [SPECIES_GRAVELER] = 1,
    [SPECIES_GOLEM] = 1,
    [SPECIES_PONYTA] = 0,
    [SPECIES_RAPIDASH] = 0,
    [SPECIES_SLOWPOKE] = 0,
    [SPECIES_SLOWBRO] = 0,
    [SPECIES_MAGNEMITE] = 0,
    [SPECIES_MAGNETON] = 0,
    [SPECIES_FARFETCHD] = 1,
    [SPECIES_DODUO] = 2,
    [SPECIES_DODRIO] = 2,
    [SPECIES_SEEL] = 2,
    [SPECIES_DEWGONG] = 2,
    [SPECIES_GRIMER] = 2,
    [SPECIES_MUK] = 2,
    [SPECIES_SHELLDER] = 2,
    [SPECIES_CLOYSTER] = 2,
    [SPECIES_GASTLY] = 2,
    [SPECIES_HAUNTER] = 2,
    [SPECIES_GENGAR] = 2,
    [SPECIES_ONIX] = 2,
    [SPECIES_DROWZEE] = 2,
    [SPECIES_HYPNO] = 1,
    [SPECIES_KRABBY] = 2,
    [SPECIES_KINGLER] = 2,
    [SPECIES_VOLTORB] = 0,
    [SPECIES_ELECTRODE] = 0,
    [SPECIES_EXEGGCUTE] = 0,
    [SPECIES_EXEGGUTOR] = 1,
    [SPECIES_CUBONE] = 1,
    [SPECIES_MAROWAK] = 1,
    [SPECIES_HITMONLEE] = 2,
    [SPECIES_HITMONCHAN] = 2,
    [SPECIES_LICKITUNG] = 1,
    [SPECIES_KOFFING] = 2,
    [SPECIES_WEEZING] = 2,
    [SPECIES_RHYHORN] = 1,
    [SPECIES_RHYDON] = 1,
    [SPECIES_CHANSEY] = 0,
    [SPECIES_TANGELA] = 0,
    [SPECIES_KANGASKHAN] = 1,
    [SPECIES_HORSEA] = 0,
    [SPECIES_SEADRA] = 0,
    [SPECIES_GOLDEEN] = 0,
    [SPECIES_SEAKING] = 0,
    [SPECIES_STARYU] = 2,
    [SPECIES_STARMIE] = 2,
    [SPECIES_MR_MIME] = 0,
    [SPECIES_SCYTHER] = 1,
    [SPECIES_JYNX] = 2,
    [SPECIES_ELECTABUZZ] = 1,
    [SPECIES_MAGMAR] = 0,
    [SPECIES_PINSIR] = 2,
    [SPECIES_TAUROS] = 2,
    [SPECIES_MAGIKARP] = 0,
    [SPECIES_GYARADOS] = 0,
    [SPECIES_LAPRAS] = 2,
    [SPECIES_DITTO] = 2,
    [SPECIES_EEVEE] = 2,
    [SPECIES_VAPOREON] = 0,
    [SPECIES_JOLTEON] = 0,
    [SPECIES_FLAREON] = 0,
    [SPECIES_PORYGON] = 0,
    [SPECIES_OMANYTE] = 0,
    [SPECIES_OMASTAR] = 0,
    [SPECIES_KABUTO] = 2,
    [SPECIES_KABUTOPS] = 2,
    [SPECIES_AERODACTYL] = 0,
    [SPECIES_SNORLAX] = 1,
    [SPECIES_ARTICUNO] = 0,
    [SPECIES_ZAPDOS] = 0,
    [SPECIES_MOLTRES] = 0,
    [SPECIES_DRATINI] = 0,
    [SPECIES_DRAGONAIR] = 0,
    [SPECIES_DRAGONITE] = 2,
    [SPECIES_MEWTWO] = 2,
    [SPECIES_MEW] = 0,
    [SPECIES_CHIKORITA] = 1,
    [SPECIES_BAYLEEF] = 1,
    [SPECIES_MEGANIUM] = 1,
    [SPECIES_CYNDAQUIL] = 1,
    [SPECIES_QUILAVA] = 1,
    [SPECIES_TYPHLOSION] = 1,
    [SPECIES_TOTODILE] = 2,
    [SPECIES_CROCONAW] = 2,
    [SPECIES_FERALIGATR] = 2,
    [SPECIES_SENTRET] = 2,
    [SPECIES_FURRET] = 2,
    [SPECIES_HOOTHOOT] = 2,
    [SPECIES_NOCTOWL] = 2,
    [SPECIES_LEDYBA] = 0,
    [SPECIES_LEDIAN] = 0,
    [SPECIES_SPINARAK] = 1,
    [SPECIES_ARIADOS] = 0,
    [SPECIES_CROBAT] = 2,
    [SPECIES_CHINCHOU] = 2,
    [SPECIES_LANTURN] = 0,
    [SPECIES_PICHU] = 0,
    [SPECIES_CLEFFA] = 0,
    [SPECIES_IGGLYBUFF] = 1,
    [SPECIES_TOGEPI] = 2,
    [SPECIES_TOGETIC] = 2,
    [SPECIES_NATU] = 0,
    [SPECIES_XATU] = 0,
    [SPECIES_MAREEP] = 2,
    [SPECIES_FLAAFFY] = 0,
    [SPECIES_AMPHAROS] = 0,
    [SPECIES_BELLOSSOM] = 1,
    [SPECIES_MARILL] = 2,
    [SPECIES_AZUMARILL] = 2,
    [SPECIES_SUDOWOODO] = 1,
    [SPECIES_POLITOED] = 1,
    [SPECIES_HOPPIP] = 1,
    [SPECIES_SKIPLOOM] = 1,
    [SPECIES_JUMPLUFF] = 2,
    [SPECIES_AIPOM] = 2,
    [SPECIES_SUNKERN] = 1,
    [SPECIES_SUNFLORA] = 1,
    [SPECIES_YANMA] = 1,
    [SPECIES_WOOPER] = 0,
    [SPECIES_QUAGSIRE] = 0,
    [SPECIES_ESPEON] = 2,
    [SPECIES_UMBREON] = 2,
    [SPECIES_MURKROW] = 2,
    [SPECIES_SLOWKING] = 0,
    [SPECIES_MISDREAVUS] = 0,
    [SPECIES_UNOWN] = 0,
    [SPECIES_WOBBUFFET] = 0,
    [SPECIES_GIRAFARIG] = 1,
    [SPECIES_PINECO] = 0,
    [SPECIES_FORRETRESS] = 2,
    [SPECIES_DUNSPARCE] = 2,
    [SPECIES_GLIGAR] = 2,
    [SPECIES_STEELIX] = 0,
    [SPECIES_SNUBBULL] = 0,
    [SPECIES_GRANBULL] = 2,
    [SPECIES_QWILFISH] = 0,
    [SPECIES_SCIZOR] = 0,
    [SPECIES_SHUCKLE] = 1,
    [SPECIES_HERACROSS] = 2,
    [SPECIES_SNEASEL] = 0,
    [SPECIES_TEDDIURSA] = 0,
    [SPECIES_URSARING] = 2,
    [SPECIES_SLUGMA] = 0,
    [SPECIES_MAGCARGO] = 0,
    [SPECIES_SWINUB] = 2,
    [SPECIES_PILOSWINE] = 2,
    [SPECIES_CORSOLA] = 0,
    [SPECIES_REMORAID] = 0,
    [SPECIES_OCTILLERY] = 0,
    [SPECIES_DELIBIRD] = 0,
    [SPECIES_MANTINE] = 2,
    [SPECIES_SKARMORY] = 0,
    [SPECIES_HOUNDOUR] = 0,
    [SPECIES_HOUNDOOM] = 0,
    [SPECIES_KINGDRA] = 0,
    [SPECIES_PHANPY] = 0,
    [SPECIES_DONPHAN] = 0,
    [SPECIES_PORYGON2] = 0,
    [SPECIES_STANTLER] = 2,
    [SPECIES_SMEARGLE] = 1,
    [SPECIES_TYROGUE] = 2,
    [SPECIES_HITMONTOP] = 2,
    [SPECIES_SMOOCHUM] = 1,
    [SPECIES_ELEKID] = 1,
    [SPECIES_MAGBY] = 1,
    [SPECIES_MILTANK] = 1,
    [SPECIES_BLISSEY] = 1,
    [SPECIES_RAIKOU] = 0,
    [SPECIES_ENTEI] = 2,
    [SPECIES_SUICUNE] = 0,
    [SPECIES_LARVITAR] = 1,
    [SPECIES_PUPITAR] = 0,
    [SPECIES_TYRANITAR] = 1,
    [SPECIES_LUGIA] = 0,
    [SPECIES_HO_OH] = 1,
    [SPECIES_CELEBI] = 1,
    [SPECIES_OLD_UNOWN_B] = 0,
    [SPECIES_OLD_UNOWN_C] = 0,
    [SPECIES_OLD_UNOWN_D] = 0,
    [SPECIES_OLD_UNOWN_E] = 0,
    [SPECIES_OLD_UNOWN_F] = 0,
    [SPECIES_OLD_UNOWN_G] = 0,
    [SPECIES_OLD_UNOWN_H] = 0,
    [SPECIES_OLD_UNOWN_I] = 0,
    [SPECIES_OLD_UNOWN_J] = 0,
    [SPECIES_OLD_UNOWN_K] = 0,
    [SPECIES_OLD_UNOWN_L] = 0,
    [SPECIES_OLD_UNOWN_M] = 0,
    [SPECIES_OLD_UNOWN_N] = 0,
    [SPECIES_OLD_UNOWN_O] = 0,
    [SPECIES_OLD_UNOWN_P] = 0,
    [SPECIES_OLD_UNOWN_Q] = 0,
    [SPECIES_OLD_UNOWN_R] = 0,
    [SPECIES_OLD_UNOWN_S] = 0,
    [SPECIES_OLD_UNOWN_T] = 0,
    [SPECIES_OLD_UNOWN_U] = 0,
    [SPECIES_OLD_UNOWN_V] = 0,
    [SPECIES_OLD_UNOWN_W] = 0,
    [SPECIES_OLD_UNOWN_X] = 0,
    [SPECIES_OLD_UNOWN_Y] = 0,
    [SPECIES_OLD_UNOWN_Z] = 0,
    [SPECIES_TREECKO] = 1,
    [SPECIES_GROVYLE] = 0,
    [SPECIES_SCEPTILE] = 1,
    [SPECIES_TORCHIC] = 0,
    [SPECIES_COMBUSKEN] = 0,
    [SPECIES_BLAZIKEN] = 0,
    [SPECIES_MUDKIP] = 0,
    [SPECIES_MARSHTOMP] = 0,
    [SPECIES_SWAMPERT] = 0,
    [SPECIES_POOCHYENA] = 2,
    [SPECIES_MIGHTYENA] = 2,
    [SPECIES_ZIGZAGOON] = 2,
    [SPECIES_LINOONE] = 2,
    [SPECIES_WURMPLE] = 0,
    [SPECIES_SILCOON] = 2,
    [SPECIES_BEAUTIFLY] = 0,
    [SPECIES_CASCOON] = 2,
    [SPECIES_DUSTOX] = 1,
    [SPECIES_LOTAD] = 1,
    [SPECIES_LOMBRE] = 1,
    [SPECIES_LUDICOLO] = 1,
    [SPECIES_SEEDOT] = 1,
    [SPECIES_NUZLEAF] = 1,
    [SPECIES_SHIFTRY] = 0,
    [SPECIES_NINCADA] = 1,
    [SPECIES_NINJASK] = 1,
    [SPECIES_SHEDINJA] = 1,
    [SPECIES_TAILLOW] = 2,
    [SPECIES_SWELLOW] = 2,
    [SPECIES_SHROOMISH] = 1,
    [SPECIES_BRELOOM] = 1,
    [SPECIES_SPINDA] = 1,
    [SPECIES_WINGULL] = 0,
    [SPECIES_PELIPPER] = 0,
    [SPECIES_SURSKIT] = 2,
    [SPECIES_MASQUERAIN] = 0,
    [SPECIES_WAILMER] = 2,
    [SPECIES_WAILORD] = 0,
    [SPECIES_SKITTY] = 0,
    [SPECIES_DELCATTY] = 2,
    [SPECIES_KECLEON] = 1,
    [SPECIES_BALTOY] = 1,
    [SPECIES_CLAYDOL] = 0,
    [SPECIES_NOSEPASS] = 0,
    [SPECIES_TORKOAL] = 1,
    [SPECIES_SABLEYE] = 2,
    [SPECIES_BARBOACH] = 0,
    [SPECIES_WHISCASH] = 0,
    [SPECIES_LUVDISC] = 0,
    [SPECIES_CORPHISH] = 0,
    [SPECIES_CRAWDAUNT] = 0,
    [SPECIES_FEEBAS] = 2,
    [SPECIES_MILOTIC] = 0,
    [SPECIES_CARVANHA] = 0,
    [SPECIES_SHARPEDO] = 0,
    [SPECIES_TRAPINCH] = 1,
    [SPECIES_VIBRAVA] = 1,
    [SPECIES_FLYGON] = 1,
    [SPECIES_MAKUHITA] = 2,
    [SPECIES_HARIYAMA] = 1,
    [SPECIES_ELECTRIKE] = 1,
    [SPECIES_MANECTRIC] = 0,
    [SPECIES_NUMEL] = 1,
    [SPECIES_CAMERUPT] = 0,
    [SPECIES_SPHEAL] = 2,
    [SPECIES_SEALEO] = 2,
    [SPECIES_WALREIN] = 0,
    [SPECIES_CACNEA] = 1,
    [SPECIES_CACTURNE] = 1,
    [SPECIES_SNORUNT] = 2,
    [SPECIES_GLALIE] = 0,
    [SPECIES_LUNATONE] = 1,
    [SPECIES_SOLROCK] = 0,
    [SPECIES_AZURILL] = 2,
    [SPECIES_SPOINK] = 0,
    [SPECIES_GRUMPIG] = 2,
    [SPECIES_PLUSLE] = 0,
    [SPECIES_MINUN] = 0,
    [SPECIES_MAWILE] = 2,
    [SPECIES_MEDITITE] = 0,
    [SPECIES_MEDICHAM] = 0,
    [SPECIES_SWABLU] = 0,
    [SPECIES_ALTARIA] = 0,
    [SPECIES_WYNAUT] = 0,
    [SPECIES_DUSKULL] = 0,
    [SPECIES_DUSCLOPS] = 0,
    [SPECIES_ROSELIA] = 0,
    [SPECIES_SLAKOTH] = 2,
    [SPECIES_VIGOROTH] = 2,
    [SPECIES_SLAKING] = 1,
    [SPECIES_GULPIN] = 1,
    [SPECIES_SWALOT] = 2,
    [SPECIES_TROPIUS] = 1,
    [SPECIES_WHISMUR] = 0,
    [SPECIES_LOUDRED] = 2,
    [SPECIES_EXPLOUD] = 2,
    [SPECIES_CLAMPERL] = 0,
    [SPECIES_HUNTAIL] = 0,
    [SPECIES_GOREBYSS] = 0,
    [SPECIES_ABSOL] = 0,
    [SPECIES_SHUPPET] = 0,
    [SPECIES_BANETTE] = 0,
    [SPECIES_SEVIPER] = 2,
    [SPECIES_ZANGOOSE] = 0,
    [SPECIES_RELICANTH] = 1,
    [SPECIES_ARON] = 2,
    [SPECIES_LAIRON] = 2,
    [SPECIES_AGGRON] = 2,
    [SPECIES_CASTFORM] = 0,
    [SPECIES_VOLBEAT] = 0,
    [SPECIES_ILLUMISE] = 2,
    [SPECIES_LILEEP] = 2,
    [SPECIES_CRADILY] = 0,
    [SPECIES_ANORITH] = 0,
    [SPECIES_ARMALDO] = 0,
    [SPECIES_RALTS] = 1,
    [SPECIES_KIRLIA] = 1,
    [SPECIES_GARDEVOIR] = 1,
    [SPECIES_BAGON] = 2,
    [SPECIES_SHELGON] = 2,
    [SPECIES_SALAMENCE] = 0,
    [SPECIES_BELDUM] = 0,
    [SPECIES_METANG] = 0,
    [SPECIES_METAGROSS] = 0,
    [SPECIES_REGIROCK] = 2,
    [SPECIES_REGICE] = 2,
    [SPECIES_REGISTEEL] = 2,
    [SPECIES_KYOGRE] = 2,
    [SPECIES_GROUDON] = 0,
    [SPECIES_RAYQUAZA] = 1,
    [SPECIES_LATIAS] = 0,
    [SPECIES_LATIOS] = 2,
    [SPECIES_JIRACHI] = 0,
    [SPECIES_DEOXYS] = 0,
    [SPECIES_CHIMECHO] = 0,
    [SPECIES_EGG] = 1,
    [SPECIES_UNOWN_B] = 0,
    [SPECIES_UNOWN_C] = 0,
    [SPECIES_UNOWN_D] = 0,
    [SPECIES_UNOWN_E] = 0,
    [SPECIES_UNOWN_F] = 0,
    [SPECIES_UNOWN_G] = 0,
    [SPECIES_UNOWN_H] = 0,
    [SPECIES_UNOWN_I] = 0,
    [SPECIES_UNOWN_J] = 0,
    [SPECIES_UNOWN_K] = 0,
    [SPECIES_UNOWN_L] = 0,
    [SPECIES_UNOWN_M] = 0,
    [SPECIES_UNOWN_N] = 0,
    [SPECIES_UNOWN_O] = 0,
    [SPECIES_UNOWN_P] = 0,
    [SPECIES_UNOWN_Q] = 0,
    [SPECIES_UNOWN_R] = 0,
    [SPECIES_UNOWN_S] = 0,
    [SPECIES_UNOWN_T] = 0,
    [SPECIES_UNOWN_U] = 0,
    [SPECIES_UNOWN_V] = 0,
    [SPECIES_UNOWN_W] = 0,
    [SPECIES_UNOWN_X] = 0,
    [SPECIES_UNOWN_Y] = 0,
    [SPECIES_UNOWN_Z] = 0,
    [SPECIES_UNOWN_EMARK] = 0,
    [SPECIES_UNOWN_QMARK] = 0,
};

const struct SpritePalette gMonIconPaletteTable[] =
{
    { gMonIconPalettes[0], POKE_ICON_BASE_PAL_TAG + 0 },
    { gMonIconPalettes[1], POKE_ICON_BASE_PAL_TAG + 1 },
    { gMonIconPalettes[2], POKE_ICON_BASE_PAL_TAG + 2 },

// There are only 3 actual palettes. The following are unused
// and don't point to valid data.
    { gMonIconPalettes[3], POKE_ICON_BASE_PAL_TAG + 3 },
    { gMonIconPalettes[4], POKE_ICON_BASE_PAL_TAG + 4 },
    { gMonIconPalettes[5], POKE_ICON_BASE_PAL_TAG + 5 },
};

const struct OamData sMonIconOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
};

// fastest to slowest

static const union AnimCmd sAnim_0[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(1, 6),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_1[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_2[] =
{
    ANIMCMD_FRAME(0, 14),
    ANIMCMD_FRAME(1, 14),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_3[] =
{
    ANIMCMD_FRAME(0, 22),
    ANIMCMD_FRAME(1, 22),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_4[] =
{
    ANIMCMD_FRAME(0, 29),
    ANIMCMD_FRAME(0, 29), // frame 0 is repeated
    ANIMCMD_JUMP(0),
};

const union AnimCmd *const sMonIconAnims[] =
{
    sAnim_0,
    sAnim_1,
    sAnim_2,
    sAnim_3,
    sAnim_4,
};

static const union AffineAnimCmd sAffineAnim_0[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 0, 10),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_1[] =
{
    AFFINEANIMCMD_FRAME(-2, -2, 0, 122),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const sMonIconAffineAnims[] =
{
    sAffineAnim_0,
    sAffineAnim_1,
};

const u16 sSpriteImageSizes[3][4] =
{
    [ST_OAM_SQUARE] = 
    {
        [SPRITE_SIZE(8x8)]   =  0x20,
        [SPRITE_SIZE(16x16)] =  0x80,
        [SPRITE_SIZE(32x32)] = 0x200,
        [SPRITE_SIZE(64x64)] = 0x800,
    },
    [ST_OAM_H_RECTANGLE] = 
    {
        [SPRITE_SIZE(16x8)]  =  0x40,
        [SPRITE_SIZE(32x8)]  =  0x80,
        [SPRITE_SIZE(32x16)] = 0x100,
        [SPRITE_SIZE(64x32)] = 0x400,
    },
    [ST_OAM_V_RECTANGLE] = 
    {
        [SPRITE_SIZE(8x16)]  =  0x40,
        [SPRITE_SIZE(8x32)]  =  0x80,
        [SPRITE_SIZE(16x32)] = 0x100,
        [SPRITE_SIZE(32x64)] = 0x400,
    },
};

u8 CreateMonIcon(u16 species, void (*callback)(struct Sprite *), s16 x, s16 y, u8 subpriority, u32 personality, bool32 handleDeoxys)
{
    u8 spriteId;
    struct MonIconSpriteTemplate iconTemplate =
    {
        .oam = &sMonIconOamData,
        .image = GetMonIconPtr(species, personality, handleDeoxys),
        .anims = sMonIconAnims,
        .affineAnims = sMonIconAffineAnims,
        .callback = callback,
        .paletteTag = POKE_ICON_BASE_PAL_TAG + gMonIconPaletteIndices[species],
    };

    if (species > NUM_SPECIES)
        iconTemplate.paletteTag = POKE_ICON_BASE_PAL_TAG;

    spriteId = CreateMonIconSprite(&iconTemplate, x, y, subpriority);

    UpdateMonIconFrame(&gSprites[spriteId]);

    return spriteId;
}

u8 CreateMonIconNoPersonality(u16 species, void (*callback)(struct Sprite *), s16 x, s16 y, u8 subpriority, bool32 handleDeoxys)
{
    u8 spriteId;
    struct MonIconSpriteTemplate iconTemplate =
    {
        .oam = &sMonIconOamData,
        .image = NULL,
        .anims = sMonIconAnims,
        .affineAnims = sMonIconAffineAnims,
        .callback = callback,
        .paletteTag = POKE_ICON_BASE_PAL_TAG + gMonIconPaletteIndices[species],
    };

    iconTemplate.image = GetMonIconTiles(species, handleDeoxys);
    spriteId = CreateMonIconSprite(&iconTemplate, x, y, subpriority);

    UpdateMonIconFrame(&gSprites[spriteId]);

    return spriteId;
}

u16 GetIconSpecies(u16 species, u32 personality)
{
    u16 result;

    if (species == SPECIES_UNOWN)
    {
        u16 letter = GetUnownLetterByPersonality(personality);
        if (letter == 0)
            letter = SPECIES_UNOWN;
        else
            letter += (SPECIES_UNOWN_B - 1);
        result = letter;
    }
    else
    {
        if (species > NUM_SPECIES)
            result = INVALID_ICON_SPECIES;
        else
            result = species;
    }

    return result;
}

u16 GetUnownLetterByPersonality(u32 personality)
{
    if (!personality)
        return 0;
    else
        return GET_UNOWN_LETTER(personality);
}

u16 GetIconSpeciesNoPersonality(u16 species)
{
    u16 value;

    if (MailSpeciesToSpecies(species, &value) == SPECIES_UNOWN)
    {
        if (value == 0)
            value += SPECIES_UNOWN;
        else
            value += (SPECIES_UNOWN_B - 1);
        return value;
    }
    else
    {
        if (species > (SPECIES_UNOWN_B - 1))
            species = INVALID_ICON_SPECIES;
        return GetIconSpecies(species, 0);
    }
}

const u8 *GetMonIconPtr(u16 species, u32 personality, bool32 handleDeoxys)
{
    return GetMonIconTiles(GetIconSpecies(species, personality), handleDeoxys);
}

void FreeAndDestroyMonIconSprite(struct Sprite *sprite)
{
    sub_80D328C(sprite);
}

void LoadMonIconPalettes(void)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(gMonIconPaletteTable); i++)
        LoadSpritePalette(&gMonIconPaletteTable[i]);
}

// unused
void SafeLoadMonIconPalette(u16 species)
{
    u8 palIndex;
    if (species > NUM_SPECIES)
        species = INVALID_ICON_SPECIES;
    palIndex = gMonIconPaletteIndices[species];
    if (IndexOfSpritePaletteTag(gMonIconPaletteTable[palIndex].tag) == 0xFF)
        LoadSpritePalette(&gMonIconPaletteTable[palIndex]);
}

void LoadMonIconPalette(u16 species)
{
    u8 palIndex = gMonIconPaletteIndices[species];
    if (IndexOfSpritePaletteTag(gMonIconPaletteTable[palIndex].tag) == 0xFF)
        LoadSpritePalette(&gMonIconPaletteTable[palIndex]);
}

void FreeMonIconPalettes(void)
{
    u8 i;
    for (i = 0; i < ARRAY_COUNT(gMonIconPaletteTable); i++)
        FreeSpritePaletteByTag(gMonIconPaletteTable[i].tag);
}

// unused
void SafeFreeMonIconPalette(u16 species)
{
    u8 palIndex;
    if (species > NUM_SPECIES)
        species = INVALID_ICON_SPECIES;
    palIndex = gMonIconPaletteIndices[species];
    FreeSpritePaletteByTag(gMonIconPaletteTable[palIndex].tag);
}

void FreeMonIconPalette(u16 species)
{
    u8 palIndex;
    palIndex = gMonIconPaletteIndices[species];
    FreeSpritePaletteByTag(gMonIconPaletteTable[palIndex].tag);
}

void SpriteCB_MonIcon(struct Sprite *sprite)
{
    UpdateMonIconFrame(sprite);
}

const u8* GetMonIconTiles(u16 species, bool32 handleDeoxys)
{
    const u8* iconSprite = gMonIconTable[species];
    if (species == SPECIES_DEOXYS && handleDeoxys == TRUE)
    {
        iconSprite = (const u8*)(0x400 + (u32)iconSprite); // use the specific Deoxys form icon (Speed in this case)
    }
    return iconSprite;
}

void sub_80D304C(u16 offset)
{
    s32 i;
    const struct SpritePalette* monIconPalettePtr;

    if (offset <= 0xA0)
    {
        monIconPalettePtr = gMonIconPaletteTable;
        for(i = 5; i >= 0; i--)
        {
            LoadPalette(monIconPalettePtr->data, offset, 0x20);
            offset += 0x10;
            monIconPalettePtr++;
        }
    }
}

u8 GetValidMonIconPalIndex(u16 species)
{
    if (species > NUM_SPECIES)
        species = INVALID_ICON_SPECIES;
    return gMonIconPaletteIndices[species];
}

u8 GetMonIconPaletteIndexFromSpecies(u16 species)
{
    return gMonIconPaletteIndices[species];
}

const u16* GetValidMonIconPalettePtr(u16 species)
{
    if (species > NUM_SPECIES)
        species = INVALID_ICON_SPECIES;
    return gMonIconPaletteTable[gMonIconPaletteIndices[species]].data;
}

u8 UpdateMonIconFrame(struct Sprite *sprite)
{
    u8 result = 0;

    if (sprite->animDelayCounter == 0)
    {
        s16 frame = sprite->anims[sprite->animNum][sprite->animCmdIndex].frame.imageValue;

        switch (frame)
        {
        case -1:
            break;
        case -2:
            sprite->animCmdIndex = 0;
            break;
        default:
            RequestSpriteCopy(
                // pointer arithmetic is needed to get the correct pointer to perform the sprite copy on.
                // because sprite->images is a struct def, it has to be casted to (u8 *) before any
                // arithmetic can be performed.
                (u8 *)sprite->images + (sSpriteImageSizes[sprite->oam.shape][sprite->oam.size] * frame),
                (u8 *)(OBJ_VRAM0 + sprite->oam.tileNum * TILE_SIZE_4BPP),
                sSpriteImageSizes[sprite->oam.shape][sprite->oam.size]);
            sprite->animDelayCounter = sprite->anims[sprite->animNum][sprite->animCmdIndex].frame.duration & 0xFF;
            sprite->animCmdIndex++;
            result = sprite->animCmdIndex;
            break;
        }
    }
    else
    {
        sprite->animDelayCounter--;
    }
    return result;
}

static u8 CreateMonIconSprite(struct MonIconSpriteTemplate *iconTemplate, s16 x, s16 y, u8 subpriority)
{
    u8 spriteId;

    struct SpriteFrameImage image = { NULL, sSpriteImageSizes[iconTemplate->oam->shape][iconTemplate->oam->size] };

    struct SpriteTemplate spriteTemplate =
    {
        .tileTag = 0xFFFF,
        .paletteTag = iconTemplate->paletteTag,
        .oam = iconTemplate->oam,
        .anims = iconTemplate->anims,
        .images = &image,
        .affineAnims = iconTemplate->affineAnims,
        .callback = iconTemplate->callback,
    };

    spriteId = CreateSprite(&spriteTemplate, x, y, subpriority);
    gSprites[spriteId].animPaused = TRUE;
    gSprites[spriteId].animBeginning = FALSE;
    gSprites[spriteId].images = (const struct SpriteFrameImage *)iconTemplate->image;
    return spriteId;
}

void sub_80D328C(struct Sprite *sprite)
{
    struct SpriteFrameImage image = { NULL, sSpriteImageSizes[sprite->oam.shape][sprite->oam.size] };
    sprite->images = &image;
    DestroySprite(sprite);
}

void SetPartyHPBarSprite(struct Sprite *sprite, u8 animNum)
{
    sprite->animNum = animNum;
    sprite->animDelayCounter = 0;
    sprite->animCmdIndex = 0;
}
