#ifndef REVIEWWINDOW_H
#define REVIEWWINDOW_H

#include <QDialog>
#include <QList>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>
#include "flashcard.h"

class ReviewWindow : public QDialog {
    Q_OBJECT

   public:
    ReviewWindow(QList<Flashcard>& flashcards, QWidget* parent = nullptr);

   private slots:
    void flipCard();
    void nextCard();
    void previousCard();
    void onCheckBoxChanged();

   private:
    QList<Flashcard>& flashcards;
    int currentIndex;
    bool isShowingDefinition;
    QLabel* cardLabel;
    QCheckBox* checkBox;
    QPushButton* flipButton;
    QPushButton* nextButton;
    QPushButton* previousButton;
    QProgressBar* progressBar;

    void setupUI();
    void showFlashcard();
    void updateProgressBar();
};

#endif // REVIEWWINDOW_H
