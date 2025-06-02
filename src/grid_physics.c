/*
 * Filename: grid_physics.c
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#include "grid_physics.h"

#include "grid.h"
#include "screen.h"

#include "fwk/physics.h"

#include <kdebug.h>

#define MASK_TILE_NUMBER 0x3FF

static void moveForward(GridMovable *movable, u16 h_gap, u16 v_gap);
static void handleCrossingCrossed(GridMovable *movable);
static bool turnIfRequested(GridMovable *movable);
static void updateGridPosAfterCrossingCrossed(GridMovable *movable);
static bool checkCollisions(GridMovable *movable);
static void updatePrevCrossingAfterCrossingCrossed(GridMovable *movable);
static void placeInCrossing(GridMovable *movable);

static void triggerDerezzing(GridMovable *movable);

static V2s16 collisionPosition(GridMovable *movable);

void VEH_move(GridMovable *movable) {

    movable->justTurned = FALSE;

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

    movable->object.viewIsDirty = TRUE;
}

static void handleCrossingCrossed(GridMovable *movable) {

    updateGridPosAfterCrossingCrossed(movable);
    updatePrevCrossingAfterCrossingCrossed(movable);
    placeInCrossing(movable);
    checkCollisions(movable);

    movable->gridPosDelta = 0;

    turnIfRequested(movable);

    movable->object.viewIsDirty = TRUE;
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

static bool checkCollisions(GridMovable *movable) {

    V2u16 tileCollisionPos = SCREEN_posToTile(movable->object.mapPos);
    u16 tileCollisionAttrFG = MAP_getTile(mapGridFG, tileCollisionPos.x, tileCollisionPos.y);

    // FG
    if (movable->direction & RIGHT) {
        if (tileCollisionAttrFG == 4 || tileCollisionAttrFG == 8 || tileCollisionAttrFG == 2) {
            triggerDerezzing(movable);
            return TRUE;
        }
    } else if (movable->direction & UP) {
        if (tileCollisionAttrFG == 4 || tileCollisionAttrFG == 8 || tileCollisionAttrFG == 2) {
            triggerDerezzing(movable);
            return TRUE;
        }
    } else if (movable->direction & DOWN) {
        if (tileCollisionAttrFG == 1 || tileCollisionAttrFG == 7 || tileCollisionAttrFG == 11) {
            triggerDerezzing(movable);
            return TRUE;
        }
    } else if (movable->direction & LEFT) {
        if (tileCollisionAttrFG == 1 || tileCollisionAttrFG == 7 || tileCollisionAttrFG == 11) {
            triggerDerezzing(movable);
            return TRUE;
        }
    }

    // BG
    u16 tileCollisionAttrBG = MAP_getTile(mapGridBG, tileCollisionPos.x, tileCollisionPos.y);
    if ((tileCollisionAttrBG & MASK_TILE_NUMBER) > 1) {
        triggerDerezzing(movable);
        return TRUE;
    }

    return FALSE;
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

static void triggerDerezzing(GridMovable *movable) { movable->object.justBegunDerezzing = TRUE; }

static V2s16 collisionPosition(GridMovable *movable) {

    if (movable->direction & UP) {
        return (V2s16){movable->object.mapPos.x, movable->object.mapPos.y - 1};
    }

    if (movable->direction & DOWN) {
        return (V2s16){movable->object.mapPos.x - 1, movable->object.mapPos.y - 1};
    }

    if (movable->direction & LEFT) {
        return (V2s16){movable->object.mapPos.x - 1, movable->object.mapPos.y - 1};
    }

    // (movable->direction & RIGHT)
    return (V2s16){movable->object.mapPos.x, movable->object.mapPos.y - 1};
}
