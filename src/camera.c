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

Box_s32 cameraView;

static Vect2D_s32 screenMinBounds;
static Vect2D_s32 screenMaxBounds;

static Box_s32 *subject;

static bool subjectLockedH;
static bool subjectLockedV;

static u16 centerOffsetH;
static u16 centerOffsetV;

static V2s32 planMaxBounds;

void setupCamera(s32 screenLeftBound, s32 screenUpperBound, u16 width, u16 height, s32 planInitX, s32 planInitY, s32 planMaxX, s32 planMaxY) {

    cameraView.min.x = planInitX;
    cameraView.min.y = planInitY;
    cameraView.w = width;
    cameraView.h = height;
    updateBoxMax(&cameraView);

    planMaxBounds.x = planMaxX;
    planMaxBounds.y = planMaxY;

    screenMinBounds.x = screenLeftBound;
    screenMinBounds.y = screenUpperBound;
    screenMaxBounds.x = screenLeftBound + width;
    screenMaxBounds.y = screenUpperBound + height;

    centerOffsetH = width >> 1;
    centerOffsetV = height >> 1;

    subjectLockedH = FALSE;
    subjectLockedV = FALSE;

}
void cameraFocus(Box_s32 * objectToTrack) {

    subject = objectToTrack;
}

void updateCamera() {

    s32 normalizedMinX = subject->min.x - centerOffsetH;
    if (subjectLockedH) {
        KLog("Already locked H");
        cameraView.min.x = normalizedMinX;

    } else {
        if (cameraView.min.x == normalizedMinX) {
            subjectLockedH = TRUE;
            KLog("Just locked H");

        } else {
            KLog("Unlocked H");
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
        KLog("Unlocking H");
        cameraView.min.x = 0;
        subjectLockedH = FALSE;

    } else if (cameraView.max.x > planMaxBounds.x) {
        KLog("Unlocking H");
        cameraView.min.x = planMaxBounds.x - cameraView.w;
        subjectLockedH = FALSE;
    }

    s32 normalizedMinY = subject->min.y - centerOffsetV;
    if (subjectLockedV) {

        KLog("Already locked V");
        if (normalizedMinY < 0) {
            KLog("Unlocking V");
            cameraView.min.y = 0;
            subjectLockedV = FALSE;
        } else {
            cameraView.min.y = normalizedMinY;
        }

    } else {
        if (cameraView.min.y == normalizedMinY) {
            subjectLockedV = TRUE;
            KLog("Just locked V");

        } else {
            KLog("Unlocked V");
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
        KLog("Unlocking V");
        cameraView.min.y = 0;
        subjectLockedV = FALSE;

    } else if (cameraView.max.y > planMaxBounds.y) {
        KLog("Unlocking V");
        cameraView.min.y = planMaxBounds.y - cameraView.h;
        subjectLockedV = FALSE;
    }

    updateBoxMax(&cameraView);
}

V2s32 positionInScreen(Box_s32 *subject) {

    V2s32 relative = {
        .x = subject->min.x - cameraView.min.x + screenMinBounds.x,
        .y = subject->min.y - cameraView.min.y + screenMinBounds.y
    };

    return relative;
}

