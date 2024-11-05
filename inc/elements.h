/*
 * elements.h
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#ifndef INC_ELEMENTS_H_
#define INC_ELEMENTS_H_

#include <genesis.h>

#include "fwk/physics.h"

#define ALIVE 0x01
#define DEAD 0x02

typedef struct Config Config;
typedef struct Player Player;
typedef struct Game Game;
typedef struct GameResult GameResult;
typedef struct TiledObject TiledObject;

struct Config {
  u8 difficulty;
  u8 lives;
};

struct Player {
  u8 lives;
  u16 score;
  u8 joystick;
};

struct Game {
  const Config* config;
};

struct GameResult {
  u16 p1_score;
};

struct GridObject {
  char* name;
  s16 x;
  s16 y;
  s16 grid_x;
  s16 grid_y;
  void* conn_right;
  void* conn_down;
};

struct VehicleStartPosition {
  char* name;
  s16 x;
  s16 y;
  s16 grid_x;
  s16 grid_y;
  u16 direction;
};

#endif /* INC_ELEMENTS_H_ */
