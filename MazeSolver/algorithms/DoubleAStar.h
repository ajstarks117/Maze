#pragma once
#include "../core/Maze.h"
#include "../core/AlgorithmResult.h"
#include "../core/Types.h"
#include <functional>

class DoubleAStar {
public:
    static AlgorithmResult solve(
    Maze& maze,
    std::function<void(Cell*, Cell*)> stepCallback = nullptr
);
};