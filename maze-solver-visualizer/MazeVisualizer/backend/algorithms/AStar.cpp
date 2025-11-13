#include "AStar.h"
#include <limits>
#include <iostream>
#include <unordered_set>

AlgorithmResult AStarSolver::solve(Maze& maze) {
    AlgorithmResult result;
    Timer timer;  // measure execution time
    maze.resetForSolve();

    auto& startCell = maze.at(maze.start.first, maze.start.second);
    auto& endCell   = maze.at(maze.end.first, maze.end.second);

    // Priority queue (min-heap) comparing total cost f = g + h
    auto cmp = [](Cell* a, Cell* b) {
        return (a->g_cost + a->h_cost) > (b->g_cost + b->h_cost);
    };
    std::priority_queue<Cell*, std::vector<Cell*>, decltype(cmp)> pq(cmp);

    startCell.g_cost = 0;
    startCell.h_cost = Heuristic::manhattan(
        {startCell.x, startCell.y}, {endCell.x, endCell.y});
    pq.push(&startCell);

    while (!pq.empty()) {
        Cell* current = pq.top();
        pq.pop();

        if (current->visited)
            continue;

        current->visited = true;
        result.visitedOrder.push_back(current);

        // ✅ Goal reached
        if (current == &endCell)
            break;

        // Explore navigable neighbors
        for (const auto& coord : maze.navigableNeighbors(current->x, current->y)) {
            int nx = coord.first;
            int ny = coord.second;
            Cell& neighbor = maze.at(nx, ny);

            if (neighbor.visited || neighbor.wall)
                continue;

            int tentativeG = current->g_cost + 1;

            // If this path to neighbor is shorter
            if (tentativeG < neighbor.g_cost) {
                neighbor.g_cost = tentativeG;
                neighbor.h_cost = Heuristic::manhattan(
                    {neighbor.x, neighbor.y}, {endCell.x, endCell.y});
                neighbor.parent = current;
                pq.push(&neighbor);
            }
        }
    }

    // ✅ Reconstruct path
    if (endCell.parent != nullptr || (&endCell == &startCell))
        result.path = PathHelper::reconstructPath(&endCell);

    // ✅ Fill metrics
    result.metrics.pathLength = static_cast<int>(result.path.size());
    result.metrics.nodesExplored = static_cast<int>(result.visitedOrder.size());
    result.metrics.timeTakenMs = timer.elapsedMs();

    return result;
}
