#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "controller.h"
#include "widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    MainWindow();
    ~MainWindow();

   private slots:
    void modeChanged(int index);

   private:

    QComboBox *modeComboBox_;
    Widget* widget_;
};
#endif // MAINWINDOW_H
