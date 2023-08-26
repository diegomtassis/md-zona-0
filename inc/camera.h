/*
 * Filename: camera.h
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <genesis.h>
#include "fwk/physics.h"

void setupCamera(s32 screenInitX, s32 screenInitY, u16 width, u16 height, s32 planInitX, s32 planInitY, s32 planMaxX, s32 planMaxY);
void cameraFocus(Box_s32 *);
void updateCamera();

extern Box_s32 cameraView;

V2s32 positionInScreen(Box_s32 *);

#endif /* INC_CAMERA_H_ */
