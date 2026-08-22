#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

class Logger
{
public:
    static void Log(std::initializer_list<std::string> strs, bool nl = true)
    {
        for (std::string str : strs)
        {
            std::cout << str << " ";
        }
        if (nl)
            std::cout << std::endl;
    }
};

#endif