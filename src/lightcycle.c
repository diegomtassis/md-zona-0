/*
 * Filename: lightcycle.c
 * Created: August 21st 2023
 * Author: diegomtassis
 */

#include "lightcycle.h"

#include <kdebug.h>

#include "camera.h"
#include "screen.h"

#include "gfx_grid.h"
#include "gfx_lightcycles.h"
#include "tiled.h"

#define ANIM_LEFT 1
#define ANIM_LEFT_FLIP_H 1

#define ANIM_RIGHT 0
#define ANIM_RIGHT_FLIP_H 1

#define ANIM_UP 1
#define ANIM_UP_FLIP_H 0

#define ANIM_DOWN 0
#define ANIM_DOWN_FLIP_H 0

#define SPEED_ZERO FIX32_0
#define SPEED_H_SLOW FIX32(0.2)
#define SPEED_H_FAST FIX32(0.4)
#define SPEED_V_SLOW FIX32(0.1)
#define SPEED_V_FAST FIX32(0.2)

#define BOOST 0x10

#define LIGHTCYCLE_WIDTH 24
#define LIGHTCYCLE_HEIGHT 24

static void updateMovement(LightCycle *lightCycle);
static V2s32 normalizePosition(GridMovable *movable);

void initLightCycle(LightCycle *lightCycle) {

    lightCycle->health = ALIVE;
    lightCycle->finished = FALSE;

    // position
    MovableInitMarker *movableMarker = (MovableInitMarker *)movables_markers[0];

    lightCycle->movable.object.mapPos.x = FIX32(movableMarker->x);
    lightCycle->movable.object.mapPos.y = FIX32(movableMarker->y);

    lightCycle->movable.object.gridPos.x = movableMarker->grid_x;
    lightCycle->movable.object.gridPos.y = movableMarker->grid_y;

    kprintf("grid pos init: x:%d, y:%d", lightCycle->movable.object.gridPos.x, lightCycle->movable.object.gridPos.y);

    lightCycle->movable.object.size.x = LIGHTCYCLE_WIDTH;
    lightCycle->movable.object.size.y = LIGHTCYCLE_HEIGHT;
    lightCycle->movable.object.box.w = lightCycle->movable.object.size.x;
    lightCycle->movable.object.box.h = lightCycle->movable.object.size.y;

    updateMovableBox(&lightCycle->movable);

    kprintf("map pos init: x:%d, y:%d", lightCycle->movable.object.box.min.x, lightCycle->movable.object.box.min.y);

    // movement
    lightCycle->movable.direction = DOWN;
    lightCycle->movable.mov.x = -SPEED_H_SLOW;
    lightCycle->movable.mov.y = SPEED_V_SLOW;

    // lightCycle->movable.mov.x = 0;
    // lightCycle->movable.mov.y = 0;

    lightCycle->movable.turn = 0;

    // next crossing
    lightCycle->movable.nextCrossing.x = movableMarker->x - 16;
    lightCycle->movable.nextCrossing.y = movableMarker->y + 8;

    kprintf("next crossing init: x:%d, y:%d", lightCycle->movable.nextCrossing.x, lightCycle->movable.nextCrossing.y);

    // figure out the position to draw the sprite
    V2s32 posNormalized = normalizePosition(&lightCycle->movable);

    lightCycle->sprite = SPR_addSprite(&sprite_lightcycle_flynn,         //
                                       posNormalized.x, posNormalized.y, //
                                       TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setAnim(lightCycle->sprite, ANIM_DOWN);
}

void moveLightCycle(LightCycle *lightCycle) {

    updateMovement(lightCycle);
    updatePosition(&lightCycle->movable);

    V2s32 posNormalized = normalizePosition(&lightCycle->movable);

    SPR_setPosition(lightCycle->sprite, posNormalized.x, posNormalized.y);
}

static void updateMovement(LightCycle *lightCycle) {

    // speed is already set and keeps constant until something happens
    if (lightCycle->movable.justTurned) {
        if (lightCycle->movable.direction & DOWN) {
            lightCycle->movable.mov.x = -SPEED_H_SLOW;
            lightCycle->movable.mov.y = SPEED_V_SLOW;

        } else if (lightCycle->movable.direction & UP) {
            lightCycle->movable.mov.x = SPEED_H_SLOW;
            lightCycle->movable.mov.y = -SPEED_V_SLOW;

        } else if (lightCycle->movable.direction & LEFT) {
            lightCycle->movable.mov.x = -SPEED_H_SLOW;
            lightCycle->movable.mov.y = -SPEED_V_SLOW;

        } else if (lightCycle->movable.direction & RIGHT) {
            lightCycle->movable.mov.x = SPEED_H_SLOW;
            lightCycle->movable.mov.y = SPEED_V_SLOW;
        }
    }
}

void drawLightCycle(LightCycle *lightCycle) {

    if (lightCycle->movable.justTurned) {
        u8 direction = lightCycle->movable.direction;
        if (direction & DOWN) {
            SPR_setAnim(lightCycle->sprite, ANIM_DOWN);
            SPR_setHFlip(lightCycle->sprite, ANIM_DOWN_FLIP_H);

        } else if (direction & UP) {
            SPR_setAnim(lightCycle->sprite, ANIM_UP);
            SPR_setHFlip(lightCycle->sprite, ANIM_UP_FLIP_H);

        } else if (direction & LEFT) {
            SPR_setAnim(lightCycle->sprite, ANIM_LEFT);
            SPR_setHFlip(lightCycle->sprite, ANIM_LEFT_FLIP_H);

        } else if (direction & RIGHT) {
            SPR_setAnim(lightCycle->sprite, ANIM_RIGHT);
            SPR_setHFlip(lightCycle->sprite, ANIM_RIGHT_FLIP_H);
        }
    }
};

// Take into account the sprite shape
static V2s32 normalizePosition(GridMovable *movable) {

    V2s32 posInView = mapToView(&movable->object.box.min);
    V2s32 posInScreen = viewToScreen(&posInView);

    if (movable->direction & DOWN) {
        V2s32 normalized = {.x = posInScreen.x + 0, .y = posInScreen.y - 20};
        // kprintf("normalized: x:%d, y:%d", normalized.x, normalized.y);
        return normalized;

    } else if (movable->direction & UP) {

    } else if (movable->direction & LEFT) {

    } // else movable->direction & RIGHT

    V2s32 normalized;
    return normalized;
};
