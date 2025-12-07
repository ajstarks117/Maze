#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Maze Solver Visualizer");
    setMinimumSize(1200, 800);

    setupUI();
    setupConnections();
}

void MainWindow::setupUI() {
    // -------------------------
    // CENTRAL WIDGET
    // -------------------------
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // -------------------------
    // SPLITTER (Maze | Control Panel)
    // -------------------------
    splitter_ = new QSplitter(Qt::Horizontal, centralWidget);

    // -------------------------
    // MAZE CONTAINER (White Card)
    // -------------------------
    QWidget* mazeContainer = new QWidget(splitter_);
    mazeContainer->setObjectName("mazeContainer");  // QSS styling

    QVBoxLayout* mazeLayout = new QVBoxLayout(mazeContainer);
    mazeLayout->setContentsMargins(20, 20, 20, 20);
    mazeLayout->setSpacing(0);

    // MazeWidget inside container
    mazeWidget_ = new MazeWidget(mazeContainer);
    mazeWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mazeLayout->addWidget(mazeWidget_);

    splitter_->addWidget(mazeContainer);

    // -------------------------
    // CONTROL PANEL
    // -------------------------
    controlPanel_ = new ControlPanel(splitter_);
    controlPanel_->setObjectName("controlPanel");

    splitter_->addWidget(controlPanel_);

    // Layout stretch: 70% maze, 30% controls
    splitter_->setStretchFactor(0, 7);
    splitter_->setStretchFactor(1, 3);

    // Add splitter to main layout
    mainLayout->addWidget(splitter_);
}

void MainWindow::setupConnections() {

    // Reset maze view when new maze is generated
    connect(controlPanel_, &ControlPanel::mazeGenerated,
            mazeWidget_, &MazeWidget::resetView);

    // Reset view when reset is clicked
    connect(controlPanel_, &ControlPanel::resetRequested,
            mazeWidget_, &MazeWidget::resetView);

    // Backend animation frame updates → maze visually animates
    BackendInterface::onAnimationFrame = [this](const AnimationFrame& frame) {
        mazeWidget_->updateAnimationFrame(frame);
    };

    // When maze is generated, reset visuals
    BackendInterface::onMazeGenerated = [this]() {
        mazeWidget_->resetView();
    };
}
