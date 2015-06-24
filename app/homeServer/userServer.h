#ifndef USERSERVER_H
#define USERSERVER_H

#include "tcpServer.h"

#include "homeServer_global.h"
#include "utils.h"

#include <QObject>
#include <QMap>
#include <QHostAddress>

/**
 * @brief The userServer class handles communication with the users.
 *  It uses a TCP server to get new connections.
 *  It can send and receive data from the modules, but do not store anything.
 */

class QTcpServer;
class QTcpSocket;
class QTimer;
class QFile;

class HOMESERVERSHARED_EXPORT userServer: public tcpServer
{
    Q_OBJECT
public:
    explicit userServer(QObject *parent = Q_NULLPTR);
    ~userServer() {}

private:
    virtual void processLine(QTcpSocket *client, const QString &line);
    virtual bool processCommand(QTcpSocket *client, const QString &command);

};

#endif // USERSERVER_H

