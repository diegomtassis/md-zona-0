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
bool c_pushed;

static void readPlayerInput();

void PLAYER_init() { CYCLE_init(&lightCycle); }
void PLAYER_release() { CYCLE_release(&lightCycle); }

void PLAYER_act() {

    if (lightCycle.movable.object.health & DEREZZED) {
        return;
    }

    lightCycle.movable.object.justBegunDerezzing = FALSE;
    lightCycle.movable.object.viewIsDirty = FALSE;

    if (lightCycle.movable.object.health & ALIVE) {
        readPlayerInput();
        CYCLE_step(&lightCycle);
        if (lightCycle.movable.object.justBegunDerezzing) {
            CYCLE_crash(&lightCycle);
        }
    } else {
        // Already DEREZZING
        if (SPR_isAnimationDone(lightCycle.movable.object.sprite)) {
            lightCycle.movable.object.health = DEREZZED;
        }
    }

    if (lightCycle.movable.object.viewIsDirty) {
        // kprintf("P1: ACT");
        CYCLE_setRenderInfo(&lightCycle);
    }
}

static void readPlayerInput() {

    u16 value = JOY_readJoypad(JOY_1);
    if (value & BUTTON_DIR) {
        if (!dir_pushed) {
            lightCycle.movable.turn = value;
        }
        dir_pushed = TRUE;
    } else {
        dir_pushed = FALSE;
    }

    if (value & BUTTON_C) {
        if (!c_pushed) {
            // detect flank
            lightCycle.movable.object.justBegunDerezzing = TRUE;
        }
        c_pushed = TRUE;
    } else {
        c_pushed = FALSE;
    }
};
