/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "grid.h"
#include "fwk/physics.h"

Box_s32 cameraView;

void setupCamera(s32 x, s32 y, u16 width, u16 height) {

    cameraView.min.x = x;
    cameraView.min.y = y;
    cameraView.w = width;
    cameraView.h = height;

    updateBoxMax(&cameraView);
}

void updateCamera() {

    updateBoxMax(&cameraView);
}

V2u16 cordsInView(Box_s32 *subject) {

    V2u16 relative = {
        .x = subject->min.x - cameraView.min.x,
        .y = subject->min.y - cameraView.min.y
    };

    return relative;
}

