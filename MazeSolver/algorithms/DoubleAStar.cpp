#include "DoubleAStar.h"
#include "../core/Utility.h"
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>

struct DoubleAStarCompare {
    bool operator()(Cell* a, Cell* b) const {
        return a->f_cost() > b->f_cost();
    }
};

AlgorithmResult DoubleAStar::solve(Maze& maze, std::function<void(Cell*, Cell*)> stepCallback){
    AlgorithmResult result;
    RobustTimer timer;
    
    maze.reset();
    Cell* start = maze.getStart();
    Cell* goal = maze.getGoal();
    
    if (!start || !goal) {
        return result;
    }
    
    std::priority_queue<Cell*, std::vector<Cell*>, DoubleAStarCompare> openForward;
    std::priority_queue<Cell*, std::vector<Cell*>, DoubleAStarCompare> openBackward;
    
    std::vector<bool> closedForward(maze.getWidth() * maze.getHeight(), false);
    std::vector<bool> closedBackward(maze.getWidth() * maze.getHeight(), false);
    
    std::vector<Cell*> cameFromForward(maze.getWidth() * maze.getHeight(), nullptr);
    std::vector<Cell*> cameFromBackward(maze.getWidth() * maze.getHeight(), nullptr);
    
    start->g_cost = 0.0;
    start->h_cost = Utility::manhattanDistance(start, goal);
    openForward.push(start);
    cameFromForward[start->y * maze.getWidth() + start->x] = start;
    
    goal->g_cost = 0.0;
    goal->h_cost = Utility::manhattanDistance(goal, start);
    openBackward.push(goal);
    cameFromBackward[goal->y * maze.getWidth() + goal->x] = goal;
    
    Cell* meetingPoint = nullptr;
    
    timer.start(2000); 
    
    while ((!openForward.empty() || !openBackward.empty()) && !timer.isTimeout()) {
        // Expand forward
        if (!openForward.empty()) {
            Cell* current = openForward.top();
            openForward.pop();
            
            int idx = current->y * maze.getWidth() + current->x;
            if (closedForward[idx]) continue;
            
            closedForward[idx] = true;
            result.visitedOrder.push_back(current);

            // [ANIMATION FIX] Forward visit
            if (stepCallback) stepCallback(current, nullptr);
            
            if (closedBackward[idx]) {
                meetingPoint = current;
                result.success = true;
                break;
            }
            
            for (Cell* neighbor : maze.getNeighbors(current)) {
                int nidx = neighbor->y * maze.getWidth() + neighbor->x;
                if (closedForward[nidx]) continue;
                
                double new_g_cost = current->g_cost + 1.0;
                if (cameFromForward[nidx] == nullptr || new_g_cost < neighbor->g_cost) {
                    neighbor->g_cost = new_g_cost;
                    neighbor->h_cost = Utility::manhattanDistance(neighbor, goal);
                    neighbor->parent = current;
                    cameFromForward[nidx] = current;
                    openForward.push(neighbor);

                    // [ANIMATION FIX] Forward frontier
                    if (stepCallback) stepCallback(nullptr, neighbor);
                }
            }
        }
        
        // Expand backward
        if (!openBackward.empty() && !result.success) {
            Cell* current = openBackward.top();
            openBackward.pop();
            
            int idx = current->y * maze.getWidth() + current->x;
            if (closedBackward[idx]) continue;
            
            closedBackward[idx] = true;
            result.visitedOrder.push_back(current);

            // [ANIMATION FIX] Backward visit
            if (stepCallback) stepCallback(current, nullptr);
            
            if (closedForward[idx]) {
                meetingPoint = current;
                result.success = true;
                break;
            }
            
            for (Cell* neighbor : maze.getNeighbors(current)) {
                int nidx = neighbor->y * maze.getWidth() + neighbor->x;
                if (closedBackward[nidx]) continue;
                
                double new_g_cost = current->g_cost + 1.0;
                if (cameFromBackward[nidx] == nullptr || new_g_cost < neighbor->g_cost) {
                    neighbor->g_cost = new_g_cost;
                    neighbor->h_cost = Utility::manhattanDistance(neighbor, start);
                    neighbor->parent = current;
                    cameFromBackward[nidx] = current;
                    openBackward.push(neighbor);

                    // [ANIMATION FIX] Backward frontier
                    if (stepCallback) stepCallback(nullptr, neighbor);
                }
            }
        }
    }
    
    result.metrics.timeTakenMs = timer.stop();
    
    if (timer.isTimeout()) {
        result.success = false;
    }
    
    if (result.success && meetingPoint) {
        std::vector<Cell*> path;
        
        Cell* current = meetingPoint;
        while (current != start) {
            path.push_back(current);
            current = cameFromForward[current->y * maze.getWidth() + current->x];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        
        current = cameFromBackward[meetingPoint->y * maze.getWidth() + meetingPoint->x];
        while (current != goal) {
            path.push_back(current);
            current = cameFromBackward[current->y * maze.getWidth() + current->x];
        }
        if (!path.empty() && path.back() != goal) {
            path.push_back(goal);
        }
        
        result.path = path;
        result.metrics.pathLength = result.path.size();
    }
    
    result.metrics.nodesExplored = result.visitedOrder.size();
    
    return result;
}