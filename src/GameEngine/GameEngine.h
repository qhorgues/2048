#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <stdbool.h>

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
 * @brief Fait apparaître un nombre dans le jeu.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void spawnRandomNumber(struct GameEngine * gameEngine);



/**
 * @brief Fait le déplacement du haut.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveUp(struct GameEngine * gameEngine);



/**
 * @brief Fait le déplacement du bas.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveDown(struct GameEngine * gameEngine);



/**
 * @brief Fait le déplacement à gauche.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveLeft(struct GameEngine * gameEngine);



/**
 * @brief Fait le jeu à droite.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveRight(struct GameEngine * gameEngine);



/**
 * @brief Regarde si le jeu est fini.
 * 
 * @param[in] gameEngine Le jeu du 2048.
 * 
 * @return bool - Renvoie false si le jeu n'est pas fini, true sinon.
 */
bool isEnding(struct GameEngine * gameEngine);

#endif