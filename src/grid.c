/*
 * Filename: grid.c
 * Created: August 17th 2023
 * Author: diegomtassis
 */

#include "grid.h"

#include "fwk/vdp_utils.h"
#include "gfx_grid.h"
#include "ribbons.h"

#include "grid_physics.h"
#include "screen.h"

Map *mapGridBG;
Map *mapGridFG;

static VDPPlane BG_RIBBONS = BG_A;

static Ribbon *ribbons[10];
static u8 ribbonsCount = 0;

// Mapping from RibbonType to base tile offset
static u16 ribbonTypeToBaseTile[3] = {0}; // Will be initialized in GRID_load

static RibbonStep pendingRibbonSteps[10];
static u8 pendingRibbonStepsCount = 0;

static void drawRibbonStep(RibbonStep *ribbonStep);

u16 GRID_load(u16 vram_base, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG) {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    
    u16 vram_idx = vram_base;
    
    // BG
    u16 idx_tile_bg_grid = vram_idx;
    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;
    
    mapGridBG = MAP_create(mapDefinitionBG, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));
    
    // FG
    u16 idx_tile_fg_grid = vram_idx;
    RIBBONS_init(idx_tile_fg_grid);
    vram_idx += tileset_ribbons.numTile;
    mapGridFG = MAP_create(mapDefinitionFG, BG_A, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, idx_tile_fg_grid));

    // Initialize ribbon type to base tile mapping
    // For now, all ribbon types use the same base tile
    ribbonTypeToBaseTile[JETWALL_RED] = idx_tile_fg_grid; 
    ribbonTypeToBaseTile[JETWALL_GREEN] = idx_tile_fg_grid; 
    ribbonTypeToBaseTile[JETWALL_FLASH] = idx_tile_fg_grid;

    // Initialize ribbons array to NULL pointers
    for (u8 i = 0; i < 10; i++) {
        ribbons[i] = NULL;
    }
    ribbonsCount = 0;

    return vram_idx;
}

void GRID_scroll(V2s16 point, bool redraw) {
    MAP_scrollToEx(mapGridBG, point.x, point.y, redraw);
    MAP_scrollToEx(mapGridFG, point.x, point.y, redraw);
}

void GRID_release() {
    // Clear all ribbon pointers
    for (u8 i = 0; i < 10; i++) {
        ribbons[i] = NULL;
    }
    ribbonsCount = 0;

    MEM_free(mapGridBG);
    MEM_free(mapGridFG);
}

void GRID_trackRibbonStep(RibbonStep *ribbonStep) { 
    if (pendingRibbonStepsCount < 10) {
        pendingRibbonSteps[pendingRibbonStepsCount] = *ribbonStep;
        pendingRibbonStepsCount++;
    }
}

void GRID_renderNewRibbonsSteps() {

    for (u8 i = 0; i < pendingRibbonStepsCount; i++) {
        RibbonStep *ribbonStep = &pendingRibbonSteps[i];
        drawRibbonStep(ribbonStep);
    }

    pendingRibbonStepsCount = 0;
}

void GRID_trackRibbon(Ribbon *ribbon) {

    if (!ribbon) {
        return;
    }

    // Skip ribbon if already tracked
    for (u8 i = 0; i < ribbonsCount; i++) {
        if (ribbons[i] == ribbon) {
            return;
        }
    }

    // Find an available slot in the ribbons array
    for (u8 i = 0; i < 10; i++) {
        if (ribbons[i] == NULL) {
            // Store the pointer and set baseTile based on ribbon's type
            ribbons[i] = ribbon;
            ribbon->baseTile = ribbonTypeToBaseTile[ribbon->type];
            if (i >= ribbonsCount) {
                ribbonsCount++;
            }
            return;
        }
    }
}

void GRID_removeRibbon(Ribbon *ribbon) {
    if (!ribbon) {
        return;
    }

    // Find and remove the ribbon from the array
    for (u8 i = 0; i < ribbonsCount; i++) {
        if (ribbons[i] == ribbon) {
            ribbons[i] = NULL;
            return;
        }
    }
}

static void drawRibbonStep(RibbonStep *ribbonStep) {

   if (!ribbonStep || !ribbonStep->segment) {
        return;
    }

    V2u16 cycleTilePos = SCREEN_posToTile(ribbonStep->mapPos);
    u8 ribbonDirection = ribbonStep->segment->direction;
    u16 ribbonVramBaseTile = ribbonStep->segment->ribbon->baseTile;

    if (ribbonStep->first) {
        if (ribbonDirection & DOWN) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 1),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 5),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & UP) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 0),
                             cycleTilePos.x, cycleTilePos.y - 2);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 4),
                             cycleTilePos.x, cycleTilePos.y - 1);

        } else if (ribbonDirection & LEFT) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 3),
                             cycleTilePos.x - 1, cycleTilePos.y - 2);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 7),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);

        } else if (ribbonDirection & RIGHT) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 2),
                             cycleTilePos.x, cycleTilePos.y - 1);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 6),
                             cycleTilePos.x, cycleTilePos.y);
        }
    } else {
        if (ribbonDirection & DOWN) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 0),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 4),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & UP) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 1),
                             cycleTilePos.x, cycleTilePos.y - 1);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 5),
                             cycleTilePos.x, cycleTilePos.y);

        } else if (ribbonDirection & LEFT) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 2),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 6),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (ribbonDirection & RIGHT) {
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 3),
                             cycleTilePos.x, cycleTilePos.y - 1);
            VDP_setTileMapXY(BG_RIBBONS, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ribbonVramBaseTile + 7),
                             cycleTilePos.x, cycleTilePos.y);
        }
    }
}
