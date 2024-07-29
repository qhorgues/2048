#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UNUSED(x) ((void)(x))

char* createFolder(void);

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
    char* user_folder = createFolder();
    printf("%s\n", user_folder);
    free(user_folder);
    free(path_exe);
    return 0;
}