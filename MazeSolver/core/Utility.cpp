#include "Utility.h"
#include "Maze.h"
#include "AlgorithmResult.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <thread>

// RobustTimer implementation
void RobustTimer::start(long long timeout_ms) {
    start_time = std::chrono::high_resolution_clock::now();
    timeout_flag = false;
    
    // Start timeout thread
    timeout_thread = std::thread([this, timeout_ms]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
        timeout_flag = true;
    });
}

long long RobustTimer::stop() {
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // Stop timeout thread
    if (timeout_thread.joinable()) {
        timeout_thread.detach(); // Detach to avoid blocking
    }
    
    auto micro = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time).count();
    return micro > 0 ? micro : 1;
}

bool RobustTimer::isTimeout() const {
    return timeout_flag.load();
}

// TimeoutTimer implementation
TimeoutTimer::TimeoutTimer(long long timeout_ms) : timeout_ms(timeout_ms) {
    start_time = std::chrono::high_resolution_clock::now();
}

bool TimeoutTimer::isTimeout() {
    auto current = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        current - start_time).count();
    return elapsed > timeout_ms;
}

// Rest of Utility functions (keep the same implementations)
double Utility::manhattanDistance(const Cell* a, const Cell* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

std::vector<Cell*> Utility::reconstructPath(Cell* endNode) {
    std::vector<Cell*> path;
    Cell* current = endNode;
    
    while (current != nullptr) {
        path.push_back(current);
        current = current->parent;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

long long Utility::getCellKey(int x, int y) {
    return ((long long)x << 32) | (y & 0xFFFFFFFFLL);
}

bool Utility::validatePath(Maze& maze, const std::vector<Cell*>& path) {
    if (path.empty()) {
        std::cout << "VALIDATION FAILED: Empty path\n";
        return false;
    }
    if (path.front() != maze.getStart()) {
        std::cout << "VALIDATION FAILED: Path doesn't start at start position\n";
        return false;
    }
    if (path.back() != maze.getGoal()) {
        std::cout << "VALIDATION FAILED: Path doesn't end at goal position\n";
        return false;
    }
    
    for (size_t i = 1; i < path.size(); ++i) {
        int dx = std::abs(path[i]->x - path[i-1]->x);
        int dy = std::abs(path[i]->y - path[i-1]->y);
        
        if (dx + dy != 1) {
            std::cout << "VALIDATION FAILED: Non-adjacent cells at step " << i 
                      << ": (" << path[i-1]->x << "," << path[i-1]->y 
                      << ") -> (" << path[i]->x << "," << path[i]->y << ")\n";
            return false;
        }
        
        if (path[i]->wall) {
            std::cout << "VALIDATION FAILED: Path goes through wall at (" 
                      << path[i]->x << "," << path[i]->y << ")\n";
            return false;
        }
    }
    
    return true;
}

AlgorithmResult Utility::runAlgorithmSafely(Maze& maze, std::function<AlgorithmResult(Maze&)> algorithm) {
    AlgorithmResult result;
    
    try {
        result = algorithm(maze);
        
        if (result.success && !validatePath(maze, result.path)) {
            result.success = false;
            std::cout << "⚠️  Path validation failed!\n";
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.metrics.timeTakenMs = -1;
        std::cout << "⚠️  Algorithm crashed: " << e.what() << "\n";
    } catch (...) {
        result.success = false;
        result.metrics.timeTakenMs = -1;
        std::cout << "⚠️  Algorithm crashed with unknown exception!\n";
    }
    
    return result;
}

RobustMetrics Utility::runAlgorithmMultipleTimes(Maze& maze, std::function<AlgorithmResult(Maze&)> algorithm, int runs) {
    RobustMetrics metrics;
    std::vector<long long> times;
    metrics.successfulRuns = 0;
    metrics.totalRuns = runs;
    
    for (int i = 0; i < runs; ++i) {
        std::cout << "Run " << (i+1) << "/" << runs << "... ";
        AlgorithmResult result = runAlgorithmSafely(maze, algorithm);
        
        if (result.success && result.metrics.timeTakenMs > 0) {
            times.push_back(result.metrics.timeTakenMs);
            metrics.successfulRuns++;
            std::cout << "✓ (" << result.metrics.timeTakenMs << " μs)\n";
        } else {
            std::cout << "✗ (Failed)\n";
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    if (!times.empty()) {
        std::sort(times.begin(), times.end());
        metrics.bestTime = times.front();
        metrics.worstTime = times.back();
        metrics.medianTime = times[times.size() / 2];
        
        long long sum = std::accumulate(times.begin(), times.end(), 0LL);
        metrics.averageTime = sum / times.size();
        
        double variance = 0.0;
        for (auto t : times) {
            variance += std::pow(t - metrics.averageTime, 2);
        }
        metrics.standardDeviation = std::sqrt(variance / times.size());
    } else {
        metrics.bestTime = metrics.worstTime = metrics.averageTime = metrics.medianTime = -1;
        metrics.standardDeviation = 0.0;
    }
    
    return metrics;
}