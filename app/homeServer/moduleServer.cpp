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

    // frame building string keywords (to be sent to the modules)
    static const QString CMD_DATA = QStringLiteral("DATA");
    static const QString CMD_ID = QStringLiteral("ID");
    static const QString CMD_GPIO = QStringLiteral("GPIO");
    static const QString CMD_RESET = QStringLiteral("RST");

    // frame keyword from the modules
    static const QString MODULE_ID = QStringLiteral("chipID");
    static const QString MODULE_GPIO = QStringLiteral("gpio");
    static const QString MODULE_SENSOR = QStringLiteral("sensor");
    static const QString MODULE_OK = QStringLiteral("ok");
    static const QString MODULE_ERROR = QStringLiteral("error");

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
    //GPIO:<pin>,<state>
    const QString command = QString(QStringLiteral("%1:%2:%3,%4")).arg(CMD_CMD).arg(CMD_GPIO).arg(gpioPin).arg(state);
    sendCommandToModule(moduleID, command);
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

bool moduleServer::processLine(QTcpSocket *client, const QByteArray &line)
{
    QXmlStreamReader xml(line);
    bool ok;
    const int moduleID = clientIDFromIP(client->peerAddress()).toInt(&ok);
    if (!ok) {
        Q_EMIT message(tr("moduleServer: client matching errpr: I could not get the module ID as int from %1 ")
                       .arg(clientIDFromIP(client->peerAddress()).toString()), utils::Warning);
        return false;
    }

    while (!xml.atEnd()) {
            xml.readNext();
            if (xml.hasError()) {
                Q_EMIT message(tr("moduleServer: XML parsing error: %1").arg(xml.errorString()), utils::Warning);
                return false;
            }
            if (xml.tokenType() == QXmlStreamReader::StartElement) {
                const QString msgType = xml.name().toString();
                if (msgType == MODULE_ID) {
                    processIDElement(client, xml);
                } else if (msgType == MODULE_GPIO) {
                    processGPIOElement(moduleID, xml);
                } else if (msgType == MODULE_SENSOR) {
                } else if (msgType == MODULE_OK) {
                } else if (msgType == MODULE_ERROR) {

                } else {
                    Q_EMIT message(tr("moduleServer: XML parsing error: the element %1 was not recognized")
                                   .arg(msgType), utils::Warning);
                }
            }
      }
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

void moduleServer::processIDElement(QTcpSocket *client, const QXmlStreamReader &reader)
{
    if (reader.attributes().count() != 1) {
        Q_EMIT message(tr("moduleServer: XML parsing error: the chip ID message should contain only one attribute")
                       .arg(reader.errorString()), utils::Warning);
        return;
    }
    bool ok;
    int id = reader.attributes().first().value().toInt(&ok);
    if (!ok) {
        Q_EMIT message(tr("moduleServer: XML parsing error: I could not convert %1 to int ")
                       .arg(reader.attributes().first().value().toString()), utils::Warning);
        return;
    }
    m_clientsList.insert(client, qVariantFromValue(id));
    Q_EMIT message(tr("%1 sent me his ID (%2)").arg(clientIDFromIP(client->peerAddress()).toString()).arg(id), utils::Info);
    Q_EMIT newModuleConnected(id);
}

void moduleServer::processGPIOElement(int moduleID, const QXmlStreamReader &reader)
{
    if (reader.attributes().count() != 2) {
        Q_EMIT message(tr("moduleServer: XML parsing error: the gpio message should contain only 2 attributes")
                       .arg(reader.errorString()), utils::Warning);
        return;
    }
    bool okPin, okValue;
    int pin = reader.attributes().first().value().toInt(&okPin);
    int value = reader.attributes().last().value().toInt(&okValue);
    if (!okPin || !okValue) {
        Q_EMIT message(tr("moduleServer: XML parsing error: I could not convert %1 or %2 to int ")
                       .arg(reader.attributes().first().value().toString()).arg(reader.attributes().last().value().toString()), utils::Warning);
        return;
    }
    bool state = value != 0;

    Q_EMIT gpioChanged(moduleID, pin, state);
    qDebug() << "gpio changed: " << moduleID << pin << state;
}
