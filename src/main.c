/*
 * main.c
 *
 *  Created on: Aug 01, 2023
 *      Author: diegomtassis
 */

#include <genesis.h>

#include "../inc/fwk/printer.h"
#include "../inc/fwk/vdp_utils.h"
#include "../inc/zona0_file.h"

//#include "../inc/fwk/logger.h"

#define LOADING_TIME	3000

int main(bool hard) {

	// default resolution
	VDP_setScreenWidth256();
	VDP_setScreenHeight224();

	initPrinter();

	if (hard) {

		// zona0 file
		VDP_setScreenWidth320();
		printDisclaimer();
		JOY_waitPress(JOY_1, BUTTON_BTN);
		clearDisclaimer();
		VDP_setScreenWidth256();

		waitMs(50);
	}

	resetTileMemory();

	while (1) {

		// log_memory();
		SYS_doVBlankProcess();
	}

	return 0;
}
