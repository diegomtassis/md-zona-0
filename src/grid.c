/*
 * Filename: grid.c
 * Created: August 17th 2023
 * Author: diegomtassis
 */

#include "grid.h"

#include "fwk/vdp_utils.h"
#include "gfx_grid.h"

#include "grid_physics.h"
#include "screen.h"

Map *mapGridBG;
Map *mapGridFG;

u16 ribbonVramBaseTile;

static RibbonStep pendingRibbonStep;

u16 GRID_load(u16 vram_base, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG) {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 vram_idx = vram_base;

    u16 idx_tile_bg_grid = vram_idx;
    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    mapGridBG = MAP_create(mapDefinitionBG, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    u16 idx_tile_fg_grid = vram_idx;
    ribbonVramBaseTile = idx_tile_fg_grid;
    VDP_loadTileSet(&tileset_ribbons, idx_tile_fg_grid, DMA);
    vram_idx += tileset_ribbons.numTile;
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

void GRID_addRibbonStep(RibbonStep *ribbonStep) { pendingRibbonStep = *ribbonStep; }

void GRID_updateRibbons() {

    RibbonStep *ribbonStep = &pendingRibbonStep;

    V2u16 cycleTilePos = SCREEN_posToTile(ribbonStep->mapPos);
    u8 ribbonDirection = ribbonStep->direction;

    VDPPlane plane = BG_A;

    u16 ribbonVramBaseTile = ribbonStep->baseTile;

    if (ribbonStep->first) {
        if (ribbonDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 1),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 5),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 0), cycleTilePos.x,
                             cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 4), cycleTilePos.x,
                             cycleTilePos.y - 1);

        } else if (ribbonDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 3),
                             cycleTilePos.x - 1, cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 7),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);

        } else if (ribbonDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 2), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 6), cycleTilePos.x,
                             cycleTilePos.y);
        }
    } else {
        if (ribbonDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 0),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 4),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 1), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 5), cycleTilePos.x,
                             cycleTilePos.y);

        } else if (ribbonDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 2),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 6),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 3), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 7), cycleTilePos.x,
                             cycleTilePos.y);
        }
    }
}
