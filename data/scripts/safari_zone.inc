SafariZone_EventScript_OutOfBallsMidBattle::
	setvar VAR_SAFARI_ZONE_STATE, 1
	special ExitSafariMode
	setwarp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
	end

SafariZone_EventScript_Exit::
	setvar VAR_SAFARI_ZONE_STATE, 1
	special ExitSafariMode
	warp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
	waitstate
	end

SafariZone_EventScript_RetirePrompt::
	lockall
	msgbox SafariZone_Text_WouldYouLikeToExit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_Retire
	releaseall
	end

SafariZone_EventScript_Retire::
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_TimesUp::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_TimesUp
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_OutOfBalls::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_OutOfBalls
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

EventScript_PokeBlockFeeder::
	lockall
	special GetPokeblockFeederInFront
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPresent
	msgbox SafariZone_Text_PlacePokeblockOnFeeder, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_ChoosePokeblock
	releaseall
	end

SafariZone_EventScript_ChoosePokeblock::
	fadescreen FADE_TO_BLACK
	special OpenPokeblockCaseOnFeeder
	waitstate
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPlaced
	end

SafariZone_EventScript_PokeblockPlaced::
	message SafariZone_Text_PokeblockWasPlaced
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_EventScript_PokeblockPresent::
	message SafariZone_Text_PokeblockStillHere
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_Text_WouldYouLikeToExit:
	.string "¿Quieres salir de la Zona Safari\n"
	.string "ahora mismo?$"

SafariZone_Text_TimesUp:
	.string "¡Ding, dong! ¡Se acabó el tiempo!\n"
	.string "Tu partida Safari ha terminado.$"

SafariZone_Text_OutOfBalls:
	.string "Te has quedado sin Safari Balls.\n"
	.string "Tu partida Safari ha terminado.$"

SafariZone_Text_PlacePokeblockOnFeeder:
	.string "¿Quieres colocar un Pokécubo\n"
	.string "en el comedero de Pokécubos?$"

SafariZone_Text_PokeblockStillHere:
	.string "El {STR_VAR_1} que colocaste\n"
	.string "antes sigue aquí.$"

SafariZone_Text_PokeblockWasPlaced:
	.string "Has colocado un {STR_VAR_1}\n"
	.string "en el comedero de Pokécubos.$"

Route121_SafariZoneEntrance_Text_WelcomeToSafariZone:
	.string "¡Hola! ¡Ésta es la Zona Safari!\p"
	.string "Aquí puedes ver muchos tipos\n"
	.string "de Pokémon poco comunes en Hoenn.\p"
	.string "Y además los verás en estado\n"
	.string "salvaje, en su hábitat natural.\p"
	.string "Lo que es más, hemos abierto nuestras\n"
	.string "puertas a los entrenadores Pokémon.\p"
	.string "¡Puedes atrapar los Pokémon\n"
	.string "que quieras y quedártelos para siempre!\p"
	.string "¡Pasa y disfruta de la Zona Safari!$"

Route121_SafariZoneEntrance_Text_WelcomeFirstTime:
	.string "¡Hola! ¡Ésta es la Zona Safari!\n"
	.string "¿Es la primera vez que vienes?$"

Route121_SafariZoneEntrance_Text_ComeInAndEnjoy:
	.string "¡Pasa y disfruta de la Zona Safari!$"

Route121_SafariZoneEntrance_Text_FirstTimeInfo:
	.string "Cuando entras en la Zona Safari,\n"
	.string "te dan 30 Safari Balls para\l"
	.string "atrapar Pokémon.\p"
	.string "La partida Safari se acaba cuando\n"
	.string "te quedas sin Safari Balls\l"
	.string "o has dado 500 pasos.\p"
	.string "¡Pasa y disfruta de la Zona Safari!$"

Route121_SafariZoneEntrance_Text_WouldYouLikeToPlay:
	.string "¡Hola! ¡Ésta es la Zona Safari!\p"
	.string "¡Atrapa todo lo que puedas por solo\n"
	.string "500¥! ¿Quieres jugar una partida\l"
	.string "Safari?$"

Route121_SafariZoneEntrance_Text_PlayAnotherTime:
	.string "No hay problema.\n"
	.string "Ven cuando quieras.$"

Route121_SafariZoneEntrance_Text_NotEnoughMoney:
	.string "Lo siento, no tienes bastante dinero.$"

Route121_SafariZoneEntrance_Text_ThatWillBe500Please:
	.string "Son 500¥, por favor.$"

Route121_SafariZoneEntrance_Text_HereAreYourSafariBalls:
	.string "Aquí tienes tus Safari Balls.$"

Route121_SafariZoneEntrance_Text_Received30SafariBalls:
	.string "{PLAYER} recibió 30 Safari Balls.$"

Route121_SafariZoneEntrance_Text_PleaseEnjoyYourself:
	.string "Te avisaremos cuando se termine\n"
	.string "la partida.\p"
	.string "Hasta entonces, pásatelo bien.\n"
	.string "¡La aventura ha comenzado!$"

Route121_SafariZoneEntrance_Text_PCIsFull:
	.string "Lo siento, pero todas las Cajas\n"
	.string "de tu PC están llenas.$"

Route121_SafariZoneEntrance_Text_YouNeedPokeblockCase:
	.string "Lo siento, pero parece que no tienes\n"
	.string "ningún tubo de Pokécubos.\p"
	.string "Tu partida Safari será mucho más\n"
	.string "fructífera si usas Pokécubos.\p"
	.string "Vuelve cuando tengas un tubo.\p"
	.string "Puedes conseguir uno en la sala\n"
	.string "de concursos de Calagua.$"

