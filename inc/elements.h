/*
 * elements.h
 * Created: August 8th 2023
 * Author: diegomtassis
 */

#ifndef INC_ELEMENTS_H_
#define INC_ELEMENTS_H_

#include <genesis.h>

#define ALIVE 0x01
#define DEAD 0x02

typedef struct Config Config;
typedef struct Game Game;
typedef struct GameResult GameResult;
typedef struct Player Player;

struct Config {
    u8 difficulty;
    u8 lives;
};

struct Player {
    u8 id;
    u8 lives;
    u16 score;
};

struct Game {
    const Config* config;
    Player* p1;
};

struct GameResult {
    u16 p1_score;
};

#endif /* INC_ELEMENTS_H_ */
