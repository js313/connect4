#ifndef DEFS_H
#define DEFS_H

#define DEBUG
#ifdef DEBUG
#define ASSERT(n)                                   \
    if (!(n))                                       \
    {                                               \
        std::cout << #n << " - Failed ";            \
        std::cout << "On " << __DATE__ << " ";      \
        std::cout << "At " << __TIME__ << " ";      \
        std::cout << "In File " << __FILE__ << " "; \
        std::cout << "At Line " << __LINE__ << " "; \
        exit(1);                                    \
    }
#else
#define ASSERT(n)
#endif

#include <cstdint>

const int COLS = 6; // Height
const int ROWS = 7; // Width

enum SIDE
{
    WHITE = 0,
    BLACK,
    NONE
};
const char SIDE_REPR[3] = {'X', 'O', '*'};

const int BitboardToPos[64] = {
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    35, 36, 37, 38, 39, 40, 41, -1,
    28, 29, 30, 31, 32, 33, 34, -1,
    21, 22, 23, 24, 25, 26, 27, -1,
    14, 15, 16, 17, 18, 19, 20, -1,
    7, 8, 9, 10, 11, 12, 13, -1,
    0, 1, 2, 3, 4, 5, 6, -1};

const int PosToBitboard[42] = {
    56, 57, 58, 59, 60, 61, 62,
    48, 49, 50, 51, 52, 53, 54,
    40, 41, 42, 43, 44, 45, 46,
    32, 33, 34, 35, 36, 37, 38,
    24, 25, 26, 27, 28, 29, 30,
    16, 17, 18, 19, 20, 21, 22};

#define RowColToIdx(row, col) (col * ROWS + row)
#define MASK(row, col) ((uint64_t)(((uint64_t)1) << (uint64_t)PosToBitboard[RowColToIdx(row, col)]))

#endif