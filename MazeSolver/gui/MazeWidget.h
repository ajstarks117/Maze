#pragma once

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "BackendInterface.h"

/*
 * MazeWidget:
 * - Renders the maze grid
 * - Renders walls, empty tiles, visited, current, and path cells
 * - Centers the maze inside a styled container (defined in MainWindow/QSS)
 * - Responds to BackendInterface animation updates
 */

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    explicit MazeWidget(QWidget* parent = nullptr);

    void setCellSize(int size) {
        cellSize_ = size;
        update();
    }

public slots:
    void updateAnimationFrame(const AnimationFrame& frame);
    void resetView();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawGrid(QPainter& painter, const MazeState& mazeState);
    void drawCells(QPainter& painter, const MazeState& mazeState);
    void drawCell(QPainter& painter, int x, int y, const QColor& color);

private:
    int cellSize_ = 25;

    AnimationFrame currentFrame_;

    // ==========================
    // Modern Color Palette
    // ==========================
    QColor wallColor_     = QColor("#2f3336");   // Dark gray / black walls
    QColor emptyColor_    = QColor("#ffffff");   // Clean white path
    QColor startColor_    = QColor("#1dd75f");   // Bright green
    QColor endColor_      = QColor("#eb4034");   // Bright red
    QColor visitedColor_  = QColor("#70d6ff");   // Light blue (visited)
    QColor currentColor_  = QColor("#0094ff");   // Live search frontier
    QColor pathColor_     = QColor("#ffcd66");   // Yellow final path
};

