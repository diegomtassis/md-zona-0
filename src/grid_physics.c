/*
 * Filename: grid_physics.c
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#include "grid_physics.h"

#include "fwk/physics.h"

#include <kdebug.h>

static void moveForward(GridMovable *movable, u16 h_gap, u16 v_gap);
static void handleCrossingCrossed(GridMovable *movable);
static bool turnIfRequested(GridMovable *movable);
static void updateGridPosAfterCrossingCrossed(GridMovable *movable);
static void updatePrevCrossingAfterCrossingCrossed(GridMovable *movable);
static void placeInCrossing(GridMovable *movable);

void VEH_move(GridMovable *movable) {

    movable->justTurned = FALSE;
    movable->spritePosJustChanged = FALSE;

    u16 prevGridPosDelta = movable->gridPosDelta;
    movable->gridPosDelta += movable->speed;

    // 2 steps
    if (movable->gridPosDelta >= 99) {
        handleCrossingCrossed(movable);

    } else if (movable->gridPosDelta > 49 && prevGridPosDelta <= 49) {
        moveForward(movable, 8, 4);
    }
}

static void moveForward(GridMovable *movable, u16 h_gap, u16 v_gap) {

    if (movable->direction & DOWN) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x - h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y + v_gap;

    } else if (movable->direction & UP) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x + h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y - v_gap;

    } else if (movable->direction & LEFT) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x - h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y - v_gap;

    } else if (movable->direction & RIGHT) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x + h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y + v_gap;
    }

    // kprintf("P1: cycle pos in map: x:%d, y:%d", movable->object.mapPos.x, movable->object.mapPos.y);

    movable->spritePosJustChanged = TRUE;
}

static void handleCrossingCrossed(GridMovable *movable) {

    // kprintf("P1: Crossing crossed!");

    updateGridPosAfterCrossingCrossed(movable);
    // kprintf("P1: cycle pos in grid: x:%d, y:%d", movable->object.gridPos.x, movable->object.gridPos.y);

    updatePrevCrossingAfterCrossingCrossed(movable);
    // kprintf("P1: prev cross pos in map: x:%d, y:%d", movable->mapPrevCrossing.x, movable->mapPrevCrossing.y);

    placeInCrossing(movable);
    // kprintf("P1: cycle pos in map: x:%d, y:%d", movable->object.mapPos.x, movable->object.mapPos.y);

    movable->gridPosDelta = 0;

    bool turned = turnIfRequested(movable);
    // if (turned) {
    //     kprintf("P1: cycle turned [%d]!", movable->direction);
    // }

    movable->spritePosJustChanged = TRUE;
}

static bool turnIfRequested(GridMovable *movable) {

    if (movable->turn) {

        u8 previousDirection = movable->direction;
        u8 newDirection = movable->turn;

        if (((previousDirection & UP) && (newDirection & DOWN)) ||
            ((previousDirection & DOWN) && (newDirection & UP)) ||
            ((previousDirection & LEFT) && (newDirection & RIGHT)) ||
            ((previousDirection & RIGHT) && (newDirection & LEFT))) {
            return FALSE;
        }

        movable->direction = newDirection;
        movable->turn = 0;
        movable->justTurned = TRUE;

        return TRUE;
    }

    return FALSE;
}

static void updateGridPosAfterCrossingCrossed(GridMovable *movable) {

    if (movable->direction & DOWN) {
        movable->object.gridPos.y++;

    } else if (movable->direction & UP) {
        movable->object.gridPos.y--;

    } else if (movable->direction & LEFT) {
        movable->object.gridPos.x--;

    } else if (movable->direction & RIGHT) {
        movable->object.gridPos.x++;
    }
}

static void updatePrevCrossingAfterCrossingCrossed(GridMovable *movable) {

    if (movable->direction & DOWN) {
        movable->mapPrevCrossing.x -= 16;
        movable->mapPrevCrossing.y += 8;

    } else if (movable->direction & UP) {
        movable->mapPrevCrossing.x += 16;
        movable->mapPrevCrossing.y -= 8;

    } else if (movable->direction & LEFT) {
        movable->mapPrevCrossing.x -= 16;
        movable->mapPrevCrossing.y -= 8;

    } else if (movable->direction & RIGHT) {
        movable->mapPrevCrossing.x += 16;
        movable->mapPrevCrossing.y += 8;
    }
}

static void placeInCrossing(GridMovable *movable) {
    movable->object.mapPos.x = movable->mapPrevCrossing.x;
    movable->object.mapPos.y = movable->mapPrevCrossing.y;
}
