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

static bool runGame();

static bool runLevel(u16 vramIdx);

static void initLevelObjects();
static void initCamera();
static void showReadyModal();

static void releaseLevelObjects();

GameResult GAME_run(const Config config[static 1]) {

    initGame(config);
    runGame();

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

static bool runGame() {

    u16 vramIdx = idx_tile_malloc;

    SPR_init();

    // hud
    vramIdx = SCREEN_setupHud(vramIdx);

    bool game_over = FALSE;
    while (!game_over) {
        runLevel(vramIdx);
    }

    SPR_end();
}

static bool runLevel(u16 vramIdx) {

    // grid
    vramIdx = GRID_load(vramIdx, &map_zona_14_BG, &map_zona_14_FG);

    initLevelObjects();
    initCamera();
    showReadyModal();

    bool mission_accomplished = FALSE;
    bool derezzed = FALSE;
    while (!mission_accomplished && !derezzed) {
        if (!paused) {
            PLAYER_act();

            if (lightCycle.movable.object.justBegunDerezzing) {
                CAM_still();
            }

            if (lightCycle.movable.object.viewIsDirty) {
                CAM_update();
            }

            SPR_update();

            derezzed = lightCycle.movable.object.health & DEREZZED;
        }

        SYS_doVBlankProcess();
    }

    releaseLevelObjects();
    GRID_release();

    return mission_accomplished;
}

static void initLevelObjects() {
    PAL_setPalette(PAL2, palette_cycles.data, DMA);
    PLAYER_init();
}

static void initCamera() {

    // camera
    MapInfo *grid_map_info = map_info_zona_14[0];
    CAM_setup(grid_map_info->width, grid_map_info->height);
    CAM_track(&lightCycle.movable.object);
    CAM_update();

    SYS_doVBlankProcess();
    SPR_update();
};

static void showReadyModal() {

    SCREEN_showReady();
    SPR_update();
    JOY_waitPress(JOY_1, BUTTON_ALL);
    SCREEN_clearMessage();
    SYS_doVBlankProcess();
    SPR_update();
};

static void releaseLevelObjects() { PLAYER_release(); }
