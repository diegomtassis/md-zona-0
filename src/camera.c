/*
 * Filename: camera.c
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#include "camera.h"

#include "elements.h"
#include "fwk/physics.h"

#include "grid.h"
#include "screen.h"

#include <kdebug.h>

#define MIN_PADDING_H 32
#define MIN_PADDING_V 24

// Camera
Box_s16 cameraView;
static V2s16 subjectOffsetInView;
static V2s16 viewMinBounds;
static V2s16 viewMaxBounds;

// Map
static V2s16 mapSize;

// Subject
static GridObject *subject;
static GridObject *object;

static void focus();
static void updatePositionInScreen(GridObject *object);
static void scrollMapToScreen();

static void updateBox16Max(Box_s16 *box);

static V2s16 mapToView(V2s16 *);

void CAM_setup(s16 mapWidth, s16 mapHeight) {

    cameraView.w = CAMERA_VIEW_WIDTH;
    cameraView.h = CAMERA_VIEW_HEIGHT;

    mapSize.x = mapWidth;
    mapSize.y = mapHeight;

    viewMinBounds.x = 0;
    viewMaxBounds.y = 0;

    viewMaxBounds.x = mapSize.x - CAMERA_VIEW_WIDTH;
    viewMaxBounds.y = mapSize.y - CAMERA_VIEW_HEIGHT;
}

void CAM_track(GridObject *objectToTrack) {

    // subject to track
    subject = objectToTrack;

    // box for the subject
    Box_s16 subjectBox = {                                           //
                          .w = objectToTrack->sprite->definition->w, //
                          .h = objectToTrack->sprite->definition->h};

    // adapt camera offset to subject dimensions
    subjectOffsetInView.x = (cameraView.w - subjectBox.w) / 2;
    subjectOffsetInView.y = (cameraView.h - subjectBox.h) / 2;

    // The subject is as well an object to display
    CAM_awareOf(subject);
}

void CAM_awareOf(GridObject *objectToTrack) { object = objectToTrack; }

void CAM_still() { subject = 0; }

void CAM_update() {

    if (subject) {
        focus();
        scrollMapToScreen();
    }

    updatePositionInScreen(object);
}

static void focus() {

    // kprintf("CAM: focusing...");

    if (!subject) {
        return;
    }

    // horizontal
    s16 viewMinX = subject->spritePosInMap.x - subjectOffsetInView.x;
    if (viewMinX < viewMinBounds.x) {
        // not centered, to the left
        cameraView.min.x = viewMinBounds.x;
    } else if (viewMinX > viewMaxBounds.x) {
        // not centered, to the right
        cameraView.min.x = viewMaxBounds.x;
    } else {
        // centered
        cameraView.min.x = viewMinX;
    }

    // vertical
    s16 viewMinY = subject->spritePosInMap.y - subjectOffsetInView.y;
    if (viewMinY < viewMinBounds.y) {
        // not centered, up
        cameraView.min.y = viewMinBounds.y;
    } else if (viewMinY > viewMaxBounds.y) {
        // not centered, down
        cameraView.min.y = viewMaxBounds.y;
    } else {
        // centered
        cameraView.min.y = viewMinY;
    }

    updateBox16Max(&cameraView);
}

static void updatePositionInScreen(GridObject *object) {

    V2s16 objectPosInView = mapToView(&object->spritePosInMap);
    V2s16 objectPosInScreen = SCREEN_viewToScreen(&objectPosInView);

    SPR_setPosition(object->sprite, objectPosInScreen.x, objectPosInScreen.y);
}

static void scrollMapToScreen() {

    GRID_scroll(SCREEN_screenInMap(&cameraView.min), FALSE);
}

V2s16 mapToView(V2s16 *subject) {

    return (V2s16){.x = subject->x - cameraView.min.x, .y = subject->y - cameraView.min.y};
}

static void updateBox16Max(Box_s16 *box) {

    box->max.x = box->min.x + box->w - 1;
    box->max.y = box->min.y + box->h - 1;
}
