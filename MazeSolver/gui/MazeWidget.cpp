#include "MazeWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <algorithm>

MazeWidget::MazeWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(600, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

    // [FIX] Use Singleton to get data
    MazeState mazeState = BackendInterface::get().getCurrentMazeState();
    if (mazeState.cells.empty()) return;

    // Dimensions
    int maxW = width() - 80;
    int maxH = height() - 80;
    int cellW = maxW / mazeState.width;
    int cellH = maxH / mazeState.height;
    cellSize_ = std::max(1, std::min(cellW, cellH));

    int mazePixelW = mazeState.width * cellSize_;
    int mazePixelH = mazeState.height * cellSize_;
    int offsetX = (width() - mazePixelW) / 2;
    int offsetY = (height() - mazePixelH) / 2;

    // Draw Background
    QRect borderRect(offsetX - 10, offsetY - 10, mazePixelW + 20, mazePixelH + 20);
    painter.setBrush(QColor("#2f3336")); 
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(borderRect, 8, 8);

    painter.translate(offsetX, offsetY);

    // Draw Cells
    for (const auto& cell : mazeState.cells) {
        if (cell.wall) {
            drawCell(painter, cell.x, cell.y, wallColor_);
        } else {
            drawCell(painter, cell.x, cell.y, emptyColor_);
        }
    }

    // Draw Visited/Path
    for (const auto* cell : currentFrame_.visitedCells) { if(cell) drawCell(painter, cell->x, cell->y, visitedColor_); }
    for (const auto* cell : currentFrame_.currentCells) { if(cell) drawCell(painter, cell->x, cell->y, currentColor_); }
    for (const auto* cell : currentFrame_.pathCells)    { if(cell) drawCell(painter, cell->x, cell->y, pathColor_); }

    // Start/End
    if (mazeState.startCell) drawCell(painter, mazeState.startCell->x, mazeState.startCell->y, startColor_);
    if (mazeState.endCell)   drawCell(painter, mazeState.endCell->x, mazeState.endCell->y, endColor_);
}

void MazeWidget::drawCell(QPainter& painter, int x, int y, const QColor& color) {
    QRect cellRect(x * cellSize_, y * cellSize_, cellSize_, cellSize_);
    painter.fillRect(cellRect, color);
}