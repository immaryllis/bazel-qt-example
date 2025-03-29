#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    const QString styleSheet = R"(
QWidget {
    background-color: #f5f7fa;
    color: #333333;
    font-family: "Segoe UI", Arial, sans-serif;
    font-size: 12px;
    selection-background-color: #a8c5e0;
    selection-color: #333333;
}

QMainWindow, QDialog {
    background-color: #e6e9ef;
    border: 1px solid #d1d9e6;
}

QPushButton {
    background-color: #d1d9e6;
    border: 1px solid #c0c8d5;
    border-radius: 4px;
    padding: 5px 10px;
    min-width: 80px;
}

QPushButton:hover {
    background-color: #c0c8d5;
}

QPushButton:pressed {
    background-color: #a8b4c8;
}

QPushButton:disabled {
    background-color: #e0e4eb;
    color: #999999;
}

QLineEdit {
    background-color: white;
    border: 1px solid #c0c8d5;
    border-radius: 3px;
    padding: 5px;
    selection-background-color: #a8c5e0;
}

QLineEdit:focus {
    border: 1px solid #7a9cc6;
}

QComboBox {
    background-color: white;
    border: 1px solid #c0c8d5;
    border-radius: 3px;
    padding: 5px;
    min-width: 6em;
}

QComboBox:editable {
    background: white;
}

QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 20px;
    border-left-width: 1px;
    border-left-color: #c0c8d5;
    border-left-style: solid;
    border-top-right-radius: 3px;
    border-bottom-right-radius: 3px;
}

QScrollBar:vertical {
    border: none;
    background: #e6e9ef;
    width: 10px;
}

QScrollBar::handle:vertical {
    background: #c0c8d5;
    min-height: 20px;
    border-radius: 4px;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QProgressBar {
    border: 1px solid #c0c8d5;
    border-radius: 3px;
    text-align: center;
    background-color: white;
}

QProgressBar::chunk {
    background-color: #a8c5e0;
    width: 10px;
}


QGroupBox {
    border: 1px solid #d1d9e6;
    border-radius: 4px;
    margin-top: 10px;
    padding-top: 15px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    left: 10px;
    padding: 0 3px;
}
})";
    a.setStyleSheet(styleSheet);
    w.show();
    return a.exec();
}
