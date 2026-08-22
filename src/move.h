#ifndef MOVE_H
#define MOVE_H

#include "defs.h"

struct Move
{
public:
    int row; // 0-6
    int col; // 0-7
    SIDE side;
    int score;
};

#endif