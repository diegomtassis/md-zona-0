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
#define DEREZZING 0x02
#define DEREZZED 0x04

typedef struct Box_s16 Box_s16;
typedef struct Config Config;
typedef struct Player Player;
typedef struct Game Game;
typedef struct GameResult GameResult;

struct Box_s16 {
    V2s16 min;
    V2s16 max;
    u16 w;
    u16 h;
};

struct Config {
    u8 difficulty;
    u8 lives;
};

struct Player {
    u8 lives;
    u16 score;
};

struct Game {
    const Config *config;
};

struct GameResult {
    u16 p1_score;
};

#endif /* INC_ELEMENTS_H_ */
