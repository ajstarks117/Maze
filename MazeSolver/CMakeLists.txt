cmake_minimum_required(VERSION 3.16)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt6 path explicitly
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64")

# Find Qt6 components - MUST BE BEFORE ANY QT COMMANDS
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Set MinGW specific settings
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# Terminal version (your existing code)
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

# Find Threads for terminal version
find_package(Threads REQUIRED)
target_link_libraries(MazeSolver Threads::Threads)

# GUI version - ONLY if Qt6 is found
if(Qt6_FOUND)
    message(STATUS "Qt6 found, building GUI version")
    
    # List all header files that contain Q_OBJECT
    set(GUI_HEADERS
        gui/MainWindow.h
        gui/MazeWidget.h
        gui/ControlPanel.h
    )
    
    # List all source files
    set(GUI_SOURCES
        gui/main_gui.cpp
        gui/MainWindow.cpp
        gui/MazeWidget.cpp
        gui/ControlPanel.cpp
    )

    # Generate MOC files
    qt6_generate_moc("gui/MainWindow.h" "moc_MainWindow.cpp")
    qt6_generate_moc("gui/MazeWidget.h" "moc_MazeWidget.cpp")
    qt6_generate_moc("gui/ControlPanel.h" "moc_ControlPanel.cpp")
    
    set(GUI_MOC_SOURCES
        "moc_MainWindow.cpp"
        "moc_MazeWidget.cpp"
        "moc_ControlPanel.cpp"
    )

    add_executable(MazeSolverGUI ${GUI_SOURCES} ${GUI_MOC_SOURCES})

    # Link Qt libraries
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
        ${CMAKE_CURRENT_BINARY_DIR}  # For generated MOC files
    )

    # Copy Qt DLLs for MinGW (Windows)
    if(WIN32 AND MINGW)
        add_custom_command(TARGET MazeSolverGUI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "C:/Qt/6.9.3/mingw_64/bin/Qt6Core.dll" "$<TARGET_FILE_DIR:MazeSolverGUI>"
            COMMAND ${CMAKE_COMMAND} -E copy
            "C:/Qt/6.9.3/mingw_64/bin/Qt6Widgets.dll" "$<TARGET_FILE_DIR:MazeSolverGUI>"
            COMMAND ${CMAKE_COMMAND} -E copy
            "C:/Qt/6.9.3/mingw_64/bin/Qt6Gui.dll" "$<TARGET_FILE_DIR:MazeSolverGUI>"
        )
    endif()
else()
    message(WARNING "Qt6 not found, skipping GUI version")
endif()

# Compiler settings
if(MSVC)
    target_compile_options(MazeSolver PRIVATE /W4)
    if(Qt6_FOUND)
        target_compile_options(MazeSolverGUI PRIVATE /W4)
    endif()
else()
    target_compile_options(MazeSolver PRIVATE -Wall -Wextra -O2)
    if(Qt6_FOUND)
        target_compile_options(MazeSolverGUI PRIVATE -Wall -Wextra -O2)
    endif()
endif()