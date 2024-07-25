#ifndef INTERFACE_H
#define INTERFACE_H

#include "GameStatus.h"
#include "Board/Board.h"
#include "Intercations.h"

typedef void Interface;

Interface* initInterface(void);
void update(Interface* interface, enum GameStatus status, struct Board const* board);
enum Interactions getInteraction(Interface* interface, enum GameStatus* status);
void freeInterface(Interface* interface);

#endif