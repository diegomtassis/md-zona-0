/*
 * Filename: grid.h
 * Created Date: August 17th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_H_
#define INC_GRID_H_

#include <genesis.h>

extern Map *mapGridBG;
extern Map *mapGridFG;

extern u16 ribbonsVramBaseTile;

#define MAP_WIDTH 1008
#define MAP_HEIGTH 560

typedef struct {
    u8 direction;
    u16 baseTile;
    bool first;
} RibbonSegment;
typedef struct {
    V2s16 mapPos;
    u8 direction;
    u16 baseTile;
    bool first;
} RibbonStep;

u16 GRID_load(u16 vramBase, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG);
void GRID_scroll(V2s16 point, bool redraw);
void GRID_release();

void GRID_addRibbonStep(RibbonStep *ribbonStep);
void GRID_updateRibbons();

#endif /* INC_GRID_H_ */
