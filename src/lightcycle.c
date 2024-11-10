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

#define SPEED_ZERO 0
#define SPEED_SLOW 2
#define SPEED_FAST 8

#define BOOST 0x10

#define LIGHTCYCLE_WIDTH 24
#define LIGHTCYCLE_HEIGHT 24

static void updateMovement(LightCycle *lightCycle);
static V2s16 normalizePosition(GridMovable *movable);

void initLightCycle(LightCycle *lightCycle) {

    lightCycle->health = ALIVE;
    lightCycle->finished = FALSE;

    // initialize position
    MovableInitMarker *movableMarker = (MovableInitMarker *)movables_markers[0];

    lightCycle->movable.object.mapPos.x = movableMarker->x;
    lightCycle->movable.object.mapPos.y = movableMarker->y;

    lightCycle->movable.object.gridPos.x = movableMarker->grid_x;
    lightCycle->movable.object.gridPos.y = movableMarker->grid_y;

    kprintf("P1: Grid pos [init]: x:%d, y:%d", lightCycle->movable.object.gridPos.x,
            lightCycle->movable.object.gridPos.y);
    kprintf("P1: Map pos [init]: x:%d, y:%d", lightCycle->movable.object.mapPos.x, lightCycle->movable.object.mapPos.y);

    // initialize movement
    lightCycle->movable.direction = DOWN;
    lightCycle->movable.speed = SPEED_SLOW;

    lightCycle->movable.gridPosDelta = 0;

    lightCycle->movable.mapPrevCrossing.x = movableMarker->x;
    lightCycle->movable.mapPrevCrossing.y = movableMarker->y;
    kprintf("P1: Map prev crossing [init]: x:%d, y:%d", lightCycle->movable.mapPrevCrossing.x,
            lightCycle->movable.mapPrevCrossing.y);

    lightCycle->movable.turn = 0;

    lightCycle->movable.justTurned = FALSE;

    // figure out the position to draw the sprite
    V2s16 posNormalized = normalizePosition(&lightCycle->movable);

    lightCycle->sprite = SPR_addSprite(&sprite_lightcycle_flynn,         //
                                       posNormalized.x, posNormalized.y, //
                                       TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setAnim(lightCycle->sprite, ANIM_DOWN);
}

void moveLightCycle(LightCycle *lightCycle) {

    updateMovement(lightCycle);
    updatePosition(&lightCycle->movable);

    V2s16 posNormalized = normalizePosition(&lightCycle->movable);

    SPR_setPosition(lightCycle->sprite, posNormalized.x, posNormalized.y);
}

static void updateMovement(LightCycle *lightCycle) {}

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
static V2s16 normalizePosition(GridMovable *movable) {

    V2s16 posInView = mapToView(&movable->object.mapPos);
    V2s16 posInScreen = viewToScreen(&posInView);

    if (movable->direction & DOWN) {
        V2s16 normalized = {.x = posInScreen.x - 16, .y = posInScreen.y - 12};
        return normalized;

    } else if (movable->direction & UP) {
        V2s16 normalized = {.x = posInScreen.x + 0, .y = posInScreen.y - 20};
        return normalized;

    } else if (movable->direction & LEFT) {
        V2s16 normalized = {.x = posInScreen.x - 24, .y = posInScreen.y - 21};
        return normalized;
    }

    // else movable->direction & RIGHT
    V2s16 normalized = {.x = posInScreen.x, .y = posInScreen.y - 9};
    return normalized;
};
