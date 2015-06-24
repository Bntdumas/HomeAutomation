#ifndef MODULESERVER_H
#define MODULESERVER_H

#include "homeServer_global.h"
#include "utils.h"
#include "tcpServer.h"

#include <QObject>
#include <QMap>
#include <QHostAddress>


/**
 * @brief The moduleServer class handles communication with the wifi enabled modules.
 *  It uses a TCP server to get new connections.
 *  It can send and receive data from the modules, but do not store anything.
 */

class QTcpServer;
class QTcpSocket;
class QTimer;
class QFile;

class HOMESERVERSHARED_EXPORT moduleServer: public tcpServer
{
    Q_OBJECT
public:
    explicit moduleServer(QObject *parent = Q_NULLPTR);
    ~moduleServer() {}

    void resetModules();

private Q_SLOTS:
    void pollingTimerTimeout() Q_DECL_OVERRIDE;

private:
    void processLine(QTcpSocket *client, const QString &line) Q_DECL_OVERRIDE;
    bool processCommand(QTcpSocket *client, const QString &command) Q_DECL_OVERRIDE;
    QString getValue(const QString &command);

    bool m_state;

};

#endif // MODULESERVER_H

