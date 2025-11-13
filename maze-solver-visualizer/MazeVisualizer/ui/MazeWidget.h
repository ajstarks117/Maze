#pragma once
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "../backend/core/Maze.h"

class MazeWidget : public QWidget {
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = nullptr)
        : QWidget(parent), maze(nullptr), currentStep(0) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MazeWidget::stepForward);
    }

    void setMaze(Maze* m) { maze = m; update(); }
    void setPath(const std::vector<Cell*>& p) { path = p; currentStep = 0; update(); }

    void startAnimation(int delayMs = 20) {
        currentStep = 0;
        timer->start(delayMs);
    }

    void stopAnimation() { timer->stop(); }

protected:
    void paintEvent(QPaintEvent*) override {
        if (!maze) return;

        QPainter painter(this);
        int cellWidth = width() / maze->width;
        int cellHeight = height() / maze->height;

        for (int y = 0; y < maze->height; ++y) {
            for (int x = 0; x < maze->width; ++x) {
                const Cell& c = maze->atConst(x, y);
                QRect rect(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
                if (c.wall)
                    painter.fillRect(rect, Qt::black);
                else
                    painter.fillRect(rect, Qt::white);
                painter.drawRect(rect);
            }
        }

        // Draw path gradually
        painter.setBrush(Qt::yellow);
        for (int i = 0; i < currentStep && i < (int)path.size(); ++i) {
            Cell* c = path[i];
            QRect rect(c->x * cellWidth, c->y * cellHeight, cellWidth, cellHeight);
            painter.fillRect(rect, QColor(255, 215, 0)); // gold
        }

        // Start and end
        painter.fillRect(QRect(maze->start.first * cellWidth, maze->start.second * cellHeight,
                               cellWidth, cellHeight), Qt::green);
        painter.fillRect(QRect(maze->end.first * cellWidth, maze->end.second * cellHeight,
                               cellWidth, cellHeight), Qt::red);
    }

private slots:
    void stepForward() {
        if (currentStep < (int)path.size()) {
            currentStep++;
            update();
        } else {
            timer->stop();
        }
    }

private:
    Maze* maze;
    std::vector<Cell*> path;
    int currentStep;
    QTimer* timer;
};
