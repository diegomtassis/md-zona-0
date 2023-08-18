/*
 * Filename: grid_physics.h
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_PHYSICS_H_
#define INC_GRID_PHYSICS_H_

#include <genesis.h>

typedef struct {
	V2f16 pos;
	V2u16 size;
	V2f16 mov;
	Box_s16 box;

	V2s16 position;
	V2s16 globalPosition;
	V2u16 centerOffset;

} Object_f16;

#endif /* INC_GRID_PHYSICS_H_ */
