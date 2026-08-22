#ifndef BOARD_H
#define BOARD_H

#include <stack>
#include <string>
#include <iostream>

#include "defs.h"
#include "move.h"

class Board
{
    uint64_t p1bitboard; // White
    uint64_t p2bitboard; // Black
    std::stack<Move> history;

    bool IsMoveValid(Move move);

public:
    Board();
    Move MakeMove(int col, SIDE side);
    void UnmakeLastMove();
    int GetLastEmptyCellOfRow(int row);
    Move GetLastMove();
    SIDE GetSideAt(int row, int col);
    void Show();
    bool IsBoardFilled();

    SIDE SetBoard(std::string &moves);
    void ResetBoard();
};

#endif