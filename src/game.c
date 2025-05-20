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

GameResult GAME_run(const Config config[static 1]) {

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
    vramIdx = SCREEN_setupHud(vramIdx);

    // grid
    vramIdx = GRID_load(vramIdx, &map_zona_14);

    // objects
    initLevelObjects();

    // camera
    MapInfo *grid_map_info = map_info_zona_14[0];
    CAM_setup(grid_map_info->width, grid_map_info->height);
    CAM_track(&lightCycle.movable.object);
    CAM_update();

    SYS_doVBlankProcess();
    SPR_update();

    while (!game_over && !mission_accomplished) {
        if (!paused) {
            PLAYER_act();
            if (lightCycle.movable.spritePosJustChanged) {
                CAM_update();
                SPR_update();
            }

            game_over = lightCycle.health & DEAD;
        }

        SYS_doVBlankProcess();
    }

    return mission_accomplished;
}

static void initLevelObjects() {
    PAL_setPalette(PAL2, palette_cycles.data, DMA);
    PAL_setPalette(PAL3, palette_explosion.data, DMA);

    SPR_addSprite(&sprite_explosion, //
                  30, 5,             //
                  TILE_ATTR(PAL3, TRUE, FALSE, FALSE));

    PLAYER_init();
}
