Roulette_EventScript_Table1::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_GameCorner_EventScript_NoCoinCase
	setvar VAR_0x8004, 0
	getpokenewsactive POKENEWS_GAME_CORNER
	goto_if_eq VAR_RESULT, FALSE, Roulette_EventScript_Play
	addvar VAR_0x8004, ROULETTE_SPECIAL_RATE
	goto Roulette_EventScript_Play
	end

Roulette_EventScript_Table2::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_GameCorner_EventScript_NoCoinCase
	setvar VAR_0x8004, 1
	getpokenewsactive POKENEWS_GAME_CORNER
	goto_if_eq VAR_RESULT, FALSE, Roulette_EventScript_Play
	addvar VAR_0x8004, ROULETTE_SPECIAL_RATE
	goto Roulette_EventScript_Play
	end

Roulette_EventScript_Play::
	special PlayRoulette
	waitstate
	end

Roulette_Text_PlayMinimumWagerIsX::
	.string "La apuesta mínima en esta mesa\n"
	.string "es {STR_VAR_1}. ¿Quieres jugar?$"

Roulette_Text_NotEnoughCoins::
	.string "No tienes bastantes fichas.$"

Roulette_Text_SpecialRateTable::
	.string "¡Hora de la bonificación especial!$"

Roulette_Text_ControlsInstruction::
	.string "Coloca tu apuesta con el + panel\n"
	.string "de control y pulsa el Botón A.$"

Roulette_Text_ItsAHit::
	.string "¡Premio!$"

Roulette_Text_Jackpot::
	.string "¡Premio gordo!$"

Roulette_Text_NothingDoing::
	.string "¡Gana la casa!$"

Roulette_Text_YouveWonXCoins::
	.string "¡Has ganado {STR_VAR_1} fichas!$"

Roulette_Text_NoCoinsLeft::
	.string "No tienes bastantes fichas.$"

Roulette_Text_KeepPlaying::
	.string "¿Quieres seguir jugando?$"

Roulette_Text_BoardWillBeCleared::
	.string "Se va a limpiar la mesa de la ruleta.$"

@ Unused
Roulette_Text_YouDontHaveACoinCase::
	.string "No tienes un monedero$"

Roulette_Text_CoinCaseIsFull::
	.string "¡Tu Monedero está lleno!\n"
	.string "Cambia tus fichas por premios.$"
