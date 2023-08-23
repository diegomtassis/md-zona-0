/*
 * Filename: grid_physics.h
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_PHYSICS_H_
#define INC_GRID_PHYSICS_H_

#include <genesis.h>

#include <fwk/physics.h>

#define UP			0x01
#define DOWN		0x02
#define LEFT		0x04
#define RIGHT		0x08

typedef struct {

	V2f32 pos;
	V2u16 size;
	Box_s32 box;

} GridObject;

typedef struct {
	GridObject object;
	V2f32 mov;
	u8 direction;
	u8 turn;
	bool justTurned;
	V2s32 nextCrossing;

} GridMovable;

void updatePosition(GridMovable* movable);
void handleCrossing(GridMovable* movable);

void updateMovableBox(GridMovable* movable);

Box_s32 targetBox(const GridMovable* subject);
Box_s32 targetHBox(const GridMovable* subject);
Box_s32 targetVBox(const GridMovable* subject);

#endif /* INC_GRID_PHYSICS_H_ */
