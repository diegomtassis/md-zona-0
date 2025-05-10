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

// Map
static V2s16 mapSize;

// Camera
Box_s16 cameraView;
static V2s16 subjectOffsetInView;

// Subject
static GridObject *subject;
static Box_s16 subjectBox;

static bool subjectLockedH;
static bool subjectLockedV;

static void focus();

static void updateBox16Max(Box_s16 *box);

void setupCamera(s16 mapWidth, s16 mapHeight) {

    mapSize.x = mapWidth;
    mapSize.y = mapHeight;

    cameraView.w = CAMERA_VIEW_WIDTH;
    cameraView.h = CAMERA_VIEW_HEIGHT;

    subjectLockedH = FALSE;
    subjectLockedV = FALSE;
}
void cameraFocusOn(GridObject *objectToTrack) {

    // subject
    subject = objectToTrack;

    // box for the subject
    subjectBox.w = 24;
    subjectBox.h = 24;
    subjectBox.min.x = subject->spritePosInMap.x;
    subjectBox.min.y = subject->spritePosInMap.y;
    updateBox16Max(&subjectBox);

    // adapt camera offset to subject dimensions
    V2s16 offset = {.x = (cameraView.w - subjectBox.w) / 2, .y = (cameraView.h - subjectBox.h) / 2};
    subjectOffsetInView = offset;
    kprintf("CAM: center offset in view: x:%d, y:%d", subjectOffsetInView.x, subjectOffsetInView.y);

    V2s16 subjectOffsetInScreeen = viewToScreen(&subjectOffsetInView);
    kprintf("CAM: center offset in screen: x:%d, y:%d", subjectOffsetInScreeen.x, subjectOffsetInScreeen.y);

    // update camera position
    // V2s16 viewPosInMap = {.x = max(subjectBox.min.x - subjectOffsetInView.x, 0),
    //                       .y = max(subjectBox.min.y - subjectOffsetInView.y, 0)};

    V2s16 viewPosInMap = {.x = subjectBox.min.x - subjectOffsetInView.x,
                          .y = subjectBox.min.y - subjectOffsetInView.y};

    cameraView.min.x = viewPosInMap.x;
    cameraView.min.y = viewPosInMap.y;
    kprintf("CAM: view pos in map: x:%d, y:%d", cameraView.min.x, cameraView.min.y);

    updateBox16Max(&cameraView);

    subjectLockedH = TRUE;
    subjectLockedV = TRUE;
}

void updateCamera() {

    // focus();

    V2s16 subjectPosInView = mapToView(&subject->spritePosInMap);
    kprintf("CAM: subject sprite pos in view: x:%d, y:%d", subjectPosInView.x, subjectPosInView.y);

    V2s16 subjectPosInScreen = viewToScreen(&subjectPosInView);
    kprintf("CAM: subject sprite pos in screen: x:%d, y:%d", subjectPosInScreen.x, subjectPosInScreen.y);

    SPR_setPosition(subject->sprite, subjectPosInScreen.x, subjectPosInScreen.y);

    V2s16 screenPosInMap = screenInMap(&cameraView.min);
    kprintf("CAM: screen pos in map: x:%d, y:%d", screenPosInMap.x, screenPosInMap.y);
    scrollGrid(screenPosInMap);
}

static void focus() {

    subjectBox.min.x = subject->mapPos.x;
    subjectBox.min.y = subject->mapPos.y;

    s16 normalizedMinX = subjectBox.min.x - subjectOffsetInView.x;
    if (subjectLockedH) {
        // Already locked H
        cameraView.min.x = normalizedMinX;

    } else {
        if (cameraView.min.x == normalizedMinX) {
            subjectLockedH = TRUE;
            // Just locked H

        } else {
            // Not locked H
            s16 leftPadded = subjectBox.min.x - MIN_PADDING_H;
            if (leftPadded < cameraView.min.x) {
                cameraView.min.x = leftPadded;
            } else {
                s16 rightPadded = subjectBox.max.x + MIN_PADDING_H;
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

    s16 normalizedMinY = subjectBox.min.y - subjectOffsetInView.y;
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
            s16 upPadded = subjectBox.min.y - MIN_PADDING_V;
            if (upPadded < cameraView.min.y) {
                cameraView.min.y = upPadded;
            } else {
                s16 downPadded = subjectBox.max.y + MIN_PADDING_V;
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

    V2s16 relative = {.x = subject->x - cameraView.min.x, .y = subject->y - cameraView.min.y};

    return relative;
}

static void updateBox16Max(Box_s16 *box) {

    box->max.x = box->min.x + box->w - 1;
    box->max.y = box->min.y + box->h - 1;
}
