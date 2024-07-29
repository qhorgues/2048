#ifndef GAME_HISTORY_H
#define GAME_HISTORY_H

#include <stdbool.h>
#include <stddef.h>
#include "GameEngine.h"

#define NUMBER_SAVE_GAME 5

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

bool checkIfNewBestScore(struct GameEngine* gameEngine, struct GameHistory *gameHistory);

void freeGameHistory(struct GameHistory *history);

#endif