/*
 * Filename: camera.h
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <genesis.h>

#include "fwk/physics.h"

extern Box_s32 cameraView;

void setupCamera(s32 x, s32 y, u16 width, u16 height);
void updateCamera();

V2u16 cordsInView(Box_s32 *);

#endif /* INC_CAMERA_H_ */
