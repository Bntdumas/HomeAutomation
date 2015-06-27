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
    static const QString CMD_CMD = QStringLiteral("CMD");
    static const QString CMD_RESET = QStringLiteral("RST");

    // frame keyword from the modules
    static const QString MODULE_ID = QStringLiteral("chipID");
    static const QString MODULE_GPIOS = QStringLiteral("gpios");
    static const QString MODULE_GPIO = QStringLiteral("gpio");
    static const QString MODULE_SENSOR = QStringLiteral("sensor");
    static const QString MODULE_SENSORS = QStringLiteral("sensors");
    static const QString MODULE_MODULE_DATA = QStringLiteral("module_data");
    static const QString MODULE_OK = QStringLiteral("ok");
    static const QString MODULE_ERROR = QStringLiteral("error");

    // frame string separators
    static const QString SEP_END_LINE = QStringLiteral("\n");
}

moduleServer::moduleServer(QObject *parent) :
    tcpServer(parent)
    ,m_state(false)
{
    qRegisterMetaType<devices::DeviceSubType>("devices::DeviceSubType");
}

bool moduleServer::resetModules()
{
    return sendAll(CMD_RESET);
}

bool moduleServer::resetModule(int moduleID)
{
    Q_EMIT message(tr("moduleServer: sending reset to %1 ").arg(moduleID), utils::Warning);
    return sendCommandToModule(moduleID, CMD_RESET);
}

bool moduleServer::resetModule(QTcpSocket *socket)
{
    return send(socket, CMD_RESET, true);
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

void moduleServer::sendTestGPIOSwitch(int moduleID)
{
    if (clientFromID(moduleID)) {
        setModuleGPIO(moduleID, 4, m_state);
    }
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
    //requestDataFromModules();
    m_state = !m_state;

    sendTestGPIOSwitch(10557940);
    sendTestGPIOSwitch(16670068);
    sendTestGPIOSwitch(16670915);
    sendTestGPIOSwitch(16669492);
}


void moduleServer::idTimerTimeout()
{
    // send reset to module if it didn't send it's ID
    if (m_clientswithoutID.isEmpty()) {
        m_idTimer->stop();
        return;
    }

    QMap<QTcpSocket*, QTime>::iterator i;
    for (i = m_clientswithoutID.begin(); i != m_clientswithoutID.end(); ++i) {
        QTcpSocket *client = i.key();
        if (client->isValid()) {
            qDebug() << "second since connection" << i.value().secsTo(QTime::currentTime());
            if (i.value().secsTo(QTime::currentTime()) >= 5) {
                resetModule(client);
            }
        }
    }



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
            if (xml.isStartElement()) {
                const QString msgType = xml.name().toString();
               // qDebug() << msgType;
                if (msgType == MODULE_ID) {
                    processIDElement(client, xml);
                } else if (msgType == MODULE_GPIO) {
                    processGPIOElement(moduleID, xml);
                } else if (msgType == MODULE_SENSOR) {
                    processSensorElement(moduleID, xml);
                } else if (msgType == MODULE_OK) {
                } else if (msgType == MODULE_ERROR) {
                } else if (msgType == MODULE_GPIOS || msgType == MODULE_SENSORS || msgType == MODULE_MODULE_DATA) {
                    continue;
                } else {
                    Q_EMIT message(tr("moduleServer: XML parsing error: the element %1 was not recognized")
                                   .arg(msgType), utils::Warning);
                }
            }
      }
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
    // <chipID value="123456"/>
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
    m_clientswithoutID.remove(client);
    Q_EMIT message(tr("%1 sent me his ID (%2)").arg(clientIDFromIP(client->peerAddress()).toString()).arg(id), utils::Info);
    Q_EMIT newModuleConnected(id);
}

void moduleServer::processGPIOElement(int moduleID, const QXmlStreamReader &reader)
{
    // <gpio pin=\"0\" value=\"1\"/>
    if (reader.attributes().count() != 2) {
        Q_EMIT message(tr("moduleServer: XML parsing error: the gpio message should contain exactly 2 attributes"), utils::Warning);
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
    Q_EMIT message(tr("%1 sent gpio information: pin %2 = %3").arg(moduleID).arg(pin).arg(state?QStringLiteral("true"):QStringLiteral("false")), utils::Info);
    Q_EMIT gpioChanged(moduleID, pin, state);
}

void moduleServer::processSensorElement(int moduleID, const QXmlStreamReader &reader)
{
    // <sensor pin=\"2\" type=\"temperature\" value=\"22.5\"/>
    //qDebug() << Q_FUNC_INFO;
    if (reader.attributes().count() != 3) {
        Q_EMIT message(tr("moduleServer: XML parsing error: the sensor message should contain exactly 2 attributes"), utils::Warning);
        return;
    }

    //first attribute: module pin
    bool okPin;
    int pin = reader.attributes().first().value().toInt(&okPin);
    if (!okPin) {
        Q_EMIT message(tr("moduleServer: XML parsing error: I could not convert %1 to int")
                       .arg(reader.attributes().first().value().toString()), utils::Warning);
        return;
    }

    //second attribute: sensor type
    const QString subType = reader.attributes().at(1).value().toString();
    // let's try to match it to a compatible DeviceSubType
    devices::DeviceSubType subTypeEnum = devices::enumFromStringSubType(subType);
    if (!devices::subTypeCompatible(devices::Sensor, subTypeEnum)) {
        Q_EMIT message(tr("moduleServer: XML parsing error: I could not retrieve the sensor type from %1").arg(subType), utils::Warning);
        return;
    }

    // third attribute: sensor value
    bool okValue;
    double value = reader.attributes().last().value().toDouble(&okValue);
    if (!okValue) {
        Q_EMIT message(tr("moduleServer: XML parsing error: I could not convert %1 to double")
                       .arg(reader.attributes().last().value().toString()), utils::Warning);
        return;
    }
    Q_EMIT sensorUpdated(moduleID, pin, subTypeEnum, value);
    Q_EMIT message(tr("%1 sent sensor information: %2=%3 (pin %4)").arg(moduleID).arg(subType).arg(value).arg(pin), utils::Info);
}
