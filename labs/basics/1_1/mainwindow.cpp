#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
//    ui->setupUi(this);

    QWidget* window = new QWidget(this);
    setCentralWidget(window);
    QVBoxLayout* layout = new QVBoxLayout(window);

    spinBox = new QSpinBox(this);
    spinBox->setRange(0, 99);
    spinBox->setValue(0);
    connect(spinBox, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxValueChanged);
    layout->addWidget(spinBox);

    listWidget = new QListWidget(this);
    connect(listWidget, &QListWidget::itemClicked, this, &MainWindow::questionView);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onListItemDoubleClicked);

    numberLabel = new QLabel("Номер билета: ", this);
    layout->addWidget(numberLabel);

    nameLabel = new QLabel("Имя билета: ", this);
    layout->addWidget(nameLabel);

    nameEdit = new QLineEdit(this);
    connect(nameEdit, &QLineEdit::returnPressed, this, &MainWindow::rename);
    layout->addWidget(nameEdit);

    comboBox = new QComboBox(this);
    comboBox->addItems({"Не повторён", "Желательно повторить", "Выучен"});
    connect(comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onStatusChanged);

    nextQuestionButton = new QPushButton("Следующий билет", this);
    connect(nextQuestionButton, &QPushButton::clicked, this, &MainWindow::NextQuestion);
    previousQuestionButton = new QPushButton("Предыдущий билет", this);
    connect(previousQuestionButton, &QPushButton::clicked, this, &MainWindow::PreviousQuestion);

    totalProgress = new QProgressBar(this);
    totalProgress->setRange(0, 100);
    totalProgress->setValue(0);

    greenProgress = new QProgressBar(this);
    greenProgress->setRange(0, 100);
    greenProgress->setValue(0);


    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(nextQuestionButton);
    buttons->addWidget(previousQuestionButton);

    layout->addWidget(listWidget);
    layout->addWidget(comboBox);
    layout->addLayout(buttons);
    layout->addWidget(totalProgress);
    layout->addWidget(greenProgress);
}

MainWindow::~MainWindow() {
    //delete ui;
}


void MainWindow::onListItemDoubleClicked(QListWidgetItem* item) {
    if (item) {
        int currentStatus = ticketStatuses[item];
        int newStatus;
        if (currentStatus == 0) {
            newStatus = 2;
            seenTickets++;
            greenTickets++;
        } else if (currentStatus == 1) {
            newStatus = 2;
            greenTickets++;
        } else {
            newStatus = 1;
            greenTickets--;
        }
        ticketStatuses[item] = newStatus;
        updateColor(item);
        trackProgress();
        comboBox->setCurrentIndex(newStatus);
    }
}


void MainWindow::updateColor(QListWidgetItem* item) {
    if (!item) return;
    int status = ticketStatuses[item];
    switch (status) {
    case 0:
        item->setBackground(Qt::lightGray);
        item->setForeground(Qt::black);
        break;
    case 1:
        item->setBackground(Qt::yellow);
        break;
    case 2:
        item->setBackground(Qt::green);
        break;
    }
}


void MainWindow::onSpinBoxValueChanged(int value) {
    selectedItem = nullptr;
    listWidget->clear();
    ticketStatuses.clear();
    ticketNumbers.clear();
    totalCount = value;
    seenTickets = 0;
    greenTickets = 0;

    for (int i = 1; i <= value; ++i) {
        QListWidgetItem* item = new QListWidgetItem("Билет " + QString::number(i), listWidget);
        ticketStatuses[item] = 0;
        ticketNumbers[item] = i;
        updateColor(item);
    }

    trackProgress();
    selectedItem = nullptr;
    nameLabel->setText("Имя билета: ");
    numberLabel->setText("Номер билета: ");
    nameEdit->clear();
    comboBox->setCurrentIndex(0);
}

void MainWindow::rename() {
    if (selectedItem) {
        QString newName = nameEdit->text();
        if (!newName.isEmpty()) {
            selectedItem->setText(newName);
            nameLabel->setText("Имя билета: " + newName);
        }
        else {
            QMessageBox::information(0, "Ошибка", "Некорректное имя билета.");
        }
    }
}

void MainWindow::onStatusChanged(int index) {
    if(selectedItem){
        int prevStatus = ticketStatuses[selectedItem];
        ticketStatuses[selectedItem] = index;
        updateColor(selectedItem);
        if (prevStatus == 0 && (index == 1 || index == 2)) {
            seenTickets++;
        } else if ((prevStatus == 1 || prevStatus == 2) && index == 0) {
            seenTickets--;
        }

        if (prevStatus != 2 && index == 2) {
            greenTickets++;
        } else if (prevStatus == 2 && index != 2) {
            greenTickets--;
        }
        trackProgress();
    }
}

void MainWindow::questionView(QListWidgetItem* item) {
    if (item && item != selectedItem) {
        if (selectedItem) {
            lastSeen.push(selectedItem);
        }
        selectedItem = item;
        nameLabel->setText("Имя билета: " + item->text());
        nameEdit->setText(item->text());
        int ticketNumber = ticketNumbers[item];
        numberLabel->setText("Номер билета: " + QString::number(ticketNumber));
        comboBox->setCurrentIndex(ticketStatuses[item]);
    }
}


void MainWindow::NextQuestion() {
    int greenValue = greenTickets / totalCount;
    if (greenValue < 1) {
        int randomIndex = QRandomGenerator::global()->bounded(ticketNumbers.size());
        while (ticketStatuses[listWidget->item(randomIndex)] == 2) {
            randomIndex = QRandomGenerator::global()->bounded(ticketNumbers.size());
        }
        selectedItem = listWidget->item(randomIndex);
        listWidget->setCurrentItem(selectedItem);
        nameLabel->setText("Имя билета: " + selectedItem->text());
        numberLabel->setText("Номер билета: " + QString::number(ticketNumbers[selectedItem]));
        comboBox->setCurrentIndex(ticketStatuses[selectedItem]);
    }
    else {
        selectedItem = nullptr;
    }
}

void MainWindow::PreviousQuestion() {
    if (!lastSeen.isEmpty()) {
        QListWidgetItem* prevTicket = lastSeen.pop();
        if (prevTicket) {
            listWidget->setCurrentItem(prevTicket);
            questionView(prevTicket);
        }
    }
}


void MainWindow::trackProgress() {
        int total = 100 * seenTickets / totalCount;
        totalProgress->setValue(total);
        int green = 100 * greenTickets / totalCount;
        greenProgress->setValue(green);
}
