#include <iostream>
#include <string>

#include "game.h"

int main()
{
    Game game;
    std::cout << "Type 'help' to get the list of available commands\n";
    while (!game.endProgram)
    {
        std::string command = game.input->GetCommand();
        game.ProcessCommand(command);
    }
    return 0;
}