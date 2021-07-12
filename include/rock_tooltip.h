#ifndef GUARD_ROCK_TOOLTIP_H
#define GUARD_ROCK_TOOLTIP_H

// Tiempo que se mantiene el tooltip
#define TOOLTIP_WAIT_TIME       90  // Tiempo en frames. 60 frames -> 1 segundo
#define TOOLTIP_POS_X           244 // No cambiar
#define TOOLTIP_POS_Y           140 // Posición vertical del primer tooltip
#define TOOLTIP_X_MOVEMENT      34  // Cantidad de desplazamiento hacia la izquierda
#define TOOLTIP_SPEED           2   // Velocidad del desplazamiento
#define TOOLTIP_OFFSET_Y        20  // Distancia vertical entre cada tooltip
#define MAX_NUMBER_OF_TOOLTIPS  8   // Número máximo de tooltips simultáneos

// Cambialo por SE_ITEMFINDER -> es menos troll :)
#define TOOLTIP_SOUND_EFFECT SE_A      // Efecto de sonido al mostrar el tooltip

enum RockType {ROCK_KIMBERLITA, ROCK_DIORITA, ROCK_BASALTO, 
    ROCK_ROCA, ROCK_OBSIDIANA, ROCK_TOPACO};

void CreateRockTooltip(u8 rockId);

#endif /* GUARD_ROCK_TOOLTIP_H */
