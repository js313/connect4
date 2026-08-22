#ifndef MOVE_H
#define MOVE_H

#include "defs.h"

struct Move
{
public:
    int row; // 0-7
    int col; // 0-6
    SIDE side;
};

#endif