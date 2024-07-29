#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stddef.h>

#include "GameHistory.h"
#include "GameEngine.h"

static char* createFolder(size_t extra_size)
{

    #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        size_t size_env = strlen(getenv("LOCALAPPDATA"));
        char *PATH = malloc(sizeof(char) * size_env  + sizeof("/2048") + extra_size);

        strcpy(PATH, getenv("LOCALAPPDATA"));

        strcat(PATH, "/2048");

        mkdir(PATH);

    #elif defined(__linux__) || defined(unix) || defined(__unix) || defined(__unix__)
        size_t size_env = strlen(getenv("HOME"));
        char *PATH = malloc(sizeof(char) * size_env + sizeof("/.local/share/2048") + extra_size);

        strcpy(PATH, getenv("HOME"));

        strcat(PATH, "/.local/share/2048");

        mkdir(PATH, S_IRWXO | S_IRWXG | S_IRWXU);
    #elif defined(__APPLE__) // not tested
        size_t size_env = strlen(getenv("HOME"));
        char *PATH = malloc(sizeof(char) * size_env + sizeof("/Library/Application Support/2048") + extra_size);

        strcpy(PATH, getenv("HOME"));

        strcat(PATH, "/Library/Application Support/2048");
        puts(PATH);

        mkdir(PATH, S_IRWXO | S_IRWXG | S_IRWXU);
    #else
        char *PATH = malloc(sizeof("./Save") + extra_size);

        strcpy(PATH, "./Save");
        mkdir(PATH, S_IRUSR);
    #endif
    
    return PATH;
}

static void loadHistory(struct GameHistory *gameHistory);

struct GameHistory initGameHistory(void)
{
    struct GameHistory history;

    char* path = createFolder(sizeof("/best_game.2048save"));
    
    strcat(path, "/best_game.2048save");

    history.save_dir = path;
    memset(history.game, 0, sizeof(struct PastGame*) * NUMBER_SAVE_GAME);

    loadHistory(&history);

    return history;
}

static int cmp_PastGame(void const* a, void const* b)
{
    struct PastGame const* game_a = *(struct PastGame const* const*)a;
    struct PastGame const* game_b = *(struct PastGame const* const*)b;

    if (game_a == NULL && game_b == NULL)
    {
        return 0;
    }
    if (game_a == NULL)
    {
        return 1;
    }
    if (game_b == NULL)
    {
        return -1;
    }
    return game_b->score - game_a->score;
}

static void loadHistory(struct GameHistory *gameHistory)
{
    FILE *save_file = fopen(gameHistory->save_dir, "rb");

    if (save_file != NULL)
    {
        int number_save;

        if (fread(&number_save, sizeof(int), 1, save_file) == 1)
        {
            for (int i = 0; i < number_save; i++)
            {
                if (gameHistory->game[i] == NULL)
                {
                    gameHistory->game[i] = malloc(sizeof(struct PastGame));
                    if (gameHistory->game[i] == NULL)
                    {
                        for (int j = 0; j < i; j++)
                        {
                            free(gameHistory->game[j]);
                        }
                        fclose(save_file);
                        return;
                    }
                }
                if (fread(gameHistory->game[i], sizeof(struct PastGame), 1, save_file) != 1)
                {
                    for (int j = 0; j <= i; j++)
                    {
                        free(gameHistory->game[j]);
                    }
                    fclose(save_file);
                    return;
                }
            }

            qsort(gameHistory->game, (size_t)number_save, sizeof(struct PastGame*), &cmp_PastGame);
        }

        fclose(save_file);
    }

}

static void saveHistory(struct GameHistory *gameHistory)
{
    FILE *save_file = fopen(gameHistory->save_dir, "wb");

    if (save_file != NULL)
    {
        int count = 0;
        for (int i = 0; i < NUMBER_SAVE_GAME; i++)
        {
            if (gameHistory->game[i] != NULL)
            {
                count++;
            }
        }

        fwrite(&count, sizeof(int), 1, save_file);

        for (int i = 0; i < count; i++)
        {
            fwrite(gameHistory->game[i], sizeof(struct PastGame), 1, save_file);
        }

        fclose(save_file);
    }
}

static void addNewBestGame(struct GameEngine* gameEngine, struct GameHistory *gameHistory)
{
    struct PastGame* last = gameHistory->game[0];
    for (int i = 1; i < NUMBER_SAVE_GAME; i++)
    {
        struct PastGame* tmp = gameHistory->game[i];
        gameHistory->game[i] = last;
        last = tmp;
    }
    if (last != NULL)
    {
        gameHistory->game[0] = last;
    }
    else
    {
        gameHistory->game[0] = malloc(sizeof(struct PastGame));
        if (gameHistory->game[0] == NULL)
        {
            return;
        }
    }
    memcpy(gameHistory->game[0]->board, gameEngine->board, sizeof(int)*4*4);
    gameHistory->game[0]->score = gameEngine->score;
}

bool checkIfNewBestScore(struct GameEngine* gameEngine)
{
    if (gameEngine->gameHistory.game[0] == NULL || gameEngine->score > gameEngine->gameHistory.game[0]->score)
    {
        addNewBestGame(gameEngine, &gameEngine->gameHistory);
        saveHistory(&gameEngine->gameHistory);
        return true;
    }
    return false;
}

void freeGameHistory(struct GameHistory *history)
{
    free(history->save_dir);
    for (int i = 0; i < NUMBER_SAVE_GAME; i++)
    {
        if (history->game[i] != NULL)
        {
            free(history->game[i]);
        }
    }
}