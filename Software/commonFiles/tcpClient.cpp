#include "tcpClient.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QStringList>
#include <QTimer>

tcpClient::tcpClient(houseData *houseDataPtr, QObject *parent) :
    QObject(parent),
    m_server(0),
    m_msgSize(0),
    m_houseData(houseDataPtr),
    m_readyToTransmit(false),
    m_initialDataRequestsCount(0),
    m_gotInitialPowerPlugs(false),
    m_specificReceiver(0)
{
}

void tcpClient::reconnect()
{
    if (m_server) {
        delete m_server;
    }
    m_server = new QTcpSocket(this);
    qDebug() << "connecting to " << m_houseData->ip() << " (" << m_houseData->port() << ")" ;
    m_server->connectToHost(m_houseData->ip(), m_houseData->port());

    //SIGNAL -> SLOT
    connect(m_server, SIGNAL(connected()), this, SLOT(sltConnected()));
    connect(m_server, SIGNAL(disconnected()), this, SLOT(sltDisconnected()));
    connect(m_server, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(sltError(QAbstractSocket::SocketError)));
    connect(m_server, SIGNAL(readyRead()), this, SLOT(sltReadyRead()));

    //SIGNAL -> SIGNAL
    connect(m_server, SIGNAL(connected()), this,  SIGNAL(signalConnected()));
    connect(m_server, SIGNAL(disconnected()), this,  SIGNAL(signalDisconnected()));
    connect(m_server, SIGNAL(error(QAbstractSocket::SocketError)), this,  SIGNAL(signalError(QAbstractSocket::SocketError)));
}

void tcpClient::setIpAndPort(const QString& ip, int port)
{
    m_houseData->setIpAndPort(ip, port);
    reconnect();
}

void tcpClient::sltConnected()
{
    qDebug() << "connected!";
}

void tcpClient::sltReadyRead()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if  (senderSocket == 0) {
        return;
    }
    QDataStream in(senderSocket);

    if (m_msgSize == 0) {
        if (senderSocket->bytesAvailable() < 2) {
            return;
        }
        in >> m_msgSize;
    }
    if (senderSocket->bytesAvailable() < m_msgSize) {
        return;
    }

    QString message;
    in >> message;
    if ( senderSocket->peerAddress().toString() != QLatin1String("127.0.0.1")) {
        qDebug() << "Message from " << senderSocket->peerAddress() << ": " << message;
    }
    parseData(message);
    m_msgSize = 0;
    if (senderSocket->bytesAvailable()) {
        sltReadyRead();
    }
}

void tcpClient::sltDisconnected()
{
    qDebug() << m_server->localAddress() << " got disconnected!";
    m_server->abort();
}

void tcpClient::sltError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error:  " << error;
}

bool tcpClient::send(const QString &message)
{
    QTcpSocket *receiver = m_specificReceiver ? m_specificReceiver:m_server;
    if ( receiver->peerAddress().toString() != QLatin1String("127.0.0.1")) {
        qDebug() << "sending " << message << " to " << receiver->peerAddress();
    }
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << static_cast<quint16>(0); // save some space to write message size
    out << message;
    out.device()->seek(0);
    out << static_cast<quint16>(packet.size() - 2 /*size of a quint16*/);

    if (packet.size() != receiver->write(packet)) {
        return false;
    }
    return true;
}

void tcpClient::sendPowerPlugsData(bool screens , bool speakers , bool desk , bool table , bool bed , bool workshop)
{
    houseData::PlugFlags powerPlugs;
    if (screens)  powerPlugs |= houseData::Screen;
    if (speakers)     powerPlugs |= houseData::Speakers ;
    if (desk)    powerPlugs |= houseData::Desk;
    if (table)         powerPlugs |= houseData::Table;
    if (bed)         powerPlugs |= houseData::Bed;
    if (workshop) powerPlugs |= houseData::Workshop;

    sendPowerPlugsData(powerPlugs);
}

void tcpClient::sendPowerPlugsData(houseData::PlugFlags powerPlugs)
{
    QString frame = QString::fromLatin1("@L:%1$").arg(powerPlugs);
    if  (!send(frame)) {
        qDebug() << " failed to send power plugs data: " << powerPlugs << '\n'
                 << " frame: " << frame << '\n';
    }
}

bool tcpClient::parsePowerPlugs(const QString &message)
{
    qDebug() << Q_FUNC_INFO;
    QString strippedData = message;
    strippedData.remove(0, 3);
    strippedData.remove(strippedData.count()-1, 1);
    bool ok = false;
    m_houseData->setPowerPlugs( static_cast<houseData::PlugFlags>(strippedData.toInt(&ok)));
    qDebug() << "Power plugs parsed: " << m_houseData->powerPlugs();
    m_gotInitialPowerPlugs = ok;
    return ok;
}

bool tcpClient::parseData(const QString &message)
{
    if (message.startsWith('@')) {
        const QChar moduleIdentifier = message.at(1);
        switch (moduleIdentifier.toLatin1()) {
        case 'L':
            return parsePowerPlugs(message);
        default:
            return false;
        }
    }
    return false;
}
