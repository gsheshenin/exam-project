#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include "taskManager.h"
#include "client.h" // Добавляем клиент

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTask();
    void executeTasks();
    void executeTask(int index);
    void showDescription(int index);
    void updateTaskList();
    void onMessageReceived(const QString &message); // Новый слот для обработки сообщений от сервера

private:
    QLineEdit *taskNameInput;
    QLineEdit *taskDescriptionInput;
    QPushButton *addButton;
    QPushButton *executeALLButton;
    QVBoxLayout *taskListLayout;

    QVector<QLabel*> taskNameLabels; // Для хранения указателей
    QVector<QLabel*> taskStatusLabels;
    QVector<QPushButton*> executeButtons;
    QVector<QPushButton*> descriptionButtons;

    TaskManager<Task> taskManager;
    Client *client;
};

#endif // MAINWINDOW_H
