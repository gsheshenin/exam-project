#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client : public QObject {
    Q_OBJECT

public:
    Client(const QString &host, int port, QObject *parent = nullptr);
    void sendMessage(const QString &name, const QString &description);
private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

signals:
    void messageReceived(const QString &message);

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
