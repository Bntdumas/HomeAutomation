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
}

void tcpServer::createTCPServer()
{
    m_receivedLines = 0;

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

        // if we already have a client with this IP remove all occurences of it.
        QTcpSocket *previousClient = clientForIP(newSocket->peerAddress());
        if (previousClient != Q_NULLPTR) {
            m_clientsList.remove(previousClient);
            m_clientWaitingForAnswer.removeAll(previousClient);
            for (int i = 0; i < m_messageWaitingList.count(); ++i) {
                messageClientPair item = m_messageWaitingList.at(i);
                if (item.first == previousClient) {
                    m_messageWaitingList.removeAt(i);
                }
            }
            previousClient->deleteLater();
            Q_EMIT message(tr("Replacing client  %1").arg(newSocket->peerAddress().toString()), utils::Info);

        }

        m_clientsList.insert(newSocket, 0);
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
    Q_EMIT message(tr("Client left  %1").arg(deletedSocket->peerAddress().toString()), utils::Info);
}

const QString tcpServer::takeNextMessageForClient(QTcpSocket *client)
{
    //make a working copy of the list
    for (int i = m_messageWaitingList.count()-1; i > 0; --i) {
        messageClientPair item = m_messageWaitingList.at(i);
        if (item.first == client) {
            m_messageWaitingList.removeAt(i);
            return item.second;
        }
    }
    return QString();
}

QTcpSocket *tcpServer::clientForIP(const QHostAddress IP)
{
    QMap<QTcpSocket*, QVariant>::iterator i;
    for (i = m_clientsList.begin(); i != m_clientsList.end(); ++i) {
        QTcpSocket *client = i.key();
        if (client->isValid()) {
            if (client->peerAddress() == IP) {
                return client;
            }
        }
    }
    return Q_NULLPTR;
}

bool tcpServer::send(QTcpSocket * client, const QString &message, bool skipIfAlreadyInQueue)
{
    // if the client didn't answer the last message
    if (m_clientWaitingForAnswer.contains(client)) {
        messageClientPair messageToResend;
        messageToResend.first = client;
        messageToResend.second = message;

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
        int bytesWritten = client->write(message.toLatin1());
        return bytesWritten == message.length();
    }
}

void tcpServer::dataAvailable()
{
    //  qDebug() << Q_FUNC_INFO;
    
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (client == 0) {
        return;
    }
    
    if (client->canReadLine()) {
        QString msg = tr(client->readLine(MAX_MESSAGE_LENGTH).constData());
        msg.remove(SEP_END_LINE);
        Q_EMIT message(tr("Line from %1: %2").arg(client->peerAddress().toString()).arg(msg), utils::Info);

        m_receivedLines++;
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
    QMap<QTcpSocket*, QVariant>::iterator i;
    for (i = m_clientsList.begin(); i != m_clientsList.end(); ++i) {
        QTcpSocket *client = i.key();
        if (!(client->isValid() && send(client, message))) {
            ret = false;
        }
    }
    return ret;
}

