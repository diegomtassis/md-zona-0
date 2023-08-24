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

void setupCamera(s32 x, s32 y, u16 width, u16 height) {

    cameraView.min.x = x;
    cameraView.min.y = y;
    cameraView.w = width;
    cameraView.h = height;

    updateBoxMax(&cameraView);

    lockingOffsetH = width >> 1;
    lockingOffsetV = height >> 1;

    subjectLockedH = FALSE;
    subjectLockedV = FALSE;
}

void cameraFocus(Box_s32 * objectToTrack) {

    subject = objectToTrack;
}

void updateCamera() {

    s32 normalizedX = subject->min.x - lockingOffsetH;
    if (subjectLockedH) {
        cameraView.min.x = normalizedX;
        KLog("Already locked H");

    } else {
        // Subject not locked. If it's near the view center don't do anything, but if it's far from the center, follow it.
        if (cameraView.min.x == normalizedX) {
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

    s32 normalizedY = subject->min.y - lockingOffsetV;
    if (subjectLockedV) {
        cameraView.min.y = normalizedY;
        KLog("Already locked V");

    } else {
        if (cameraView.min.y == normalizedY) {
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

    updateBoxMax(&cameraView);
}

V2s32 positionInView(Box_s32 *subject) {

    V2s32 relative = {
        .x = subject->min.x - cameraView.min.x,
        .y = subject->min.y - cameraView.min.y
    };

    return relative;
}

