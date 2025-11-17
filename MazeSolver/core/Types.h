#pragma once
#include <vector>

// Forward declarations
struct Cell;
class Maze;

struct Metrics {
    size_t pathLength = 0;
    size_t nodesExplored = 0;
    long long timeTakenMs = 0;
    
    Metrics() : pathLength(0), nodesExplored(0), timeTakenMs(0) {}
};

struct RobustMetrics {
    long long bestTime = 0;
    long long worstTime = 0; 
    long long averageTime = 0;
    long long medianTime = 0;
    double standardDeviation = 0.0;
    int successfulRuns = 0;
    int totalRuns = 0;
};

struct AlgorithmResult {
    std::vector<Cell*> path;
    std::vector<Cell*> visitedOrder;
    Metrics metrics;
    bool success = false;
    
    AlgorithmResult() : success(false) {}
};