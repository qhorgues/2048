#include "Board.h"

#include <stdlib.h>
#include <time.h>


/**
 * @brief Initialise le plateau du 2048.
 * 
 * @return struct Board - Renvoie le plateau du 2048
 */
struct Board initBoard(void)
{
    struct Board board;
    
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            board.array[y][x] = 0;
        }
    }

    return board;
}



/**
 * @brief La fonction génère un 2 ou un 4 (90% de chance d'avoir 2, 10% de chance d'avoir 4).
 * 
 * @return int - Renvoie le nombre généré.
 */
static int randNewCase(void)
{
    srand(time(NULL));
    int r = rand();
    if (r < RAND_MAX / 10)
    {
        return 4;
    }
    else
    {
        return 2;
    }
}



/**
 * @brief Fait apparaître un nombre dans le plateau.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void spawnRandomNumber(struct Board * board)
{
    int randomNumber = randNewCase();
    int poseX = rand() % 4;
    int poseY = rand() % 4;

    int count = 0;
    while (board->array[poseY][poseX] != 0)
    {
        count++;
        poseX = (poseX + count) % 4;
        poseY = (poseY + count) / 4;
    }

    board->array[poseY][poseX] = randomNumber;
}



/**
 * @brief La fonction gère une partie des mouvements ainsi que ses fusions.
 * 
 * @param[in, out] board Le plateau du 2048.
 * @param[in] valut La valeur d'un élément de la grille.
 * @param[in] changeX La position en X où l'élément va se déplacer.
 * @param[in] changeY La position en Y où l'élément va se déplacer.
 * @param[in] changeXMerge La position en X où l'élément va fusionner.
 * @param[in] changeYMerge La position en Y où l'élément va fusionner.
 */
static void moves(struct Board * board, int invalidPlaces, int value, int changeX, int changeY, int changeXMerge, int changeYMerge)
{
    if (invalidPlaces > 0 && value == board->array[changeYMerge][changeXMerge])
    {   
        board->array[changeYMerge][changeXMerge] <<= 1;
    }
    else
    {
        board->array[changeY][changeX] = value;
    }
}



/**
 * @brief Fait le déplacement du haut.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveUp(struct Board * board)
{
    for (int x = 0; x < 4; x++)
    {
        int invalidPlaces = 0;

        for (int y = 0; y < 4; y++)
        {
            int value = board->array[y][x];

            if (value == 0)
                continue;

            moves(board, invalidPlaces, value, x, invalidPlaces, x, invalidPlaces - 1);

            if (y != invalidPlaces)
                board->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Fait le déplacement du bas.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveDown(struct Board * board)
{
    for (int x = 0; x < 4; x++)
    {
        int invalidPlaces = 0;

        for (int y = 3; y >= 0; y--)
        {
            int value = board->array[y][x];

            if (value == 0)
                continue;

            moves(board, invalidPlaces, value, x, 3 - invalidPlaces, x, 3 - invalidPlaces + 1);

            if (y != 3 - invalidPlaces)
                board->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Fait le déplacement à gauche.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveLeft(struct Board * board)
{
    for (int y = 0; y < 4; y++)
    {
        int invalidPlaces = 0;

        for (int x = 0; x < 4; x++)
        {
            int value = board->array[y][x];

            if (value == 0)
                continue;

            moves(board, invalidPlaces, value, invalidPlaces, y, invalidPlaces - 1, y);

            if (x != invalidPlaces)
                board->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Fait le déplacement à droite.
 * 
 * @param[in, out] board Le plateau du 2048.
 */
void moveRight(struct Board * board)
{
    for (int y = 0; y < 4; y++)
    {
        int invalidPlaces = 0;

        for (int x = 3; x >= 0; x--)
        {
            int value = board->array[y][x];

            if (value == 0)
                continue;

            moves(board, invalidPlaces, value, 3 - invalidPlaces, y, 3 - invalidPlaces + 1, y);

            if (x != 3 - invalidPlaces)
                board->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}