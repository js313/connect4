#include <algorithm>
#include <cstdlib>
#include <vector>

#include "moveEngine.h"

int MoveEngine::EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side)
{
    int score = 0;

    // Position based scoring
    int posBasedWeights[] = {0, 1, 2, 3, 2, 1, 0};
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            SIDE pceSide = pos->GetSideAt(row, col);
            if (pceSide == side)
                score += posBasedWeights[row];
            else if (pceSide == SIDE(!side))
                score -= posBasedWeights[col];
        }
    }

    // Position + Neighbour based scoring
    int neighbourBasedWeights[] = {0, 1, 10, 100, MATE_SCORE};
    int dirs[][2] = {{1, -1}, {-1, 1}, {0, 1}, {1, 0}};
    int immediateWins = 0, opponentImmediateWins = 0;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            for (const auto &dir : dirs)
            {
                int ownPieces = 0, opponentPieces = 0;
                for (int step = 0; step < 4; step++)
                {
                    SIDE pceSide = pos->GetSideAt(row + step * dir[0], col + step * dir[1]);
                    if (pceSide == side)
                        ownPieces++;
                    else if (pceSide == SIDE(!side))
                        opponentPieces++;
                }

                if (ownPieces == 0)
                    score -= neighbourBasedWeights[opponentPieces];
                else if (opponentPieces == 0)
                    score += neighbourBasedWeights[ownPieces];

                // Immediate wins
                if (ownPieces == 3 && opponentPieces == 0)
                    immediateWins++;
                else if (opponentPieces == 3 && ownPieces == 0)
                    opponentImmediateWins++;
            }
        }
    }

    // Immediate wins scoring
    score += 1000 * immediateWins;
    score -= 1000 * immediateWins;
}

int MoveEngine::AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE sideToMove,
                                SIDE perspective, int depth, int depthLimit,
                                int alpha, int beta,
                                Move *bestMove)
{
}

int MiniMaxSearch(const std::unique_ptr<Board> &pos, SIDE sideToMove, SIDE perspective, int depth, int depthLimit)
{
}

Move MoveEngine::GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side)
{
}