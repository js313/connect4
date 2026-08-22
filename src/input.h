#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <memory>

class Input
{
public:
    Input() = default;
    std::string GetCommand();
};

#endif