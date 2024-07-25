#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Board.h"

#define NUMBER_CASE 16

static int countZeroBeforeFirst1(Board board)
{
#if defined(__GNUC__)
    return (int)(__builtin_ctzll(board));
#elif defined(_MSC_VER)
#if defined(_WIN64)
    unsigned long idx;
    _BitScanForward64(&idx, (int32_t)(board));
    return (int)idx;
#else
    assert(bitboard);
    unsigned long idx;

    if (b & 0xffffffff)
    {
        _BotScanFForward(&idx, (int32_t)(board));
        return (int)(idx);
    }
    else
    {
        _BitScanForward(&idx, (int32_t)(board >> 32));
        return (int)(idx + 32);
    }
#endif
#error "Compiler not supported"
#endif
}

static Board setCaseAfterNCase_rec(Board board, size_t n, size_t pow, size_t nByteInBoard)
{
    if (nByteInBoard >= 4 * (n + 1))
    {
        if (board == 0)
        {
            return pow << 4 * n;
        }
        const size_t numberBitEmpty = (size_t)countZeroBeforeFirst1(board);
        const size_t numberCaseEmpty = numberBitEmpty / 4;
        if (numberCaseEmpty >= n + 1)
        {
            return pow << 4 * n;
        }
        else
        {
            const size_t jmp = 4 * (numberCaseEmpty + 1);
            return setCaseAfterNCase_rec(board >> jmp, n - numberCaseEmpty, pow, nByteInBoard - jmp) << jmp;
        }
    }
    return 0;
}

Board setCaseAfterNCase(Board board, int n, int pow)
{
    return setCaseAfterNCase_rec(board, (size_t)n, (size_t)pow, 64ull) | board;
}

static int randPowNewCase(void)
{
    int r = rand();
    if (r < RAND_MAX / 20)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

Board moveUP(Board board)
{
    for (size_t i = 16; i < 64; i += 4)
    {
        size_t current = (board & (0xF << i));
        size_t bottom = (board & (0xF << (i-16)));
        if (current != 0) 
        {
            if (bottom == 0)
            {
                board |= current >> 16;
                board ^= current; 
            }
            else if (current == bottom << 16)
            {
                board ^= current;
                board ^= bottom;
                board |= (current+1) >> 16;
            }
        }
    }
}

Board Board_move(Board board, Board_Move move)
{
    switch (move)
    {
    case UP:

    }
}

Board Board_init(void)
{
    srand((unsigned int)time(NULL));
    Board board = 0ull;
    int pos1 = rand() % NUMBER_CASE;
    int pos2 = rand() % (NUMBER_CASE - 1);
    int t1 = randPowNewCase();
    int t2 = randPowNewCase();

    board = setCaseAfterNCase(board, pos1, t1);
    board = setCaseAfterNCase(board, pos2, t2);
    return board;
}