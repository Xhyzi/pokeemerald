MysteryGiftScript_BattleCard::
	setvaddress MysteryGiftScript_BattleCard
	vgoto_if_set FLAG_MYSTERY_GIFT_DONE, MysteryGiftScript_BattleCardInfo
	setorcopyvar VAR_RESULT, GET_CARD_BATTLES_WON
	specialvar VAR_0x8008, GetMysteryGiftCardStat
	vgoto_if_ne VAR_0x8008, REQUIRED_CARD_BATTLES, MysteryGiftScript_BattleCardInfo
	lock
	faceplayer
	vmessage sText_MysteryGiftBattleCountCard_WonPrize
	waitmessage
	waitbuttonpress
	giveitem ITEM_POTION
	release
	setflag FLAG_MYSTERY_GIFT_DONE
	end

MysteryGiftScript_BattleCardInfo:
	lock
	faceplayer
	vmessage sText_MysteryGiftBattleCountCard
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftBattleCountCard:
	.string "Gracias por usar el sistema\n"
	.string "del regalo misterioso.\p"
	.string "Tu tarjeta de Recuento de\n"
	.string "Combates te muestra los récords\p"
	.string "de los combates mantenidos con\n"
	.string "Entrenadores que tienen la\l"
	.string "misma tarjeta.\p"
	.string "Busca entrenadores que\n"
	.string "tengan la misma tarjeta que tú.\p"
	.string "Cuando leas las noticias,\n"
	.string "podrás ver las clasificaciones.\p"
	.string "¡No te lo pierdas!$"

sText_MysteryGiftBattleCountCard_WonPrize:
	.string "Gracias por usar el sistema\n"
	.string "del regalo misterioso.\p"
	.string "¡Enhorabuena!\p"
	.string "¡Has ganado un premio por vencer\n"
	.string "a tu rival en tres combates!\p"
	.string "¡Esperamos que sigas luchando!$"
