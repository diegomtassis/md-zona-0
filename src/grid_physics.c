/*
 * Filename: grid_physics.c
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#include "grid_physics.h"

#include "fwk/physics.h"

#include <kdebug.h>

static void handleCrossingCrossed(GridMovable *movable);
static bool turnIfRequested(GridMovable *movable);
static void updateGridPos(GridMovable *movable);
static void updatePrevCrossing(GridMovable *movable);
static void placeInCrossing(GridMovable *movable);

static void updateMapPositionIfNeeded(GridMovable *movable, u16 h_gap, u16 v_gap);

void updatePosition(GridMovable *movable) {

    movable->justTurned = FALSE;

    movable->gridPosDelta += movable->speed;

    if (movable->gridPosDelta > 87) {
        updateMapPositionIfNeeded(movable, 14, 7);
    } else if (movable->gridPosDelta > 74) {
        updateMapPositionIfNeeded(movable, 12, 6);
    } else if (movable->gridPosDelta > 61) {
        updateMapPositionIfNeeded(movable, 10, 5);
    } else if (movable->gridPosDelta > 49) {
        updateMapPositionIfNeeded(movable, 8, 4);
    } else if (movable->gridPosDelta > 37) {
        updateMapPositionIfNeeded(movable, 6, 3);
    } else if (movable->gridPosDelta > 24) {
        updateMapPositionIfNeeded(movable, 4, 2);
    } else if (movable->gridPosDelta > 12) {
        updateMapPositionIfNeeded(movable, 2, 1);
    } 

    handleCrossing(movable);
}

void handleCrossing(GridMovable *movable) {

    if (movable->gridPosDelta >= 99) {
        handleCrossingCrossed(movable);
    }
}

void updateMovableBox(GridMovable *movable) {

    // movable->object.box.min.x = fix32ToInt(movable->object.mapPos.x);
    // movable->object.box.min.y = fix32ToInt(movable->object.mapPos.y);
    // updateBoxMax(&movable->object.box);
}

static void updateMapPositionIfNeeded(GridMovable *movable, u16 h_gap, u16 v_gap) {

    if (movable->direction & DOWN) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x - h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y + v_gap;
        updateMovableBox(movable);

    } else if (movable->direction & UP) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x + h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y - v_gap;
        updateMovableBox(movable);

    } else if (movable->direction & LEFT) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x - h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y - v_gap;
        updateMovableBox(movable);

    } else if (movable->direction & RIGHT) {
        movable->object.mapPos.x = movable->mapPrevCrossing.x + h_gap;
        movable->object.mapPos.y = movable->mapPrevCrossing.y + v_gap;
        updateMovableBox(movable);
    }
}

static void handleCrossingCrossed(GridMovable *movable) {

    kprintf("P1: Crossing crossed!");

    if (movable->direction & DOWN) {

    } else if (movable->direction & UP) {

    } else if (movable->direction & LEFT) {

    } else if (movable->direction & RIGHT) {
    }

    updatePrevCrossing(movable);
    kprintf("P1: Prev crossing: x:%d, y:%d", movable->mapPrevCrossing.x, movable->mapPrevCrossing.y);

    placeInCrossing(movable);

    updateGridPos(movable);
    kprintf("P1: Grid pos: x:%d, y:%d", movable->object.gridPos.x, movable->object.gridPos.y);

    movable->gridPosDelta = 0;

    bool turned = turnIfRequested(movable);
    if (turned) {
        updateMovableBox(movable);
        kprintf("P1: Turned [%d]!", movable->direction);
    }
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

static void updateGridPos(GridMovable *movable) {

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

static void updatePrevCrossing(GridMovable *movable) {

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
    updateMovableBox(movable);
}
