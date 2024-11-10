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
static void updateNextCrossing(GridMovable *movable);

void updatePosition(GridMovable *movable) {

    movable->justTurned = FALSE;

    movable->object.mapPos.x += movable->mov.x;
    movable->object.mapPos.y += movable->mov.y;
    updateMovableBox(movable);

    handleCrossing(movable);
}

void handleCrossing(GridMovable *movable) {

    if (movable->direction & DOWN) {
        // has the movable reached the next crossing?
        if (movable->object.box.min.x < movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }

    } else if (movable->direction & UP) {
        if (movable->object.box.min.x > movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }

    } else if (movable->direction & LEFT) {
        if (movable->object.box.min.x < movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }

    } else if (movable->direction & RIGHT) {
        if (movable->object.box.min.x > movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }
    }
}

void updateMovableBox(GridMovable *movable) {

    movable->object.box.min.x = fix32ToInt(movable->object.mapPos.x);
    movable->object.box.min.y = fix32ToInt(movable->object.mapPos.y);
    updateBoxMax(&movable->object.box);
}

Box_s32 targetBox(const GridMovable *movable) {

    Box_s32 box = {.w = movable->object.size.x, .h = movable->object.size.y};
    box.min.x = fix32ToInt(movable->object.mapPos.x + movable->mov.x);
    box.min.y = fix32ToInt(movable->object.mapPos.y + movable->mov.y);
    updateBoxMax(&box);

    return box;
}

Box_s32 targetHBox(const GridMovable *movable) {

    Box_s32 box = {.w = movable->object.size.x, .h = movable->object.size.y};
    box.min.x = fix32ToInt(movable->object.mapPos.x + movable->mov.x);
    box.min.y = fix32ToInt(movable->object.mapPos.y);
    updateBoxMax(&box);

    return box;
}

Box_s32 targetVBox(const GridMovable *movable) {

    Box_s32 box = {.w = movable->object.size.x, .h = movable->object.size.y};
    box.min.x = fix32ToInt(movable->object.mapPos.x);
    box.min.y = fix32ToInt(movable->object.mapPos.y + movable->mov.y);
    updateBoxMax(&box);

    return box;
}

static void handleCrossingCrossed(GridMovable *movable) {

    kprintf("P1: Map pos: x:%d, y:%d", movable->object.box.min.x, movable->object.box.min.y);
    kprintf("P1: Crossing crossed!");

    updateGridPos(movable);
    kprintf("P1: Grid pos: x:%d, y:%d", movable->object.gridPos.x, movable->object.gridPos.y);

    bool turned = turnIfRequested(movable);
    if (turned) {
        updateMovableBox(movable);
        kprintf("P1: Turned [%d]!", movable->direction);
    }

    updateNextCrossing(movable);
    kprintf("P1: Next crossing: x:%d, y:%d", movable->nextCrossing.x, movable->nextCrossing.y);
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

        if (newDirection & UP) {
            movable->object.mapPos.x = intToFix32(movable->nextCrossing.x);
            movable->object.mapPos.y = intToFix32(movable->nextCrossing.y);

        } else if (newDirection & DOWN) {
            movable->object.mapPos.x = intToFix32(movable->nextCrossing.x);
            movable->object.mapPos.y = intToFix32(movable->nextCrossing.y);

        } else if (newDirection & LEFT) {
            movable->object.mapPos.x = intToFix32(movable->nextCrossing.x);
            movable->object.mapPos.y = intToFix32(movable->nextCrossing.y);

        } else if (newDirection & RIGHT) {
            movable->object.mapPos.x = intToFix32(movable->nextCrossing.x);
            movable->object.mapPos.y = intToFix32(movable->nextCrossing.y);
        }

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

static void updateNextCrossing(GridMovable *movable) {

    if (movable->direction & DOWN) {
        movable->nextCrossing.x += -16;
        movable->nextCrossing.y += 8;

    } else if (movable->direction & UP) {
        movable->nextCrossing.x += 16;
        movable->nextCrossing.y += -8;

    } else if (movable->direction & LEFT) {
        movable->nextCrossing.x += -16;
        movable->nextCrossing.y += -8;

    } else if (movable->direction & RIGHT) {
        movable->nextCrossing.x += 16;
        movable->nextCrossing.y += 8;
    }
}