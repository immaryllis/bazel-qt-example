#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QObject>
#include <QProgressBar>
#include <QString>
#include <QMessageBox>
#include <QStack>
#include <QVBoxLayout>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}  // namespace Ui

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    
private slots:
    void onSpinBoxValueChanged(int value);
    void questionView(QListWidgetItem* item);
    void rename();
    void onStatusChanged(int index);
    void onListItemDoubleClicked(QListWidgetItem* item);
    void NextQuestion();
    void PreviousQuestion();
    void trackProgress();
    
    
private:
    //Ui::MainWindow* ui;
    void updateColor(QListWidgetItem* item);
    QSpinBox* spinBox;
    QListWidget* listWidget;
    QLabel* nameLabel;
    QLabel* numberLabel;
    QLineEdit* nameEdit;
    QComboBox* comboBox;
    QPushButton* nextQuestionButton;
    QPushButton* previousQuestionButton;
    QProgressBar* totalProgress;
    QProgressBar* greenProgress;
    QListWidgetItem* selectedItem = nullptr;
    QMap<QListWidgetItem*, int> ticketStatuses;
    QMap<QListWidgetItem*, int> ticketNumbers;
    QStack<QListWidgetItem*> lastSeen;
    QListWidgetItem* currentQuestion = nullptr;
    int totalCount = 0;
    int seenTickets = 0;
    int greenTickets = 0;
};
#endif  // MAINWINDOW_H
