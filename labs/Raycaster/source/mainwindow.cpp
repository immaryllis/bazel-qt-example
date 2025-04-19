#include "../headers/mainwindow.h"
#include "../headers/controller.h"
#include "../headers/widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QDebug>

MainWindow::MainWindow(): widget_(new Widget()), modeComboBox_(new QComboBox()) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *modeLayout = new QHBoxLayout();
    modeLayout->addWidget(new QLabel("Mode:"));

    modeComboBox_ = new QComboBox();

    modeComboBox_->addItem("Light");
    modeComboBox_->addItem("Polygons");
    modeComboBox_->addItem("Static lights");
    //modeComboBox_->setCurrentIndex(Controller::Mode::Light);
    connect(modeComboBox_, &QComboBox::currentIndexChanged, this, &MainWindow::modeChanged);

    modeLayout->addWidget(modeComboBox_);
    modeLayout->addStretch();
    mainLayout->addLayout(modeLayout);
    mainLayout->addWidget(widget_);

    setCentralWidget(centralWidget);
    centralWidget->setLayout(mainLayout);
    setWindowTitle("Raycaster");
    resize(1024, 960);
    show();
    /*
    qDebug() << "MainWindow constructing";
    connect(this, &MainWindow::destroyed, [](){ qDebug() << "MainWindow destroyed"; });*/
}


void MainWindow::modeChanged(int mode) {
    widget_->setMode(static_cast<Controller::Mode>(mode));
}

MainWindow::~MainWindow()
{
}


