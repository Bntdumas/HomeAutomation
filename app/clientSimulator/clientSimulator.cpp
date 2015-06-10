#include "clientSimulator.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QtMath>

namespace {
static const int TCP_PORT = 5003;
static const QHostAddress::SpecialAddress SERVER_IP = QHostAddress::LocalHost;
}

clientSimulator::clientSimulator(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &clientSimulator::slotReadyRead);
}

clientSimulator::~clientSimulator()
{

}

void clientSimulator::connectToServer()
{
    m_socket->connectToHost(SERVER_IP, TCP_PORT);
}

bool clientSimulator::isConnected()
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void clientSimulator::processCommand(const QString &cmd)
{

}

void clientSimulator::parseMessage(const QString &msg)
{

}

bool clientSimulator::sendID()
{
    return send(QStringLiteral("esp8266\n"));
}

bool clientSimulator::sendOK()
{
    return send(QStringLiteral("OK\n"));
}

bool clientSimulator::sendData(bool random)
{
    float temp = random ? (qrand() % 30 + 250) / 10 : 22.5;
    int gpio1 = random ? qrand() % 2:1;
    int gpio2 = random ? qrand() % 2:0;
    int gpio3 = random ? qrand() % 2:1;
    bool sendFirst = send(QString(QStringLiteral("$TEMP:%1").arg(temp)));
    bool sendSecond = send(QString(QStringLiteral("$GPIO:1,%1;2,%2;3,%3;")).arg(gpio1).arg(gpio2).arg(gpio3));
    return sendFirst && sendSecond;
}

void clientSimulator::slotReadyRead()
{
    qDebug() << Q_FUNC_INFO;
}

bool clientSimulator::send(const QString &msg)
{
    return m_socket->write(msg.toLatin1()) == msg.length();
}
