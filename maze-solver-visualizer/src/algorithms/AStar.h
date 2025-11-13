#pragma once

#include "../core/Maze.h"
#include "../core/Utility.h"
#include "../core/AlgorithmResult.h"
#include <queue>
#include <vector>

// =========================================
// A* Pathfinding Algorithm
// =========================================
class AStarSolver {
public:
    static AlgorithmResult solve(Maze& maze);
};