SafariZone_South_Text_StillHaveTimeExit:
	.string "Todavía te queda tiempo. ¿Quieres\n"
	.string "salir de la Zona Safari ahora mismo?$"

SafariZone_South_Text_EnjoyTheRestOfYourAdventure:
	.string "¡Sigue disfrutando de tu aventura!$"

SafariZone_South_Text_ExitEarlyThankYouForPlaying:
	.string "Muy bien.\p"
	.string "Me quedaré con las Safari Balls\n"
	.string "que te sobran.\p"
	.string "Gracias por jugar con nosotros.\n"
	.string "Vuelve siempre que quieras.$"

SafariZone_South_Text_GoodLuck:
	.string "¡Buena suerte!\p"
	.string "Si necesitas algo, lo que sea,\n"
	.string "avísame, ¿de acuerdo?$"

SafariZone_South_Text_Boy:
	.string "¿No lo sabías?\p"
	.string "Si pones un Pokécubo en ese cajón,\n"
	.string "atraerás a muchos Pokémon.$"

SafariZone_South_Text_Man:
	.string "Quiero seguir adentrándome,\n"
	.string "pero he olvidado mi Bici.\p"
	.string "Tengo la corazonada de que en\n"
	.string "las zonas más distantes viven\l"
	.string "Pokémon raros.$"

SafariZone_Southwest_Text_Woman:
	.string "A veces, cuando lanzo un Pokécubo\n"
	.string "a un Pokémon, no le hace ni caso.\p"
	.string "¿Los Pokémon también son\n"
	.string "tiquismiquis con la comida?$"

SafariZone_Northwest_Text_Man:
	.string "Uf… Uf…\n"
	.string "He… llegado hasta… aquí…\p"
	.string "Estoy agotado… No me quedan\n"
	.string "fuerzas para atrapar Pokémon…$"

SafariZone_North_Text_Fisherman:
	.string "Estoy en una misión de búsqueda\n"
	.string "de Pokémon raros de tipo agua.\p"
	.string "¿Sabes por dónde queda el lago?$"

SafariZone_North_Text_Man:
	.string "Voy a atrapar un montón de Pokémon\n"
	.string "raros para cambiarlos con mis amigos.$"

SafariZone_South_Text_Youngster:
	.string "He puesto un Pokécubo en el comedero\n"
	.string "de Pokécubos, pero ha desaparecido.\p"
	.string "Creo que algún Pokémon se lo habrá\n"
	.string "comido cuando no miraba.$"

Route121_SafariZoneEntrance_Text_TrainerTip:
	.string "Pista para entrenadores\n"
	.string "de la Zona Safari\p"
	.string "Lanza Pokécubos a los Pokémon salvajes\n"
	.string "para que no se escapen corriendo.$"

SafariZone_Southwest_Text_RestHouseSign:
	.string "Posada\n"
	.string "“Alivia tus cansados pies.”$"

SafariZone_RestHouse_Text_Youngster:
	.string "No tengo ningún Pokécubo, pero\n"
	.string "he atrapado bastantes Pokémon.\p"
	.string "Puedes mejorar tus posibilidades\n"
	.string "de atrapar Pokémon acercándote a ellos\l"
	.string "antes de lanzar una Safari Ball.$"

SafariZone_RestHouse_Text_PsychicM:
	.string "Si usas Pokécubos, los Pokémon\n"
	.string "salvajes no escaparán tan rápido.\p"
	.string "Pero no vale la pena dar Pokécubos a los\n"
	.string "Pokémon que son más mansos…$"

SafariZone_RestHouse_Text_FatMan:
	.string "Si pones un Pokécubo en el Comedero\n"
	.string "de Pokécubos, atraerás a Pokémon.\p"
	.string "Creo que a los Pokémon que comparten\n"
	.string "la misma naturaleza les gusta el mismo\l"
	.string "tipo de Pokécubo.$"

SafariZone_South_Text_AreaOffLimits1:
	.string "Aún se está construyendo esta zona.\n"
	.string "¡No te acerques, por favor!$"

SafariZone_Southeast_Text_ExpansionIsFinished:
	.string "Ya ha concluido el proyecto\n"
	.string "de ampliación de la Zona Safari.\p"
	.string "Espero que disfrutes de las nuevas\n"
	.string "instalaciones.$"

SafariZone_South_Text_AreaOffLimits2:
	.string "Aún se está construyendo esta zona.\n"
	.string "¡No te acerques, por favor!$"

SafariZone_Southeast_Text_LittleGirl:
	.string "¡Guau! ¡Eh! ¡Nunca había visto\n"
	.string "estos Pokémon antes!$"

SafariZone_Southeast_Text_FatMan:
	.string "Todos los Pokémon de esta zona\n"
	.string "son nuevos para mí.\p"
	.string "¡Y tengo permiso para atraparlos!\n"
	.string "¡Qué suerte!$"

SafariZone_Southeast_Text_RichBoy:
	.string "Los Pokémon que hay por aquí\n"
	.string "no parece que sean de Hoenn.$"

SafariZone_Northeast_Text_Boy:
	.string "Ya solo me quedan dos Safari Balls.\p"
	.string "Lo estoy pasando mal teniendo que\n"
	.string "decidir cuáles atrapo y cuáles no.$"

SafariZone_Northeast_Text_Woman:
	.string "He oído que hay Pikachu por aquí.\n"
	.string "¿Por dónde andarán?$"

SafariZone_Northeast_Text_Girl:
	.string "¡Bah!\n"
	.string "¡Oye, no pillo nada!\p"
	.string "¡Con lo que cuesta la entrada\n"
	.string "y yo no voy a atrapar ni uno!$"
