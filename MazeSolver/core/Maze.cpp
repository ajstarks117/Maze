#include "Maze.h"
#include <iostream>
#include <stack>
#include <cstdlib> // rand
#include <ctime>   // time
#include <algorithm>
#include <limits>

Maze::Maze(int width, int height) : width(width), height(height) {
    // Ensure odd dimensions
    if (width % 2 == 0) this->width++;
    if (height % 2 == 0) this->height++;
    
    cells.resize(this->width * this->height);
    initializeMaze();
    generate();
}

void Maze::initializeMaze() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            // All cells start as walls (Black)
            cells[index] = Cell(x, y, true); 
        }
    }
}

void Maze::generate() {
    generateRecursiveBacktracking();
    
    // SAFETY: Force Start and End to be open (White)
    if (getStart()) { getStart()->wall = false; getStart()->visited = false; }
    if (getGoal()) { getGoal()->wall = false; getGoal()->visited = false; }
}

void Maze::generateRecursiveBacktracking() {
    reset();
    std::stack<Cell*> stack;
    
    // [FIX] Simple, crash-proof seeding
    std::srand(static_cast<unsigned>(std::time(0)));
    
    // Start at (1,1)
    Cell* start = getCellPtr(1, 1);
    if (!start) return;
    
    start->wall = false;
    start->visited = true;
    stack.push(start);
    
    const int dx[] = {2, 0, -2, 0};
    const int dy[] = {0, 2, 0, -2};
    
    while (!stack.empty()) {
        Cell* current = stack.top();
        
        // [FIX] Simple manual shuffle logic to replace std::shuffle
        int directions[] = {0, 1, 2, 3};
        for (int i = 0; i < 4; ++i) {
            int r = std::rand() % 4;
            std::swap(directions[i], directions[r]);
        }
        
        bool foundUnvisited = false;
        
        for (int i = 0; i < 4; ++i) {
            int dir = directions[i];
            int newX = current->x + dx[dir];
            int newY = current->y + dy[dir];
            
            if (newX > 0 && newX < width - 1 && newY > 0 && newY < height - 1) {
                Cell* neighbor = getCellPtr(newX, newY);
                // If it's a wall, we can carve into it
                if (neighbor && neighbor->wall) { 
                    int wallX = current->x + dx[dir] / 2;
                    int wallY = current->y + dy[dir] / 2;
                    Cell* wall = getCellPtr(wallX, wallY);
                    
                    if (wall) wall->wall = false; // Break the wall
                    
                    neighbor->wall = false;
                    neighbor->visited = true;
                    stack.push(neighbor);
                    foundUnvisited = true;
                    break;
                }
            }
        }
        
        if (!foundUnvisited) stack.pop();
    }
    reset(); // Clean up for the solver
}

void Maze::reset() {
    for (auto& cell : cells) {
        cell.visited = false;
        cell.g_cost = std::numeric_limits<double>::max();
        cell.h_cost = 0.0;
        cell.parent = nullptr;
    }
}

Cell* Maze::getCellPtr(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
    return &cells[y * width + x];
}

Cell* Maze::getStart() { return getCellPtr(1, 1); }
Cell* Maze::getGoal() { return getCellPtr(width - 2, height - 2); }

std::vector<Cell*> Maze::getNeighbors(Cell* cell) {
    std::vector<Cell*> neighbors;
    if (!cell) return neighbors;
    const int dx[] = {1, 0, -1, 0};
    const int dy[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; ++i) {
        int newX = cell->x + dx[i];
        int newY = cell->y + dy[i];
        Cell* neighbor = getCellPtr(newX, newY);
        if (neighbor && !neighbor->wall) neighbors.push_back(neighbor);
    }
    return neighbors;
}

std::string Maze::toAscii() const { return ""; }
void Maze::markPath(const std::vector<Cell*>& path) {}