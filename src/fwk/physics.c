/*
 * physics.c
 *
 *  Created on: Apr 19, 2019
 *      Author: diegomtassis
 */

#include "fwk/physics.h"

void updateBoxMax(Box_s32* box) {

	box->max.x = box->min.x + box->w - 1;
	box->max.y = box->min.y + box->h - 1;
}

bool contained(V2s16 subject_pos, const Box_s32* object_box) {

	return (IN_BETWEEN & axisXPxRelativePos(subject_pos.x, object_box))
			&& (IN_BETWEEN & axisYPxRelativePos(subject_pos.y, object_box));
}

bool overlap(const Box_s32* subject_box, const Box_s32* object_box) {

	return (OVERLAPPED & axisXBoxRelativePos(subject_box, object_box))
			&& (OVERLAPPED & axisYBoxRelativePos(subject_box, object_box));
}

bool isAboveBaseUpwardProjection(const Box_s32* subject_box, const Box_s32* object_box) {

	if (subject_box->max.y > object_box->max.y) {
		return FALSE;
	}

	return IN_BETWEEN & axisXPxRelativePos(subject_box->min.x + subject_box->w / 2, object_box);
}

bool shareBase(const Box_s32* subject_box, const Box_s32* object_box) {

	if (subject_box->max.y != object_box->max.y) {
		return FALSE;
	}

	return IN_BETWEEN & axisXPxRelativePos(subject_box->min.x + subject_box->w / 2, object_box);
}

bool hitAbove(const Box_s32* subject_box, const Box_s32* object_box) {

	if (OVERLAPPED & axisXBoxRelativePos(subject_box, object_box)) {
		return IN_BETWEEN & axisYPxRelativePos(subject_box->max.y + 1, object_box);
	}

	return FALSE;
}

bool hitUnder(const Box_s32* subject_box, const Box_s32* object_box) {

	if (OVERLAPPED & axisXBoxRelativePos(subject_box, object_box)) {
		return IN_BETWEEN & axisYPxRelativePos(subject_box->min.y - 1, object_box);
	}

	return FALSE;
}

bool hitLeft(const Box_s32* subject_box, const Box_s32* object_box) {

	if (OVERLAPPED & axisYBoxRelativePos(subject_box, object_box)) {
		return IN_BETWEEN & axisXPxRelativePos(subject_box->max.x + 1, object_box);
	}

	return FALSE;
}

bool hitRight(const Box_s32* subject_box, const Box_s32* object_box) {

	if (OVERLAPPED & axisYBoxRelativePos(subject_box, object_box)) {
		return IN_BETWEEN & axisXPxRelativePos(subject_box->min.x - 1, object_box);
	}

	return FALSE;
}

s16 adjacentYAbove(const Box_s32* subject_box, const Box_s32* object_box) {

	return object_box->min.y - subject_box->h;
}

s16 adjacentYUnder(const Box_s32* subject_box, const Box_s32* object_box) {

	return object_box->max.y + 1;
}

s16 adjacentXOnTheLeft(const Box_s32* subject_box, const Box_s32* object_box) {
	return object_box->min.x - subject_box->w;
}

s16 adjacentXOnTheRight(const Box_s32* subject_box, const Box_s32* object_box) {
	return object_box->max.x + 1;
}

u8 axisXBoxRelativePos(const Box_s32* subject_box, const Box_s32* object_box) {

	if (TO_THE_RIGHT & axisXPxRelativePos(subject_box->min.x, object_box)) {
		return TO_THE_RIGHT;
	}

	if (TO_THE_LEFT & axisXPxRelativePos(subject_box->max.x, object_box)) {
		return TO_THE_LEFT;
	}

	return OVERLAPPED;
}

u8 axisYBoxRelativePos(const Box_s32* subject_box, const Box_s32* object_box) {

	if (UNDER & axisYPxRelativePos(subject_box->min.y, object_box)) {
		return UNDER;
	}

	if (ABOVE & axisYPxRelativePos(subject_box->max.y, object_box)) {
		return ABOVE;
	}

	return OVERLAPPED;
}

u8 axisXPxRelativePos(s32 x_px, const Box_s32* object_box) {

	if (x_px < object_box->min.x) {
		return TO_THE_LEFT;
	}

	if (x_px > object_box->max.x) {
		return TO_THE_RIGHT;
	}

	return IN_BETWEEN;
}

u8 axisYPxRelativePos(s32 y_px, const Box_s32* object_box) {

	if (y_px < object_box->min.y) {
		return ABOVE;
	}

	if (y_px > object_box->max.y) {
		return UNDER;
	}

	return IN_BETWEEN;
}
