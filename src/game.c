/*
 * Filename: game.c
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#include "game.h"

#include <genesis.h>

#include "../res/grid.h"
#include "../res/sprites.h"

#include "../inc/config.h"

#include "../inc/fwk/commons.h"
#include "../inc/fwk/vdp_utils.h"


#define DEFAULT_FLASH_WAIT 2000

static Game* createGame(const Config config[static 1]);
static void releaseGame(Game *game);

static Game *current_game;

static void displayGrid();
static void displayFlynn();

GameResult runGame(const Config config[static 1]) {

    current_game = createGame(config);

    SPR_init();

    bool game_over = FALSE;

    displayGrid();
    displayFlynn();

    while (!game_over) {

        //	log_memory();
        SPR_update();
        SYS_doVBlankProcess();
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

static void displayFlynn() {

    PAL_setPalette(PAL1, palette_sprites.data, DMA);

    SPR_init();

    Sprite* p1_cycle = SPR_addSprite(&flynn_sprite, 64, 15,
        TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    SPR_setAnim(p1_cycle, 0);

    SPR_update();
}
