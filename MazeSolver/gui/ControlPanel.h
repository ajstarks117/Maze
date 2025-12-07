#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

#include "BackendInterface.h"

/*
 * ControlPanel:
 * - All UI controls for generating mazes, selecting algorithms,
 *   changing speed, starting/stopping the solver, and showing results.
 * - This header is fully cleaned & ready for modern UI styling (QSS).
 */

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);

signals:
    void mazeGenerated();
    void solveStarted();
    void resetRequested();
    void stepRequested();

private slots:
    void onGenerateClicked();
    void onSolveClicked();
    void onResetClicked();
    void onStepClicked();
    void onSpeedChanged(int value);
    void updateResults(const AlgorithmResult& results);

private:
    // Setup helpers
    void setupMazeControls(QVBoxLayout* mainLayout);
    void setupAlgorithmControls(QVBoxLayout* mainLayout);
    void setupSimulationControls(QVBoxLayout* mainLayout);
    void setupResultsPanel(QVBoxLayout* mainLayout);

private:
    // ==========
    // Maze Setup
    // ==========
    QSpinBox* widthSpin_;
    QSpinBox* heightSpin_;
    QComboBox* generatorCombo_;
    QPushButton* generateButton_;

    // =================
    // Algorithm Options
    // =================
    QButtonGroup* algorithmGroup_;
    QRadioButton* dijkstraRadio_;
    QRadioButton* astarRadio_;
    QRadioButton* bidirectionalRadio_;
    QRadioButton* jpsRadio_;

    // ====================
    // Simulation Controls
    // ====================
    QPushButton* solveButton_;
    QPushButton* resetButton_;
    QPushButton* stepButton_;
    QSlider* speedSlider_;

    // ============
    // Results Panel
    // ============
    QLabel* statusLabel_;
    QLabel* pathLengthLabel_;
    QLabel* nodesExploredLabel_;
    QLabel* timeTakenLabel_;
};

