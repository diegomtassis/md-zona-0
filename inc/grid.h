/*
 * Filename: grid.h
 * Created Date: August 17th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_H_
#define INC_GRID_H_

#include <genesis.h>

extern Map *gridMap;

#define MAP_WIDTH 1008
#define MAP_HEIGTH 560

u16 loadGrid(u16 vramBase, const MapDefinition *);
void scrollGrid(V2s16 point);

#endif /* INC_GRID_H_ */
