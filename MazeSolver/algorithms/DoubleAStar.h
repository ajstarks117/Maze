#pragma once
#include "../core/Maze.h"
#include "../core/AlgorithmResult.h"
#include "../core/Types.h"

class DoubleAStar {
public:
    static AlgorithmResult solve(Maze& maze);
};