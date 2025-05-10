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

	V2s16 gridPos;
	V2s16 mapPos;
    Sprite* sprite;
	V2s16 spritePosInMap;

} GridObject;

typedef struct {
	GridObject object;
	u16 speed;
	u8 direction;
	u16 gridPosDelta;
	V2s16 mapPrevCrossing;
	u8 turn;
	bool justTurned;
	bool updateSprite;

} GridMovable;

void updatePosition(GridMovable* movable);

#endif /* INC_GRID_PHYSICS_H_ */
