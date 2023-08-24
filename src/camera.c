/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "fwk/physics.h"

#include <kdebug.h>

#define MAX_GAP_H		120
#define MAX_GAP_V		110

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
            // s32 gapH = cameraView.min.x - normalizedX;
            // if (gapH > MAX_GAP_H) {
            //     cameraView.min.x += 8;
            // } else if (gapH < -MAX_GAP_H) {
            //     cameraView.min.x -= 8;
            // }
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

