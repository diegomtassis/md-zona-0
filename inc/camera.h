/*
 * Filename: camera.h
 * Created: August 14th 2023
 * Author: diegomtassis
 */

#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <genesis.h>
#include "fwk/physics.h"
#include "elements.h"
#include "grid_physics.h"

#define CAMERA_VIEW_WIDTH		288
#define CAMERA_VIEW_HEIGHT		160

void setupCamera(s16 mapWidth, s16 mapHeight);
void cameraFocusOn(GridObject *);
void updateCamera();

extern Box_s16 cameraView;

V2s16 mapToView(V2s16 *);

#endif /* INC_CAMERA_H_ */
