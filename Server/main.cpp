#include <QCoreApplication>
#include "TcpServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TcpServer server;
    server.startServer();
    return a.exec();
}
