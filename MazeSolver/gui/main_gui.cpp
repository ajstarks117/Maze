#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // Application info
    app.setApplicationName("Maze Solver Visualizer");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("MazeSolver");

    // ================================
    // GLOBAL QSS STYLING (Modern UI)
    // ================================
    QString qss = R"(
        QWidget {
            font-family: 'Segoe UI', Roboto, Arial;
            color: #333;
            background: #f4f5f7;
        }

        /* ---- Group Boxes (modern white cards) ---- */
        QGroupBox {
            background: #ffffff;
            border-radius: 12px;
            border: 1px solid #dcdcdc;
            margin-top: 12px;
            padding: 14px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 4px 8px;
            background: transparent;
            font-size: 14px;
            font-weight: 600;
        }

        /* ---- Buttons ---- */
        QPushButton {
            background-color: #3a5bfd;
            color: white;
            height: 38px;
            border-radius: 8px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #2e4bd6;
        }
        QPushButton:disabled {
            background-color: #a8b0c3;
        }

        /* Reset button (gray style) */
        QPushButton#resetButton {
            background: #e5e7eb;
            color: #333;
        }

        /* ---- Inputs (SpinBox, ComboBox) ---- */
        QSpinBox, QComboBox {
            background: #ffffff;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            height: 34px;
            padding-left: 6px;
            font-size: 14px;
        }

        /* ---- Slider ---- */
        QSlider::groove:horizontal {
            height: 6px;
            background: #e5e7eb;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #3a5bfd;
            width: 14px;
            height: 14px;
            border-radius: 7px;
            margin: -5px 0;
        }

        /* ---- Maze Container Card ---- */
        #mazeContainer {
            background: #ffffff;
            border-radius: 16px;
            border: 1px solid #e6e6e6;
            padding: 24px;
        }

        /* ---- Control Panel ---- */
        #controlPanel {
            background: transparent;
        }
    )";

    app.setStyleSheet(qss);

    // ================================
    // MAIN WINDOW
    // ================================
    MainWindow window;
    window.show();

    return app.exec();
}
