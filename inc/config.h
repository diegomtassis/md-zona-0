/*
 * config.h
 *
 *  Created on: Aug 8, 2023
 *      Author: diegomtassis
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <genesis.h>

#include "game.h"

typedef enum {
	EASY, //
	NORMAL, //
	HARD, //
	MANIAC,
} Difficulty;

void initConfig();
Config setUpGame();

#endif /* INC_CONFIG_H_ */
