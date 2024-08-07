#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Game/Game.h"

#include "GameEngine/GameEngine.h"

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
    char *str = removeExeInPath(argv[0]);
    run(str);
    free(str);
    return 0;
}