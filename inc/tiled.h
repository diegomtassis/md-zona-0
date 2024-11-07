/*
 * tiled.h
 * Created: November 7th 2024
 * Author: diegomtassis
 */

#ifndef INC_TILED_H_
#define INC_TILED_H_

#include <genesis.h>

typedef struct GridBoundaryMarker GridBoundaryMarker;
typedef struct MovableInitMarker MovableInitMarker;

struct GridBoundaryMarker {
  char* name;
  s16 x;
  s16 y;
  s16 grid_x;
  s16 grid_y;
  void* conn_right;
  void* conn_down;
};

struct MovableInitMarker {
  char* name;
  s16 x;
  s16 y;
  s16 grid_x;
  s16 grid_y;
  u16 direction;
};

#endif /* INC_TILED_H_ */
