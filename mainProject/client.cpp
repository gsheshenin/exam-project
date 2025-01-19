#include "client.h"
#include <QDataStream>
#include <QDebug>

Client::Client(const QString &host, int port, QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &Client::onErrorOccurred);
}



void Client::sendMessage(const QString &name, const QString &description) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        //тело запроса
        QString body = QString("name=%1&description=%2").arg(name).arg(description);

        //HTTP-запрос
        QString httpRequest = QString("POST /add_task HTTP/1.1\r\n"
                                      "Host: localhost:8080\r\n"
                                      "Content-Type: application/x-www-form-urlencoded\r\n"
                                      "Content-Length: %1\r\n"
                                      "\r\n"
                                      "%2")
                                  .arg(body.length())
                                  .arg(body);

        socket->write(httpRequest.toUtf8());
        socket->flush();
    }
}

void Client::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        emit messageReceived(line);
    }
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError;
}
