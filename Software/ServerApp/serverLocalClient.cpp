#include "serverLocalClient.h"
#include "houseData.h"
#include "tcpServer.h"

#include <QTimer>
#include <QTcpSocket>

serverLocalClient::serverLocalClient(houseData *houseDataPtr, tcpServer *server, QObject *parent) :
    tcpClient(houseDataPtr, parent),
    m_serverPointer(server)
{
    m_gotInitialPowerPlugs= true;

    m_broadcastTimer = new QTimer(this);
    connect(m_broadcastTimer, SIGNAL(timeout()), this, SLOT(broadcastInformations()));
    m_broadcastTimer->start(2000);
}

void serverLocalClient::sendInitialDataToClient(QTcpSocket *client)
{
    m_specificReceiver = client;
    sendPowerPlugsData(m_houseData->powerPlugs());
    m_specificReceiver = 0;
}

void serverLocalClient::broadcastInformations()
{
    Q_FOREACH(QTcpSocket *client, m_serverPointer->clients())
    {
        if (client->isValid() && client != m_server) {
            m_specificReceiver = client;
            sendPowerPlugsData(m_houseData->powerPlugs());
            m_specificReceiver = 0;
        }
    }
    m_broadcastTimer->start(2000);
}
