/*
 * Filename: screen.h
 * Created Date: August 26th 2023
 * Author: diegomtassis
 */

#ifndef INC_SCREEN_H_
#define INC_SCREEN_H_

#include <genesis.h>

#include "fwk/physics.h"

#define HUD_LEFT_COLUMN_WIDTH   16

extern Map *hudMap;

u16 SCREEN_setupHud(u16);

void SCREEN_showReady();
void SCREEN_clearMessage();

V2s16 SCREEN_viewToScreen(V2s16 *subject);

V2s16 SCREEN_screenToView(V2s16 *subject);

V2s16 SCREEN_screenInMap(V2s16 *viewInMap);

V2u16 SCREEN_posToTile(V2s16 position);

#endif /* INC_SCREEN_H_ */
