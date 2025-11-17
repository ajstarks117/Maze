#pragma once
#include <chrono>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>
#include "Types.h"

// Forward declaration
class Maze;

class RobustTimer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::atomic<bool> timeout_flag{false};
    std::thread timeout_thread;
    
public:
    void start(long long timeout_ms = 5000);
    long long stop();
    bool isTimeout() const;
};

class TimeoutTimer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    long long timeout_ms;
    
public:
    TimeoutTimer(long long timeout_ms = 5000);
    bool isTimeout();
};

namespace Utility {
    double manhattanDistance(const Cell* a, const Cell* b);
    std::vector<Cell*> reconstructPath(Cell* endNode);
    long long getCellKey(int x, int y);
    bool validatePath(Maze& maze, const std::vector<Cell*>& path);
    AlgorithmResult runAlgorithmSafely(Maze& maze, std::function<AlgorithmResult(Maze&)> algorithm);
    RobustMetrics runAlgorithmMultipleTimes(Maze& maze, std::function<AlgorithmResult(Maze&)> algorithm, int runs = 3);
}