/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "fwk/physics.h"

#include <kdebug.h>

#define MIN_PADDING_H		16
#define MIN_PADDING_V		8

Box_s32 cameraView;

static Box_s32 *subject;

static bool subjectLockedH;
static bool subjectLockedV;

static u16 lockingOffsetH;
static u16 lockingOffsetV;

static V2s32 screenMaxBounds;

void setupCamera(s32 x, s32 y, u16 width, u16 height, s32 maxX, s32 maxY) {

    cameraView.min.x = x;
    cameraView.min.y = y;
    cameraView.w = width;
    cameraView.h = height;

    updateBoxMax(&cameraView);

    lockingOffsetH = width >> 1;
    lockingOffsetV = height >> 1;

    subjectLockedH = FALSE;
    subjectLockedV = FALSE;

    screenMaxBounds.x = maxX;
    screenMaxBounds.y = maxY;
}

void cameraFocus(Box_s32 * objectToTrack) {

    subject = objectToTrack;
}

void updateCamera() {

    s32 normalizedMinX = subject->min.x - lockingOffsetH;
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

    } else if (cameraView.max.x > screenMaxBounds.x) {
        KLog("Unlocking H");
        cameraView.min.x = screenMaxBounds.x - cameraView.w;
        subjectLockedH = FALSE;
    }

    s32 normalizedMinY = subject->min.y - lockingOffsetV;
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

    } else if (cameraView.max.y > screenMaxBounds.y) {
        KLog("Unlocking V");
        cameraView.min.y = screenMaxBounds.y - cameraView.h;
        subjectLockedV = FALSE;
    }

    updateBoxMax(&cameraView);
}

V2s32 positionInView(Box_s32 *subject) {

    V2s32 relative = {
        .x = subject->min.x - cameraView.min.x,
        .y = subject->min.y - cameraView.min.y
    };

    return relative;
}

