#include <QCoreApplication>
#include "httpserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    HttpServer server;
    if (!server.listen(QHostAddress::Any, 8080)) {
        qCritical() << "Unable to start the server:" << server.errorString();
        return 1;
    }
    qDebug() << "Server started on port 8080";
    return a.exec();
}
