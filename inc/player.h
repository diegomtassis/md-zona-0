/*
 * Filename: player.h
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#ifndef INC_PLAYER_H_
#define INC_PLAYER_H_

#include "lightcycle.h"

extern Player player;

extern LightCycle lightCycle;

void PLAYER_init();
void PLAYER_act();
void PLAYER_release();

#endif /* INC_PLAYER_H_ */
