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
typedef struct LightCycle LightCycle;

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
    Player* p1;
};

struct GameResult {
    u16 p1_score;
};

struct LightCycle {
    Object_f16 object;
    u8 health;
    u8 finished;
    u8 direction;
    bool boost;
    bool airborne;
    bool immunity;
    Sprite* sprite;
};

#endif /* INC_ELEMENTS_H_ */
