/*
 * Filename: grid.c
 * Created: August 17th 2023
 * Author: diegomtassis
 */

#include "grid.h"

#include "gfx_grid.h"

#include "fwk/vdp_utils.h"

Map *gridMap;

void displayGrid() {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 idx_tile_bg_grid = idx_tile_malloc;
    u16 vram_idx = idx_tile_bg_grid;

    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    gridMap = MAP_create(&map_zona0_zona14, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    MAP_scrollTo(gridMap, 400, 0);
}

