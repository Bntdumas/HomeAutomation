#include "tcpServer.h"

#include "utils.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QDebug>
#include <QTime>
#include <QFile>

namespace {
    // server settings
    static const int TCP_PORT = 5003;
    static const int MAX_MESSAGE_LENGTH = 500;
    static const QString SEP_END_LINE = QStringLiteral("\n");
}

tcpServer::tcpServer(QObject *parent) :
    QObject(parent)
    ,m_tcpServer(Q_NULLPTR)
{
    m_pollingTimer = new QTimer(this);
    m_pollingTimer->setInterval(500);
    m_pollingTimer->setSingleShot(false);
    connect(m_pollingTimer, &QTimer::timeout, this, &tcpServer::pollingTimerTimeout);


    m_idTimer = new QTimer(this);
    m_idTimer->setInterval(5000);
    m_idTimer->setSingleShot(false);
    connect(m_idTimer, &QTimer::timeout, this, &tcpServer::idTimerTimeout);
}

void tcpServer::createTCPServer()
{
    m_receivedLines = 0;

    //reset timers
    m_pollingTimer->stop();
    m_idTimer->stop();

    // remove all clients
    QList<QTcpSocket *> clientsToRemove;
    QMap<QTcpSocket *, QVariant>::const_iterator i;
    for (i = m_clientsList.constBegin(); i != m_clientsList.constEnd(); ++i) {
         QTcpSocket *client = i.key();
        if (client && client->isValid()) {
            clientsToRemove.append(client);
        }
    }

    Q_FOREACH(QTcpSocket *client, clientsToRemove) {
        removeClient(client);
    }


    //make sure all lists are empty
    if (!m_clientsList.isEmpty() ||
            !m_clientswithoutID.isEmpty() ||
            !m_messageWaitingList.isEmpty() ||
            !m_clientWaitingForAnswer.isEmpty()) {
        Q_EMIT message(tr("tcpServer: All the lists are not empty."), utils::SoftwareError);
    }

    // create the server
    if (m_tcpServer) {
        delete m_tcpServer;
    }

    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &tcpServer::newConnection);

    if (m_tcpServer->listen(QHostAddress::Any, TCP_PORT)) {
        Q_EMIT message(tr("TCP server started on port %1").arg(TCP_PORT), utils::Success);
    } else {
        Q_EMIT message(tr("the server could not start on port %1 (%2)").arg(TCP_PORT).arg(m_tcpServer->errorString()), utils::NetworkError);
    }
}

bool tcpServer::isListening()
{
    return m_tcpServer->isListening();
}

void tcpServer::newConnection()
{
    QTcpSocket *newSocket =  m_tcpServer->nextPendingConnection();
    if (newSocket) {

        Q_EMIT newClient(newSocket->peerAddress());
        // if we already have a client with this IP remove all occurences of it.
        QTcpSocket *previousClient = clientFromIP(newSocket->peerAddress());
        if (previousClient != Q_NULLPTR) {
            removeClient(previousClient);
        }

        m_clientsList.insert(newSocket, 0);
        m_clientswithoutID.insert(newSocket, QTime::currentTime());
        m_idTimer->start();
        connect(newSocket, &QTcpSocket::disconnected, this, &tcpServer::connectionRemoved);
        connect(newSocket, &QTcpSocket::readyRead, this, &tcpServer::dataAvailable);
        
        Q_EMIT message(tr("New client  %1").arg(newSocket->peerAddress().toString()), utils::Info);
    }
}

void tcpServer::connectionRemoved()
{
    QTcpSocket *deletedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!deletedSocket) {
        return;
    }
    removeClient(deletedSocket);
}

const QString tcpServer::takeNextMessageForClient(QTcpSocket *client)
{
    //make a working copy of the list
    for (int i = m_messageWaitingList.count(); i > 0; --i) {
        const int itemIndex = i-1; // count is 1 based, but index is 0 based
        messageClientPair item = m_messageWaitingList.at(itemIndex);
        if (item.first == client) {
            m_messageWaitingList.removeAt(itemIndex);
            return item.second;
        }
    }
    return QString();
}

