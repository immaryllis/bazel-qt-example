#include "reviewwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

ReviewWindow::ReviewWindow(QList<Flashcard>& flashcards, QWidget* parent)
    : QDialog(parent), flashcards(flashcards), currentIndex(0), isShowingDefinition(false) {
    /*if (flashcards.isEmpty()) {
        QMessageBox::information(this, "No Flashcards", "There are no flashcards to review.");
        this->close();
        return;
    } */
    setupUI();
    showFlashcard();
}

void ReviewWindow::setupUI() {
    setWindowTitle("Review Flashcards");
    resize(400, 300);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    cardLabel = new QLabel(this);
    cardLabel->setAlignment(Qt::AlignCenter);

    checkBox = new QCheckBox("Learnt", this);

    flipButton = new QPushButton("Flip", this);
    QPushButton* nextButton = new QPushButton("Next", this);
    QPushButton* previousButton = new QPushButton("Previous", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(progressBar);
    layout->addWidget(cardLabel);
    layout->addWidget(checkBox);
    layout->addWidget(flipButton);
    layout->addWidget(nextButton);
    layout->addWidget(previousButton);

    connect(flipButton, &QPushButton::clicked, this, &ReviewWindow::flipCard);
    connect(nextButton, &QPushButton::clicked, this, &ReviewWindow::nextCard);
    connect(previousButton, &QPushButton::clicked, this, &ReviewWindow::previousCard);
    connect(checkBox, &QCheckBox::stateChanged, this, &ReviewWindow::onCheckBoxChanged);
}

void ReviewWindow::showFlashcard() {
    if (currentIndex < flashcards.size()) {
        checkBox->setChecked(flashcards[currentIndex].isLearnt());
        isShowingDefinition = false;
        cardLabel->setText(flashcards[currentIndex].getWord());
        updateProgressBar();
    }
}

void ReviewWindow::flipCard() {
    if (currentIndex < flashcards.size()) {
        if (isShowingDefinition) {
            cardLabel->setText(flashcards[currentIndex].getWord());
        } else {
            cardLabel->setText(flashcards[currentIndex].getDefinition());
        }
        isShowingDefinition = !isShowingDefinition;
    }
}

void ReviewWindow::nextCard() {
    if (currentIndex < flashcards.size() - 1) {
        currentIndex++;
    } else {
        currentIndex = 0;
    }
    showFlashcard();
}

void ReviewWindow::previousCard() {
    if (currentIndex > 0) {
        currentIndex--;
    } else {
        currentIndex = flashcards.size() - 1;
    }
    showFlashcard();
}

void ReviewWindow::onCheckBoxChanged() {
    if (currentIndex < flashcards.size()) {
        flashcards[currentIndex].setLearnt(checkBox->isChecked());
        updateProgressBar();
    }
}

void ReviewWindow::updateProgressBar() {
    int learntCount = 0;
    for (const Flashcard& flashcard : flashcards) {
        if (flashcard.isLearnt()) {
            learntCount++;
        }
    }
    double percentage = (static_cast<double>(learntCount) / flashcards.size()) * 100;
    progressBar->setValue(static_cast<int>(percentage));
}


