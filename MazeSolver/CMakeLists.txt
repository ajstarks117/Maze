cmake_minimum_required(VERSION 3.16)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly list source files (don't use GLOB_RECURSE)
set(SRC_FILES
    main.cpp
    core/Maze.cpp
    core/Utility.cpp
    algorithms/Dijkstra.cpp
    algorithms/AStar.cpp
    algorithms/DoubleAStar.cpp
    algorithms/JumpPointSearch.cpp
)

# Create executable
add_executable(MazeSolver ${SRC_FILES})

# Compiler settings
if(MSVC)
    target_compile_options(MazeSolver PRIVATE /W4)
else()
    target_compile_options(MazeSolver PRIVATE -Wall -Wextra -O2)
endif()