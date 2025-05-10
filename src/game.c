/*
 * Filename: game.c
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#include "game.h"

#include <genesis.h>

#include "camera.h"
#include "config.h"
#include "fwk/commons.h"
#include "fwk/vdp_utils.h"
#include "gfx_grid.h"
#include "gfx_lightcycles.h"
#include "grid.h"
#include "player.h"
#include "screen.h"
#include "tiled.h"

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
        .p1_score = player.score, //
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

    u16 vramIdx = idx_tile_malloc;

    // hud
    vramIdx = setupHud(vramIdx);

    // grid
    vramIdx = loadGrid(vramIdx, &map_zona0_zona14);
    
    // objects
    initLevelObjects();
    
    // camera
    MapInfo *grid_map_info = map_info[0];
    setupCamera(grid_map_info->width, grid_map_info->height);
    cameraFocusOn(&lightCycle.movable.object);
    updateCamera();

    SPR_update();

    while (!game_over && !mission_accomplished) {
        if (!paused) {
            playerActs();
            if (lightCycle.movable.updateSprite) {
                updateCamera();
                SPR_update();
                lightCycle.movable.updateSprite = FALSE;
            }
        }

        SYS_doVBlankProcess();
    }

    return game_over;
}

static void initLevelObjects() {
    PAL_setPalette(PAL1, palette_sprites.data, DMA);

    initPlayer();
}
