#ifndef QUIZWINDOW_H
#define QUIZWINDOW_H

#include <QDialog>
#include <QList>
#include "flashcard.h"

class QLabel;
class QLineEdit;
class QPushButton;

class QuizWindow : public QDialog {
    Q_OBJECT

   public:
    QuizWindow(const QList<Flashcard>& flashcards, QWidget* parent = nullptr);
    int correctAnswers = 0;

   private slots:
    void checkAnswer();

   private:
    QList<Flashcard> flashcards;
    int currentIndex;
    QLabel* questionLabel;
    QLabel* resultLabel;
    QLineEdit* answerInput;
    QPushButton* submitButton;
    void setupUI();
    void showNextQuestion();
    void showResult();

};

#endif // QUIZWINDOW_H
