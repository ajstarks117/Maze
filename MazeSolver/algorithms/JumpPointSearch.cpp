#include "JumpPointSearch.h"
#include "../core/Utility.h"
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>

struct JPSCompare {
    bool operator()(Cell* a, Cell* b) const {
        return a->f_cost() > b->f_cost();
    }
};

AlgorithmResult JumpPointSearch::solve(Maze& maze, std::function<void(Cell*, Cell*)> stepCallback){
    AlgorithmResult result;
    RobustTimer timer;
    
    maze.reset();
    Cell* start = maze.getStart();
    Cell* goal = maze.getGoal();
    
    if (!start || !goal) {
        return result;
    }
    
    std::priority_queue<Cell*, std::vector<Cell*>, JPSCompare> openSet;
    std::vector<bool> closedSet(maze.getWidth() * maze.getHeight(), false);
    
    start->g_cost = 0.0;
    start->h_cost = Utility::manhattanDistance(start, goal);
    openSet.push(start);
    
    timer.start(2000); 
    
    while (!openSet.empty() && !timer.isTimeout()) {
        Cell* current = openSet.top();
        openSet.pop();
        
        int currentIndex = current->y * maze.getWidth() + current->x;
        if (closedSet[currentIndex]) continue;
        
        closedSet[currentIndex] = true;
        result.visitedOrder.push_back(current);

        // [ANIMATION FIX]
        if (stepCallback) {
            stepCallback(current, nullptr);
        }
        
        if (current == goal) {
            result.success = true;
            break;
        }
        
        for (Cell* neighbor : maze.getNeighbors(current)) {
            int neighborIndex = neighbor->y * maze.getWidth() + neighbor->x;
            if (closedSet[neighborIndex]) continue;
            
            double new_g_cost = current->g_cost + 1.0;
            
            if (new_g_cost < neighbor->g_cost) {
                neighbor->g_cost = new_g_cost;
                neighbor->h_cost = Utility::manhattanDistance(neighbor, goal);
                neighbor->parent = current;
                openSet.push(neighbor);

                // [ANIMATION FIX]
                if (stepCallback) {
                    stepCallback(nullptr, neighbor);
                }
            }
        }
    }
    
    result.metrics.timeTakenMs = timer.stop();
    
    if (timer.isTimeout()) {
        result.success = false;
    }
    
    if (result.success) {
        result.path = Utility::reconstructPath(maze.getGoal());
        result.metrics.pathLength = result.path.size();
    }
    
    result.metrics.nodesExplored = result.visitedOrder.size();
    
    return result;
}