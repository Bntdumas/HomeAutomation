#ifndef HOMENETWORK_H
#define HOMENETWORK_H

#include "homeServer_global.h"
#include "utils.h"

#include <QObject>
#include <QMap>
#include <QHostAddress>


/**
 * @brief The homeServer class handles communication with the wifi enabled modules.
 *  It uses a TCP server to get new connections.
 *  It can send and receive data from the modules, but do not store anything.
 */

class QTcpServer;
class QTcpSocket;
class QTimer;
class QFile;

class HOMENETWORKSHARED_EXPORT homeServer: public QObject
{
    Q_OBJECT
public:
    explicit homeServer(QObject *parent = Q_NULLPTR);
    ~homeServer() {}

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
    void resetModules();

Q_SIGNALS:
    /**
     * @brief When a new module is connected, emit a signal with it's chip ID.
     */
    void newModuleConnected(const int chipID);

    /**
     * @brief Debug messages
     */
    void message(const QString &text, const utils::messageType type);

public Q_SLOTS:
    /**
     * @brief Sends a string based message to the specified client
     */
    bool send(QTcpSocket *client, const QString &message);
    bool sendAll(const QString &message);

private Q_SLOTS:
    void dataAvailable();
    void newConnection();
    void connectionRemoved();
    void pollingTimerTimeout();

private:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
    
    /**
     * @brief The list of the connected modules, the int corresponds to the chip ID.
     *  the chip ID is sent by the module on connection
     */
    QMap<QTcpSocket*, int> m_clientsList;

    /**
     * @brief For each frame sent to a client, a reponse needs to be sent back
     *  this list contains clients that did not answer a message yet
     */
    QList< QTcpSocket* > m_clientWaitingForAnswer;

    /**
     * @brief remove a message from teh waiting list and returns it.
     */
    const QString takeNextMessageForClient(QTcpSocket* client);
    
    /** @brief keep a list of the message that needs to be re-sent to a particular socket (module).
      */
    typedef QPair<QTcpSocket*, QString> messageClientPair;
    QList< messageClientPair > m_messageWaitingList;

    /**
     * @brief This timer allows querying clients for data at regular interval
     */
    QTimer *m_pollingTimer;

    /**
     * @brief for testing, the number of completes lines received by the server.
     */
    int m_receivedLines;

    /**
     * @brief Processing methods, a line can contain several commands.
     */
    void processLine(QTcpSocket *client, const QString &line);
    bool processCommand(QTcpSocket *client, const QString &command);


    /** @brief  get the value out of a command syntax
    * <STRING><SEP_NAME_VALUE><VALUE>
    */
    QString getValue(const QString &command);
    QTcpSocket *clientForIP(const QHostAddress IP);
    bool m_state;

};

#endif // HOMENETWORK_H

