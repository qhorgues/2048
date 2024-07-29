
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <memory.h>
#include "GameEngine.h"
#include "GameHistory.h"

static void spawnRandomNumber(struct GameEngine * gameEngine);

/**
 * @brief Initialise le jeu du 2048.
 * 
 * @return struct GameEngine - Renvoie le jeu du 2048.
 */
struct GameEngine initGameEngine(void)
{
    struct GameEngine gameEngine;
    gameEngine.gameHistory = initGameHistory();
    srand((unsigned int)time(NULL));

    resetGameEngine(&gameEngine);

    return gameEngine;
}



/**
 * @brief La fontion réinitialise le tableau.
 * 
 * @param[out] gameEngine Le jeu du  2048
 */
void resetGameEngine(struct GameEngine *gameEngine)
{
    memset(gameEngine->board, 0, sizeof(int) * 4 * 4);

    gameEngine->score = 0;
    spawnRandomNumber(gameEngine);
    spawnRandomNumber(gameEngine);
}



/**
 * @brief La fonction génère un 2 ou un 4 (90% de chance d'avoir 2, 10% de chance d'avoir 4).
 * 
 * @return int - Renvoie le nombre généré.
 */
static int randNewCase(void)
{
    int r = rand();
    if (r < RAND_MAX / 10)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}



/**
 * @brief Fait apparaître un nombre dans le jeu.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 */
