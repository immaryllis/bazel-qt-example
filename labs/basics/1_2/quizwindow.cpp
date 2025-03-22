#include "quizwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

QuizWindow::QuizWindow(const QList<Flashcard>& flashcards, QWidget* parent)
    : QDialog(parent), flashcards(flashcards), currentIndex(0) {

    setupUI();
    showNextQuestion();
}

void QuizWindow::setupUI() {
    setWindowTitle("Quiz Mode");
    resize(400, 200);

    questionLabel = new QLabel(this);
    answerInput = new QLineEdit(this);
    submitButton = new QPushButton("Submit", this);
    resultLabel = new QLabel(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(resultLabel);
    layout->addWidget(questionLabel);
    layout->addWidget(answerInput);
    layout->addWidget(submitButton);

    connect(submitButton, &QPushButton::clicked, this, &QuizWindow::checkAnswer);
}

void QuizWindow::showNextQuestion() {
    answerInput->clear();
    if (currentIndex < flashcards.size()) {
        resultLabel->setText("Your progress: " + QString::number(currentIndex) + "/" + QString::number(flashcards.size()));
        questionLabel->setText("What is the definition of: " + flashcards[currentIndex].getWord());
    }
}

void QuizWindow::checkAnswer() {
    QString userAnswer = answerInput->text();
    QString correctAnswer = flashcards[currentIndex].getDefinition();

    if (userAnswer == correctAnswer) {
        correctAnswers++;
        QMessageBox::information(this, "Correct", "Your answer is correct!");
    } else {
        QMessageBox::warning(this, "Incorrect", "The correct answer is: " + correctAnswer);
    }

    currentIndex = currentIndex + 1;
    showNextQuestion();
    if (currentIndex == flashcards.size()) {
        this->close();
        showResult();
    }
}

void QuizWindow::showResult() {
    QDialog resultDialog(this);
    resultDialog.setWindowTitle("Quiz Results");
    resultDialog.resize(400, 200);
    double percentage = (static_cast<double>(correctAnswers) / flashcards.size()) * 100;
    QLabel* resultLabel = new QLabel(&resultDialog);
    resultLabel->setText("Your score: " + QString::number(percentage) + "%");
    resultLabel->setAlignment(Qt::AlignCenter);
    QPushButton* okButton = new QPushButton("OK", &resultDialog);
    connect(okButton, &QPushButton::clicked, &resultDialog, &QDialog::accept);
    QVBoxLayout* layout = new QVBoxLayout(&resultDialog);
    layout->addWidget(resultLabel);
    layout->addWidget(okButton);
    resultDialog.setLayout(layout);

    resultDialog.exec();

            //this->close();
}
