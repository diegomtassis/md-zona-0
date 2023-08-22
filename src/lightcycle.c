/*
 * Filename: lightcycle.c
 * Created: August 21st 2023
 * Author: diegomtassis
 */

#include "lightcycle.h"

#include <kdebug.h>

#include "gfx_lightcycles.h"

#define ANIM_LEFT		    1
#define ANIM_LEFT_FLIP_H	1

#define ANIM_RIGHT		    0
#define ANIM_RIGHT_FLIP_H	1

#define ANIM_UP	    	    1
#define ANIM_UP_FLIP_H	   	0

#define ANIM_DOWN       	0
#define ANIM_DOWN_FLIP_H	0

#define SPEED_ZERO			FIX16_0
#define SPEED_H_SLOW		FIX16(0.2)
#define SPEED_H_FAST		FIX16(0.4)
#define SPEED_V_SLOW		FIX16(0.1)
#define SPEED_V_FAST    	FIX16(0.2)

#define BOOST		0x10

#define PLAYER_HEIGHT 24
#define PLAYER_WIDTH 24

static void calculateNextMovement(LightCycle* lightCycle);

void initLightCycle(LightCycle* lightCycle) {

    lightCycle->health = ALIVE;
    lightCycle->finished = FALSE;

    // position
    lightCycle->movable.object.pos.x = FIX16(64);
    lightCycle->movable.object.pos.y = FIX16(15);
    lightCycle->movable.object.size.x = 24;
    lightCycle->movable.object.size.y = 24;
    lightCycle->movable.object.box.w = lightCycle->movable.object.size.x;
    lightCycle->movable.object.box.h = lightCycle->movable.object.size.y;

    updateBox(&lightCycle->movable);

    KLog_S2("Movable position - box min - x:", lightCycle->movable.object.box.min.x, ", y:", lightCycle->movable.object.box.min.y);
    KLog_S2("Movable position - box max - x:", lightCycle->movable.object.box.max.x, ", y:", lightCycle->movable.object.box.max.y);

    // movement
    lightCycle->movable.direction = DOWN;
    lightCycle->movable.mov.x = -SPEED_H_SLOW;
    lightCycle->movable.mov.y = SPEED_V_SLOW;

    // lightCycle->movable.mov.x = 0;
    // lightCycle->movable.mov.y = 0;

    lightCycle->movable.turn = 0;

    // next crossing
    lightCycle->movable.nextCrossing.x = 64 - 16; // min.x
    lightCycle->movable.nextCrossing.y = 38 + 15; // max.y
    KLog_U2("Init next crossing - x:", lightCycle->movable.nextCrossing.x, ", y:", lightCycle->movable.nextCrossing.y);

    lightCycle->sprite = SPR_addSprite(&flynn_sprite, fix16ToInt(lightCycle->movable.object.pos.x), fix16ToInt(lightCycle->movable.object.pos.y), TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setAnim(lightCycle->sprite, ANIM_DOWN);
}

void moveLightCycle(LightCycle* lightCycle) {

    calculateNextMovement(lightCycle);
    updatePosition(&lightCycle->movable);
    SPR_setPosition(lightCycle->sprite, fix16ToInt(lightCycle->movable.object.pos.x), fix16ToInt(lightCycle->movable.object.pos.y));
}

static void calculateNextMovement(LightCycle* lightCycle) {

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

void drawLightCycle(LightCycle* lightCycle) {

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
