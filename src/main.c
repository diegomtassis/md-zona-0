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
#include "../res/grid.h"

 //#include "../inc/fwk/logger.h"

#define LOADING_TIME	3000

static void displayGrid();

int main(bool hard) {

	// default resolution
	VDP_setScreenWidth320();
	VDP_setScreenHeight224();

	initPrinter();

	if (hard) {

		// zona0 file
		printDisclaimer();
		JOY_waitPress(JOY_1, BUTTON_BTN);
		clearDisclaimer();

		waitMs(50);
	}

	resetTileMemory();

	displayGrid();

	while (1) {

		// log_memory();
		SYS_doVBlankProcess();
	}

	return 0;
}



static void displayGrid() {

	PAL_setPalette(PAL2, palette_grid.data, DMA);

	u16 idx_tile_bg_grid = idx_tile_malloc;
	u16 vram_idx = idx_tile_bg_grid;

	VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
	vram_idx += tileset_grid.numTile;

	Map *bg;
	bg = MAP_create(&map_zona0_zona14, BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, idx_tile_bg_grid));

	MAP_scrollTo(bg, 4, 4);
}

