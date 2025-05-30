#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Linguine");
    QString styleSheet = R"(
        QWidget {
            background-color:   #d8dbfd;
            color: #333333;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }

        QPushButton {
            background-color:   #babeef;
            color: #333333;
            border: 1px solid   #a2aafd;
            border-radius: 5px;
            padding: 10px 20px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color:  #b69ffe;
        }

        QPushButton:pressed {
            background-color:  #a083fc;
        }

        QLineEdit {
            background-color:  #e7e9fd;
            color: #333333;
            border: 1px solid  #a2aafd;
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
            background-color:  #e7e9fd;
            color: #333333;
            border: 1px solid   #a2aafd;
            border-radius: 5px;
            text-align: center;
        }

        QProgressBar::chunk {
            background-color: #a8d8ea;
            border-radius: 5px;
        }

        QInputDialog {
            background-color:   #d8dbfd;
            color: #333333;
            font-size: 14px;
        }

        QInputDialog QLabel {
            color: #333333;
            font-weight: bold;
        }

        QInputDialog QLineEdit {
            background-color:  #e7e9fd;
            color: #333333;
            border: 1px solid  #a2aafd;
            border-radius: 5px;
            padding: 2px;
        }

        QInputDialog QPushButton {
            background-color:   #babeef;
            color: #333333;
            border: 1px solid  #a2aafd;
            border-radius: 5px;
            padding: 2px 20px;
        }

        QInputDialog QPushButton:hover {
            background-color:  #b69ffe;
        }

        QInputDialog QPushButton:pressed {
            background-color: #a083fc;
        }
    )";
    a.setStyleSheet(styleSheet);
    w.show();
    return a.exec();
}
