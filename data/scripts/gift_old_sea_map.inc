MysteryGiftScript_OldSeaMap::
	setvaddress MysteryGiftScript_OldSeaMap
	lock
	faceplayer
	vgoto_if_set FLAG_RECEIVED_OLD_SEA_MAP, OldSeaMap_Obtained
	vgoto_if_set FLAG_CAUGHT_MEW, OldSeaMap_Obtained
	checkitem ITEM_OLD_SEA_MAP
	vgoto_if_eq VAR_RESULT, TRUE, OldSeaMap_Obtained
	vmessage sText_MysteryGiftOldSeaMapForYou
	waitmessage
	waitbuttonpress
	checkitemspace ITEM_OLD_SEA_MAP
	vgoto_if_eq VAR_RESULT, FALSE, OldSeaMap_NoBagSpace
	giveitem ITEM_OLD_SEA_MAP
	setflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	setflag FLAG_RECEIVED_OLD_SEA_MAP
	vmessage sText_MysteryGiftOldSeaMapUseAtPort
	waitmessage
	waitbuttonpress
	release
	end

OldSeaMap_NoBagSpace:
	vmessage sText_MysteryGiftOldSeaMapBagFull
	waitmessage
	waitbuttonpress
	release
	end

OldSeaMap_Obtained:
	vmessage sText_MysteryGiftOldSeaMapThankYou
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftOldSeaMapForYou:
	.string "Gracias por usar el sistema\n"
	.string "del regalo misterioso.\p"
	.string "Debes de ser {PLAYER}, ¿no?\p"
	.string "Pues tenemos un Mapa Viejo para ti.$"

sText_MysteryGiftOldSeaMapUseAtPort:
	.string "Parece que puedes usarlo en el\n"
	.string "puerto de ciudad Calagua.\p"
	.string "¿Por qué no curioseas un poco\n"
	.string "para ver de qué va la cosa?$"

sText_MysteryGiftOldSeaMapThankYou:
	.string "Gracias por usar el sistema\n"
	.string "del regalo misterioso.$"

sText_MysteryGiftOldSeaMapBagFull:
	.string "Lo siento, {PLAYER}. El bolsillo\n"
	.string "de los objetos clave está lleno.\p"
	.string "Guarda algo en el PC y vuelve\n"
	.string "más tarde.$"
