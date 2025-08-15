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

u16 ribbonsVramBaseTile;

static RibbonStepDefinition pendingRibbonStepDef;

u16 GRID_load(u16 vram_base, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG) {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 vram_idx = vram_base;

    u16 idx_tile_bg_grid = vram_idx;
    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    mapGridBG = MAP_create(mapDefinitionBG, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    u16 idx_tile_fg_grid = vram_idx;
    ribbonsVramBaseTile = idx_tile_fg_grid;
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

void GRID_addRibbonStep(RibbonStepDefinition *ribbonStepDef) { pendingRibbonStepDef = *ribbonStepDef; }

void GRID_updateRibbons() {

    RibbonStepDefinition *ribbonStepDef = &pendingRibbonStepDef;

    V2u16 cycleTilePos = SCREEN_posToTile(ribbonStepDef->mapPos);
    u8 ribbonDirection = ribbonStepDef->direction;

    VDPPlane plane = BG_A;

    u16 ribbonsVramBaseTile = ribbonStepDef->baseTile;

    if (ribbonStepDef->first) {
        if (ribbonDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 1),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 5),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 0), cycleTilePos.x,
                             cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 4), cycleTilePos.x,
                             cycleTilePos.y - 1);

        } else if (ribbonDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 3),
                             cycleTilePos.x - 1, cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 7),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);

        } else if (ribbonDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 2), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 6), cycleTilePos.x,
                             cycleTilePos.y);
        }
    } else {
        if (ribbonDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 0),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 4),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 1), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 5), cycleTilePos.x,
                             cycleTilePos.y);

        } else if (ribbonDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 2),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 6),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 3), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonsVramBaseTile + 7), cycleTilePos.x,
                             cycleTilePos.y);
        }
    }
}
