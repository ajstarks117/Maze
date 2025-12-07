# 🧩 Maze Solver - Build & Run Guide

This guide explains how to build and run both the **Terminal** and **GUI** versions of the Maze Solver project.

---

## ✅ Prerequisites

Before you begin, ensure you have the following installed:

1.  **C++ Compiler**: MinGW-w64 (GCC) or MSVC.
2.  **CMake**: Version 3.16 or higher ([Download](https://cmake.org/download/)).
3.  **Qt 6 SDK**: Required for the GUI version ([Download](https://www.qt.io/download-qt-installer)).
    * *Note:* During installation, ensure you select the **MinGW** component if you are using MinGW.

---

## 💻 1. Terminal Version

The terminal version is a command-line tool for running algorithms and analyzing performance stats.

### Option A: Using CMake (Recommended)

1.  Open a terminal in the project folder (`MazeSolver/`).
2.  Run the following commands:

    ```bash
    # 1. Create a build directory
    mkdir build_terminal
    cd build_terminal

    # 2. Configure the project
    cmake -G "MinGW Makefiles" ..

    # 3. Build the executable
    mingw32-make
    ```

3.  **Run:**
    ```bash
    ./MazeSolver.exe
    ```

### Option B: Using g++ Directly (Quick)

If you just want to compile the terminal version without CMake:

```bash
cd MazeSolver

g++ -std=c++17 -O2 -pthread \
    main.cpp \
    core/Maze.cpp \
    core/Utility.cpp \
    algorithms/Dijkstra.cpp \
    algorithms/AStar.cpp \
    algorithms/DoubleAStar.cpp \
    algorithms/JumpPointSearch.cpp \
    -o MazeSolver.exe

# Run
./MazeSolver.exe



