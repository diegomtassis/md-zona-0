/*
 * Filename: lightcycle.c
 * Created: August 21st 2023
 * Author: diegomtassis
 */

#include "lightcycle.h"

#include <kdebug.h>

#include "camera.h"
#include "screen.h"

#include "gfx_lightcycles.h"

#define ANIM_LEFT		    1
#define ANIM_LEFT_FLIP_H	1

#define ANIM_RIGHT		    0
#define ANIM_RIGHT_FLIP_H	1

#define ANIM_UP	    	    1
#define ANIM_UP_FLIP_H	   	0

#define ANIM_DOWN       	0
#define ANIM_DOWN_FLIP_H	0

#define SPEED_ZERO			FIX32_0
#define SPEED_H_SLOW		FIX32(0.4)
#define SPEED_H_FAST		FIX32(0.8)
#define SPEED_V_SLOW		FIX32(0.2)
#define SPEED_V_FAST    	FIX32(0.4)

#define BOOST		0x10

#define LIGHTCYCLE_WIDTH 24
#define LIGHTCYCLE_HEIGHT 24

static void calculateNextMovement(LightCycle* lightCycle);

void initLightCycle(LightCycle* lightCycle) {

    lightCycle->health = ALIVE;
    lightCycle->finished = FALSE;

    // position
    lightCycle->movable.object.pos.x = FIX32(496);
    lightCycle->movable.object.pos.y = FIX32(12);
    lightCycle->movable.object.size.x = LIGHTCYCLE_WIDTH;
    lightCycle->movable.object.size.y = LIGHTCYCLE_HEIGHT;
    lightCycle->movable.object.box.w = lightCycle->movable.object.size.x;
    lightCycle->movable.object.box.h = lightCycle->movable.object.size.y;

    updateMovableBox(&lightCycle->movable);

    // movement
    lightCycle->movable.direction = DOWN;
    lightCycle->movable.mov.x = -SPEED_H_SLOW;
    lightCycle->movable.mov.y = SPEED_V_SLOW;

    // lightCycle->movable.mov.x = 0;
    // lightCycle->movable.mov.y = 0;

    lightCycle->movable.turn = 0;

    // next crossing
    lightCycle->movable.nextCrossing.x = 504;
    lightCycle->movable.nextCrossing.y = 32;

    V2s32 posInView = positionInView(&lightCycle->movable.object.box.min);
    V2s32 posInScreen = positionInScreen(&posInView);

    lightCycle->sprite = SPR_addSprite(&sprite_lightcycle_flynn, //
        posInScreen.x, posInScreen.y, //
        TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setAnim(lightCycle->sprite, ANIM_DOWN);
}

void moveLightCycle(LightCycle* lightCycle) {

    calculateNextMovement(lightCycle);
    updatePosition(&lightCycle->movable);

    V2s32 posInView = positionInView(&lightCycle->movable.object.box.min);
    V2s32 posInScreen = positionInScreen(&posInView);

    SPR_setPosition(lightCycle->sprite, posInScreen.x, posInScreen.y);
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
