#pragma once
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <QCoreApplication> 
#include <QThread>

#include "Types.h"
#include "../core/Maze.h"
#include "../core/Utility.h"
#include "../algorithms/Dijkstra.h"
#include "../algorithms/AStar.h"
#include "../algorithms/DoubleAStar.h"
#include "../algorithms/JumpPointSearch.h"

enum class MazeGenerator { RecursiveBacktracker };
enum class PathfindingAlgorithm { Dijkstra, AStar, BidirectionalAStar, JumpPointSearch };

struct AnimationFrame {
    std::vector<Cell*> visitedCells;
    std::vector<Cell*> currentCells;
    std::vector<Cell*> pathCells;
};

// [SINGLETON FIX] This class guarantees only ONE maze exists
class BackendInterface {
private:
    Maze* globalMaze = nullptr;

    // Private Constructor
    BackendInterface() {
        globalMaze = new Maze(21, 21); // Default maze on startup
    }

public:
    // Delete copy constructors to prevent duplicates
    BackendInterface(const BackendInterface&) = delete;
    void operator=(const BackendInterface&) = delete;

    // Access the single shared instance
    static BackendInterface& get() {
        static BackendInterface instance;
        return instance;
    }

    // Signals
    std::function<void()> onMazeGenerated;
    std::function<void(const AnimationFrame&)> onAnimationFrame;
    std::function<void(const AlgorithmResult&)> onSolveComplete;

    void generateMaze(int width, int height, MazeGenerator type) {
        (void)type;
        if (globalMaze) delete globalMaze;
        globalMaze = new Maze(width, height); // This generates the maze
        if (onMazeGenerated) onMazeGenerated();
    }

    MazeState getCurrentMazeState() {
        if (!globalMaze) return MazeState(21, 21);

        MazeState state(globalMaze->getWidth(), globalMaze->getHeight());
        auto& coreCells = globalMaze->getCells();
        state.cells.clear();
        
        // Convert Core Cells to GUI Cells
        for (const auto& c : coreCells) {
            // IMPORTANT: Copy the 'wall' and 'visited' status correctly
            Cell guiCell(c.x, c.y, c.wall);
            guiCell.visited = c.visited;
            state.cells.push_back(guiCell);
        }

        if (globalMaze->getStart()) 
            state.startCell = &state.cells[globalMaze->getStart()->y * state.width + globalMaze->getStart()->x];
        
        if (globalMaze->getGoal()) 
            state.endCell = &state.cells[globalMaze->getGoal()->y * state.width + globalMaze->getGoal()->x];

        return state;
    }

    void startSolve(PathfindingAlgorithm algorithm, int speed) {
        if (!globalMaze) return;
        globalMaze->reset(); 

        std::vector<Cell*> visited;
        std::vector<Cell*> frontier;

        auto stepCallback = [&](Cell* v, Cell* f) {
            if (v) visited.push_back(v);
            if (f) frontier.push_back(f);

            if (speed < 10) {
                unsigned long delay = (11 - speed) * 5; 
                QThread::msleep(delay);
            }

            AnimationFrame frame;
            frame.visitedCells = visited;
            frame.currentCells = frontier;

            if (onAnimationFrame) onAnimationFrame(frame);
            QCoreApplication::processEvents(); 
        };

        AlgorithmResult result;
        switch (algorithm) {
            case PathfindingAlgorithm::Dijkstra: result = Dijkstra::solve(*globalMaze, stepCallback); break;
            case PathfindingAlgorithm::AStar: result = AStar::solve(*globalMaze, stepCallback); break;
            case PathfindingAlgorithm::BidirectionalAStar: result = DoubleAStar::solve(*globalMaze, stepCallback); break;
            case PathfindingAlgorithm::JumpPointSearch: result = JumpPointSearch::solve(*globalMaze, stepCallback); break;
        }

        AnimationFrame finalFrame;
        finalFrame.visitedCells = visited;
        finalFrame.pathCells = result.path;
        
        if (onAnimationFrame) onAnimationFrame(finalFrame);
        if (onSolveComplete) onSolveComplete(result);
    }

    void reset() {
        if (globalMaze) globalMaze->reset();
        if (onMazeGenerated) onMazeGenerated();
    }

    void step() {} // Placeholder
};