/*
 * Filename: game.c
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#include "game.h"

#include <genesis.h>
#include "config.h"

#include "gfx_grid.h"
#include "gfx_lightcycles.h"

#include "grid.h"
#include "player.h"
#include "camera.h"

#include "fwk/commons.h"
#include "fwk/vdp_utils.h"

#define DEFAULT_FLASH_WAIT 2000

static void initGame(const Config config[static 1]);

static Game current_game;

volatile bool paused = FALSE;

/**
 * @brief run a level.
 *
 * @return goal accomplished
 */
static bool runLevel();

static void initLevelObjects();

GameResult runGame(const Config config[static 1]) {

    initGame(config);

    SPR_init();

    bool game_over = FALSE;

    while (!game_over) {

        game_over = !runLevel();
    }

    SPR_end();

    GameResult result = {
        //
        .p1_score = player.score,  //
    };

    return result;
}

static void initGame(const Config config[static 1]) {

    current_game.config = config;

    player.lives = config->lives;
    player.score = 0;
}

static bool runLevel() {

    bool game_over = FALSE;
    bool mission_accomplished = FALSE;

    displayGrid();

    setupCamera(400, 0, VDP_getScreenWidth(), VDP_getScreenHeight());

    initLevelObjects();
    SPR_update();

    while (!game_over && !mission_accomplished) {
        
        if (!paused) {
            playerActs();
            SPR_update();
            updateCamera();
        }

        VDP_showFPS(FALSE);
        VDP_showCPULoad();
        SYS_doVBlankProcess();
    }

    return game_over;
}

static void initLevelObjects() {

    PAL_setPalette(PAL1, palette_sprites.data, DMA);

    initPlayer();
}
