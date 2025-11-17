#include "Maze.h"
#include <iostream>
#include <stack>
#include <random>
#include <algorithm>


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
            cells[index] = Cell(x, y, true);
        }
    }
}

void Maze::generate() {
    generateRecursiveBacktracking();
}

void Maze::generateRecursiveBacktracking() {
    reset();
    
    std::stack<Cell*> stack;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int startX = 1;
    int startY = 1;
    
    Cell* start = getCellPtr(startX, startY);
    if (!start) return;
    
    start->wall = false;
    start->visited = true;
    stack.push(start);
    
    const int dx[] = {2, 0, -2, 0};
    const int dy[] = {0, 2, 0, -2};
    
    while (!stack.empty()) {
        Cell* current = stack.top();
        
        // C++17 compatible shuffle
        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), gen);  // Fixed!
        
        bool foundUnvisited = false;
        
        for (int i = 0; i < 4; ++i) {
            int dir = directions[i];
            int newX = current->x + dx[dir];
            int newY = current->y + dy[dir];
            
            if (newX > 0 && newX < width - 1 && newY > 0 && newY < height - 1) {
                Cell* neighbor = getCellPtr(newX, newY);
                if (neighbor && neighbor->wall && !neighbor->visited) {
                    int wallX = current->x + dx[dir] / 2;
                    int wallY = current->y + dy[dir] / 2;
                    Cell* wall = getCellPtr(wallX, wallY);
                    if (wall) wall->wall = false;
                    
                    neighbor->wall = false;
                    neighbor->visited = true;
                    stack.push(neighbor);
                    foundUnvisited = true;
                    break;
                }
            }
        }
        
        if (!foundUnvisited) {
            stack.pop();
        }
    }
    
    reset();
}
void Maze::reset() {
    for (auto& cell : cells) {
        cell.visited = false;
        cell.g_cost = std::numeric_limits<double>::max();  // Reset to large value
        cell.h_cost = 0.0;
        cell.parent = nullptr;
    }
}

Cell* Maze::getCellPtr(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) 
        return nullptr;
    return &cells[y * width + x];
}

Cell* Maze::getStart() {
    return getCellPtr(1, 1);
}

Cell* Maze::getGoal() {
    return getCellPtr(width - 2, height - 2);
}

std::vector<Cell*> Maze::getNeighbors(Cell* cell) {
    std::vector<Cell*> neighbors;
    if (!cell) return neighbors;
    
    const int dx[] = {1, 0, -1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    for (int i = 0; i < 4; ++i) {
        int newX = cell->x + dx[i];
        int newY = cell->y + dy[i];
        
        Cell* neighbor = getCellPtr(newX, newY);
        if (neighbor && !neighbor->wall) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

std::string Maze::toAscii() const {
    std::string result;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Cell* cell = &cells[y * width + x];
            
            if (x == 1 && y == 1) {
                result += 'S';
            } else if (x == width - 2 && y == height - 2) {
                result += 'E';
            } else if (cell->wall) {
                result += '#';
            } else if (cell->visited) {
                result += '.';
            } else {
                result += ' ';
            }
        }
        result += '\n';
    }
    
    return result;
}

void Maze::markPath(const std::vector<Cell*>& path) {
    for (auto& cell : cells) {
        cell.visited = false;
    }
    
    for (Cell* cell : path) {
        if (cell && cell != getStart() && cell != getGoal()) {
            cell->visited = true;
        }
    }
}