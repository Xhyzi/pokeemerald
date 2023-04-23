.set LOCALID_DAYCARE_LADY, 1

Route117_EventScript_DaycareMan::
	lock
	faceplayer
	special GetDaycareMonNicknames
	specialvar VAR_RESULT, GetDaycareState
	goto_if_eq VAR_RESULT, DAYCARE_EGG_WAITING, Route117_EventScript_DaycareEggWaiting
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, Route117_EventScript_CheckOnOneMon
	goto_if_eq VAR_RESULT, DAYCARE_TWO_MONS, Route117_EventScript_CheckOnTwoMons
	msgbox Route117_Text_SeeWifeIfYoudLikeMeToRaiseMon, MSGBOX_DEFAULT
	release
	end

Route117_EventScript_DaycareEggWaiting::
	msgbox Route117_Text_DoYouWantEgg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_EventScript_DaycareAcceptEgg
	msgbox Route117_Text_IWillKeepDoYouWantIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_EventScript_DaycareAcceptEgg
	msgbox Route117_Text_IllKeepIt, MSGBOX_DEFAULT
	clearflag FLAG_PENDING_DAYCARE_EGG
	special RejectEggFromDayCare
	release
	end

Route117_EventScript_DaycareAcceptEgg::
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	goto_if_ne VAR_RESULT, PARTY_SIZE, Route117_EventScript_DaycareReceiveEgg
	msgbox Route117_Text_YouHaveNoRoomForIt, MSGBOX_DEFAULT
	release
	end

Route117_EventScript_DaycareReceiveEgg::
	message Route117_Text_ReceivedEgg
	playfanfare MUS_LEVEL_UP
	waitfanfare
	waitbuttonpress
	msgbox Route117_Text_TakeGoodCareOfIt, MSGBOX_DEFAULT
	special GiveEggFromDaycare
	clearflag FLAG_PENDING_DAYCARE_EGG
	release
	end

Route117_EventScript_CheckMonReceivedMail::
	specialvar VAR_RESULT, CheckDaycareMonReceivedMail
	call_if_eq VAR_RESULT, 1, Route117_EventScript_MonReceivedMail
	return

Route117_EventScript_MonReceivedMail::
	msgbox Route117_Text_FriendlyWithOtherTrainersMon, MSGBOX_DEFAULT
	return

Route117_EventScript_CheckOnOneMon::
	special GetDaycareMonNicknames
	msgbox Route117_Text_YourMonIsDoingFine, MSGBOX_DEFAULT
	setvar VAR_0x8004, 0
	call Route117_EventScript_CheckMonReceivedMail
	release
	end

Route117_EventScript_CheckOnTwoMons::
	special GetDaycareMonNicknames
	msgbox Route117_Text_YourMonsAreDoingFine, MSGBOX_DEFAULT
	special SetDaycareCompatibilityString
	special ShowFieldMessageStringVar4
	waitmessage
	waitbuttonpress
	setvar VAR_0x8004, 0
	call Route117_EventScript_CheckMonReceivedMail
	setvar VAR_0x8004, 1
	call Route117_EventScript_CheckMonReceivedMail
	release
	end

Route117_PokemonDayCare_EventScript_DaycareWoman::
	lock
	faceplayer
	specialvar VAR_RESULT, GetDaycareState
	goto_if_eq VAR_RESULT, DAYCARE_EGG_WAITING, Route117_PokemonDayCare_EventScript_EggWaiting
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, Route117_PokemonDayCare_EventScript_OneMonInDaycare
	goto_if_eq VAR_RESULT, DAYCARE_TWO_MONS, Route117_PokemonDayCare_EventScript_TwoMonsInDaycare
	msgbox Route117_PokemonDayCare_Text_WouldYouLikeUsToRaiseAMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_GiveMonToRaise
	msgbox Route117_PokemonDayCare_Text_FineThenComeAgain, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_GiveMonToRaise::
	specialvar VAR_RESULT, CountPartyNonEggMons
	goto_if_eq VAR_RESULT, 1, Route117_PokemonDayCare_EventScript_OnlyOneMon
	specialvar VAR_RESULT, CountPartyAliveNonEggMons
	goto_if_eq VAR_RESULT, 2, Route117_PokemonDayCare_EventScript_OnlyTwoAliveMons
	msgbox Route117_PokemonDayCare_Text_WhichMonShouldWeRaise, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special ChooseSendDaycareMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, Route117_PokemonDayCare_EventScript_ComeAgain
	specialvar VAR_RESULT, CountPartyAliveNonEggMons_IgnoreVar0x8004Slot
	goto_if_eq VAR_RESULT, 0, Route117_PokemonDayCare_EventScript_OnlyOneAliveMon
	specialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies
	waitse
	playmoncry VAR_0x8005, CRY_MODE_NORMAL
	msgbox Route117_PokemonDayCare_Text_WellRaiseYourMon, MSGBOX_DEFAULT
	waitmoncry
	special StoreSelectedPokemonInDaycare
	incrementgamestat GAME_STAT_USED_DAYCARE
	specialvar VAR_RESULT, GetDaycareState
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, Route117_PokemonDayCare_EventScript_CanRaiseOneMore
	release
	end

