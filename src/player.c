/*
 * Filename: player.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "player.h"

#include <genesis.h>

#include "gfx_lightcycles.h"

#include "grid_physics.h"

Player player;

LightCycle lightCycle;

bool joy_pushed;

static void handleInputPlayer();

void initPlayer() {

    initLightCycle(&lightCycle);
}

void playerActs() {

    if (!(lightCycle.health & ALIVE) || lightCycle.finished) {
        return;
    }

    handleInputPlayer();
    moveLightCycle(&lightCycle);
    drawLightCycle(&lightCycle);
}

static void handleInputPlayer() {

    u16 value = JOY_readJoypad(player.joystick);
    if (value & BUTTON_DIR) {
        if (!joy_pushed) {
            lightCycle.movable.turn = value;
        }
        joy_pushed = TRUE;

    } else {
        joy_pushed = FALSE;
    }
};
