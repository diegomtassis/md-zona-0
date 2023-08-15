/*
 * config.c
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#include "../inc/config.h"

#include <genesis.h>

#include "fwk/commons.h"
#include "elements.h"

static void setUpDefaults();

static Config* current_config;

void initConfig() {

	// whatever is the current config is trash, no need to free
	current_config = 0;
}

Config setUpGame() {

	setUpDefaults();

	if (!current_config) {
		current_config = MEM_calloc(sizeof(*current_config));
		current_config->difficulty = NORMAL;
	}

	return *current_config;
}

static void setUpDefaults() {

}
