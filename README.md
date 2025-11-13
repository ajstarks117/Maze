# 🧩 Maze Solver Visualizer
A modern, interactive Qt-based application that visualizes pathfinding algorithms navigating through randomly generated mazes in real-time.

## 🚀 Features

### 🏗 Maze Generation
- Recursive Backtracker Maze Generation
- Adjustable width & height
- Instant regeneration

### 🔍 Pathfinding Algorithms
✔ **Dijkstra's Algorithm**  
✔ **A\* Search**  
✔ **Bidirectional A\* Search**  
*(Extendable — ready for more algorithms)*

### 🎨 Visualization
- Smooth animated maze traversal
- Real-time node exploration
- Color-coded:
  - Walls
  - Free cells
  - Start & end
  - Path explored
  - Final shortest path

### 🖥 Built With
- **C++17**
- **Qt 6 (Widgets, QGraphicsView)**
- **CMake**
- Custom rendering components:
  - `MazeWidget`
  - `Maze` grid engine
  - `AlgorithmResult` metrics

---

## 📂 Project Structure
maze-solver-visualizer/
│
├── backend/
│ ├── core/ # Maze, Cell, Utility classes
│ └── algorithms/ # Dijkstra, A*, Bidirectional A*
│
├── ui/
│ ├── MazeWidget.h # Custom maze renderer
│ └── MazeWidget.cpp
│
├── mainwindow.ui # Qt Creator UI layout
├── mainwindow.cpp
├── mainwindow.h
├── main.cpp
├── CMakeLists.txt
│
├── RUN.md # How to run
├── BUILD.md # How to build
└── INSTALL.md # How to distribute


---

## 🛠 Building the Project
Follow detailed instructions in:  
👉 [`BUILD.md`](BUILD.md)

---

## ▶️ Running the Application
Instructions available in:  
👉 [`RUN.md`](RUN.md)

---

## 📦 Packaging / Installer
To create a distributable `.exe` bundle:  
👉 [`INSTALL.md`](INSTALL.md)

---

## 🙌 Contributing
Feel free to submit:
- new algorithms  
- UI improvements  
- performance patches  

PRs are welcome!

---

## 📄 License
This project is released under the **MIT License**.

---

# 📗 **BUILD.md**

```md
# 🛠 BUILD.md — Build Instructions for Maze Solver Visualizer

This document explains how to compile the project using:

- Qt Creator (recommended)
- Terminal + CMake
- MinGW or MSVC toolchains

---

# 1️⃣ Requirements

✔ Qt 6.x  
✔ CMake 3.16+  
✔ MinGW or MSVC compiler  
✔ Ninja (optional, faster builds)

---

# 2️⃣ Build Using Qt Creator (Easiest)

1. Open **Qt Creator**
2. Select:
File → Open Project → CMakeLists.txt
4. Press **Configure Project**
5. Click the green **Run ▶**

Done! 🎉

---

# 3️⃣ Build Using Terminal

Navigate to project root:

```powershell
cd path\to\maze-solver-visualizer
