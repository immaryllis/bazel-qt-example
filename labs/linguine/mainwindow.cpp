#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), score(0), totalTasks(10), currentTask(0), elapsedSeconds(0)
{
    setupDatabase();
    setupMenuBar();
    setupMainLayout();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->start(1000);
    resize(750, 750);
}

MainWindow::~MainWindow()
{
    if (db.isOpen()) {
        db.close();
    }
}

void MainWindow::setupDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("linguine.db");

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "type TEXT, "
        "question TEXT, "
        "correct_answer TEXT)");
    query.exec("INSERT OR IGNORE INTO tasks (type, question, correct_answer) VALUES "
        "('translation', 'Hello, how are you?', 'Привет, как дела?'),"
        "('translation', 'Good morning!', 'Доброе утро!'),"
        "('grammar', 'Выберите правильный вариант:', 'I am going to the park.'),"
        "('grammar', 'Выберите правильный вариант:', 'She is reading a book.')");
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Меню", this);
    QAction *exitAction = new QAction("Выход", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);
    setMenuBar(menuBar);
}

void MainWindow::setupMainLayout()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createTranslationPage());
    stackedWidget->addWidget(createGrammarPage());
    QHBoxLayout *navLayout = new QHBoxLayout();
    QPushButton *translationBtn = new QPushButton("Перевод", this);
    QPushButton *grammarBtn = new QPushButton("Грамматика", this);
    connect(translationBtn, &QPushButton::clicked, this, &MainWindow::showTranslationExercise);
    connect(grammarBtn, &QPushButton::clicked, this, &MainWindow::showGrammarExercise);
    navLayout->addWidget(translationBtn);
    navLayout->addWidget(grammarBtn);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, totalTasks);
    progressBar->setValue(currentTask);
    scoreLabel = new QLabel("Оценка: 0/" + QString::number(totalTasks), this);
    timerLabel = new QLabel("Время: 00:00", this);
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(scoreLabel);
    infoLayout->addWidget(timerLabel);
    QPushButton *helpBtn = new QPushButton("Помощь", this);
    connect(helpBtn, &QPushButton::clicked, this, &MainWindow::showHelpDialog);
    mainLayout->addLayout(navLayout);
    mainLayout->addWidget(stackedWidget);
    mainLayout->addWidget(progressBar);
    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(helpBtn);

    setCentralWidget(centralWidget);
}

QWidget* MainWindow::createTranslationPage()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *questionLabel = new QLabel(getNextQuestion("translation"), this);
    QTextEdit *answerInput = new QTextEdit(this);
    answerInput->setObjectName("translationInput");
    QPushButton *submitBtn = new QPushButton("Отправить", this);
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::checkTranslationAnswer);

    layout->addWidget(questionLabel);
    layout->addWidget(answerInput);
    layout->addWidget(submitBtn);

    return page;
}

QWidget* MainWindow::createGrammarPage()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *questionLabel = new QLabel(getNextQuestion("grammar"), this);
    QRadioButton *option1 = new QRadioButton("I am going to the park.", this);
    QRadioButton *option2 = new QRadioButton("I going to the park.", this);
    QRadioButton *option3 = new QRadioButton("I go to the park am.", this);
    option1->setObjectName("correctOption");
    QPushButton *submitBtn = new QPushButton("Отправить", this);
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::checkGrammarAnswer);

    layout->addWidget(questionLabel);
    layout->addWidget(option1);
    layout->addWidget(option2);
    layout->addWidget(option3);
    layout->addWidget(submitBtn);

    return page;
}

QString MainWindow::getNextQuestion(const QString &type)
{
    QSqlQuery query(db);
    query.prepare("SELECT question FROM tasks WHERE type = :type LIMIT 1 OFFSET :offset");
    query.bindValue(":type", type);
    query.bindValue(":offset", currentTask);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "No more questions.";
}

QString MainWindow::getCorrectAnswer(const QString &type)
{
    QSqlQuery query(db);
    query.prepare("SELECT correct_answer FROM tasks WHERE type = :type LIMIT 1 OFFSET :offset");
    query.bindValue(":type", type);
    query.bindValue(":offset", currentTask);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

void MainWindow::showTranslationExercise()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showGrammarExercise()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::checkTranslationAnswer()
{
    QTextEdit *input = findChild<QTextEdit*>("translationInput");
    QString userAnswer = input->toPlainText().trimmed();
    QString correctAnswer = getCorrectAnswer("translation");

    if (userAnswer == correctAnswer) {
        score++;
        QMessageBox::information(this, "Результат", "Правильно!");
    } else {
        QMessageBox::warning(this, "Результат", "Неправильно. Правильный ответ: " + correctAnswer);
    }

    currentTask++;
    progressBar->setValue(currentTask);
    scoreLabel->setText("Оценка: " + QString::number(score) + "/" + QString::number(totalTasks));

    if (currentTask >= totalTasks) {
        QMessageBox::information(this, "Завершено", "Все задания выполнены! Оценка: " + QString::number(score));
        QApplication::quit();
    }
}

void MainWindow::checkGrammarAnswer()
{
    QRadioButton *correctOption = findChild<QRadioButton*>("correctOption");
    QString correctAnswer = getCorrectAnswer("grammar");
    if (correctOption->isChecked() && correctOption->text() == correctAnswer) {
        score++;
        QMessageBox::information(this, "Результат", "Правильно!");
    } else {
        QMessageBox::warning(this, "Результат", "Неправильно. Правильный ответ: " + correctAnswer);
    }

    currentTask++;
    progressBar->setValue(currentTask);
    scoreLabel->setText("Оценка: " + QString::number(score) + "/" + QString::number(totalTasks));

    if (currentTask >= totalTasks) {
        QMessageBox::information(this, "Завершено", "Все задания выполнены! Оценка: " + QString::number(score));
        QApplication::quit();
    }
}

void MainWindow::updateTimer()
{
    elapsedSeconds++;
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    timerLabel->setText(QString("Время: %1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::showHelpDialog()
{
    QDialog *helpDialog = new QDialog(this);
    helpDialog->setWindowTitle("Помощь");
    QVBoxLayout *layout = new QVBoxLayout(helpDialog);
    QLabel *helpText = new QLabel("Нажмите 'Перевод' или 'Грамматика' для выбора упражнения.\n"
        "Введите ответ и нажмите 'Отправить'.\n"
        "Следите за прогрессом и временем!", this);
    QPushButton *closeBtn = new QPushButton("Закрыть", this);
    connect(closeBtn, &QPushButton::clicked, helpDialog, &QDialog::close);
    layout->addWidget(helpText);
    layout->addWidget(closeBtn);
    helpDialog->exec();
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}
