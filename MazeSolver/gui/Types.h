#pragma once
#include <vector>
#include "../core/Cell.h" // Import core cell definition

// GUI-specific MazeState
struct MazeState {
    int width, height;
    std::vector<Cell> cells;
    Cell* startCell;
    Cell* endCell;
    
    MazeState(int w = 21, int h = 21) : width(w), height(h), startCell(nullptr), endCell(nullptr) {
        cells.reserve(width * height);
    }
};