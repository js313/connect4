#ifndef MOVEENGINE_H
#define MOVEENGINE_H

#include <memory>
#include <vector>

#include "move.h"
#include "board.h"
#include "defs.h"

class MoveEngine
{
    const int MAX_DEPTH = 10;
    // Can be optimized in 2 ways, 1. better way to store this count, 2. calculate and update the values based in move, so instead of calculating the whole thing just modify values affected by the last move
    std::vector<std::vector<short>> blackOpenSides =
        std::vector<std::vector<short>>(ROWS, std::vector<short>(COLS, 0));
    std::vector<std::vector<short>> whiteOpenSides =
        std::vector<std::vector<short>>(ROWS, std::vector<short>(COLS, 0));

    Move AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE side, int depth, bool maximizing);
    int CountOpenPos(const std::unique_ptr<Board> &pos, int row, int col, SIDE side);

public:
    MoveEngine() = default;
    Move GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side);
    int EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side);
};

#endif