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

static bool dir_pushed;
static bool b_pushed;

static u8 turnTo;

static void readPlayerInput();

void PLAYER_init() { CYCLE_init(&lightCycle); }
void PLAYER_release() { CYCLE_release(&lightCycle); }

void PLAYER_act() {

    readPlayerInput();
    CYCLE_act(&lightCycle, turnTo, b_pushed);
}

static void readPlayerInput() {

    u16 value = JOY_readJoypad(JOY_1);
    if (value & BUTTON_DIR) {
        if (!dir_pushed) {
            turnTo = value & BUTTON_DIR;
        }
        dir_pushed = TRUE;
    } else {
        dir_pushed = FALSE;
    }

    if (value & BUTTON_B) {
        b_pushed = TRUE;
    } else {
        b_pushed = FALSE;
    }
};
