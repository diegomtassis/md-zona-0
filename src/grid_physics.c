/*
 * Filename: grid_physics.c
 * Created: August 18th 2023
 * Author: diegomtassis
 */

#include "grid_physics.h"

#include "fwk/physics.h"


void updateBox(GridMovable* movable) {

	movable->object.box.min.x = fix16ToInt(movable->object.pos.x);
	movable->object.box.min.y = fix16ToInt(movable->object.pos.y);
	updateBoxMax(&movable->object.box);
}

Box_s16 targetBox(const GridMovable* movable) {

	Box_s16 box = { .w = movable->object.size.x, .h = movable->object.size.y };
	box.min.x = fix16ToInt(fix16Add(movable->object.pos.x, movable->mov.x));
	box.min.y = fix16ToInt(fix16Add(movable->object.pos.y, movable->mov.y));
	updateBoxMax(&box);

	return box;
}

Box_s16 targetHBox(const GridMovable* movable) {

	Box_s16 box = { .w = movable->object.size.x, .h = movable->object.size.y };
	box.min.x = fix16ToInt(fix16Add(movable->object.pos.x, movable->mov.x));
	box.min.y = fix16ToInt(movable->object.pos.y);
	updateBoxMax(&box);

	return box;
}

Box_s16 targetVBox(const GridMovable* movable) {

	Box_s16 box = { .w = movable->object.size.x, .h = movable->object.size.y };
	box.min.x = fix16ToInt(movable->object.pos.x);
	box.min.y = fix16ToInt(fix16Add(movable->object.pos.y, movable->mov.y));
	updateBoxMax(&box);

	return box;
}