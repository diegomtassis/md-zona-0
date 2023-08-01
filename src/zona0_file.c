/*
 * zona0_file.c
 *
 *  Created on: Aug 01, 2023
 *      Author: diegomtassis
 */

#include <genesis.h>

#include "../inc/fwk/printer.h"

#define WAIT_MS_BEFORE 	1100
#define WAIT_MS_BETWEEN	750

void printDisclaimer() {

	VDP_setTextPalette(PAL2);

	turnPrinterOn();
	printerWait(WAIT_MS_BEFORE);

	print("Video Game Program: Zona 0 MD");
	printerWait(WAIT_MS_BETWEEN);

	println("");
	println("");
	println("Port by Diego Martin, 2023-2023");
	print("Work in progress");
	printerWait(WAIT_MS_BETWEEN);

	println("");
	println("");
	print("Zona 0 is a Topo Soft property");
	printerWait(WAIT_MS_BETWEEN);

	println("");
	println("");
	print("This information priority one");
	printerWait(WAIT_MS_BETWEEN);

	println("");
	println("");
	print("End of Line");
}

void clearDisclaimer() {

	turnPrinterOff();
}
