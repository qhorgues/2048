#include <stdio.h>
#include "2048_Engine/Board/Board.h"


Board Board_init(void);

int main(void)
{
    Board board = Board_init();
    printf("%llX\n", board);
    return 0;
}