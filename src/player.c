/*
 * Filename: player.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "player.h"

#include <genesis.h>

#include "sprites.h"


#define ANIM_LEFT		    1
#define ANIM_LEFT_FLIP_H	1

#define ANIM_RIGHT		    0
#define ANIM_RIGHT_FLIP_H	1

#define ANIM_UP	    	    1
#define ANIM_UP_FLIP_H	   	0

#define ANIM_DOWN       	0
#define ANIM_DOWN_FLIP_H	0

#define SPEED_ZERO			FIX16_0
#define SPEED_H_SLOW		FIX16(0.5)
#define SPEED_H_FAST		FIX16(1)
#define SPEED_V_SLOW		FIX16(0.25)
#define SPEED_V_FAST    	FIX16(0.5)

#define UP			0x01
#define DOWN		0x02
#define RIGHT		0x04
#define LEFT		0x10
#define BOOST		0x20

#define PLAYER_HEIGHT 24
#define PLAYER_WIDTH 24

struct LightCycle lightCycle;

bool joy_pushed;
bool joy_flank;
u16 direction = 0;

static void handleInputPlayer();
static void moveLightCycle();
static void drawLightCycle();

void initPlayer() {

    PAL_setPalette(PAL1, palette_sprites.data, DMA);

    SPR_init();

    Sprite* p1_cycle = SPR_addSprite(&flynn_sprite, 64, 15,
        TILE_ATTR(PAL1, TRUE, FALSE, FALSE));

    SPR_setAnim(p1_cycle, ANIM_DOWN);

    lightCycle.health = ALIVE;
    lightCycle.finished = FALSE;
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
}

static void handleInputPlayer() {

    u8 joy = JOY_1;
    u16 value = JOY_readJoypad(joy);

    if (value & BUTTON_DIR) {
        if (!joy_pushed) {
            // detect flank
            joy_flank = TRUE;
        }
        joy_pushed = TRUE;
        direction = value;

    } else {
        joy_pushed = FALSE;
    }
};

static void moveLightCycle() {


};

static void drawLightCycle() {

    if (joy_flank) {
        joy_flank = FALSE;

        if (direction & BUTTON_DOWN) {
            SPR_setAnim(lightCycle.sprite, ANIM_DOWN);
            SPR_setHFlip(lightCycle.sprite, ANIM_DOWN_FLIP_H);

        } else if (direction & BUTTON_UP) {
            SPR_setAnim(lightCycle.sprite, ANIM_UP);
            SPR_setHFlip(lightCycle.sprite, ANIM_UP_FLIP_H);

        } else if (direction & BUTTON_LEFT) {
            SPR_setAnim(lightCycle.sprite, ANIM_LEFT);
            SPR_setHFlip(lightCycle.sprite, ANIM_LEFT_FLIP_H);

        } else if (direction & BUTTON_RIGHT) {
            SPR_setAnim(lightCycle.sprite, ANIM_RIGHT);
            SPR_setHFlip(lightCycle.sprite, ANIM_RIGHT_FLIP_H);
        }
    }
};
