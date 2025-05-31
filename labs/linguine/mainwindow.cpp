#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), score(0), totalTasks(10), currentTask(0), elapsedSeconds(0),
    wrongAttempts(0), taskTimeLimit(7 * 60), elapsedTaskTime(0), currentDifficulty("Easy")
{
    setupDatabases();
    setupMenuBar();
    setupMainLayout();

    globalTimer = new QTimer(this);
    connect(globalTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    taskTimer = new QTimer(this);
    connect(taskTimer, &QTimer::timeout, this, [=]() {
        elapsedTaskTime++;
        timerLabel->setText(QString("Время: %1:%2")
                                .arg(elapsedTaskTime / 60, 2, 10, QChar('0'))
                                .arg(elapsedTaskTime % 60, 2, 10, QChar('0')));
        if (elapsedTaskTime >= taskTimeLimit) {
            taskTimer->stop();
            QMessageBox::warning(this, "Время истекло", "Общее время превысило лимит 7 минут. Задание завершено.");
            QApplication::quit();
        }
    });
}

MainWindow::~MainWindow()
{
    if (dbTranslations.isOpen()) dbTranslations.close();
    if (dbGrammar.isOpen()) dbGrammar.close();
}

void MainWindow::setupDatabases()
{
    dbTranslations = QSqlDatabase::addDatabase("QSQLITE", "translationsConnection");
    dbTranslations.setDatabaseName("translations.db");
    if (!dbTranslations.open()) {
        qDebug() << "Failed to open translations database:" << dbTranslations.lastError().text();
        return;
    }

    dbGrammar = QSqlDatabase::addDatabase("QSQLITE", "grammarConnection");
    dbGrammar.setDatabaseName("grammar.db");
    if (!dbGrammar.open()) {
        qDebug() << "Failed to open grammar database:" << dbGrammar.lastError().text();
        return;
    }

            // Setup translations table
    QSqlQuery queryTranslations(dbTranslations);
    queryTranslations.exec("DROP TABLE IF EXISTS tasks");
    queryTranslations.exec("CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "difficulty TEXT, "
        "question TEXT, "
        "correct_answer TEXT)");

    queryTranslations.exec("INSERT INTO tasks (difficulty, question, correct_answer) VALUES "
        "('Easy', 'Hello, how are you?', 'Привет, как дела?'),"
        "('Easy', 'Good morning!', 'Доброе утро!'),"
        "('Easy', 'I am happy.', 'Я счастлив.'),"
        "('Medium', 'Where is the library?', 'Где библиотека?'),"
        "('Medium', 'I like to read books.', 'Мне нравится читать книги.'),"
        "('Medium', 'Can you help me?', 'Можете ли вы мне помочь?'),"
        "('Hard', 'The meeting is scheduled for tomorrow.', 'Встреча запланирована на завтра.'),"
        "('Hard', 'She has been studying Russian for two years.', 'Она изучает русский два года.'),"
        "('Hard', 'This is a challenging task.', 'Это сложное задание.'),"
        "('Hard', 'We will travel to Moscow next month.', 'Мы поедем в Москву в следующем месяце.')");

            // Setup grammar table
    QSqlQuery queryGrammar(dbGrammar);
    queryGrammar.exec("DROP TABLE IF EXISTS tasks");
    queryGrammar.exec("CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "question TEXT, "
        "options TEXT, "
        "correct_answer TEXT, "
        "hint TEXT)");

    queryGrammar.exec("INSERT INTO tasks (question, options, correct_answer, hint) VALUES "
        "('She ________ in Florida but prefers California.', "
        "'lives,goes,arrives', 'lives', 'Think about where someone currently resides.'),"
        "('When we ________ on vacation, we never fly.', "
        "'are,go,went', 'go', 'Consider the present tense action of taking a vacation.'),"
        "('I ________ four languages, but I love Italian above all.', "
        "'talk,speak,talks', 'speak', 'Use the verb commonly associated with language ability.'),"
        "('That dog always ________ with his head against the wall.', "
        "'goes,bark,sleeps', 'bark', 'Focus on the dog’s typical behavior.'),"
        "('We don''t ________ which airport the plane goes from.', "
        "'know,think,like', 'know', 'Choose the verb for understanding information.'),"
        "('My girlfriend ________ her eyes when there''s a horror movie on TV.', "
        "'open,shut,closes', 'shuts', 'Think about a natural reaction to fear.')");
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Меню", this);
    QAction *exitAction = new QAction("Выход", this);
    QAction *difficultyAction = new QAction("Уровень сложности", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(difficultyAction, &QAction::triggered, this, &MainWindow::showDifficultyDialog);
    fileMenu->addAction(difficultyAction);
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);
    setMenuBar(menuBar);
}

void MainWindow::setupMainLayout()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createInitialPage());

    QHBoxLayout *navLayout = new QHBoxLayout();
    QPushButton *translationBtn = new QPushButton("Перевод", this);
    QPushButton *grammarBtn = new QPushButton("Грамматика", this);
    connect(translationBtn, &QPushButton::clicked, this, &MainWindow::showUpdateTranslation);
    connect(grammarBtn, &QPushButton::clicked, this, &MainWindow::showUpdateGrammar);
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

