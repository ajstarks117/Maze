#include "Dijkstra.h"
#include <limits>
#include <unordered_set>
#include <iostream>

AlgorithmResult DijkstraSolver::solve(Maze& maze) {
    AlgorithmResult result;
    Timer timer; // start measuring time
    maze.resetForSolve();

    auto& startCell = maze.at(maze.start.first, maze.start.second);
    auto& endCell   = maze.at(maze.end.first, maze.end.second);

    // Min-heap based on g_cost (distance)
    auto cmp = [](Cell* a, Cell* b) { return a->g_cost > b->g_cost; };
    std::priority_queue<Cell*, std::vector<Cell*>, decltype(cmp)> pq(cmp);

    startCell.g_cost = 0;
    pq.push(&startCell);

    while (!pq.empty()) {
        Cell* current = pq.top();
        pq.pop();

        if (current->visited)
            continue;

        current->visited = true;
        result.visitedOrder.push_back(current);

        if (current == &endCell)
            break; // found shortest path

        for (auto [nx, ny] : maze.navigableNeighbors(current->x, current->y)) {
            Cell& neighbor = maze.at(nx, ny);
            if (neighbor.visited || neighbor.wall)
                continue;

            int tentative = current->g_cost + 1; // uniform cost grid
            if (tentative < neighbor.g_cost) {
                neighbor.g_cost = tentative;
                neighbor.parent = current;
                pq.push(&neighbor);
            }
        }
    }

    // Reconstruct shortest path
    if (endCell.parent != nullptr || (&endCell == &startCell))
        result.path = PathHelper::reconstructPath(&endCell);

    // Fill metrics
    result.metrics.pathLength = (int)result.path.size();
    result.metrics.nodesExplored = (int)result.visitedOrder.size();
    result.metrics.timeTakenMs = timer.elapsedMs();

    return result;
}
