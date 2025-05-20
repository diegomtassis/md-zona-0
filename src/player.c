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
bool b_flank;

static void readPlayerInput();

static void killPlayer();

void PLAYER_init() { CYCLE_init(&lightCycle); }

void PLAYER_act() {

    if (!(lightCycle.health & ALIVE) || lightCycle.finished) {
        return;
    }

    if (b_flank && (lightCycle.health & ALIVE)) {
        
        b_flank = FALSE;
        killPlayer();
        return;
    }

    readPlayerInput();
    CYCLE_move(&lightCycle);
    if (lightCycle.movable.spritePosJustChanged) {
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
            b_flank = TRUE;
        }
        b_pushed = TRUE;
    } else {
        b_pushed = FALSE;
    }
};

static void killPlayer() {
    lightCycle.health = DEAD;
    lightCycle.finished = TRUE;
};
