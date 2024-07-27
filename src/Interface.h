#ifndef INTERFACE_H
#define INTERFACE_H

#include "GameStatus.h"
#include "GameEngine/GameEngine.h"
#include "Interactions.h"

typedef void Interface;

Interface* initInterface(char const* dir_exe);
void update(Interface* interface, enum GameStatus status, struct GameEngine const* gameEngine);
enum Interactions getInteraction(Interface* interface, enum GameStatus* status, struct GameEngine const* gameEngine);
void freeInterface(Interface* interface);

#endif