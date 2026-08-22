#ifndef GAME_H
#define GAME_H

#include <memory>

#include "board.h"
#include "defs.h"
#include "input.h"
#include "moveEngine.h"

class Game
{
    bool isGameOver;
    SIDE side;
    std::unique_ptr<Board> pos;
    std::unique_ptr<MoveEngine> moveEngine;

public:
    bool endProgram;
    std::unique_ptr<Input> input;
    Game();

    void Update();
    SIDE CheckGameState();
    void Reset();
    void SetBoard(std::string &moves);
    void ShowBoard();

    void ProcessCommand(std::string &command);
};

#endif