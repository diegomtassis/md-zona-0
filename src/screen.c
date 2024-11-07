/*
 * Filename: screen.c
 * Created: August 26th 2023
 * Author: diegomtassis
 */

#include "screen.h"

#include "gfx_hud.h"

#include "fwk/vdp_utils.h"

Map *hudMap;

Sprite *leftColumSprite;
Sprite *rightColumSprite;

Sprite *helmet1Sprite;
Sprite *helmet2Sprite;
Sprite *helmet3Sprite;
Sprite *helmet4Sprite;

u16 displayHud(u16 vramBase) {

    PAL_setPalette(PAL2, palette_hud.data, DMA_QUEUE);

    u16 idx_tile_bg_hud = vramBase;
    u16 vram_idx = idx_tile_bg_hud;

    VDP_loadTileSet(&tileset_hud_low, idx_tile_bg_hud, DMA_QUEUE);
    vram_idx += tileset_hud_low.numTile;

    VDP_setWindowVPos(TRUE, 20); // the last 8 rows

    VDP_setTileMapEx(
        WINDOW, &tilemap_hud_low,                                  //
        TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, idx_tile_bg_hud), //
        0, 20, 0, 0, 40, 8, DMA_QUEUE);

    // left column
    leftColumSprite = SPR_addSprite(&sprite_hud_left_column, //
                                    0, 0,                    //
                                    TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    rightColumSprite = SPR_addSprite(&sprite_hud_right_column, //
                                     304, 0,                   //
                                     TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    helmet1Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 96,              //
                                  TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    helmet2Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 112,             //
                                  TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    helmet3Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 128,             //
                                  TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
    SPR_setFrame(helmet3Sprite, 1);

    helmet4Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 144,             //
                                  TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
    SPR_setFrame(helmet4Sprite, 1);

    SPR_update();

    return vram_idx;
}

V2s32 viewToScreen(V2s32 *subject) {

    V2s32 position = {.x = subject->x + HUD_LEFT_COLUMN_WIDTH, .y = subject->y};

    return position;
}

V2s32 screenToView(V2s32 *subject) {

    V2s32 position = {.x = subject->x - HUD_LEFT_COLUMN_WIDTH, .y = subject->y};

    return position;
}
