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

    PAL_setPalette(PAL2, palette_hud.data, DMA_QUEUE);

    u16 idx_tile_bg_hud = vramBase;
    u16 vram_idx = idx_tile_bg_hud;

    VDP_loadTileSet(&tileset_hud_low, idx_tile_bg_hud, DMA_QUEUE);
    vram_idx += tileset_hud_low.numTile;

    VDP_setWindowVPos(TRUE, 20); // the last 8 rows

    VDP_setTileMapEx(WINDOW, &tilemap_hud_low, //
        TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, idx_tile_bg_hud), //
        0, 20, 0, 0, 40, 8, DMA_QUEUE);

    return vram_idx;
}
