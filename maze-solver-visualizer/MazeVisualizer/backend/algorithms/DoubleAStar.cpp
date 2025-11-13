#include "DoubleAStar.h"
#include "../core/Utility.h"
#include <queue>
#include <unordered_map>
#include <chrono>

AlgorithmResult DoubleAStarSolver::solve(Maze& maze) {
    AlgorithmResult result;
    maze.resetForSolve();

    auto startTime = std::chrono::high_resolution_clock::now();

    Cell* start = &maze.at(maze.start.first, maze.start.second);
    Cell* goal = &maze.at(maze.end.first, maze.end.second);

    auto cmp = [](Cell* a, Cell* b) {
        return (a->g_cost + a->h_cost) > (b->g_cost + b->h_cost);
    };

    std::priority_queue<Cell*, std::vector<Cell*>, decltype(cmp)> openF(cmp), openB(cmp);
    std::unordered_map<int, Cell*> visitedF, visitedB;

    start->g_cost = 0;
    start->h_cost = Heuristic::manhattan(start, goal);
    openF.push(start);
    visitedF[start->y * maze.width + start->x] = start;

    goal->g_cost = 0;
    goal->h_cost = Heuristic::manhattan(goal, start);
    openB.push(goal);
    visitedB[goal->y * maze.width + goal->x] = goal;

    Cell* meeting = nullptr;

    while (!openF.empty() && !openB.empty()) {
        auto expand = [&](std::priority_queue<Cell*, std::vector<Cell*>, decltype(cmp)>& open,
                          std::unordered_map<int, Cell*>& visited,
                          std::unordered_map<int, Cell*>& otherVisited,
                          bool forward) -> Cell* {
            if (open.empty()) return nullptr;
            Cell* current = open.top();
            open.pop();

            if (current->visited)
                return nullptr;

            current->visited = true;
            result.visitedOrder.push_back(current);

            int key = current->y * maze.width + current->x;
            if (otherVisited.count(key))
                return current;

            for (auto [nx, ny] : maze.navigableNeighbors(current->x, current->y)) {
                Cell& neighbor = maze.at(nx, ny);
                if (neighbor.visited || neighbor.wall)
                    continue;

                int tentativeG = current->g_cost + 1;
                if (tentativeG < neighbor.g_cost) {
                    neighbor.g_cost = tentativeG;
                    neighbor.h_cost = Heuristic::manhattan(&neighbor, forward ? goal : start);
                    neighbor.parent = current;
                    open.push(&neighbor);
                    visited[ny * maze.width + nx] = &neighbor;
                }
            }

            return nullptr;
        };

        Cell* meet = expand(openF, visitedF, visitedB, true);
        if (meet) { meeting = meet; break; }

        meet = expand(openB, visitedB, visitedF, false);
        if (meet) { meeting = meet; break; }
    }

    if (meeting) {
        // Merge forward and backward paths
        std::vector<Cell*> path1 = PathHelper::reconstructPath(meeting);
        std::vector<Cell*> path2;

        Cell* node = visitedB[meeting->y * maze.width + meeting->x]->parent;
        while (node) {
            path2.push_back(node);
            node = node->parent;
        }

        std::reverse(path2.begin(), path2.end());
        path1.insert(path1.end(), path2.begin(), path2.end());
        result.path = path1;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.metrics.timeTakenMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    result.metrics.pathLength = static_cast<int>(result.path.size());
    result.metrics.nodesExplored = static_cast<int>(result.visitedOrder.size());

    return result;
}
