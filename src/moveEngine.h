#ifndef MOVEENGINE_H
#define MOVEENGINE_H

#include <memory>

#include "move.h"
#include "board.h"
#include "defs.h"

class MoveEngine
{
    static constexpr int MAX_DEPTH = 10;
    static constexpr int MATE_SCORE = 1000000;

    int AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE sideToMove,
                        SIDE perspective, int depth, int depthLimit,
                        int alpha, int beta,
                        Move *bestMove);

    int MiniMaxSearch(const std::unique_ptr<Board> &pos, SIDE sideToMove, SIDE perspective, int depth, int depthLimit);

public:
    MoveEngine() = default;
    Move GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side);
    int EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side);
};

#endif
