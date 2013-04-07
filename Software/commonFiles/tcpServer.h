#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "tcpClient.h"

class QTcpServer;
class QTcpSocket;
class QTimer;

class tcpServer : public QObject
{
    Q_OBJECT

public:
    explicit tcpServer(QObject *parent = 0);
    ~tcpServer(){}

    QList<QTcpSocket *> clients() {  return m_clients; }

public Q_SLOTS:
    bool sendAll(const QString &message);

private Q_SLOTS:
    void newConnection();
    void dataAvailable();
    void clientDisconnected();

Q_SIGNALS:
    void sendInitialData(QTcpSocket *client);
    void dataRecieved(const QString &message);

private:
    bool send(QTcpSocket * client, const QString &message);

    QTcpServer *m_server;
    QList<QTcpSocket *> m_clients;
    quint16 m_msgSize;
};

#endif // SERVER_H
