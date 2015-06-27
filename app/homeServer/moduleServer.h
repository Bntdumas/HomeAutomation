#ifndef MODULESERVER_H
#define MODULESERVER_H

#include "homeServer_global.h"
#include "devices.h"
#include "utils.h"
#include "tcpServer.h"

#include <QObject>
#include <QMap>
#include <QHostAddress>
#include <QXmlStreamReader>

/**
 * @brief The moduleServer class handles communication with the wifi enabled modules.
 *  It uses a TCP server to get new connections.
 *  It can send and receive data from the modules, but do not store anything.
 */

class QTcpServer;
class QTcpSocket;
class QTimer;
class QFile;

/**
 * @brief The moduleServer class communicates with the ESP8266 wifi modules.
 */

class HOMESERVERSHARED_EXPORT moduleServer: public tcpServer
{
    Q_OBJECT
public:
    explicit moduleServer(QObject *parent = Q_NULLPTR);
    ~moduleServer() {}

    void setAutomaticPolling(bool state);

    /**
      * These methods send pre-made commands to the module(s)
      */
    bool resetModules();
    bool resetModule(int moduleID);
    bool resetModule(QTcpSocket *socket);

    bool requestDataFromModules();
    bool requestDataFromModule(int moduleID);

    bool requestIDFromModules();
    bool requestIDFromModule(int moduleID);

    bool setModuleGPIO(int moduleID, int gpioPin, bool state);

    void sendTestGPIOSwitch(int moduleID);

Q_SIGNALS:
    void gpioChanged(int moduleID, int gpioPin, bool state);
    void sensorUpdated(int moduleID, int gpioPin, devices::DeviceSubType type, double value);

    void okReceived(int moduleID);
    void errorReceived(int moduleID, const QString &msg);

private Q_SLOTS:
    void pollingTimerTimeout() Q_DECL_OVERRIDE;
    void idTimerTimeout() Q_DECL_OVERRIDE;

private:
    bool processLine(QTcpSocket *client, const QByteArray &line) Q_DECL_OVERRIDE;
    bool sendCommandToModule(int moduleID, const QString &command);

    /**
     * @brief methods to process the xml elements from a received line
     */
    void processIDElement(QTcpSocket *client, const QXmlStreamReader &reader);
    void processGPIOElement(int moduleID, const QXmlStreamReader &reader);
    void processSensorElement(int moduleID, const QXmlStreamReader &reader);

    bool m_state;


};

#endif // MODULESERVER_H

