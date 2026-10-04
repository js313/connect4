#include "moveEngine.h"

int MoveEngine::CountOpenPos(const std::unique_ptr<Board> &pos, int row, int col, SIDE side)
{
    auto isInBounds = [](int r, int c)
    {
        return r >= 0 && r < ROWS && c >= 0 && c < COLS;
    };
    int cnt = 0;
    const int directions[8][2] = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1},
        {1, 1},
        {-1, -1},
        {1, -1},
        {-1, 1},
    };

    for (auto &direction : directions)
    {
        int step = 0;
        for (; step < 4; step++)
        {
            int nextRow = row + direction[0] * step;
            int nextCol = col + direction[1] * step;
            if (!isInBounds(nextRow, nextCol) || pos->GetSideAt(nextRow, nextCol) == SIDE(!side))
                break;
        }

        if (step == 4)
            cnt++;
    }

    return cnt;
}

int MoveEngine::EvaluatePos(const std::unique_ptr<Board> &pos, SIDE side)
{
    if (pos->CheckBoardState() != NONE)
        return 10000;
    int eval = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            blackOpenSides[i][j] = CountOpenPos(pos, i, j, BLACK);
            whiteOpenSides[i][j] = CountOpenPos(pos, i, j, WHITE);

            eval -= blackOpenSides[i][j];
            eval += whiteOpenSides[i][j];
        }
    }

    return eval;
}

Move MoveEngine::AlphaBetaSearch(const std::unique_ptr<Board> &pos, SIDE side, int depth, bool maximizing = false)
{
    if (depth == MAX_DEPTH)
        return {pos->GetLastMove().row, pos->GetLastMove().col, pos->GetLastMove().side, EvaluatePos(pos, pos->GetLastMove().side)};
    int maxScore = INT_MIN;
    int minScore = INT_MAX;
    Move bestMove;
    for (auto &move : pos->GenerateAllMoves(side))
    {
        pos->MakeMove(move.row, move.side);
        move.score = AlphaBetaSearch(pos, SIDE(!side), depth + 1, !maximizing).score;
        pos->UnmakeLastMove();
        if (maximizing && move.score > maxScore)
        {
            maxScore = move.score;
            bestMove = move;
        }
        else if (!maximizing && move.score < minScore)
        {
            minScore = move.score;
            bestMove = move;
        }
    }
    return bestMove;
}

Move MoveEngine::GetNextBestMove(const std::unique_ptr<Board> &pos, SIDE side)
{
    std::cout << "SCORE: " << AlphaBetaSearch(pos, side, 0).score << "\n";
    return AlphaBetaSearch(pos, side, 0);
}
