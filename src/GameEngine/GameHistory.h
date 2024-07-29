#ifndef GAME_HISTORY_H
#define GAME_HISTORY_H

#include <stdbool.h>
#include <stddef.h>

#define NUMBER_SAVE_GAME 5

struct GameEngine;

struct PastGame
{
    int board[4][4];
    int score;
};

struct GameHistory
{
    char* save_dir;
    struct PastGame* game[NUMBER_SAVE_GAME];
};

struct GameHistory initGameHistory(void);

bool checkIfNewBestScore(struct GameEngine* gameEngine);

void freeGameHistory(struct GameHistory *history);

#include "GameEngine.h"
#endif