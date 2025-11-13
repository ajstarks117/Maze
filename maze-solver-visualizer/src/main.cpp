#include "core/Maze.h"
#include "core/Utility.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/AStar.h"
#include "algorithms/DoubleAStar.h"
#include <iostream>
using namespace std;

int main() {
    // Create a maze of size 31x31 (odd dimensions recommended)
    Maze maze(101, 101);
    maze.generateRecursiveBacktracker();

    cout << "Maze (ASCII):\n" << maze.toAscii() << "\n\n";

    // -------------------------------
    // Run Dijkstra
    // -------------------------------
    AlgorithmResult dijkstraResult = DijkstraSolver::solve(maze);
    cout << "===== Dijkstra Algorithm =====\n";
    cout << "Path length   : " << dijkstraResult.metrics.pathLength << "\n";
    cout << "Nodes explored: " << dijkstraResult.metrics.nodesExplored << "\n";
    cout << "Time taken    : " << dijkstraResult.metrics.timeTakenMs << " ms\n\n";

    // -------------------------------
    // Run A*
    // -------------------------------
    AlgorithmResult aStarResult = AStarSolver::solve(maze);
    cout << "===== A* Algorithm =====\n";
    cout << "Path length   : " << aStarResult.metrics.pathLength << "\n";
    cout << "Nodes explored: " << aStarResult.metrics.nodesExplored << "\n";
    cout << "Time taken    : " << aStarResult.metrics.timeTakenMs << " ms\n\n";

    // -------------------------------
    // Run Double A*
    // -------------------------------
    AlgorithmResult doubleAStarResult = DoubleAStarSolver::solve(maze);
    cout << "===== Double A* (Bidirectional) =====\n";
    cout << "Path length   : " << doubleAStarResult.metrics.pathLength << "\n";
    cout << "Nodes explored: " << doubleAStarResult.metrics.nodesExplored << "\n";
    cout << "Time taken    : " << doubleAStarResult.metrics.timeTakenMs << " ms\n\n";

    return 0;
}
