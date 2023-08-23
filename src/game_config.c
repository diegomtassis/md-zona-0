/*
 * game_config.c
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#include "game_config.h"

#include <genesis.h>

#include "fwk/commons.h"
#include "elements.h"

static void setUpDefaults();

Config currentConfig;

void initConfig() {

}

void setUpGame() {

	setUpDefaults();
	currentConfig.difficulty = NORMAL;
}

static void setUpDefaults() {

}