static void spawnRandomNumber(struct GameEngine * gameEngine)
{
    int randomNumber = randNewCase();
    int poseX = rand() % 4;
    int poseY = rand() % 4;

    int count = poseX + poseY * 4;
    while (gameEngine->board[poseY][poseX] != 0)
    {
        count++;
        poseX = count % 4;
        poseY = (count / 4) % 4;
    }

    gameEngine->board[poseY][poseX] = randomNumber;
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
 * @param[in] merge Vérifie si le dernier élément a déjà été fusionné.
 * 
 * @return bool - Renvoie true si le nombre a fusionner, false sinon.
 */
static bool moves(struct GameEngine * gameEngine, int * invalidPlaces, int value, int changeX, int changeY, int changeXMerge, int changeYMerge, bool merge)
{
    if (*invalidPlaces > 0 && !merge && value == gameEngine->board[changeYMerge][changeXMerge])
    {   
        gameEngine->board[changeYMerge][changeXMerge]++;
        gameEngine->score += 1 << gameEngine->board[changeYMerge][changeXMerge];
        (*invalidPlaces)--;
        return true;
    }
    
    gameEngine->board[changeY][changeX] = value;
    return false;
}



/**
 * @brief Fait le déplacement du haut.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 *
 * @return bool - Renvoie true si le mouvement a eu lieu, false sinon.
 */
static bool moveUp(struct GameEngine * gameEngine)
{
    bool moveApplied = false;
    for (int x = 0; x < 4; x++)
    {
        int invalidPlaces = 0;
        bool merge = false;

        for (int y = 0; y < 4; y++)
        {
            int value = gameEngine->board[y][x];

            if (value == 0)
                continue;

            merge = moves(gameEngine, &invalidPlaces, value, x, invalidPlaces, x, invalidPlaces - 1, merge);

            if (y != invalidPlaces)
            {
                gameEngine->board[y][x] = 0;
                moveApplied = true;
            }
                
            invalidPlaces++;
        }
    }

    return moveApplied;
}



/**
 * @brief Fait le déplacement du bas.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 *
 * @return bool - Renvoie true si le mouvement a eu lieu, false sinon.
 */
static bool moveDown(struct GameEngine * gameEngine)
{
    bool moveApplied = false;
    for (int x = 0; x < 4; x++)
    {
        int invalidPlaces = 0;
        bool merge = false;

        for (int y = 3; y >= 0; y--)
        {
            int value = gameEngine->board[y][x];

            if (value == 0)
                continue;

            merge = moves(gameEngine, &invalidPlaces, value, x, 3 - invalidPlaces, x, 3 - invalidPlaces + 1, merge);

            if (y != 3 - invalidPlaces)
            {
                gameEngine->board[y][x] = 0;
                moveApplied = true;
            }
                
            invalidPlaces++;
        }
    }
    
    return moveApplied;
}



/**
 * @brief Fait le déplacement à gauche.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 *
 * @return bool - Renvoie true si le mouvement a eu lieu, false sinon.
 */
static bool moveLeft(struct GameEngine * gameEngine)
{
    bool moveApplied = false;
    for (int y = 0; y < 4; y++)
    {
        int invalidPlaces = 0;
        bool merge = false;

        for (int x = 0; x < 4; x++)
        {
            int value = gameEngine->board[y][x];

            if (value == 0)
                continue;

            merge = moves(gameEngine, &invalidPlaces, value, invalidPlaces, y, invalidPlaces - 1, y, merge);

            if (x != invalidPlaces)
            {
                gameEngine->board[y][x] = 0;
                moveApplied = true;
            }
                
            invalidPlaces++;
        }
    }

    return moveApplied;
}



/**
 * @brief Fait le déplacement à droite.
 * 
 * @param[in, out] gameEngine Le jeu du 2048.
 *
 * @return bool - Renvoie true si le mouvement a eu lieu, false sinon.
 */
static bool moveRight(struct GameEngine * gameEngine)
{
    bool moveApplied = false;
    for (int y = 0; y < 4; y++)
    {
        int invalidPlaces = 0;
        bool merge = false;

        for (int x = 3; x >= 0; x--)
        {
            int value = gameEngine->board[y][x];

            if (value == 0)
                continue;

            merge = moves(gameEngine, &invalidPlaces, value, 3 - invalidPlaces, y, 3 - invalidPlaces + 1, y, merge);

            if (x != 3 - invalidPlaces)
            {
                gameEngine->board[y][x] = 0;
                moveApplied = true;
            }   
            invalidPlaces++;
        }
    }
    return moveApplied;
}


/**
 * @brief Permet de faire un mouvement.
 * 
 * @param gameEngine[in, out] Le jeu du 2048.
 * @param interaction[in] Le mouvement.
 */
void move(struct GameEngine * gameEngine, enum Interactions interaction)
{
    bool moveApplied = false;
    switch (interaction)
    {
    case INTERACTION_MOVE_UP:
        moveApplied = moveUp(gameEngine);
        break;
    case INTERACTION_MOVE_DOWN:
        moveApplied = moveDown(gameEngine);
        break;
    case INTERACTION_MOVE_LEFT:
        moveApplied = moveLeft(gameEngine);
        break;
    case INTERACTION_MOVE_RIGHT:
        moveApplied = moveRight(gameEngine);
        break;
    case INTERACTION_CHANGE_GAME_STATUS:
    case INTERACTION_QUIT:
    default:
        break;
    }
    if (moveApplied)
    {
        spawnRandomNumber(gameEngine);
    }
}


/**
 * @brief Regarde si le jeu est fini.
 * 
 * @param[in] gameEngine Le jeu du 2048.
 */
bool isEnding(struct GameEngine const *gameEngine)
{
    if (gameEngine->board[3][3] == 0)
    {
        return false;
    }
    for (int i = 0; i < 3; i++)
    {
        if (gameEngine->board[3][i] == 0 || gameEngine->board[i][3] == 0
            || gameEngine->board[3][i] == gameEngine->board[3][i+1] 
            || gameEngine->board[i][3] == gameEngine->board[i+1][3])
        {
            return false;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameEngine->board[i][j] == 0
                || gameEngine->board[i][j] == gameEngine->board[i+1][j]
                || gameEngine->board[i][j] == gameEngine->board[i][j+1])
            {
                return false;
            }
        }
    }
    return true;
}



/**
 * @brief Cette fonction libère le jeu du 2048.
 * 
 * @param gameEngine Le jeu du 2048. 
 */
void freeGameEngine(struct GameEngine * gameEngine)
{
    freeGameHistory(&(gameEngine->gameHistory));
}