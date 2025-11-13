#pragma once

#include "../core/Maze.h"
#include "../core/Utility.h"
#include "../core/AlgorithmResult.h"
#include <queue>
#include <vector>

class DijkstraSolver {
public:
    static AlgorithmResult solve(Maze& maze);
};
