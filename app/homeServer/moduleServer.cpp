#include "moduleServer.h"

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
    static const QString CMD_GPIO = QStringLiteral("GPIO");
    static const QString CMD_RESET = QStringLiteral("RST");

    // frame string separators
    static const QString SEP_END_LINE = QStringLiteral("\n");
}

moduleServer::moduleServer(QObject *parent) :
    tcpServer(parent)
    ,m_state(false)
{}

bool moduleServer::resetModules()
{
    return sendAll(CMD_RESET);
}

bool moduleServer::resetModule(int moduleID)
{
    return sendCommandToModule(moduleID, CMD_RESET);
}

bool moduleServer::requestDataFromModules()
{
    return sendAll(CMD_DATA);
}

bool moduleServer::requestDataFromModule(int moduleID)
{
   return sendCommandToModule(moduleID, CMD_DATA);
}

bool moduleServer::requestIDFromModules()
{
    return sendAll(CMD_ID);
}

bool moduleServer::requestIDFromModule(int moduleID)
{
    return sendCommandToModule(moduleID, CMD_ID);
}

bool moduleServer::setModuleGPIO(int moduleID, int gpioPin, bool state)
{

}

void moduleServer::setAutomaticPolling(bool state)
{
    if (state) {
        m_pollingTimer->start();
    } else {
        m_pollingTimer->stop();
    }
}

void moduleServer::pollingTimerTimeout()
{
    sendAll(CMD_DATA);
    m_state = !m_state;
}

void moduleServer::processLine(QTcpSocket *client, const QString &line)
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

bool moduleServer::processCommand(QTcpSocket *client, const QString &command)
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
        m_clientsList.insert(client, qVariantFromValue(id));
        Q_EMIT message(tr("%1 sent me his ID (%2)").arg(client->peerAddress().toString()).arg(id), utils::Info);
        Q_EMIT newModuleConnected(id);
        return true;
    } //else if ()
    return false;
}

QString moduleServer::getValue(const QString &command)
{
    /*  if (command.count(SEP_NAME_VALUE) == 1) {
        return command.split(SEP_NAME_VALUE).last();
    }*/
    return QString();
}

bool moduleServer::sendCommandToModule(int moduleID, const QString &command)
{
    QTcpSocket *client = clientFromID(moduleID);
    if (!client) {
        Q_EMIT message(tr("The client with ID %1 could not be found in the client list.").arg(moduleID), utils::SoftwareError);
        return false;
    }
    return send(client, command, true);
}
