#pragma once

#include "Cell.h"
#include <vector>
#include <utility>
#include <string>
using namespace std;

class Maze {
public:
    int width;
    int height;
    vector<Cell> cells; // row-major: y * width + x

    // start and end coordinates (in cell coordinates)
    pair<int,int> start;
    pair<int,int> end;

    // ctor ensures odd dimensions (ideal for perfect maze with walls between passages)
    Maze(int w = 31, int h = 21);

    // accessors
    Cell& at(int x, int y);
    const Cell& atConst(int x, int y) const;
    bool inBounds(int x, int y) const;

    // generator
    void generateRecursiveBacktracker();

    // reset visitation / pathfinding fields
    void resetForSolve();

    // neighbor helpers
    vector<pair<int,int>> neighbors4(int x, int y) const;
    vector<pair<int,int>> navigableNeighbors(int x, int y) const;

    // ASCII dump for quick testing
    string toAscii() const;

private:
    inline int indexOf(int x, int y) const { return y * width + x; }
};
