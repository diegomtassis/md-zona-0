/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "grid.h"
#include "fwk/physics.h"

Box_s16 cameraView;

void setupCamera(u16 x, u16 y, u16 width, u16 height) {

    cameraView.min.x = x;
    cameraView.min.y = y;
    cameraView.w = width;
    cameraView.h = height;

    updateBoxMax(&cameraView);
}

void updateCamera() {

    updateBoxMax(&cameraView);
}

V2u16 cordsInView(Box_s16 *subject) {

    V2u16 relative = {
        .x = subject->min.x - cameraView.min.x,
        .y = subject->min.y - cameraView.min.y
    };

    return relative;
}

