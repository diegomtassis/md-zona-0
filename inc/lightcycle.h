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
    u8 finished;
    bool boost;
    bool airborne;
    bool immunity;
};

void initLightCycle(LightCycle* lightCycle);

void moveLightCycle(LightCycle* lightCycle);

void updateLightCycleDrawInfo(LightCycle* lightCycle);

#endif /* INC_LIGHTCYCLE_H_ */
