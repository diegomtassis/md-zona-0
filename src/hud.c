/*
 * Filename: hud.c
 * Created: August 26th 2023
 * Author: diegomtassis
 */

#include "hud.h"

#include "gfx_hud.h"

#include "fwk/vdp_utils.h"

Map *hudMap;

u16 displayHud(u16 vramBase) {

    PAL_setPalette(PAL2, palette_hud.data, DMA);

    u16 idx_tile_bg_hud = vramBase;
    u16 vram_idx = idx_tile_bg_hud;

    VDP_loadTileSet(&tileset_hud, idx_tile_bg_hud, DMA);
    vram_idx += tileset_hud.numTile;

    hudMap = MAP_create(&map_hud, BG_A, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, idx_tile_bg_hud));

    MAP_scrollTo(hudMap, 0, 0);

    return vram_idx;
}
