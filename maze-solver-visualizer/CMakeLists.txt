# ==========================================================
# Project setup
# ==========================================================
cmake_minimum_required(VERSION 3.16)
project(MazeSolverVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Force standard manually
if (MSVC)
    add_compile_options(/std:c++17)
else()
    add_compile_options(-std=c++17)
endif()


# ==========================================================
# Find Qt 6 (Widgets module + core essentials)
# ==========================================================
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64")
find_package(Qt6 6.9.3 REQUIRED COMPONENTS Core Gui Widgets)

# ==========================================================
# Collect all source files recursively
# ==========================================================
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.h
    src/gui/*.cpp
    src/gui/*.h
    src/core/*.cpp
    src/core/*.h
    src/algorithms/*.cpp
    src/algorithms/*.h
    src/visualization/*.cpp
    src/visualization/*.h
)

# ==========================================================
# Define executable target
# ==========================================================
qt_add_executable(MazeSolverVisualizer
    ${SRC_FILES}
    src/gui/resources.qrc
)

# ==========================================================
# Link with Qt libraries
# ==========================================================
target_link_libraries(MazeSolverVisualizer
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# ==========================================================
# Set output directory
# ==========================================================
set_target_properties(MazeSolverVisualizer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ==========================================================
# Enable automatic MOC, RCC, and UIC handling
# ==========================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ==========================================================
# Add include directories
# ==========================================================
target_include_directories(MazeSolverVisualizer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visualization
)

# ==========================================================
# Optional: enable warnings for debugging
# ==========================================================
if (MSVC)
    target_compile_options(MazeSolverVisualizer PRIVATE /W4)
else()
    target_compile_options(MazeSolverVisualizer PRIVATE -Wall -Wextra -pedantic)
endif()

# ==========================================================
# Print helpful message
# ==========================================================
message(STATUS "✅ Maze Solver Visualizer configured successfully!")
message(STATUS "💡 To build:")
message(STATUS "   mkdir build && cd build")
message(STATUS "   cmake .. && cmake --build .")