QWidget* MainWindow::createInitialPage()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(page);
    QLabel *welcomeLabel = new QLabel("Выберите тип задания: Перевод или Грамматика", this);
    layout->addWidget(welcomeLabel);
    return page;
}

QWidget* MainWindow::createTranslationPage()
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(page);

    QString question = getNextQuestion("translation");
    QLabel *questionLabel = new QLabel(question, this);
    if (question == "No more questions.") {
        QMessageBox::warning(this, "Ошибка", "Нет доступных заданий для перевода на уровне " + currentDifficulty + ".");
        return page;
    }

    QTextEdit *answerInput = new QTextEdit(this);
    answerInput->setObjectName("translationInput");
    answerInput->installEventFilter(this);
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

    QButtonGroup *oldGroup = findChild<QButtonGroup*>("grammarOptions");
    if (oldGroup) {
        delete oldGroup;
    }

    QString question = getNextQuestion("grammar");
    QLabel *questionLabel = new QLabel(question, this);
    layout->addWidget(questionLabel);

    if (question == "No more questions.") {
        QMessageBox::warning(this, "Ошибка", "Нет доступных грамматических заданий.");
        return page;
    }

    QStringList options = getOptions("grammar");
    QString correctAnswer = getCorrectAnswer("grammar");

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setObjectName("grammarOptions");

    QString normalizedCorrectAnswer = correctAnswer.trimmed().toLower();
    normalizedCorrectAnswer.replace(QRegularExpression("[.!?]\\s*$"), "");

    for (const QString &option : options) {
        QString normalizedOption = option.trimmed().toLower();
        normalizedOption.replace(QRegularExpression("[.!?]\\s*$"), "");
        QRadioButton *radioBtn = new QRadioButton(option.trimmed(), this);
        radioBtn->setProperty("isCorrect", normalizedOption == normalizedCorrectAnswer);
        buttonGroup->addButton(radioBtn);
        layout->addWidget(radioBtn);
    }

    QPushButton *submitBtn = new QPushButton("Отправить", this);
    QPushButton *helpBtn = new QPushButton("Подсказка", this);
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::checkGrammarAnswer);
    connect(helpBtn, &QPushButton::clicked, this, &MainWindow::showGrammarHelp);

    layout->addWidget(submitBtn);
    layout->addWidget(helpBtn);

    return page;
}

