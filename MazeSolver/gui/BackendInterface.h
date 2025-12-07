#pragma once
#include <vector>
#include <string>
#include <functional>
#include "Types.h"  // Include the complete definitions
#include "../algorithms/Dijkstra.h"
#include "../algorithms/AStar.h"
#include "../algorithms/DoubleAStar.h"
#include "../algorithms/JumpPointSearch.h"
#include "../core/Utility.h"
#include "../core/Maze.h"
#include "../core/Types.h"

enum class MazeGenerator {
    RecursiveBacktracker
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

inline void BackendInterface::startSolve(PathfindingAlgorithm algorithm, int speed)
{
    // Rebuild a fresh Maze from current GUI MazeState
    int w = currentMazeState.width;
    int h = currentMazeState.height;

    Maze coreMaze(w, h);
    auto& coreCells = coreMaze.getCells();

    // Copy the GUI cells into core Maze cells
    for (int i = 0; i < w * h; i++)
    {
        coreCells[i].wall = currentMazeState.cells[i].wall;
    }

    Cell* start = coreMaze.getStart();
    Cell* goal  = coreMaze.getGoal();

    std::vector<Cell*> visited;
    std::vector<Cell*> frontier;
    std::vector<Cell*> path;

    // Animation callback (will be null until we patch solvers)
    std::function<void(Cell*, Cell*)> callback = [&](Cell* v, Cell* f){
        if (v) visited.push_back(v);
        if (f) frontier.push_back(f);

        AnimationFrame frame;
        frame.visitedCells = visited;
        frame.currentCells = frontier;
        frame.pathCells = path;

        if (onAnimationFrame)
            onAnimationFrame(frame);
    };

    AlgorithmResult result;

    switch (algorithm)
    {
        case PathfindingAlgorithm::Dijkstra:
            result = Dijkstra::solve(coreMaze, callback);
            break;

        case PathfindingAlgorithm::AStar:
            result = AStar::solve(coreMaze, callback);
            break;

        case PathfindingAlgorithm::BidirectionalAStar:
            result = DoubleAStar::solve(coreMaze, callback);
            break;

        case PathfindingAlgorithm::JumpPointSearch:
            result = JumpPointSearch::solve(coreMaze, callback);
            break;
    }

    // Final path
    path = result.path;

    AnimationFrame lastFrame;
    lastFrame.visitedCells = visited;
    lastFrame.currentCells = {};
    lastFrame.pathCells = path;

    if (onAnimationFrame)
        onAnimationFrame(lastFrame);

    if (onSolveComplete)
        onSolveComplete(result);
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