QTcpSocket *tcpServer::clientFromID(const QVariant &clientID)
{
    QMap<QTcpSocket*, QVariant>::const_iterator i;
    for (i = m_clientsList.constBegin(); i != m_clientsList.constEnd(); ++i) {
        QTcpSocket *client = i.key();
        if (client && client->isValid()) {
            if (i.value() == clientID) {
                return client;
            }
        }
    }
    return Q_NULLPTR;
}

QHostAddress tcpServer::clientIPFromID(const QVariant &clientID)
{
    QTcpSocket * socket = clientFromID(clientID);
    if (socket) {
        return socket->peerAddress();
    } else {
        return QHostAddress();
    }
}

QVariant tcpServer::clientIDFromIP(const QHostAddress IP)
{
    QMap<QTcpSocket*, QVariant>::const_iterator i;
    for (i = m_clientsList.constBegin(); i != m_clientsList.constEnd(); ++i) {
        QTcpSocket *client = i.key();
        if (client && client->isValid()) {
            if (client->peerAddress() == IP) {
                return i.value();
            }
        }
    }
    return QVariant();
}

QTcpSocket *tcpServer::clientFromIP(const QHostAddress IP)
{
    QMap<QTcpSocket*, QVariant>::const_iterator i;
    for (i = m_clientsList.constBegin(); i != m_clientsList.constEnd(); ++i) {
        QTcpSocket *client = i.key();
        if (client && client->isValid()) {
            if (client->peerAddress() == IP) {
                return client;
            }
        }
    }
    return Q_NULLPTR;
}

void tcpServer::removeClient(QTcpSocket *client)
{
    Q_EMIT message(tr("Removing client  %1").arg(client->peerAddress().toString()), utils::Info);
    m_clientsList.remove(client);
    m_clientWaitingForAnswer.removeAll(client);
    for (int i = 0; i < m_messageWaitingList.count(); ++i) {
        messageClientPair item = m_messageWaitingList.at(i);
        if (item.first == client) {
            m_messageWaitingList.removeAt(i);
        }
    }
    client->deleteLater();
}

bool tcpServer::send(QTcpSocket * client, const QString &msg, bool skipIfAlreadyInQueue)
{
    if (!client) {
        Q_EMIT message(tr("Trying to send a frame to an invalid client"), utils::SoftwareError);
        return false;
    }

    // if the client didn't answer the last message
    if (m_clientWaitingForAnswer.contains(client)) {
        messageClientPair messageToResend;
        messageToResend.first = client;
        messageToResend.second = msg;

        if (skipIfAlreadyInQueue) {
            Q_FOREACH(const messageClientPair &messageInList, m_messageWaitingList) {
                if (messageInList == messageToResend)
                    return false;
            }
        }
        m_messageWaitingList.append(messageToResend);
        return false;
    } else {
        // Prepare frame ID
        m_clientWaitingForAnswer.append(client);
        int bytesWritten = client->write(msg.toLatin1());
        return bytesWritten == msg.length();
    }
}

void tcpServer::dataAvailable()
{   
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (client == 0) {
        return;
    }
    
    if (client->canReadLine()) {
        QByteArray msg = client->readLine(MAX_MESSAGE_LENGTH);
        msg.chop(1); //remove \n
        //Q_EMIT message(tr("Line from %1: %2").arg(client->peerAddress().toString()).arg(QString::fromLatin1(msg.constData())), utils::Info);
        m_receivedLines++;
        //qDebug() << msg << m_receivedLines;

        m_clientWaitingForAnswer.removeAll(client);

        //send eventual next message in line
        const QString nextMessage = takeNextMessageForClient(client);
        if (!nextMessage.isEmpty()) { // send next pending message if available
            send(client, nextMessage);
        }

        // process the message
        processLine(client, msg);
    }
}

bool tcpServer::sendAll(const QString &message)
{
    bool ret = true;
    if (m_clientsList.isEmpty()) {
        return false;
    }
    QMap<QTcpSocket*, QVariant>::const_iterator i;
    for (i = m_clientsList.constBegin(); i != m_clientsList.constEnd(); ++i) {
        QTcpSocket *client = i.key();
        if (client && !(client->isValid() && send(client, message))) {
            ret = false;
        }
    }
    return ret;
}

