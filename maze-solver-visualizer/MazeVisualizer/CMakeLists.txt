cmake_minimum_required(VERSION 3.16)

project(MazeVisualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# -----------------------------------------
# Add both frontend (Qt) and backend sources
# -----------------------------------------
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    # Backend source files
    backend/core/Maze.cpp
    backend/core/Maze.h
    backend/core/Utility.cpp
    backend/core/Utility.h
    backend/core/AlgorithmResult.h

    backend/algorithms/Dijkstra.cpp
    backend/algorithms/Dijkstra.h
    backend/algorithms/AStar.cpp
    backend/algorithms/AStar.h
    backend/algorithms/DoubleAStar.cpp
    backend/algorithms/DoubleAStar.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MazeVisualizer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ui/MazeWidget.cpp ui/MazeWidget.h
    )
else()
    if(ANDROID)
        add_library(MazeVisualizer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(MazeVisualizer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MazeVisualizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(MazeVisualizer PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MazeVisualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MazeVisualizer)
endif()
