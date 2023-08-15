/*
 * Filename: player.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "player.h"

#include <genesis.h>

#define ANIM_LEFT		    1
#define ANIM_LEFT_FLIP_H	1

#define ANIM_RIGHT		    0
#define ANIM_RIGHT_FLIP_H	1

#define ANIM_UP	    	    1
#define ANIM_UP_FLIP_V	   	0

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

static void handleInputPlayer();
static void moveLightCycle();
static void drawLightCycle();

void playerActs() {

    if (!(lightCycle.health & ALIVE) || lightCycle.finished) {
        return;
    }

    handleInputPlayer();
    moveLightCycle();

    drawLightCycle();
}

static void handleInputPlayer() {};

static void moveLightCycle() {};

static void drawLightCycle() {};
