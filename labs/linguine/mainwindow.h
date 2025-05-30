#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTimer>
#include <QStackedWidget>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QRadioButton>
#include <QSqlQuery>

class QButtonGroup;
class QDialog;
class QComboBox;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    QSqlDatabase dbTranslations;
    QSqlDatabase dbGrammar;
    QTimer *globalTimer;
    QTimer *taskTimer;
    QStackedWidget *stackedWidget;
    QProgressBar *progressBar;
    QLabel *scoreLabel;
    QLabel *timerLabel;
    int score;
    int totalTasks;
    int currentTask;
    int elapsedSeconds;
    int wrongAttempts;
    int taskTimeLimit;
    int elapsedTaskTime;
    QString currentDifficulty;

    void setupDatabases();
    void setupMenuBar();
    void setupMainLayout();
    QWidget* createInitialPage();
    QWidget* createTranslationPage();
    QWidget* createGrammarPage();
    int getTaskCount(const QString &type);
    QString getNextQuestion(const QString &type);
    QString getCorrectAnswer(const QString &type);
    QStringList getOptions(const QString &type);
    QString getHint(const QString &type);
    void showTranslationExercise();
    void showGrammarExercise();
    void checkTranslationAnswer();
    void checkGrammarAnswer();
    void updateTimer();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void showHelpDialog();
    void showGrammarHelp();
    void exitApplication();
    void showDifficultyDialog();
    void setDifficulty(int index);
    void resetTask();

   private slots:
    void onTimerTimeout();
};

#endif // MAINWINDOW_H
