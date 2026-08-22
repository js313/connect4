#ifndef MOVEENGINE_H
#define MOVEENGINE_H

#include <memory>

#include "move.h"
#include "board.h"

class MoveEngine
{
    const int MAX_DEPTH = 10;
    Move AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE side, int depth);

public:
    MoveEngine() = default;
    Move GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side);
    int EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side);
};

#endif