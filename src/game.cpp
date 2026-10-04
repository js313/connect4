#include "game.h"
#include "logger.h"

Game::Game()
{
    pos = std::make_unique<Board>();
    moveEngine = std::make_unique<MoveEngine>();
    pos->Show();
    isGameOver = false;
    endProgram = false;
    side = WHITE;
}

SIDE Game::CheckGameState()
{
    auto getWinner = [this](SIDE side)
    {
        if (side == WHITE)
        {
            isGameOver = true;
            return WHITE;
        }
        else if (side == BLACK)
        {
            isGameOver = true;
            return BLACK;
        }
        return NONE;
    };
    for (int j = COLS - 1; j >= 0; j--)
    {
        for (int i = 0; i < ROWS; i++)
        {
            if (i > ROWS - 3 && j < 3)
                break;
            SIDE side = pos->GetSideAt(i, j);
            if (side == NONE)
                continue;
            // RIGHT
            int k = 0;
            for (; k < 4 && i + k < ROWS; k++)
            {
                if (pos->GetSideAt(i + k, j) != side)
                    break;
            }
            if (k >= 4)
                return getWinner(side);
            // DOWN
            k = 0;
            for (; k < 4 && j - k >= 0; k++)
            {
                if (pos->GetSideAt(i, j - k) != side)
                    break;
            }
            if (k >= 4)
                return getWinner(side);
            // DOWN-RIGHT
            k = 0;
            for (; k < 4 && i + k < ROWS && j - k >= 0; k++)
            {
                if (pos->GetSideAt(i + k, j - k) != side)
                    break;
            }
            if (k >= 4)
                return getWinner(side);
            // DOWN-LEFT
            k = 0;
            for (; k < 4 && i - k >= 0 && j - k >= 0; k++)
            {
                if (pos->GetSideAt(i - k, j - k) != side)
                    break;
            }
            if (k >= 4)
                return getWinner(side);
        }
    }
    if (pos->IsBoardFilled())
    {
        isGameOver = true;
        return NONE;
    }
    return NONE;
}

void Game::Update(bool ispvp = true)
{
    SIDE winningSide = NONE;
    while (!isGameOver)
    {
        int row;
        if (side && !ispvp)
        {
            Move bestMove = moveEngine->GetNextBestMove(pos, side);
            pos->MakeMove(bestMove.row, side);
        }
        else
        {
            std::cout << (side ? "BLACK> " : "WHITE> ");
            std::string command = input->GetCommand();
            if (command == "quit")
                break;
            row = stoi(command);
            row--;
            pos->MakeMove(row, side);
        }
        winningSide = CheckGameState();
        side = SIDE(!side);
        pos->Show();
    }
    if (winningSide == NONE)
        std::cout << "DRAW!\n";
    else if (winningSide == WHITE)
        std::cout << "WHITE Won!\n";
    else if (winningSide == BLACK)
        std::cout << "BLACK Won!\n";
}

void Game::Reset()
{
    isGameOver = false;
    side = WHITE;
    pos->ResetBoard();
    pos->Show();
}

void Game::SetBoard(std::string &moves)
{
    side = pos->SetBoard(moves);
    pos->Show();
}

void Game::ShowBoard()
{
    pos->Show();
}

void Game::ProcessCommand(std::string &command)
{
    if (command == "new")
    {
        Reset();
    }
    else if (command == "set")
    {
        std::string moves;
        std::cout << "Enter sequene of moves> ";
        moves = input->GetCommand();
        SetBoard(moves);
    }
    else if (command == "pvp")
    {
        Update(true);
    }
    else if (command == "pvc")
    {
        Update(false);
    }
    else if (command == "quit")
    {
        endProgram = true;
    }
    else if (command == "help")
    {
        std::cout << "Available commands:\n";
        std::cout << "  new  - Start a new game\n";
        std::cout << "  pvp  - Play player vs player\n";
        std::cout << "  pvc  - Play player vs computer\n";
        std::cout << "  set  - Set the board with a sequence of moves\n";
        std::cout << "  quit - Exit the program\n";
        std::cout << "  help - Show this help message\n";
    }
    else
    {
        std::cout << "Unrecognized command, type 'help'\n";
    }
}