Route117_PokemonDayCare_EventScript_ComeAgain::
	msgbox Route117_PokemonDayCare_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_CanRaiseOneMore::
	msgbox Route117_PokemonDayCare_Text_WeCanRaiseOneMore, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_GiveMonToRaise
	goto Route117_PokemonDayCare_EventScript_ComeAgain
	end

Route117_PokemonDayCare_EventScript_OnlyOneMon::
	msgbox Route117_PokemonDayCare_Text_YouHaveJustOneMon, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_OnlyOneAliveMon::
	msgbox Route117_PokemonDayCare_Text_WhatWillYouBattleWith, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_OnlyTwoAliveMons::
	msgbox Route117_PokemonDayCare_Text_YoullBeLeftWithJustOne, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_EggWaiting::
	msgbox Route117_PokemonDayCare_Text_HusbandWasLookingForYou, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_YourMonHasGrownXLevels::
	msgbox Route117_PokemonDayCare_Text_YourMonHasGrownXLevels, MSGBOX_DEFAULT
	return

Route117_PokemonDayCare_EventScript_DisplayLevelsGained::
	specialvar VAR_RESULT, GetNumLevelsGainedFromDaycare
	call_if_ne VAR_RESULT, 0, Route117_PokemonDayCare_EventScript_YourMonHasGrownXLevels
	return

Route117_PokemonDayCare_EventScript_OneMonInDaycare::
	msgbox Route117_PokemonDayCare_Text_GoodToSeeYou, MSGBOX_DEFAULT
	setvar VAR_0x8004, 0
	call Route117_PokemonDayCare_EventScript_DisplayLevelsGained
	msgbox Route117_PokemonDayCare_Text_WeCanRaiseOneMore, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_GiveMonToRaise
	msgbox Route117_PokemonDayCare_Text_TakeYourMonBack, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_TryRetrieveMon
	goto Route117_PokemonDayCare_EventScript_ComeAgain
	end

Route117_PokemonDayCare_EventScript_TryRetrieveMon::
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	goto_if_eq VAR_RESULT, PARTY_SIZE, Route117_PokemonDayCare_EventScript_NoRoom
	specialvar VAR_RESULT, GetDaycareState
	setvar VAR_0x8004, 0
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, Route117_PokemonDayCare_EventScript_CostPrompt
	special ShowDaycareLevelMenu
	waitstate
	copyvar VAR_0x8004, VAR_RESULT
	goto_if_eq VAR_RESULT, DAYCARE_EXITED_LEVEL_MENU, Route117_PokemonDayCare_EventScript_ComeAgain
	goto Route117_PokemonDayCare_EventScript_CostPrompt
	end

Route117_PokemonDayCare_EventScript_CostPrompt::
	special GetDaycareCost
	msgbox Route117_PokemonDayCare_Text_ItWillCostX, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_CheckEnoughMoney
	goto Route117_PokemonDayCare_EventScript_ComeAgain
	end

Route117_PokemonDayCare_EventScript_CheckEnoughMoney::
	specialvar VAR_RESULT, IsEnoughForCostInVar0x8005
	goto_if_eq VAR_RESULT, 1, Route117_PokemonDayCare_EventScript_RetrieveMon
	msgbox Route117_PokemonDayCare_Text_NotEnoughMoney, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_EventScript_RetrieveMon::
	applymovement LOCALID_DAYCARE_LADY, Route117_PokemonDayCare_Movement_RetrieveDaycareMon
	waitmovement 0
	specialvar VAR_RESULT, TakePokemonFromDaycare
	special SubtractMoneyFromVar0x8005
	playse SE_SHOP
	msgbox Route117_PokemonDayCare_Text_HeresYourMon, MSGBOX_DEFAULT
	waitse
	playmoncry VAR_RESULT, CRY_MODE_NORMAL
	msgbox Route117_PokemonDayCare_Text_TookBackMon, MSGBOX_DEFAULT
	waitmoncry
	specialvar VAR_RESULT, GetDaycareState
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, Route117_PokemonDayCare_EventScript_AskRetrieveOtherMon
	goto Route117_PokemonDayCare_EventScript_ComeAgain
	end

