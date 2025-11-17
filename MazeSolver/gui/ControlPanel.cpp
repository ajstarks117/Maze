#include "ControlPanel.h"
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    
    setupMazeControls(mainLayout);
    setupAlgorithmControls(mainLayout);
    setupSimulationControls(mainLayout);
    setupResultsPanel(mainLayout);
    mainLayout->addStretch(); // Push everything to top
    
    // Connect backend signals
    BackendInterface::onSolveComplete = [this](const AlgorithmResult& results) {
        updateResults(results);
    };
}

void ControlPanel::setupMazeControls(QVBoxLayout* mainLayout) {
    auto* group = new QGroupBox("Maze Setup");
    auto* layout = new QVBoxLayout(group);
    
    auto* sizeLayout = new QHBoxLayout();
    widthSpin_ = new QSpinBox();
    widthSpin_->setRange(5, 101);
    widthSpin_->setValue(21);
    heightSpin_ = new QSpinBox();
    heightSpin_->setRange(5, 101);
    heightSpin_->setValue(21);
    
    sizeLayout->addWidget(new QLabel("Width:"));
    sizeLayout->addWidget(widthSpin_);
    sizeLayout->addWidget(new QLabel("Height:"));
    sizeLayout->addWidget(heightSpin_);
    
    generatorCombo_ = new QComboBox();
    generatorCombo_->addItem("Recursive Backtracker");
    generatorCombo_->addItem("Prim's Algorithm");
    generatorCombo_->addItem("Kruskal's Algorithm");
    generatorCombo_->addItem("DFS Randomized");
    
    generateButton_ = new QPushButton("Generate New Maze");
    
    layout->addLayout(sizeLayout);
    layout->addWidget(new QLabel("Generator:"));
    layout->addWidget(generatorCombo_);
    layout->addWidget(generateButton_);
    
    connect(generateButton_, &QPushButton::clicked, this, &ControlPanel::onGenerateClicked);
    
    mainLayout->addWidget(group);
}

void ControlPanel::setupAlgorithmControls(QVBoxLayout* mainLayout) {
    auto* group = new QGroupBox("Pathfinding Algorithm");
    auto* layout = new QVBoxLayout(group);
    
    algorithmGroup_ = new QButtonGroup(this);
    
    dijkstraRadio_ = new QRadioButton("Dijkstra's Algorithm");
    astarRadio_ = new QRadioButton("A* Search");
    bidirectionalRadio_ = new QRadioButton("Bidirectional A*");
    jpsRadio_ = new QRadioButton("Jump Point Search");
    
    algorithmGroup_->addButton(dijkstraRadio_, 0);
    algorithmGroup_->addButton(astarRadio_, 1);
    algorithmGroup_->addButton(bidirectionalRadio_, 2);
    algorithmGroup_->addButton(jpsRadio_, 3);
    
    astarRadio_->setChecked(true); // Default selection
    
    layout->addWidget(dijkstraRadio_);
    layout->addWidget(astarRadio_);
    layout->addWidget(bidirectionalRadio_);
    layout->addWidget(jpsRadio_);
    
    mainLayout->addWidget(group);
}

void ControlPanel::setupSimulationControls(QVBoxLayout* mainLayout) {
    auto* group = new QGroupBox("Simulation Controls");
    auto* layout = new QVBoxLayout(group);
    
    solveButton_ = new QPushButton("Start Solve");
    resetButton_ = new QPushButton("Reset");
    stepButton_ = new QPushButton("Step Forward");
    
    auto* speedLayout = new QHBoxLayout();
    speedLayout->addWidget(new QLabel("Speed:"));
    speedSlider_ = new QSlider(Qt::Horizontal);
    speedSlider_->setRange(1, 10);
    speedSlider_->setValue(5);
    speedLayout->addWidget(speedSlider_);
    
    layout->addWidget(solveButton_);
    layout->addWidget(resetButton_);
    layout->addWidget(stepButton_);
    layout->addLayout(speedLayout);
    
    connect(solveButton_, &QPushButton::clicked, this, &ControlPanel::onSolveClicked);
    connect(resetButton_, &QPushButton::clicked, this, &ControlPanel::onResetClicked);
    connect(stepButton_, &QPushButton::clicked, this, &ControlPanel::onStepClicked);
    connect(speedSlider_, &QSlider::valueChanged, this, &ControlPanel::onSpeedChanged);
    
    mainLayout->addWidget(group);
}

void ControlPanel::setupResultsPanel(QVBoxLayout* mainLayout) {
    auto* group = new QGroupBox("Results");
    auto* layout = new QVBoxLayout(group);
    
    statusLabel_ = new QLabel("Status: Ready");
    pathLengthLabel_ = new QLabel("Path Length: --");
    nodesExploredLabel_ = new QLabel("Nodes Explored: --");
    timeTakenLabel_ = new QLabel("Time Taken: --");
    
    layout->addWidget(statusLabel_);
    layout->addWidget(pathLengthLabel_);
    layout->addWidget(nodesExploredLabel_);
    layout->addWidget(timeTakenLabel_);
    
    mainLayout->addWidget(group);
}

void ControlPanel::onGenerateClicked() {
    int width = widthSpin_->value();
    int height = heightSpin_->value();
    MazeGenerator generator = static_cast<MazeGenerator>(generatorCombo_->currentIndex());
    
    BackendInterface::generateMaze(width, height, generator);
    emit mazeGenerated();
    statusLabel_->setText("Status: Maze Generated");
}

void ControlPanel::onSolveClicked() {
    int algorithmId = algorithmGroup_->checkedId();
    int speed = speedSlider_->value();
    
    PathfindingAlgorithm algorithm = static_cast<PathfindingAlgorithm>(algorithmId);
    BackendInterface::startSolve(algorithm, speed);
    emit solveStarted();
    statusLabel_->setText("Status: Solving...");
}

void ControlPanel::onResetClicked() {
    BackendInterface::reset();
    emit resetRequested();
    statusLabel_->setText("Status: Reset");
    updateResults(AlgorithmResult());
}

void ControlPanel::onStepClicked() {
    BackendInterface::step();
    emit stepRequested();
}

void ControlPanel::onSpeedChanged(int value) {
    // Speed change can be handled in real-time if needed
    Q_UNUSED(value);
}

void ControlPanel::updateResults(const AlgorithmResult& results) {
    if (results.success) {
        statusLabel_->setText("Status: Solved");
        pathLengthLabel_->setText(QString("Path Length: %1").arg(results.pathLength));
        nodesExploredLabel_->setText(QString("Nodes Explored: %1").arg(results.nodesExplored));
        timeTakenLabel_->setText(QString("Time Taken: %1 ms").arg(results.timeTakenMs));
    } else {
        statusLabel_->setText("Status: No Solution Found");
        pathLengthLabel_->setText("Path Length: --");
        nodesExploredLabel_->setText("Nodes Explored: --");
        timeTakenLabel_->setText("Time Taken: --");
    }
}