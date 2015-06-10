#include "homeServer.h"

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

    // frame string keywords
    static const QString CMD_DATA = QStringLiteral("DATA");
    static const QString CMD_ID = QStringLiteral("ID");
    static const QString CMD_OK = QStringLiteral("OK");
    static const QString CMD_ERR = QStringLiteral("ERROR");
    static const QString CMD_TEMP = QStringLiteral("TEMP");
    static const QString CMD_GPIO = QStringLiteral("GPIO");
    static const QString CMD_RESET = QStringLiteral("RST");

    // frame string separators
    static const QString SEP_END_LINE = QStringLiteral("\n");
}

homeServer::homeServer(QObject *parent) :
    QObject(parent)
  ,m_state(false)
  ,m_tcpServer(Q_NULLPTR)
{
    m_pollingTimer = new QTimer(this);
    m_pollingTimer->setInterval(500);
    m_pollingTimer->setSingleShot(false);
    connect(m_pollingTimer, &QTimer::timeout, this, &homeServer::pollingTimerTimeout);
}

homeServer::~homeServer()
{
}

void homeServer::createTCPServer()
{
    m_receivedLines = 0;

    // create the server
    if (m_tcpServer) {
        delete m_tcpServer;
    }

    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &homeServer::newConnection);

    if (m_tcpServer->listen(QHostAddress::Any, TCP_PORT)) {
        Q_EMIT message(tr("TCP server started on port %1").arg(TCP_PORT), utils::Success);
    } else {
        Q_EMIT message(tr("the server could not start on port %1 (%2)").arg(TCP_PORT).arg(m_tcpServer->errorString()), utils::NetworkError);
    }

    m_pollingTimer->start();
}

bool homeServer::isListening()
{
    return m_tcpServer->isListening();
}

void homeServer::resetModules()
{
    sendAll(CMD_RESET);
}

void homeServer::newConnection()
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
        connect(newSocket, &QTcpSocket::disconnected, this, &homeServer::connectionRemoved);
        connect(newSocket, &QTcpSocket::readyRead, this, &homeServer::dataAvailable);
        
        Q_EMIT message(tr("New client  %1").arg(newSocket->peerAddress().toString()), utils::Info);
    }
}

void homeServer::connectionRemoved()
{
    QTcpSocket *deletedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!deletedSocket) {
        //qWarning("Client left, but couldn't get the socket handle.");
        return;
    }
        Q_EMIT message(tr("Client left  %1").arg(deletedSocket->peerAddress().toString()), utils::Info);
}

void homeServer::pollingTimerTimeout()
{
    sendAll(CMD_DATA);
    m_state = !m_state;
   // sendAll(QString("CMD:GPIO:4:%1").arg(m_state));
    //sendAll(CMD_RESET);
}

const QString homeServer::takeNextMessageForClient(QTcpSocket *client)
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

void homeServer::processLine(QTcpSocket *client, const QString &line)
{
    // eventually separate the commands into several subcommands
    /*if (line.contains(SEP_SUB_COMMAND)) {
        Q_FOREACH(const QString &command, line.split(SEP_SUB_COMMAND)) {
            processCommand(client, command);
        }
    } else {
        processCommand(client, line);
    }*/
}

bool homeServer::processCommand(QTcpSocket *client, const QString &command)
{
    QString commandCopy = command;
    /*if (!commandCopy.startsWith(SEP_COMMAND_START)) {
        Q_EMIT message(tr("Got a command without starting character:  %1").arg(commandCopy), utils::Warning);
        return false;
    }*/
    // remove first character
    commandCopy.remove(0, 1);

    //test keywords
    if (commandCopy.startsWith(CMD_ID)) {
        int id = getValue(command).toInt();
        m_clientsList.insert(client, id);
        Q_EMIT message(tr("%1 sent me his ID (%2)").arg(client->peerAddress().toString()).arg(id), utils::Info);
        Q_EMIT newModuleConnected(id);
        return true;
    } //else if ()
    return false;
}

QString homeServer::getValue(const QString &command)
{
  /*  if (command.count(SEP_NAME_VALUE) == 1) {
        return command.split(SEP_NAME_VALUE).last();
    }*/
    return QString();
}

QTcpSocket *homeServer::clientForIP(const QHostAddress IP)
{
    QMap<QTcpSocket*, int>::iterator i;
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

bool homeServer::send(QTcpSocket * client, const QString &message)
{
    // if the client didn't answer the last message
    if (m_clientWaitingForAnswer.contains(client)) {
        messageClientPair messageToResend;
        messageToResend.first = client;
        messageToResend.second = message;

        // add the message in the waiting list
        // if it's a "DATA" message, and there's already one in the waiting list, don't add one
        if (message == CMD_DATA) {
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


void homeServer::dataAvailable()
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

bool homeServer::sendAll(const QString &message)
{
    bool ret = true;
    if (m_clientsList.isEmpty()) {
        return false;
    }
    QMap<QTcpSocket*, int>::iterator i;
    for (i = m_clientsList.begin(); i != m_clientsList.end(); ++i) {
        QTcpSocket *client = i.key();
        if (!(client->isValid() && send(client, message))) {
            ret = false;
        }
    }
    return ret;
}

