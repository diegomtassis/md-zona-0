/*
 * main.c
 * Created: Aug 01, 2023
 * Author: diegomtassis
 */

#include <genesis.h>

#include "../inc/fwk/printer.h"
#include "../inc/fwk/vdp_utils.h"


#include "elements.h"
#include "game.h"
#include "config.h"

#define LOADING_TIME	3000

int main(bool hard) {

	// default resolution
	VDP_setScreenWidth320();
	VDP_setScreenHeight224();

	// initPrinter();

	// if (hard) {

	// 	// zona0 file
	// 	printDisclaimer();
	// 	JOY_waitPress(JOY_1, BUTTON_BTN);
	// 	clearDisclaimer();

	// 	waitMs(50);
	// }

	resetTileMemory();

	initConfig();

	while (1) {

		// log_memory();
		Config config = setUpGame();

		GameResult result = runGame(&config);

		SYS_doVBlankProcess();
	}

	return 0;
}
