/*
 * Filename: grid.h
 * Created Date: August 17th 2023
 * Author: diegomtassis
 */

#ifndef INC_GRID_H_
#define INC_GRID_H_

#include <genesis.h>

#include "ribbons.h"

extern Map *mapGridBG;
extern Map *mapGridFG;

#define MAP_WIDTH 1008
#define MAP_HEIGTH 560

u16 GRID_load(u16 vramBase, const MapDefinition *mapDefinitionBG, const MapDefinition *mapDefinitionFG);
void GRID_scroll(V2s16 point, bool redraw);
void GRID_release();

void GRID_trackRibbonStep(RibbonStep *ribbonStep);
void GRID_renderNewRibbonsSteps();

void GRID_trackRibbon(Ribbon *ribbon);
void GRID_removeRibbon(Ribbon *ribbon);


#endif /* INC_GRID_H_ */
