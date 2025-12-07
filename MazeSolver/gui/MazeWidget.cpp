#include "MazeWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <algorithm> // For std::max, std::min

MazeWidget::MazeWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(600, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Initial generation (will be overwritten by ControlPanel signals)
    BackendInterface::generateMaze(21, 21, MazeGenerator::RecursiveBacktracker);
}

void MazeWidget::updateAnimationFrame(const AnimationFrame& frame) {
    currentFrame_ = frame;
    update(); // Triggers paintEvent
}

void MazeWidget::resetView() {
    currentFrame_ = AnimationFrame();
    update();
}

void MazeWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    MazeState mazeState = BackendInterface::getCurrentMazeState();
    if (mazeState.cells.empty()) return;

    // ======================================
    // 1. DYNAMIC CELL SIZE
    // ======================================
    // Leave some padding (40px on each side)
    int maxW = width() - 80; 
    int maxH = height() - 80;

    // Calculate max possible cell size that fits
    int cellW = maxW / mazeState.width;
    int cellH = maxH / mazeState.height;

    // Ensure cell size is at least 1 pixel
    cellSize_ = std::max(1, std::min(cellW, cellH));

    // Calculate actual pixel dimensions of the maze
    int mazePixelW = mazeState.width * cellSize_;
    int mazePixelH = mazeState.height * cellSize_;

    // ======================================
    // 2. CENTER THE MAZE
    // ======================================
    int offsetX = (width() - mazePixelW) / 2;
    int offsetY = (height() - mazePixelH) / 2;

    // ======================================
    // 3. DRAW BORDER CARD
    // ======================================
    // Draw a background card/border slightly larger than the maze
    QRect borderRect(
        offsetX - 10,
        offsetY - 10,
        mazePixelW + 20,
        mazePixelH + 20
    );

    painter.setBrush(QColor("#2f3336")); // Dark background for contrast
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(borderRect, 8, 8);

    // ======================================
    // 4. TRANSFORM PAINTER
    // ======================================
    // Shift coordinate system so (0,0) is the top-left of the maze
    painter.translate(offsetX, offsetY);

    // ======================================
    // 5. DRAW CONTENT
    // ======================================
    drawGrid(painter, mazeState);
    drawCells(painter, mazeState);
}

void MazeWidget::drawGrid(QPainter& painter, const MazeState& mazeState) {
    // Faint grid lines
    painter.setPen(QPen(QColor(200, 200, 200, 30), 1));

    // Vertical lines
    for (int x = 0; x <= mazeState.width; ++x) {
        painter.drawLine(x * cellSize_, 0, x * cellSize_, mazeState.height * cellSize_);
    }

    // Horizontal lines
    for (int y = 0; y <= mazeState.height; ++y) {
        painter.drawLine(0, y * cellSize_, mazeState.width * cellSize_, y * cellSize_);
    }
}

void MazeWidget::drawCells(QPainter& painter, const MazeState& mazeState) {
    // 1. Draw Static Layer (Walls and Empty Cells)
    for (const auto& cell : mazeState.cells) {
        if (cell.wall) {
            drawCell(painter, cell.x, cell.y, wallColor_);
        } else {
            drawCell(painter, cell.x, cell.y, emptyColor_);
        }
    }

    // 2. Draw Visited Cells (Animation History)
    for (const auto* cell : currentFrame_.visitedCells) {
        if (cell) drawCell(painter, cell->x, cell->y, visitedColor_);
    }

    // 3. Draw Frontier/Current Cells (Active Animation)
    for (const auto* cell : currentFrame_.currentCells) {
        if (cell) drawCell(painter, cell->x, cell->y, currentColor_);
    }

    // 4. Draw Final Path (Result)
    for (const auto* cell : currentFrame_.pathCells) {
        if (cell) drawCell(painter, cell->x, cell->y, pathColor_);
    }

    // 5. Draw Start & End Positions (Overlay)
    if (mazeState.startCell) {
        drawCell(painter, mazeState.startCell->x, mazeState.startCell->y, startColor_);
        
        // Optional: Draw 'S' text
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", cellSize_ / 1.5, QFont::Bold));
        painter.drawText(
            mazeState.startCell->x * cellSize_, 
            mazeState.startCell->y * cellSize_, 
            cellSize_, cellSize_, 
            Qt::AlignCenter, "S"
        );
    }

    if (mazeState.endCell) {
        drawCell(painter, mazeState.endCell->x, mazeState.endCell->y, endColor_);
        
        // Optional: Draw 'E' text
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", cellSize_ / 1.5, QFont::Bold));
        painter.drawText(
            mazeState.endCell->x * cellSize_, 
            mazeState.endCell->y * cellSize_, 
            cellSize_, cellSize_, 
            Qt::AlignCenter, "E"
        );
    }
}

void MazeWidget::drawCell(QPainter& painter, int x, int y, const QColor& color) {
    QRect cellRect(x * cellSize_, y * cellSize_, cellSize_, cellSize_);

    // Fill the cell
    painter.fillRect(cellRect, color);

    // Subtle border for definition
    painter.setPen(QPen(QColor(0, 0, 0, 20), 1));
    painter.drawRect(cellRect);
}