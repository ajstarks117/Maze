#include "Maze.h"
#include <iostream>
#include <stack>
#include <random>
#include <algorithm>
#include <chrono> 
#include <limits>

Maze::Maze(int width, int height) : width(width), height(height) {
    if (width % 2 == 0) this->width++;
    if (height % 2 == 0) this->height++;
    
    cells.resize(this->width * this->height);
    initializeMaze();
    generate();
}

void Maze::initializeMaze() {
    for (int i = 0; i < width * height; ++i) {
        // x and y can be calculated from index
        int y = i / width;
        int x = i % width;
        cells[i] = Cell(x, y, true); // Set as WALL (true)
    }
}

void Maze::generate() {
    generateRecursiveBacktracking();
    // Force Start/End open
    if (getStart()) { getStart()->wall = false; getStart()->visited = false; }
    if (getGoal()) { getGoal()->wall = false; getGoal()->visited = false; }
}

void Maze::generateRecursiveBacktracking() {
    reset();
    std::stack<Cell*> stack;
    
    // Crash-proof seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    
    Cell* start = getCellPtr(1, 1);
    if (!start) return;
    
    start->wall = false;
    start->visited = true;
    stack.push(start);
    
    const int dx[] = {2, 0, -2, 0};
    const int dy[] = {0, 2, 0, -2};
    
    while (!stack.empty()) {
        Cell* current = stack.top();
        std::vector<int> dirs = {0, 1, 2, 3};
        std::shuffle(dirs.begin(), dirs.end(), gen);
        
        bool carved = false;
        for (int dir : dirs) {
            int nx = current->x + dx[dir];
            int ny = current->y + dy[dir];
            
            Cell* neighbor = getCellPtr(nx, ny);
            if (neighbor && neighbor->wall) { // If it's a wall, it's valid to carve
                // Carve the wall in between
                Cell* wall = getCellPtr(current->x + dx[dir]/2, current->y + dy[dir]/2);
                if (wall) wall->wall = false;
                
                neighbor->wall = false;
                neighbor->visited = true;
                stack.push(neighbor);
                carved = true;
                break;
            }
        }
        if (!carved) stack.pop();
    }
    reset();
}

void Maze::reset() {
    for (auto& c : cells) {
        c.visited = false;
        c.g_cost = std::numeric_limits<double>::max();
        c.h_cost = 0.0;
        c.parent = nullptr;
    }
}

Cell* Maze::getCellPtr(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
    return &cells[y * width + x];
}

Cell* Maze::getStart() { return getCellPtr(1, 1); }
Cell* Maze::getGoal() { return getCellPtr(width - 2, height - 2); }

std::vector<Cell*> Maze::getNeighbors(Cell* c) {
    std::vector<Cell*> n;
    if (!c) return n;
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {1, -1, 0, 0};
    for (int i=0; i<4; ++i) {
        Cell* adj = getCellPtr(c->x + dx[i], c->y + dy[i]);
        if (adj && !adj->wall) n.push_back(adj);
    }
    return n;
}

std::string Maze::toAscii() const { return ""; }
void Maze::markPath(const std::vector<Cell*>&) {}