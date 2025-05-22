/*
 * Filename: lightcycle.h
 * Created: August 21st 2023
 * Author: diegomtassis
 */


#ifndef INC_LIGHTCYCLE_H_
#define INC_LIGHTCYCLE_H_

#include "genesis.h"

#include "elements.h"
#include "grid_physics.h"

typedef struct LightCycle LightCycle;

struct LightCycle {
    GridMovable movable;
    u8 health;
    bool justBegunDerezzing;
    bool boost;
    bool airborne;
    bool immunity;
};

void CYCLE_init(LightCycle* lightCycle);

void CYCLE_step(LightCycle* lightCycle);

void CYCLE_crash(LightCycle* lightCycle);

void CYCLE_setRenderInfo(LightCycle* lightCycle);

#endif /* INC_LIGHTCYCLE_H_ */
