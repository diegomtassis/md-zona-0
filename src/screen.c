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

Sprite *spriteReady;

u16 SCREEN_setupHud(u16 vramBase) {

    PAL_setPalette(PAL1, palette_hud.data, DMA_QUEUE);

    u16 idx_tile_bg_hud = vramBase;
    u16 vram_idx = idx_tile_bg_hud;

    VDP_loadTileSet(&tileset_hud_low, idx_tile_bg_hud, DMA_QUEUE);
    vram_idx += tileset_hud_low.numTile;

    VDP_setWindowVPos(TRUE, 20); // the last 8 rows

    VDP_setTileMapEx(WINDOW, &tilemap_hud_low,                                  //
                     TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, idx_tile_bg_hud), //
                     0, 20, 0, 0, 40, 8, DMA_QUEUE);

    // left column
    leftColumSprite = SPR_addSprite(&sprite_hud_left_column, //
                                    0, 0,                    //
                                    TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    rightColumSprite = SPR_addSprite(&sprite_hud_right_column, //
                                     304, 0,                   //
                                     TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    helmet1Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 96,              //
                                  TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    helmet2Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 112,             //
                                  TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    helmet3Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 128,             //
                                  TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setFrame(helmet3Sprite, 1);

    helmet4Sprite = SPR_addSprite(&sprite_hud_helmet, //
                                  0, 144,             //
                                  TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setFrame(helmet4Sprite, 1);

    SPR_update();

    return vram_idx;
}

V2s16 SCREEN_viewToScreen(V2s16 *object) {

    V2s16 position = {.x = object->x + HUD_LEFT_COLUMN_WIDTH, .y = object->y};

    return position;
}

V2s16 SCREEN_screenToView(V2s16 *object) {

    V2s16 position = {.x = object->x - HUD_LEFT_COLUMN_WIDTH, .y = object->y};

    return position;
}

V2s16 SCREEN_screenInMap(V2s16 *viewInMap) {

    V2s16 position = {.x = viewInMap->x - HUD_LEFT_COLUMN_WIDTH, .y = viewInMap->y};

    return position;
}

void SCREEN_showReady() {
    if (!spriteReady) {
        spriteReady = SPR_addSprite(&sprite_text_ready, //
                                    110, 110,           //
                                    TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    }
    SPR_setVisibility(spriteReady, VISIBLE);
}

void SCREEN_clearMessage() {
    if (spriteReady) {
        SPR_setVisibility(spriteReady, HIDDEN);
    }
}