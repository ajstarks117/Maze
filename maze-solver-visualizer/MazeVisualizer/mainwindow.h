#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runSelectedAlgorithm();
    void generateNewMaze();

private:
    void setupUI();
    void visualizeMaze();
    void visualizePath();

    // --- UI Elements ---
    QGraphicsView *mazeView;
    QGraphicsScene *mazeScene;

    QLabel *statsLabel;
    QComboBox *algoSelector;
    QPushButton *runButton;
    QPushButton *generateButton;

    // --- Helper ---
    void showAlgorithmResult(const QString &name, int pathLen, int nodes, double timeMs);
};

#endif // MAINWINDOW_H
