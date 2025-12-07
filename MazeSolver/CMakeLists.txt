cmake_minimum_required(VERSION 3.16)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt6 path explicitly
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64")

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Set MinGW specific settings
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# ========== TERMINAL VERSION ==========
set(TERMINAL_SRC_FILES
    main.cpp
    core/Maze.cpp
    core/Utility.cpp
    algorithms/Dijkstra.cpp
    algorithms/AStar.cpp
    algorithms/DoubleAStar.cpp
    algorithms/JumpPointSearch.cpp
)

add_executable(MazeSolver ${TERMINAL_SRC_FILES})

# Force console application on Windows
if(WIN32)
    target_link_options(MazeSolver PRIVATE -mconsole)
endif()

find_package(Threads REQUIRED)
target_link_libraries(MazeSolver Threads::Threads)

# ========== GUI VERSION ==========
if(Qt6_FOUND)
    set(GUI_SOURCES
        gui/main_gui.cpp
        gui/MainWindow.cpp
        gui/MazeWidget.cpp
        gui/ControlPanel.cpp
    )

    set(GUI_HEADERS
        gui/MainWindow.h
        gui/MazeWidget.h
        gui/ControlPanel.h
    )

    # Method 1: Use AUTOMOC property (modern approach)
    add_executable(MazeSolverGUI ${GUI_SOURCES})
    set_target_properties(MazeSolverGUI PROPERTIES
        AUTOMOC ON
    )

    target_link_libraries(MazeSolverGUI 
        Qt6::Core 
        Qt6::Widgets 
        Qt6::Gui
    )

    # Include directories for GUI
    target_include_directories(MazeSolverGUI PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
    )

    # Deploy Qt dependencies on Windows
    if(WIN32)
        find_program(WINDEPLOYQT windeployqt HINTS ${QT_DIR}/bin)
        if(WINDEPLOYQT)
            add_custom_command(TARGET MazeSolverGUI POST_BUILD
                COMMAND ${WINDEPLOYQT} --verbose --compiler-runtime "$<TARGET_FILE:MazeSolverGUI>"
                COMMENT "Deploying Qt dependencies..."
            )
        endif()
    endif()
else()
    message(WARNING "Qt6 not found, skipping GUI version")
endif()

# Compiler settings
target_compile_options(MazeSolver PRIVATE -Wall -Wextra -O2)
if(Qt6_FOUND)
    target_compile_options(MazeSolverGUI PRIVATE -Wall -Wextra -O2)
endif()