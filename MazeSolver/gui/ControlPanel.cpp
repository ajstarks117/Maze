#include "ControlPanel.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("controlPanel");

    // Main vertical layout for all group boxes
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(18);

    // Setup UI sections
    setupMazeControls(mainLayout);
    setupAlgorithmControls(mainLayout);
    setupSimulationControls(mainLayout);
    setupResultsPanel(mainLayout);

    mainLayout->addStretch(); // Push groups to top

    // Backend → Results UI updates
    BackendInterface::onSolveComplete = [this](const AlgorithmResult& results) {
        updateResults(results);
    };
}

//
// ========================
//  Maze Setup Controls
// ========================
//
void ControlPanel::setupMazeControls(QVBoxLayout* mainLayout)
{
    auto* group = new QGroupBox("Maze Setup");
    auto* layout = new QVBoxLayout(group);
    layout->setSpacing(10);

    // Size selection (width / height)
    auto* sizeLayout = new QHBoxLayout();
    sizeLayout->setSpacing(10);

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

    // Maze generator dropdown
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

    // Signal
    connect(generateButton_, &QPushButton::clicked,
            this, &ControlPanel::onGenerateClicked);

    mainLayout->addWidget(group);
}

//
// ========================
//  Algorithm Selection
// ========================
//
void ControlPanel::setupAlgorithmControls(QVBoxLayout* mainLayout)
{
    auto* group = new QGroupBox("Pathfinding Algorithm");
    auto* layout = new QVBoxLayout(group);
    layout->setSpacing(6);

    algorithmGroup_ = new QButtonGroup(this);

    dijkstraRadio_ = new QRadioButton("Dijkstra's Algorithm");
    astarRadio_ = new QRadioButton("A* Search");
    bidirectionalRadio_ = new QRadioButton("Bidirectional A*");
    jpsRadio_ = new QRadioButton("Jump Point Search");

    algorithmGroup_->addButton(dijkstraRadio_, 0);
    algorithmGroup_->addButton(astarRadio_, 1);
    algorithmGroup_->addButton(bidirectionalRadio_, 2);
    algorithmGroup_->addButton(jpsRadio_, 3);

    astarRadio_->setChecked(true); // Default

    layout->addWidget(dijkstraRadio_);
    layout->addWidget(astarRadio_);
    layout->addWidget(bidirectionalRadio_);
    layout->addWidget(jpsRadio_);

    mainLayout->addWidget(group);
}

//
// ========================
//  Simulation Controls
// ========================
//
void ControlPanel::setupSimulationControls(QVBoxLayout* mainLayout)
{
    auto* group = new QGroupBox("Simulation Controls");
    auto* layout = new QVBoxLayout(group);
    layout->setSpacing(10);

    solveButton_ = new QPushButton("Start Solve");
    resetButton_ = new QPushButton("Reset");
    resetButton_->setObjectName("resetButton"); // QSS: Gray button
    stepButton_ = new QPushButton("Step Forward");

    auto* buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(10);
    buttonRow->addWidget(solveButton_);
    buttonRow->addWidget(resetButton_);

    // Speed slider
    auto* speedLayout = new QHBoxLayout();
    speedLayout->setSpacing(8);
    speedLayout->addWidget(new QLabel("Speed:"));

    speedSlider_ = new QSlider(Qt::Horizontal);
    speedSlider_->setRange(1, 10);
    speedSlider_->setValue(5);

    speedLayout->addWidget(speedSlider_);

    layout->addLayout(buttonRow);
    layout->addWidget(stepButton_);
    layout->addLayout(speedLayout);

    // Signals
    connect(solveButton_, &QPushButton::clicked,
            this, &ControlPanel::onSolveClicked);

    connect(resetButton_, &QPushButton::clicked,
            this, &ControlPanel::onResetClicked);

    connect(stepButton_, &QPushButton::clicked,
            this, &ControlPanel::onStepClicked);

    connect(speedSlider_, &QSlider::valueChanged,
            this, &ControlPanel::onSpeedChanged);

    mainLayout->addWidget(group);
}

//
// ========================
//  Results Panel
// ========================
//
void ControlPanel::setupResultsPanel(QVBoxLayout* mainLayout)
{
    auto* group = new QGroupBox("Results");
    auto* layout = new QVBoxLayout(group);
    layout->setSpacing(6);

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

//
// ========================
//  Slots Implementation
// ========================
//

void ControlPanel::onGenerateClicked()
{
    int width = widthSpin_->value();
    int height = heightSpin_->value();
    MazeGenerator gen = static_cast<MazeGenerator>(generatorCombo_->currentIndex());

    BackendInterface::generateMaze(width, height, gen);

    emit mazeGenerated();
    statusLabel_->setText("Status: Maze Generated");
}

void ControlPanel::onSolveClicked()
{
    PathfindingAlgorithm algorithm =
        static_cast<PathfindingAlgorithm>(algorithmGroup_->checkedId());

    int speed = speedSlider_->value();

    BackendInterface::startSolve(algorithm, speed);

    emit solveStarted();
    statusLabel_->setText("Status: Solving...");
}

void ControlPanel::onResetClicked()
{
    BackendInterface::reset();
    emit resetRequested();

    statusLabel_->setText("Status: Reset");

    // Clear results
    updateResults(AlgorithmResult());
}

void ControlPanel::onStepClicked()
{
    BackendInterface::step();
    emit stepRequested();
}

void ControlPanel::onSpeedChanged(int value)
{
    Q_UNUSED(value);
    // Can be used if real-time speed adjustments are required
}

void ControlPanel::updateResults(const AlgorithmResult& results)
{
    if (results.success) {
        statusLabel_->setText("Status: Solved");
        // [FIX] Use .metrics. to access the data correctly
        pathLengthLabel_->setText(QString("Path Length: %1").arg(results.metrics.pathLength));
        nodesExploredLabel_->setText(QString("Nodes Explored: %1").arg(results.metrics.nodesExplored));
        
        double timeVal = results.metrics.timeTakenMs;
        if (timeVal < 1000) {
             timeTakenLabel_->setText(QString("Time Taken: %1 us").arg(timeVal));
        } else {
             timeTakenLabel_->setText(QString("Time Taken: %1 ms").arg(timeVal / 1000.0, 0, 'f', 2));
        }
    } else {
        statusLabel_->setText("Status: No Solution Found");
        pathLengthLabel_->setText("Path Length: --");
        nodesExploredLabel_->setText("Nodes Explored: --");
        timeTakenLabel_->setText("Time Taken: --");
    }
}