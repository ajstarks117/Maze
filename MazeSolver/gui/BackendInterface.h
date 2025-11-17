#pragma once
#include <vector>
#include <string>
#include <functional>
#include "Types.h"  // Include the complete definitions

enum class MazeGenerator {
    RecursiveBacktracker,
    Prims,
    Kruskals,
    DFSRandomized
};

enum class PathfindingAlgorithm {
    Dijkstra,
    AStar,
    BidirectionalAStar,
    JumpPointSearch
};

struct AlgorithmResult {
    bool success = false;
    int pathLength = 0;
    int nodesExplored = 0;
    long long timeTakenMs = 0;
    std::vector<Cell*> path;
    std::vector<Cell*> visitedOrder;
};

struct AnimationFrame {
    std::vector<Cell*> currentCells;
    std::vector<Cell*> visitedCells;
    std::vector<Cell*> pathCells;
    bool isComplete = false;
};

// Backend interface - you'll implement these functions
class BackendInterface {
public:
    // Maze generation
    static void generateMaze(int width, int height, MazeGenerator type);
    
    // Pathfinding
    static void startSolve(PathfindingAlgorithm algorithm, int speed);
    static void reset();
    static void step();
    
    // State queries
    static MazeState getCurrentMazeState();
    static AnimationFrame getCurrentAnimationFrame();
    static AlgorithmResult getResults();
    
    // Callbacks for UI updates
    static inline std::function<void()> onMazeGenerated;
    static inline std::function<void(const AnimationFrame&)> onAnimationFrame;
    static inline std::function<void(const AlgorithmResult&)> onSolveComplete;
    
    // Dummy implementations for compilation
    static inline MazeState currentMazeState;
    static inline AnimationFrame currentAnimationFrame;
    static inline AlgorithmResult currentResults;
};

// Dummy implementations (remove when connecting to real backend)
inline void BackendInterface::generateMaze(int width, int height, MazeGenerator type) {
    currentMazeState = MazeState(width, height);
    
    // Generate dummy maze cells (simple border maze)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool wall = (x == 0 || y == 0 || x == width-1 || y == height-1);
            currentMazeState.cells.emplace_back(x, y, wall);
        }
    }
    
    // Set start and end
    if (width > 2 && height > 2) {
        currentMazeState.startCell = &currentMazeState.cells[1 * width + 1];
        currentMazeState.endCell = &currentMazeState.cells[(height-2) * width + (width-2)];
        currentMazeState.startCell->wall = false;
        currentMazeState.endCell->wall = false;
    }
    
    if (onMazeGenerated) onMazeGenerated();
}

inline MazeState BackendInterface::getCurrentMazeState() {
    return currentMazeState;
}

inline AnimationFrame BackendInterface::getCurrentAnimationFrame() {
    return currentAnimationFrame;
}

inline AlgorithmResult BackendInterface::getResults() {
    return currentResults;
}

// Stub implementations (you'll replace these)
inline void BackendInterface::startSolve(PathfindingAlgorithm algorithm, int speed) {
    // Create dummy animation for testing
    currentAnimationFrame = AnimationFrame();
    auto mazeState = getCurrentMazeState();
    
    // Add some dummy visited cells for visualization
    for (int i = 1; i < mazeState.width - 1 && i < mazeState.height - 1; i++) {
        if (i < mazeState.cells.size()) {
            currentAnimationFrame.visitedCells.push_back(&mazeState.cells[i]);
        }
    }
    
    // Add dummy path
    if (mazeState.startCell && mazeState.endCell) {
        currentAnimationFrame.pathCells.push_back(mazeState.startCell);
        currentAnimationFrame.pathCells.push_back(mazeState.endCell);
    }
    
    // Set dummy results
    currentResults.success = true;
    currentResults.pathLength = 10;
    currentResults.nodesExplored = 50;
    currentResults.timeTakenMs = 100;
    
    if (onAnimationFrame) onAnimationFrame(currentAnimationFrame);
    if (onSolveComplete) onSolveComplete(currentResults);
}

inline void BackendInterface::reset() {
    currentAnimationFrame = AnimationFrame();
    currentResults = AlgorithmResult();
}

inline void BackendInterface::step() {
    // Simple step implementation for testing
    auto mazeState = getCurrentMazeState();
    if (currentAnimationFrame.visitedCells.size() < mazeState.cells.size() / 2) {
        int nextIndex = currentAnimationFrame.visitedCells.size() + 1;
        if (nextIndex < mazeState.cells.size()) {
            currentAnimationFrame.visitedCells.push_back(&mazeState.cells[nextIndex]);
        }
    }
    
    if (onAnimationFrame) onAnimationFrame(currentAnimationFrame);
}