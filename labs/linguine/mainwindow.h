#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void showTranslationExercise();
    void showGrammarExercise();
    void checkTranslationAnswer();
    void checkGrammarAnswer();
    void updateTimer();
    void showHelpDialog();
    void exitApplication();

   private:
    QStackedWidget *stackedWidget;
    QProgressBar *progressBar;
    QLabel *scoreLabel;
    QLabel *timerLabel;
    QTimer *timer;
    int score;
    int totalTasks;
    int currentTask;
    int elapsedSeconds;

    QSqlDatabase db;
    void setupMenuBar();
    void setupMainLayout();
    void setupDatabase();
    QWidget* createTranslationPage();
    QWidget* createGrammarPage();
    QString getNextQuestion(const QString &type);
    QString getCorrectAnswer(const QString &type);
};

#endif // MAINWINDOW_H
