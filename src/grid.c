/*
 * Filename: grid.c
 * Created: August 17th 2023
 * Author: diegomtassis
 */

#include "grid.h"

#include "fwk/vdp_utils.h"
#include "gfx_grid.h"

Map *mapGridBG;
Map *mapGridFG;

u16 GRID_load(u16 vram_base, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG) {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 vram_idx = vram_base;

    u16 idx_tile_bg_grid = vram_idx;
    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    mapGridBG = MAP_create(mapDefinitionBG, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    u16 idx_tile_fg_grid = vram_idx;
    VDP_loadTileSet(&tileset_trails, idx_tile_fg_grid, DMA);
    vram_idx += tileset_trails.numTile;
    mapGridFG = MAP_create(mapDefinitionFG, BG_A, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, idx_tile_fg_grid));

    return vram_idx;
}

void GRID_scroll(V2s16 point, bool redraw) {
    MAP_scrollToEx(mapGridBG, point.x, point.y, redraw);
    MAP_scrollToEx(mapGridFG, point.x, point.y, redraw);
}

void GRID_release() {
    MEM_free(mapGridBG);
    MEM_free(mapGridFG);
}
