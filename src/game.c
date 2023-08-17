/*
 * Filename: game.c
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#include "game.h"

#include <genesis.h>
#include "config.h"

#include "gfx_grid.h"
#include "gfx_lightcycles.h""

#include "player.h"


#include "fwk/commons.h"
#include "fwk/vdp_utils.h"

#define DEFAULT_FLASH_WAIT 2000

static Game* createGame(const Config config[static 1]);
static void releaseGame(Game *game);

static Game *current_game;

volatile bool paused = FALSE;

/**
 * @brief run a grid.
 *
 * @return goal accomplished
 */
static bool runGrid();

static void displayGrid();

GameResult runGame(const Config config[static 1]) {

    current_game = createGame(config);

    SPR_init();

    bool game_over = FALSE;

    while (!game_over) {

        game_over = !runGrid();
    }

    SPR_end();

    GameResult result = {
        //
        .p1_score = current_game->p1->score,  //
    };

    releaseGame(current_game);
    current_game = 0;

    return result;
}

static Game *createGame(const Config config[static 1]) {
    Game *game = MEM_calloc(sizeof * game);

    game->config = config;

    game->p1 = MEM_calloc(sizeof(*game->p1));
    game->p1->lives = config->lives;
    game->p1->score = 0;

    return game;
}

static void releaseGame(Game *game) {
    if (!game) {
        return;
    }

    if (game->p1) {
        MEM_free(game->p1);
        game->p1 = 0;
    }

    MEM_free(game);
}

static bool runGrid() {
    bool game_over = FALSE;
    bool mission_accomplished = FALSE;

    displayGrid();
    initPlayer();

    while (!game_over && !mission_accomplished) {
        if (!paused) {
            playerActs();
            SPR_update();
        }

        VDP_showFPS(FALSE);
        VDP_showCPULoad();
        SYS_doVBlankProcess();
    }

    return game_over;
}

static void displayGrid() {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 idx_tile_bg_grid = idx_tile_malloc;
    u16 vram_idx = idx_tile_bg_grid;

    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    Map *bg;
    bg = MAP_create(&map_zona0_zona14, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    MAP_scrollTo(bg, 400, 0);
}
