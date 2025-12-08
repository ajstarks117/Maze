#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QScrollArea>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Maze Solver Visualizer");
    setMinimumSize(1100, 750); 

    setupUI();
    setupConnections();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    splitter_ = new QSplitter(Qt::Horizontal, centralWidget);

    // ===============================================
    // LEFT SIDE: Maze (Top) + Insights (Bottom)
    // ===============================================
    QWidget* leftContainer = new QWidget(splitter_);
    leftContainer->setObjectName("mazeContainer");
    
    QVBoxLayout* leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0); 

    // 1. Maze Widget
    mazeWidget_ = new MazeWidget(leftContainer);
    mazeWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // 2. Insight Display (Bottom Panel)
    // [CHANGE] Using QTextBrowser here
    insightDisplay_ = new QTextBrowser(leftContainer);
    insightDisplay_->setReadOnly(true);
    insightDisplay_->setFixedHeight(180);
    insightDisplay_->setPlaceholderText("Solve the maze to see algorithm insights here...");
    
    // [WORKS NOW] This function exists in QTextBrowser
    insightDisplay_->setOpenExternalLinks(true); 
    
    insightDisplay_->setStyleSheet(R"(
        QTextBrowser {
            background-color: #ffffff;
            color: #333;
            border: none;
            border-top: 1px solid #e0e0e0; 
            padding: 16px;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }
    )");

    leftLayout->addWidget(mazeWidget_);
    leftLayout->addWidget(insightDisplay_);

    splitter_->addWidget(leftContainer);

    // ===============================================
    // RIGHT SIDE: Control Panel
    // ===============================================
    QScrollArea* scrollArea = new QScrollArea(splitter_);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; }");

    controlPanel_ = new ControlPanel(scrollArea);
    scrollArea->setWidget(controlPanel_);

    splitter_->addWidget(scrollArea);

    splitter_->setStretchFactor(0, 75); 
    splitter_->setStretchFactor(1, 25); 

    mainLayout->addWidget(splitter_);
}

void MainWindow::setupConnections() {
    connect(controlPanel_, &ControlPanel::mazeGenerated, mazeWidget_, &MazeWidget::resetView);
    connect(controlPanel_, &ControlPanel::resetRequested, mazeWidget_, &MazeWidget::resetView);

    // [CHANGE] Connect to setText since QTextBrowser doesn't have a direct setHtml slot that matches exactly,
    // but setHtml is available as a public slot in base class, however sometimes explicit cast helps.
    // Actually, QTextBrowser inherits setHtml from QTextEdit, so this works fine:
    connect(controlPanel_, &ControlPanel::explanationUpdated, 
            insightDisplay_, &QTextBrowser::setHtml);

    BackendInterface::get().onAnimationFrame = [this](const AnimationFrame& frame) {
        mazeWidget_->updateAnimationFrame(frame);
    };

    BackendInterface::get().onMazeGenerated = [this]() {
        mazeWidget_->resetView();
    };
}