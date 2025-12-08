#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTextBrowser> // [CHANGE] Use QTextBrowser instead of QTextEdit
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
    
    // [CHANGE] The display area is now a browser
    QTextBrowser* insightDisplay_;
};