#pragma once
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "BackendInterface.h"

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    explicit MazeWidget(QWidget* parent = nullptr);
    void setCellSize(int size) { cellSize_ = size; update(); }
    
public slots:
    void updateAnimationFrame(const AnimationFrame& frame);
    void resetView();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawGrid(QPainter& painter);
    void drawCells(QPainter& painter);
    void drawCell(QPainter& painter, int x, int y, const QColor& color);
    
    int cellSize_ = 25;
    AnimationFrame currentFrame_;
    
    // Colors
    QColor wallColor_ = QColor(45, 45, 45);
    QColor emptyColor_ = QColor(255, 255, 255);
    QColor startColor_ = QColor(76, 175, 80);    // Green
    QColor endColor_ = QColor(244, 67, 54);      // Red
    QColor visitedColor_ = QColor(187, 222, 251); // Light blue
    QColor pathColor_ = QColor(255, 193, 7);     // Amber
    QColor currentColor_ = QColor(33, 150, 243); // Blue
};