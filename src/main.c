#include <stdio.h>
#include "2048_Engine/Board/Board.h"

extern Board setCaseAfterNCase(Board board, int pos, int pow);

int main(void)
{
    Board board = { 0xF };
    board = setCaseAfterNCase(board, 1, 0xF);
    printf("%lx\n", board);
    return 0;
}