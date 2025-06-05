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
    bool boost;
    bool airborne;
    bool immunity;
    TrailSegmentDefinition trailDef;
};

void CYCLE_init(LightCycle *lightCycle);

void CYCLE_act(LightCycle *lightCycle, u8 turnTo, bool boost);

void CYCLE_release(LightCycle *lightCycle);

#endif /* INC_LIGHTCYCLE_H_ */
