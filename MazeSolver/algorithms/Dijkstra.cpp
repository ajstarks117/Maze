#include "Dijkstra.h"
#include "../core/Utility.h"
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>
#include <limits>

struct DijkstraCompare {
    bool operator()(Cell* a, Cell* b) const {
        return a->g_cost > b->g_cost;
    }
};

AlgorithmResult Dijkstra::solve(Maze& maze, std::function<void(Cell*, Cell*)> stepCallback){
    AlgorithmResult result;
    RobustTimer timer;
    
    maze.reset();
    Cell* start = maze.getStart();
    Cell* goal = maze.getGoal();
    
    if (!start || !goal) return result;
    
    std::priority_queue<Cell*, std::vector<Cell*>, DijkstraCompare> openSet;
    std::vector<bool> closedSet(maze.getWidth() * maze.getHeight(), false);
    
    start->g_cost = 0.0;
    openSet.push(start);
    
    // [FIX] If animating (stepCallback exists), increase timeout to 5 minutes (300000ms)
    // Otherwise keep 2 seconds for terminal benchmarks.
    long long timeout = stepCallback ? 300000 : 2000;
    timer.start(timeout); 
    
    while (!openSet.empty() && !timer.isTimeout()) {
        Cell* current = openSet.top();
        openSet.pop();
        
        int currentIndex = current->y * maze.getWidth() + current->x;
        if (closedSet[currentIndex]) continue;
        
        closedSet[currentIndex] = true;
        result.visitedOrder.push_back(current);
        
        if (stepCallback) stepCallback(current, nullptr);
        
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
                neighbor->parent = current;
                openSet.push(neighbor);

                if (stepCallback) stepCallback(nullptr, neighbor);
            }
        }
    }
    
    result.metrics.timeTakenMs = timer.stop();
    if (timer.isTimeout()) result.success = false;
    
    if (result.success) {
        result.path = Utility::reconstructPath(maze.getGoal());
        result.metrics.pathLength = result.path.size();
    }
    result.metrics.nodesExplored = result.visitedOrder.size();
    return result;
}