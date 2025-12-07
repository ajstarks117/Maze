#include "MazeWidget.h"
#include <QPainter>
#include <QPaintEvent>

MazeWidget::MazeWidget(QWidget* parent)
    : QWidget(parent)
{
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

    MazeState mazeState = BackendInterface::getCurrentMazeState();
    if (mazeState.cells.empty()) return;

    // ======================================
    // 1. DYNAMIC CELL SIZE (important fix)
    // ======================================
    int maxW = width()  - 80; // padding
    int maxH = height() - 80;

    int cellW = maxW / mazeState.width;
    int cellH = maxH / mazeState.height;

    cellSize_ = std::max(5, std::min(cellW, cellH));   // avoid 0 or too small

    // Pixel dimensions
    int mazePixelW = mazeState.width * cellSize_;
    int mazePixelH = mazeState.height * cellSize_;

    // ======================================
    // 2. CENTER THE MAZE
    // ======================================
    int offsetX = (width()  - mazePixelW) / 2;
    int offsetY = (height() - mazePixelH) / 2;

    // ======================================
    // 3. DRAW BORDER AROUND MAZE
    // ======================================
    QRect borderRect(
        offsetX - 10,
        offsetY - 10,
        mazePixelW + 20,
        mazePixelH + 20
    );

    painter.setBrush(QColor("#2f3336"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(borderRect, 10, 10);

    // ======================================
    // 4. Translate painter INTO maze area
    // ======================================
    painter.translate(offsetX, offsetY);

    // ======================================
    // 5. Draw actual maze
    // ======================================
    drawGrid(painter, mazeState);
    drawCells(painter, mazeState);
}

void MazeWidget::drawGrid(QPainter& painter, const MazeState& mazeState) {
    painter.setPen(QPen(QColor(200, 200, 200, 80), 1));

    // Draw vertical grid lines
    for (int x = 0; x <= mazeState.width; ++x) {
        painter.drawLine(x * cellSize_, 0, x * cellSize_, mazeState.height * cellSize_);
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= mazeState.height; ++y) {
        painter.drawLine(0, y * cellSize_, mazeState.width * cellSize_, y * cellSize_);
    }
}

void MazeWidget::drawCells(QPainter& painter, const MazeState& mazeState) {
    // --- Draw walls & empty cells ---
    for (const auto& cell : mazeState.cells) {
        QColor color = cell.wall ? wallColor_ : emptyColor_;
        drawCell(painter, cell.x, cell.y, color);
    }

    // --- Draw visited cells ---
    for (const auto* cell : currentFrame_.visitedCells) {
        if (cell)
            drawCell(painter, cell->x, cell->y, visitedColor_);
    }

    // --- Draw current frontier ---
    for (const auto* cell : currentFrame_.currentCells) {
        if (cell)
            drawCell(painter, cell->x, cell->y, currentColor_);
    }

    // --- Draw final path ---
    for (const auto* cell : currentFrame_.pathCells) {
        if (cell)
            drawCell(painter, cell->x, cell->y, pathColor_);
    }

    // --- Draw start & end markers ---
    if (mazeState.startCell)
        drawCell(painter, mazeState.startCell->x, mazeState.startCell->y, startColor_);

    if (mazeState.endCell)
        drawCell(painter, mazeState.endCell->x, mazeState.endCell->y, endColor_);
}

void MazeWidget::drawCell(QPainter& painter, int x, int y, const QColor& color) {
    QRect cellRect(x * cellSize_, y * cellSize_, cellSize_, cellSize_);

    // Fill base color
    painter.fillRect(cellRect, color);

    // Subtle border for modern tile look
    painter.setPen(QPen(QColor(0, 0, 0, 25), 1));
    painter.drawRect(cellRect);
}
