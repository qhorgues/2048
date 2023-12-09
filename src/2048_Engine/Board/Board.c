#include <stdlib.h>
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

        if (b & 0xffffffff) {
            _BotScanFForward(&idx, (int32_t)(board) );
            return (int)(idx);
        } else {
            _BitScanForward(&idx, (int32_t)(board >> 32));
            return (int)(idx + 32);
        }
    #endif
#error "Compiler not supported"
#endif
}

Board setCaseAfterNCase(Board board, size_t pos, size_t pow)
{
    size_t numberCaseEmpty = (size_t)countZeroBeforeFirst1(board) / 4;
    if (numberCaseEmpty > pos)
    {
        return board & pow << 4*pos;
    }
    else
    {
        size_t currentPos = 4*numberCaseEmpty;
        pos -= numberCaseEmpty;
        Board tmpBoard = board >> currentPos;
        while (currentPos < sizeof(board)*4) 
        {
            size_t nZero = (size_t)countZeroBeforeFirst1(tmpBoard);
            if (nZero < 4)
            {
                currentPos += 4;
                tmpBoard <<= 4ull;
                pos--;
            }
            else if (nZero >= pos*4)
            {
                return board & pow << (currentPos + 4*pos);
            }
            else
            {
                Board bitDecal = ((size_t)(nZero/4))*4;
                tmpBoard >>= bitDecal;
                currentPos += bitDecal;
                pos -= bitDecal/4;
            }
        }
    }
    return board;
}

Board Board_init(void)
{
    srand((unsigned int)time(NULL));
    /*
    Board board = 0ull;
    int pos1 = rand() % NUMBER_CASE;
    int pos2 = rand() % NUMBER_CASE -1;
    int t1 = (rand() % 2) +1;
    int t2 = (rand() % 2) +1;
    */
    return (Board){0};

}