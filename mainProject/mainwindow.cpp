#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    taskNameInput = new QLineEdit(this);
    taskDescriptionInput = new QLineEdit(this);
    addButton = new QPushButton("Add Task", this);
    executeALLButton = new QPushButton("Execute ALL Tasks", this);
    taskListLayout = new QVBoxLayout();

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(taskNameInput);
    layout->addWidget(taskDescriptionInput);
    layout->addWidget(addButton);
    layout->addWidget(executeALLButton);
    layout->addLayout(taskListLayout);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(executeALLButton, &QPushButton::clicked, this, &MainWindow::executeTasks);

    client = new Client("127.0.0.1", 8080, this);
    connect(client, &Client::messageReceived, this, &MainWindow::onMessageReceived);
}

MainWindow::~MainWindow() {}

void MainWindow::addTask() {
    QString name = taskNameInput->text();
    QString description = taskDescriptionInput->text();
    if (!name.isEmpty()) {
        Task newTask(name.toStdString(), description.toStdString());
        newTask.addObserver(new TaskStatusObserver());
        taskManager.addTask(newTask);
        client->sendMessage(name, description);
        updateTaskList();

    }
}

void MainWindow::executeTask(int index) {
    if (index >= 0 && index < taskManager.getTasks().size()) {
        Task& task = taskManager.getTasks()[index];
        task.execute();
        taskStatusLabels[index]->setText(QString::fromStdString(task.getStatus()));
        QString message = QString("execute_task=%1").arg(QString::fromStdString(task.getName()));
        client->sendMessage(message, "");

    }
}

void MainWindow::executeTasks() {
    taskManager.set_strategy(std::make_unique<SequentialExecution>());
    taskManager.executeTasks();
    updateTaskList();

    QString message = "execute_all_tasks";
    client->sendMessage(message, "");
}

void MainWindow::showDescription(int index) {
    if (index >= 0 && index < taskManager.getTasks().size()) {
        const Task& task = taskManager.getTasks()[index];
        QString description = QString::fromStdString(task.getDescription());

        // Отправляем запрос на получение описания задачи на сервер
        QString message = QString("get_description=%1").arg(QString::fromStdString(task.getName()));
        client->sendMessage(message, "");

        QMessageBox::information(this, "Task Description", description);
    }
}

void MainWindow::updateTaskList() {
    for (size_t i = 0; i < taskNameLabels.size(); ++i) {
        delete taskNameLabels[i];
        delete taskStatusLabels[i];
        delete executeButtons[i];
        delete descriptionButtons[i];
    }

    taskNameLabels.clear();
    taskStatusLabels.clear();
    executeButtons.clear();
    descriptionButtons.clear();

    const auto& tasks = taskManager.getTasks();

    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task& task = tasks[i];

        QHBoxLayout *taskLayout = new QHBoxLayout();

        QLabel *taskNameLabel = new QLabel(QString::fromStdString(task.getName()), this);
        taskNameLabels.push_back(taskNameLabel);

        QLabel *taskStatusLabel = new QLabel(QString::fromStdString(task.getStatus()), this);
        taskStatusLabels.push_back(taskStatusLabel);

        QPushButton *executeButton = new QPushButton("Execute", this);
        executeButtons.push_back(executeButton);
        connect(executeButton, &QPushButton::clicked, this, [this, i]() { executeTask(static_cast<int>(i)); });

        QPushButton *descriptionButton = new QPushButton("Show Description", this);
        descriptionButtons.push_back(descriptionButton);
        connect(descriptionButton, &QPushButton::clicked, this, [this, i]() { showDescription(static_cast<int>(i)); });

        taskLayout->addWidget(taskNameLabel);
        taskLayout->addWidget(taskStatusLabel);
        taskLayout->addWidget(executeButton);
        taskLayout->addWidget(descriptionButton);

        taskListLayout->addLayout(taskLayout);
    }
}

void MainWindow::onMessageReceived(const QString &message) {
    if (message.startsWith("Task Description:")) {
        QString description = message.mid(18);
        QMessageBox::information(this, "Task Description", description);
    } else {
        QMessageBox::information(this, "Server Message", message);
    }
}