Route117_PokemonDayCare_EventScript_AskRetrieveOtherMon::
	msgbox Route117_PokemonDayCare_Text_TakeOtherOneBackToo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_TryRetrieveMon
	goto Route117_PokemonDayCare_EventScript_ComeAgain
	end

Route117_PokemonDayCare_EventScript_NoRoom::
	msgbox Route117_PokemonDayCare_Text_YourTeamIsFull, MSGBOX_DEFAULT
	release
	end

Route117_PokemonDayCare_Movement_RetrieveDaycareMon:
	delay_16
	delay_16
	face_left
	delay_16
	delay_16
	face_right
	delay_16
	delay_16
	face_up
	walk_slow_up
	set_invisible
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	face_down
	set_visible
	walk_slow_down
	step_end

@ Unused. Possibly a commented script, or a typo end added to the above Movement script
Route117_PokemonDayCare_EventScript_UnusedEnd::
	end

Route117_PokemonDayCare_EventScript_TwoMonsInDaycare::
	msgbox Route117_PokemonDayCare_Text_GoodToSeeYou, MSGBOX_DEFAULT
	setvar VAR_0x8004, 0
	call Route117_PokemonDayCare_EventScript_DisplayLevelsGained
	setvar VAR_0x8004, 1
	call Route117_PokemonDayCare_EventScript_DisplayLevelsGained
	msgbox Route117_PokemonDayCare_Text_TakeYourMonBack, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route117_PokemonDayCare_EventScript_TryRetrieveMon
	msgbox Route117_PokemonDayCare_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

@ Unused
Route117_PokemonDayCare_EventScript_UnusedRetrieveMon::
	special ShowDaycareLevelMenu
	waitstate
	goto_if_eq VAR_RESULT, 2, Route117_PokemonDayCare_EventScript_ComeAgain
	copyvar VAR_0x8004, VAR_RESULT
	specialvar VAR_RESULT, TakePokemonFromDaycare
	msgbox Route117_PokemonDayCare_Text_HeresYourMon, MSGBOX_DEFAULT
	msgbox Route117_PokemonDayCare_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

EventScript_EggHatch::
	lockall
	msgbox Text_EggHatchHuh, MSGBOX_DEFAULT
	special EggHatch
	waitstate
	releaseall
	end

Route117_Text_SeeWifeIfYoudLikeMeToRaiseMon: @ 8291FCF
	.string "Soy el encargado de la Guardería.\p"
	.string "Ayudo a los entrenadores a cuidar\n"
	.string "de sus preciosos Pokémon.\p"
	.string "Si quieres que me encargue del tuyo,\n"
	.string "ve a hablar con mi mujer.$"

Route117_Text_DoYouWantEgg: @ 829205D
	.string "¡Ah, eres tú!\p"
	.string "¡Hemos cuidado de tu Pokémon y vaya\n"
	.string "sorpresa que nos hemos llevado!\p"
	.string "¡Tu Pokémon ha tenido un huevo!\p"
	.string "No sabemos cómo ha sucedido,\n"
	.string "pero es de tu Pokémon, seguro.\p"
	.string "¿Lo quieres?$"

Route117_Text_YourMonIsDoingFine: @ 8292114
	.string "¡Ah, eres tú! Me alegro de verte.\n"
	.string "Tu {STR_VAR_1} está estupendamente.$"

Route117_Text_IllKeepIt: @ 8292149
	.string "Muy bien, entonces me seguiré\n"
	.string "ocupando de él. ¡Gracias!$"

Route117_Text_YouHaveNoRoomForIt: @ 829216A
	.string "No tienes sitio para él…\n"
	.string "Vuelve cuando lo tengas.$"

Route117_Text_ReceivedEgg: @ 82921A4
	.string "{PLAYER} recibió el huevo del\n"
	.string "Encargado de la guardería.$"

Route117_Text_TakeGoodCareOfIt: @ 82921CF
	.string "Cuídalo bien.$"

Route117_Text_FriendlyWithOtherTrainersMon: @ 82921E5
	.string "Por cierto, con respecto a tu\n"
	.string "{STR_VAR_1}, parece que se ha hecho\l"
	.string "amigo del {STR_VAR_2} de {STR_VAR_3}.\p"
	.string "Es incluso posible que haya\n"
	.string "recibido una carta.$"

@ Unused
Route117_Text_SeeWifeIfYouWantToPickUpMon: @ 829225A
	.string "Si quieres recoger tu Pokémon,\n"
	.string "habla con mi mujer.$"

