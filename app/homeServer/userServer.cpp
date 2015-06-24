#include "userServer.h"

userServer::userServer(QObject *parent) :
    tcpServer(parent)
{}


void userServer::processLine(QTcpSocket *client, const QString &line)
{

}

bool userServer::processCommand(QTcpSocket *client, const QString &command)
{

}
