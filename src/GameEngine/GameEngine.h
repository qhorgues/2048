#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <stdbool.h>
#include "Interactions.h"

/**
 * @brief Cette structure représente le jeu 2048.
 */
struct GameEngine
{
  int board[4][4];
  int score;
};



/**
 * @brief Initialise le jeu du 2048.
 * 
 * @return struct GameEngine - Renvoie le jeu du 2048.
 */
struct GameEngine initGameEngine(void);

/**
 * @brief Permet de faire un mouvement.
 * 
 * @param gameEngine[in, out] Le jeu du 2048.
 * @param interaction[in] Le mouvement.
 */
void move(struct GameEngine * gameEngine, enum Interactions interaction);

/**
 * @brief Regarde si le jeu est fini.
 * 
 * @param[in] gameEngine Le jeu du 2048.
 * 
 * @return bool - Renvoie false si le jeu n'est pas fini, true sinon.
 */
bool isEnding(struct GameEngine * gameEngine);

#endif