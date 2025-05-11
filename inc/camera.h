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

void CAM_setup(s16 mapWidth, s16 mapHeight);
void CAM_focusOn(GridObject * subject);
void CAM_update();

extern Box_s16 cameraView;

V2s16 CAM_mapToView(V2s16 *);

#endif /* INC_CAMERA_H_ */
