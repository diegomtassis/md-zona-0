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

u16 displayHud(u16);

V2s32 viewToScreen(V2s32 *subject);

V2s32 screenToView(V2s32 *subject);

#endif /* INC_SCREEN_H_ */
