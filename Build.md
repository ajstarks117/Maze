# Maze Solver - Run Guide

## 🚀 Quick Start

### Prerequisites
- **Windows 10/11** with Command Prompt or PowerShell
- **OR** MSYS2 UCRT64 terminal (recommended)
- No additional installations required!

### Method 1: One-Click Run (Easiest)
1. Download the project files to a folder called `MazeSolver`
2. Double-click `run.bat` (if provided) or use the command below:
3. Open Command Prompt in the MazeSolver folder and run:
   ```cmd
   MazeSolver.exe
   ```

### Method 2: Compile from Source
```cmd
# Navigate to project folder
cd MazeSolver

# Compile the project
g++ -std=c++17 -O2 -pthread -I. core\Maze.cpp core\Utility.cpp algorithms\Dijkstra.cpp algorithms\AStar.cpp algorithms\DoubleAStar.cpp algorithms\JumpPointSearch.cpp main.cpp -o MazeSolver.exe

# Run it
MazeSolver.exe
```

## 📁 Project Structure
```
MazeSolver/
 ├── core/           # Core maze and utility classes
 ├── algorithms/     # Pathfinding algorithms
 ├── main.cpp        # Main menu program
 └── MazeSolver.exe  # Compiled executable
```

## 🎮 How to Use

### Main Menu Options:
1. **Set Maze Size** - Change maze dimensions (5-101)
2. **Generate New Maze** - Create a random maze
3. **Solve with Dijkstra** - Classic uniform-cost search
4. **Solve with A*** - Heuristic-guided search  
5. **Solve with Bidirectional A*** - Search from both ends
6. **Solve with Jump Point Search** - Optimized A* variant
7. **Solve with All Algorithms** - Compare all methods
8. **Robust Analysis** - Run multiple times for reliable timing
9. **Display Current Maze** - Show the maze without solving
10. **Exit** - Quit the program

### Example Session:
```
1. Set maze size to 31x31
2. Generate new maze
3. Solve with A* to see the path
7. Compare all algorithms to see performance differences
8. Run robust analysis for reliable timing data
```

## ⚡ Performance Tips

- **Small mazes (21x21)**: Fast results, good for testing
- **Medium mazes (51x51)**: Better performance comparison
- **Large mazes (101x101)**: Stress test algorithms
- **Use Option 8**: For most reliable timing data (multiple runs)

## 🔧 Troubleshooting

### If MazeSolver.exe doesn't exist:
```cmd
# Recompile the project
g++ -std=c++17 -O2 -pthread -I. core\*.cpp algorithms\*.cpp main.cpp -o MazeSolver.exe
```

### If g++ is not found:
- Use the pre-compiled `MazeSolver.exe` if available
- Or install MinGW-w64 and add to PATH

### If you see timeout messages:
- The maze might be too complex
- Try generating a new maze
- Reduce maze size

## 🎯 What You'll See

- **Maze Display**: `#` = walls, `S` = start, `E` = end, `.` = path
- **Performance Metrics**: Path length, nodes explored, time taken
- **Algorithm Comparison**: See which method is fastest
- **Path Visualization**: Watch the solution unfold

## 📊 Understanding the Output

```
===== A* =====
Path Length: 73          # Steps from start to goal
Nodes Explored: 75       # Cells checked during search  
Time: 1.14 ms           # Execution time
Success: Yes            # Whether path was found
```

## 🌟 Features Demonstrated

- **4 Pathfinding Algorithms**: Dijkstra, A*, Bidirectional A*, JPS
- **Recursive Backtracking**: Maze generation algorithm
- **Robust Timing**: Accurate performance measurements
- **Visualization**: See the solution path
- **Menu System**: Easy-to-use interface

## 🤝 Share with Friends

Just send them:
1. The `MazeSolver` folder with all files
2. This `RUN.md` guide
3. The `MazeSolver.exe` executable

They can run it immediately without any installations!

---

**Enjoy exploring pathfinding algorithms!** 🎮

*Project by GROUP-10 - C++17 Maze Solver*