Route117_Text_YourMonsAreDoingFine: @ 8292299
	.string "¡Ah, eres tú! Tu {STR_VAR_1} y el\n"
	.string "{STR_VAR_2} están muy bien.$"

Route117_Text_IWillKeepDoYouWantIt: @ 82922C6
	.string "Yo me lo quedaría.\n"
	.string "Pero imagino que lo quieres.$"

Route117_PokemonDayCare_Text_WouldYouLikeUsToRaiseAMon: @ 82922F4
	.string "Soy la encargada de la guardería.\p"
	.string "Podemos ocuparnos de tus Pokémon.\p"
	.string "¿Quieres que cuidemos de alguno?$"

Route117_PokemonDayCare_Text_WhichMonShouldWeRaise: @ 8292349
	.string "¿Qué Pokémon quieres que cuidemos?$"

Route117_PokemonDayCare_Text_WellRaiseYourMon: @ 8292370
	.string "Estupendo, nos ocuparemos de tu\n"
	.string "{STR_VAR_1} cuanto sea necesario.\p"
	.string "Vuelve cuando quieras.$"

Route117_PokemonDayCare_Text_WeCanRaiseOneMore: @ 82923AF
	.string "Podemos cuidar a dos de tus Pokémon.\n"
	.string "¿Quieres que cuidemos de otro más?$"

Route117_PokemonDayCare_Text_HusbandWasLookingForYou: @ 82923F6
	.string "Mi marido te estaba buscando.$"

Route117_PokemonDayCare_Text_FineThenComeAgain: @ 8292416
	.string "Muy bien.\n"
	.string "Vuelve cuando quieras.$"

Route117_PokemonDayCare_Text_NotEnoughMoney: @ 8292432
	.string "No tienes bastante dinero.$"

Route117_PokemonDayCare_Text_TakeOtherOneBackToo: @ 829244F
	.string "¿Quieres llevarte el otro también?$"

Route117_PokemonDayCare_Text_ComeAgain: @ 8292476
	.string "Muy bien.\n"
	.string "Vuelve cuando quieras.$"

Route117_PokemonDayCare_Text_GoodToSeeYou: @ 8292488
	.string "¡Ah, eres tú! Me alegro de verte.\n"
	.string "Y tú te alegrarás de saber esto…$"

Route117_PokemonDayCare_Text_YourMonHasGrownXLevels: @ 82924CC
	.string "Tu {STR_VAR_1} ha crecido\n"
	.string "estos niveles: ¡{STR_VAR_2}!$"

Route117_PokemonDayCare_Text_YourTeamIsFull: @ 82924EF
	.string "Tu equipo Pokémon está lleno.\n"
	.string "Haz un hueco y vuelve a verme.$"

@ Unused
Route117_PokemonDayCare_Text_TakeBackWhichMon: @ 8292527
	.string "¿Qué Pokémon quieres recoger$"

Route117_PokemonDayCare_Text_ItWillCostX: @ 8292549
	.string "Si quieres llevarte a tu {STR_VAR_1},\n"
	.string "tendrás que pagar {STR_VAR_2}?.$"

Route117_PokemonDayCare_Text_HeresYourMon: @ 8292575
	.string "¡Perfecto!\n"
	.string "Aquí tienes a tu Pokémon.$"

Route117_PokemonDayCare_Text_TookBackMon: @ 8292593
	.string "{PLAYER} recogió a {STR_VAR_1} de\n"
	.string "la Guardería Pokémon.$"

Route117_PokemonDayCare_Text_YouHaveJustOneMon: @ 82925BB
	.string "Anda, pero si solo tienes un Pokémon…\p"
	.string "Vuelve cuando tengas alguno más.$"

Route117_PokemonDayCare_Text_TakeYourMonBack: @ 82925F6
	.string "¿Quieres recoger un Pokémon?$"

Route117_PokemonDayCare_Text_WhatWillYouBattleWith: @ 8292617
	.string "Si me dejas ese Pokémon,\n"
	.string "no podrás entablar combates.\p"
	.string "Vuelve en otra ocasión.$"

Text_EggHatchHuh: @ 8292668
	.string "¿Y esto?$"

Route117_PokemonDayCare_Text_YoullBeLeftWithJustOne: @ 829266D
	.string "¿Eh?\n"
	.string "A ver, a ver.\p"
	.string "Si me dejas ese Pokémon aquí, \n"
	.string "solo te quedarás con uno en forma.\p"
	.string "Será mejor que atrapes alguno más,\n"
	.string "vamos, digo yo.$"
