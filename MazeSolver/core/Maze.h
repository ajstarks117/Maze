#pragma once
#include <vector>
#include "Cell.h"
#include <string>

class Maze {
private:
    int width, height;
    std::vector<Cell> cells;
    
    void initializeMaze();
    void generateRecursiveBacktracking();
    
    public:
    Maze(int width = 51, int height = 51);
    Cell* getCellPtr(int x, int y);
    
    void generate();
    void reset();
    std::string toAscii() const;
    void markPath(const std::vector<Cell*>& path);
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Cell* getStart();
    Cell* getGoal();
    std::vector<Cell>& getCells() { return cells; }
    
    // For algorithms
    std::vector<Cell*> getNeighbors(Cell* cell);
};