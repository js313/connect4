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
    p1bitboard &= 0;
    p2bitboard &= 0;
    while (!history.empty())
        history.pop();
}

bool Board::IsMoveValid(Move move)
{
    return true;
}

Move Board::MakeMove(int row, SIDE side)
{
    int col = GetLastEmptyCellOfRow(row);
    Move move = {row, col, side};
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

    return move;
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
    uint64_t mergedBoard = p1bitboard | p2bitboard;
    uint64_t mask = (1 << row) << 16;
    int emptyCell = 6;

    while (!(mergedBoard & mask) && emptyCell > 0)
    {
        mask <<= 8;
        emptyCell--;
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
    return false;
}

std::list<Move> Board::GenerateAllMoves(SIDE side)
{
    std::list<Move> legalMoveList;
    for (int i = 0; i < ROWS; i++)
    {
        int col = GetLastEmptyCellOfRow(i);
        if (col > -1)
        {
            legalMoveList.push_back({i, col, side});
        }
    }

    return legalMoveList;
}