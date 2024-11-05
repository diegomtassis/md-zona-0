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

  initConfig();

  while (1) {
    // log_memory();
    setUpGame();

    GameResult result = runGame(&currentConfig);

    SYS_doVBlankProcess();
  }

  return 0;
}
