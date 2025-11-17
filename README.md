<div align="center">

# 🧩 **Maze Solver – Interactive Pathfinding Visualizer**

![C++17](https://img.shields.io/badge/C++-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A powerful **C++17 terminal-based maze solver** featuring real-time visualization, multiple algorithms, performance analysis, and a clean interactive interface.

---

</div>

## 📚 **Table of Contents**
1. [Overview](#-overview)
2. [Features](#-features)
3. [Quick Start](#-quick-start)
4. [Project Structure](#-project-structure)
5. [Usage Guide](#-usage-guide)
6. [Performance Insights](#-performance-insights)
7. [Technical Details](#-technical-details)
8. [Educational Value](#-educational-value)
9. [Contributing](#-contributing)
10. [License](#-license)
11. [Authors](#-authors)
12. [Acknowledgments](#-acknowledgments)

---

## 🧭 **Overview**
This project is a fully interactive **pathfinding visualizer** built in modern **C++17**, designed to compare the performance of famous graph search algorithms on grid-based mazes.

It includes:
- Professional code architecture  
- Clean ASCII visualization  
- Robust analytical tools  
- Multiple algorithms + performance comparison  

---

## 🎯 **Features**

### 🧠 **Pathfinding Algorithms**
- **Dijkstra’s Algorithm** – Uniform-cost search  
- **A\*** – Heuristic-guided search  
- **Bidirectional A\*** – Faster convergence  
- **Jump Point Search (JPS)** – Optimized A\* for grid maps  

### 🏗️ **Maze Generation**
- **Recursive Backtracking** (perfect mazes)  
- Supports **5×5** to **101×101** maze sizes  
- Always generates a **solvable** maze  

### 📊 **Analytics Engine**
- Execution time (microsecond precision)  
- Nodes explored  
- Path length  
- Multi-run statistical analysis (std deviation)  
- Algorithm comparison table  

### 🎮 **Interactive Terminal UI**
- Menu-driven system  
- ASCII maze visualization  
- Timeout protection  
- Crash-safe execution  

---

## 🚀 **Quick Start**

### ✔️ **Prerequisites**
- Windows 10/11  
- CMD or MSYS2 UCRT64  
- No external libraries needed  

---

### ▶️ **Running the Executable**
```cmd
MazeSolver.exe
▶️ Compile from Source
g++ -std=c++17 -O2 -pthread -I. core\Maze.cpp core\Utility.cpp algorithms\*.cpp main.cpp -o MazeSolver.exe
MazeSolver.exe
```
---
### 📁 Project Structure
```cmd
MazeSolver/
├── core/
│   ├── Cell.h/.cpp              # Maze cell representation
│   ├── Maze.h/.cpp              # Maze creation & handling
│   ├── Utility.h/.cpp           # Timing utilities
│   ├── Types.h                  # Shared structs
│   └── AlgorithmResult.h        # Algorithm result container
├── algorithms/
│   ├── Dijkstra.h/.cpp          # Dijkstra’s algorithm
│   ├── AStar.h/.cpp             # A* search
│   ├── DoubleAStar.h/.cpp       # Bidirectional A*
│   └── JumpPointSearch.h/.cpp   # Jump Point Search
├── main.cpp                     # Menu + app logic
├── MazeSolver.exe               # Ready-to-run executable
└── README.md                    # This file
```
---
🎮 Usage Guide
Steps:
- Run the application
- Set maze size – Option 1
- Generate maze – Option 2
- Run any algorithm – Options 3 to 6
- Compare algorithms – Option 7
- Run reliability analysis – Option 8
```cmd
Sample Session:
1. Set maze size to 31x31
2. Generate new maze
3. Run A*
7. Compare all algorithms
8. Run robust multi-run analysis
```
---
## Performance Insights
### Algorithm Ranking (Typical)
| Rank | Algorithm         | Reason                      |
| ---- | ----------------- | --------------------------- |
| 🥇 1 | Jump Point Search | Fastest due to pruning      |
| 🥈 2 | A*                | Balanced speed + optimality |
| 🥉 3 | Bidirectional A*  | Faster for large grids      |
| 4    | Dijkstra          | Slow but consistent         |

---

#### 📌 Sample Output

===== A* =====
Path Length: 73
Nodes Explored: 75
Time: 1.14 ms
Success: Yes

#### 📌 Comparison Table

Algorithm           Path Length  Nodes Explored  Time        Success
Dijkstra            73           79              1.415 ms    Yes
A*                  73           75              1.140 ms    Yes
Bidirectional A*    73           107             918 µs      Yes
Jump Point Search   73           75              33 µs       Yes


### 🔧 Technical Details
Built With
- C++17
- STL (priority_queue, vector, chrono)
- Multi-threaded timing
- Zero external dependencies
- Algorithmic Concepts
- Recursive Backtracking
- Manhattan-distance heuristic
- Jump Point pruning
- Bidirectional search space reduction

### Reliability Features
- [x] Timeout protection
- [x] Memory safety
- [x] Path verification
- [x] Statistical performance testing
- [x] Clean error handling

### 🎓 Educational Value

This project teaches:
- Graph theory (maze as a graph)
- Pathfinding algorithms
- Time & space complexity
- Clean C++ architecture
- Benchmarking & analytics
- Performance optimization

🤝 Contributing

You can contribute by:
- Adding new algorithms
- Adding more maze generators
- Improving visualization
- Enhancing analysis
- Writing unit tests

📜 License

This project is licensed under the MIT License.

👥 Authors

- **Ajinkya Ubale** - [@ajstarks117](https://github.com/ajstarks117)
- **Ajaya Nandiyawar** - [@Ajaya-Nandiyawar](https://github.com/Ajaya-Nandiyawar)
- **Rishi Agrawal** - [@rishiagrawal02](https://github.com/rishiagrawal02)
- **Abhijeet Ambat** - [@IPPYON596](https://github.com/IPPYON596)

🙏 Acknowledgments

Inspired by classic pathfinding literature

Developed for research & education

Thanks to the C++ community for guidance

<div align="center">

⭐ If you like this project, consider giving it a star!
🎮 Happy Pathfinding!

</div>

