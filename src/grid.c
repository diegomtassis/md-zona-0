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

u16 trailsVramBaseTile;

static TrailSegmentDefinition pendingTrailSegmentDef;

u16 GRID_load(u16 vram_base, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG) {

    PAL_setPalette(PAL0, palette_grid.data, DMA);

    u16 vram_idx = vram_base;

    u16 idx_tile_bg_grid = vram_idx;
    VDP_loadTileSet(&tileset_grid, idx_tile_bg_grid, DMA);
    vram_idx += tileset_grid.numTile;

    mapGridBG = MAP_create(mapDefinitionBG, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, idx_tile_bg_grid));

    u16 idx_tile_fg_grid = vram_idx;
    trailsVramBaseTile = idx_tile_fg_grid;
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

void GRID_addSegment(TrailSegmentDefinition *trailSegmentDef) { pendingTrailSegmentDef = *trailSegmentDef; }

void GRID_updateSegments() {

    TrailSegmentDefinition *trailSegmentDef = &pendingTrailSegmentDef;

    V2u16 cycleTilePos = SCREEN_posToTile(trailSegmentDef->mapPos);
    u8 trailDirection = trailSegmentDef->direction;

    VDPPlane plane = BG_A;

    u16 trailsVramBaseTile = trailSegmentDef->baseTile;

    if (trailSegmentDef->first) {
        if (trailDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 1),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 5),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (trailDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 0), cycleTilePos.x,
                             cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 4), cycleTilePos.x,
                             cycleTilePos.y - 1);

        } else if (trailDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 3),
                             cycleTilePos.x - 1, cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 7),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);

        } else if (trailDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 2), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 6), cycleTilePos.x,
                             cycleTilePos.y);
        }
    } else {
        if (trailDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 0),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 4),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (trailDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 1), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 5), cycleTilePos.x,
                             cycleTilePos.y);

        } else if (trailDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 2),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 6),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (trailDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 3), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 7), cycleTilePos.x,
                             cycleTilePos.y);
        }
    }
}
