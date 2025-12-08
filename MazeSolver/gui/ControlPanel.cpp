#include "ControlPanel.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("controlPanel");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(18);

    setupMazeControls(mainLayout);
    setupAlgorithmControls(mainLayout);
    setupSimulationControls(mainLayout);
    setupResultsPanel(mainLayout);

    mainLayout->addStretch(); 

    // Backend → Results UI updates
    BackendInterface::get().onSolveComplete = [this](const AlgorithmResult& results) {
        updateResults(results);
    };
}

void ControlPanel::setupMazeControls(QVBoxLayout* mainLayout)
{
    auto* group = new QGroupBox("Maze Setup");
    auto* layout = new QVBoxLayout(group);
    layout->setSpacing(10);

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

    connect(generateButton_, &QPushButton::clicked,
            this, &ControlPanel::onGenerateClicked);

    mainLayout->addWidget(group);
}

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

    astarRadio_->setChecked(true);

    layout->addWidget(dijkstraRadio_);
    layout->addWidget(astarRadio_);
    layout->addWidget(bidirectionalRadio_);
    layout->addWidget(jpsRadio_);

    mainLayout->addWidget(group);
}

void ControlPanel::setupSimulationControls(QVBoxLayout* mainLayout)
{
    auto* group = new QGroupBox("Simulation Controls");
    auto* layout = new QVBoxLayout(group);
    layout->setSpacing(10);

    solveButton_ = new QPushButton("Start Solve");
    resetButton_ = new QPushButton("Reset");
    resetButton_->setObjectName("resetButton"); 
    stepButton_ = new QPushButton("Step Forward");

    auto* buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(10);
    buttonRow->addWidget(solveButton_);
    buttonRow->addWidget(resetButton_);

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

void ControlPanel::onGenerateClicked() {
    int width = widthSpin_->value();
    int height = heightSpin_->value();
    MazeGenerator gen = static_cast<MazeGenerator>(generatorCombo_->currentIndex());
    BackendInterface::get().generateMaze(width, height, gen);
    emit mazeGenerated();
    statusLabel_->setText("Status: Maze Generated");
}

void ControlPanel::onSolveClicked() {
    PathfindingAlgorithm algorithm = static_cast<PathfindingAlgorithm>(algorithmGroup_->checkedId());
    int speed = speedSlider_->value();
    BackendInterface::get().startSolve(algorithm, speed);
    emit solveStarted();
    statusLabel_->setText("Status: Solving...");
}

void ControlPanel::onStepClicked() {
    BackendInterface::get().step();
    emit stepRequested();
}

void ControlPanel::onSpeedChanged(int value) {
    Q_UNUSED(value);
}

void ControlPanel::onResetClicked() {
    BackendInterface::get().reset();
    emit resetRequested();
    statusLabel_->setText("Status: Reset");
    updateResults(AlgorithmResult());
    emit explanationUpdated(""); 
}

// [UPDATED] Adds "Learn More" links to the HTML
void ControlPanel::updateResults(const AlgorithmResult& results)
{
    if (results.success) {
        statusLabel_->setText("Status: Solved");
        pathLengthLabel_->setText(QString("Path Length: %1").arg(results.metrics.pathLength));
        nodesExploredLabel_->setText(QString("Nodes Explored: %1").arg(results.metrics.nodesExplored));
        
        double timeVal = results.metrics.timeTakenMs;
        if (timeVal < 1000) {
             timeTakenLabel_->setText(QString("Time Taken: %1 us").arg(timeVal));
        } else {
             timeTakenLabel_->setText(QString("Time Taken: %1 ms").arg(timeVal / 1000.0, 0, 'f', 2));
        }

        int selectedId = algorithmGroup_->checkedId();
        QString explanation;

        // Common style for the link
        QString linkStyle = "color: #007bff; text-decoration: none; font-weight: bold;";

        switch (selectedId) {
            case 0: // Dijkstra
                explanation = 
                    "<h3 style='color:#2c3e50; margin-top:0;'>Dijkstra's Algorithm</h3>"
                    "<p><b>Mechanism:</b> Dijkstra explores nodes in concentric circles, expanding equally in all directions. "
                    "It guarantees the shortest path by always selecting the unvisited node with the smallest known distance.</p>"
                    "<p><b>Cons:</b> Wasteful in grid-based maps because it acts like a 'flood fill'.</p>"
                    "<p>🔗 <a style='" + linkStyle + "' href='https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/'>Read more on GeeksforGeeks</a></p>";
                break;
            case 1: // A*
                explanation = 
                    "<h3 style='color:#2c3e50; margin-top:0;'>A* Search Algorithm</h3>"
                    "<p><b>Mechanism:</b> A* improves on Dijkstra by using a <i>heuristic</i>. "
                    "It selects paths based on <b>f(n) = g(n) + h(n)</b>, where h(n) is the estimated distance to the goal.</p>"
                    "<p><b>Result:</b> This makes the search 'greedy', pulling the frontier towards the goal.</p>"
                    "<p>🔗 <a style='" + linkStyle + "' href='https://www.geeksforgeeks.org/a-search-algorithm/'>Read more on GeeksforGeeks</a></p>";
                break;
            case 2: // Bidirectional
                explanation = 
                    "<h3 style='color:#2c3e50; margin-top:0;'>Bidirectional A*</h3>"
                    "<p><b>Mechanism:</b> Two searches run simultaneously: one forward from Start and one backward from Goal. "
                    "They meet in the middle.</p>"
                    "<p><b>Why:</b> Searching two small circles (πr²/2) is faster than searching one large circle (πr²).</p>"
                    "<p>🔗 <a style='" + linkStyle + "' href='https://www.geeksforgeeks.org/bidirectional-search/'>Read more on GeeksforGeeks</a></p>";
                break;
            case 3: // JPS
                explanation = 
                    "<h3 style='color:#2c3e50; margin-top:0;'>Jump Point Search (JPS)</h3>"
                    "<p><b>Mechanism:</b> JPS is optimized for grids. It 'jumps' over empty spaces along straight lines "
                    "and only stops at interesting intersections (jump points).</p>"
                    "<p><b>Benefit:</b> It skips redundant checks, making it incredibly fast for open maps.</p>"
                    "<p>🔗 <a style='" + linkStyle + "' href='https://zerowidth.com/2013/a-visual-explanation-of-jump-point-search/'>Read more on Web</a></p>";
                break;
            default:
                explanation = "";
                break;
        }
        
        emit explanationUpdated(explanation);

    } else {
        statusLabel_->setText("Status: No Solution Found");
        pathLengthLabel_->setText("Path Length: --");
        nodesExploredLabel_->setText("Nodes Explored: --");
        timeTakenLabel_->setText("Time Taken: --");
        
        emit explanationUpdated("<p style='color:red;'><b>No Path Found:</b> The goal is unreachable.</p>");
    }
}