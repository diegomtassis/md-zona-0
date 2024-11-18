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
#define SPEED_SLOW 1
#define SPEED_FAST 8

#define BOOST 0x10

#define LIGHTCYCLE_WIDTH 24
#define LIGHTCYCLE_HEIGHT 24

static void updateSpritePositionInMap(GridMovable *movable);

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

    lightCycle->movable.gridPosDelta = 90;

    lightCycle->movable.mapPrevCrossing.x = movableMarker->x;
    lightCycle->movable.mapPrevCrossing.y = movableMarker->y;
    kprintf("P1: Map prev crossing [init]: x:%d, y:%d", lightCycle->movable.mapPrevCrossing.x,
            lightCycle->movable.mapPrevCrossing.y);

    lightCycle->movable.turn = 0;
    lightCycle->movable.justTurned = FALSE;

    lightCycle->movable.object.sprite = SPR_addSprite(&sprite_lightcycle_flynn, //
                                                      0, 0,                     //
                                                      TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    updateLightCycleDrawInfo(lightCycle);
}

void moveLightCycle(LightCycle *lightCycle) { updatePosition(&lightCycle->movable); }

void updateLightCycleDrawInfo(LightCycle *lightCycle) {

    updateSpritePositionInMap(&lightCycle->movable);

    if (lightCycle->movable.justTurned) {
        u8 direction = lightCycle->movable.direction;
        if (direction & DOWN) {
            SPR_setAnim(lightCycle->movable.object.sprite, ANIM_DOWN);
            SPR_setHFlip(lightCycle->movable.object.sprite, ANIM_DOWN_FLIP_H);

        } else if (direction & UP) {
            SPR_setAnim(lightCycle->movable.object.sprite, ANIM_UP);
            SPR_setHFlip(lightCycle->movable.object.sprite, ANIM_UP_FLIP_H);

        } else if (direction & LEFT) {
            SPR_setAnim(lightCycle->movable.object.sprite, ANIM_LEFT);
            SPR_setHFlip(lightCycle->movable.object.sprite, ANIM_LEFT_FLIP_H);

        } else if (direction & RIGHT) {
            SPR_setAnim(lightCycle->movable.object.sprite, ANIM_RIGHT);
            SPR_setHFlip(lightCycle->movable.object.sprite, ANIM_RIGHT_FLIP_H);
        }
    }
};

// Take into account the sprite shape
static void updateSpritePositionInMap(GridMovable *movable) {

    if (movable->direction & DOWN) {
        movable->object.spritePosInMap.x = movable->object.mapPos.x - 24;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 8;

    } else if (movable->direction & UP) {
        movable->object.spritePosInMap.x = movable->object.mapPos.x + 0;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 20;

    } else if (movable->direction & LEFT) {
        movable->object.spritePosInMap.x = movable->object.mapPos.x - 24;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 21;

    } else { // movable->direction & RIGHT
        movable->object.spritePosInMap.x = movable->object.mapPos.x;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 9;
    }

    kprintf("P1: sprite pos in map: x:%d, y:%d", movable->object.spritePosInMap.x,
            movable->object.spritePosInMap.y);
};
