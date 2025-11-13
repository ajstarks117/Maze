#pragma once
#include "Cell.h"
#include <chrono>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm> // for std::reverse

// =========================================
// Timer Utility
// =========================================
struct Timer {
    std::chrono::high_resolution_clock::time_point start;
    Timer() { start = std::chrono::high_resolution_clock::now(); }
    double elapsedMs() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

// =========================================
// Heuristic Functions
// =========================================
namespace Heuristic {

    // Manhattan distance between coordinates
    inline int manhattan(std::pair<int, int> a, std::pair<int, int> b) {
        return std::abs(a.first - b.first) + std::abs(a.second - b.second);
    }

    // Manhattan distance between Cell pointers
    inline int manhattan(const Cell* a, const Cell* b) {
        return std::abs(a->x - b->x) + std::abs(a->y - b->y);
    }

}

// =========================================
// Metrics Struct
// =========================================
struct Metrics {
    int pathLength = 0;
    int nodesExplored = 0;
    double timeTakenMs = 0.0;
};

// =========================================
// Path Reconstruction Helper
// =========================================
struct PathHelper {
    static std::vector<Cell*> reconstructPath(Cell* endCell) {
        std::vector<Cell*> path;
        for (Cell* c = endCell; c != nullptr; c = c->parent)
            path.push_back(c);
        std::reverse(path.begin(), path.end());
        return path;
    }
};
