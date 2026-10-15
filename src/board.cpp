#include <iostream>

#include "board.h"
#include "logger.h"

Board::Board()
{
    // memset(pos, NONE, sizeof(SIDE) * COLS * ROWS);  // Doesn't work as it is setting NONE byte by bytes not actually 8 bytes at a time which we expect for int vals
    ResetBoard();
    std::cout << "Board initialized\n";
}

SIDE Board::SetBoard(std::string &moves)
{
    SIDE side = WHITE;
    for (auto &c : moves)
    {
        int row = c - '0';
        if (row < 1 || row > 7)
            continue;

        MakeMove(--row, side);
        side = SIDE(!side);
    }

    return side;
}

void Board::ResetBoard()
{
    p1bitboard = 0;
    p2bitboard = 0;
    while (!history.empty())
        history.pop();
}

bool Board::IsMoveValid(Move move)
{
    return move.row >= 0 && move.row < ROWS &&
           move.col >= 0 && move.col < COLS &&
           GetLastEmptyCellOfRow(move.row) == move.col;
}

bool Board::MakeMove(int row, SIDE side)
{
    int col = GetLastEmptyCellOfRow(row);
    Move move = {row, col, side, 0};
    if (!IsMoveValid(move))
        return false;
    // std::cout << row << " " << col << " " << side << " " << MASK(row, col) << " " << (((uint64_t)1) << ((uint)48)) << "\n";

    history.push(move);
    if (side == WHITE)
    {
        p1bitboard |= MASK(row, col);
    }
    else
    {
        p2bitboard |= MASK(row, col);
    }

    return true;
}

void Board::UnmakeLastMove()
{
    ASSERT(!history.empty());

    Move lastMove = history.top();
    history.pop();

    if (lastMove.side == WHITE)
        p1bitboard &= (~MASK(lastMove.row, lastMove.col));
    else
        p2bitboard &= (~MASK(lastMove.row, lastMove.col));
}

int Board::GetLastEmptyCellOfRow(int row)
{
    if (row < 0 || row >= ROWS)
        return COLS;

    uint64_t mergedBoard = p1bitboard | p2bitboard;
    // Bitboard columns are encoded bottom-up from bit 56 toward bit 16.
    uint64_t mask = static_cast<uint64_t>(1) << (56 + row);
    int emptyCell = 0;

    while ((mergedBoard & mask) && emptyCell < COLS)
    {
        mask >>= 8;
        ++emptyCell;
    }
    return emptyCell;
}

void Board::Show()
{
    for (int j = COLS - 1; j >= 0; j--)
    {
        for (int i = 0; i < ROWS; i++)
        {
            SIDE side = GetSideAt(i, j);
            switch (side)
            {
            case WHITE:
                std::cout << SIDE_REPR[WHITE];
                break;
            case BLACK:
                std::cout << SIDE_REPR[BLACK];
                break;
            default:
                std::cout << SIDE_REPR[NONE];
                break;
            }
            std::cout << "  ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < ROWS - 1; i++)
    {
        std::cout << "---";
    }

    std::cout << "-";
    std::cout << std::endl;
}

Move Board::GetLastMove()
{
    return history.top();
}

SIDE Board::GetSideAt(int row, int col)
{
    if (p1bitboard & MASK(row, col))
        return WHITE;
    else if (p2bitboard & MASK(row, col))
        return BLACK;
    return NONE;
}

bool Board::IsBoardFilled()
{
    for (int row = 0; row < ROWS; ++row)
    {
        if (GetLastEmptyCellOfRow(row) < COLS)
            return false;
    }
    return true;
}

std::list<Move> Board::GenerateAllMoves(SIDE side)
{
    std::list<Move> legalMoveList = std::list<Move>();
    for (int i = 0; i < ROWS; i++)
    {
        int col = GetLastEmptyCellOfRow(i);
        if (col < COLS)
            legalMoveList.push_back({i, col, side, 0});
    }

    return legalMoveList;
}

SIDE Board::CheckBoardState()
{
    for (int j = COLS - 1; j >= 0; j--)
    {
        for (int i = 0; i < ROWS; i++)
        {
            SIDE side = GetSideAt(i, j);
            if (side == NONE)
                continue;
            // RIGHT
            int k = 0;
            for (; k < 4 && i + k < ROWS; k++)
            {
                if (GetSideAt(i + k, j) != side)
                    break;
            }
            if (k >= 4)
                return side;
            // DOWN
            k = 0;
            for (; k < 4 && j - k >= 0; k++)
            {
                if (GetSideAt(i, j - k) != side)
                    break;
            }
            if (k >= 4)
                return side;
            // DOWN-RIGHT
            k = 0;
            for (; k < 4 && i + k < ROWS && j - k >= 0; k++)
            {
                if (GetSideAt(i + k, j - k) != side)
                    break;
            }
            if (k >= 4)
                return side;
            // DOWN-LEFT
            k = 0;
            for (; k < 4 && i - k >= 0 && j - k >= 0; k++)
            {
                if (GetSideAt(i - k, j - k) != side)
                    break;
            }
            if (k >= 4)
                return side;
        }
    }
    return NONE;
}
