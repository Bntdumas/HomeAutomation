#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "homeServer_global.h"
#include "utils.h"

#include <QObject>
#include <QMap>
#include <QTime>
#include <QHostAddress>


/**
 * @brief The tcpServer is a base class that sets up the base for tcp communication
 * with either the hardware modules or the users
 */

class QTcpServer;
class QTcpSocket;
class QTimer;
class QFile;

class HOMESERVERSHARED_EXPORT tcpServer: public QObject
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = Q_NULLPTR);
    ~tcpServer() {}

public:
    /**
     * @brief (re)create the server
     */
    void createTCPServer();

    /**
      * These methods are mainly used for testing purposes
      */
    bool isListening();
    int receivedLines() {return m_receivedLines;}
    void resetLineCounter() {m_receivedLines = 0;}
    int connectedClients() {return m_clientsList.count();}


    /**
     * @brief convert between clients ID, IP
     */
    QHostAddress clientIPFromID(const QVariant &clientID);
    QVariant clientIDFromIP(const QHostAddress IP);

Q_SIGNALS:
    /**
     * @brief When a new module is connected, emit a signal with the client ID.
     */
    void newModuleConnected(const QVariant &clientID);

    /**
     * @brief When a new module is connected, emit a signal with the client IP (sent before we get it's ID).
     */
    void newClient(const QHostAddress &clientIP);

    /**
     * @brief Debug messages
     */
    void message(const QString &text, const utils::messageType type);

public Q_SLOTS:
    /**
     * @brief Sends a string based message to the specified client
     * @param skipIfAlreadyInQueue: if the message is already scheduled to be sent later on, do not put it in the list again
     */
    bool send(QTcpSocket *client, const QString &msg, bool skipIfAlreadyInQueue = false);
    bool sendAll(const QString &message);


private Q_SLOTS:
    void dataAvailable();
    void newConnection();
    void connectionRemoved();

protected Q_SLOTS:
    virtual void pollingTimerTimeout() {} //default does nothing
    virtual void idTimerTimeout() {}

protected:
    /**
     * @brief Processing method
     */
    virtual bool processLine(QTcpSocket *client, const QByteArray &line) = 0;

    /**
     * @brief remove a message from the waiting list and returns it.
     */
    const QString takeNextMessageForClient(QTcpSocket* client);


    /**
     * @brief The list of the connected modules, the variant corresponds to the client ID.
     */
    QMap<QTcpSocket*, QVariant> m_clientsList;


    /**
     * @brief This timer allows querying clients for data at regular interval
     */
    QTimer *m_pollingTimer;

    /**
     * @brief client without ID and timestamp corresponding to the time they were connected
     */
    QMap<QTcpSocket *, QTime> m_clientswithoutID;

    /**
     * @brief when a new client is connected, it needs to send it's ID within the timer time, otherwise a reset command is sent.
     */
    QTimer *m_idTimer;

    /**
     * @brief convert between clients ID, IP and sockets
     */
    QTcpSocket *clientFromID(const QVariant &clientID);
    QTcpSocket *clientFromIP(const QHostAddress IP);

    /**
     * @brief Cleanup the client form the different lists
     */
    void removeClient(QTcpSocket * client);


private:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;


    /**
     * @brief For each frame sent to a client, a reponse needs to be sent back
     *  this list contains clients that did not answer a message yet
     */
    QList< QTcpSocket* > m_clientWaitingForAnswer;
    
    /** @brief keep a list of the message that needs to be re-sent to a particular socket (module).
      */
    typedef QPair<QTcpSocket*, QString> messageClientPair;
    QList< messageClientPair > m_messageWaitingList;


    /**
     * @brief for testing, the number of completes lines received by the server.
     */
    int m_receivedLines;
};

#endif // TCPSERVER_H

