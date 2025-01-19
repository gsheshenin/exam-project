#include "httpserver.h"
#include <QDebug>

void HttpServer::handleRequest(const QByteArray &request, QTcpSocket *socket) {
    QString response;
    QString requestStr = QString::fromUtf8(request).trimmed();

    qDebug() << "Received request:" << requestStr;  // Logging

    if (requestStr.startsWith("GET /show HTTP/1.1")) {
        std::vector<Task> tasks = getTasks();
        QString taskList;
        for (const auto& task : tasks) {
            taskList += QString("Task: %1, Description: %2, Status: %3\n")
                            .arg(QString::fromStdString(task.getName()))
                            .arg(QString::fromStdString(task.getDescription()))
                            .arg(QString::fromStdString(task.getStatus()));
        }
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n" + taskList;
    } else if (requestStr.startsWith("POST /add_task HTTP/1.1")) {

        QString body = requestStr.section("\r\n\r\n", 1);
        QString name = body.section("name=", 1).section("&", 0, 0);
        QString description = body.section("description=", 1);

        if (!name.isEmpty() && !description.isEmpty()) {
            Task newTask(name.toStdString(), description.toStdString());
            addTask(newTask);
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nTask added.";
        } else {
            response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid task format.";
        }
    } else if (requestStr.startsWith("POST /execute_task HTTP/1.1")) {

        QString body = requestStr.section("\r\n\r\n", 1);
        QString name = body.section("name=", 1).section("&", 0, 0);

        if (!name.isEmpty()) {

            std::lock_guard<std::mutex> lock(tasksMutex);
            for (auto& task : tasks) {
                if (task.getName() == name.toStdString()) {
                    task.execute();
                    break;
                }
            }
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nTask executed.";
        } else {
            response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nInvalid task format.";
        }
    } else {
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nUnknown command.";
    }

    qDebug() << "Sending response:" << response;

    socket->write(response.toUtf8());
    socket->flush();
    socket->disconnectFromHost();
}

#include "httpserver.moc"
