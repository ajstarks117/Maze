#include "JumpPointSearch.h"
#include "../core/Utility.h"
#include <queue>
#include <vector>
#include <functional>
#include <cmath>

// Helper to check if a node is walkable
bool isWalkable(Maze& maze, int x, int y) {
    if (x < 0 || x >= maze.getWidth() || y < 0 || y >= maze.getHeight()) return false;
    Cell* c = maze.getCellPtr(x, y);
    return c && !c->wall;
}

// The core JUMP function
Cell* jump(Maze& maze, Cell* current, int dx, int dy, Cell* goal) {
    int x = current->x;
    int y = current->y;
    
    // 1. Step forward
    x += dx;
    y += dy;
    
    // 2. Check bounds/wall
    if (!isWalkable(maze, x, y)) return nullptr;
    
    Cell* next = maze.getCellPtr(x, y);
    
    // 3. Goal found?
    if (next == goal) return next;
    
    // 4. Junction/Forced Neighbor Check (Orthogonal only for this maze type)
    // If moving Horizontally, check for open nodes above/below that imply a turn
    if (dx != 0) {
        if ((isWalkable(maze, x, y - 1) && !isWalkable(maze, x - dx, y - 1)) ||
            (isWalkable(maze, x, y + 1) && !isWalkable(maze, x - dx, y + 1))) {
            return next;
        }
        // Also simpler check: Is this a generic intersection? 
        // (Up or Down is open)
        if (isWalkable(maze, x, y - 1) || isWalkable(maze, x, y + 1)) return next;
    }
    // If moving Vertically
    else if (dy != 0) {
        if ((isWalkable(maze, x - 1, y) && !isWalkable(maze, x - 1, y - dy)) ||
            (isWalkable(maze, x + 1, y) && !isWalkable(maze, x + 1, y - dy))) {
            return next;
        }
        // Simpler check: Is this a generic intersection?
        // (Left or Right is open)
        if (isWalkable(maze, x - 1, y) || isWalkable(maze, x + 1, y)) return next;
    }
    
    // 5. Recursive Jump
    return jump(maze, next, dx, dy, goal);
}

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
    
    if (!start || !goal) return result;
    
    std::priority_queue<Cell*, std::vector<Cell*>, JPSCompare> openSet;
    std::vector<bool> closedSet(maze.getWidth() * maze.getHeight(), false);
    
    start->g_cost = 0.0;
    start->h_cost = Utility::manhattanDistance(start, goal);
    openSet.push(start);
    
    // Timer setup (GUI vs Terminal)
    long long timeout = stepCallback ? 300000 : 2000;
    timer.start(timeout); 
    
    while (!openSet.empty() && !timer.isTimeout()) {
        Cell* current = openSet.top();
        openSet.pop();
        
        // Mark visited
        int currentIndex = current->y * maze.getWidth() + current->x;
        if (closedSet[currentIndex]) continue;
        closedSet[currentIndex] = true;
        result.visitedOrder.push_back(current);
        
        // Visualize "Jumping from here"
        if (stepCallback) stepCallback(current, nullptr);
        
        if (current == goal) {
            result.success = true;
            break;
        }
        
        // Try jumping in all 4 cardinal directions
        const int dx[] = {1, -1, 0, 0};
        const int dy[] = {0, 0, 1, -1};
        
        for (int i = 0; i < 4; ++i) {
            // Attempt to jump
            Cell* jumpNode = jump(maze, current, dx[i], dy[i], goal);
            
            if (jumpNode) {
                // We found a jump point!
                int jumpIndex = jumpNode->y * maze.getWidth() + jumpNode->x;
                
                if (!closedSet[jumpIndex]) {
                    // Calculate cost (distance from parent + distance to jump)
                    double dist = std::abs(jumpNode->x - current->x) + std::abs(jumpNode->y - current->y);
                    double new_g = current->g_cost + dist;
                    
                    if (new_g < jumpNode->g_cost || jumpNode->g_cost == 0) { // 0 check for fresh nodes
                        jumpNode->g_cost = new_g;
                        jumpNode->h_cost = Utility::manhattanDistance(jumpNode, goal);
                        jumpNode->parent = current;
                        
                        openSet.push(jumpNode);
                        
                        // Visualize "Found a Jump Point"
                        if (stepCallback) stepCallback(nullptr, jumpNode);
                    }
                }
            }
        }
    }
    
    result.metrics.timeTakenMs = timer.stop();
    if (timer.isTimeout()) result.success = false;
    
    if (result.success) {
        // [IMPORTANT] Path reconstruction for JPS requires filling gaps
        // Since parents are far away, we need to interpolate for the visual path
        std::vector<Cell*> rawPath;
        Cell* curr = goal;
        while (curr != nullptr) {
            rawPath.push_back(curr);
            
            Cell* par = curr->parent;
            if (par) {
                // Interpolate between current and parent
                int dX = (par->x > curr->x) ? 1 : (par->x < curr->x) ? -1 : 0;
                int dY = (par->y > curr->y) ? 1 : (par->y < curr->y) ? -1 : 0;
                
                int tempX = curr->x + dX;
                int tempY = curr->y + dY;
                
                while (tempX != par->x || tempY != par->y) {
                    rawPath.push_back(maze.getCellPtr(tempX, tempY));
                    tempX += dX;
                    tempY += dY;
                }
            }
            curr = par;
        }
        std::reverse(rawPath.begin(), rawPath.end());
        result.path = rawPath;
        result.metrics.pathLength = result.path.size();
    }
    
    result.metrics.nodesExplored = result.visitedOrder.size();
    return result;
}