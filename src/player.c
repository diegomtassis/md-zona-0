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

bool dir_pushed;

bool b_pushed;

static void readPlayerInput();

void PLAYER_init() { CYCLE_init(&lightCycle); }

void PLAYER_act() {

    if (lightCycle.derezzed) {
        return;
    }

    lightCycle.justDied = FALSE;
    lightCycle.movable.viewIsDirty = FALSE;

    if (lightCycle.health & ALIVE) {
        readPlayerInput();
        if (lightCycle.justDied) {
            CYCLE_crash(&lightCycle);
        } else {
            CYCLE_step(&lightCycle);
        }
    } else {
        // Already DEAD
        if (SPR_isAnimationDone(lightCycle.movable.object.sprite)) {
            lightCycle.derezzed = TRUE;
        }
    }

    if (lightCycle.movable.viewIsDirty) {
        // kprintf("P1: ACT");
        CYCLE_setRenderInfo(&lightCycle);
    }
}

static void readPlayerInput() {

    u16 value = JOY_readJoypad(player.joystick);
    if (value & BUTTON_DIR) {
        if (!dir_pushed) {
            lightCycle.movable.turn = value;
        }
        dir_pushed = TRUE;
    } else {
        dir_pushed = FALSE;
    }

    if (value & BUTTON_B) {
        if (!b_pushed) {
            // detect flank
            lightCycle.justDied = TRUE;
        }
        b_pushed = TRUE;
    } else {
        b_pushed = FALSE;
    }
};
