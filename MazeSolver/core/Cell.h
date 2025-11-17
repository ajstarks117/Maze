#pragma once

struct Cell {
    int x, y;
    bool wall;
    bool visited;
    
    // Pathfinding fields
    double g_cost;
    double h_cost;
    Cell* parent;
    
    Cell(int x = 0, int y = 0, bool isWall = false) 
        : x(x), y(y), wall(isWall), visited(false), 
          g_cost(0.0), h_cost(0.0), parent(nullptr) {}
    
    double f_cost() const { return g_cost + h_cost; }
};