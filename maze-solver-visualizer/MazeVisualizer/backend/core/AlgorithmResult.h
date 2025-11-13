#pragma once
#include <vector>
#include "Cell.h"
#include "Utility.h"

// =========================================
// Shared structure for all algorithms
// =========================================
struct AlgorithmResult {
    std::vector<Cell*> path;
    std::vector<Cell*> visitedOrder;
    Metrics metrics;
};
