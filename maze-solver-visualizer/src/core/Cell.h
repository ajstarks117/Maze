#pragma once
#include <climits>

struct Cell {
    int x = 0;
    int y = 0;
    bool wall = true;
    bool visited = false;   // used by the generator
    int g_cost = INT_MAX;   // for pathfinding
    int h_cost = 0;         // heuristic
    Cell* parent = nullptr; // parent pointer for path reconstruction

    Cell() = default;
    Cell(int _x, int _y, bool _wall = true)
        : x(_x), y(_y), wall(_wall), visited(false),
          g_cost(INT_MAX), h_cost(0), parent(nullptr) {}
};
