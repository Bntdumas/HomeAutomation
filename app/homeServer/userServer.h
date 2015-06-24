#ifndef USERSERVER_H
#define USERSERVER_H

#include "tcpServer.h"

#include "homeServer_global.h"
#include "utils.h"

#include <QObject>
#include <QMap>
#include <QHostAddress>

/**
 * @brief The moduleServer class communicates with the users (via Qt apps).
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
    virtual void processLine(QTcpSocket *client, const QString &line) {/*TODO*/;}
    virtual bool processCommand(QTcpSocket *client, const QString &command) {/*TODO*/;}

};

#endif // USERSERVER_H

