#pragma once
#include <vector>

// Complete Cell structure definition
struct Cell {
    int x, y;
    bool wall;
    bool visited;
    double g_cost;
    double h_cost;
    Cell* parent;
    
    Cell(int x = 0, int y = 0, bool wall = false) 
        : x(x), y(y), wall(wall), visited(false), 
          g_cost(0.0), h_cost(0.0), parent(nullptr) {}
    
    double f_cost() const { return g_cost + h_cost; }
};

// Complete MazeState structure definition
struct MazeState {
    int width, height;
    std::vector<Cell> cells;
    Cell* startCell;
    Cell* endCell;
    
    MazeState(int w = 21, int h = 21) : width(w), height(h), startCell(nullptr), endCell(nullptr) {
        cells.reserve(width * height);
    }
};