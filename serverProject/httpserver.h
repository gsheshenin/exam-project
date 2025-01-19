#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <vector>
#include <mutex>
#include "task.h"

class HttpServer : public QTcpServer {
    Q_OBJECT
public:
    HttpServer(QObject *parent = nullptr) : QTcpServer(parent) {
        connect(this, &QTcpServer::newConnection, this, &HttpServer::handleNewConnection);
    }

    void addTask(const Task& task) {
        std::lock_guard<std::mutex> lock(tasksMutex);
        tasks.push_back(task);
    }

    std::vector<Task> getTasks() {
        std::lock_guard<std::mutex> lock(tasksMutex);
        return tasks;
    }

private slots:
    void handleNewConnection() {
        QTcpSocket *socket = nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
            QByteArray request = socket->readAll();
            handleRequest(request, socket);
        });
    }

    void handleRequest(const QByteArray &request, QTcpSocket *socket);

private:
    std::vector<Task> tasks;
    std::mutex tasksMutex;
};

#endif // HTTPSERVER_H
