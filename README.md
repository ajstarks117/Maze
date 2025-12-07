🧩 Maze Solver - Interactive Pathfinding Visualizer
https://img.shields.io/badge/C++-17-blue.svg
https://img.shields.io/badge/Platform-Windows-lightgrey.svg
https://img.shields.io/badge/License-MIT-green.svg

A comprehensive C++17 terminal-based maze solver that implements and compares multiple pathfinding algorithms with real-time visualization and robust performance analysis.

🎯 Features
🧠 Multiple Pathfinding Algorithms
Dijkstra's Algorithm - Uniform cost search

A Search* - Heuristic-guided optimal pathfinding

Bidirectional A* - Simultaneous forward/backward search

Jump Point Search - Optimized A* for uniform-cost grids

🏗️ Maze Generation
Recursive Backtracking - Perfect maze generation

Customizable Sizes - 5x5 to 101x101 mazes.

Guaranteed Solvability - Always generates solvable mazes

📊 Advanced Analytics
Real-time Performance Metrics - Execution time, nodes explored, path length

Robust Statistical Analysis - Multiple runs with standard deviation

Comparative Analysis - Side-by-side algorithm comparison

Path Validation - Ensures solution correctness

🎮 Interactive Interface
Menu-driven System - Easy navigation and control

Visual Path Display - ASCII visualization with path tracing

Live Performance Data - Microsecond-precision timing

Error Handling - Timeout protection and crash recovery

🚀 Quick Start
Prerequisites
Windows 10/11 with Command Prompt

OR MSYS2 UCRT64 terminal

No additional installations required!

Running the Application
Method 1: One-Click Execution
cmd
# Simply double-click or run:
MazeSolver.exe
Method 2: Compile from Source
cmd
# Compile the project
g++ -std=c++17 -O2 -pthread -I. core\Maze.cpp core\Utility.cpp algorithms\*.cpp main.cpp -o MazeSolver.exe

# Run the executable
MazeSolver.exe
📁 Project Structure
text
MazeSolver/
├── core/
│   ├── Cell.h/cpp              # Maze cell representation
│   ├── Maze.h/cpp              # Maze generation and management
│   ├── Utility.h/cpp           # Timing and utility functions
│   ├── Types.h                 # Common data structures
│   └── AlgorithmResult.h       # Algorithm results container
├── algorithms/
│   ├── Dijkstra.h/cpp          # Dijkstra's algorithm
│   ├── AStar.h/cpp             # A* search algorithm
│   ├── DoubleAStar.h/cpp       # Bidirectional A*
│   └── JumpPointSearch.h/cpp   # Jump Point Search
├── main.cpp                    # Interactive menu system
├── MazeSolver.exe              # Compiled executable
└── README.md                   # This file
🎮 How to Use
Launch the application: Run MazeSolver.exe

Set maze size: Choose option 1 (recommended: 21x21 for testing)

Generate maze: Choose option 2 to create a new maze

Solve with algorithms: Options 3-6 for individual algorithms

Compare performance: Option 7 for side-by-side comparison

Robust analysis: Option 8 for statistical reliability testing

Example Session
text
1. Set maze size to 31x31
2. Generate new maze  
3. Solve with A* to see the optimal path
7. Compare all algorithms to see performance differences
8. Run robust analysis for reliable timing data
📊 Performance Insights
Algorithm Efficiency (Typical Results)
Jump Point Search 🥇 - Fastest, optimized for grids

A* 🥈 - Excellent balance of speed and optimality

Bidirectional A* 🥉 - Good for complex mazes

Dijkstra - Reliable but slower, explores uniformly

Sample Output
text
===== A* =====
Path Length: 73
Nodes Explored: 75
Time: 1.14 ms
Success: Yes

📊 PERFORMANCE COMPARISON
Algorithm           Path Length  Nodes Explored  Time      Success
Dijkstra            73           79              1.415 ms  Yes
A*                  73           75              1.140 ms  Yes  
Bidirectional A*    73           107             918 μs    Yes
Jump Point Search   73           75              33 μs     Yes
🔧 Technical Details
Built With
C++17 - Modern C++ standards

STL Algorithms - Priority queues, vectors, chrono

Multi-threading - Robust timing with timeout protection

Template-free Design - Simple, readable code

Key Algorithms
Recursive Backtracking - O(n) maze generation

A Search* - O(b^d) with Manhattan heuristic

Jump Point Search - O(b^d) with path compression

Bidirectional Search - O(b^(d/2)) reduced search space

Robust Features
✅ Timeout Protection - Prevents infinite loops

✅ Path Validation - Ensures solution correctness

✅ Exception Handling - Graceful error recovery

✅ Statistical Analysis - Reliable performance data

✅ Memory Safety - Proper resource management

🎓 Educational Value
This project demonstrates:

Graph Theory - Maze as connected graph

Algorithm Analysis - Time/space complexity comparison

Data Structures - Priority queues, stacks, vectors

Software Engineering - Modular design, error handling

Performance Optimization - Efficient algorithm implementation

🤝 Contributing
Feel free to:

Add new pathfinding algorithms

Implement different maze generation techniques

Enhance the visualization system

Improve performance metrics

Add unit tests

📜 License
This project is open source and available under the MIT License.

👥 Authors
Ajinkya Ubale - ajstarks117

Ajaya Nandiyawar - Ajaya-Nandiyawar

Rishi Agrawal - rishiagrawal02

Abhijeet Ambat - IPPYON596

🙏 Acknowledgments
Inspired by classical pathfinding algorithms

Built for educational and research purposes

Thanks to the C++ community for best practices

⭐ Star this repo if you found it helpful!

Happy pathfinding! 🎮