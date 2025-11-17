#include "MainWindow.h"
#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Maze Solver Visualizer");
    setMinimumSize(1200, 800);
    
    setupUI();
    setupConnections();
}

void MainWindow::setupUI() {
    auto* centralWidget = new QWidget(this);
    auto* layout = new QHBoxLayout(centralWidget);
    
    splitter_ = new QSplitter(Qt::Horizontal, centralWidget);
    
    // Maze widget takes 70% of space
    mazeWidget_ = new MazeWidget(splitter_);
    splitter_->addWidget(mazeWidget_);
    
    // Control panel takes 30% of space
    controlPanel_ = new ControlPanel(splitter_);
    splitter_->addWidget(controlPanel_);
    
    splitter_->setStretchFactor(0, 7); // Maze: 70%
    splitter_->setStretchFactor(1, 3); // Controls: 30%
    
    layout->addWidget(splitter_);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections() {
    connect(controlPanel_, &ControlPanel::mazeGenerated, mazeWidget_, &MazeWidget::resetView);
    connect(controlPanel_, &ControlPanel::resetRequested, mazeWidget_, &MazeWidget::resetView);
    
    // Connect backend animation updates to maze widget
    BackendInterface::onAnimationFrame = [this](const AnimationFrame& frame) {
        mazeWidget_->updateAnimationFrame(frame);
    };
    
    BackendInterface::onMazeGenerated = [this]() {
        mazeWidget_->resetView();
    };
}