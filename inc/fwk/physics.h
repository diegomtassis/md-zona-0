/*
 * physics.h
 *
 *  Created on: Apr 19, 2019
 *      Author: diegomtassis
 */

#ifndef INC_PHYSICS_H_
#define INC_PHYSICS_H_

#include <genesis.h>

#define IN_BETWEEN		0x01
#define OVERLAPPED		0x02
#define TO_THE_LEFT		0x04
#define TO_THE_RIGHT	0x08
#define ABOVE			0x10
#define UNDER			0x20

typedef struct {
	V2s32 min;
	V2s32 max;
	u16 w;
	u16 h;
} Box_s32;

void updateBoxMax(Box_s32* box);

bool contained(V2s16 subject_pos, const Box_s32* object_box);
bool overlap(const Box_s32* subject_box, const Box_s32* object_box);

bool isAboveBaseUpwardProjection(const Box_s32* subject_box, const Box_s32* object_box);
bool shareBase(const Box_s32* subject_box, const Box_s32* object_box);

bool hitAbove(const Box_s32* subject_box, const Box_s32* object_box);
bool hitUnder(const Box_s32* subject_box, const Box_s32* object_box);
bool hitLeft(const Box_s32* subject_box, const Box_s32* object_box);
bool hitRight(const Box_s32* subject_box, const Box_s32* object_box);

s16 adjacentYAbove(const Box_s32* subject_box, const Box_s32* object_box);
s16 adjacentYUnder(const Box_s32* subject_box, const Box_s32* object_box);
s16 adjacentXOnTheLeft(const Box_s32* subject_box, const Box_s32* object_box);
s16 adjacentXOnTheRight(const Box_s32* subject_box, const Box_s32* object_box);

u8 axisXBoxRelativePos(const Box_s32*, const Box_s32*);
u8 axisYBoxRelativePos(const Box_s32*, const Box_s32*);
u8 axisXPxRelativePos(s32, const Box_s32*);
u8 axisYPxRelativePos(s32, const Box_s32*);

#endif /* INC_PHYSICS_H_ */
