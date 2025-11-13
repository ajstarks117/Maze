#include "mainwindow.h"
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    setWindowTitle("Maze Solver Visualizer");
    resize(1200, 700);

    // Left side: Maze view
    mazeScene = new QGraphicsScene(this);
    mazeView = new QGraphicsView(mazeScene, this);
    mazeView->setRenderHint(QPainter::Antialiasing);
    mazeView->setBackgroundBrush(Qt::black);

    // Right side: Control panel
    QWidget *controlPanel = new QWidget;
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);

    QLabel *title = new QLabel("Maze Solver Visualizer");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50;");

    algoSelector = new QComboBox;
    algoSelector->addItems({"Dijkstra", "A* Search", "Bidirectional A*"});

    runButton = new QPushButton("Run Algorithm");
    generateButton = new QPushButton("Generate New Maze");
    statsLabel = new QLabel("Path Length: -\nNodes Explored: -\nTime Taken: - ms");

    controlLayout->addWidget(title);
    controlLayout->addSpacing(10);
    controlLayout->addWidget(new QLabel("Select Algorithm:"));
    controlLayout->addWidget(algoSelector);
    controlLayout->addWidget(runButton);
    controlLayout->addWidget(generateButton);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(statsLabel);
    controlLayout->addStretch();

    // Splitter between maze view and control panel
    QSplitter *splitter = new QSplitter;
    splitter->addWidget(mazeView);
    splitter->addWidget(controlPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);

    // Connect buttons
    connect(runButton, &QPushButton::clicked, this, &MainWindow::runSelectedAlgorithm);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateNewMaze);

    // Draw initial maze
    visualizeMaze();
}

void MainWindow::visualizeMaze()
{
    mazeScene->clear();

    const int rows = 20;
    const int cols = 20;
    const int cellSize = 25;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QColor color = (QRandomGenerator::global()->bounded(100) < 30) ? Qt::black : Qt::white;
            mazeScene->addRect(j * cellSize, i * cellSize, cellSize, cellSize, Qt::NoPen, QBrush(color));
        }
    }

    // Add start and end points
    mazeScene->addRect(0, 0, cellSize, cellSize, Qt::NoPen, QBrush(Qt::green));
    mazeScene->addRect((cols - 1) * cellSize, (rows - 1) * cellSize, cellSize, cellSize, Qt::red);
}

void MainWindow::visualizePath()
{
    // Example visualization of a found path
    const int cellSize = 25;
    for (int i = 1; i < 19; ++i) {
        mazeScene->addRect(i * cellSize, i * cellSize, cellSize, cellSize, Qt::NoPen, QBrush(Qt::yellow));
    }
}

void MainWindow::runSelectedAlgorithm()
{
    QString selectedAlgo = algoSelector->currentText();

    // For now, simulate backend result
    int pathLength = QRandomGenerator::global()->bounded(100, 200);
    int nodesExplored = QRandomGenerator::global()->bounded(300, 600);
    double timeTaken = 1.0 + QRandomGenerator::global()->bounded(4000) / 1000.0;

    showAlgorithmResult(selectedAlgo, pathLength, nodesExplored, timeTaken);
    visualizePath();
}

void MainWindow::generateNewMaze()
{
    visualizeMaze();
    statsLabel->setText("Path Length: -\nNodes Explored: -\nTime Taken: - ms");
}

void MainWindow::showAlgorithmResult(const QString &name, int pathLen, int nodes, double timeMs)
{
    statsLabel->setText(
        QString("Algorithm: %1\nPath Length: %2\nNodes Explored: %3\nTime Taken: %4 ms")
            .arg(name).arg(pathLen).arg(nodes).arg(timeMs, 0, 'f', 3)
        );
}
