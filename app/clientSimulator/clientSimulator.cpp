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
    qDebug() << Q_FUNC_INFO;
    float temp = random ? ((qrand() % 30 + 250) / 10) : 22.5;
    int gpio1 = random ? (qrand() % 2):1;
    int gpio2 = random ? (qrand() % 2):0;
    int gpio3 = random ? (qrand() % 2):1;
    bool sendFirst = send(QString(QStringLiteral("<sensor temperature=\"%1\"/>").arg(temp)));
    bool sendSecond = send(QString(QStringLiteral("<gpio1 value=\"%1\"/><gpio2 value=\"%2\"/><gpio3 value=\"%3\"/>"))
                           .arg(gpio1).arg(gpio2).arg(gpio3));
    return sendFirst && sendSecond;
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
    else
        sendError();

}

bool clientSimulator::send(const QString &msg)
{
    const QString line = msg + QStringLiteral("\n");
    return m_socket->write(line.toLatin1()) == line.length();
}
