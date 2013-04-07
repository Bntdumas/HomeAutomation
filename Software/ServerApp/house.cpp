#include "house.h"

#include "tcpServer.h"
#include "serverLocalClient.h"
#include "houseData.h"

#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

House::House(QObject *parent) :
    QObject(parent),
    m_houseLights(0),
    m_player(0),
    m_houseSensors(0),
    m_houseData(0),
    m_server(0),
    m_serverLocalClient(0)
{
    init();
}

House::~House()
{
}

void House::init()
{
    m_houseData = new houseData(this);

    m_server = new tcpServer(this);
    m_serverLocalClient = new serverLocalClient(m_houseData, m_server,  this);
    m_serverLocalClient->setIpAndPort(QLatin1String("127.0.0.1"), 50885);
    connect(m_server, SIGNAL(sendInitialData(QTcpSocket*)), m_serverLocalClient, SLOT(sendInitialDataToClient(QTcpSocket*)));

    connect(m_houseData, SIGNAL(requestUpdateLightningData(houseData::PlugFlags)), m_serverLocalClient, SLOT(sendPowerPlugsData(houseData::PlugFlags)));
}
