/*
 * main.c
 * Created: Aug 01, 2023
 * Author: diegomtassis
 */

#include <genesis.h>

#include "elements.h"
#include "fwk/printer.h"
#include "fwk/vdp_utils.h"
#include "zona0_file.h"
#include "game.h"
#include "game_config.h"
#include "gfx_grid.h"
#include "tiled.h"

static void logMarkers();

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

    logMarkers();

    initConfig();

    while (1) {
        setUpGame();

        GameResult result = GAME_run(&currentConfig);

        SYS_doVBlankProcess();
    }

    return 0;
}

static void logMarkers() {

    // grid info
    MapInfo *map_def = (MapInfo *)map_info_zona_14[0];
    kprintf("TILED: %s - width:%d, height:%d", map_def->name, map_def->width, map_def->height);

    // grid boundaries
    GridBoundaryMarker *gridMarker;
    for (int i = 0; i < 4; i++) {

        gridMarker = (GridBoundaryMarker *)grid_markers_zona_14[i];
        kprintf("TILED: %s - x:%d, y:%d, grid_x:%d, grid_y:%d", gridMarker->name, gridMarker->x, gridMarker->y,
                gridMarker->grid_x, gridMarker->grid_y);
    }

    // movables
    MovableInitMarker *movableMarker;
    for (int i = 0; i < 1; i++) {

        movableMarker = (MovableInitMarker *)movables_markers_zona_14[i];
        kprintf("TILED: %s - x:%d, y:%d, grid_x:%d, grid_y:%d, direction:%d", movableMarker->name, movableMarker->x,
                movableMarker->y, movableMarker->grid_x, movableMarker->grid_y, movableMarker->direction);
    }
}
