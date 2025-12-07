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

enum class MazeGenerator { RecursiveBacktracker, Prims, Kruskals };
enum class PathfindingAlgorithm { Dijkstra, AStar, BidirectionalAStar, JumpPointSearch };

struct AnimationFrame {
    std::vector<Cell*> visitedCells;
    std::vector<Cell*> currentCells;
    std::vector<Cell*> pathCells;
};

class BackendInterface {
private:
    // [FIX] 'inline static' ensures there is exactly ONE maze shared by the whole app.
    // No extra .cpp file needed.
    inline static Maze* globalMaze = nullptr;

public:
    static inline std::function<void()> onMazeGenerated;
    static inline std::function<void(const AnimationFrame&)> onAnimationFrame;
    static inline std::function<void(const AlgorithmResult&)> onSolveComplete;

    static void generateMaze(int width, int height, MazeGenerator type) {
        (void)type; 
        if (globalMaze) delete globalMaze;
        
        // Create the maze. This triggers the generation logic.
        globalMaze = new Maze(width, height);
        
        if (onMazeGenerated) onMazeGenerated();
    }

    static MazeState getCurrentMazeState() {
        if (!globalMaze) return MazeState(21, 21);

        MazeState state(globalMaze->getWidth(), globalMaze->getHeight());
        auto& coreCells = globalMaze->getCells();
        state.cells.clear();
        state.cells.reserve(coreCells.size());

        for (const auto& c : coreCells) {
            Cell guiCell(c.x, c.y, c.wall);
            guiCell.visited = c.visited;
            state.cells.push_back(guiCell);
        }

        Cell* start = globalMaze->getStart();
        Cell* end = globalMaze->getGoal();
        
        if (start) state.startCell = &state.cells[start->y * state.width + start->x];
        if (end) state.endCell = &state.cells[end->y * state.width + end->x];

        return state;
    }

    static void startSolve(PathfindingAlgorithm algorithm, int speed) {
        if (!globalMaze) return;
        globalMaze->reset(); 

        std::vector<Cell*> visited;
        std::vector<Cell*> frontier;

        auto stepCallback = [&](Cell* v, Cell* f) {
            if (v) visited.push_back(v);
            if (f) frontier.push_back(f);

            if (speed < 10) { 
                // Simple delay for animation
                int delay = (11 - speed) * 5; 
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

    static void reset() {
        if (globalMaze) globalMaze->reset();
        if (onMazeGenerated) onMazeGenerated(); 
    }

    static void step() {} 
};