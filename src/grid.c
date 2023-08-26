/*
 * Filename: grid.c
 * Created: August 17th 2023
 * Author: diegomtassis
 */

#include "grid.h"

#include "camera.h"

#include "gfx_grid.h"

#include "fwk/vdp_utils.h"

#define INIT_OFFSET_X   400;
#define INIT_OFFSET_Y   0;

Map *gridMap;

u16 displayGrid(u16 vram_base, u32 x, u32 y) {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 idx_tile_bg_grid = vram_base;
    u16 vram_idx = idx_tile_bg_grid;

    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    gridMap = MAP_create(&map_zona0_zona14, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    MAP_scrollTo(gridMap, x, y);

    return vram_idx;
}

void scrollGrid() {

    MAP_scrollTo(gridMap, cameraView.min.x, cameraView.min.y);
}

