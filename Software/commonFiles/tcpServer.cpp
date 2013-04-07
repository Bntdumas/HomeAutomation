#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include "tcpServer.h"

tcpServer::tcpServer(QObject *parent) :
    QObject(parent)
  ,m_msgSize(0)
{
    m_server = new QTcpServer(this);
    if (!m_server->listen(QHostAddress::Any, 50885)) {
        qDebug() << "<server> couldn't start: " <<  m_server->errorString();
     } else  {
        qDebug() << "<server> started on port " << m_server->serverPort() << "add: " << this;
        connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }
    m_msgSize = 0;
}

bool tcpServer::sendAll(const QString &message)
{
    bool ret = true;
    if (m_clients.isEmpty()) {
        return false;
    }
    Q_FOREACH(QTcpSocket *client, m_clients)   {
        if (client->isValid()) {
            send(client, message);
        }
        else {
            ret = false;
        }
    }
    return ret;
}

void tcpServer::newConnection()
{
    QTcpSocket *newClient = m_server->nextPendingConnection();
    qDebug() << "<server> new connection: " << newClient->peerAddress();
    m_clients << newClient;

    connect(newClient, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(newClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    emit sendInitialData(newClient);
}

void tcpServer::dataAvailable()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) {
        return;
    }

    QDataStream in(socket);

    if (m_msgSize == 0) {
        if (socket->bytesAvailable() < 2  /*quint16 size*/) {
            return;
        }
        in >> m_msgSize;
    }

    if (socket->bytesAvailable() < m_msgSize) {
        return;
    }

    QString message;
    in >> message;
    sendAll(message); //broadcast recieved message to update all clients
    emit dataRecieved(message);
    m_msgSize = 0;
}

void tcpServer::clientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) {
        return;
    }
    qDebug() << "<server> " << socket->peerAddress() << "is now disconnected";

    m_clients.removeOne(socket);
    socket->deleteLater();
}

bool tcpServer::send(QTcpSocket * client, const QString &message)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << static_cast<quint16>(0); // save some space to write message size
    out << message;
    out.device()->seek(0);
    out << static_cast<quint16>(packet.size() - 2 /*size of a quint16*/);

    if (packet.size() != client->write(packet)) {
        return false;
    }
    return true;
}
