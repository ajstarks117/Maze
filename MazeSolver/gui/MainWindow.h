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
    
    QSplitter* splitter_;
    MazeWidget* mazeWidget_;
    ControlPanel* controlPanel_;
};