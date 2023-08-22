/*
 * Filename: grid_physics.c
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#include "grid_physics.h"

#include "fwk/physics.h"

#include <kdebug.h>

static void handleCrossingCrossed(GridMovable* movable);
static bool turnIfRequested(GridMovable* movable);
static void updateNextCrossing(GridMovable* movable);

void updatePosition(GridMovable* movable) {

    movable->justTurned = FALSE;

    movable->object.pos.x += movable->mov.x;
    movable->object.pos.y += movable->mov.y;
    updateBox(movable);

    handleCrossing(movable);
}

void handleCrossing(GridMovable* movable) {

    if (movable->direction & DOWN) {
        // has the movable reached the next crossing?
        if (movable->object.box.max.x < movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }

    } else if (movable->direction & UP) {
        if (movable->object.box.min.x > movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }

    } else if (movable->direction & LEFT) {
        if (movable->object.box.max.x < movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }

    } else if (movable->direction & RIGHT) {
        if (movable->object.box.min.x > movable->nextCrossing.x) {
            handleCrossingCrossed(movable);
        }
    }
}

void updateBox(GridMovable* movable) {

    movable->object.box.min.x = fix16ToInt(movable->object.pos.x);
    movable->object.box.min.y = fix16ToInt(movable->object.pos.y);
    updateBoxMax(&movable->object.box);
}

Box_s16 targetBox(const GridMovable* movable) {

    Box_s16 box = { .w = movable->object.size.x, .h = movable->object.size.y };
    box.min.x = fix16ToInt(fix16Add(movable->object.pos.x, movable->mov.x));
    box.min.y = fix16ToInt(fix16Add(movable->object.pos.y, movable->mov.y));
    updateBoxMax(&box);

    return box;
}

Box_s16 targetHBox(const GridMovable* movable) {

    Box_s16 box = { .w = movable->object.size.x, .h = movable->object.size.y };
    box.min.x = fix16ToInt(fix16Add(movable->object.pos.x, movable->mov.x));
    box.min.y = fix16ToInt(movable->object.pos.y);
    updateBoxMax(&box);

    return box;
}

Box_s16 targetVBox(const GridMovable* movable) {

    Box_s16 box = { .w = movable->object.size.x, .h = movable->object.size.y };
    box.min.x = fix16ToInt(movable->object.pos.x);
    box.min.y = fix16ToInt(fix16Add(movable->object.pos.y, movable->mov.y));
    updateBoxMax(&box);

    return box;
}

static void handleCrossingCrossed(GridMovable* movable) {

    if (turnIfRequested(movable)) {
        updateBox(movable);
    }

    updateNextCrossing(movable);
}

static bool turnIfRequested(GridMovable* movable) {

    if (movable->turn) {

        u8 previousDirection = movable->direction;
        u8 newDirection = movable->turn;

        if (((previousDirection & UP) && (newDirection & DOWN))
            || ((previousDirection & DOWN) && (newDirection & UP))
            || ((previousDirection & LEFT) && (newDirection & RIGHT))
            || ((previousDirection & RIGHT) && (newDirection & LEFT))) {
            return FALSE;
        }

        movable->direction = newDirection;
        movable->turn = 0;
        movable->justTurned = TRUE;

        if (newDirection & UP) {
            if (previousDirection & LEFT) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y - movable->object.size.y);
            } else if (previousDirection & RIGHT) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y - movable->object.size.y);
            }
        } else if (newDirection & DOWN) {
            if (previousDirection & LEFT) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x - movable->object.size.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y);
            } else if (previousDirection & RIGHT) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x - movable->object.size.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y);
            }
        } else if (newDirection & LEFT) {
            if (previousDirection & UP) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x - movable->object.size.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y - movable->object.size.y);
            } else if (previousDirection & DOWN) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x - movable->object.size.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y - movable->object.size.y);
            }
        } else if (newDirection & RIGHT) {
            if (previousDirection & UP) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y);
            } else if (previousDirection & DOWN) {
                movable->object.pos.x = intToFix16(movable->nextCrossing.x);
                movable->object.pos.y = intToFix16(movable->nextCrossing.y);
            }
        }

        KLog_S2("Turn in crossing - x:", movable->nextCrossing.x, ", y:", movable->nextCrossing.y);

        return TRUE;
    }

    return FALSE;
}

static void updateNextCrossing(GridMovable* movable) {

    KLog_S2("Movable position - box min - x:", movable->object.box.min.x, ", y:", movable->object.box.min.y);
    KLog_S2("Movable position - box max - x:", movable->object.box.max.x, ", y:", movable->object.box.max.y);

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

    KLog_S2("Updated next crossing - x:", movable->nextCrossing.x, ", y:", movable->nextCrossing.y);
}