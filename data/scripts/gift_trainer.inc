MysteryGiftScript_VisitingTrainer::
	setvaddress MysteryGiftScript_VisitingTrainer
	special ValidateEReaderTrainer
	vgoto_if_eq VAR_RESULT, 0, MysteryGiftScript_VisitingTrainerArrived
	lock
	faceplayer
	vmessage sText_MysteryGiftVisitingTrainerInstructions
	waitmessage
	waitbuttonpress
	release
	end

MysteryGiftScript_VisitingTrainerArrived:
	lock
	faceplayer
	vmessage sText_MysteryGiftVisitingTrainerArrived
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftVisitingTrainerInstructions:
	.string "Gracias por usar el sistema\n"
	.string "del regalo misterioso.\p"
	.string "Al llevar la tarjeta misteriosa,\n"
	.string "podrás participar en una encuesta\l"
	.string "en la tienda Pokémon.\p"
	.string "Aprovecha las encuestas para\n"
	.string "invitar a entrenadores\l"
	.string "a Arrecípolis.\p"
	.string "Voy a darte una contraseña secreta\n"
	.string "para la encuesta:\p"
	.string "“Quiero entrenadores\n"
	.string "muy fuertes”\p"
	.string "Escríbela en un cuestionario y\n"
	.string "envíala al sistema de comunicación\l"
	.string "inalámbrica.$"

sText_MysteryGiftVisitingTrainerArrived:
	.string "Gracias por usar el sistema\n"
	.string "del regalo misterioso.\p"
	.string "Ha llegado un Entrenador\n"
	.string "a Arrecípolis y te está buscando.\p"
	.string "Esperamos que disfrutes del\n"
	.string "combate con el entrenador.\p"
	.string "Si quieres, puedes invitar a más\n"
	.string "entrenadores usando otras\l"
	.string "contraseñas.\p"
	.string "Busca otras contraseñas\n"
	.string "que vayan bien.$"
