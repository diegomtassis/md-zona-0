/*
 * Filename: grid_physics.h
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_PHYSICS_H_
#define INC_GRID_PHYSICS_H_

#include <genesis.h>

#include <fwk/physics.h>

typedef struct {

	V2f16 pos;
	V2u16 size;
	Box_s16 box;

	V2u16 centerOffset;
	V2u16 viewPosition;

} GridObject;

typedef struct {
	GridObject object;
	V2f16 mov;
	u8 direction;
	V2u16 nextCrossing;

} GridMovable;

void updateBox(GridMovable* movable);


Box_s16 targetBox(const GridMovable* subject);
Box_s16 targetHBox(const GridMovable* subject);
Box_s16 targetVBox(const GridMovable* subject);

#endif /* INC_GRID_PHYSICS_H_ */
