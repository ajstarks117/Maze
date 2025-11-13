#include "backend/core/Maze.h"
#include "backend/algorithms/Dijkstra.h"
#include "backend/algorithms/AStar.h"
#include "backend/algorithms/DoubleAStar.h"

#include "ui/MazeWidget.h"   // Your drawing widget

private:
Maze *maze;
MazeWidget *mazeWidget;

// UI widgets (connect these from Designer)
QLineEdit *widthInput;
QLineEdit *heightInput;
QPushButton *generateBtn;

QRadioButton *astarBtn;
QRadioButton *dijkstraBtn;
QRadioButton *biastarBtn;

QPushButton *startBtn;
QPushButton *resetBtn;
