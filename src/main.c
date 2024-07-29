#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GameEngine/GameEngine.h"
#include "GameEngine/GameHistory.h"

#define UNUSED(x) ((void)(x))

char *removeExeInPath(char const *exe_path)
{
    size_t len_path = strlen(exe_path);
    while (len_path > 0 && exe_path[len_path - 1] != '/' && exe_path[len_path - 1] != '\\')
    {
        len_path -= 1;
    }
    char *path = malloc(sizeof(char) * (len_path + 1));
    if (path != NULL)
    {
        strncpy(path, exe_path, len_path);
        path[len_path] = '\0';
    }
    return path;
}


int main(int argc, char **argv)
{
    UNUSED(argc);
    char *path_exe = removeExeInPath(argv[0]);
    free(path_exe);

    struct GameEngine engine = initGameEngine();
    engine.score = 1548153;
    struct GameHistory history = initGameHistory();
    checkIfNewBestScore(&engine);
    freeGameHistory(&history);

    return 0;
}