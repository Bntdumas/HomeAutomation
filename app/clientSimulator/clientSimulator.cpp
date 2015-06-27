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
    connect(m_socket, &QTcpSocket::connected, this, &clientSimulator::sendID);
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

void clientSimulator::sendIDOnConnect(bool flag)
{
    if (!flag)
        disconnect(m_socket, &QTcpSocket::connected, this, &clientSimulator::sendID);
    else
        connect(m_socket, &QTcpSocket::connected, this, &clientSimulator::sendID);
}

bool clientSimulator::sendID()
{
    return send(QStringLiteral("<chipID value=\"123456\"/>"));
}

bool clientSimulator::sendOK()
{
    return send(QStringLiteral("<ok/>"));
}

bool clientSimulator::sendError()
{
    return send(QStringLiteral("<error/>"));
}

bool clientSimulator::sendData(bool random)
{
    QString gpioValues;
    for (int i = 0; i < 5; ++i) {
        int state = random ? (qrand() % 2):1;
        gpioValues.append(QStringLiteral("<gpio pin=\"%1\" value=\"%2\"/>").arg(i).arg(state));
    }

    QString sensorValues;
    for (int i = 5; i < 7; ++i) {
        gpioValues.append(QStringLiteral("<sensor pin=\"%1\" type=\"Temperature\" value=\"22.5\"/>").arg(i));
    }
    gpioValues.append(QStringLiteral("<sensor pin=\"7\" type=\"Light\" value=\"1\"/>"));
    gpioValues.append(QStringLiteral("<sensor pin=\"8\" type=\"Humidity\" value=\"0\"/>"));

    return send(QStringLiteral("<module_data>%1%2</module_data>").arg(gpioValues).arg(sensorValues));
}

void clientSimulator::slotReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (client == 0) {
        return;
    }

    QString msg = tr(client->readAll().constData());
    msg.remove(QStringLiteral("\n"));

    if(msg == QStringLiteral("DATA"))
        sendData(false);
    else if(msg.startsWith(QStringLiteral("CMD:")))
        sendOK();
    else if(msg == QStringLiteral("RST"))
        sendOK();
    else if(msg == QStringLiteral("ID"))
        sendID();
    else
        sendError();

}

bool clientSimulator::send(const QString &msg)
{
    const QString line = msg + QStringLiteral("\n");
    return m_socket->write(line.toLatin1()) == line.length();
}
