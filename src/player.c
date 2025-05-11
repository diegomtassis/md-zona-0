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

void PLAYER_init() { CYCLE_init(&lightCycle); }

void PLAYER_act() {

    if (!(lightCycle.health & ALIVE) || lightCycle.finished) {
        return;
    }

    handleInputPlayer();
    CYCLE_move(&lightCycle);
    if (lightCycle.movable.updateSprite) {
        CYCLE_updateRenderInfo(&lightCycle);
    }
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
