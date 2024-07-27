#include <stdbool.h>

#include "../Interface.h"
#include "../GameEngine/GameEngine.h"

void run(char const* current_dir)
{
    struct GameEngine engine = initGameEngine();
    enum GameStatus gameStatus = IN_GAME;
    enum Interactions interaction;
    Interface* interface = initInterface(current_dir);
    update(interface, gameStatus, &engine);
    while (true)
    {
        interaction = getInteraction(interface, &gameStatus, &engine);
        if (interaction == INTERACTION_QUIT)
        {
            break;
        }
        switch (gameStatus)
        {
        case IN_GAME:
            move(&engine, interaction);
            update(interface, gameStatus, &engine);
            break;
        default:
            break;
        }
        if (isEnding(&engine))
        {
            break;
        }
    }
    freeInterface(interface);
}