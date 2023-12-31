/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "fwk/physics.h"

#include <kdebug.h>

#define MIN_PADDING_H		32
#define MIN_PADDING_V		24

 // World
static V2s32 worldSize;

// Camera
Box_s32 cameraView;

static u16 centerOffsetH;
static u16 centerOffsetV;

// Subject
static Box_s32 *subject;

static bool subjectLockedH;
static bool subjectLockedV;


void setupCamera( //
    s32 worldWidth, s32 worldHeight, s32 worldInitPosX, s32 worldInitPosY) {

    worldSize.x = worldWidth;
    worldSize.y = worldHeight;

    centerOffsetH = CAMERA_VIEW_WIDTH / 2;
    centerOffsetV = CAMERA_VIEW_HEIGHT / 2;

    cameraView.w = CAMERA_VIEW_WIDTH;
    cameraView.h = CAMERA_VIEW_HEIGHT;
    cameraView.min.x = worldInitPosX;
    cameraView.min.y = worldInitPosY;
    updateBoxMax(&cameraView);

    subjectLockedH = FALSE;
    subjectLockedV = FALSE;
}
void cameraFocus(Box_s32 * objectToTrack) {

    subject = objectToTrack;
}

void updateCamera() {

    s32 normalizedMinX = subject->min.x - centerOffsetH;
    if (subjectLockedH) {
        // Already locked H
        cameraView.min.x = normalizedMinX;

    } else {
        if (cameraView.min.x == normalizedMinX) {
            subjectLockedH = TRUE;
            // Just locked H

        } else {
            // Not locked H
            s32 leftPadded = subject->min.x - MIN_PADDING_H;
            if (leftPadded < cameraView.min.x) {
                cameraView.min.x = leftPadded;
            } else {
                s32 rightPadded = subject->max.x + MIN_PADDING_H;
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

    } else if (cameraView.max.x > worldSize.x) {
        // Unlocking H
        cameraView.min.x = worldSize.x - cameraView.w;
        subjectLockedH = FALSE;
    }

    s32 normalizedMinY = subject->min.y - centerOffsetV;
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
            s32 upPadded = subject->min.y - MIN_PADDING_V;
            if (upPadded < cameraView.min.y) {
                cameraView.min.y = upPadded;
            } else {
                s32 downPadded = subject->max.y + MIN_PADDING_V;
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

    } else if (cameraView.max.y > worldSize.y) {
        // Unlocking V
        cameraView.min.y = worldSize.y - cameraView.h;
        subjectLockedV = FALSE;
    }

    updateBoxMax(&cameraView);
}

V2s32 positionInView(V2s32 *subject) {

    V2s32 relative = {
        .x = subject->x - cameraView.min.x,
        .y = subject->y - cameraView.min.y
    };

    return relative;
}
