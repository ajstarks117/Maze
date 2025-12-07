#include <iostream>
#include <iomanip>
#include <limits>
#include "core/Utility.h"
#include "core/Maze.h"
#include "core/Types.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/AStar.h"
#include "algorithms/DoubleAStar.h"
#include "algorithms/JumpPointSearch.h"

void printMetrics(const std::string& algorithmName, const AlgorithmResult& result) {
    std::cout << "\n===== " << algorithmName << " =====\n";
    std::cout << "Path Length: " << result.metrics.pathLength << "\n";
    std::cout << "Nodes Explored: " << result.metrics.nodesExplored << "\n";
    
    if (result.metrics.timeTakenMs == -1) {
        std::cout << "Time: FAILED\n";
    } else if (result.metrics.timeTakenMs < 1000) {
        std::cout << "Time: " << result.metrics.timeTakenMs << " μs\n";
    } else {
        std::cout << "Time: " << (result.metrics.timeTakenMs / 1000.0) << " ms\n";
    }
    
    std::cout << "Success: " << (result.success ? "Yes" : "No") << "\n";
}

void printRobustMetrics(const std::string& algorithmName, const RobustMetrics& metrics) {
    std::cout << "\n===== " << algorithmName << " (Robust Analysis) =====\n";
    std::cout << "Successful Runs: " << metrics.successfulRuns << "/" << metrics.totalRuns << "\n";
    
    if (metrics.successfulRuns > 0) {
        std::cout << "Best Time: " << metrics.bestTime << " μs\n";
        std::cout << "Worst Time: " << metrics.worstTime << " μs\n";
        std::cout << "Average Time: " << metrics.averageTime << " μs\n";
        std::cout << "Median Time: " << metrics.medianTime << " μs\n";
        std::cout << "Std Deviation: " << std::fixed << std::setprecision(2) 
                  << metrics.standardDeviation << " μs\n";
    } else {
        std::cout << "No successful runs!\n";
    }
}

void displayMenu() {
    std::cout << "\n"
                 "🚀 Interactive Maze Solver - C++17 Terminal Version\n"
                 "==================================================\n"
                 "1. Set Maze Size (Current: 21x21)\n"
                 "2. Generate New Maze\n"
                 "3. Solve with Dijkstra\n"
                 "4. Solve with A*\n"
                 "5. Solve with Bidirectional A*\n"
                 "6. Solve with Jump Point Search\n"
                 "7. Solve with All Algorithms (Compare)\n"
                 "8. Robust Analysis (Multiple Runs)\n"
                 "9. Display Current Maze\n"
                 "10. Exit\n"
                 "Choose an option (1-10): ";
}

