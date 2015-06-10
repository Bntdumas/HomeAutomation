#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QTcpSocket>

#include "homeNetwork.h"
#include "clientSimulator.h"

class NetworkModuleTest : public QObject
{
    Q_OBJECT

public:
    NetworkModuleTest();

private Q_SLOTS:
    void init();
    void cleanup();

    // using local simulator
    void testSendLotsOfLines();

    //using real esp8266 module connected via wifi
    void testWifiConnection();
    void testCommands();


private:
    homeNetwork *m_server;
    clientSimulator *m_client;

    void initSimulator();
    void waitForRealClientConnected(int numberOfExpectedClients);
};

NetworkModuleTest::NetworkModuleTest()
{
}

void NetworkModuleTest::init()
{
    m_server = new homeNetwork();
    QVERIFY(m_server->isListening());
}

void NetworkModuleTest::cleanup()
{
    delete m_server;
}

// simple test,
void NetworkModuleTest::testSendLotsOfLines()
{
    initSimulator();

    const int messageToSend = 100;
    //have the client send lots of lines and make sure the server got them all
    for (int i = 0; i < messageToSend; ++i) {
       QVERIFY(m_client->sendID());
       QCoreApplication::instance()->processEvents();
    }

    QTimer timer;
    timer.setSingleShot(true);
    timer.start(3000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(m_server->receivedLines(), messageToSend);

    delete m_client;
    m_client = 0;
}

void NetworkModuleTest::testWifiConnection()
{
  //  waitForRealClientConnected(3);
}

void NetworkModuleTest::testCommands()
{
    waitForRealClientConnected(3);
    m_server->resetLineCounter();
    const int messageToSend = 100;
    for (int i = 0; i < messageToSend; i++) {
        m_server->sendAll("DATA");
    }

    QTimer timer;
    timer.setSingleShot(true);
    timer.start(90000);

    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    QCOMPARE(m_server->receivedLines(), messageToSend * 3);
}

void NetworkModuleTest::initSimulator()
{
    QTimer timer;
    timer.setSingleShot(true);
    m_client = new clientSimulator();
    QSignalSpy spyNewData(m_client->socket(), SIGNAL(stateChanged(QAbstractSocket::SocketState)));

    // connect to server, timeout 3 seconds
    m_client->connectToServer();
    timer.start(3000);

    while (!m_client->isConnected() && timer.isActive())
        QCoreApplication::instance()->processEvents();

    //QAbstractSocket::HostLookupState
    //QAbstractSocket::ConnectingState
    //QAbstractSocket::ConnectedState
    QVERIFY(spyNewData.count() == 3);
    QVERIFY(m_client->isConnected());
    QCOMPARE(m_server->connectedClients(), 1);
}

void NetworkModuleTest::waitForRealClientConnected(int numberOfExpectedClients)
{
    QTimer timer;
    timer.setSingleShot(true);
    qDebug() << "######## NOW RESET THE MODULES ########";
    timer.start(3000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();

    // wait for 5seconds, that should give enough time for clients to connect
    timer.start(5000);
    while (timer.isActive())
        QCoreApplication::instance()->processEvents();


    QCOMPARE(m_server->connectedClients(), numberOfExpectedClients);
}

QTEST_MAIN(NetworkModuleTest)

#include "tst_NetworkModuleTest.moc"
