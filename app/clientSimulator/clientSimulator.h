#ifndef CLIENTSIMULATOR_H
#define CLIENTSIMULATOR_H

#include <QObject>
#include <QAbstractSocket>
#include "clientSimulator_global.h"

/**
 * @brief This is a module that simulates the tcp connection with an esp8266 module.
 * it is used to test the tcp server.
 */
class QTcpSocket;

class CLIENTSIMULATORSHARED_EXPORT clientSimulator: public QObject
{
    Q_OBJECT
public:
    explicit clientSimulator(QObject *parent = 0);
    ~clientSimulator();
    void connectToServer();

    bool isConnected();
    QTcpSocket *socket() { return m_socket; }

    /**
     * @brief for testing
     */
    void setCorrupted(bool flag) {m_corrupted = flag;}

    void sendIDOnConnect(bool flag);

public Q_SLOTS:
    bool sendID();
    bool sendOK();
    bool sendError();
    bool sendData(bool random);

private Q_SLOTS:
    void slotReadyRead();

private:
    QTcpSocket *m_socket;
    bool send(const QString &msg);

    bool m_corrupted;

};

#endif // CLIENTSIMULATOR_H
