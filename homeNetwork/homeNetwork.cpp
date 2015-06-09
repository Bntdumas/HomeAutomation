#include "homeNetwork.h"

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
static const QString CMD_DATA = "DATA";
static const QString CMD_ID = "ID";
static const QString CMD_OK = "OK";
static const QString CMD_ERR = "ERROR";
static const QString CMD_TEMP = "TEMP";
static const QString CMD_GPIO = "GPIO";
static const QString CMD_RESET = "RST";

// frame string separators
static const QString SEP_COMMAND_START = "$";
static const QString SEP_SUB_COMMAND = "#";
static const QString SEP_NAME_VALUE = ":"; //separator between name of a command and it's value
static const QString SEP_GPIO_PIN_VALUE = ",";
static const QString SEP_MSG_ID = "/";
}

homeNetwork::homeNetwork(QObject *parent) :
    QObject(parent)
  ,m_receivedLines(0)
  ,m_state(false)
{
    
    // create the server
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &homeNetwork::newConnection);
    
    if (m_tcpServer->listen(QHostAddress::Any, TCP_PORT)) {
        qDebug() << "TCP server started on port " << TCP_PORT;
    } else {
        qDebug() << "ERROR: the server could not start on port" << TCP_PORT << ": " << m_tcpServer->errorString();
        qFatal("The server could not start");
    }
    
    m_pollingTimer = new QTimer(this);
    m_pollingTimer->setInterval(500);
    m_pollingTimer->setSingleShot(false);
    connect(m_pollingTimer, &QTimer::timeout, this, &homeNetwork::pollingTimerTimeout);
    m_pollingTimer->start();
}

homeNetwork::~homeNetwork()
{
    
}

bool homeNetwork::isListening()
{
    return m_tcpServer->isListening();
}

void homeNetwork::resetModules()
{
    sendAll(CMD_RESET);
}

void homeNetwork::newConnection()
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
            qDebug() << "Replacing client " << previousClient->peerAddress().toString();
        }

        m_clientsList.insert(newSocket, 0);
        connect(newSocket, &QTcpSocket::disconnected, this, &homeNetwork::connectionRemoved);
        connect(newSocket, &QTcpSocket::readyRead, this, &homeNetwork::dataAvailable);
        
     //   qDebug() << "new client: " << newSocket->peerAddress().toString();
    }
}

void homeNetwork::connectionRemoved()
{
    QTcpSocket *deletedSocket = qobject_cast<QTcpSocket*>(sender());
    if (!deletedSocket) {
        //qWarning("Client left, but couldn't get the socket handle.");
        return;
    }
    
    qDebug() << "Client left: " << deletedSocket->peerAddress().toString();
}

void homeNetwork::pollingTimerTimeout()
{
    sendAll(CMD_DATA);
    m_state = !m_state;
   // sendAll(QString("CMD:GPIO:4:%1").arg(m_state));
    //sendAll(CMD_RESET);
}

const QString homeNetwork::takeNextMessageForClient(QTcpSocket *client)
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

void homeNetwork::processLine(QTcpSocket *client, const QString &line)
{
    // eventually separate the commands into several subcommands
    if (line.contains(SEP_SUB_COMMAND)) {
        Q_FOREACH(const QString &command, line.split(SEP_SUB_COMMAND)) {
            processCommand(client, command);
        }
    } else {
        processCommand(client, line);
    }
}

bool homeNetwork::processCommand(QTcpSocket *client, const QString &command)
{
    QString commandCopy = command;
    if (!commandCopy.startsWith(SEP_COMMAND_START)) {
        qDebug() << "got a command without starting character" << commandCopy;
        return false;
    }
    // remove first character
    commandCopy.remove(0, 1);

    //test keywords
    if (commandCopy.startsWith(CMD_ID)) {
        int id = getValue(command).toInt();
        m_clientsList.insert(client, id);
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << " " << client->peerAddress().toString() << " sent me his ID: " << id;
        Q_EMIT newModuleConnected(id);
        return true;
    } else if ()
    return false;
}

QString homeNetwork::getValue(const QString &command)
{
    if (command.count(SEP_NAME_VALUE) == 1) {
        return command.split(SEP_NAME_VALUE).last();
    }
    return QString();
}

QTcpSocket *homeNetwork::clientForIP(const QHostAddress IP)
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

bool homeNetwork::send(QTcpSocket * client, const QString &message)
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
        const QString packet = QString::number(m_currentFrameID) + message;
        m_clientWaitingForAnswer.append(client);
        int bytesWritten = client->write(packet.toLatin1());
        return bytesWritten == packet.length();
    }
}


void homeNetwork::dataAvailable()
{
  //  qDebug() << Q_FUNC_INFO;
    
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (client == 0) {
        return;
    }
    
    if (client->canReadLine()) {
        QString message(client->readLine(MAX_MESSAGE_LENGTH));
        message.remove('\n');
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << message;
        m_receivedLines++;

        m_clientWaitingForAnswer.removeAll(client);

        //send eventual next message in line
        const QString nextMessage = takeNextMessageForClient(client);
        if (!nextMessage.isEmpty()) { // send next pending message if available
            send(client, nextMessage);
        }

        // process the message
        processLine(client, message);
    }
}


bool homeNetwork::sendAll(const QString &message)
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

