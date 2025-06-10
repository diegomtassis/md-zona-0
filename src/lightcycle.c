/*
 * Filename: lightcycle.c
 * Created: August 21st 2023
 * Author: diegomtassis
 */

#include "lightcycle.h"

#include <kdebug.h>

#include "camera.h"
#include "grid.h"
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
#define SPEED_SLOW 8
#define SPEED_FAST 12

#define BOOST 0x10

static void setCycleRenderInfo(LightCycle *lightCycle, bool force);
static void setCycleSpritePositionInMap(LightCycle *lightCycle);
static void setExplosionSpritePositionInMap(GridMovable *movable);
static void setSpriteAnim(LightCycle *lightCycle);

static void CYCLE_step(LightCycle *lightCycle);
static void CYCLE_crash(LightCycle *lightCycle);
static void CYCLE_setRenderInfo(LightCycle *lightCycle);

static void addTrailSegment(TrailSegmentDefinition *trailSegmentDef);
static TrailSegmentDefinition figureOutTrailSegment(GridMovable *movable);

void CYCLE_init(LightCycle *lightCycle) {

    lightCycle->movable.object.health = ALIVE;

    // Initialize position
    MovableInitMarker *cycleMarker = (MovableInitMarker *)movables_markers_zona_14[0];

    lightCycle->movable.object.gridPos.x = cycleMarker->grid_x;
    lightCycle->movable.object.gridPos.y = cycleMarker->grid_y;

    lightCycle->movable.object.mapPos.x = cycleMarker->x;
    lightCycle->movable.object.mapPos.y = cycleMarker->y;

    // Initialize movement
    lightCycle->movable.direction = DOWN;
    lightCycle->movable.speed = SPEED_SLOW;

    lightCycle->movable.gridPosDelta = 10;

    lightCycle->movable.mapPrevCrossing.x = cycleMarker->x;
    lightCycle->movable.mapPrevCrossing.y = cycleMarker->y;

    lightCycle->movable.turnTo = 0;
    lightCycle->movable.justTurned = FALSE;

    // Create the sprite. Position will be set by the camera.
    lightCycle->movable.object.sprite = SPR_addSprite(&sprite_lightcycle_flynn, //
                                                      0, 0,                     //
                                                      TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    setCycleRenderInfo(lightCycle, TRUE);
}

void CYCLE_act(LightCycle *lightCycle, u8 turnTo, bool boost) {

    if (lightCycle->movable.object.health & DEREZZED) {
        return;
    }

    lightCycle->boost = boost;
    lightCycle->movable.speed = boost ? SPEED_FAST : SPEED_SLOW;
    lightCycle->movable.turnTo = turnTo;
    lightCycle->movable.object.justBegunDerezzing = FALSE;
    lightCycle->movable.object.viewIsDirty = FALSE;

    if (lightCycle->movable.object.health & ALIVE) {
        CYCLE_step(lightCycle);
        if (lightCycle->movable.object.justBegunDerezzing) {
            CYCLE_crash(lightCycle);
        }
    } else {
        // Already DEREZZING
        if (SPR_isAnimationDone(lightCycle->movable.object.sprite)) {
            lightCycle->movable.object.health = DEREZZED;
        }
    }

    if (lightCycle->movable.object.viewIsDirty) {
        // kprintf("P1: ACT");
        CYCLE_setRenderInfo(lightCycle);
        addTrailSegment(&lightCycle->trailDef);
        lightCycle->trailDef = figureOutTrailSegment(&lightCycle->movable);
    }
}

void CYCLE_step(LightCycle *lightCycle) { VEH_move(&lightCycle->movable); }

void CYCLE_crash(LightCycle *lightCycle) {

    lightCycle->movable.object.health = DEREZZING;

    // Replace the cycle sprite with an explosion
    SPR_releaseSprite(lightCycle->movable.object.sprite);

    PAL_setPalette(PAL3, palette_explosion.data, DMA);
    lightCycle->movable.object.sprite = SPR_addSprite(&sprite_explosion, //
                                                      0, 0,              // position set by the camera
                                                      TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
    SPR_setAnimationLoop(lightCycle->movable.object.sprite, FALSE);
    lightCycle->movable.object.viewIsDirty = TRUE;
}

void CYCLE_setRenderInfo(LightCycle *lightCycle) {

    if (lightCycle->movable.object.health & ALIVE) {
        setCycleRenderInfo(lightCycle, FALSE);
        return;
    };

    if (lightCycle->movable.object.justBegunDerezzing) {
        setExplosionSpritePositionInMap(&lightCycle->movable);
        return;
    };
}

void CYCLE_release(LightCycle *lightCycle) {

    // Release the sprite.
    SPR_releaseSprite(lightCycle->movable.object.sprite);
}

static void setCycleRenderInfo(LightCycle *lightCycle, bool force) {

    setCycleSpritePositionInMap(lightCycle);
    if (lightCycle->movable.justTurned || force) {
        setSpriteAnim(lightCycle);
    }
    SPR_setFrame(lightCycle->movable.object.sprite, lightCycle->boost ? 1 : 0);
}

// Take into account the sprite shape
static void setCycleSpritePositionInMap(LightCycle *lightCycle) {

    GridMovable *movable = &lightCycle->movable;

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

    if (lightCycle->boost) {
        movable->object.spritePosInMap.y -= 4;
    }
}

// Take into account the sprite shape
static void setExplosionSpritePositionInMap(GridMovable *movable) {

    movable->object.spritePosInMap.x = movable->object.mapPos.x - 20;
    movable->object.spritePosInMap.y = movable->object.mapPos.y - 20;
}

static void setSpriteAnim(LightCycle *lightCycle) {

    GridMovable *movable = &lightCycle->movable;

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

static void addTrailSegment(TrailSegmentDefinition *trailSegmentDef) {

    V2u16 cycleTilePos = SCREEN_posToTile(trailSegmentDef->mapPos);
    u8 trailDirection = trailSegmentDef->direction;

    VDPPlane plane = BG_A;

    if (trailSegmentDef->first) {
        if (trailDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 1),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 5),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (trailDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 0), cycleTilePos.x,
                             cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 4), cycleTilePos.x,
                             cycleTilePos.y - 1);

        } else if (trailDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 3),
                             cycleTilePos.x - 1, cycleTilePos.y - 2);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 7),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);

        } else if (trailDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 2), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 6), cycleTilePos.x,
                             cycleTilePos.y);
        }
    } else {
        if (trailDirection & DOWN) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 0),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 4),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (trailDirection & UP) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 1), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 5), cycleTilePos.x,
                             cycleTilePos.y);

        } else if (trailDirection & LEFT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 2),
                             cycleTilePos.x - 1, cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 6),
                             cycleTilePos.x - 1, cycleTilePos.y);

        } else if (trailDirection & RIGHT) {
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 3), cycleTilePos.x,
                             cycleTilePos.y - 1);
            VDP_setTileMapXY(plane, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, trailsVramBaseTile + 7), cycleTilePos.x,
                             cycleTilePos.y);
        }
    }
}

static TrailSegmentDefinition figureOutTrailSegment(GridMovable *movable) {

    return (TrailSegmentDefinition){
        .direction = movable->direction, .first = movable->gridPosDelta < 50, .mapPos = movable->object.mapPos};
}
