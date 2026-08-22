#include "input.h"

std::string Input::GetCommand()
{
    std::string command;
    std::cin >> command;

    return command;
}