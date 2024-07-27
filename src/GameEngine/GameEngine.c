#include "GameEngine.h"

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


/**
 * @brief Initialise le jeu du 2048.
 * 
 * @return struct GameEngine - Renvoie le jeu du 2048.
 */
struct GameEngine initGameEngine(void)
{
    struct GameEngine gameEngine;
    
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            gameEngine.array[y][x] = 0;
        }
    }

    gameEngine.score = 0;

    return gameEngine;
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
 * @brief Fait apparaître un nombre dans le jeu.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void spawnRandomNumber(struct GameEngine * gameEngine)
{
    int randomNumber = randNewCase();
    int poseX = rand() % 4;
    int poseY = rand() % 4;

    int count = poseX + poseY * 4;
    while (gameEngine->array[poseY][poseX] != 0)
    {
        count++;
        poseX = count % 4;
        poseY = (count / 4) % 4;
    }

    gameEngine->array[poseY][poseX] = randomNumber;
}



/**
 * @brief La fonction gère une partie des mouvements ainsi que ses fusions.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 * @param[in] valut La valeur d'un élément de la grille.
 * @param[in] changeX La position en X où l'élément va se déplacer.
 * @param[in] changeY La position en Y où l'élément va se déplacer.
 * @param[in] changeXMerge La position en X où l'élément va fusionner.
 * @param[in] changeYMerge La position en Y où l'élément va fusionner.
 */
static void moves(struct GameEngine * gameEngine, int * invalidPlaces, int value, int changeX, int changeY, int changeXMerge, int changeYMerge)
{
    if (*invalidPlaces > 0 && value == gameEngine->array[changeYMerge][changeXMerge])
    {   
        gameEngine->array[changeYMerge][changeXMerge]++;
        gameEngine->score += gameEngine->array[changeYMerge][changeXMerge];
        (*invalidPlaces)--;
    }
    else
    {
        gameEngine->array[changeY][changeX] = value;
    }
}



/**
 * @brief Fait le déplacement du haut.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveUp(struct GameEngine * gameEngine)
{
    for (int x = 0; x < 4; x++)
    {
        int invalidPlaces = 0;

        for (int y = 0; y < 4; y++)
        {
            int value = gameEngine->array[y][x];

            if (value == 0)
                continue;

            moves(gameEngine, &invalidPlaces, value, x, invalidPlaces, x, invalidPlaces - 1);

            if (y != invalidPlaces)
                gameEngine->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Fait le déplacement du bas.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveDown(struct GameEngine * gameEngine)
{
    for (int x = 0; x < 4; x++)
    {
        int invalidPlaces = 0;

        for (int y = 3; y >= 0; y--)
        {
            int value = gameEngine->array[y][x];

            if (value == 0)
                continue;

            moves(gameEngine, &invalidPlaces, value, x, 3 - invalidPlaces, x, 3 - invalidPlaces + 1);

            if (y != 3 - invalidPlaces)
                gameEngine->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Fait le déplacement à gauche.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveLeft(struct GameEngine * gameEngine)
{
    for (int y = 0; y < 4; y++)
    {
        int invalidPlaces = 0;

        for (int x = 0; x < 4; x++)
        {
            int value = gameEngine->array[y][x];

            if (value == 0)
                continue;

            moves(gameEngine, &invalidPlaces, value, invalidPlaces, y, invalidPlaces - 1, y);

            if (x != invalidPlaces)
                gameEngine->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Fait le déplacement à droite.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
void moveRight(struct GameEngine * gameEngine)
{
    for (int y = 0; y < 4; y++)
    {
        int invalidPlaces = 0;

        for (int x = 3; x >= 0; x--)
        {
            int value = gameEngine->array[y][x];

            if (value == 0)
                continue;

            moves(gameEngine, &invalidPlaces, value, 3 - invalidPlaces, y, 3 - invalidPlaces + 1, y);

            if (x != 3 - invalidPlaces)
                gameEngine->array[y][x] = 0;
                
            invalidPlaces++;
        }
    }
}



/**
 * @brief Regarde si le jeu est fini.
 * 
 * @param[in] gameEngine Le jeu du 2048.
 */
bool isEnding(struct GameEngine * gameEngine)
{
    struct GameEngine copyEngine = *gameEngine;
    moveUp(&copyEngine);
    moveDown(&copyEngine);
    moveLeft(&copyEngine);
    moveRight(&copyEngine);

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (copyEngine.array[y][x] == 0)
            {
                return false;
            }
        }
    }

    return true;
}