int MainWindow::getTaskCount(const QString &type)
{
    QSqlDatabase db = (type == "translation") ? dbTranslations : dbGrammar;
    QSqlQuery query(db);
    QString queryStr = (type == "translation")
                           ? "SELECT COUNT(*) FROM tasks WHERE difficulty = :difficulty"
                           : "SELECT COUNT(*) FROM tasks";
    query.prepare(queryStr);
    if (type == "translation") query.bindValue(":difficulty", currentDifficulty);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

QString MainWindow::getNextQuestion(const QString &type)
{
    QSqlDatabase db = (type == "translation") ? dbTranslations : dbGrammar;
    int totalTasksForType = getTaskCount(type);
    if (totalTasksForType == 0) return "No more questions.";

    int taskIndex = currentTask % totalTasksForType;
    QSqlQuery query(db);
    QString queryStr = (type == "translation")
                           ? "SELECT question FROM tasks WHERE difficulty = :difficulty LIMIT 1 OFFSET :offset"
                           : "SELECT question FROM tasks LIMIT 1 OFFSET :offset";
    query.prepare(queryStr);
    if (type == "translation") query.bindValue(":difficulty", currentDifficulty);
    query.bindValue(":offset", taskIndex);
    if (query.exec() && query.next()) {
        return query.value(0).toString().trimmed();
    }
    return "No more questions.";
}

QString MainWindow::getCorrectAnswer(const QString &type)
{
    QSqlDatabase db = (type == "translation") ? dbTranslations : dbGrammar;
    int totalTasksForType = getTaskCount(type);
    if (totalTasksForType == 0) return "";

    int taskIndex = currentTask % totalTasksForType;
    QSqlQuery query(db);
    QString queryStr = (type == "translation")
                           ? "SELECT correct_answer FROM tasks WHERE difficulty = :difficulty LIMIT 1 OFFSET :offset"
                           : "SELECT correct_answer FROM tasks LIMIT 1 OFFSET :offset";
    query.prepare(queryStr);
    if (type == "translation") query.bindValue(":difficulty", currentDifficulty);
    query.bindValue(":offset", taskIndex);
    if (query.exec() && query.next()) {
        return query.value(0).toString().trimmed();
    }
    return "";
}

QStringList MainWindow::getOptions(const QString &type)
{
    QSqlDatabase db = (type == "translation") ? dbTranslations : dbGrammar;
    int totalTasksForType = getTaskCount(type);
    if (totalTasksForType == 0) return QStringList();

    int taskIndex = currentTask % totalTasksForType;
    QSqlQuery query(db);
    QString queryStr = (type == "translation")
                           ? "SELECT options FROM tasks WHERE difficulty = :difficulty LIMIT 1 OFFSET :offset"
                           : "SELECT options FROM tasks LIMIT 1 OFFSET :offset";
    query.prepare(queryStr);
    if (type == "translation") query.bindValue(":difficulty", currentDifficulty);
    query.bindValue(":offset", taskIndex);
    if (query.exec() && query.next()) {
        QString optionsStr = query.value(0).toString().trimmed();
        QStringList options = optionsStr.split(",");
        for (QString &option : options) {
            option = option.trimmed();
        }
        return options;
    }
    return QStringList();
}

QString MainWindow::getHint(const QString &type)
{
    QSqlDatabase db = (type == "translation") ? dbTranslations : dbGrammar;
    int totalTasksForType = getTaskCount(type);
    if (totalTasksForType == 0) return "";

    int taskIndex = currentTask % totalTasksForType;
    QSqlQuery query(db);
    QString queryStr = (type == "translation")
                           ? "SELECT hint FROM tasks WHERE difficulty = :difficulty LIMIT 1 OFFSET :offset"
                           : "SELECT hint FROM tasks LIMIT 1 OFFSET :offset";
    query.prepare(queryStr);
    if (type == "translation") query.bindValue(":difficulty", currentDifficulty);
    query.bindValue(":offset", taskIndex);
    if (query.exec() && query.next()) {
        return query.value(0).toString().trimmed();
    }
    return "";
}

void MainWindow::showTranslationExercise()
{
    while (stackedWidget->count() > 0) {
        QWidget *widget = stackedWidget->widget(0);
        stackedWidget->removeWidget(widget);
        delete widget;
    }
    stackedWidget->addWidget(createTranslationPage());
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showUpdateTranslation() {
    currentTask = 0;
    score = 0;
    wrongAttempts = 0;
    elapsedTaskTime = 0;
    timerLabel->setText(QString("Время: %1:%2")
                                    .arg(elapsedTaskTime / 60, 2, 10, QChar('0'))
                                    .arg(elapsedTaskTime % 60, 2, 10, QChar('0')));
    progressBar->setValue(0);
    scoreLabel->setText("Оценка: 0/" + QString::number(totalTasks));

    taskTimer->stop();
    taskTimer->start(1000);
    showTranslationExercise();
}

void MainWindow::showUpdateGrammar() {
    currentTask = 0;
    score = 0;
    wrongAttempts = 0;
    elapsedTaskTime = 0;
    timerLabel->setText(QString("Время: %1:%2")
                                    .arg(elapsedTaskTime / 60, 2, 10, QChar('0'))
                                    .arg(elapsedTaskTime % 60, 2, 10, QChar('0')));
    progressBar->setValue(0);
    scoreLabel->setText("Оценка: 0/" + QString::number(totalTasks));

    taskTimer->stop();
    taskTimer->start(1000);
    showGrammarExercise();
}

void MainWindow::showGrammarExercise()
{
    while (stackedWidget->count() > 0) {
        QWidget *widget = stackedWidget->widget(0);
        stackedWidget->removeWidget(widget);
        delete widget;
    }
    stackedWidget->addWidget(createGrammarPage());
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::checkTranslationAnswer()
{
    QTextEdit *input = findChild<QTextEdit*>("translationInput");
    QString userAnswer = input->toPlainText().trimmed().toLower();
    QString correctAnswer = getCorrectAnswer("translation").toLower();

    if (userAnswer == correctAnswer) {
        score++;
        currentTask++;
        progressBar->setValue(currentTask);
        scoreLabel->setText("Оценка: " + QString::number(score) + "/" + QString::number(totalTasks));
        QMessageBox::information(this, "Результат", "Правильно!");
        showTranslationExercise();
    } else {
        wrongAttempts++;
        if (wrongAttempts < 3) {
            QMessageBox::warning(this, "Ошибка", "Неправильно. У вас осталось " + QString::number(3 - wrongAttempts) + " попытки.");
        } else {
            resetTask();
            QMessageBox::warning(this, "Лимит исчерпан", "Превышен лимит 3 ошибок. Задание начинается заново.");
        }
    }

    progressBar->setValue(currentTask);
    scoreLabel->setText("Оценка: " + QString::number(score) + "/" + QString::number(totalTasks));

    if (currentTask >= totalTasks) {
        taskTimer->stop();
        QMessageBox::information(this, "Завершено", "Все задания выполнены! Оценка: " + QString::number(score));
        QApplication::quit();
    }
}

void MainWindow::checkGrammarAnswer()
{
    QButtonGroup *buttonGroup = findChild<QButtonGroup*>("grammarOptions");
    QAbstractButton *selectedButton = buttonGroup->checkedButton();

    if (!selectedButton) {
        QMessageBox::warning(this, "Ошибка", "Выберите один из вариантов!");
        return;
    }

    bool isCorrect = selectedButton->property("isCorrect").toBool();

    if (isCorrect) {
        score++;
        currentTask++;
        progressBar->setValue(currentTask);
        scoreLabel->setText("Оценка: " + QString::number(score) + "/" + QString::number(totalTasks));
        QMessageBox::information(this, "Результат", "Правильно!");
        showGrammarExercise();
    } else {
        wrongAttempts++;
        if (wrongAttempts < 3) {
            QMessageBox::warning(this, "Ошибка", "Неправильно. У вас осталось " + QString::number(3 - wrongAttempts) + " попытки.");
        } else {
            resetTask();
            QMessageBox::warning(this, "Лимит исчерпан", "Превышен лимит 3 ошибок. Задание начинается заново.");
        }
    }

    progressBar->setValue(currentTask);
    scoreLabel->setText("Оценка: " + QString::number(score) + "/" + QString::number(totalTasks));

    if (currentTask >= totalTasks) {
        taskTimer->stop();
        QMessageBox::information(this, "Завершено", "Все задания выполнены! Оценка: " + QString::number(score));
        QApplication::quit();
    }
}

void MainWindow::updateTimer()
{
    elapsedSeconds++;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            QTextEdit *textEdit = qobject_cast<QTextEdit*>(obj);
            if (textEdit && textEdit->objectName() == "translationInput") {
                checkTranslationAnswer();
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::showHelpDialog()
{
    QDialog *helpDialog = new QDialog(this);
    helpDialog->setWindowTitle("Помощь");
    QVBoxLayout *layout = new QVBoxLayout(helpDialog);
    QLabel *helpText = new QLabel("Нажмите 'Перевод' или 'Грамматика' для выбора упражнения.\n"
        "Введите ответ и нажмите 'Отправить' или Enter для перевода.\n"
        "Выберите вариант и нажмите 'Отправить' для грамматики.\n"
        "Нажмите 'Подсказка' для помощи в грамматических заданиях.\n"
        "Следите за прогрессом и временем!", this);
    QPushButton *closeBtn = new QPushButton("Закрыть", this);
    connect(closeBtn, &QPushButton::clicked, helpDialog, &QDialog::close);
    layout->addWidget(helpText);
    layout->addWidget(closeBtn);
    helpDialog->exec();
}

void MainWindow::showGrammarHelp()
{
    QString hint = getHint("grammar");
    if (!hint.isEmpty()) {
        QMessageBox::information(this, "Подсказка", hint);
    } else {
        QMessageBox::warning(this, "Ошибка", "Подсказка недоступна.");
    }
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}

void MainWindow::showDifficultyDialog()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Выберите уровень сложности");
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QComboBox *difficultyCombo = new QComboBox(this);
    difficultyCombo->addItems({"Easy", "Medium", "Hard"});
    difficultyCombo->setCurrentText(currentDifficulty);

    QPushButton *selectBtn = new QPushButton("Выбрать", this);
    connect(selectBtn, &QPushButton::clicked, [=]() {
        setDifficulty(difficultyCombo->currentIndex());
        dialog->close();
    });

    layout->addWidget(difficultyCombo);
    layout->addWidget(selectBtn);
    dialog->exec();
}

void MainWindow::setDifficulty(int index)
{
    QString difficulties[] = {"Easy", "Medium", "Hard"};
    currentDifficulty = difficulties[index];
    currentTask = 0;
    score = 0;
    wrongAttempts = 0;
    elapsedTaskTime = 0;
    taskTimer->stop();
    progressBar->setValue(0);
    scoreLabel->setText("Оценка: 0/" + QString::number(totalTasks));
    while (stackedWidget->count() > 0) {
        QWidget *widget = stackedWidget->widget(0);
        stackedWidget->removeWidget(widget);
        delete widget;
    }
    stackedWidget->addWidget(createInitialPage());
    stackedWidget->setCurrentIndex(0);
    taskTimer->start(1000);
}

void MainWindow::resetTask()
{
    wrongAttempts = 0;
    if (stackedWidget->currentIndex() == 0 && stackedWidget->count() > 0) {
        QWidget *currentWidget = stackedWidget->widget(0);
        if (dynamic_cast<QTextEdit*>(currentWidget->findChild<QTextEdit*>("translationInput"))) {
            showUpdateTranslation();
        } else {
            showUpdateGrammar();
        }
    }
}

void MainWindow::onTimerTimeout() {}
