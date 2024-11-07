/*
 * Filename: camera.h
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <genesis.h>
#include "fwk/physics.h"

#define CAMERA_VIEW_WIDTH		288
#define CAMERA_VIEW_HEIGHT		160

void setupCamera(s32 mapWidth, s32 mapHeight, s32 mapInitX, s32 mapInitY);
void cameraFocus(Box_s32 *);
void updateCamera();

extern Box_s32 cameraView;

V2s32 mapToView(V2s32 *);

#endif /* INC_CAMERA_H_ */
