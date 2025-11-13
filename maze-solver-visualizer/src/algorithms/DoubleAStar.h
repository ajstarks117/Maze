#pragma once

#include "../core/Maze.h"
#include "../core/Utility.h"
#include "../core/AlgorithmResult.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>

class DoubleAStarSolver {
public:
    static AlgorithmResult solve(Maze& maze);
};
