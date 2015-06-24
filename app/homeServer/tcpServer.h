#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "homeServer_global.h"
#include "utils.h"

#include <QObject>
#include <QMap>
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
    int resetLineCounter() {m_receivedLines = 0;}
    int connectedClients() {return m_clientsList.count();}

Q_SIGNALS:
    /**
     * @brief When a new module is connected, emit a signal with the client ID.
     */
    void newModuleConnected(const QVariant &clientID);

    /**
     * @brief Debug messages
     */
    void message(const QString &text, const utils::messageType type);

public Q_SLOTS:
    /**
     * @brief Sends a string based message to the specified client
     * @param skipIfAlreadyInQueue: if the message is already scheduled to be sent later on, do not put it in the list again
     */
    bool send(QTcpSocket *client, const QString &message, bool skipIfAlreadyInQueue = false);
    bool sendAll(const QString &message);

private Q_SLOTS:
    void dataAvailable();
    void newConnection();
    void connectionRemoved();

protected Q_SLOTS:
    virtual void pollingTimerTimeout() {} //default does nothing

protected:
    /**
     * @brief Processing methods, a line can contain several commands.
     *  they need to be reimplemented
     */
    virtual void processLine(QTcpSocket *client, const QString &line) = 0;
    virtual bool processCommand(QTcpSocket *client, const QString &command) = 0;

    QTcpSocket *clientForIP(const QHostAddress IP);

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

