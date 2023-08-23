/*
 * Filename: camera.h
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <genesis.h>

#include "fwk/physics.h"

extern Box_s16 cameraView;

void setupCamera(u16 x, u16 y, u16 width, u16 height);
void updateCamera();

V2u16 cordsInView(Box_s16 *);

#endif /* INC_CAMERA_H_ */