int getIntegerInput(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between " 
                      << minVal << " and " << maxVal << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

void solveWithAlgorithm(Maze& maze, int choice) {
    AlgorithmResult result;
    std::string algoName;
    
    auto algorithm = [choice](Maze& m) -> AlgorithmResult {
        switch (choice) {
            case 3: return Dijkstra::solve(m);
            case 4: return AStar::solve(m);
            case 5: return DoubleAStar::solve(m);
            case 6: return JumpPointSearch::solve(m);
            default: return AlgorithmResult();
        }
    };
    
    switch (choice) {
        case 3: algoName = "Dijkstra"; break;
        case 4: algoName = "A*"; break;
        case 5: algoName = "Bidirectional A*"; break;
        case 6: algoName = "Jump Point Search"; break;
        default: return;
    }
    
    std::cout << "\nRunning " << algoName << "...\n";
    result = Utility::runAlgorithmSafely(maze, algorithm);
    
    printMetrics(algoName, result);
    
    if (result.success) {
        std::cout << "\nPath Visualization:\n";
        maze.markPath(result.path);
        std::cout << maze.toAscii() << std::endl;
        maze.reset();
    } else {
        std::cout << "No valid path found!\n";
    }
}

void solveAllAlgorithms(Maze& maze) {
    std::cout << "\n🧪 Running All Algorithms for Comparison...\n";
    
    // UPDATED: Wrapped in lambdas to handle the stepCallback parameter mismatch
    AlgorithmResult dijkstra = Utility::runAlgorithmSafely(maze, [](Maze& m){ return Dijkstra::solve(m); });
    AlgorithmResult astar = Utility::runAlgorithmSafely(maze, [](Maze& m){ return AStar::solve(m); });
    AlgorithmResult doubleAstar = Utility::runAlgorithmSafely(maze, [](Maze& m){ return DoubleAStar::solve(m); });
    AlgorithmResult jps = Utility::runAlgorithmSafely(maze, [](Maze& m){ return JumpPointSearch::solve(m); });
    
    // Display individual results
    printMetrics("Dijkstra", dijkstra);
    printMetrics("A*", astar);
    printMetrics("Bidirectional A*", doubleAstar);
    printMetrics("Jump Point Search", jps);
    
    // Display comparison table
    std::cout << "\n📊 PERFORMANCE COMPARISON\n";
    std::cout << "=========================\n";
    std::cout << std::left << std::setw(20) << "Algorithm" 
              << std::setw(12) << "Path Length" 
              << std::setw(15) << "Nodes Explored" 
              << std::setw(10) << "Time" 
              << std::setw(8) << "Success" << "\n";
    
    std::cout << std::string(65, '-') << "\n";
    
    auto printRow = [](const std::string& name, const AlgorithmResult& result) {
        std::string timeStr;
        if (result.metrics.timeTakenMs == -1) {
            timeStr = "FAILED";
        } else if (result.metrics.timeTakenMs < 1000) {
            timeStr = std::to_string(result.metrics.timeTakenMs) + " μs";
        } else {
            timeStr = std::to_string(result.metrics.timeTakenMs / 1000.0) + " ms";
        }
        
        std::cout << std::left << std::setw(20) << name
                  << std::setw(12) << result.metrics.pathLength
                  << std::setw(15) << result.metrics.nodesExplored
                  << std::setw(10) << timeStr
                  << std::setw(8) << (result.success ? "Yes" : "No") << "\n";
    };
    
    printRow("Dijkstra", dijkstra);
    printRow("A*", astar);
    printRow("Bidirectional A*", doubleAstar);
    printRow("Jump Point Search", jps);
    
    // Show visualization of the fastest successful algorithm
    AlgorithmResult* fastest = nullptr;
    if (astar.success) fastest = &astar;
    if (jps.success && (!fastest || jps.metrics.timeTakenMs < fastest->metrics.timeTakenMs)) fastest = &jps;
    if (dijkstra.success && (!fastest || dijkstra.metrics.timeTakenMs < fastest->metrics.timeTakenMs)) fastest = &dijkstra;
    if (doubleAstar.success && (!fastest || doubleAstar.metrics.timeTakenMs < fastest->metrics.timeTakenMs)) fastest = &doubleAstar;
    
    if (fastest) {
        std::cout << "\nFastest Algorithm Path Visualization:\n";
        maze.markPath(fastest->path);
        std::cout << maze.toAscii() << std::endl;
        maze.reset();
    }
}

void runRobustAnalysis(Maze& maze) {
    std::cout << "\n🔬 Running Robust Analysis (3 runs per algorithm)...\n";
    
    // UPDATED: Wrapped in lambdas here as well
    RobustMetrics dijkstraMetrics = Utility::runAlgorithmMultipleTimes(maze, [](Maze& m){ return Dijkstra::solve(m); }, 3);
    RobustMetrics astarMetrics = Utility::runAlgorithmMultipleTimes(maze, [](Maze& m){ return AStar::solve(m); }, 3);
    RobustMetrics doubleAstarMetrics = Utility::runAlgorithmMultipleTimes(maze, [](Maze& m){ return DoubleAStar::solve(m); }, 3);
    RobustMetrics jpsMetrics = Utility::runAlgorithmMultipleTimes(maze, [](Maze& m){ return JumpPointSearch::solve(m); }, 3);
    
    printRobustMetrics("Dijkstra", dijkstraMetrics);
    printRobustMetrics("A*", astarMetrics);
    printRobustMetrics("Bidirectional A*", doubleAstarMetrics);
    printRobustMetrics("Jump Point Search", jpsMetrics);
}

int main() {
    int mazeSize = 21;
    Maze maze(mazeSize, mazeSize);
    int choice;
    
    std::cout << "🚀 Interactive Maze Solver - C++17 Terminal Version\n";
    std::cout << "==================================================\n";
    
    do {
        displayMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number 1-10.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                int newSize = getIntegerInput("Enter maze size (minimum 5, maximum 101): ", 5, 101);
                mazeSize = newSize;
                maze = Maze(mazeSize, mazeSize);
                std::cout << "Maze size set to " << mazeSize << "x" << mazeSize << "\n";
                break;
            }
            
            case 2:
                maze.generate();
                std::cout << "New maze generated!\n";
                std::cout << "\nCurrent Maze:\n";
                std::cout << maze.toAscii() << std::endl;
                break;
            
            case 3:
            case 4:
            case 5:
            case 6:
                if (maze.getStart() && maze.getGoal()) {
                    solveWithAlgorithm(maze, choice);
                } else {
                    std::cout << "Please generate a maze first!\n";
                }
                break;
            
            case 7:
                if (maze.getStart() && maze.getGoal()) {
                    solveAllAlgorithms(maze);
                } else {
                    std::cout << "Please generate a maze first!\n";
                }
                break;
                
            case 8:
                if (maze.getStart() && maze.getGoal()) {
                    runRobustAnalysis(maze);
                } else {
                    std::cout << "Please generate a maze first!\n";
                }
                break;
            
            case 9:
                std::cout << "\nCurrent Maze:\n";
                std::cout << maze.toAscii() << std::endl;
                break;
            
            case 10:
                std::cout << "Thank you for using Maze Solver! Goodbye!\n";
                break;
            
            default:
                std::cout << "Invalid choice! Please enter 1-10.\n";
                break;
        }
        
    } while (choice != 10);
    
    return 0;
}