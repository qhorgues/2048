#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* createFolder(void) {

    #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        size_t size_env = strlen(getenv("LOCALAPPDATA"));
        char *PATH = malloc(sizeof(char) * size_env  + sizeof("/2048"));

        strcpy(PATH, getenv("LOCALAPPDATA"));

        strcat(PATH, "/2048");

        mkdir(PATH);

    #elif defined(__linux__) || defined(unix) || defined(__unix) || defined(__unix__)
        size_t size_env = strlen(getenv("HOME"));
        char *PATH = malloc(sizeof(char) * size_env + sizeof("/.local/share/2048"));

        strcpy(PATH, getenv("HOME"));

        strcat(PATH, "/.local/share/2048");
        puts(PATH);

        mkdir(PATH, S_IRWXO | S_IRWXG | S_IRWXU);
    #elif defined(__APPLE__) // not tested
        size_t size_env = strlen(getenv("HOME"));
        char *PATH = malloc(sizeof(char) * size_env + sizeof("/Library/Application Support/2048"));

        strcpy(PATH, getenv("HOME"));

        strcat(PATH, "/Library/Application Support/2048");
        puts(PATH);

        mkdir(PATH, S_IRWXO | S_IRWXG | S_IRWXU);
    #else
        char *PATH = malloc(sizeof("./Save"));

        strcpy(PATH, "./Save");
        mkdir(PATH, S_IRUSR);
    #endif
    
    return PATH;
}