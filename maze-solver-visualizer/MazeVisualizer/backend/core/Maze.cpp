#include "Maze.h"
#include <stack>
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>
using namespace std;

Maze::Maze(int w, int h) {
    // force odd dimensions for classic grid mazes
    width = (w % 2 == 0) ? w + 1 : w;
    height = (h % 2 == 0) ? h + 1 : h;

    cells.reserve(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // set interior odd positions as passages (initially not walls)
            bool wall = true;
            if ((x % 2 == 1) && (y % 2 == 1)) wall = false;
            cells.emplace_back(x, y, wall);
        }
    }

    // default corners-ish start/end, on odd positions
    start = {1, 1};
    end = {width - 2, height - 2};
}

Cell& Maze::at(int x, int y) {
    return cells[indexOf(x, y)];
}

const Cell& Maze::atConst(int x, int y) const {
    return cells[indexOf(x, y)];
}

bool Maze::inBounds(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < height);
}

vector<pair<int,int>> Maze::neighbors4(int x, int y) const {
    vector<pair<int,int>> out;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i], ny = y + dy[i];
        if (inBounds(nx, ny)) out.emplace_back(nx, ny);
    }
    return out;
}

vector<pair<int,int>> Maze::navigableNeighbors(int x, int y) const {
    vector<pair<int,int>> out;
    auto n = neighbors4(x, y);
    for (auto &p : n) {
        if (!atConst(p.first, p.second).wall) out.push_back(p);
    }
    return out;
}

void Maze::resetForSolve() {
    for (auto &c : cells) {
        c.visited = false;
        c.g_cost = INT_MAX;
        c.h_cost = 0;
        c.parent = nullptr;
    }
}

// Recursive Backtracker (iterative using stack)
// We consider passage cells at odd coordinates; walls are at even coords and between passages.
void Maze::generateRecursiveBacktracker() {
    // initialize: mark all walls true except odd grid positions as passage cells
    for (auto &c : cells) {
        if ((c.x % 2 == 1) && (c.y % 2 == 1)) {
            c.wall = false;
            c.visited = false;
        } else {
            c.wall = true;
            c.visited = false;
        }
        c.parent = nullptr;
    }

    // RNG seeded by time
    mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

    stack<pair<int,int>> st;
    int sx = start.first;
    int sy = start.second;
    if (!inBounds(sx, sy)) {
        sx = 1; sy = 1;
        start = {sx, sy};
    }
    st.push({sx, sy});
    at(sx, sy).visited = true;

    while (!st.empty()) {
        auto [cx, cy] = st.top();

        // neighbors two steps away (to maintain walls between cells)
        vector<pair<int,int>> choices;
        const int dx[4] = {0, 0, 2, -2};
        const int dy[4] = {2, -2, 0, 0};

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (inBounds(nx, ny) && !at(nx, ny).visited) {
                choices.emplace_back(nx, ny);
            }
        }

        if (!choices.empty()) {
            uniform_int_distribution<int> dist(0, (int)choices.size() - 1);
            auto [nx, ny] = choices[dist(rng)];

            // knock down wall between (cx,cy) and (nx,ny)
            int wx = (cx + nx) / 2;
            int wy = (cy + ny) / 2;
            at(wx, wy).wall = false;
            at(nx, ny).wall = false;
            at(nx, ny).visited = true;
            st.push({nx, ny});
        } else {
            st.pop();
        }
    }

    // make sure start and end are open
    if (inBounds(start.first, start.second)) at(start.first, start.second).wall = false;
    if (inBounds(end.first, end.second)) at(end.first, end.second).wall = false;
}

// ASCII representation: '#' for wall, ' ' for passage,
// 'S' for start, 'E' for end.
string Maze::toAscii() const {
    ostringstream oss;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (start.first == x && start.second == y) {
                oss << 'S';
            } else if (end.first == x && end.second == y) {
                oss << 'E';
            } else if (atConst(x, y).wall) {
                oss << '#';
            } else {
                oss << ' ';
            }
        }
        oss << '\n';
    }
    return oss.str();
}
