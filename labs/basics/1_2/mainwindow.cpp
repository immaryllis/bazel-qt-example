#include "mainwindow.h"
#include "reviewwindow.h"
#include "quizwindow.h"
#include <QTableWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    connectSignals();
}

void MainWindow::setupUI() {
    setWindowTitle("Language Learning App");
    resize(620, 400);

    flashcardTable = new QTableWidget(this);
    flashcardTable->setColumnCount(2);
    flashcardTable->setHorizontalHeaderLabels({"Word", "Definition"});
    flashcardTable->horizontalHeader()->setStretchLastSection(true);
    flashcardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    flashcardTable->setSelectionBehavior(QTableWidget::SelectRows);
    flashcardTable->setEditTriggers(QTableWidget::DoubleClicked);

    setCentralWidget(flashcardTable);

    addButton = new QPushButton("Add Flashcard", this);
    reviewButton = new QPushButton("Review Flashcards", this);
    quizButton = new QPushButton("Start Quiz", this);
    deleteButton = new QPushButton("Delete Flashcard", this);

    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->addWidget(addButton);
    toolbar->addWidget(reviewButton);
    toolbar->addWidget(quizButton);
    toolbar->addWidget(deleteButton);
}

void MainWindow::connectSignals() {
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addFlashcard);
    connect(reviewButton, &QPushButton::clicked, this, &MainWindow::reviewFlashcards);
    connect(quizButton, &QPushButton::clicked, this, &MainWindow::startQuiz);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteFlashcard);
    connect(flashcardTable, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);
}

void MainWindow::addFlashcard() {
    QInputDialog dialogAddWord;
    dialogAddWord.setWindowTitle("Add Flashcard");
    dialogAddWord.setLabelText("Enter word:");
    dialogAddWord.setFixedSize(400, 400);
    QString word;
    if (dialogAddWord.exec() == QDialog::Accepted) {
        word = dialogAddWord.textValue().trimmed();
        QInputDialog dialogAddDef;
        dialogAddDef.setWindowTitle("Add Flashcard");
        dialogAddDef.setLabelText("Enter definition:");
        dialogAddDef.resize(400, 400);
        QString definition;
        if (dialogAddDef.exec() == QDialog::Accepted) {
            definition = dialogAddDef.textValue().trimmed();
            flashcards.append(Flashcard(word, definition, false));
            int row = flashcardTable->rowCount();
            flashcardTable->insertRow(row);
            flashcardTable->setItem(row, 0, new QTableWidgetItem(word));
            flashcardTable->setItem(row, 1, new QTableWidgetItem(definition));
        }
    }
}

void MainWindow::reviewFlashcards() {
    ReviewWindow* reviewWindow = new ReviewWindow(flashcards, this);
    reviewWindow->show();
}

void MainWindow::startQuiz() {
    if (flashcards.isEmpty()) {
        QMessageBox::information(this, "No Flashcards", "Add flashcards to take a quiz.");
        return;
    }
    QuizWindow* quizWindow = new QuizWindow(flashcards, this);
    quizWindow->show();
}

void MainWindow::deleteFlashcard() {
    int selectedRow = flashcardTable->currentRow();
    if (selectedRow >= 0 && selectedRow < flashcards.size()) {
        flashcards.removeAt(selectedRow);
        flashcardTable->removeRow(selectedRow);
    } else {
        QMessageBox::warning(this, "No Selection", "Please select a flashcard to delete.");
    }
}


void MainWindow::onCellChanged(int row, int column) {
    if (row >= 0 && row < flashcards.size()) {
        QString newValue = flashcardTable->item(row, column)->text().trimmed();
        if (column == 0) {
            flashcards[row].setWord(newValue);
        } else if (column == 1) {
            flashcards[row].setDefinition(newValue);
        }
        flashcardTable->item(row, column)->setText(newValue);
    }
}
