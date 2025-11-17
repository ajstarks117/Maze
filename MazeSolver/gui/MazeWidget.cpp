#include "MazeWidget.h"
#include <QPainter>
#include <QTimer>

MazeWidget::MazeWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(600, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Generate a default maze for testing
    BackendInterface::generateMaze(21, 21, MazeGenerator::RecursiveBacktracker);
}

void MazeWidget::updateAnimationFrame(const AnimationFrame& frame) {
    currentFrame_ = frame;
    update();
}

void MazeWidget::resetView() {
    currentFrame_ = AnimationFrame();
    update();
}

void MazeWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    drawGrid(painter);
    drawCells(painter);
}

void MazeWidget::drawGrid(QPainter& painter) {
    auto mazeState = BackendInterface::getCurrentMazeState();
    if (mazeState.cells.empty()) return;
    
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    
    int width = mazeState.width;
    int height = mazeState.height;
    
    for (int x = 0; x <= width; ++x) {
        painter.drawLine(x * cellSize_, 0, x * cellSize_, height * cellSize_);
    }
    for (int y = 0; y <= height; ++y) {
        painter.drawLine(0, y * cellSize_, width * cellSize_, y * cellSize_);
    }
}

void MazeWidget::drawCells(QPainter& painter) {
    auto mazeState = BackendInterface::getCurrentMazeState();
    if (mazeState.cells.empty()) return;
    
    // Draw walls and empty cells
    for (const auto& cell : mazeState.cells) {
        QColor color = cell.wall ? wallColor_ : emptyColor_;
        drawCell(painter, cell.x, cell.y, color);
    }
    
    // Draw visited cells
    for (const auto& cellPtr : currentFrame_.visitedCells) {
        if (cellPtr) {
            drawCell(painter, cellPtr->x, cellPtr->y, visitedColor_);
        }
    }
    
    // Draw current exploring cells
    for (const auto& cellPtr : currentFrame_.currentCells) {
        if (cellPtr) {
            drawCell(painter, cellPtr->x, cellPtr->y, currentColor_);
        }
    }
    
    // Draw path
    for (const auto& cellPtr : currentFrame_.pathCells) {
        if (cellPtr) {
            drawCell(painter, cellPtr->x, cellPtr->y, pathColor_);
        }
    }
    
    // Draw start and end (always on top)
    if (mazeState.startCell) {
        drawCell(painter, mazeState.startCell->x, mazeState.startCell->y, startColor_);
    }
    if (mazeState.endCell) {
        drawCell(painter, mazeState.endCell->x, mazeState.endCell->y, endColor_);
    }
}

void MazeWidget::drawCell(QPainter& painter, int x, int y, const QColor& color) {
    QRect cellRect(x * cellSize_, y * cellSize_, cellSize_, cellSize_);
    painter.fillRect(cellRect, color);
    
    // Add border for better visibility
    painter.setPen(QPen(QColor(100, 100, 100), 1));
    painter.drawRect(cellRect);
}