#pragma once
#include "../core/Maze.h"
#include "../core/AlgorithmResult.h"
#include "../core/Types.h"

class Dijkstra {
public:
    static AlgorithmResult solve(Maze& maze);
};