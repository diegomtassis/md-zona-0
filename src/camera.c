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
static Box_s16 subjectBox;

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

    // subject
    subject = objectToTrack;

    // box for the subject
    subjectBox.w = objectToTrack->sprite->definition->w;
    subjectBox.h = objectToTrack->sprite->definition->h;

    // adapt camera offset to subject dimensions
    subjectOffsetInView.x = (cameraView.w - subjectBox.w) / 2;
    subjectOffsetInView.y = (cameraView.h - subjectBox.h) / 2;

    // kprintf("CAM: center offset in view: x:%d, y:%d", subjectOffsetInView.x, subjectOffsetInView.y);
}

void CAM_update() {

    focus();
    updatePositionInScreen(subject);
    scrollMapToScreen();
}

static void focus() {

    // kprintf("CAM: focusing...");

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

    // kprintf("CAM: view pos in map: x:%d, y:%d", cameraView.min.x, cameraView.min.y);

    updateBox16Max(&cameraView);
}

static void updatePositionInScreen(GridObject *object) {

    V2s16 subjectPosInView = mapToView(&subject->spritePosInMap);
    // kprintf("CAM: subject sprite pos in view: x:%d, y:%d", subjectPosInView.x, subjectPosInView.y);

    V2s16 subjectPosInScreen = SCREEN_viewToScreen(&subjectPosInView);
    // kprintf("CAM: subject sprite pos in screen: x:%d, y:%d", subjectPosInScreen.x, subjectPosInScreen.y);

    SPR_setPosition(subject->sprite, subjectPosInScreen.x, subjectPosInScreen.y);
}

static void scrollMapToScreen() {

    V2s16 screenPosInMap = SCREEN_screenInMap(&cameraView.min);
    // kprintf("CAM: screen pos in map: x:%d, y:%d", screenPosInMap.x, screenPosInMap.y);
    GRID_scroll(screenPosInMap);
}

V2s16 mapToView(V2s16 *subject) {

    V2s16 posInView = {.x = subject->x - cameraView.min.x, .y = subject->y - cameraView.min.y};

    return posInView;
}

static void updateBox16Max(Box_s16 *box) {

    box->max.x = box->min.x + box->w - 1;
    box->max.y = box->min.y + box->h - 1;
}
