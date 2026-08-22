#include "moveEngine.h"

int MoveEngine::EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side)
{
    return 1;
}

Move MoveEngine::AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE side, int depth)
{
    if (depth == MAX_DEPTH)
    {
        int maxScore = INT_MIN;
        Move bestMove;
        for (auto &move : pos->GenerateAllMoves(side))
        {
            move.score = EvaluatePos(pos, side);
            if (move.score > maxScore)
            {
                bestMove = move;
            }
        }
        return bestMove;
    }
    return AlphaBetaSearch(pos, SIDE(!side), depth + 1);
}

Move MoveEngine::GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side)
{
    return AlphaBetaSearch(pos, side, 0);
}
