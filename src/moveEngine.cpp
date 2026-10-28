#include <algorithm>
#include <cstdlib>
#include <climits>
#include <vector>
#include <algorithm>

#include "moveEngine.h"

// TODO: Check for beak patterns like in Mark robers video
int MoveEngine::EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side)
{
    const SIDE winner = pos->CheckBoardState();
    if (winner != NONE)
        return winner == side ? MATE_SCORE : -MATE_SCORE;
    if (pos->IsBoardFilled())
        return 0;

    int score = 0;

    // Position based scoring
    int posBasedWeights[] = {0, 1, 2, 6, 2, 1, 0};
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            SIDE pceSide = pos->GetSideAt(row, col);
            if (pceSide == side)
                score += posBasedWeights[row];
            else if (pceSide == SIDE(!side))
                score -= posBasedWeights[row];
        }
    }

    // Position + Neighbour based scoring
    int neighbourBasedWeights[] = {0, 1, 10, 100, MATE_SCORE};
    int dirs[][2] = {{1, 1}, {-1, 1}, {0, 1}, {1, 0}};

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            for (const auto &dir : dirs)
            {
                int endRow = row + dir[0] * 3;
                int endCol = col + dir[1] * 3;

                if (endRow < 0 || endRow >= ROWS || endCol < 0 || endCol >= COLS)
                    continue;

                int ownPieces = 0, opponentPieces = 0;
                for (int step = 0; step < 4; step++)
                {
                    int rowMov = row + step * dir[0];
                    int colMov = col + step * dir[1];
                    SIDE pceSide = pos->GetSideAt(rowMov, colMov);
                    if (pceSide == side)
                        ownPieces++;
                    else if (pceSide == SIDE(!side))
                        opponentPieces++;
                }

                if (ownPieces == 0)
                    score -= neighbourBasedWeights[opponentPieces];
                else if (opponentPieces == 0)
                    score += neighbourBasedWeights[ownPieces];
            }
        }
    }

    // Immediate Wins
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

int MoveEngine::MiniMaxSearch(const std::unique_ptr<Board> &pos, SIDE sideToMove, SIDE perspective, int depth, int depthLimit, Move *bestMove)
{
    const SIDE winner = pos->CheckBoardState();
    if (winner != NONE)
    {
        const int score = winner == perspective ? 2 * MATE_SCORE - depth : -2 * MATE_SCORE + depth;
        return score;
    }

    if (pos->IsBoardFilled())
        return 0;

    if (depth >= depthLimit)
        return EvaluatePos(pos, perspective);

    std::list<Move> allMoves = pos->GenerateAllMoves(sideToMove);
    if (allMoves.empty())
        return 0;

    bool maximizing = (sideToMove == perspective);
    int bestScore = maximizing ? INT_MIN : INT_MAX;

    for (auto &move : allMoves)
    {
        if (!pos->MakeMove(move.row, sideToMove))
            continue;

        int score = MiniMaxSearch(pos, SIDE(!sideToMove), perspective, depth + 1, depthLimit, nullptr);
        pos->UnmakeLastMove();
        move.score = score;

        if ((maximizing && move.score >= bestScore) || (!maximizing && move.score <= bestScore))
        {
            bestScore = move.score;
            if (bestMove != nullptr)
            {
                std::cout << move.row << " " << score << "\n";
                *bestMove = move;
            }
        }
    }

    return bestScore;
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

    if (pos->IsBoardFilled())
        return 0;

    if (depth >= depthLimit)
        return EvaluatePos(pos, perspective);

    std::list<Move> allMoves = pos->GenerateAllMoves(sideToMove);
    if (allMoves.empty())
        return 0;

    // Move ordering for better alpha beta pruning
    std::vector<Move> moves(allMoves.begin(), allMoves.end());
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
                  return std::abs(left.row - center) < std::abs(right.row - center); });

    bool maximizing = (sideToMove == perspective);
    int bestScore = maximizing ? INT_MIN : INT_MAX;

    for (auto &move : moves)
    {
        if (!pos->MakeMove(move.row, sideToMove))
            continue;

        move.score = AlphaBetaSearch(pos, SIDE(!sideToMove), perspective, depth + 1, depthLimit, alpha, beta, nullptr);
        pos->UnmakeLastMove();

        if ((maximizing && move.score > bestScore) || (!maximizing && move.score < bestScore))
        {
            bestScore = move.score;
            if (bestMove != nullptr)
            {
                std::cout << move.row << " " << move.score << "\n";
                *bestMove = move;
            }
        }

        if (maximizing)
            alpha = std::max(alpha, bestScore);
        else
            beta = std::min(beta, bestScore);

        if (alpha >= beta)
            break;
    }

    return bestScore;
}

Move MoveEngine::GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side)
{
    Move bestMove = {-1, -1, side, 0};
    std::list<Move> allMoves = pos->GenerateAllMoves(side);

    // int bs = MiniMaxSearch(pos, side, side, 0, 8, &bestMove);
    int bs = AlphaBetaSearch(pos, side, side, 0, 8, INT_MIN, INT_MAX, &bestMove);
    std::cout << bestMove.row << "," << bestMove.col << ": " << bestMove.score << std::endl;

    return bestMove;
}