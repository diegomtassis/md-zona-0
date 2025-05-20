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
#define SPEED_SLOW 6
#define SPEED_FAST 10

#define BOOST 0x10

static void setRenderInfo(GridMovable *movable, bool force);
static void setSpritePositionInMap(GridMovable *movable);
static void setSpriteAnim(GridMovable *movable);

void CYCLE_init(LightCycle *lightCycle) {

    lightCycle->health = ALIVE;
    lightCycle->finished = FALSE;

    // Initialize position
    MovableInitMarker *cycleMarker = (MovableInitMarker *)movables_markers_zona_14[0];

    lightCycle->movable.object.gridPos.x = cycleMarker->grid_x;
    lightCycle->movable.object.gridPos.y = cycleMarker->grid_y;
    // kprintf("P1: cycle pos in grid [init]: x:%d, y:%d", lightCycle->movable.object.gridPos.x,
    //         lightCycle->movable.object.gridPos.y);

    lightCycle->movable.object.mapPos.x = cycleMarker->x;
    lightCycle->movable.object.mapPos.y = cycleMarker->y;
    // kprintf("P1: cycle pos in map [init]: x:%d, y:%d", lightCycle->movable.object.mapPos.x,
    //         lightCycle->movable.object.mapPos.y);

    // Initialize movement
    lightCycle->movable.direction = DOWN;
    lightCycle->movable.speed = SPEED_SLOW;

    lightCycle->movable.gridPosDelta = 10;

    lightCycle->movable.mapPrevCrossing.x = cycleMarker->x;
    lightCycle->movable.mapPrevCrossing.y = cycleMarker->y;
    // kprintf("P1: prev cross pos in map [init]: x:%d, y:%d", lightCycle->movable.mapPrevCrossing.x,
    //         lightCycle->movable.mapPrevCrossing.y);

    lightCycle->movable.turn = 0;
    lightCycle->movable.justTurned = FALSE;

    // Create the sprite. Position will be set by the camera.
    lightCycle->movable.object.sprite = SPR_addSprite(&sprite_lightcycle_flynn, //
                                                      0, 0,                     //
                                                      TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    setRenderInfo(&lightCycle->movable, TRUE);
}

void CYCLE_move(LightCycle *lightCycle) { VEH_move(&lightCycle->movable); }

void CYCLE_setRenderInfo(LightCycle *lightCycle) { setRenderInfo(&lightCycle->movable, FALSE); };

static void setRenderInfo(GridMovable *movable, bool force) {
    setSpritePositionInMap(movable);

    if (movable->justTurned || force) {
        setSpriteAnim(movable);
    }
}

// Take into account the sprite shape
static void setSpritePositionInMap(GridMovable *movable) {

    if (movable->direction & DOWN) {
        movable->object.spritePosInMap.x = movable->object.mapPos.x - 16;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 12;

    } else if (movable->direction & UP) {
        movable->object.spritePosInMap.x = movable->object.mapPos.x - 8;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 12;

    } else if (movable->direction & LEFT) {
        movable->object.spritePosInMap.x = movable->object.mapPos.x - 16;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 12;

    } else { // movable->direction & RIGHT
        movable->object.spritePosInMap.x = movable->object.mapPos.x - 8;
        movable->object.spritePosInMap.y = movable->object.mapPos.y - 12;
    }

    // kprintf("P1: sprite pos in map: x:%d, y:%d", movable->object.spritePosInMap.x, movable->object.spritePosInMap.y);
};

static void setSpriteAnim(GridMovable *movable) {

    u8 direction = movable->direction;
    if (direction & DOWN) {
        SPR_setAnim(movable->object.sprite, ANIM_DOWN);
        SPR_setHFlip(movable->object.sprite, ANIM_DOWN_FLIP_H);

    } else if (direction & UP) {
        SPR_setAnim(movable->object.sprite, ANIM_UP);
        SPR_setHFlip(movable->object.sprite, ANIM_UP_FLIP_H);

    } else if (direction & LEFT) {
        SPR_setAnim(movable->object.sprite, ANIM_LEFT);
        SPR_setHFlip(movable->object.sprite, ANIM_LEFT_FLIP_H);

    } else if (direction & RIGHT) {
        SPR_setAnim(movable->object.sprite, ANIM_RIGHT);
        SPR_setHFlip(movable->object.sprite, ANIM_RIGHT_FLIP_H);
    }
}
