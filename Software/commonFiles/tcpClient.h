#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include "houseData.h"

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;
class QTimer;

/**
 * @brief Handle TCP communication for the Home Automation System.
 */

class tcpClient : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief tcpClient constructor
     * @param parent
     * @param The Server IP
     * @param The server Port
     */
    explicit tcpClient(houseData *houseDataPtr, QObject *parent = 0);
    ~tcpClient(){}

    /**
     * @brief (re)connect to the server, using saved IP and Port
     */
    void reconnect();

    /**
      * @brief convenience function, the IP/Port is actually stored in HouseData.
      **/
    Q_INVOKABLE void setIpAndPort(const QString& ip, int port);

    QTcpSocket * clientSocket() { return m_server; }

public Q_SLOTS:
    /**
     * @brief send the current house lightning state to the server
     */
    void sendPowerPlugsData(bool screens , bool speakers , bool desk , bool table , bool bed , bool workshop);
    void sendPowerPlugsData(houseData::PlugFlags powerPlugs);

Q_SIGNALS:
    /**
     * @brief socket-related signals
     */
    void signalConnected();
    void signalDisconnected();
    void signalError(QAbstractSocket::SocketError error);   

private Q_SLOTS:
    void sltReadyRead();
    void sltConnected();
    void sltDisconnected();
    void sltError(QAbstractSocket::SocketError sltError);

    /**
     * @brief parse the recieved data
     * @param the recieved message (via TCP)
     * @return boolean according to the parsing state (success/failure)
     */
    bool parseData(const QString &message);

protected:
    /**
     * @brief send a message to the server, except if m_specificReceiver is not null, int his case it is sent to this particular TCPSocket
     * @param message: the string message
     * @return true if sending succeeded
     */
    bool send(const QString &message);

    /**
     * @brief parse a power plugs TCP trame and save the new retrieved values
     * @param message the TCP frame to parse
     * @return true if succeeded
     */
    bool parsePowerPlugs(const QString &message);

    QTcpSocket *m_server;
    quint16 m_msgSize;
    houseData *m_houseData;
    bool m_readyToTransmit;

    int m_initialDataRequestsCount;

    /**
     * @brief these are set to true if at least one frame have been parsed for each
     */
    bool m_gotInitialPowerPlugs;


    QTcpSocket *m_specificReceiver;
};

#endif // TCPCLIENT_H
