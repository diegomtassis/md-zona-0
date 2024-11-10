/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "fwk/physics.h"
#include "elements.h"

#include <kdebug.h>

#define MIN_PADDING_H		32
#define MIN_PADDING_V		24

 // Map
static V2s16 mapSize;

// Camera
Box_s16 cameraView;

static u16 centerOffsetH;
static u16 centerOffsetV;

// Subject
static Box_s16 *subject;

static bool subjectLockedH;
static bool subjectLockedV;

static void updateBox16Max(Box_s16 *box);

void setupCamera( //
    s16 mapWidth, s16 mapHeight, s16 mapInitPosX, s16 mapInitPosY) {

    mapSize.x = mapWidth;
    mapSize.y = mapHeight;

    centerOffsetH = CAMERA_VIEW_WIDTH / 2;
    centerOffsetV = CAMERA_VIEW_HEIGHT / 2;

    cameraView.w = CAMERA_VIEW_WIDTH;
    cameraView.h = CAMERA_VIEW_HEIGHT;
    cameraView.min.x = mapInitPosX;
    cameraView.min.y = mapInitPosY;
    updateBox16Max(&cameraView);

    subjectLockedH = FALSE;
    subjectLockedV = FALSE;
}
void cameraFocus(Box_s16 * objectToTrack) {

    subject = objectToTrack;
}

void updateCamera() {

    s16 normalizedMinX = subject->min.x - centerOffsetH;
    if (subjectLockedH) {
        // Already locked H
        cameraView.min.x = normalizedMinX;

    } else {
        if (cameraView.min.x == normalizedMinX) {
            subjectLockedH = TRUE;
            // Just locked H

        } else {
            // Not locked H
            s16 leftPadded = subject->min.x - MIN_PADDING_H;
            if (leftPadded < cameraView.min.x) {
                cameraView.min.x = leftPadded;
            } else {
                s16 rightPadded = subject->max.x + MIN_PADDING_H;
                if (rightPadded > cameraView.max.x) {
                    cameraView.min.x = rightPadded - cameraView.w;
                }
            }
        }
    }

    if (cameraView.min.x < 0) {
        // Unlocking H
        cameraView.min.x = 0;
        subjectLockedH = FALSE;

    } else if (cameraView.max.x > mapSize.x) {
        // Unlocking H
        cameraView.min.x = mapSize.x - cameraView.w;
        subjectLockedH = FALSE;
    }

    s16 normalizedMinY = subject->min.y - centerOffsetV;
    if (subjectLockedV) {

        // Already locked V
        if (normalizedMinY < 0) {
            // Unlocking V
            cameraView.min.y = 0;
            subjectLockedV = FALSE;
        } else {
            cameraView.min.y = normalizedMinY;
        }

    } else {
        if (cameraView.min.y == normalizedMinY) {
            subjectLockedV = TRUE;
            // Just locked V

        } else {
            // Unlocked V
            s16 upPadded = subject->min.y - MIN_PADDING_V;
            if (upPadded < cameraView.min.y) {
                cameraView.min.y = upPadded;
            } else {
                s16 downPadded = subject->max.y + MIN_PADDING_V;
                if (downPadded > cameraView.max.y) {
                    cameraView.min.y = downPadded - cameraView.h;
                }
            }
        }
    }

    if (cameraView.min.y < 0) {
        // Unlocking V
        cameraView.min.y = 0;
        subjectLockedV = FALSE;

    } else if (cameraView.max.y > mapSize.y) {
        // Unlocking V
        cameraView.min.y = mapSize.y - cameraView.h;
        subjectLockedV = FALSE;
    }

    updateBox16Max(&cameraView);
}

V2s16 mapToView(V2s16 *subject) {

    V2s16 relative = {
        .x = subject->x - cameraView.min.x,
        .y = subject->y - cameraView.min.y
    };

    return relative;
}

static void updateBox16Max(Box_s16* box) {

	box->max.x = box->min.x + box->w - 1;
	box->max.y = box->min.y + box->h - 1;
}
