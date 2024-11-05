/*
 * main.c
 * Created: Aug 01, 2023
 * Author: diegomtassis
 */

#include <genesis.h>

#include "elements.h"
#include "fwk/printer.h"
#include "fwk/vdp_utils.h"
#include "game.h"
#include "game_config.h"
#include "gfx_grid.h"

static void logTiledObjects();

int main(bool hard) {

  // default resolution
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  initPrinter();

  // if (hard) {

  // 	// zona0 file
  // 	printDisclaimer();
  // 	JOY_waitPress(JOY_1, BUTTON_BTN);
  // 	clearDisclaimer();

  // 	waitMs(50);
  // }

  resetTileMemory();

  logTiledObjects();

  initConfig();

  while (1) {
    // log_memory();
    setUpGame();

    GameResult result = runGame(&currentConfig);

    SYS_doVBlankProcess();
  }

  return 0;
}


static void logTiledObjects() {

  // Grid objects
  TiledGridObject * gridObject;
  for (int i = 0; i < 4; i++) {

    gridObject = (TiledGridObject *)grid_objects[i];
    kprintf("%s - x:%d, y:%d, grid_x:%d, grid_y:%d", gridObject->name, gridObject->x, gridObject->y, gridObject->grid_x, gridObject->grid_y);
  }

  // Vehicles
  VehicleStartPosition* vehicle;
  for (int i = 0; i < 2; i++) {

    vehicle = (VehicleStartPosition *)vehicles_start[i];
    kprintf("%s - x:%d, y:%d, grid_x:%d, grid_y:%d, direction:%d", vehicle->name, vehicle->x, vehicle->y, vehicle->grid_x, vehicle->grid_y, vehicle->direction);
  }
}
