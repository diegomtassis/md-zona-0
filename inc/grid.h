/*
 * Filename: grid.h
 * Created Date: August 17th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_H_
#define INC_GRID_H_

#include <genesis.h>

extern Map *gridMap;

#define GRID_WIDTH 960
#define GRID_HEIGTH 488

u16 displayGrid(u16 vramBase, u32, u32);
void scrollGrid();

#endif /* INC_GRID_H_ */
