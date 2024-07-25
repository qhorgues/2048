#ifndef BOARD_H
#define BOARD_H

struct Board
{
  int array[4][4];
};



/**
 * @brief Initialise le plateau du 2048.
 * 
 * @return struct Board - Renvoie le plateau du 2048
 */
struct Board initBoard(void);



/**
 * @brief Fait apparaître un nombre dans le plateau.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void spawnRandomNumber(struct Board * board);



/**
 * @brief Fait le déplacement du haut.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveUp(struct Board * board);



/**
 * @brief Fait le déplacement du bas.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveDown(struct Board * board);



/**
 * @brief Fait le déplacement à gauche.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveLeft(struct Board * board);



/**
 * @brief Fait le déplacement à droite.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveRight(struct Board * board);

#endif