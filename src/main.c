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

//   resetTileMemory();

//   initConfig();

  // while (object != 0)
  //{

  //}

  //   initPrinter();
  //   turnPrinterOn();
  //   println("1");
  //   println(((TiledObject *)grid_objects[0])->name);
  //   println("2");
  //   println(((TiledObject *)grid_objects[1])->name);
  //   println("3");
  //   println(((TiledObject *)grid_objects[2])->name);
  //   println("4");
  //   println(((TiledObject *)grid_objects[3])->name);


VDP_drawText("Foo bar werjkthwkejh", 5, 5);

  VDP_drawText(((TiledObject *)grid_objects[0])->name, 1, 1);
  VDP_drawText(((TiledObject *)grid_objects[5])->name, 1, 2);
  VDP_drawText(((TiledObject *)grid_objects[2])->name, 1, 3);
  VDP_drawText(((TiledObject *)grid_objects[3])->name, 1, 4);
  VDP_drawText(((TiledObject *)grid_objects[4])->name, 1, 5);
  VDP_drawText(((TiledObject *)grid_objects[5])->name, 1, 6);
  VDP_drawText(((TiledObject *)grid_objects[6])->name, 1, 7);

  while (1) {
    // log_memory();
    //		setUpGame();

    //		GameResult result = runGame(&currentConfig);

    SYS_doVBlankProcess();
  }

  return 0;
}
