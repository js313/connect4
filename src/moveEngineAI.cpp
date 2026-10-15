#include <algorithm>
#include <cstdlib>
#include <vector>

#include "moveEngine.h"

int MoveEngine::EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side)
{
    const SIDE winner = pos->CheckBoardState();
    if (winner != NONE)
        return winner == side ? MATE_SCORE : -MATE_SCORE;
    if (pos->IsBoardFilled())
        return 0;

    // Score each four-cell window once. A window containing both sides cannot
    // become a winning line, so it contributes nothing.
    constexpr int weights[] = {0, 1, 10, 100, MATE_SCORE};
    constexpr int directions[][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int score = 0;

    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            for (const auto &direction : directions)
            {
                const int endRow = row + 3 * direction[0];
                const int endCol = col + 3 * direction[1];
                if (endRow < 0 || endRow >= ROWS || endCol < 0 || endCol >= COLS)
                    continue;

                int ownPieces = 0;
                int opponentPieces = 0;
                for (int step = 0; step < 4; ++step)
                {
                    const SIDE cell = pos->GetSideAt(row + step * direction[0],
                                                     col + step * direction[1]);
                    if (cell == side)
                        ++ownPieces;
                    else if (cell != NONE)
                        ++opponentPieces;
                }

                if (opponentPieces == 0)
                    score += weights[ownPieces];
                else if (ownPieces == 0)
                    score -= weights[opponentPieces];
            }
        }
    }

    // The center column participates in more possible winning lines.
    constexpr int centerWeights[ROWS] = {0, 1, 2, 3, 2, 1, 0};
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            const SIDE cell = pos->GetSideAt(row, col);
            if (cell == side)
                score += centerWeights[row];
            else if (cell != NONE)
                score -= centerWeights[row];
        }
    }

    // At the search horizon, distinguish real one-move wins from open threes
    // whose empty cell may not yet be playable because of gravity.
    auto countImmediateWins = [&pos](SIDE player)
    {
        int winningMoves = 0;
        for (const Move &move : pos->GenerateAllMoves(player))
        {
            if (!pos->MakeMove(move.row, player))
                continue;
            if (pos->CheckBoardState() == player)
                ++winningMoves;
            pos->UnmakeLastMove();
        }
        return winningMoves;
    };
    score += 1000 * countImmediateWins(side);
    score -= 1200 * countImmediateWins(SIDE(!side));

    return score;
}

int MoveEngine::AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE sideToMove,
                                SIDE perspective, int depth, int depthLimit,
                                int alpha, int beta,
                                Move *bestMove)
{
    const SIDE winner = pos->CheckBoardState();
    if (winner != NONE)
    {
        const int score = winner == perspective ? MATE_SCORE - depth : -MATE_SCORE + depth;
        return score;
    }

    if (depth >= depthLimit)
        return pos->IsBoardFilled() ? 0 : EvaluatePos(pos, perspective);

    auto generatedMoves = pos->GenerateAllMoves(sideToMove);
    if (generatedMoves.empty())
        return 0; // A full board with no winner is a draw.

    std::vector<Move> moves(generatedMoves.begin(), generatedMoves.end());
    const int center = (ROWS - 1) / 2;
    for (Move &move : moves)
    {
        pos->MakeMove(move.row, move.side);
        move.score = pos->CheckBoardState() == sideToMove ? 1 : 0;
        pos->UnmakeLastMove();
    }
    std::sort(moves.begin(), moves.end(), [](const Move &left, const Move &right)
              {
                  if (left.score != right.score)
                      return left.score > right.score;
                  return std::abs(left.row - center) < std::abs(right.row - center);
              });

    const bool maximizing = sideToMove == perspective;
    int bestScore = maximizing ? -MATE_SCORE : MATE_SCORE;
    for (Move &move : moves)
    {
        pos->MakeMove(move.row, move.side);
        move.score = AlphaBetaSearch(pos, SIDE(!sideToMove), perspective,
                                     depth + 1, depthLimit, alpha, beta, nullptr);
        pos->UnmakeLastMove();

        if ((maximizing && move.score > bestScore) || (!maximizing && move.score < bestScore))
        {
            bestScore = move.score;
            if (bestMove != nullptr)
                *bestMove = move;
        }

        if (maximizing)
        {
            alpha = std::max(alpha, bestScore);
        }
        else
            beta = std::min(beta, bestScore);

        if (alpha >= beta)
            break;
    }
    return bestScore;
}

Move MoveEngine::GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side)
{
    Move bestMove{-1, -1, side, 0};
    int score = 0;
    // Keep the best result from a completed shallower search available while
    // progressively searching deeper.
    for (int depthLimit = 1; depthLimit <= MAX_DEPTH; ++depthLimit)
    {
        Move iterationBest{-1, -1, side, 0};
        score = AlphaBetaSearch(pos, side, side, 0, depthLimit,
                                -MATE_SCORE, MATE_SCORE, &iterationBest);
        if (iterationBest.row >= 0)
        {
            bestMove = iterationBest;
        }
    }
    bestMove.score = score;
    std::cout << "SCORE: " << bestMove.score << "\n";
    return bestMove;
}
