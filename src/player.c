/*
 * Filename: player.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "player.h"

#include <genesis.h>
#include <lightcycle.h>

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
#define SPEED_H_SLOW		FIX16(1)
#define SPEED_H_FAST		FIX16(2)
#define SPEED_V_SLOW		FIX16(0.5)
#define SPEED_V_FAST    	FIX16(1)

#define UP			0x01
#define DOWN		0x02
#define LEFT		0x04
#define RIGHT		0x08
#define BOOST		0x10

#define PLAYER_HEIGHT 24
#define PLAYER_WIDTH 24

Player player;

LightCycle lightCycle;

bool joy_pushed;
u8 turn = 0;

static void handleInputPlayer();
static void moveLightCycle();
static void drawLightCycle();

static void calculateNextMovement();
static void updatePosition();

void initPlayer() {

    lightCycle.health = ALIVE;
    lightCycle.finished = FALSE;
    lightCycle.gridMovable.direction = DOWN;
    lightCycle.gridMovable.object.pos.x = FIX16(64);
    lightCycle.gridMovable.object.pos.y = FIX16(15);
    lightCycle.gridMovable.object.size.x = 24;
    lightCycle.gridMovable.object.size.y = 24;

    // sprite
    PAL_setPalette(PAL1, palette_sprites.data, DMA);

    SPR_init();

    Sprite* p1_cycle = SPR_addSprite(&flynn_sprite, fix16ToInt(lightCycle.gridMovable.object.pos.x), fix16ToInt(lightCycle.gridMovable.object.pos.y), TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    SPR_setAnim(p1_cycle, ANIM_DOWN);

    lightCycle.sprite = p1_cycle;

    SPR_update();
}

void playerActs() {

    if (!(lightCycle.health & ALIVE) || lightCycle.finished) {
        return;
    }

    handleInputPlayer();
    moveLightCycle();

    drawLightCycle();

    // reset turn
    turn = 0;
}

static void handleInputPlayer() {

    u16 value = JOY_readJoypad(player.joystick);
    if (value & BUTTON_DIR) {
        if (!joy_pushed) {
            turn = value;
        }
        joy_pushed = TRUE;

    } else {
        joy_pushed = FALSE;
    }
};

static void moveLightCycle() {

    calculateNextMovement();
    updatePosition();
    SPR_setPosition(lightCycle.sprite, fix16ToInt(lightCycle.gridMovable.object.pos.x), fix16ToInt(lightCycle.gridMovable.object.pos.y));
}

static void calculateNextMovement() {

    if (turn) {
        lightCycle.gridMovable.direction = turn;
    }

    if (lightCycle.gridMovable.direction & DOWN) {
        lightCycle.gridMovable.mov.x = -SPEED_H_SLOW;
        lightCycle.gridMovable.mov.y = SPEED_V_SLOW;

    } else if (lightCycle.gridMovable.direction & UP) {
        lightCycle.gridMovable.mov.x = SPEED_H_SLOW;
        lightCycle.gridMovable.mov.y = -SPEED_V_SLOW;

    } else if (lightCycle.gridMovable.direction & LEFT) {
        lightCycle.gridMovable.mov.x = -SPEED_H_SLOW;
        lightCycle.gridMovable.mov.y = -SPEED_V_SLOW;

    } else if (lightCycle.gridMovable.direction & RIGHT) {
        lightCycle.gridMovable.mov.x = SPEED_H_SLOW;
        lightCycle.gridMovable.mov.y = SPEED_V_SLOW;
    }
}

static void updatePosition() {

    lightCycle.gridMovable.object.pos.x += lightCycle.gridMovable.mov.x;
    lightCycle.gridMovable.object.pos.y += lightCycle.gridMovable.mov.y;

    // update box
    updateBox(&lightCycle.gridMovable.object);
}

static void drawLightCycle() {

    if (turn) {
        if (turn & BUTTON_DOWN) {
            SPR_setAnim(lightCycle.sprite, ANIM_DOWN);
            SPR_setHFlip(lightCycle.sprite, ANIM_DOWN_FLIP_H);

        } else if (turn & BUTTON_UP) {
            SPR_setAnim(lightCycle.sprite, ANIM_UP);
            SPR_setHFlip(lightCycle.sprite, ANIM_UP_FLIP_H);

        } else if (turn & BUTTON_LEFT) {
            SPR_setAnim(lightCycle.sprite, ANIM_LEFT);
            SPR_setHFlip(lightCycle.sprite, ANIM_LEFT_FLIP_H);

        } else if (turn & BUTTON_RIGHT) {
            SPR_setAnim(lightCycle.sprite, ANIM_RIGHT);
            SPR_setHFlip(lightCycle.sprite, ANIM_RIGHT_FLIP_H);
        }
    }
};
