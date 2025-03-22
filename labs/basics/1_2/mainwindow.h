#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "flashcard.h"

class QTableWidget;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);

   private slots:
    void addFlashcard();
    void reviewFlashcards();
    void startQuiz();
    void deleteFlashcard();
    void onCellChanged(int row, int column);

   private:
    QList<Flashcard> flashcards;
    QTableWidget* flashcardTable;
    QPushButton* addButton;
    QPushButton* reviewButton;
    QPushButton* quizButton;
    QPushButton* deleteButton;

    void setupUI();
    void connectSignals();
    void updateFlashcardsFromTable();
};

#endif // MAINWINDOW_H
