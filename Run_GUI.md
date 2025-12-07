# 2. GUI Version (Qt6)
## The GUI version visualizes the maze generation and solving process in real-time.

- Step 1: Locate your Qt Path
#### You need to know where Qt is installed. It usually looks like:

#### C:/Qt/6.x.x/mingw_64

- Note: Replace 6.x.x with your specific version (e.g., 6.9.3).

- Step 2: Build with CMake
Open a terminal in the project folder (MazeSolver/).

## Run the following commands (replace the Qt path with yours):

# 1. Create a clean build directory
'''bash
mkdir build_gui
cd build_gui
'''
# 2. Configure with Qt path
# REPLACE "C:/Qt/6.9.3/mingw_64" with your actual path!
'''bash
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.3/mingw_64" ..
'''
# 3. Build the application
'''bash
mingw32-make
'''

- Step 3: Run the GUI
Find the executable in the build_gui folder:
'''bash 
./MazeSolverGUI.exe
'''

---

# Troubleshooting
## "Qt6 not found" Error
### If CMake complains it cannot find Qt6:

- Ensure you installed the Qt 6.x -> MinGW component in the Qt Maintenance Tool.

- Check that the path in -DCMAKE_PREFIX_PATH is correct and points to the mingw_64 folder inside your Qt version.

- "Missing DLLs" (Qt6Core.dll, etc.)
-  If the app crashes or complains about missing DLLs when you try to run it:

#### Open your terminal.

#### Navigate to your build_gui folder.

#### Run the Qt deployment tool (adjust path as needed):

#### Bash
'''bash
C:/Qt/6.9.3/mingw_64/bin/windeployqt.exe MazeSolverGUI.exe
'''

- This copies all necessary Qt files next to your executable so it can run.

## "Maze is just an empty square / Animation is frozen"
- Ensure you have updated BackendInterface.h with the full logic provided in the fix guide.

- Ensure you have updated all algorithm files (Dijkstra.cpp, etc.) to call the stepCallback
