#pragma once

#include <QMainWindow>
#include <QSplitter>
#include "MazeWidget.h"
#include "ControlPanel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void setupUI();
    void setupConnections();

    QSplitter* splitter_;      // Split view (Maze | Controls)
    MazeWidget* mazeWidget_;   // Left side (maze visualizer)
    ControlPanel* controlPanel_; // Right side (controls)
};

