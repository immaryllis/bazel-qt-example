#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    QString styleSheet = R"(
        QWidget {
            background-color:  #d3f1df;
            color: #333333;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }

        QPushButton {
            background-color:  #e7befd;
            color: #333333;
            border: 1px solid  #b1f9ce;
            border-radius: 5px;
            padding: 10px 20px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #88a8b8;
        }

        QPushButton:pressed {
            background-color: #7898a8;
        }

        QLineEdit {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #b1f9ce;
            border-radius: 5px;
            padding: 5px;
            font-size: 14px;
        }

        QLabel {
            color: #333333;
            font-size: 16px;
            font-weight: bold;
        }

        QProgressBar {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid  #b1f9ce ;
            border-radius: 5px;
            text-align: center;
        }

        QProgressBar::chunk {
            background-color: #a8d8ea;
            border-radius: 5px;
        }

        QInputDialog {
            background-color:  #d3f1df;
            color: #333333;
            font-size: 14px;
        }

        QInputDialog QLabel {
            color: #333333;
            font-weight: bold;
        }

        QInputDialog QLineEdit {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid  #b1f9ce ;
            border-radius: 5px;
            padding: 2px;
        }

        QInputDialog QPushButton {
            background-color:  #e7befd ;
            color: #333333;
            border: 1px solid #b1f9ce;
            border-radius: 5px;
            padding: 2px 20px;
        }

        QInputDialog QPushButton:hover {
            background-color: #88a8b8;
        }

        QInputDialog QPushButton:pressed {
            background-color: #7898a8;
        }
    )";
    a.setStyleSheet(styleSheet);
    w.show();
    return a.exec();
}


