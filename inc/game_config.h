/*
 * game_config.h
 *
 *  Created on: Aug 8, 2023
 *      Author: diegomtassis
 */

#ifndef INC_GAMECONFIG_H_
#define INC_GAMECONFIG_H_

#include <genesis.h>

#include "game.h"

typedef enum {
	EASY, //
	NORMAL, //
	HARD, //
	MANIAC,
} Difficulty;

void initConfig();
void setUpGame();

extern Config currentConfig;

#endif /* INC_GAMECONFIG_H_ */
