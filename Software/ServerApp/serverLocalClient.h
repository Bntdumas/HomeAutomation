#ifndef SERVERLOCALCLIENT_H
#define SERVERLOCALCLIENT_H
#include "tcpClient.h"

class tcpServer;
/**
 * @brief A slightly modified tcpClient class to run on the same machine as the house server.
 */
class serverLocalClient : public tcpClient
{
    Q_OBJECT
public:
    explicit serverLocalClient(houseData *houseDataPtr, tcpServer *server, QObject *parent = 0);

public Q_SLOTS:
    void sendInitialDataToClient(QTcpSocket *client);
    void broadcastInformations();

private:
    QTimer *m_broadcastTimer;
    tcpServer *m_serverPointer;
};

#endif // SERVERLOCALCLIENT_H
